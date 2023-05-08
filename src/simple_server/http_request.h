#ifndef HTTP_RESQUEST_H
#define HTTP_RESQUEST_H

#include "http_message.h"

class HttpRequest : public HttpMessage
{
public:
    explicit HttpRequest(
        HttpVersion version = HttpVersion::HTTP1_1);

    HttpMethod getMethod();

    std::string getPath();

    void parse(std::string text);

    std::string toString();

private:
    HttpMethod m_method;
    std::string m_path;

    void parseStartLine(std::string line);
    void parseHeaderLine();
    void parseBody();

    // std::string host;
    // std::string userArgent;
    // std::string accept;
    // std::string acceptLanguage;
    // std::string acceptEncoding;
};

#endif
