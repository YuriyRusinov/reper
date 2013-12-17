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

#include "kkspp_config.h"

class _PP_EXPORT KKSHistogram
{
public:
    KKSHistogram(const QMap<int, double>& data, double xmin, double xmax, int n);
    KKSHistogram();
    KKSHistogram(const KKSHistogram& orig);
    virtual ~KKSHistogram();
    
    const QMap<int, double>& getVec (void) const;
    void setVec (const QMap<int, double>& data);
    
    void setRange (double xmin, double xmax);
    double getXMin (void) const;
    double getXMax (void) const;
    
    int size (void) const;
    void setSize (int n);

    //ksa
    QString toString() const;
    bool fromString(const QString & str);

    bool isEmpty() const;

private:
    QMap<int, double> dHist;
    double m_xmin;
    double m_xmax;
    int m_num;

    bool m_isEmpty;

};

Q_DECLARE_METATYPE (KKSHistogram);

#endif	/* KKSHISTOGRAM_H */

