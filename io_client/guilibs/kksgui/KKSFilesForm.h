/***********************************************************************
 * Module:  KKSFilesForm.h
 * Author:  yuriyrusinov
 * Modified: 20.11.2010 г.
 * Purpose: Declaration of the class KKSFilesForm
 * Comment: Данный класс унаследован от QDialog и предназначен для
 *  визуализации списка присоединенных файлов .
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSFilesForm_h)
#define __KKSSITOOOM_KKSFilesForm_h

#include "kksgui_config.h"

#include <QDialog>

#include <KKSList.h>

class QListView;
class QPushButton;
class QAbstractItemModel;

class KKSFile;

class _GUI_EXPORT KKSFilesForm : public QDialog
{
public:
    KKSFilesForm (const KKSList<KKSFile *>& files, QWidget * parent=0, Qt::WFlags f=0);
    virtual ~KKSFilesForm (void);

    KKSFile * getFile (void) const;

private:
    //
    // Functions
    //
    void initWidgets (void);
    void initModel (void);

private:
    //
    // Variables
    //
    KKSList<KKSFile *> m_fileList;
    QListView * lvFiles;
    QAbstractItemModel * filesModel;

    QPushButton * pbOk;
    QPushButton * pbCancel;

private:
    Q_OBJECT
};

#endif
