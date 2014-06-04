/***********************************************************************
 * Module:  KKSFilter.h
 * Author:  sergey
 * Modified: 25 декабря 2008 г. 17:22:35
 * Purpose: Declaration of the class KKSFilter
 * Comment: фильтр по атрибуту
 *    используется при фильтрации справочников, табелей, журналов и прочих  ИО, которые являются контейнерными.
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSFilter_h)
#define __KKSSITOOOM_KKSFilter_h

#include "KKSValue.h"
#include "KKSList.h"
#include "KKSData.h"
#include "KKSRecord.h"
#include "kkspp_config.h"

class KKSAttribute;

class _PP_EXPORT KKSFilter : public KKSRecord//KKSData
{
public:
    enum FilterOper{
        foEq = 1, // =
        foGr = 2, // >
        foLess = 3, // <
        foGrEq = 4, // >=
        foLessEq = 5, // <=
        foIn = 6, // in ()
        foNotIn = 7, // not in ()
        foBetween = 8, // between %1 and %2
        foLike = 9, // like 'AAA'
        foNotEq = 10, // <>
        foIsNull = 11, //isnull
        foIsNotNull = 12, //is not null
        foInSQL = 13, // in (select ...)
        foLikeIn = 14, //LIKE '%AAA%'
        foLikeStart = 15, //LIKE 'AAA%'
        foLikeEnd = 16, //LIKE '%AAA'
    };

    KKSFilter();
    KKSFilter(const KKSAttribute * a, const KKSValue * v, KKSFilter::FilterOper op);
    KKSFilter(const KKSFilter & other);
    ~KKSFilter();


    void setAttribute(const KKSAttribute * a);
    void setValue(const KKSValue * value);
    void setOperation(KKSFilter::FilterOper oper);
    void addValue(const KKSValue * value);
    void setValues(const KKSList<const KKSValue *> & values);
    void removeValue(int index);
    void clearValues();

    const KKSAttribute  * attribute() const;
    //KKSAttribute  * attribute();
    const KKSValue * value(int index) const;
    //const KKSValue * value(int index);
    const KKSList<const KKSValue *> & values() const;
    KKSFilter::FilterOper operation() const;

    bool caseSensitive() const;
    void setCaseSensitive(bool cs);

    bool recursive() const;
    void setRecursive(bool r);

    bool isCorrect() const;
    QString constructForIn() const;

    bool operator== (const KKSFilter& anotherFilter) const;
    bool operator!= (const KKSFilter& anotherFilter) const;

protected:
private:
    const KKSAttribute* m_attribute;
    KKSList<const KKSValue *> m_values;
    FilterOper m_oper;
    bool m_caseSensitive;
    bool m_recursive;
};

Q_DECLARE_METATYPE ( KKSFilter );

class _PP_EXPORT KKSFilterGroup : public KKSRecord//KKSData
{
public:
    KKSFilterGroup(bool AND = true);
    KKSFilterGroup(const KKSFilterGroup & other);
    ~KKSFilterGroup();
    
    KKSFilterGroup & operator = (const KKSFilterGroup & other);

    void setAND(bool _and = true);
    bool AND() const;
    
    void addFilter(const KKSFilter * f);
    void addFilters(const KKSList<const KKSFilter*> & filters);
    void setFilters(const KKSList<const KKSFilter*> & filters);
    void removeFilter(int index);
    void removeFilters(int attrId);
    

    void addGroup(const KKSFilterGroup * g);
    void addGroups(const KKSList<const KKSFilterGroup*> & groups);
    void setGroups(const KKSList<const KKSFilterGroup*> & groups);
    void removeGroup(int index);

    void clear();
    void clearFilters();
    void clearGroups();

    const KKSFilter * filter(int index) const;
    const KKSList<const KKSFilter*> & filters() const;
    const KKSFilterGroup * group(int index) const;
    const KKSList<const KKSFilterGroup*> & groups() const;

private:
    bool m_AND;

    KKSList<const KKSFilter *> m_filters;
    KKSList<const KKSFilterGroup*> m_groups;
};

Q_DECLARE_METATYPE( KKSFilterGroup );

#endif

