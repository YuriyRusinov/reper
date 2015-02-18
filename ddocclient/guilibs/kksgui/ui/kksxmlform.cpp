#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QXmlStreamNotationDeclaration>
#include <QXmlStreamAttributes>
#include <QXmlStreamAttribute>
#include <QIODevice>
#include <QMessageBox>
#include <QStringRef>
#include <QStringList>
#include <QList>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QStringList>
#include <QtDebug>

#include <KKSCategory.h>
#include <KKSObject.h>
#include <KKSAttribute.h>
#include <KKSAttrType.h>
#include <KKSAttribute.h>
#include <KKSAttrValue.h>
#include <KKSObjectExemplar.h>
#include <KKSEIOData.h>
#include <KKSTemplate.h>
#include <KKSType.h>
#include "KKSItemDelegate.h"
#include "defines.h"

#include "kksxmlform.h"
#include "ui_kksxml_form.h"

/*
 * Класс KKSXMLForm: форма импорта-экспорта
 */

KKSXMLForm :: KKSXMLForm (KKSObject *obj, QString formTitle, bool for_export, QWidget *parent)
    : QDialog (parent),
    xmlFileName (QString()),
    c (0),
    io (obj),
    isExport (for_export)
{
    ui = new Ui::XML_form;
    ui->setupUi (this);

    if (!formTitle.isEmpty())
        this->setWindowTitle (formTitle);

    if (io)
        io->addRef ();

    KKSItemDelegate *itemDeleg = new KKSItemDelegate (this);
    ui->tVPreview->setItemDelegate (itemDeleg);

    ui->cbCharset->setEnabled (isExport);
    ui->lEFieldDelimiter->setReadOnly (!isExport);
    ui->lETextDelimiter->setReadOnly (!isExport);
    ui->gbPars->setVisible (false);
    if (isExport)
    {
        ui->cbCharset->setCurrentIndex (0);//Text (QString("UTF-8"));
        charSet = ui->cbCharset->currentText ();
        ui->lEFieldDelimiter->setText (",");
        fieldDelim = ui->lEFieldDelimiter->text ();
        ui->lETextDelimiter->setText ("\"");
        textDelim = ui->lETextDelimiter->text ();
    }

    connect (ui->tbFile, SIGNAL (clicked()), this, SLOT (uploadFile()) );
    //connect (ui->tbHFile, SIGNAL (clicked()), this, SLOT (uploadHFile()) );
    connect (ui->pbPars, SIGNAL (clicked()), this, SLOT (viewParameters()) );

    connect (ui->cbCharset, SIGNAL (currentIndexChanged (const QString &)), this, SLOT (setCharset (const QString&)) );
    connect (ui->lEFieldDelimiter, SIGNAL (textChanged (const QString &)), this, SLOT (setFieldDelim (const QString&)) );
    connect (ui->lETextDelimiter, SIGNAL (textChanged (const QString &)), this, SLOT (setTextDelim (const QString&)) );
    connect (ui->pbOK, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (ui->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

KKSXMLForm :: ~KKSXMLForm (void)
{
    if (c)
        c->release ();

    if (io)
        io->release ();

    delete ui;
}

void KKSXMLForm :: setImport (bool value)
{
    ui->lEFileName->setReadOnly (value);
}

void KKSXMLForm :: uploadFile (void)
{
    QString fName;
    QFileDialog *xmlDialog = new QFileDialog (this, isExport ? tr ("Save file") : tr ("Open file"), QDir::currentPath());
    QStringList filters;
    filters << tr ("XML files (*.xml)")
            << tr ("All files (*.*)");

    xmlDialog->setFilters (filters);
    xmlDialog->setModal (true);
    xmlDialog->setDefaultSuffix ("xml");
    if (isExport)
    {
        xmlDialog->setFileMode (QFileDialog::AnyFile);
        xmlDialog->setAcceptMode (QFileDialog::AcceptSave);
    }
    else
    {
        xmlDialog->setFileMode (QFileDialog::ExistingFile);
        xmlDialog->setAcceptMode (QFileDialog::AcceptOpen);
    }

    if (xmlDialog->exec () == QDialog::Accepted)
    {
        QStringList files = xmlDialog->selectedFiles ();
        if (!files.isEmpty())
            fName = files[0];
    }
    qDebug () << __PRETTY_FUNCTION__ << fName;

    xmlDialog->setParent (0);
    delete xmlDialog;
    if (fName.isEmpty())
        return;

    xmlFileName = fName;
    ui->lEFileName->setText (xmlFileName);

    if (!isExport)
    {
        this->xmlParse ();
    }
    //readDataFile ();

}
/*
void KKSXMLForm :: uploadHFile (void)
{
    QString fXmlName;
    QFileDialog *xmlDialog = new QFileDialog (this, isExport ? tr ("Save header file") : tr ("Open header file"), QDir::currentPath());
    QStringList filters;
    filters << tr ("XML files (*.xml)")
            << tr ("All files (*.*)");

    xmlDialog->setFilters (filters);
    xmlDialog->setModal (true);
    xmlDialog->setDefaultSuffix ("xml");
    if (isExport)
    {
        xmlDialog->setFileMode (QFileDialog::AnyFile);
        xmlDialog->setAcceptMode (QFileDialog::AcceptSave);
    }
    else
    {
        xmlDialog->setFileMode (QFileDialog::ExistingFile);
        xmlDialog->setAcceptMode (QFileDialog::AcceptOpen);
    }

    if (xmlDialog->exec () == QDialog::Accepted)
    {
        QStringList files = xmlDialog->selectedFiles ();
        if (!files.isEmpty())
            fXmlName = files[0];
    }

    xmlDialog->setParent (0);
    delete xmlDialog;
    if (fXmlName.isEmpty())
        return;

    xmlFileName = fXmlName;
    ui->lEHFileName->setText (xmlFileName);
    if (!isExport)
        this->xmlParse ();
    if (!csvFileName.isEmpty())
        readDataFile ();
}
 */

void KKSXMLForm :: xmlParse (void)
{
    QFile *xmlFile = new QFile (xmlFileName);
    if (!xmlFile->exists())
        return;

    ui->lEErrorString->clear ();
    xmlFile->open (QIODevice::ReadOnly);
    QXmlStreamReader * xmlStreamR = new QXmlStreamReader(xmlFile);
    QString xmlHeaderStr;
    QXmlStreamWriter * xmlStreamW = new QXmlStreamWriter (&xmlHeaderStr);

    while (!xmlStreamR->atEnd())
    {
        QXmlStreamReader::TokenType tType = xmlStreamR->readNext ();
        if (!xmlStreamR->hasError())
        {
            xmlStreamW->writeCurrentToken (*xmlStreamR);
            qDebug () << __PRETTY_FUNCTION__ << tType << xmlStreamR->name().toString() << xmlStreamR->text ().toString();
            if (tType == QXmlStreamReader::Invalid)
            {
                qDebug() << __PRETTY_FUNCTION__ << QString ("Invalid token");
                break;
            }
            if (xmlStreamR->isDTD())
                readNotation (xmlStreamR->entityDeclarations ());
            else if (xmlStreamR->isStartElement () &&
                    ((xmlStreamR->name ().toString().compare ("Reference", Qt::CaseInsensitive) == 0) ||
                    (xmlStreamR->name().toString().compare("header", Qt::CaseInsensitive) == 0))
                    )
                continue;
            else if (xmlStreamR->isStartElement () && xmlStreamR->name ().toString().compare ("category",  Qt::CaseInsensitive) == 0)
            {
                KKSCategory * pCat = readCategory (xmlStreamR);
                if (pCat && pCat->type()->id() != 10)
                {
                    if (c)
                        c->release ();
                    c = new KKSCategory (*pCat);
                    c->setAttributes (pCat->attributes());
                }
                else if (pCat)
                {
                    c->setTableCategory (pCat);
                    //qDebug () << __PRETTY_FUNCTION__ << pCat->attributes().count();
                }
                if (pCat)
                    pCat->release ();
            }
            else if (xmlStreamR->isStartElement() && xmlStreamR->name().toString().compare ("body", Qt::CaseInsensitive) == 0)
            {
                qDebug () << __PRETTY_FUNCTION__ << "body";
                this->readData (xmlStreamR);
            }
            else if (xmlStreamR->isEndDocument())
            {
                qDebug () << __PRETTY_FUNCTION__ << xmlStreamR->name ().toString() << tType;
            }
//            else if (xmlStreamR->isStartElement () && xmlStreamR->name ().toString().compare ("Attributes", Qt::CaseInsensitive) == 0)
//                readAttributes (xmlStreamR, attrFields);
        }
        else
        {
            qCritical() << tr ("Error is %1, line is %2, column %3 ").arg (xmlStreamR->errorString()).arg (xmlStreamR->lineNumber()).arg (xmlStreamR->columnNumber());
            QMessageBox::critical (this, tr("XML header error"), tr ("Error is %1, line is %2, column %3 ").arg (xmlStreamR->errorString()).arg (xmlStreamR->lineNumber()).arg (xmlStreamR->columnNumber()), QMessageBox::Ok);
            ui->lEFileName->clear ();
            //ui->lEHFileName->clear ();
            ui->lEErrorString->setText (tr("File : %1, error is %2, line is %3, column is %4").arg (xmlFileName).arg (xmlStreamR->errorString()).arg (xmlStreamR->lineNumber()).arg (xmlStreamR->columnNumber()));
            return;
        }
    }

    if (!c || !c->tableCategory())
        return;

    int nAttrs = c->tableCategory()->attributes().count();
    QStandardItemModel *pModel = new QStandardItemModel (0, nAttrs+1);
    QAbstractItemModel *oldModel = ui->tVPreview->model ();
    ui->tVPreview->setModel (pModel);
    if (oldModel)
        delete oldModel;

//    if (c)
//        c->release ();
/*
    if (io && io->category() && io->category()->tableCategory())
        c = new KKSCategory (*io->category()->tableCategory());
    else
        c = new KKSCategory ();
    QStringList codeList;
    attrCodes.clear ();
*/
    int i(1);
    pModel->setHeaderData (0, Qt::Horizontal, tr("Unique id"), Qt::DisplayRole);
    pModel->setHeaderData (0, Qt::Horizontal, QString("unique_id"), Qt::UserRole);
    for (KKSMap<int, KKSCategoryAttr*>::const_iterator p=c->tableCategory()->attributes().constBegin(); 
            p != c->tableCategory()->attributes().constEnd(); 
            p++)
    {
//        codeList.append (attrFields[i].code);
//        attrCodes.append (attrFields[i].code);
        KKSCategoryAttr * ca = p.value();
        pModel->setHeaderData (i, Qt::Horizontal, (ca->name().isEmpty() ? ca->code() : ca->name()), Qt::DisplayRole);
        pModel->setHeaderData (i, Qt::Horizontal, ca->code(), Qt::UserRole);
        ++i;
    }

    xmlFile->close ();
}

KKSCategory * KKSXMLForm :: readCategory (QXmlStreamReader* reader)
{
    if (!reader->isStartElement () || reader->name ().toString().compare ("Category", Qt::CaseInsensitive) != 0)
        return 0;

    QXmlStreamAttributes cXmlAttrs = reader->attributes();
    if (cXmlAttrs.isEmpty())
        return 0;

    QString cIdStr (cXmlAttrs[0].value().toString());
    int idCat = cIdStr.mid(1, cIdStr.length()).toInt();
    qDebug () << __PRETTY_FUNCTION__ << cIdStr << idCat;

    KKSCategory * cat = new KKSCategory ();
    QXmlStreamReader::TokenType tType;// = reader->readNext ();
    for (tType = reader->readNext(); tType != QXmlStreamReader::EndElement && reader->name ().toString().compare ("Category", Qt::CaseInsensitive) != 0 && !reader->atEnd(); tType = reader->readNext())
    {
        if (tType == QXmlStreamReader::StartElement && reader->name ().toString().compare ("cname", Qt::CaseInsensitive) == 0)
        {
            QString cName = reader->readElementText();
            cat->setName (cName);
        }
        else if (tType == QXmlStreamReader::StartElement && reader->name ().toString().compare ("ccode", Qt::CaseInsensitive) == 0)
        {
            QString cCode = reader->readElementText();
            cat->setCode (cCode);
        }
        else if (tType == QXmlStreamReader::StartElement && reader->name ().toString().compare ("ctype", Qt::CaseInsensitive) == 0)
        {
            int idType = reader->readElementText().toInt();
            //qDebug () << __PRETTY_FUNCTION__ << idType;
            if (idType != 10)
                cat->setType (KKSType::defType1());
            else
                cat->setType (KKSType::createType10());
        }
        else if (tType == QXmlStreamReader::StartElement && reader->name ().toString().compare ("cdescription", Qt::CaseInsensitive) == 0)
        {
            QString cDesc = reader->readElementText();
            cat->setDesc (cDesc);
        }
        else if (tType == QXmlStreamReader::StartElement && reader->name ().toString().compare ("cis_main", Qt::CaseInsensitive) == 0)
        {
            QString cMain = reader->readElementText();
            if (QString::compare (cMain, QString("true"), Qt::CaseInsensitive)==0)
                cat->setMain (true);
            else
                cat->setMain (false);
        }
        else if (tType == QXmlStreamReader::StartElement && reader->name ().toString().compare ("attributes", Qt::CaseInsensitive) == 0)
        {
            QXmlStreamAttributes cXmlNumAttrs = reader->attributes();
            int nAttrs = cXmlNumAttrs.isEmpty() ? 0 : cXmlNumAttrs[0].value().toString().toInt();
            if (nAttrs > 0)
            {
                //qDebug () << __PRETTY_FUNCTION__ << nAttrs;
                QXmlStreamReader::TokenType tTypeA;
                for (tTypeA = reader->readNext (); tTypeA != QXmlStreamReader::StartElement && reader->name ().toString().compare ("attribute", Qt::CaseInsensitive) != 0;tTypeA = reader->readNext ())
                    ;
                for (int i=0; i<nAttrs; i++)
                {
                    //qDebug () << __PRETTY_FUNCTION__ << reader->name ().toString() << tType;
                    QXmlStreamReader::TokenType tTypeA;
                    readAttribute (reader, cat);
                    for (tTypeA = reader->readNext (); tTypeA != QXmlStreamReader::StartElement && reader->name ().toString().compare ("attribute", Qt::CaseInsensitive) != 0 && !reader->atEnd(); tTypeA = reader->readNext ())
                        ;//qDebug () << __PRETTY_FUNCTION__ << reader->name ().toString() << tType;
                }
                for (tTypeA = reader->readNext (); tTypeA != QXmlStreamReader::EndElement && reader->name ().toString().compare ("attributes", Qt::CaseInsensitive) != 0 && !reader->atEnd(); tTypeA = reader->readNext ())
                    ;
            }
        }
    }
    return cat;
}

void KKSXMLForm :: readAttribute (QXmlStreamReader* reader, KKSCategory * cat)
{
    if (!cat || !reader->isStartElement () || reader->name ().toString().compare ("attribute", Qt::CaseInsensitive) != 0)
    {
        qDebug () << __PRETTY_FUNCTION__ << reader->tokenString () << reader->name ().toString() << (reader->name ().toString().compare ("attribute", Qt::CaseInsensitive) );
        return;
    }

    QXmlStreamAttributes aXmlAttrs = reader->attributes();
    if (aXmlAttrs.isEmpty())
        return;

    QString aIdStr (aXmlAttrs[0].value().toString());
    int idAttr = aIdStr.mid(1, aIdStr.length()).toInt();
    qDebug () << __PRETTY_FUNCTION__ << aIdStr << idAttr;
    KKSCategoryAttr * cAttr = new KKSCategoryAttr;
    cAttr->setId (idAttr);
    QXmlStreamReader::TokenType tType;// = reader->readNext ();
    //qDebug () << __PRETTY_FUNCTION__ << tType << reader->name().toString();
    for (tType = reader->readNext (); tType != QXmlStreamReader::EndElement && reader->name ().toString().compare ("attribute", Qt::CaseInsensitive) != 0; tType=reader->readNext ())
    {
        qDebug () << __PRETTY_FUNCTION__ << tType << reader->name ().toString();
        if (tType == QXmlStreamReader::StartElement && reader->name ().toString().compare ("code", Qt::CaseInsensitive) == 0)
        {
            QString acCode = reader->readElementText();
            cAttr->setCode (acCode);
        }
        else if (tType == QXmlStreamReader::StartElement && reader->name ().toString().compare ("name", Qt::CaseInsensitive) == 0)
        {
            QString acName = reader->readElementText();
            cAttr->setName (acName);
        }
        else if (tType == QXmlStreamReader::StartElement && reader->name ().toString().compare ("type", Qt::CaseInsensitive) == 0)
        {
            QString acType = reader->readElementText();
            int idAType = acType.toInt();
            KKSAttrType * aType = new KKSAttrType (idAType);
            cAttr->setType (aType);
            aType->release ();
        }
        else if (tType == QXmlStreamReader::StartElement && reader->name ().toString().compare ("mandatory", Qt::CaseInsensitive) == 0)
        {
            QString isMandStr = reader->readElementText();
            bool isM = (QString::compare (isMandStr, QString("true"), Qt::CaseInsensitive)==0);
            cAttr->setMandatory (isM);
        }
        else if (tType == QXmlStreamReader::StartElement && reader->name ().toString().compare ("read_only", Qt::CaseInsensitive) == 0)
        {
            QString isROStr = reader->readElementText();
            bool isRO = (QString::compare (isROStr, QString("true"), Qt::CaseInsensitive)==0);
            cAttr->setReadOnly (isRO);
        }
        else if (tType == QXmlStreamReader::StartElement && reader->name ().toString().compare ("def_width", Qt::CaseInsensitive) == 0)
        {
            QString defWStr = reader->readElementText();
            int aDefW = defWStr.toInt();
            cAttr->setDefWidth (aDefW);
        }
        else if (tType == QXmlStreamReader::StartElement && reader->name ().toString().compare ("table_name", Qt::CaseInsensitive) == 0)
        {
            QString tName = reader->readElementText();
            cAttr->setTableName (tName);
        }
        else if (tType == QXmlStreamReader::StartElement && reader->name ().toString().compare ("column_name", Qt::CaseInsensitive) == 0)
        {
            QString cName = reader->readElementText();
            cAttr->setColumnName (cName);
        }
        else if (tType == QXmlStreamReader::StartElement && reader->name ().toString().compare ("ref_column_name", Qt::CaseInsensitive) == 0)
        {
            QString crName = reader->readElementText();
            cAttr->setRefColumnName (crName);
        }
        else if (tType == QXmlStreamReader::EndElement)
            qDebug() << __PRETTY_FUNCTION__ << reader->name ().toString();
        //tType = reader->readNext ();
    }
    //reader->readNext ();
    cat->addAttribute (idAttr, cAttr);
    qDebug () << __PRETTY_FUNCTION__ << idAttr << cat->attributes().count();
}

QString KKSXMLForm :: getXMLFile (void) const
{
    return xmlFileName;
}

QString KKSXMLForm :: getCharset (void) const
{
    return charSet;
}

QString KKSXMLForm :: getFieldDelimiter (void) const
{
    return fieldDelim;
}

QString KKSXMLForm :: getTextDelimiter (void) const
{
    return textDelim;
}

void KKSXMLForm :: readNotation (const QVector<QXmlStreamEntityDeclaration>& entityDecls)
{
    charSet = QString();
    fieldDelim = QString();
    textDelim = QString();
    for (int i=0; i<entityDecls.count() && (charSet.isEmpty() || fieldDelim.isEmpty() || textDelim.isEmpty()); i++)
    {
/*        qDebug () << __PRETTY_FUNCTION__ << entityDecls.at(i).name().toString()
                                         << entityDecls.at(i).notationName ().toString()
                                         << entityDecls.at(i).publicId().toString()
                                         << entityDecls.at(i).systemId().toString()
                                         << entityDecls.at(i).value().toString();
*/
        int isCharset = entityDecls.at(i).name().toString().compare("charset", Qt::CaseInsensitive);
        if (isCharset == 0)
        {
            charSet = entityDecls.at(i).value().toString();
            int charInd = ui->cbCharset->findText (charSet, Qt::MatchExactly);
            ui->cbCharset->setCurrentIndex (charInd);
        }
        int isFieldDelimiter = entityDecls.at(i).name().toString().compare("field_delimiter", Qt::CaseInsensitive);
        if (isFieldDelimiter == 0)
        {
            fieldDelim = entityDecls.at(i).value().toString();
            ui->lEFieldDelimiter->setText (fieldDelim);
        }
        int isTextDelimiter = entityDecls.at(i).name().toString().compare("text_delimiter", Qt::CaseInsensitive);
        if (isTextDelimiter == 0)
        {
            textDelim = entityDecls.at(i).value().toString();
            ui->lETextDelimiter->setText (textDelim);
        }
    }
}

void KKSXMLForm :: readAttributes (QXmlStreamReader* reader, QList<AttrField>& aFields)
{
    if (!reader->isStartElement () || reader->name ().toString().compare ("Attributes", Qt::CaseInsensitive) != 0)
        return;

    aFields.clear ();

    while (!reader->atEnd())
    {
        reader->readNext ();

        if (reader->isEndElement())
            break;

        AttrField af;

        if (reader->isStartElement())
        {
            if (reader->name ().toString().compare ("attribute", Qt::CaseInsensitive) == 0)
                readField (reader, af);
            else
                return;
            if (!af.code.isEmpty() && af.idType >= 0 )
                aFields.append (af);
        }
    }
}

void KKSXMLForm :: readField (QXmlStreamReader* reader, AttrField& af)
{
    if (!reader->isStartElement () || reader->name ().toString().compare ("attribute", Qt::CaseInsensitive) != 0)
        return;

    while (!reader->atEnd())
    {
        reader->readNext ();
        if (reader->isEndElement())
            break;

        if (reader->isStartElement())
        {
            if (reader->name ().toString().compare ("code", Qt::CaseInsensitive) == 0)
                af.code = reader->readElementText();
            else if (reader->name ().toString().compare ("name", Qt::CaseInsensitive) == 0)
                af.name = reader->readElementText();
            else if (reader->name ().toString().compare ("type", Qt::CaseInsensitive) == 0)
                af.idType = reader->readElementText().toInt();
        }
    }
}

AttrFields KKSXMLForm :: getAttributes (void) const
{
    return attrFields;
}

void KKSXMLForm :: readDataFile (void)
{
    if (xmlFileName.isEmpty() || charSet.isEmpty () || fieldDelim.isEmpty() || textDelim.isEmpty() || !ui->tVPreview->model ())
        return;
    QFile xmlFile (xmlFileName);
    if (!isExport)
        emit loadCSV (&xmlFile, charSet, fieldDelim, textDelim, ui->tVPreview->model (), this);
}

KKSCategory * KKSXMLForm :: getCategory (void) const
{
    return c;
}

KKSObject * KKSXMLForm :: getIO (void) const
{
    return io;
}

void KKSXMLForm :: clearCopies (void)
{
    oeStrings.clear ();
}

void KKSXMLForm :: addCopy (const QStringList& oeStr)
{
    if (oeStr.isEmpty())
        return;

    oeStrings.append (oeStr);
}

QAbstractItemModel * KKSXMLForm :: getPreliminaryData (void) const
{
    return ui->tVPreview->model ();
}

const QList<QStringList>& KKSXMLForm :: getCopies (void) const
{
    return oeStrings;
}

const QStringList& KKSXMLForm :: getAttrCodeList (void) const
{
    return attrCodes;
}

int KKSXMLForm :: initExportData (const KKSMap<qint64, KKSEIOData *>& ioData, const KKSCategory *c)
{
    if (!c || !isExport)
        return ERROR_CODE;

    QAbstractItemModel *pModel = 0;
    if (ui->tVPreview->model())
    {
        pModel = ui->tVPreview->model();
        if (pModel->rowCount())
            pModel->removeRows (0, pModel->rowCount());
        if (pModel->columnCount())
            pModel->removeColumns (0, pModel->columnCount());
        pModel->insertColumns (0, c->attributes().count());
    }
    else
    {
        pModel = new QStandardItemModel (0, c->attributes().count());
        ui->tVPreview->setModel (pModel);
    }

    int i=0;
    KKSTemplate *t = new KKSTemplate (c->defTemplate());
    if (!t)
        return ERROR_CODE;

    QList<KKSAttrView*> attrs_list = t->sortedAttrs();
    QStringList headers;
    for (KKSMap<qint64, KKSEIOData *>::const_iterator pio=ioData.constBegin(); pio != ioData.constEnd(); pio++)
    {
        pModel->insertRows (i, 1);
        QModelIndex wIndex = pModel->index (i, 0);
        pModel->setData (wIndex, pio.key(), Qt::UserRole);
        KKSEIOData *d = pio.value();

        for (int ii=0; ii<attrs_list.count(); ii++)
        {
            if (i==0)
                headers << attrs_list[ii]->title();

            KKSAttrView * v = attrs_list [ii];
            QModelIndex cIndex = pModel->index (i, ii);
            QString attrCode = v->code ();
            pModel->setData (cIndex, d->fields().value(attrCode), Qt::DisplayRole);
        }
        i++;
    }

    for (int j=0; j<headers.count(); j++)
        pModel->setHeaderData (j, Qt::Horizontal, headers[j], Qt::DisplayRole);

    t->release ();
    return OK_CODE;
}

void KKSXMLForm :: setExportModel (QAbstractItemModel * recModel)
{
    QAbstractItemModel * oldModel = ui->tVPreview->model ();
    ui->tVPreview->setModel (recModel);
    if (oldModel && oldModel != recModel)
        delete oldModel;
}

void KKSXMLForm :: viewParameters (void)
{
    bool isPars = !ui->gbPars->isVisible();
    ui->gbPars->setVisible (isPars);

    QString txt = isPars ? "<<<" : ">>>";
    QString old = ui->pbPars->text ();
    old.truncate (old.length() - 3);
    ui->pbPars->setText (old+txt);
}

void KKSXMLForm :: setCharset (const QString& text)
{
    charSet = text;
}

void KKSXMLForm :: setFieldDelim (const QString& text)
{
    fieldDelim = text;
}

void KKSXMLForm :: setTextDelim (const QString& text)
{
    textDelim = text;
}

void KKSXMLForm :: readData (QXmlStreamReader* reader)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (!reader)
        return;
}