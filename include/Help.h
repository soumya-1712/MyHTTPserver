#ifndef HELPREQUESTHANDLER_H
#define HELPREQUESTHANDLER_H

#include "Request.h"

namespace web { 
    class HelpRequestHandler : public RequestHandler {
    public:
        HelpRequestHandler() = default; 
        virtual ~HelpRequestHandler() = default; 

        virtual int constructResponse(const Request& request, Response& response) const; 
    };
}

#endif