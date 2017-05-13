// CLine.h: interface for the CCLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLINE_H__F98927DB_3AE3_4F19_AD21_E35A3900D86F__INCLUDED_)
#define AFX_CLINE_H__F98927DB_3AE3_4F19_AD21_E35A3900D86F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include"P2.h"

class CCLine  
{
public:
	CCLine();
	virtual ~CCLine();
	void MoveTo(CDC *,CP2);//�ƶ���ָ��λ��
	void MoveTo(CDC *,double,double,CRGB);
	void LineTo(CDC *,CP2);//����ֱ�ߣ������յ�
	void LineTo(CDC *,double,double,CRGB);
	CRGB Interpolation(double,double,double,CRGB,CRGB);//���Բ�ֵ
public:
	CP2 P0;//���
	CP2 P1;//�յ�
};

#endif // !defined(AFX_CLINE_H__F98927DB_3AE3_4F19_AD21_E35A3900D86F__INCLUDED_)
