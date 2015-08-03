#ifndef STRUCTS_H
#define STRUCTS_H
#include <QString>
struct PolygonProp
{
 int xn;
 int yn;
 int W;
 int H;
 int KCh;
 int KPix;
};

struct ImageProerty
{
 int W,H; // Ширина, высота изображения
 int KolvoChan; // Количество спектральных каналов
};

/*Новый проект*/
struct StructNewProj
{

};

/*Параметры проекта*/
struct DataFileProj
{
 QString DataType;
 QString FoolNameData;
 QString PathFileProj;
 QString PathData;
 QString PathPolygon;
 QString FoolNameImageStart;
 int Ch;
 int ChR,ChG,ChB;
 float KR,KG,KB;
};

struct StaticProp
{
 float Midle;
 float SKO;
 float KofAssim;
 float KofEksc;
};

struct IsoDataClassProp
{
 float *Zk;
 float *SKO;
 float SKOMax;
 int NumChSKOMax;
 quint32 KPix;
 float Dsr;
 bool Objed;
};

struct IsoDataMetaClass
{
 float D;
 int NumCl1,NumCl2;
};

typedef struct tagFILEHEADER
{
  quint16 bfType;
  quint32 bfSize;
  quint16 bfReserved1;
  quint16 bfReserved2;
  quint32 bfOffBits;
}FILEHEADER;

typedef struct tagINFOHEADER
{
  quint32 biSize;
  long  biWidth;
  long  biHeight;
  quint16  biPlanes;
  quint16  biBitCount;
  quint32 biCompression;
  quint32 biSizeImage;
  long  biXPelsPerMeter;
  long  biYPelsPerMeter;
  quint32 biClrUsed;
  quint32 biClrImportant;
}INFOHEADER;

struct RGBColor
{
 quint8 R;
 quint8 G;
 quint8 B;
};

class A
{
public:
 A(int n=0)
 {
  this->mass=new int[n];
  this->n=n;
 }
 ~A()
 {
  delete[] mass;
 }
 int size()
 {
  return n;
 }
 int operator [] (int i)
 {
  return mass[i];
 }
 A operator = (A b)
 {
  if(this->n>0)
   delete[] this->mass;

  this->mass=new int[b.size()];
  for(int i=0;i<b.size();i++)
   this->mass[i]=b[i];
 }

 private:
  int *mass;
  int n;
};

class X
{
 public:
 X(int *count)
 {
  *count=*count+1;
 }
 /*
Если при создании экземпляра класса X в конструкторе
указывать ссылку на одну и ту же переменную, то в ней
будет записано количество экземпляров данного класса
*/
};

class Point
{
 public:
 int x,y;
 Point(int x=0,int y=0)
 {
  this->x=x;
  this->y=y;
 }
 Point operator + (Point b)
 {
  Point c;
  c.x=this->x+b.x;
  c.y=this->y+b.y;
  return c;
 }
 Point operator - (Point b)
 {
  Point c;
  c.x=this->x-b.x;
  c.y=this->y-b.y;
  return c;
 }
};

#endif // STRUCTS_H



