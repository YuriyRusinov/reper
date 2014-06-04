#include "logger.h"

void Logger::log(const string & message) const
{
    SysJurn(m_pstkmpr, const_cast<char*>(message.c_str()), NULL, 0);
}

template<typename T>
const T& Logger::log(const string & message, const T& result) const
{
    log(message);
    return result;
}

template const bool& Logger::log(const std::string & , const bool&) const;
template const u long& Logger::log(const std::string & , const u long&) const;
