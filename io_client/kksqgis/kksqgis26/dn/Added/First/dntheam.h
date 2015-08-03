#ifndef THEAM_H
#define THEAM_H
#include <QList>
#include "dnmathadd.h"
//#include "Added/multispecpolyprop.h"

struct BatimetrResult
{
 double a;
 double b;
 double h;
 double FMin;
};

struct BigthResult
{
 double a;
 double b;
 double h;
 double R1a;
 double R2a;
 double R3a;
};

class DNTheam
{

public:
    DNTheam();
    void SelectWater();
    double Batinometr(int TypeBottom,float R485,float R560,float R660,float Ra425, float Ra545, float Ra605);
    QList <BigthResult> CalcFunkBatimetrObr(int TypeBottom);
    BatimetrResult CalcFunkBatimetrObr(double a,double b,double h,double R1b,double R2b,double R3b);
    BatimetrResult CalcFunkBatimetr(double a, double b, double h, float R1a, float R2a, float R3a,double R1b,double R2b,double R3b);
    BatimetrResult CalcFunkBatimetr2(double a, double b, double h, float R1a, float R2a, float R3a,double R1b,double R2b,double R3b);
    double CalcFunkBatimetrNew(int TypeBottom,float R485, float R560, float R660, float *Ra);
    double CalcFunkNew(double Rdp,double Ra,double Rb,double k);
};

#endif // THEAM_H
