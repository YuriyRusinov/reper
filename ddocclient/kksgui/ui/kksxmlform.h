/***********************************************************************
 * Module:  kksxmlform.h
 * Author:  yuriyrusinov
 * Modified: 10 марта 2009 г.
 * Purpose: Declaration of the class KKSXMLForm
 * Comment: Данный класс унаследован от QDialog и предназначен для задания
 * параметров импорта/экспорта ИО.
 ***********************************************************************/

#ifndef _XML_FORM_H
#define _XML_FORM_H

#include <QDialog>
#include <QXmlStreamEntityDeclaration>
#include <QVector>
#include <QList>
#include <QStringList>

#include <kksgui_config.h>

#include <KKSList.h>
#include <KKSMap.h>

namespace Ui
{
    class XML_form;
};

struct AttrField
{
    QString code;
    QString name;
    int idType;

    AttrField (QString codeStr = QString(), QString nameStr = QString(), int type = -1) : code (codeStr), name(nameStr), idType (type) {};
};

typedef QList<AttrField> AttrFields;

class QIODevice;
class QAbstractItemModel;

class KKSCategory;
class KKSObject;
class KKSObjectExemplar;
class KKSEIOData;

class _GUI_EXPORT KKSXMLForm : public QDialog
{
public:
    KKSXMLForm (KKSObject *obj, QString formTitle=QString(), bool for_export=false, QWidget *parent=0);
    ~KKSXMLForm (void);

    void setImport (bool value);

    QString getCSVFile (void) const;
    QString getXMLFile (void) const;
    QString getCharset (void) const;
    QString getFieldDelimiter (void) const;
    QString getTextDelimiter (void) const;
    AttrFields getAttributes (void) const;
    KKSCategory *getCategory (void) const;
    KKSObject *getIO (void) const;

    void clearCopies (void);
    void addCopy (const QStringList& oeStr);
    const QList<QStringList>& getCopies (void) const;
    //void clearAttrCodes (void);
    //void addAttrCode (const QString& aCode);
    const QStringList& getAttrCodeList (void) const;
    QAbstractItemModel *getPreliminaryData (void) const;

    int initExportData (const KKSMap<qint64, KKSEIOData *>& ioData, const KKSCategory *c);
    void setExportModel (QAbstractItemModel * recModel);

signals:
    //void loadAttributes (KKSCategory * c, const QStringList& attrCodeList);
    void loadCSV (QIODevice *csvDev, QString codeName, QString fDelim, QString tDelim, QAbstractItemModel *dataModel, KKSXMLForm * xForm);

private slots:
    void uploadFile (void);
    void uploadHFile (void);
    void viewParameters (void);
    void setCharset (const QString& text);
    void setFieldDelim (const QString& text);
    void setTextDelim (const QString& text);

private:
    //
    // Functions
    //
    void xmlParse (void);
    void readNotation (const QVector<QXmlStreamEntityDeclaration>& entityDecls);
    void readAttributes (QXmlStreamReader* reader, QList<AttrField>& aFields);
    void readField (QXmlStreamReader* reader, AttrField& af);
    void readDataFile (void);
    KKSCategory * readCategory (QXmlStreamReader* reader);
    void readAttribute (QXmlStreamReader* reader, KKSCategory * cat);

private:
    //
    // Variables
    //
    Ui::XML_form *ui;
    QString csvFileName;
    QString xmlFileName;
    QString charSet;
    QString fieldDelim;
    QString textDelim;

    QList<AttrField> attrFields;
    KKSCategory * c;
    KKSObject * io;
    //KKSList <KKSObjectExemplar*> objCopies;
    QList<QStringList> oeStrings;
    QStringList attrCodes;
    bool isExport;

private:
    Q_OBJECT
};

#endif
