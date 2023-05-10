#include "http_response.h"

#include "utils.h"
#include <fstream>

HttpResponse::HttpResponse(HttpVersion version)
    : HttpMessage(version), m_statusCode(OK)
{
    
}

void HttpResponse::sendText(const std::string& text)
{
    setBody(text);
}

void HttpResponse::sendFile(const std::string &filePath)
{
    std::ifstream f(filePath);

    std::string fileContent;
    std::string line;

    if(f.is_open()) {
        while(getline(f, line)) {
            fileContent += line;
        }
    }
    else {
        perror("File not found!");
    }

    f.close();

    setBody(fileContent);
}

void HttpResponse::setStatusCode(HttpStatusCode code)
{
    m_statusCode = code;
}

std::string HttpResponse::toString()
{
    std::string result;

    std::string statusLine = 
        Utils::JoinStrings(
            {
                HttpMessage::GetVersionString(getVersion()),
                std::to_string(m_statusCode),
                HttpMessage::GetStatusCodeString(m_statusCode)
            }, ' ');

    //status line
    result += statusLine + "\n";
    //headers
    std::map<std::string, std::string> headers = getHeaders();
    for (auto itr = headers.begin(); itr != headers.end(); ++itr) {
        std::string key = itr->first;
        std::string val = itr->second;
        result += key + ":" + val + "\n";
    }
    //empty line
    result += "\n";
    //body
    result += getBody();

    return result;
}
