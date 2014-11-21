#ifndef _REPER_MAIN_WINDOW_H
#define _REPER_MAIN_WINDOW_H

#include <QMainWindow>

class QMdiArea;

class KKSObjEditor;

namespace Ui
{
    class ReperMainWindowForm;
};

class ReperMainWindow : public QMainWindow
{
public:
    ReperMainWindow (QWidget * parent = 0, Qt::WindowFlags flags = 0);
    virtual ~ReperMainWindow (void);

private slots:
    void slotConnect (void);
    void slotDisconnect (void);
    void slot3DMod (void);
    void slotRLI (void);
    void slotClose (void);

    void slotSearch (void);
    void slotCompare (void);

    void slotCreateNewObjEditor(KKSObjEditor * objEditor);
private:
    //
    // Variables
    //
    Ui::ReperMainWindowForm * UI;
    QMdiArea * m_mdiArea;

private:
    Q_OBJECT
};

#endif
