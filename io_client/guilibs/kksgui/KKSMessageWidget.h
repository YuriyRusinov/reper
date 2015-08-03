/***********************************************************************
 * Module:  KKSMessageWidget.h
 * Author:  yuriyrusinov
 * Modified: 30 апреля 2009 г. 12:30:05
 * Purpose: Declaration of the class KKSMessageWIdget
 * Comment: Данный класс унаследован от QDialog и предназначен для 
 * написания сообщения
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSMessageWidget_h)
#define __KKSSITOOOM_KKSMessageWidget_h

#include "kksgui_config.h"

#include <QWidget>
#include <QList>
#include <QDialog>

class QPushButton;
class QTextEdit;
class QAbstractItemModel;

class KKSRecWidget;

class _GUI_EXPORT KKSMessageWidget : public QDialog
{
    public:
        KKSMessageWidget (KKSRecWidget *rp, KKSRecWidget *rio, QWidget *parent=0);
        ~KKSMessageWidget (void);

        QList<int> getDlsList (void) const;
        QString message (void) const;
        QList<int> getAttachments (void) const;
        QAbstractItemModel *getAttachmentsModel (void) const;

    signals:
        void addDocument (void);

    private slots:
        void addAttachment (void);
        void delAttachment (void);
        void accept (void);

    private:
        //
        // Functions
        //
        void init_widgets (void);
        QList<int> getIDList (KKSRecWidget *rw) const;

    private:
        //
        // Variables
        //
        friend class KKSObjEditorFactory;

        KKSRecWidget * recWPos;
        KKSRecWidget * recWIOs;
        QTextEdit *teMessage;
        QPushButton *pbOk;
        QPushButton *pbCancel;

    private:
        Q_OBJECT
};
#endif
