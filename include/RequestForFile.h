#ifndef FILESYSTEMREQUESTHANDLER_H
#define FILESYSTEMREQUESTHANDLER_H

#include <string>
#include <functional>
#include "mimetypes.h"
#include "Request.h"
#include "Response.h"

namespace web { 
    class FileSystemRequestHandler : public RequestHandler {
    private:
        std::function<void(const std::string&)> logging_; 
        MimeTypes mimeTypes_; 

    public:
        FileSystemRequestHandler() = default; 
        virtual ~FileSystemRequestHandler() = default; 

        void setRoot(const std::string& root); 
        void setLogging(std::function<void(const std::string&)> logging);
        MimeTypes& getMimeTypes(); 

        virtual int constructResponse(const Request& request, Response& response) const override; 
    };
}

#endif