#include "KKSFilter.h"
#include "KKSSearchTemplate.h"

KKSSearchTemplate :: KKSSearchTemplate (int id, KKSFilterGroup * sg, const QString& name, int idAuth)
    : KKSRecord (id, name),
    mainGroup (sg),
    author (idAuth)
{
    if (mainGroup)
        mainGroup->addRef ();
}

KKSSearchTemplate :: KKSSearchTemplate (const KKSSearchTemplate& st)
    : KKSRecord (st),
    mainGroup (st.mainGroup),
    author (st.author)
{
    if (mainGroup)
        mainGroup->addRef ();
}

KKSSearchTemplate :: ~KKSSearchTemplate (void)
{
    if (mainGroup)
        mainGroup->release ();
}

KKSFilterGroup *KKSSearchTemplate :: getMainGroup (void) const
{
    return mainGroup;
}

void KKSSearchTemplate :: setMainGroup (KKSFilterGroup *sg)
{
    if (mainGroup)
        mainGroup->release ();

    mainGroup = sg;

    if (mainGroup)
        mainGroup->addRef ();
}

int KKSSearchTemplate :: getAuthor (void) const
{
    return author;
}

void KKSSearchTemplate :: setAuthor (int idAuthor)
{
    author = idAuthor;
}
