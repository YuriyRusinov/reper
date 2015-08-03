#ifndef _SearchRadioForm_H
#define _SearchRadioForm_H

#include <QDialog>

class QLabel;

namespace Ui
{
    class Search_Radio_Form;
};

class SearchRadioForm : public QDialog
{
public:
    SearchRadioForm (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~SearchRadioForm (void);

    QImage getImage (void) const;
    void setImage (const QImage& im);

private slots:
    void loadImage (void);
    void clearImage (void);
    void calcPars (void);

private:
    //
    // Functions
    //
    void init (void);

private:
    //
    // Variables
    //
    QImage sIm;
    Ui::Search_Radio_Form * UI;
    QLabel * lImageP;
private:
    Q_OBJECT
};

#endif
