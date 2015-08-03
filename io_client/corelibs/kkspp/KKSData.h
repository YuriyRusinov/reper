/***********************************************************************
 * Module:  KKSData.h
 * Author:  sergey
 * Modified: 8 декабря 2008 г. 16:53:33
 * Purpose: Declaration of the class KKSData
 * Comment: базовый класс для всех классов ККССИТО
 *    Основное его применение - подсчет количества ссылок на объекты класса и их автоматическое уничтожение, когда количество ссылок стало равным нулю.
 ***********************************************************************/

/*!\defgroup PP_GROUP Подсистема представления дданных
*/

#if !defined(__KKSSITOOOM_KKSData_h)
#define __KKSSITOOOM_KKSData_h

#include "kkspp_config.h"
#include "KKSPtr.h"

/*! \ingroup PP_GROUP
\class KKSData
\brief Базовый класс для всех классов ККССИТО
 
 Основное его применение - подсчет количества ссылок на объекты класса и их автоматическое уничтожение, когда количество ссылок стало равным нулю.
*/
class _PP_EXPORT KKSData
{
public:
   void addRef(void) const;//!<Увеличить счетчик. Добавилась еще одна ссылка на объект
   void release(void) const;//!<Уменьшить счетчик. Ссылка на объект больше не используется. Когда количество используемых ссылок станет равным 0, объект будет удален.
   int refCount(void) const;//!<Возвращается количество используемых ссылок на объект

   KKSData();//!<Конструктор
   virtual ~KKSData();//!<Деструктор

protected:
private:
   mutable int m_refCount;
};

typedef KKSPtr<KKSData> KKSDataPtr;

#endif
