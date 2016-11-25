#ifndef _REPER_MAIN_WINDOW_H
#define _REPER_MAIN_WINDOW_H

#include <QMainWindow>

#include "imagecreatorform.h"

class QMdiArea;
class QToolBar;

class KKSObjEditor;
class generatingDataPlus;
class Assistant;
class SearchRadioImageCalc;

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
    void slot3DView (void);
    void slotRLI (void);
    void slotClose (void);

    void slotSearchByImage (void);
    void slotCompare (void);
    void slotGologram (void);
    void slotGologramCalc (generatingDataPlus gdp);
    void slotViewImage (void);

    void slotCreateNewObjEditor(KKSObjEditor * objEditor);

    void setActionsEnabled(bool enabled);
    void isActiveSubWindow(const KKSObjEditor * editor, bool * yes);
    void searchIm (const QImage& sIm);

    void slotHelp (void);
    void slotSettings (void);
    void slotSetWindow (QWidget *w);
private:
    //
    // Functions
    //
    QWidget * activeKKSSubWindow();
    void initToolBars (void);
private:
    //
    // Variables
    //
    Ui::ReperMainWindowForm * UI;
    QMdiArea * m_mdiArea;
    Assistant * ass;
    QToolBar * tbActions;
    QToolBar * tbCalc;
    QToolBar * tbOthers;
    SearchRadioImageCalc * imCalc;

private:
    Q_OBJECT
};

#endif
