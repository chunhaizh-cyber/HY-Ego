#!/usr/bin/env node
/**
 * GB-3：真实网页 DOM 驱动（Playwright Chromium）。
 * 只做页面交互与观测取证，不注入需求节点、不绕过理解链路。
 *
 * 环境变量：
 *   SESSION_URL   正式 session URL（含 /session/<token>）
 *   EVIDENCE_DIR  证据输出目录
 *   PROBE_NL      正向自然语言输入（必填）
 *   NEGATIVE_NL   可选负例输入
 *   OBSERVE_MS    发送后观察窗口（默认 360000）
 *   CASE_ID       用例标识写入时间线
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

async function sendViaDom(page, text) {
  const input = page.locator('#input');
  await input.fill(text);
  // 正式页面 CSP 为 script-src 'unsafe-inline'（无 unsafe-eval）。
  // 禁止使用 page.waitForFunction/字符串 eval；改用 locator + 响应等待。
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
  // 输入框清空表示页面提交处理器已跑完（仍不证明业务理解）
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

function pageWait(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

async function observe(page, base, evidenceDir, timeline, label, observeMs) {
  const started = Date.now();
  let tick = 0;
  while (Date.now() - started < observeMs) {
    tick += 1;
    const messages = await readMessages(page);
    const meta = await readPageMeta(page);
    const control = await fetchControl(base);
    const snap = {
      at: stamp(),
      label,
      tick,
      elapsedMs: Date.now() - started,
      meta,
      messages,
      controlHttp: control.httpStatus,
      controlStatus: control.body?.status ?? null,
      controlSequence: control.body?.sequence ?? null,
      controlAgeMs: control.body?.ageMs ?? null,
      overview: control.body?.snapshot?.overview ?? null,
      receivedDemandCognition: control.body?.snapshot?.receivedDemandCognition ?? null,
      demands: control.body?.snapshot?.demands ?? null,
      tasks: control.body?.snapshot?.tasks ?? null,
      learning: control.body?.snapshot?.learning ?? null,
    };
    writeJson(path.join(evidenceDir, `observe-${label}-${String(tick).padStart(3, '0')}.json`), snap);
    timeline.push({
      at: snap.at,
      event: `observe:${label}`,
      tick,
      messageCount: messages.length,
      demandItems: Array.isArray(snap.demands?.items) ? snap.demands.items.length : null,
      taskItems: Array.isArray(snap.tasks?.items) ? snap.tasks.items.length : null,
      controlStatus: snap.controlStatus,
    });
    if (tick === 1 || tick % 5 === 0) {
      await page.screenshot({
        path: path.join(evidenceDir, `screen-${label}-${String(tick).padStart(3, '0')}.png`),
        fullPage: true,
      });
    }
    await page.waitForTimeout(5000);
  }
}

async function main() {
  const sessionUrl = req('SESSION_URL').replace(/\/$/, '');
  const evidenceDir = ensureDir(req('EVIDENCE_DIR'));
  const probeNl = req('PROBE_NL');
  const negativeNl = process.env.NEGATIVE_NL || '';
  const observeMs = Number(process.env.OBSERVE_MS || '360000');
  const caseId = process.env.CASE_ID || 'GB3-E2E-01';
  const base = sessionUrl; // /api/* 挂在 session 基础路径下

  const timeline = [];
  const result = {
    caseId,
    sessionUrl,
    startedAt: stamp(),
    probeNl,
    negativeNl: negativeNl || null,
    observeMs,
    driver: 'playwright-chromium-dom',
    notes: [
      '本驱动只通过 #input + form submit 模拟交互者；不直接 POST 冒充网页。',
      'HTTP 202/页面回显不等于需求理解或治理闭环。',
    ],
  };

  const browser = await chromium.launch({
    headless: true,
    args: ['--disable-gpu', '--no-sandbox'],
  });
  // bypassCSP：仅解除观测驱动对 Playwright 注入的限制；不改产品 CSP/业务语义。
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
    writeJson(path.join(evidenceDir, 'page-meta-loaded.json'), meta0);
    timeline.push({ at: stamp(), event: 'loaded', meta: meta0 });

    // 正向用例
    timeline.push({ at: stamp(), event: 'send-positive', text: probeNl });
    const positivePost = await sendViaDom(page, probeNl);
    timeline.push({ at: stamp(), event: 'positive-post-accepted', ...positivePost });
    await page.screenshot({ path: path.join(evidenceDir, 'screen-02-after-positive-send.png'), fullPage: true });
    writeJson(path.join(evidenceDir, 'messages-after-positive.json'), await readMessages(page));
    await observe(page, base, evidenceDir, timeline, 'positive', observeMs);

    // 负例（可选，较短观察）
    if (negativeNl) {
      timeline.push({ at: stamp(), event: 'send-negative', text: negativeNl });
      const negativePost = await sendViaDom(page, negativeNl);
      timeline.push({ at: stamp(), event: 'negative-post-accepted', ...negativePost });
      await page.screenshot({ path: path.join(evidenceDir, 'screen-03-after-negative-send.png'), fullPage: true });
      await observe(page, base, evidenceDir, timeline, 'negative', Math.min(observeMs, 120000));
    }

    // 正常结束：点页面「结束」触发 /api/close
    timeline.push({ at: stamp(), event: 'click-close' });
    await page.click('#close');
    await page.waitForTimeout(1500);
    await page.screenshot({ path: path.join(evidenceDir, 'screen-04-after-close.png'), fullPage: true });
    result.closeClicked = true;
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
