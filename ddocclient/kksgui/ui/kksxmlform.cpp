#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QXmlStreamNotationDeclaration>
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
#include <KKSAttrValue.h>
#include <KKSObjectExemplar.h>
#include <KKSEIOData.h>
#include <KKSTemplate.h>
#include "KKSItemDelegate.h"
#include "defines.h"

#include "kksxmlform.h"
#include "ui_kksxml_form.h"

/*
 * Класс KKSXMLForm: форма импорта-экспорта
 */

KKSXMLForm :: KKSXMLForm (KKSObject *obj, QString formTitle, bool for_export, QWidget *parent)
    : QDialog (parent),
    csvFileName (QString()),
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
    connect (ui->tbHFile, SIGNAL (clicked()), this, SLOT (uploadHFile()) );
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
    QFileDialog *csvDialog = new QFileDialog (this, isExport ? tr ("Save file") : tr ("Open file"), QDir::currentPath());
    QStringList filters;
    filters << tr ("CSV files (*.csv)")
            << tr ("All files (*.*)");

    csvDialog->setFilters (filters);
    csvDialog->setModal (true);
    csvDialog->setDefaultSuffix ("csv");
    if (isExport)
    {
        csvDialog->setFileMode (QFileDialog::AnyFile);
        csvDialog->setAcceptMode (QFileDialog::AcceptSave);
    }
    else
    {
        csvDialog->setFileMode (QFileDialog::ExistingFile);
        csvDialog->setAcceptMode (QFileDialog::AcceptOpen);
    }

    if (csvDialog->exec () == QDialog::Accepted)
    {
        QStringList files = csvDialog->selectedFiles ();
        if (!files.isEmpty())
            fName = files[0];
    }

    csvDialog->setParent (0);
    delete csvDialog;
    if (fName.isEmpty())
        return;

    csvFileName = fName;
    ui->lEFileName->setText (csvFileName);

    if (!xmlFileName.isEmpty())
        readDataFile ();
}

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
        if (!xmlStreamR->error())
        {
            xmlStreamW->writeCurrentToken (*xmlStreamR);
            qDebug () << __PRETTY_FUNCTION__ << tType;// << xmlStreamR.name().toString() << xmlStreamR.text ().toString();
            if (xmlStreamR->isDTD())
                readNotation (xmlStreamR->entityDeclarations ());
            else if (xmlStreamR->isStartElement () && xmlStreamR->name ().toString().compare ("Attributes", Qt::CaseInsensitive) == 0)
                readAttributes (xmlStreamR, attrFields);
        }
        else
        {
            QMessageBox::warning (this, tr("XML header error"), tr ("Error is %1, line is %2, column %3 ").arg (xmlStreamR->errorString()).arg (xmlStreamR->lineNumber()).arg (xmlStreamR->columnNumber()), QMessageBox::Ok);
            ui->lEFileName->clear ();
            ui->lEHFileName->clear ();
            ui->lEErrorString->setText (tr("File : %1, error is %2, line is %3, column is %4").arg (xmlFileName).arg (xmlStreamR->errorString()).arg (xmlStreamR->lineNumber()).arg (xmlStreamR->columnNumber()));
            return;
        }
    }

    int nAttrs = attrFields.count();
    QStandardItemModel *pModel = new QStandardItemModel (0, nAttrs);
    QAbstractItemModel *oldModel = ui->tVPreview->model ();
    ui->tVPreview->setModel (pModel);
    if (oldModel)
        delete oldModel;

    if (c)
        c->release ();

    if (io && io->category() && io->category()->tableCategory())
        c = new KKSCategory (*io->category()->tableCategory());
    else
        c = new KKSCategory ();
    QStringList codeList;
    attrCodes.clear ();
    for (int i=0; i<nAttrs; i++)
    {
        codeList.append (attrFields[i].code);
        attrCodes.append (attrFields[i].code);
        pModel->setHeaderData (i, Qt::Horizontal, (attrFields[i].name.isEmpty() ? attrFields[i].code : attrFields[i].name), Qt::DisplayRole);
        pModel->setHeaderData (i, Qt::Horizontal, attrFields[i].code, Qt::UserRole);
    }

    xmlFile->close ();
    emit loadAttributes (c, codeList);
}

QString KKSXMLForm :: getCSVFile (void) const
{
    return csvFileName;
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
            if (reader->name ().toString().compare ("field", Qt::CaseInsensitive) == 0)
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
    if (!reader->isStartElement () || reader->name ().toString().compare ("field", Qt::CaseInsensitive) != 0)
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
    if (csvFileName.isEmpty() || xmlFileName.isEmpty () || charSet.isEmpty () || fieldDelim.isEmpty() || textDelim.isEmpty() || !ui->tVPreview->model ())
        return;
    QFile csvFile (csvFileName);
    if (!isExport)
        emit loadCSV (&csvFile, charSet, fieldDelim, textDelim, ui->tVPreview->model (), this);
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
