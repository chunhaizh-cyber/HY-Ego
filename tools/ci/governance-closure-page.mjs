#!/usr/bin/env node
/**
 * GB-3 R2：真实网页 DOM 驱动（Playwright Chromium）。
 * - 先有界等待「自我当前交互者」正式可读证据（不把页面 service-object 编码当作正式读回）
 * - 再单独提交正句；不投乱码负例
 * - 快照按阶段命名（preflight/probe），结论按 messageId 重建，不以文件名前缀当消息归属
 * - 只观测取证，不注入需求节点、不改业务分支
 *
 * 环境变量：
 *   SESSION_URL            正式 session URL
 *   EVIDENCE_DIR           证据输出目录
 *   PROBE_NL               正向自然语言（必填）
 *   OBSERVE_MS             正句提交后观察窗（默认 900000）
 *   INTERACTANT_WAIT_MS    提交前交互者前置等待（默认 600000）
 *   CASE_ID                用例标识
 *   ROUND_ID               轮次标识（写入结果）
 */
import { chromium } from 'playwright';
import fs from 'node:fs';
import path from 'node:path';

function req(name) {
  const v = process.env[name];
  if (!v) throw new Error(`缺少环境变量 ${name}`);
  return v;
}

function ensureDir(p) {
  fs.mkdirSync(p, { recursive: true });
  return p;
}

function stamp() {
  return new Date().toISOString();
}

function writeJson(file, obj) {
  fs.writeFileSync(file, JSON.stringify(obj, null, 2), 'utf8');
}

function pageWait(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

async function readMessages(page) {
  return page.evaluate(() => {
    const nodes = [...document.querySelectorAll('#messages .message')];
    return nodes.map((n) => ({
      role: n.className.replace(/^message\s*/, '').trim(),
      text: n.textContent ?? '',
    }));
  });
}

async function readPageMeta(page) {
  return page.evaluate(() => ({
    status: document.getElementById('status')?.textContent ?? '',
    controlStatus: document.getElementById('control-status')?.textContent ?? '',
    serviceObject: document.getElementById('service-object')?.value ?? '',
    title: document.title,
    url: location.href,
  }));
}

async function fetchControl(base) {
  const r = await fetch(base + '/api/control', { cache: 'no-store' });
  const status = r.status;
  let body = null;
  try {
    body = await r.json();
  } catch {
    body = { parseError: true };
  }
  return { httpStatus: status, body };
}

function extractSnapshot(control) {
  const snap = control.body?.snapshot ?? null;
  if (!snap) {
    return {
      overview: null,
      receivedDemandCognition: null,
      demands: null,
      tasks: null,
      learning: null,
      methodCatalog: null,
      missingMethods: null,
      timeline: null,
      lastSelfError: null,
    };
  }
  return {
    overview: snap.overview ?? null,
    receivedDemandCognition: snap.receivedDemandCognition ?? null,
    demands: snap.demands ?? null,
    tasks: snap.tasks ?? null,
    learning: snap.learning ?? null,
    methodCatalog: snap.methodCatalog ?? null,
    missingMethods: snap.missingMethods ?? null,
    timeline: snap.timeline ?? null,
    lastSelfError: snap.learning?.lastSelfError ?? null,
  };
}

/**
 * 只读启发式：控制面板未直接暴露「自我.交互者」特征值。
 * 正式可读证据优先级（均不把页面 service-object 编码算作正式读回）：
 * 1) 认知观察 speaker 非空（通常需已有接收消息推进）
 * 2) 页面出现非 user 的自我侧出站消息（建立连接招呼暗示已有交互者）
 * 3) 自我执行域已完成任务且目标特征可读（候选内部「有交互者」链；记为候选，不当作姓名请求）
 */
function assessInteractant(messages, extracted) {
  const cognition = extracted.receivedDemandCognition || {};
  const speaker =
    cognition.speaker === null || cognition.speaker === undefined
      ? null
      : String(cognition.speaker);
  const nonUser = (messages || []).filter((m) => {
    const role = String(m.role || '');
    return role && !/\buser\b/i.test(role) && !/交互者/.test(role);
  });
  const tasks = Array.isArray(extracted.tasks?.items) ? extracted.tasks.items : [];
  const demands = Array.isArray(extracted.demands?.items) ? extracted.demands.items : [];
  const completedSelfTasks = tasks.filter(
    (t) =>
      t &&
      (t.state === '已完成' || t.state === '已结算' || t.state === '未执行但目标已满足')
  );
  const selfDemands = demands.filter((d) => d && d.executionDomain === '自我执行');
  const candidateInternal = {
    completedSelfTaskCount: completedSelfTasks.length,
    selfDemandCount: selfDemands.length,
    sampleCompletedTaskIds: completedSelfTasks.slice(0, 5).map((t) => t.id),
    sampleSelfDemandIds: selfDemands.slice(0, 5).map((d) => d.id),
  };
  const reasons = [];
  let ready = false;
  let strength = 'none';
  if (speaker) {
    ready = true;
    strength = 'cognition-speaker';
    reasons.push(`receivedDemandCognition.speaker=${speaker}`);
  } else if (nonUser.length > 0) {
    ready = true;
    strength = 'self-outbound-message';
    reasons.push(`页面非user消息数=${nonUser.length}（建立连接招呼等，暗示交互者已取得）`);
  } else if (completedSelfTasks.length > 0 && selfDemands.length > 0) {
    // 弱证据：可能是获取交互者内部链，也可能是其它自我任务——标记为 weak，不算强正式读回身份
    ready = false;
    strength = 'weak-self-dt-only';
    reasons.push(
      '仅见自我域 D/T 完成，控制面板未给出交互者身份编码；不把弱证据升格为正式读回身份'
    );
  } else {
    reasons.push('未见到 cognition.speaker、自我出站消息或可核验交互者身份');
  }
  return {
    ready,
    strength,
    speakerId: speaker,
    pageServiceObjectNotUsedAsProof: true,
    nonUserMessageCount: nonUser.length,
    nonUserMessagesSample: nonUser.slice(0, 3),
    candidateInternal,
    reasons,
    cognitionStage: cognition.stage ?? null,
    cognitionReason: cognition.reason ?? null,
    cognitionMessage: cognition.message ?? null,
  };
}

async function sendViaDom(page, text) {
  const input = page.locator('#input');
  await input.fill(text);
  const submit = page.locator('#form button[type="submit"]');
  const responsePromise = page.waitForResponse(
    (r) =>
      r.request().method() === 'POST' &&
      r.url().includes('/api/messages') &&
      r.status() === 202,
    { timeout: 20000 }
  );
  await submit.click();
  const response = await responsePromise;
  await expectEmptyInput(input, 15000);
  return { status: response.status(), url: response.url() };
}

async function expectEmptyInput(input, timeoutMs) {
  const deadline = Date.now() + timeoutMs;
  while (Date.now() < deadline) {
    const value = await input.inputValue();
    if (value === '') return;
    await pageWait(200);
  }
  throw new Error('提交后 #input 未在时限内清空');
}

async function captureTick(page, base, evidenceDir, timeline, phase, tick, started) {
  const messages = await readMessages(page);
  const meta = await readPageMeta(page);
  const control = await fetchControl(base);
  const extracted = extractSnapshot(control);
  const interactant = assessInteractant(messages, extracted);
  const snap = {
    at: stamp(),
    phase,
    tick,
    elapsedMs: Date.now() - started,
    meta,
    messages,
    controlHttp: control.httpStatus,
    controlStatus: control.body?.status ?? null,
    controlSequence: control.body?.sequence ?? null,
    controlAgeMs: control.body?.ageMs ?? null,
    interactantAssessment: interactant,
    ...extracted,
  };
  const file = path.join(
    evidenceDir,
    `observe-${phase}-${String(tick).padStart(3, '0')}.json`
  );
  writeJson(file, snap);
  timeline.push({
    at: snap.at,
    event: `observe:${phase}`,
    tick,
    messageCount: messages.length,
    demandItems: Array.isArray(snap.demands?.items) ? snap.demands.items.length : null,
    taskItems: Array.isArray(snap.tasks?.items) ? snap.tasks.items.length : null,
    controlStatus: snap.controlStatus,
    cognitionMessage: snap.receivedDemandCognition?.message ?? null,
    cognitionStage: snap.receivedDemandCognition?.stage ?? null,
    cognitionSpeaker: snap.receivedDemandCognition?.speaker ?? null,
    interactantReady: interactant.ready,
    interactantStrength: interactant.strength,
  });
  if (tick === 1 || tick % 5 === 0) {
    await page.screenshot({
      path: path.join(evidenceDir, `screen-${phase}-${String(tick).padStart(3, '0')}.png`),
      fullPage: true,
    });
  }
  return snap;
}

async function observePhase(page, base, evidenceDir, timeline, phase, observeMs) {
  const started = Date.now();
  let tick = 0;
  let last = null;
  while (Date.now() - started < observeMs) {
    tick += 1;
    last = await captureTick(page, base, evidenceDir, timeline, phase, tick, started);
    await page.waitForTimeout(5000);
  }
  return { ticks: tick, last };
}

function rebuildByMessageId(evidenceDir) {
  const files = fs
    .readdirSync(evidenceDir)
    .filter((f) => /^observe-.*\.json$/.test(f))
    .sort();
  const byMessage = {};
  const transitions = [];
  let prevKey = null;
  for (const f of files) {
    const snap = JSON.parse(fs.readFileSync(path.join(evidenceDir, f), 'utf8'));
    const c = snap.receivedDemandCognition || {};
    const mid =
      c.message === null || c.message === undefined ? null : String(c.message);
    const row = {
      file: f,
      at: snap.at,
      phase: snap.phase,
      tick: snap.tick,
      message: mid,
      stage: c.stage ?? null,
      reason: c.reason ?? null,
      speaker: c.speaker ?? null,
      demand: c.demand ?? null,
      meaning: c.meaning ?? null,
      scene: c.scene ?? null,
      handoffPending: c.handoffPending ?? null,
      interactantAssessment: snap.interactantAssessment ?? null,
      pageMessageCount: Array.isArray(snap.messages) ? snap.messages.length : null,
      lastSelfError: snap.lastSelfError ?? null,
    };
    const key = JSON.stringify([
      row.message,
      row.stage,
      row.speaker,
      row.demand,
      row.reason,
    ]);
    if (key !== prevKey) {
      transitions.push(row);
      prevKey = key;
    }
    if (!mid) continue;
    if (!byMessage[mid]) byMessage[mid] = { messageId: mid, frames: [], transitions: [] };
    byMessage[mid].frames.push(row);
    const lastT = byMessage[mid].transitions[byMessage[mid].transitions.length - 1];
    const tKey = JSON.stringify([row.stage, row.speaker, row.demand, row.reason]);
    const lastKey = lastT
      ? JSON.stringify([lastT.stage, lastT.speaker, lastT.demand, lastT.reason])
      : null;
    if (tKey !== lastKey) byMessage[mid].transitions.push(row);
  }
  for (const mid of Object.keys(byMessage)) {
    const frames = byMessage[mid].frames;
    byMessage[mid].first = frames[0];
    byMessage[mid].last = frames[frames.length - 1];
    byMessage[mid].frameCount = frames.length;
  }
  return { transitions, byMessage, observeFileCount: files.length };
}

async function main() {
  const sessionUrl = req('SESSION_URL').replace(/\/$/, '');
  const evidenceDir = ensureDir(req('EVIDENCE_DIR'));
  const probeNl = req('PROBE_NL');
  const observeMs = Number(process.env.OBSERVE_MS || '900000');
  const interactantWaitMs = Number(process.env.INTERACTANT_WAIT_MS || '600000');
  const caseId = process.env.CASE_ID || 'GB3-E2E-01-R2';
  const roundId = process.env.ROUND_ID || 'gb3-r2';
  const base = sessionUrl;

  const timeline = [];
  const result = {
    caseId,
    roundId,
    sessionUrl,
    startedAt: stamp(),
    probeNl,
    negativeNl: null,
    observeMs,
    interactantWaitMs,
    driver: 'playwright-chromium-dom-r2',
    notes: [
      'R2：先有界等待交互者正式证据，再只提交正句；不投乱码。',
      '页面 #service-object 编码不得升格为自我当前交互者正式读回。',
      'HTTP 202/页面回显不等于需求理解或治理闭环。',
      '快照文件名 phase 只表示采集阶段，消息归属以 receivedDemandCognition.message 为准。',
    ],
  };

  const browser = await chromium.launch({
    headless: true,
    args: ['--disable-gpu', '--no-sandbox'],
  });
  const context = await browser.newContext({
    viewport: { width: 1400, height: 900 },
    bypassCSP: true,
  });
  const page = await context.newPage();
  page.setDefaultTimeout(30000);

  try {
    timeline.push({ at: stamp(), event: 'goto', url: sessionUrl });
    await page.goto(sessionUrl, { waitUntil: 'domcontentloaded' });
    await page.waitForSelector('#input');
    await page.waitForSelector('#form button[type="submit"]');
    await page.screenshot({ path: path.join(evidenceDir, 'screen-01-loaded.png'), fullPage: true });
    const meta0 = await readPageMeta(page);
    writeJson(path.join(evidenceDir, 'page-meta-loaded.json'), {
      ...meta0,
      note: 'serviceObject 仅会话绑定展示，不得当作自我.交互者正式读回',
    });
    timeline.push({ at: stamp(), event: 'loaded', meta: meta0 });

    // —— 前置：无 NL 等待交互者 ——
    timeline.push({
      at: stamp(),
      event: 'preflight-interactant-start',
      waitMs: interactantWaitMs,
    });
    const preflightStarted = Date.now();
    let preTick = 0;
    let gate = {
      ready: false,
      strength: 'none',
      timedOut: false,
      decidedAt: null,
      assessment: null,
    };
    let lastPre = null;
    while (Date.now() - preflightStarted < interactantWaitMs) {
      preTick += 1;
      lastPre = await captureTick(
        page,
        base,
        evidenceDir,
        timeline,
        'preflight',
        preTick,
        preflightStarted
      );
      if (lastPre.interactantAssessment?.ready) {
        gate = {
          ready: true,
          strength: lastPre.interactantAssessment.strength,
          timedOut: false,
          decidedAt: stamp(),
          assessment: lastPre.interactantAssessment,
          decidedTick: preTick,
          decidedFile: `observe-preflight-${String(preTick).padStart(3, '0')}.json`,
        };
        break;
      }
      await page.waitForTimeout(5000);
    }
    if (!gate.ready) {
      gate = {
        ready: false,
        strength: lastPre?.interactantAssessment?.strength || 'none',
        timedOut: true,
        decidedAt: stamp(),
        assessment: lastPre?.interactantAssessment || null,
        decidedTick: preTick,
        decidedFile: preTick
          ? `observe-preflight-${String(preTick).padStart(3, '0')}.json`
          : null,
        note: '有界窗口内未取得可升格的正式交互者读回；不假装已满足。仍继续只投正句以区分三类停点，但前置条件标记未达成。',
      };
    }
    writeJson(path.join(evidenceDir, 'interactant-gate.json'), gate);
    timeline.push({
      at: stamp(),
      event: 'preflight-interactant-decided',
      ready: gate.ready,
      strength: gate.strength,
      timedOut: gate.timedOut,
    });
    await page.screenshot({
      path: path.join(evidenceDir, 'screen-02-after-interactant-gate.png'),
      fullPage: true,
    });
    result.interactantGate = gate;

    // —— 只投正句 ——
    timeline.push({ at: stamp(), event: 'send-probe', text: probeNl });
    const probePost = await sendViaDom(page, probeNl);
    timeline.push({ at: stamp(), event: 'probe-post-accepted', ...probePost });
    await page.screenshot({
      path: path.join(evidenceDir, 'screen-03-after-probe-send.png'),
      fullPage: true,
    });
    const messagesAfter = await readMessages(page);
    writeJson(path.join(evidenceDir, 'messages-after-probe.json'), messagesAfter);
    // 尽量尽快抓到入站 messageId（短轮询）
    let probeMessageId = null;
    for (let i = 0; i < 24; i += 1) {
      const early = await captureTick(
        page,
        base,
        evidenceDir,
        timeline,
        'probe-early',
        i + 1,
        Date.now()
      );
      const mid = early.receivedDemandCognition?.message;
      if (mid !== null && mid !== undefined) {
        probeMessageId = String(mid);
        writeJson(path.join(evidenceDir, 'probe-message-id.json'), {
          messageId: probeMessageId,
          at: early.at,
          file: `observe-probe-early-${String(i + 1).padStart(3, '0')}.json`,
          stage: early.receivedDemandCognition?.stage ?? null,
          speaker: early.receivedDemandCognition?.speaker ?? null,
          reason: early.receivedDemandCognition?.reason ?? null,
        });
        break;
      }
      await pageWait(1000);
    }
    result.probeMessageId = probeMessageId;

    const probeObs = await observePhase(
      page,
      base,
      evidenceDir,
      timeline,
      'probe',
      observeMs
    );
    result.probeObserve = { ticks: probeObs.ticks };

    // —— 关闭 ——
    timeline.push({ at: stamp(), event: 'click-close' });
    await page.click('#close');
    await page.waitForTimeout(1500);
    await page.screenshot({
      path: path.join(evidenceDir, 'screen-04-after-close.png'),
      fullPage: true,
    });
    result.closeClicked = true;

    const rebuilt = rebuildByMessageId(evidenceDir);
    writeJson(path.join(evidenceDir, 'message-rebuild.json'), rebuilt);
    result.messageRebuildSummary = {
      observeFileCount: rebuilt.observeFileCount,
      messageIds: Object.keys(rebuilt.byMessage),
      transitionCount: rebuilt.transitions.length,
    };
  } catch (err) {
    result.error = String(err && err.stack ? err.stack : err);
    try {
      await page.screenshot({ path: path.join(evidenceDir, 'screen-error.png'), fullPage: true });
    } catch {
      /* ignore */
    }
    throw err;
  } finally {
    result.finishedAt = stamp();
    result.timeline = timeline;
    writeJson(path.join(evidenceDir, 'page-driver-result.json'), result);
    writeJson(path.join(evidenceDir, 'timeline.json'), timeline);
    await browser.close();
  }
}

main().catch((err) => {
  console.error(err);
  process.exit(1);
});
