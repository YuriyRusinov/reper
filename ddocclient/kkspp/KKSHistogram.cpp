/* 
 * File:   KKSHistogram.cpp
 * Author: rusinov
 * 
 * Created on 12 Декабрь 2013 г., 13:12
 */

#include "KKSHistogram.h"

KKSHistogram::KKSHistogram(const QMap<int, double>& data, double xmin, double xmax, int n)
    : dHist (data),
    m_xmin (xmin),
    m_xmax (xmax),
    m_num (n)
{
}

KKSHistogram::KKSHistogram(const KKSHistogram& orig)
    : dHist (orig.dHist),
    m_xmin (orig.m_xmin),
    m_xmax (orig.m_xmax),
    m_num (orig.m_num)
{
}

KKSHistogram::~KKSHistogram()
{
}

const QMap<int, double>& KKSHistogram::getVec (void) const
{
    return dHist;
}

void KKSHistogram::setVec (const QMap<int, double>& data)
{
    dHist = data;
}

void KKSHistogram::setRange (double xmin, double xmax)
{
    m_xmin = xmin;
    m_xmax = xmax;
}

double KKSHistogram::getXMin (void) const
{
    return m_xmin;
}

double KKSHistogram::getXMax (void) const
{
    return m_xmax;
}

int KKSHistogram::size (void) const
{
    return m_num;
}

void KKSHistogram::setSize (int n)
{
    m_num = n;
}


