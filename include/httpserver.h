#ifndef HTPPSERVER_H
#define HTPPSERVER_H
#include <functional>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#define DEFAULT_PORT 8080
namespace web
{
    class HttpServer
    {
        std::function<void(const std::string &)> _logging;
        WORD _socketVersion;
        WSADATA _wsaData;
        SOCKET _listeningSocket;
        addrinfo _hints, *_result;
        int _maxConnections;
        int _port;

    public:
        HttpServer(int port = DEFAULT_PORT);
        void SetLogging(std::function<void(const std::string &)> logging);

        int Port() const;
        void SetPort(int port);

        std::string LocalUrl() const;

        bool Init();  
        bool Start(); 
        void WaitForRequests(std::function<int(const class Request &, class Response &)> onConnection);
        void Stop(); 

        static void handleRequest(std::function<int(const class Request &, class Response &)> onConnection, class Request request);
    };
}
#endif
