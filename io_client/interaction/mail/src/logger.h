#ifndef LOGGER_H
#define LOGGER_H

#include <string>

using std::string;

extern "C"
{
    #include "plot.h"
}

class Logger
{
    PSTKMPR m_pstkmpr;
public:
    Logger(PSTKMPR pstkmpr) : m_pstkmpr(pstkmpr) {};
    Logger() {};

    void setPSTKMPR(PSTKMPR pstkmpr) {m_pstkmpr = pstkmpr;}
    void log(const string &) const;
    template<typename T>
    const T& log(const string & message, const T& result) const;
};

#endif
