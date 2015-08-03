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
    int FillNumChan(QString DataString); //��������� ������� ������� ������� ������ � ������� � ������ ������� ���� �������� ������� ������ ������� �������
    QString FillStringChan(QString DataString,float *Mass); //����������� ������ � �������� � ������ � ���������������� ������� ������ ������� ������ �������� ����� ������������ � ������ �������

    QList <int> NumChan; //����� ������
    QList <int> ChB; //����� ���������� ������ ��� ���������� �������� �������� � ���������
    QList <int> ChE; //����� ��������� ������ ��� ���������� �������� �������� � ���������
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
