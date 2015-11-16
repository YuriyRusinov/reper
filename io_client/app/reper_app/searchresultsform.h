#ifndef _SearchResultsForm_H
#define _SearchResultsForm_H

#include <QWidget>
#include <QImage>

class QTreeView;
class QAbstractItemModel;
class QLabel;
class QToolBar;
class QLineEdit;
class QGroupBox;

class SearchResultsForm : public QWidget
{
public:
    SearchResultsForm (const QImage& im, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~SearchResultsForm (void);

    void setSearchImage (const QImage& sIm);
    void setResultsModel (QAbstractItemModel * mod);

private slots:
    void filterRecs (const QString& text);
    void calcChi2 (void);

private:
    //
    // Functions
    //
    void init (void);

signals:
    void calcGoodnessOfFit (QAbstractItemModel * sModel, const QImage& sIm);

private:
    //
    // Variables
    //
    QImage sImage;
    QTreeView * tView;
    QLabel * lImage;
    QToolBar * tbActions;
    QLabel * lFilter;
    QLineEdit * filterLE;
    QGroupBox * gbFilter;

private:
    Q_OBJECT
};

#endif
