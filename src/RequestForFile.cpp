#include "RequestForFile.h"
#include "HttpUtil.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>

namespace web {

class FileSystemRequestHandler {
public:
    FileSystemRequestHandler();
    ~FileSystemRequestHandler();

    void setRootPath(const std::string& rootPath);
    void setLogger(std::function<void(const std::string&)> logger);

    MimeTypes& getMimeTypes();

    int constructResponse(const web::Request& request, web::Response& response) const;

private:
    System::IO::DirectoryInfo _rootDir;
    std::function<void(const std::string&)> _logger;
    MimeTypes _mimeTypes;
};

FileSystemRequestHandler::FileSystemRequestHandler()
    : _rootDir(System::IO::DirectoryInfo("")), _logger([](const std::string& message) { std::cout << message << std::endl; })
{
    _mimeTypes.loadDefaultMimeTypes();
}

FileSystemRequestHandler::~FileSystemRequestHandler() {}

void FileSystemRequestHandler::setRootPath(const std::string& rootPath)
{
    _rootDir = System::IO::DirectoryInfo(rootPath);
}

void FileSystemRequestHandler::setLogger(std::function<void(const std::string&)> logger)
{
    _logger = logger;
}

MimeTypes& FileSystemRequestHandler::getMimeTypes()
{
    return _mimeTypes;
}

int FileSystemRequestHandler::constructResponse(const web::Request& request, web::Response& response) const
{
    _logger(request.getMethod());
    auto uri = request.getUri();
    if (uri[0] != '/')
    {
        return 500;
    }
    uri = uri.substr(1);
    uri = urlDecode(uri);
    auto fullPath = System::IO::Path::Combine(_rootDir.FullName(), uri);
    _logger(fullPath);

    for (const auto& header : request.getHeaders())
    {
        std::stringstream ss;
        ss << "[header] " << header.first << ": " << header.second;
        _logger(ss.str());
    }

    std::stringstream responseStream;

    if (System::IO::DirectoryInfo(fullPath).exists())
    {
        responseStream << "<html><body>";
        System::IO::DirectoryInfo directory(fullPath);
        responseStream << "<h1>" << request.getUri() << "</h1>";
        responseStream << "<h2>Directories:</h2>";
        responseStream << "<ul>";
        if (request.getUri() != "/")
        {
            auto relativeParentPath = directory.Parent().FullName().substr(_rootDir.FullName().size());
            if (relativeParentPath == "")
                relativeParentPath = "/";
            responseStream << "<li><a href=\"" << relativeParentPath << "\">..</a></li>";
        }
        for (const auto& dir : directory.GetDirectories())
        {
            System::IO::DirectoryInfo dirInfo(dir);
            auto relativePath = dirInfo.FullName().substr(_rootDir.FullName().size());
            responseStream << "<li><a href=\"" << relativePath << "\">" << dirInfo.Name() << "</a></li>";
        }
        responseStream << "</ul>";
        responseStream << "<h2>Files:</h2>";
        responseStream << "<ul>";
        for (const auto& file : directory.GetFiles())
        {
            System::IO::FileInfo fileInfo(file);
            auto relativePath = fileInfo.FullName().substr(_rootDir.FullName().size());
            responseStream << "<li><a href=\"" << relativePath << "\">" << fileInfo.Name() << "</a></li>";
        }
        responseStream << "</ul>";
        responseStream << "</body></html>";

        response.addHeader("Content-Type", "text/html");
    }
    else if (System::IO::FileInfo(fullPath).exists())
    {
        std::ifstream file(fullPath, std::ios::binary);
        auto extpos = fullPath.find_last_of('.');
        auto ext = extpos != std::string::npos ? fullPath.substr(extpos) : "";
        response.addHeader("Content-Type", _mimeTypes.getMimeType(ext));

        responseStream << std::istreambuf_iterator<char>(file) << std::istreambuf_iterator<char>();
    }
    else
    {
        responseStream << "<html><body>";
        responseStream << request.getUri() << " does not exist";
        responseStream << "</body></html>";

        response.addHeader("Content-Type", "text/html");
    }

    response.setResponse(responseStream.str());

    if (request.getHeaders().find("Range") != request.getHeaders().end())
    {
        auto range = request.getHeaders().at("Range");
       