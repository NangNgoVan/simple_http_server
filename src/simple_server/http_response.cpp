#include "http_response.h"

HttpResponse::HttpResponse(HttpVersion version)
    : HttpMessage(version)
{
    
}

void HttpResponse::sendText(const std::string& text)
{
    setBody(text);
}

void HttpResponse::prepareStatusLine(HttpVersion version, 
    HttpStatusCode code)
{
    setVersion(version);
    
    m_statusCode = code;
}

std::string HttpResponse::toString()
{
    // std::vector<std::string> lines;

    return "to_string";
}
