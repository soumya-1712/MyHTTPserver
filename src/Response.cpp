#include "Response.h"

using namespace web;

HttpResponse::HttpResponse() : _responseCode(200) {}

HttpResponse::~HttpResponse() {}

void HttpResponse::addHeader(const std::string& key, const std::string& value) {
    _headers.emplace(key, value);
}

void HttpResponse::setResponse(const std::string& response) {
    _response = response;
}

int HttpResponse::getResponseCode() const {
    return _responseCode;
}

void HttpResponse::setResponseCode(int code) {
    if (responseCodes.find(code) != responseCodes.end()) {
        _responseCode = code;
    } else {
        throw std::runtime_error("Invalid response code");
    }
}

std::string HttpResponse::getResponse() const {
    return _response;
}

std::map<std::string, std::string> HttpResponse::getHeaders() const {
    return _headers;
}