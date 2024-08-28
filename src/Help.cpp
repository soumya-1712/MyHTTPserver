#include "Help.h"
#include "Response.h"
#include <sstream>

class HelpRequestHandler {
public:
    HelpRequestHandler();
    int constructHelpResponse(const web::Request& request, web::Response& response) const;

private:
    std::string generateHelpHtml() const;
};

HelpRequestHandler::HelpRequestHandler()
{
}

int HelpRequestHandler::constructHelpResponse(const web::Request& request, web::Response& response) const
{
    std::string helpHtml = generateHelpHtml();
    response.addHeader("Content-Type", "text/html");
    response.setResponse(helpHtml);
    return 200;
}

std::string HelpRequestHandler::generateHelpHtml() const
{
    std::stringstream ss;
    ss << "<html><body>";
    ss << "<h1>Assistance</h1>";
    ss << "</body></html>";
    return ss.str();
}