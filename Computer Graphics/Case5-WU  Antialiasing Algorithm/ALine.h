// ALine.h: interface for the CALine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALINE_H__048BF17D_0463_44AD_BE54_377516381BD7__INCLUDED_)
#define AFX_ALINE_H__048BF17D_0463_44AD_BE54_377516381BD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include"P2.h"

class CALine
{
public:
	CALine();
	virtual ~CALine();
	void MoveTo(CDC *,CP2);//�ƶ���ָ��λ��
	void MoveTo(CDC *,double,double);
	void MoveTo(CDC *,double,double,CRGB);
	void LineTo(CDC *,CP2);//����ֱ�ߣ������յ�
	void LineTo(CDC *,double,double);
	void LineTo(CDC *,double,double,CRGB);
public:
	CP2 P0;//���
	CP2 P1;//�յ�
};

#endif // !defined(AFX_ALINE_H__048BF17D_0463_44AD_BE54_377516381BD7__INCLUDED_)
