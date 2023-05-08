#ifndef HTTP_MESSAGE_H
#define HTTP_MESSAGE_H

#include <iostream>
#include <map>

#include "exceptions.h"

class HttpMessage 
{
public:
    enum HttpVersion {
        HTTP1_0,
        HTTP1_1
    };

    enum HttpMethod {
        GET = 0,
        HEAD,
        POST,
        PUT,
        DELETE,
        CONNECT,
        OPTIONS,
        TRACE,
        PATH,
        UNKNOW
    };

    enum HttpStatusCode {
        CONTINUE = 100,
        SWITCHING_PROTOCOL = 101,
        PROCESSING = 102,
        OK = 200,
        CREATED = 201,
        ACCEPTED = 202,
        NON_AUTHORITATIVE_INFOMATION = 203,
        NO_CONTENT = 204,
        RESET_CONTENT = 205,
        PARTIAL_CONTENT = 206,
        MULTI_STATUS = 207,
        ALREADY_SUPPORTED = 208,
        IM_USED = 209,
        MULTIPLE_CHOICE = 300,
        MOVED_TEMP = 301,

    };

    HttpMessage(HttpVersion version = HTTP1_1) { m_version = version; };
    
    void setVersion(HttpVersion version) { m_version = version; };

    virtual ~HttpMessage() = default;

    HttpVersion getVersion() { return m_version; };
    

    void setHeader(std::string key, std::string value)
    {
        m_headers.emplace(key, value);
    }

    void setBody(std::string body)
    {
        m_body = body;
    }

    std::string getBody() { return m_body; };

    virtual std::string toString() = 0;

private:
    HttpVersion m_version;
    std::string m_body;
    std::map<std::string, std::string> m_headers;
};

#endif
