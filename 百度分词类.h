#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include <stdexcept>
#include <sstream>
#include <memory>
#include <json/json.h>
#include <curl/curl.h>


// ====== 简单 Token 结构 ======
struct Token {
    std::string word;   // 分词结果
    std::string pos;    // 词性(可能为空)
    int begin = -1;     // 在原文中的起始偏移（Baidu返回可能是byte偏移）
    int end = -1;     // 结束偏移（开区间）
};

// ====== Baidu Lexer 客户端 ======
class BaiduLexerClient {
public:
    // ak/sk 从百度智能云控制台获取；app_id不需要用到，但保留以便你后续扩展
    BaiduLexerClient(std::string app_id, std::string ak, std::string sk)
        : app_id_((app_id)), ak_((ak)), sk_((sk)) {
    }
    BaiduLexerClient() {}

    // 主功能：分词 + 词性标注
    std::vector<Token> lexer(const std::string& text,
        int timeout_ms = 6000 /*http超时*/) {
        if (text.empty()) return {};

        std::string token = get_access_token();  // 自动获取/缓存
        // 构造URL（access_token 放在 query）
        std::string url =
            "https://aip.baidubce.com/rpc/2.0/nlp/v1/lexer?access_token=" + token;

        // 请求体（JSON）
        Json::Value body;
        body["text"] = text;
        std::string body_str = body.toStyledString();

        // HTTP POST
        std::string resp = http_post_json(url, body_str, timeout_ms);

        // 解析结果
        return parse_lexer(resp);
    }
    void 初始化(std::string app_id, std::string ak, std::string sk) {
        app_id_ = app_id;
        ak_ = ak;
        sk_ = sk;
    }
private:
    // ========== 访问令牌管理 ==========
    std::string get_access_token() {
        using clock = std::chrono::steady_clock;

   //     std::lock_guard<std::mutex> lock(mtx_);

        // 若未过期，直接返回
        if (!access_token_.empty() && clock::now() < token_expire_tp_) {
            return access_token_;
        }
    
        // 调用 OAuth token
        std::ostringstream oss;
        oss << "https://aip.baidubce.com/oauth/2.0/token"
            << "?grant_type=client_credentials"
            << "&client_id=" << curl_easy_escape_dummy(ak_)   // 简单转义
            << "&client_secret=" << curl_easy_escape_dummy(sk_);
        const std::string url = oss.str();

        std::string resp = http_post_json(url, /*empty body*/"", /*timeout*/5000);

        // 解析 JSON
        Json::CharReaderBuilder rb;
        Json::Value root;
        std::string errs;
        std::unique_ptr<Json::CharReader> rd(rb.newCharReader());
        if (!rd->parse(resp.data(), resp.data() + resp.size(), &root, &errs)) {
            throw std::runtime_error("Parse token JSON failed: " + errs);
        }

        if (root.isMember("error")) {
            // 旧格式
            std::string err = root.get("error_description", "").asString();
            if (err.empty()) err = root.get("error", "unknown").asString();
            throw std::runtime_error("Token error: " + err);
        }

        access_token_ = root.get("access_token", "").asString();
        const int expires_in = root.get("expires_in", 3600).asInt(); // 秒

        if (access_token_.empty()) {
            throw std::runtime_error("Token missing in response");
        }

        // 提前 5 分钟过期，避免边界
        int safe_expires = (expires_in > 600) ? (expires_in - 300) : (expires_in / 2);
        token_expire_tp_ = clock::now() + std::chrono::seconds(safe_expires);
        return access_token_;
    }

    // ========== HTTP 帮助 ==========
    static size_t write_cb(char* ptr, size_t size, size_t nmemb, void* userdata) {
        auto* out = static_cast<std::string*>(userdata);
        out->append(ptr, size * nmemb);
        return nmemb;
    }

    // 简化版转义（仅用于拼query的client_id/client_secret；更严谨可用 curl_easy_escape）
    static std::string curl_easy_escape_dummy(const std::string& s) {
        std::ostringstream o;
        for (unsigned char c : s) {
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
                (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~') {
                o << c;
            }
            else {
                static const char* hex = "0123456789ABCDEF";
                o << '%' << hex[c >> 4] << hex[c & 0x0F];
            }
        }
        return o.str();
    }

    static std::string http_post_json(const std::string& url,
        const std::string& body,
        int timeout_ms) {
        CURL* curl = curl_easy_init();
        if (!curl) throw std::runtime_error("curl_easy_init failed");

        std::string response;
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Accept: application/json");
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // 不校验证书链
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // 不校验主机名
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)body.size());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, (long)timeout_ms);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 5000L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // 调试：打开详细日志（可在发布版关闭）
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        // 如需临时排查证书问题，可先关闭（仅用于定位！）
        // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        CURLcode rc = curl_easy_perform(curl);
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (rc != CURLE_OK) {
            throw std::runtime_error(std::string("curl error: ") +
                curl_easy_strerror(rc));
        }
        if (http_code < 200 || http_code >= 300) {
            std::ostringstream oss;
            oss << "HTTP " << http_code << " body: " << response;
            throw std::runtime_error(oss.str());
        }
        return response;
    }

    // ========== 结果解析 ==========
    static std::vector<Token> parse_lexer(const std::string& json_str) {
        Json::CharReaderBuilder rb;
        Json::Value root;
        std::string errs;
        std::unique_ptr<Json::CharReader> rd(rb.newCharReader());
        if (!rd->parse(json_str.data(), json_str.data() + json_str.size(), &root, &errs)) {
            throw std::runtime_error("Parse lexer JSON failed: " + errs);
        }

        // 错误处理
        if (root.isMember("error_code") && root["error_code"].asInt() != 0) {
            std::ostringstream oss;
            oss << "API error: " << root["error_code"].asInt()
                << " - " << root.get("error_msg", "").asString();
            throw std::runtime_error(oss.str());
        }

        std::vector<Token> out;

        // Baidu 典型返回：items 数组，每个 item 有 "item"(词) , "pos"(词性), "byte_offset"/"byte_length"
        const Json::Value& items = root["items"];
        if (items.isArray()) {
            out.reserve(items.size());
            for (const auto& it : items) {
                Token t;
                t.word = it.get("item", "").asString();
                t.pos = it.get("pos", "").asString();

                // 偏移（某些版本字段名不同，做几个兜底）
                int begin = -1, end = -1;
                if (it.isMember("byte_offset") && it.isMember("byte_length")) {
                    begin = it["byte_offset"].asInt();
                    end = begin + it["byte_length"].asInt();
                }
                else if (it.isMember("offset") && it.isMember("length")) {
                    begin = it["offset"].asInt();
                    end = begin + it["length"].asInt();
                }
                t.begin = begin;
                t.end = end;

                out.push_back((t));
            }
        }
        return out;
    }

private:
    std::string app_id_;
    std::string ak_;
    std::string sk_ ;
   ;
 //   std::mutex mtx_;
    std::string access_token_;
    std::chrono::steady_clock::time_point token_expire_tp_{};
};
