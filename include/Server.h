#ifndef HTPPSERVER_H
#define HTPPSERVER_H

#include <functional>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace web {
    class HttpServer {
    private:
        std::function<void(const std::string &)> _logging;
        WORD _socketVersion;
        WSADATA _wsaData;
        SOCKET _listeningSocket;
        addrinfo _hints, *_result;
        int _maxConnections;
        int _port;

    public:
        explicit HttpServer(int port = 8080); 
        void setLogging(std::function<void(const std::string &)> logging); 

        int port() const; 
        void setPort(int port);

        std::string localUrl() const;

        bool init(); 
        bool start();
        void waitForRequests(std::function<int(const class Request &, class Response &)> onConnection);
        void stop();

        static void handleRequest(std::function<int(const class Request &, class Response &)> onConnection, const class Request &request); // Pass request by const reference
    };
}

#endif