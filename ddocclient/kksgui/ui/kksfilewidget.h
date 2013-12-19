#ifndef KKSFILEWIDGET_H
#define KKSFILEWIDGET_H

#include <QWidget>
#include <kksgui_config.h>

#include <KKSList.h>
#include <KKSFile.h>

namespace Ui
{
    class kksfile_widget;
}

class QModelIndex;

class _GUI_EXPORT KKSFileWidget: public QWidget
{
    Q_OBJECT

public:
    KKSFileWidget(const KKSList<KKSFile*> & files, 
                  const KKSList<KKSFileType*> & fileTypes,
                  bool dialogMode = false,
                  QWidget *parent = 0);
    virtual ~KKSFileWidget();

    const KKSList<KKSFile*> & files() const;
    const KKSList<KKSFileType*> & fileTypes() const;

signals:
    void downloadFile(KKSFile *f, QWidget* parent);
    void fileListChanged();

private slots:
    void on_pbOK_clicked();
    void on_pbCancel_clicked();
    void on_pbDownload_clicked();
    void on_pbOpen_clicked();
    void on_pbDelete_clicked();
    
    void on_pbAddFile_clicked();
    void on_tbOpenFile_clicked();
    void on_pbAddFromServer_clicked();
    void twFilesDoubleClicked(const QModelIndex &);

    void init();
    
private:

    Ui::kksfile_widget * ui;
    KKSList<KKSFile*> m_files;
    KKSList<KKSFileType*> m_fileTypes;

    KKSFile * getCurrentFile();
    void appendFileRow(KKSFile * f);
    bool equalFileAdded();
    void searchForType();
};

#endif // KKSFILEWIDGET_H
