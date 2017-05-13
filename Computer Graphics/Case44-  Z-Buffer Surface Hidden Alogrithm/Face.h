// Face.h: interface for the CFace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACE_H__82BF93AE_A82E_407D_AF42_6E6608253C1C__INCLUDED_)
#define AFX_FACE_H__82BF93AE_A82E_407D_AF42_6E6608253C1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Vector.h"

class CFace  
{
public:
	CFace();
	virtual ~CFace();
	void SetNum(int vN);//������Ķ�����
	void SetFaceNormal(CP3,CP3,CP3);//������ķ�ʸ��
public:
	int vN;          //��Ķ�����
	int *vI;         //��Ķ�������
	CVector fNormal; //��ķ�ʸ��
	CRGB fClr;       //�����ɫ
};

#endif // !defined(AFX_FACE_H__82BF93AE_A82E_407D_AF42_6E6608253C1C__INCLUDED_)
