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

class KKSCategory;
class KKSObject;
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
    
    const KKSMap<qint64, const KKSCategory *>& category (void) const;
    void setCategory (const KKSMap<qint64, const KKSCategory *>& cat);
    
    const KKSList<const KKSObject *>& srcObject (void) const;
    void setSrcObject (const KKSList<const KKSObject *>& iosrc);
    

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
    QList<int> scList;//idScenario;
    QList<int> varList;//idVariant;
    KKSMap<qint64, const KKSCategory *> catList;
    KKSList<const KKSObject *> ioList;
    //QList<int> sourceList;// idSource;
    //QList<int> recvList;// idReceiver;
    

};

Q_DECLARE_METATYPE (KKSHistogram);

#endif	/* KKSHISTOGRAM_H */

