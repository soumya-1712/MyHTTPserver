#include "Request.h"
#include "Response.h"
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cctype>

using namespace web;

static std::map<int, std::string> responseCodes = 
std::map<int, std::string>(
    {{100, "Continue"},
      {101, "Switching Protocols"},
      {200, "OK"},
      {201, "Created"},
      {202, "Accepted"},
      {203, "Non-Authoritative Information"},
      {204, "No Content"},
      {205, "Reset Content"},
      {206, "Partial Content"},
      {300, "Multiple Choices"},
      {301, "Moved Permanently"},
      {302, "Found"},
      {303, "See Other"},
      {304, "Not Modified"},
      {305, "Use Proxy"},
      {306, "(Unused)"},
      {307, "Temporary Redirect"},
      {400, "Bad Request"},
      {401, "Unauthorized"},
      {402, "Payment Required"},
      {403, "Forbidden"},
      {404, "Not Found"},
      {405, "Method Not Allowed"},
      {406, "Not Acceptable"},
      {407, "Proxy Authentication Required"},
      {408, "Request Timeout"},
      {409, "Conflict"},
      {410, "Gone"},
      {411, "Length Required"},
      {412, "Precondition Failed"},
      {413, "Request Entity Too Large"},
      {414, "Request-URI Too Long"},
      {415, "Unsupported Media Type"},
      {416, "Requested Range Not Satisfiable"},
      {417, "Expectation Failed"},
      {500, "Internal Server Error"},
      {501, "Not Implemented"},
      {502, "Bad Gateway"},
      {503, "Service Unavailable"},
      {504, "Gateway Timeout"},
      {505, "HTTP Version Not Supported"}});

static inline std::string& trim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int c) { return !std::isspace(c); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int c) { return !std::isspace(c); }).base(), s.end());
    return s;
}

HttpRequest::HttpRequest(SOCKET socket, sockaddr_in clientInfo)
    : _socket(socket), _clientInfo(clientInfo)
{
    std::string requestData = receiveMessage();
    auto pos = requestData.find("\r\n\r\n");
    if (pos != std::string::npos) {
        std::stringstream ss;
        ss.str(requestData.substr(0, pos));
        std::string line;

        if (std::getline(ss, line)) {
            trim(line);
            auto first = line.find_first_of(' ');
            auto last = line.find_last_of(' ');
            if (first != std::string::npos && last != std::string::npos) {
                auto version = line.substr(last);
                if (trim(version) != "HTTP/1.1")
                    throw std::runtime_error("Invalid HTTP version");

                auto method = line.substr(0, first);
                this->_method = trim(method);

                auto uri = line.substr(first, last - first);
                this->_uri = trim(uri);
            }
        }

        while (std::getline(ss, line)) {
            auto pos = line.find_first_of(':');
            if (pos != std::string::npos) {
                auto key = line.substr(0, pos);
                auto value = line.substr(pos + 1);
                this->_headers.insert(std::make_pair(trim(key), trim(value)));
            }
        }

        this->_payload = requestData.substr(pos + 4);
    }
}

std::string HttpRequest::receiveMessage() {
    char buffer[BUFFER_SIZE + 1];
    int bytes;
    bytes = recv(this->_socket, buffer, BUFFER_SIZE, 0);
    buffer[bytes] = '\0';
    std::string browserData = buffer;
    while (BUFFER_SIZE == bytes) {
        bytes = recv(this->_socket, buffer, BUFFER_SIZE, 0);
        buffer[bytes] = '\0';
        browserData += buffer;
    }
    return browserData;
}

#define STREAMING_THRESHOLD 1024

void HttpRequest::handleRequest(std::function<int(const HttpRequest&, HttpResponse&)> onConnection, HttpRequest request) {
    if (request._method.empty()) {
        shutdown(request._socket, SD_BOTH);
        closesocket(request._socket);

        return;
    }

    HttpResponse response;
    int responseCode = onConnection(request, response);
    std::stringstream headers;
    headers << "HTTP/1.1 " << responseCode << " " << responseCodes[responseCode] << "\r\n";
    for (auto pair : response._headers) {
        headers << pair.first << ": " << pair.second << "\r\n";
    }
    headers << "Content-Length: " << response._response.size() << "\r\n"
            << "\r\n";
    send(request._socket, headers.str().c_str(), headers.str().size(), 0);
    send(request._socket, response._response.c_str(), response._response.size(), 0);

    shutdown(request._socket, SD_BOTH);
    closesocket(request._socket);
}

std::string HttpRequest::ipAddress() const {
    return std::string(inet_ntoa(this->_clientInfo.sin_addr));
}