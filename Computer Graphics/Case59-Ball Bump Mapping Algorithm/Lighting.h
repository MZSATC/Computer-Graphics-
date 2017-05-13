// Lighting.h: interface for the CLighting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTING_H__4E9D1F90_7696_4D33_999F_6417C555CB36__INCLUDED_)
#define AFX_LIGHTING_H__4E9D1F90_7696_4D33_999F_6417C555CB36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Light.h"
#include "Material.h"
#include "Vector.h"

class CLighting  
{
public:
	CLighting();
	CLighting(int);
	virtual ~CLighting();
	void SetLightNumber(int);//���ù�Դ����		
	CRGB Lighting(CP3,CP3,CVector,CMaterial *);//����	
public:
	int LightNum;//��Դ����
	CLight *Light;//��Դ����
	CRGB Ambient;//������
};

#endif // !defined(AFX_LIGHTING_H__4E9D1F90_7696_4D33_999F_6417C555CB36__INCLUDED_)
