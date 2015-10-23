#ifndef _SearchResultsForm_H
#define _SearchResultsForm_H

#include <QWidget>
#include <QImage>

class QTreeView;
class QAbstractItemModel;
class QLabel;
class QToolBar;
class QLineEdit;

class SearchResultsForm : public QWidget
{
public:
    SearchResultsForm (const QImage& im, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~SearchResultsForm (void);

    void setSearchImage (const QImage& sIm);
    void setResultsModel (QAbstractItemModel * mod);

private:
    //
    // Functions
    //
    void init (void);

private:
    //
    // Variables
    //
    QTreeView * tView;
    QLabel * lImage;
    QToolBar * tbActions;
    QLabel * lFilter;
    QLineEdit * filterLE;
    QImage sImage;

private:
    Q_OBJECT
};

#endif
