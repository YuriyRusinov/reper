/***********************************************************************
 * Module:  KKSData.cpp
 * Author:  sergey
 * Modified: 8 декабря 2008 г. 16:53:33
 * Purpose: Implementation of the class KKSData
 * Comment: базовый класс для всех классов ККССИТО
 *    Основное его применение - подсчет количества ссылок на объекты класса и их автоматическое уничтожение, когда количество ссылок стало равным нулю.
 ***********************************************************************/

#include "KKSData.h"
#include <QtDebug>

KKSData::KKSData()
{
    m_refCount = 1;
}

KKSData::~KKSData()
{
    m_refCount--;
    if(m_refCount > 0)
        qWarning("KKSData::~KKSData() was invoked but refCount > 0!");
    
    //qWarning("KKSData object destroyed!");
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSData::addRef()
// Purpose:    Implementation of KKSData::addRef()
// Return:     void
////////////////////////////////////////////////////////////////////////

void KKSData::addRef(void) const
{
    //qWarning("addRef() invoked!");
    m_refCount++;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSData::release()
// Purpose:    Implementation of KKSData::release()
// Return:     void
////////////////////////////////////////////////////////////////////////

void KKSData::release(void) const
{
    //qWarning("release() invoked!");
    int cnt = --m_refCount;

    if(cnt <= 0){
        delete this;
        //this = 0;
    }

}

////////////////////////////////////////////////////////////////////////
// Name:       KKSData::refCount()
// Purpose:    Implementation of KKSData::refCount()
// Return:     int
////////////////////////////////////////////////////////////////////////

int KKSData::refCount(void) const
{
   return m_refCount;
}
