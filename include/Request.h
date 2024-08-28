#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <winsock2.h>
#include <functional>
#include <string>
#include <map>

#define BUFFER_SIZE 1024 * 5 // 5KB

namespace web {
    class Request {
        SOCKET _socket;
        sockaddr_in _clientInfo;

    public:
        Request(SOCKET socket, sockaddr_in clientInfo);
        static void handleRequest(std::function<int(const Request &, class Response &)> onConnection, const Request &request); 
        std::string getMessage() const; 
        std::string ipAddress() const; 

        std::string _method;
        std::string _uri;
        std::map<std::string, std::string> _headers;
        std::string _payload;
    };

    class RequestHandler {
    public:
        virtual ~RequestHandler() = default; 
        virtual int constructResponse(const Request &request, class Response &response) const = 0; 
    };
}

#endif