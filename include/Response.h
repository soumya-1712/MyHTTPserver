#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <map>

namespace web {
    class Response {
    public:
        Response() = default;
        virtual ~Response() = default; 

        void addHeader(const std::string& key, const std::string& value);

        int responseCode_;
        std::map<std::string, std::string> headers_;
        std::string responseBody_;
    };
}

#endif