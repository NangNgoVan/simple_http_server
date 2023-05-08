#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

// #define EXCEPTION(exceptionType) struct exeptionType : public std::exception {  }

namespace Exception {

    class InternalServerError : public std::exception 
    {
        public:
            explicit InternalServerError(std::string detail = "Internal Server Error!") {
                m_detail = detail;
            };
            const char *what() const throw() {
                return m_detail.c_str();
            }
        private:
            std::string m_detail;
    };


    class HttpMessageParseError : public std::exception
    {
        public:
            const char *what() const throw() {
                return "Http message parse error!";
            }
    };

}

#endif

