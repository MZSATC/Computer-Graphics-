// Material.h: interface for the CMaterial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATERIAL_H__1161180E_9A92_4DAE_AB2E_757671AB6E12__INCLUDED_)
#define AFX_MATERIAL_H__1161180E_9A92_4DAE_AB2E_757671AB6E12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RGB.h"

class CMaterial  
{
public:
	CMaterial();
	virtual ~CMaterial();
	void SetAmbient(CRGB);//���ò��ʶԻ�����ķ�����
	void SetDiffuse(CRGB);//���ò��ʶ��������ķ�����
	void SetSpecular(CRGB);//���ò��ʶԾ��淴���ķ�����
	void SetEmit(CRGB);//���ò�������������ɫ
	void SetExp(double);//���ò��ʵĸ߹�ָ��	
public:
	CRGB M_Ambient;//���ʶԻ�����ķ�����
	CRGB M_Diffuse;//���ʶ��������ķ�����
	CRGB M_Specular;//���ʶԾ��淴���ķ�����
	CRGB M_Emit;//��������ɢ����ɫ
	double M_Exp;//���ʵĸ߹�ָ��
};

#endif // !defined(AFX_MATERIAL_H__1161180E_9A92_4DAE_AB2E_757671AB6E12__INCLUDED_)
