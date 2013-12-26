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
    float GetMin(float *Mass,unsigned long long RMass);
/************************************************************/
    float CalcMidleValue(float *Mass,unsigned long long RMass);
    float CalcSKO(float *Mass,unsigned long long RMass);
    float CalcEntropOtn(float *Mass,quint64 KDiskr,unsigned long long RMass);
    void CalcStaticParam(float *Mass,quint64 RMass);
    float CalcEvklid(float *Mass1,float *Mass2, quint64 RMass);

    void CalcCovMatr(int nMass /*размер массива*/,int nElem,double *Mass /*Массив*/,double *CovMatr /*Указатель на массив матрицы*/);
    double CalcOpredMatr(int nMass /*размер массива*/,double *Matr /*Указатель на массив матрицы*/);
    void MatrInverse(double *a,int n);
    void MatrTranspon(double *Matr,int x,int y);
    double *MatrMulti(double *Matr1,int x1,int y1,double *Matr2,int x2,int y2);
    double *MatrMulti(double *Matr,int x,int y,double a);
    void MatrAdd(double *Matr1,double *Matr2,int x,int y);

    double CalcCorel(quint64 nMass,float *Mass1,float *Mass2);

    float CalcSpecAngle(int nMass,float *Mass1,float *Mass2);
    float CalcModulVect(int nMass,float *Mass);

private:
    Moments CalcMN(float *Mass,quint32 RMass);

};

#endif // MATHADD_H
