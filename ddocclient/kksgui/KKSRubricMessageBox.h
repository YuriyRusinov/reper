/* 
 * File:   KKSRubricMessageBox.h
 * Author: yuriyrusinov
 *
 * Created on 20 Ноябрь 2012 г., 17:11
 */

#ifndef KKSRUBRICMESSAGEBOX_H
#define	KKSRUBRICMESSAGEBOX_H

#include <QDialog>
#include <QMessageBox>

class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QCheckBox;
class QLabel;

class KKSRubricMessageBox : public QDialog
{
public:
    KKSRubricMessageBox(QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~KKSRubricMessageBox();

    Qt::CheckState getDbState (void) const;
private:
    //
    // Functions
    //
    void init (void);
    
private:
    //
    // Variables
    //
    QVBoxLayout * vLay;
    QHBoxLayout * hLabelLay;
    QHBoxLayout * hButtonsLay;
    QLabel * lText;
    QPushButton * pbYes;
    QPushButton * pbNo;
    QCheckBox * chDb;

private:
    Q_OBJECT
};

#endif	/* KKSRUBRICMESSAGEBOX_H */

