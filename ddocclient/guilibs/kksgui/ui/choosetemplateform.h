#ifndef CHOOSETEMPLATEFORM_H
#define CHOOSETEMPLATEFORM_H

#include <QDialog>
#include <kksgui_config.h>

#include <KKSList.h>
#include <KKSTemplate.h>

namespace Ui
{
    class choose_template_form;
}

class _GUI_EXPORT ChooseTemplateForm : public QDialog
{
Q_OBJECT
public:
    ChooseTemplateForm(const KKSList<KKSTemplate*> & tList, QWidget *parent = 0);
    virtual ~ChooseTemplateForm();

    KKSTemplate * getCurrentTemplate();
    bool isSave (void) const;

private slots:
    void on_pbCancel_clicked();
    void on_pbOk_clicked();

    void init();

private:
    KKSList<KKSTemplate*> m_tList;
    Ui::choose_template_form * ui;
};

#endif // CHOOSETEMPLATEFORM_H
