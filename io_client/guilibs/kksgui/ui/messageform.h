#ifndef _MessageForm_H
#define _MessageForm_H

#include <kksgui_config.h>
#include <QDialog>
#include <QMap>

#include "KKSList.h"

class QDateTime;
class QAbstractItemModel;

class KKSCategory;
class KKSObjectExemplar;
class KKSAttrValue;
class KKSCategoryAttr;

namespace Ui
{
    class message_widget;
};

class _GUI_EXPORT MessageForm : public QDialog
{
public:
    MessageForm (const QMap<int, QString> & positions, //=(QMap<int, QString>()), 
                 const QMap<int, QString>& urgencyLevels, //=(QMap<int, QString>()), 
                 bool isCreate=true, 
                 QWidget *parent=0, 
                 Qt::WFlags f=0);
    virtual ~MessageForm (void);

    KKSObjectExemplar * getMessage (void) const;
    void setMessage (KKSObjectExemplar * meio);

    KKSAttrValue * getInputNumber (void) const;
    void setInputNumber (const QString& inputNumber);

    KKSAttrValue * getOutputNumber (void) const;
    void setOutputNumber (const QString& outputNumber);

    const KKSList<KKSAttrValue *>& attrValues() const;
    KKSList<KKSAttrValue *>& attrValues (void);
    void setValues (const KKSList<KKSAttrValue *>& vals);

    void setDocumentName (const QString& ioName);
    void setSenderName (const QString& senderName);
    void setMessageBody(const QString& messBody);

    void setReadTime (const QDateTime& datetime);
    void setInsertTime (const QDateTime& datetime);
    void setSentTime (const QDateTime& datetime);
    void setReceiveTime (const QDateTime& datetime);

    void selectReceiver (int idRecv);
    void setUrgency (int idUrgency);

    QList<int> getReceivers (void) const;
    void setReceivers (const QMap<int, QString>& exList);

public slots:
    void constructMessage (void);

signals:
    void loadReference (QString tableName, QWidget * valW, int attrId);
    void loadReceivers (QAbstractItemModel * mailModel);
    void addReceiver (QString tableName, QAbstractItemModel * exModel, int idAttr);
private slots:
    void loadDocument (void);
    void loadReceiver (void);
    void setMessageBody (void);
    void setSendTime (const QDateTime& datetime);
    void urgencyChanged (int index);
    void addMailList (void);
    void setMailList (void);
    void clearMailList (void);

private:
    //
    // Functions
    //
    void initReceivers (const QMap<int, QString>& pos);
    void clearReceivers (void);

private:
    Ui::message_widget * UI;
    KKSObjectExemplar * messageEIO;
    KKSList<KKSAttrValue *> attrVals;

private:
    Q_OBJECT
};

#endif
