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
    KKSHistogram(const QMap<int, double>& data=QMap<int, double>(), double xmin=0.0, double xmax=0.0, int n=-1);
    KKSHistogram(const KKSHistogram& orig);
    virtual ~KKSHistogram();
    
    const QMap<int, double>& getVec (void) const;
    void setVec (const QMap<int, double>& data);
    
    void setRange (double xmin, double xmax);
    double getXMin (void) const;
    double getXMax (void) const;
    
    int size (void) const;
    void setSize (int n);

private:
    QMap<int, double> dHist;
    double m_xmin;
    double m_xmax;
    int m_num;

};

Q_DECLARE_METATYPE (const KKSHistogram*);

#endif	/* KKSHISTOGRAM_H */

