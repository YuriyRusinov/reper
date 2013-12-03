#ifndef CALCSTRING_H
#define CALCSTRING_H

#include <QString>
#include <math.h>
#include <QList>

class DNCalcString
{
public:
    DNCalcString();
    float MainFunc(QString MainStr);
    int FillNumChan(QString DataString); //«аполн€ет массивы номеров каналов длинну и позиции в строке которые надо заменить цифрами вместо номеров каналов
    QString FillStringChan(QString DataString,float *Mass); //ѕреобразует строку с каналами в строку с соответствующими цифрами размер массива должен равн€тс€ числу используемых в строке каналов

    QList <int> NumChan; //Ќомер канала
    QList <int> ChB; //Ќомер начального канала дл€ вычислени€ среднего значени€ в диапазоне
    QList <int> ChE; //Ќомер конечного канала дл€ вычислени€ среднего значени€ в диапазоне
    QList <int> pos;
    QList <int> len;

private:
    float number(QString Str);
    float factor(QString Str);
    float expr(QString Str);
    float brackets(QString Str);
    float F_sqrt(QString Str);

    int CurPos;

};

#endif // CALCSTRING_H
