#include "kksfilewidget.h"
#include "ui_kksfile_widget.h"

#include <QMessageBox>
#include <QTreeView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QtDebug>
#include <QString>

KKSFileWidget::KKSFileWidget(const KKSList<KKSFile*> & files, 
                             const KKSList<KKSFileType*> & fileTypes,
                             bool dialogMode, 
                             QWidget *parent)
    : QWidget(parent)
{
    ui = new Ui::kksfile_widget;
    ui->setupUi(this);

    m_files = files;    
    m_fileTypes = fileTypes;

    init();

    if(!dialogMode){
        ui->pbOk->setVisible(false);
        ui->pbCancel->setVisible(false);
    }
}

KKSFileWidget::~KKSFileWidget()
{
}

void KKSFileWidget::init()
{
    ui->twFiles->header()->resizeSection(0, 250);
    ui->twFiles->header()->resizeSection(1, 70);
    ui->twFiles->header()->resizeSection(2, 70);
    ui->twFiles->header()->resizeSection(3, 100);
    ui->twFiles->header()->resizeSection(4, 250);

    int cnt = m_files.count();
    for(int i=0; i<cnt; i++){
        
        KKSFile * f = m_files.at(i);
        appendFileRow(f);
    }


    cnt = m_fileTypes.count();
    for(int i=0; i<cnt; i++){
        KKSFileType * t = m_fileTypes.at(i);
        QVariant v(t->id());
        ui->cbTypes->addItem(t->name(), v);
    }

    connect(ui->twFiles, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(twFilesDoubleClicked(const QModelIndex &)));
}

void KKSFileWidget::appendFileRow(KKSFile * f)
{
    if(!f)
        return;

    QStringList strings;
    strings << f->name();
    if(f->uploaded())
        strings << tr("Yes");
    else
        strings << tr("No");

    if(f->loaded())
        strings << tr("Yes");
    else
        strings << tr("No");

    strings << f->type().name();

    strings << f->localUrl();

    new QTreeWidgetItem(ui->twFiles, strings);
}

void KKSFileWidget::on_pbCancel_clicked()
{
    //reject();
}

void KKSFileWidget::on_pbOK_clicked()
{
    //accept();
}

const KKSList<KKSFileType*> & KKSFileWidget::fileTypes() const
{
    return m_fileTypes;
}

const KKSList<KKSFile*> & KKSFileWidget::files() const
{
    return m_files;
}

void KKSFileWidget::on_pbDownload_clicked()
{
    KKSFile * f = getCurrentFile();
    if(!f){
        QMessageBox::warning(this, 
                             tr("Download file"), 
                             tr("Choose file for download!"), 
                             QMessageBox::Ok);

        return;
    }
    
    if(f->id() <= 0){
        QMessageBox::critical(this, 
                              tr("Download file"), 
                              tr("The file is not saved to database, just locally added!\n"
                                 "You can't download it"), 
                              QMessageBox::Ok);
        return;
    }

    if(f->loaded()){
        int answer = QMessageBox::question(this, 
                                           tr("Download file"), 
                                           tr("The file is already downloaded!\n"
                                              "Do you want to download it once more?"), 
                                           QMessageBox::Yes | QMessageBox::No);
        if(answer == QMessageBox::No)
            return;
    }
    
    QString filter = QString("(*.") + f->srcExt() + ")";
    QString selectedFilter;
    QString fileName = f->name();
    QString localFile = QFileDialog::getSaveFileName(this, 
                                                     tr("Choose file to save"),
                                                     fileName,
                                                     filter,
                                                     &selectedFilter);

    if(localFile.isEmpty())
        return;

    f->setLocalUrl(localFile);

    emit downloadFile(f, this);

    int row = ui->twFiles->currentIndex().row();
    QTreeWidgetItem * item = ui->twFiles->topLevelItem(row);
    if(!item)
        return;

    if(f->loaded() && ! f->localUrl().isEmpty()){
        item->setText(2, tr("Yes"));
        item->setText(4, f->localUrl());
    }
    

}

void KKSFileWidget::twFilesDoubleClicked(const QModelIndex & index)
{
    Q_UNUSED(index);

    on_pbOpen_clicked();
}

void KKSFileWidget::on_pbOpen_clicked()
{
    KKSFile * f = getCurrentFile();
    if(!f){
        QMessageBox::warning(this, 
                             tr("Open file"), 
                             tr("Choose file for open!"), 
                             QMessageBox::Ok);

        return;
    }

    if(!f->loaded() || f->localUrl().isEmpty()){
        int answer = QMessageBox::question(this, 
                                           tr("Open file"), 
                                           tr("The file is not downloaded!\n"
                                              "Do you want to download it?"), 
                                           QMessageBox::Yes | QMessageBox::No);
        if(answer == QMessageBox::No)
            return;

        //загружаем файл
        on_pbDownload_clicked();
        //а дале мы предполагаем, что он загружен,
        //но проверяем, так ли это, на случай, если при загрузке произошли ошибки
    }
    
    if(!f->loaded() || f->localUrl().isEmpty()){
        QMessageBox::critical(this, 
                              tr("Open file"), 
                              tr("The file is not downloaded!"), 
                              QMessageBox::Ok);

        return;
    }

    if(!f->open()){
        QMessageBox::critical(this, 
                              tr("Open file"), 
                              tr("Cannot open the file!\n\n"
                                 "If you about to open file of Microsoft Visio type,\n"
                                 "try to rename it with extention. Example: filename.vsd"), 
                              QMessageBox::Ok);
    }
}

void KKSFileWidget::on_pbDelete_clicked()
{
    KKSFile * f = getCurrentFile();
    if(!f){
        QMessageBox::warning(this, 
                             tr("Remove file"), 
                             tr("Choose file for remove!"), 
                             QMessageBox::Ok);

        return;
    }

    int row = ui->twFiles->currentIndex().row();
    
    QTreeWidgetItem * item = ui->twFiles->takeTopLevelItem ( row );
    if(!item)
        return;

    delete item;

    m_files.removeAt(row);

    int cnt = m_files.count();
    if(row < cnt-1){
        if(row > 0){
            QTreeWidgetItem * item = ui->twFiles->topLevelItem(row-1);
            if(item)
                ui->twFiles->setCurrentItem(item);
        }
    }
    else{
        QTreeWidgetItem * item = ui->twFiles->topLevelItem(row);
        if(item)
            ui->twFiles->setCurrentItem(item);
    }

    emit fileListChanged();
}

void KKSFileWidget::on_tbAddFile_clicked()
{
    if(ui->lePath->text().isEmpty() || 
       ui->leName->text().isEmpty())
    {
        QMessageBox::critical(this, 
                              tr("Add file"), 
                              tr("Path or name of file is invalid!"), 
                              QMessageBox::Ok);
        return;
    }

    if(equalFileAdded()){
        QMessageBox::critical(this, 
                              tr("Add file"), 
                              tr("You cannot add one file twise!"), 
                              QMessageBox::Ok);
        return;
    }

    KKSFile * f = new KKSFile(-1, ui->leName->text(), ui->lePath->text());
    int index = ui->cbTypes->currentIndex();
    KKSFileType type(*(m_fileTypes.at(index)));
    f->setType(type);
    m_files.append(f);
    
    appendFileRow(f);

    f->release();

    emit fileListChanged();
}

void KKSFileWidget::on_tbOpenFile_clicked()
{
    QString localFile = QFileDialog::getOpenFileName(this, 
                                                     tr("Open file"));

    if(localFile.isEmpty())
        return;

    QFileInfo f(localFile);
    ui->leName->setText(f.fileName());

    ui->lePath->setText(localFile);

    searchForType();
}

void KKSFileWidget::searchForType()
{
    if(ui->cbTypes->count() == 0)
        return;

    QString filePath = ui->lePath->text();
    
    if(filePath.isEmpty())
        return;

    QString ext = filePath.section(".", -1);
    if(ext.isEmpty()){
        ui->cbTypes->setCurrentIndex(0);
        return;
    }

    bool bFound = false;
    for(int i=0; i<ui->cbTypes->count(); i++){
        KKSFileType * type = m_fileTypes.at(i);
        bFound = type->assotiated(ext.toLower());
        if(bFound){
            ui->cbTypes->setCurrentIndex(i);
            break;
        }
        else{
            bFound = type->assotiated(ext.toUpper());
            if(bFound){
                ui->cbTypes->setCurrentIndex(i);
                break;
            }
        }
    }

    if(!bFound){
        ui->cbTypes->setCurrentIndex(0);
    }
}

void KKSFileWidget::on_pbAddFromServer_clicked()
{
    //после реализации данного метода не забыть этот вызов.
    //emit fileListChanged();
}

bool KKSFileWidget::equalFileAdded()
{
    int cnt = m_files.count();
    for(int i=0; i<cnt; i++){
        KKSFile * f = m_files.at(i);
        if(f->localUrl() == ui->lePath->text())
            return true;
    }

    return false;
}

KKSFile * KKSFileWidget::getCurrentFile()
{
    KKSFile * f = NULL;
    int row = ui->twFiles->currentIndex().row();
    if(row < 0 || row >= m_files.count())
        return f;

    f = m_files.at(row);

    return f;
}
