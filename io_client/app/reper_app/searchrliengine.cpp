#include "searchradiofragmentform.h"
#include "searchrliengine.h"

SearchRLIEngine * SearchRLIEngine :: searchEngineCore = 0;

SearchRLIEngine :: SearchRLIEngine (QObject * parent)
    : QObject (parent)
{
    if (searchEngineCore)
        qFatal ("There should be only one SearchRLIEngine object");

    searchEngineCore = this;
}

SearchRLIEngine :: ~SearchRLIEngine (void)
{
    searchEngineCore = 0;
}

SearchRLIEngine * SearchRLIEngine :: getSearchObject (QObject * parent)
{
    if (!searchEngineCore)
        searchEngineCore = new SearchRLIEngine (parent);

    return searchEngineCore;
}

SearchRadioImageFragmentForm * SearchRLIEngine :: GUIInit (QWidget * parent, Qt::WindowFlags flags)
{

}
