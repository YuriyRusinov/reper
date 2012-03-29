#ifndef _CommandForm_H
#define _CommandForm_H

#include <kksgui_config.h>
#include <QDialog>
#include <QMap>
#include <QString>
#include "KKSList.h"

class QDateTime;
class QAbstractItemModel;

class KKSCategory;
class KKSObjectExemplar;
class KKSAttrValue;

namespace Ui
{
    class command_widget;
};

class _GUI_EXPORT CommandForm : public QDialog
{
public:
    CommandForm (const QMap<int, QString>& positions, //=(QMap<int, QString>()), 
                 const QMap<int, QString>& urgencyLevels, //=(QMap<int, QString>()), 
                 bool isCreate=true, 
                 QWidget *parent=0, 
                 Qt::WFlags f=0);
    ~CommandForm (void);

    KKSCategory * getCategory (void) const;
    void setCategory (KKSCategory * cat);

    KKSObjectExemplar * getCommand (void) const;
    void setCommand (KKSObjectExemplar * ceio);

    QDateTime getInsertDateTime (void) const;
    void setInsertDateTime (const QDateTime& dt);

    KKSAttrValue * getController (void) const;
    void setController (const QString& dl_name);

    KKSAttrValue * getInputNumber (void) const;
    void setInputNumber (const QString& inputNumber);

    KKSAttrValue * getOutputNumber (void) const;
    void setOutputNumber (const QString& outputNumber);

    QDateTime getDeadline (void) const;
    void setDeadLineT (const QDateTime& dt);

    QString getPeriod (void) const;
    void setPeriod (int value, QString unitText);

    KKSAttrValue * getStatus (void) const;
    void setStatus (QString status);

    KKSAttrValue * getSender (void) const;
    void setSender (QString sender_name);

    QString getCommandMessage (void) const;
    void setCommandMessage (QString text);

    const KKSList<KKSAttrValue *>& attrValues() const;
    KKSList<KKSAttrValue *>& attrValues (void);
    void setValues (const KKSList<KKSAttrValue *>& vals);

    QList<int> getExecutors (void) const;
    void setExecutors (const QMap<int, QString>& exList);
    void selectExecutor (int idEx);
    void setUrgency (int idUrgency);
    void setAttachment (QString attName);
    void setReceiveDateTime (const QDateTime& dtRecv);
    void setAcceptDateTime (const QDateTime& dtAcc);

public slots:
    void constructCommand (void);

signals:
    void loadReference (QString tableName, QWidget * valW, int attrId);
    void loadExecutors (QAbstractItemModel * exModel);
    void addExecutor (QString tableName, QAbstractItemModel * exModel, int idAttr);

private slots:
    void loadCategory (void);
    void loadController (void);
    void loadExecutor (void);
    void periodCheckStateChanged (int state);
    void setDeadLine (const QDateTime& datetime);
    void periodTimeChanged (const QString & text);
    void periodUnitChanged (int index);
    void setCommandMess (void);
    void urgencyChanged (int index);
    void loadDocument (void);
    void addMailList (void);
    void setMailList (void);
    void clearMailList (void);

private:
    //
    // Functions
    //
    void initExecutors (const QMap<int, QString>& pos);
    void clearExecutors (void);

private:
    //
    // Variables
    //
    Ui::command_widget *UI;
    KKSCategory * commandCat;
    KKSObjectExemplar * commandEIO;
    KKSList<KKSAttrValue *> attrVals;

private:
    Q_OBJECT
};

#endif
