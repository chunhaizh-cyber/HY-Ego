// Minimal Winsock2 loopback HTTP smoke (CI environment probe only).
// Mirrors HY-Ego formal host stack: WSAStartup + TCP socket + bind(INADDR_LOOPBACK)
// + listen + accept + raw HTTP/1.1 write. Does NOT use HttpListener / HTTP.sys.
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdio>
#include <cstring>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

namespace {
constexpr const char kBody[] = "hy-ego-loopback-http-ok";
}

int main() {
    WSADATA wsa{};
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::fprintf(stderr, "WSAStartup failed: %d\n", WSAGetLastError());
        return 2;
    }

    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSock == INVALID_SOCKET) {
        std::fprintf(stderr, "socket failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 2;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(0); // ephemeral port, same pattern as formal host

    if (bind(listenSock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
        std::fprintf(stderr, "bind failed: %d\n", WSAGetLastError());
        closesocket(listenSock);
        WSACleanup();
        return 2;
    }

    int addrLen = sizeof(addr);
    if (getsockname(listenSock, reinterpret_cast<sockaddr*>(&addr), &addrLen) == SOCKET_ERROR) {
        std::fprintf(stderr, "getsockname failed: %d\n", WSAGetLastError());
        closesocket(listenSock);
        WSACleanup();
        return 2;
    }
    const unsigned port = ntohs(addr.sin_port);
    std::printf("LISTEN=http://127.0.0.1:%u/\n", port);
    std::fflush(stdout);

    if (listen(listenSock, 1) == SOCKET_ERROR) {
        std::fprintf(stderr, "listen failed: %d\n", WSAGetLastError());
        closesocket(listenSock);
        WSACleanup();
        return 2;
    }

    // Single-request smoke then exit (no public bind, loopback only).
    SOCKET client = accept(listenSock, nullptr, nullptr);
    if (client == INVALID_SOCKET) {
        std::fprintf(stderr, "accept failed: %d\n", WSAGetLastError());
        closesocket(listenSock);
        WSACleanup();
        return 2;
    }

    char request[4096];
    const int got = recv(client, request, sizeof(request) - 1, 0);
    if (got <= 0) {
        std::fprintf(stderr, "recv failed or empty: %d\n", WSAGetLastError());
        closesocket(client);
        closesocket(listenSock);
        WSACleanup();
        return 2;
    }
    request[got] = '\0';

    const std::string body = kBody;
    std::string response;
    response.reserve(128 + body.size());
    response += "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/plain;charset=utf-8\r\n";
    response += "Content-Length: ";
    response += std::to_string(body.size());
    response += "\r\nConnection: close\r\n\r\n";
    response += body;

    int sent = 0;
    while (sent < static_cast<int>(response.size())) {
        const int n = send(client, response.data() + sent,
            static_cast<int>(response.size()) - sent, 0);
        if (n <= 0) {
            std::fprintf(stderr, "send failed: %d\n", WSAGetLastError());
            closesocket(client);
            closesocket(listenSock);
            WSACleanup();
            return 2;
        }
        sent += n;
    }

    closesocket(client);
    closesocket(listenSock);
    WSACleanup();
    std::printf("DONE=ok\n");
    std::fflush(stdout);
    return 0;
}
