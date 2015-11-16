#ifndef _SearchRadioImageCalc_H
#define _SearchRadioImageCalc_H

#include <QObject>
#include <QImage>

class QAbstractItemModel;

class SearchRadioImageFragmentForm;
class SearchResultsForm;

class SearchRadioImageCalc : public QObject
{
public:
    SearchRadioImageCalc (QObject * parent=0);
    virtual ~SearchRadioImageCalc (void);

    SearchRadioImageFragmentForm * GUIImageView (const QImage& im=QImage(), QWidget * parent=0, Qt::WindowFlags flags=0);
    SearchResultsForm * GUIResultsView (QWidget * parent=0, Qt::WindowFlags flags=0);
    
private slots:
    void calculateParameters (const QImage& im, double cVal);
    void calcChi2 (QAbstractItemModel * sModel, const QImage& sIm);

signals:
    void setVals (int pl, int pw, double az);

private:
    //
    // Variables
    //
    QImage searchImage;

private:
    Q_OBJECT
};

#endif
