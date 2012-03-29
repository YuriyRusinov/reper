/***********************************************************************
 * Module: JKKSSearchCriterion.h
 * Author: yuriyrusinov
 * Modified: 08 ������� 2009 �. 13:18:00
 * Purpose: Declaration of the class JKKSSearchCriterion
 ***********************************************************************/

#if !defined(__KKSSITOOOM_JKKSSearchCriterion_h)
#define __KKSSITOOOM_JKKSSearchCriterion_h

#include "kksinteractor_config.h"

#include "JKKSCategoryAttr.h"

class QDataStream;
class QTextStream;

/*
 * ��� �������� ������
 */
class _I_EXPORT JKKSSearchCriterion
{
public:
    JKKSSearchCriterion (int idc = -1,
                         const JKKSCategoryAttr& attr = JKKSCategoryAttr(), \
                         QString val = QString(), \
                         int type = -1, \
                         int oper = -1, \
                         bool caseSensitive = true, \
                         bool isNot = false);
    JKKSSearchCriterion (const JKKSSearchCriterion& sc);
    ~JKKSSearchCriterion (void);

    int id (void) const;
    void setId (int idc);

    JKKSCategoryAttr getAttribute (void) const;
    void setAttribute (const JKKSCategoryAttr& attr);

    QString getValue (void) const;
    void setValue (QString val);

    int getType (void) const;
    void setType (int type);

    int getOperation (void) const;
    void setOperation (int op);

    bool isCaseSensitive (void) const;
    void setCaseSensitive (bool cs);

    bool isNegative (void) const;
    void setNegative (bool is_not);

private:
    //
    // Functions
    //
    friend QDataStream& operator<< (QDataStream& out, const JKKSSearchCriterion& sc);
    friend QDataStream& operator>> (QDataStream& in, JKKSSearchCriterion& sc);

private:
    //
    // Variables
    //
    int idC;
    JKKSCategoryAttr attribute;
    QString value;
    int criterionType;
    int criterionOperation;
    bool criterionCaseSensitive;
    bool criterionIsNot;
};

#endif
