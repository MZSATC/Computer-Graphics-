// P2.cpp: implementation of the CP2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "P2.h"
#include "math.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CP2::CP2()
{

}

CP2::~CP2()
{

}

CP2::CP2(double x,double y)
{
	this->x=x;
	this->y=y;
}

CP2 operator +(const CP2 &p0,const CP2 &p1)//和
{	
	CP2 p;
	p.x=p0.x+p1.x;
	p.y=p0.y+p1.y;
	return p;
}

CP2 operator -(const CP2 &p0,const CP2 &p1)//差
{
	CP2 p;
	p.x=p0.x-p1.x;
	p.y=p0.y-p1.y;
	return p;
}

CP2 operator *(const CP2 &p,double k)//点和常量的积
{	
	return CP2(p.x*k,p.y*k);
}

CP2 operator *(double k,const CP2 &p)//点和常量的积
{	
	return CP2(p.x*k,p.y*k);
}

CP2 operator /(const CP2 &p0,double k)//数除
{
	if(fabs(k)<1e-6)
		k=1.0;
	CP2 p;
	p.x=p0.x/k;
	p.y=p0.y/k;
	return p;
}

CP2 operator+=(CP2 &p0,CP2 &p1)
{
	p0.x=p0.x+p1.x;
	p0.y=p0.y+p1.y;
	return p0;
}

CP2 operator-=(CP2 &p0,CP2 &p1)
{
	p0.x=p0.x-p1.x;
	p0.y=p0.y-p1.y;
	return p0;
}

CP2 operator*=(CP2 &p0,double k)
{
	p0.x=p0.x*k;
	p0.y=p0.y*k;
	return p0;
}

CP2 operator/=(CP2 &p0,double k)
{
	if(fabs(k)<1e-6)
		k=1.0;
	p0.x=p0.x/k;
	p0.y=p0.y/k;
	return p0;
}


