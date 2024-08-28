#ifndef STRINGREQUESTHANDLER_H
#define STRINGREQUESTHANDLER_H

#include <string>
#include <functional>
#include "Request.h"
#include "Response.h"

namespace web {
    class StringRequestHandler : public RequestHandler {
    private:
        std::function<void(const std::string&)> _loggingCallback;
        std::string _message;

    public:
        explicit StringRequestHandler(const std::string& message);
        virtual ~StringRequestHandler() = default;

        void setLoggingCallback(std::function<void(const std::string&)> loggingCallback);

        virtual int constructResponse(const Request& request, Response& response) const override;
    };
}

#endif