#include <QtDebug>
#include "KKSEntityFactory.h"

KKSEntityFactory :: KKSEntityFactory (QObject *parent)
    : QObject (parent)
{
}

KKSEntityFactory :: ~KKSEntityFactory (void)
{
    qDebug () << __PRETTY_FUNCTION__;
}
