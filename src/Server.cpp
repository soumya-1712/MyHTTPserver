#include "Server.h"
#include <iostream>
#include <sstream>
#include <thread>
#include "Request.h"
#include "Response.h"

using namespace web;

std::string ToString(int data) {
    std::stringstream buffer;
    buffer << data;
    return buffer.str();
}

HttpServer::HttpServer(int port)
    : _logging([](const std::string& message) { std::cout << message << std::endl; })
{
    _hints.ai_family = AF_INET;
    _hints.ai_flags = AI_PASSIVE;
    _hints.ai_socktype = SOCK_STREAM;
    _hints.ai_protocol = IPPROTO_TCP;

    _maxConnections = 50;
    _listeningSocket = INVALID_SOCKET;
    _socketVersion = MAKEWORD(2, 2);
    _port = port;
}

void HttpServer::SetLogging(std::function<void(const std::string&)> logging) {
    _logging = logging;
}

int HttpServer::Port() const {
    return _port;
}

void HttpServer::SetPort(int port) {
    _port = port;
}

std::string HttpServer::LocalUrl() const {
    std::stringstream ss;
    ss << "http://localhost:" << _port << "/";
    return ss.str();
}

bool HttpServer::Init() {
    auto resultCode = WSAStartup(_socketVersion, &_wsaData);
    if (0 != resultCode) {
        _logging("Initialize Failed. Error Code: " + ToString(resultCode));
        return false;
    }
    return true;
}

bool HttpServer::Start() {
    std::string port = ToString(_port);
    auto resultCode = getaddrinfo(NULL, port.c_str(), &_hints, &_result);
    if (0 != resultCode) {
        _logging("Resolving Address And Port Failed. Error Code: " + ToString(resultCode));
        return false;
    }
    _listeningSocket = socket(_hints.ai_family, _hints.ai_socktype, _hints.ai_protocol);
    if (INVALID_SOCKET == _listeningSocket) {
        _logging("Could't Create Socket");
        return false;
    }
    resultCode = bind(_listeningSocket, _result->ai_addr, (int)_result->ai_addrlen);
    if (SOCKET_ERROR == resultCode) {
        _logging("Bind Socket Failed");
        return false;
    }
    resultCode = listen(_listeningSocket, _maxConnections);
    if (SOCKET_ERROR == resultCode) {
        _logging("Listening On Port " + ToString(_port) + " Failed");
        return false;
    } else {
        _logging("-Server Is Up And Running.\n--Listening On Port " + ToString(_port) + "...");
    }
    return true;
}

void HttpServer::WaitForRequests(std::function<int(const HttpRequest&, HttpResponse&)> onConnection) {
    while (true) {
        sockaddr_in clientInfo;
        int clientInfoSize = sizeof(clientInfo);

        auto socket = accept(_listeningSocket, (sockaddr*)&clientInfo, &clientInfoSize);
        if (INVALID_SOCKET == socket) {
            _logging("Accepting Connection Failed");
        } else {
            _logging("Spinning thread to handle request");
            std::thread t(HttpRequest::handleRequest, onConnection, HttpRequest(socket, clientInfo));
            t.detach();
        }
    }
}

void HttpServer::Stop() {
    closesocket(_listeningSocket);
    WSACleanup();
}