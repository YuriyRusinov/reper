/* 
 * File:   KKSSearchTemplateDialog.h
 * Author: rusinov
 *
 * Created on 27 Март 2013 г., 11:41
 */

#ifndef KKSSEARCHTEMPLATEDIALOG_H
#define	KKSSEARCHTEMPLATEDIALOG_H

#include "kksgui_config.h"
#include <QDialog>

class QPushButton;

class KKSSearchTemplate;
class KKSCategory;

class KKSFiltersEditorForm;
class SaveSearchTemplateForm;

class _GUI_EXPORT KKSSearchTemplateDialog : public QDialog
{
public:
    KKSSearchTemplateDialog(KKSSearchTemplate * st, const KKSCategory * _c, const QString & tableName, bool forIO=false,  QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~KKSSearchTemplateDialog();

    KKSSearchTemplate * getSearchTemplate (void) const;
    SaveSearchTemplateForm * stypeWidget (void) const;
    KKSFiltersEditorForm * sFilterWidget (void) const;
private:
    //
    // Functions
    //
    void init (void);

private:
    //
    // Variables
    //
    KKSSearchTemplate * searchTemplate;
    SaveSearchTemplateForm * stForm;
    KKSFiltersEditorForm * filterForm;

    QPushButton * pbOk;
    QPushButton * pbCancel;
private:
    Q_OBJECT

};

#endif	/* KKSSEARCHTEMPLATEDIALOG_H */

