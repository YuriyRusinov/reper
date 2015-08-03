/* 
 * File:   KKSHistogram.h
 * Author: yuriyrusinov
 *
 * Created on 12 Декабрь 2013 г., 13:12
 */

#ifndef KKSHISTOGRAM_H
#define	KKSHISTOGRAM_H

#include <QMetaType>
#include <QMap>
#include <QPair>
#include <QList>

#include "KKSRecord.h"
#include "KKSList.h"
#include "KKSMap.h"
#include "kkspp_config.h"
#include <KKSCategory.h>
#include <KKSObject.h>
//class KKSCategory;
//class KKSObject;
class KKSObjectExemplar;

class _PP_EXPORT KKSHistogram : public KKSRecord
{
public:
    KKSHistogram(const QMap<int, QPair<double, double> > & data, double xmin, double xmax, int n);
    KKSHistogram();
    KKSHistogram(const KKSHistogram& orig);
    virtual ~KKSHistogram();
    
    const QMap<int, QPair<double, double> > & getVec (void) const;
    void setVec (const QMap<int, QPair<double, double> > & data);
    
    void clear (void);
    void setValue (int key, QPair<double, double> val);
    
    void setRange (double xmin, double xmax);
    double getXMin (void) const;
    double getXMax (void) const;
    
    int size (void) const;
    void setSize (int n);

    //ksa
    QString toString() const;
    bool fromString(const QString & str);

    bool isEmpty() const;
    
    const QList<int>& getScenario (void) const;
    void setScenario (const QList<int>& idSc);
    
    const QList<int>& getVariant (void) const;
    void setVariant (const QList<int>& idv);
    
    const QList<int>& getCategories(void) const;
    void setCategories (const QList<int>& idCat);
    
    const QList<int>& getIO(void) const;
    void setIO (const QList<int>& idIO);

    const QList<int>& getServices(void) const;
    void setServices(const QList<int>& idServ);

    const QList<int>& getPartLows(void) const;
    void setPartLows (const QList<int>& idPartLows);

private:
    //
    // Variables
    //
    QMap<int, QPair<double, double> > dHist;
    double m_xmin;
    double m_xmax;
    int m_num;

    bool m_isEmpty;
    //
    // filter parameters
    //
    QList<int> scList;
    QList<int> vList;
    QList<int> cList;
    QList<int> ioList;
    QList<int> sList;
    QList<int> plList;

};

Q_DECLARE_METATYPE (KKSHistogram);

#endif	/* KKSHISTOGRAM_H */

