#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include "http_message.h"

class HttpResponse : public HttpMessage
{
public:
    explicit HttpResponse(
        HttpVersion version = HttpVersion::HTTP1_1);

    void sendText(const std::string &text);
    void prepareStatusLine(HttpVersion version,
                           HttpStatusCode code);

    std::string toString();

private:
    HttpStatusCode m_statusCode;
};

#endif
