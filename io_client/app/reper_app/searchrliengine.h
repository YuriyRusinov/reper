#ifndef _SearchRLIEngine_H
#define _SearchRLIEngine_H

#include <QObject>

class SearchRadioImageFragmentForm;

class SearchRLIEngine : public QObject
{
public:
    static SearchRLIEngine * getSearchObject (QObject * parent = 0);
    SearchRadioImageFragmentForm * GUIInit (QWidget * parent=0, Qt::WindowFlags flags=0);

private:
    //
    // Functions
    //
    SearchRLIEngine (QObject * parent=0);
    virtual ~SearchRLIEngine (void);

private:
    //
    // Variables
    //
    static SearchRLIEngine * searchEngineCore;

private:
    Q_OBJECT
};

#endif
