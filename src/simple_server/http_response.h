#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include "http_message.h"

class HttpResponse : public HttpMessage
{
public:
    explicit HttpResponse(
        HttpVersion version = HttpVersion::HTTP1_1);

    void sendText(const std::string &text);
    void sendFile(const std::string &filePath);

    void setStatusCode(HttpStatusCode code);

    std::string toString();

private:
    HttpStatusCode m_statusCode;
};

#endif
