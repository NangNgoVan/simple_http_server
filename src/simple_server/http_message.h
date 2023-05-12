#ifndef HTTP_MESSAGE_H
#define HTTP_MESSAGE_H

#include <iostream>
#include <map>

#include "exceptions.h"

class HttpMessage
{
public:
    enum HttpVersion
    {
        HTTP1_0,
        HTTP1_1
    };

    enum HttpMethod
    {
        GET = 0,
        HEAD,
        POST,
        PUT,
        DELETE,
        CONNECT,
        OPTIONS,
        TRACE,
        PATH,
    };

    enum HttpStatusCode
    {
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
        MOVED_PERMANENTY = 301,
        FOUND = 302,
        SEE_OTHER = 303,
        NOT_MODIFIED = 304,
        TEMPORARY_REDIRECT = 307,
        BAD_REQUEST = 400,
        UNAUTHORIZED = 401,
        PAYMENT_REQUIRED = 402,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        METHOD_NOT_ALLOWED = 405,
        NOT_ACCEPTABLE = 406,
        PROXY_AUTHENTICATION_REQUIRED = 407,
        REQUEST_TIME = 408,
        CONFLICT = 409,
        GONE = 410,
        LENGTH_REQUIRED = 411,
        PRECONDITION_FAILED = 412,
        PAYLOAD_TOO_LARGE = 413,
        URI_TOO_LONG = 414,
        UNSUPPORTED_MEDIA_TYPE = 415,
        RANGE_NOT_SATISFIABLE = 416,
        EXPECTATION_FAILED = 417,
        IAM_A_TEAPOT = 418,
        MISDIRECTED_REQUEST = 421,
        LOCKED = 423,
        FAILED_DEPENDENCY = 424,
        TOO_EARLY = 425,
        TOO_MANY_REQUESTS = 429,
        REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
        UNAVALABLE_FOR_LEGAL_REASONS = 451,
        INTERNAL_SERVER_ERROR = 500,
        NOT_IMPLEMENTED = 501,
        BAD_GATEWAY = 502,
        SERVICE_UNAVAILABLE = 503,
        GATEWAY_TIMEOUT = 504,
        HTTP_VERSION_NOT_SUPPORTED = 505,
        VARIAN_ALSO_NEGOTIATES = 506,
        INSUFFICENT_STORAGE = 507,
        LOOP_DETECTED = 508,
        NOT_EXTENDED = 510,
        NETWORD_AUTHENTICATION_REQUIRED = 512
    };

    static std::string GetStatusCodeString(HttpStatusCode code)
    {
        switch (code)
        {
        case OK:
            /* code */
            return "OK";

        default:
            return "";
        }
    }

    HttpMessage(HttpVersion version = HTTP1_1) { m_version = version; };

    void setVersion(HttpVersion version) { m_version = version; };

    static std::string GetVersionString(HttpVersion version)
    {
        switch (version)
        {
        case HTTP1_0:
            return "HTTP/1.0";
        case HTTP1_1:
        default:
            return "HTTP/1.1";
        }
    }

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

    std::map<std::string, std::string> getHeaders()
    {
        return m_headers;
    }

private:
    HttpVersion m_version;
    std::string m_body;
    std::map<std::string, std::string> m_headers;
};

#endif
