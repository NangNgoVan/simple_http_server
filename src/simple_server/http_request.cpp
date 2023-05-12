#include "http_request.h"

#include "utils.h"
#include "exceptions.h"

HttpRequest::HttpRequest(HttpVersion version)
    : HttpMessage(version)
{
}

HttpRequest::HttpMethod HttpRequest::getMethod()
{
    return m_method;
};

std::string HttpRequest::getPath()
{
    return m_path;
}

void HttpRequest::parse(std::string text)
{
    std::string line;
    int lineIndex = 0;
    try
    {
        for (char c : text)
        {
            if (c == '\n')
            {

                if (lineIndex == 0)
                    parseStartLine(line);

                line = "";
                lineIndex++;
                continue;
            }

            line += c;
        }
    }
    catch (std::exception &err)
    {
        throw err;
    }
};

void HttpRequest::parseStartLine(std::string line)
{
    std::vector<std::string> splited = Utils::SplitString(line, ' ');

    if (splited.size() != 3)
    {
        perror("Http request start line invalid");
        throw Exception::HttpMessageParseError();
    }

    std::string methodStr = splited[0];

    if (methodStr == "GET")
    {
        m_method = HttpMethod::GET;
    }
    else if (methodStr == "HEAD")
    {
        m_method = HttpMethod::HEAD;
    }
    else if (methodStr == "POST")
    {
        m_method = HttpMethod::POST;
    }
    else if (methodStr == "PUT")
    {
        m_method = HttpMethod::PUT;
    }
    else if (methodStr == "DELETE")
    {
        m_method = HttpMethod::DELETE;
    }
    else if (methodStr == "CONNECT")
    {
        m_method = HttpMethod::CONNECT;
    }
    else if (methodStr == "OPTIONS")
    {
        m_method = HttpMethod::OPTIONS;
    }
    else if (methodStr == "TRACE")
    {
        m_method = HttpMethod::TRACE;
    }

    m_path = splited[1];

    std::string versionStr = splited[2];

    if (versionStr == "HTTP/1.1")
    {
        setVersion(HTTP1_1);
    }
    else if (versionStr == "HTTP/1.0")
    {
        setVersion(HTTP1_0);
    }
};

void HttpRequest::parseHeaderLine(){

};

void HttpRequest::parseBody(){

};

std::string HttpRequest::toString()
{
    return "";
}
