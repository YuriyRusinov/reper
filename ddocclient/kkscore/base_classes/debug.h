#ifndef DEBUG_H_
#define DEBUG_H_

#include "config_core.h"

#if defined( WIN32 )
#define __PRETTY_FUNCTION__ __FUNCTION__
#else
#define __FUNCTION__ __PRETTY_FUNCTION__
#endif

// Function name printing
#include <QDebug>

#define dFunc() qDebug() << __PRETTY_FUNCTION__

// Debug output

#ifndef QT_NO_DEBUG_STREAM

//class OGREnvelope;
//__CORE_EXPORT QDebug operator<<(QDebug, const OGREnvelope &);

#endif

#endif
