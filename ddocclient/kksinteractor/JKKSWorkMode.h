#if !defined (__KKSSITOOOM_JKKSWorkMode_H)
#define __KKSSITOOOM_JKKSWorkMode_H

#include "kksinteractor_config.h"
#include "jkksuid.h"
#include "JKKSWorkModeType.h"

class QDataStream;

class _I_EXPORT JKKSWorkMode : public JKKSUID 
{
public:
    JKKSWorkMode (int id=-1, const QString& wm_name=QString(), const QString& desc=QString(), const QString& symbol=QString(), const JKKSWorkModeType& type=JKKSWorkModeType(), const QString& uid = QString());
    JKKSWorkMode (const JKKSWorkMode& WM);
    ~JKKSWorkMode (void);

    int id (void) const;
    void setId (int _id);

    const QString& getName (void) const;
    void setName (const QString& name);

    const QString& getDesc (void) const;
    void setDesc (const QString& desc);

    const QString& getSymbol (void) const;
    void setSymbol (const QString& symbol);

    const JKKSWorkModeType& getType (void) const;
    void setType (const JKKSWorkModeType& type);

private:
    //
    // Functions
    //
    friend QDataStream& operator<< (QDataStream& out, const JKKSWorkMode& wm);
    friend QDataStream& operator>> (QDataStream& in, JKKSWorkMode& wm);

private:
    //
    // Variables
    //
    int idWM;
    QString wmName;
    QString wmDesc;
    QString wmSymbol;
    JKKSWorkModeType wmType;
};

#endif
