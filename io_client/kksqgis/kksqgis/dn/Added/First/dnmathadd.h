#ifndef MATHADD_H
#define MATHADD_H
#include <math.h>
#include <QQueue>
#include <QMap>
#include <QMessageBox>
#include "structs.h"
class DNMathAdd
{
 struct Moments
 {
  double M1,M2,M3,M4;
 };

public:
    DNMathAdd();
    StaticProp Prop;

/*Функции*/
    float GetMax(float *Mass,unsigned long long RMass);
    float GetMin(float *Mass,quint64 RMass);
    float* GetDeriviant(int size,float *x, float *y);
/************************************************************/
    float CalcMidleValue(float *Mass,unsigned long long RMass);
    float CalcSKO(float *Mass,unsigned long long RMass);
    float CalcEntropOtn(float *Mass,quint64 KDiskr,unsigned long long RMass);
    void CalcStaticParam(float *Mass,quint64 RMass);
    float CalcEvklid(float *Mass1,float *Mass2, quint64 RMass);

    void CalcCovMatr(quint64 nMass /*размер массива*/,int nElem,float *Mass /*Массив*/,float *CovMatr /*Указатель на массив матрицы*/);
    double CalcOpredMatr(int nMass /*размер массива*/,float *Matr /*Указатель на массив матрицы*/);
    void MatrInverse(float *a,int n);
    void MatrTranspon(float *Matr,quint64 x,quint64 y);
    float *MatrMulti(float *Matr1,quint64 x1,quint64 y1,float *Matr2,quint64 x2,quint64 y2);
    float *MatrMulti(double *Matr,int x,int y,double a);
    void MatrAdd(float *Matr1,float *Matr2,int x,int y);

    double CalcCorel(quint64 nMass,float *Mass1,float *Mass2);

    float CalcSpecAngle(int nMass,float *Mass1,float *Mass2);
    float CalcModulVect(int nMass,float *Mass);

    float VectorProjection(float *Vec1, float *Vec2, int NVec);
    void MGK(float *CovarMatr, int NCovar, float *MatrVectors, float *ValuesSelf);
private:
    Moments CalcMN(float *Mass,quint32 RMass);

};

#endif // MATHADD_H
