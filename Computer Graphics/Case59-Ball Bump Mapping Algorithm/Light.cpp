// Light.cpp: implementation of the CLight class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "Light.h"

#include "math.h"
#define PI 3.14159265

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLight::CLight()
{
	L_Diffuse=CRGB(0.0,0.0,0.0);//��Դ����������ɫ	
	L_Specular=CRGB(1.0,1.0,1.0);//��Դ����߹���ɫ
	L_Position.x=0.0,L_Position.y=0.0,L_Position.z=1000;//��Դλ��ֱ������
	L_R=1000,L_Phi=0,L_Theta=0;//��Դλ��������
	L_C0=1.0;//����˥��ϵ��
	L_C1=0.0;//����˥��ϵ��
	L_C2=0.0;//����˥��ϵ��
	L_OnOff=TRUE;//��Դ����
}

CLight::~CLight()
{

}

void CLight::SetDiffuse(CRGB dif)
{
	L_Diffuse=dif;
}

void CLight::SetSpecular(CRGB spe)
{
	L_Specular=spe;
}
void CLight::SetPosition(double x,double y,double z)
{
	L_Position.x=x;
	L_Position.y=y;
	L_Position.z=z;
}

void CLight::SetGlobal(double r,double phi,double theta)
{
	L_R=r;
	L_Phi=phi;
	L_Theta=theta;
}

void CLight::SetOnOff(BOOL onoff)
{
	L_OnOff=onoff;
}

void CLight::SetCoef(double c0,double c1,double c2)
{
	L_C0=c0;
	L_C1=c1;
	L_C2=c2;
}

void CLight::GlobalToXYZ()
{	
	L_Position.x=L_R*sin(L_Phi*PI/180)*sin(L_Theta*PI/180);
	L_Position.y=L_R*cos(L_Phi*PI/180);
	L_Position.z=L_R*sin(L_Phi*PI/180)*cos(L_Theta*PI/180);
}