/***********************************************************************
 * Module:  KKSPrerender.h
 * Author:  yuriy
 * Modified: 03 ����� 2011 �.
 * Purpose: Declaration of the class KKSPrerender
 * Comment: ������ ����� ���������� ����������� � ������ ����������
 ***********************************************************************/

#ifndef _KKSPrerender_H
#define _KKSPrerender_H

#include "kksguifactory_config.h"
#include <orprerender.h>

class ParameterList;
class ORODocument;

class _GF_EXPORT KKSPrerender : public ORPreRender
{
public:
    KKSPrerender (void);
    KKSPrerender (const QDomDocument &);
    KKSPrerender (const QDomDocument &, const ParameterList &);
    virtual ~KKSPrerender (void);

    ORODocument * generate ();
};

#endif
