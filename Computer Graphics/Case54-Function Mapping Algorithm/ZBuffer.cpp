// ZBuffer.cpp: implementation of the CZBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "ZBuffer.h"
#include "math.h"
#define Round(d) int(floor(d+0.5))
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZBuffer::CZBuffer()
{
	P=NULL;
	N=NULL;
	T=NULL;
	pHeadE=NULL;
	pCurrentB=NULL;
	pEdge=NULL;
	pCurrentE=NULL;
	pHeadB=NULL;
	zBuffer=NULL;
}

CZBuffer::~CZBuffer()
{
	for(int i=0;i<Width;i++)
	{
		delete[] zBuffer[i];
		zBuffer[i]=NULL;
	}
	if(zBuffer!=NULL)
	{
		delete zBuffer;
		zBuffer=NULL;
	}	
	ClearMemory();
}

void CZBuffer::SetPoint(CPi3 *p,CVector *n,CT2 *t,int m)
{
	P=new CPi3[m];
	N=new CVector[m];
	T=new CT2[m];
    for(int i=0;i<m;i++)
	{
		P[i]=p[i];	
		N[i]=n[i];
		T[i]=t[i];
	}
	PNum=m;
}
void CZBuffer::CreateBucket()//����Ͱ��
{
	int yMin,yMax;
	yMin=yMax=P[0].y;
	for(int i=1;i<PNum;i++)//���Ҷ���������ǵ���С�����ɨ����
	{
		if(P[i].y<yMin)
			yMin=P[i].y;//ɨ���ߵ���Сֵ
		if(P[i].y>yMax)
			yMax=P[i].y;//ɨ���ߵ����ֵ
	}
	for(int y=yMin;y<=yMax;y++)
	{
		if(yMin==y)//����Ͱͷ���
		{
			pHeadB=new CBucket;//����Ͱ��ͷ���
			pCurrentB=pHeadB;//pCurrentBΪCBucket��ǰ���ָ��
			pCurrentB->ScanLine=yMin;
			pCurrentB->pET=NULL;//û�����ӱ߱�
			pCurrentB->pNext=NULL;
		}
		else//����ɨ����
		{
			pCurrentB->pNext=new CBucket;//����Ͱ���������
			pCurrentB=pCurrentB->pNext;
			pCurrentB->ScanLine=y;
			pCurrentB->pET=NULL;
			pCurrentB->pNext=NULL;
		}
	}
}

void CZBuffer::CreateEdge()//�����߱�
{
	for(int i=0;i<PNum;i++)
	{
		pCurrentB=pHeadB;
		int j=(i+1)%PNum;//�ߵĵڶ������㣬P[i]��P[j]���ɱ�
		if(P[i].y<P[j].y)//�ߵ��յ������
		{
			pEdge=new CAET;
			pEdge->x=P[i].x;//����ET���ֵ
			pEdge->yMax=P[j].y;
			pEdge->k=(P[j].x-P[i].x)/(P[j].y-P[i].y);//����1/k
			pEdge->ps=P[i];//�󶨶������ɫ	
			pEdge->pe=P[j];
			pEdge->ns=N[i];//�󶨷�ʸ��
			pEdge->ne=N[j];
			pEdge->ts=T[i];//������	
			pEdge->te=T[j];
			pEdge->pNext=NULL;
			while(pCurrentB->ScanLine!=P[i].y)//��Ͱ��Ѱ�Ҹñߵ�yMin
			{
				pCurrentB=pCurrentB->pNext;//�Ƶ�yMin���ڵ�Ͱ���
			}		
		}
		if(P[j].y<P[i].y)//�ߵ��յ������
		{
			pEdge=new CAET;
			pEdge->x=P[j].x;
			pEdge->yMax=P[i].y;
			pEdge->k=(P[i].x-P[j].x)/(P[i].y-P[j].y);
			pEdge->ps=P[i];
			pEdge->pe=P[j];
			pEdge->ns=N[i];
			pEdge->ne=N[j];
			pEdge->ts=T[i];	
			pEdge->te=T[j];
			pEdge->pNext=NULL;
			while(pCurrentB->ScanLine!=P[j].y)
			{
				pCurrentB=pCurrentB->pNext;
			}
		}
		if(int(P[j].y)!=P[i].y)
		{
			pCurrentE=pCurrentB->pET;
			if(pCurrentE==NULL)
			{
				pCurrentE=pEdge;
				pCurrentB->pET=pCurrentE;
			}
			else
			{
				while(pCurrentE->pNext!=NULL)
				{
					pCurrentE=pCurrentE->pNext;
				}
				pCurrentE->pNext=pEdge;
			}
		}
	}
}

void CZBuffer::Phong(CDC *pDC,CP3 ViewPoint,CLighting *pLight,CMaterial *pMaterial)//�������
{
	double	z=0.0;//��ǰɨ���ߵ�z
	double	zStep=0.0;//��ǰɨ��������x������z����
	double	A,B,C,D;//ƽ�淽��Ax+By+Cz��D=0��ϵ��
	CVector V01(P[0],P[1]),V02(P[0],P[2]);
	CVector VN=Cross(V01,V02);
	A=VN.x;B=VN.y;C=VN.z;
	D=-A*P[0].x-B*P[0].y-C*P[0].z;
	zStep=-A/C;//����ֱ��z����
	CAET *pT1,*pT2;
	pHeadE=NULL;	
	for(pCurrentB=pHeadB;pCurrentB!=NULL;pCurrentB=pCurrentB->pNext)
	{
		for(pCurrentE=pCurrentB->pET;pCurrentE!=NULL;pCurrentE=pCurrentE->pNext)
		{
			pEdge=new CAET;
			pEdge->x=pCurrentE->x;
			pEdge->yMax=pCurrentE->yMax;
			pEdge->k=pCurrentE->k;
			pEdge->ps=pCurrentE->ps;
			pEdge->pe=pCurrentE->pe;
			pEdge->ns=pCurrentE->ns;
			pEdge->ne=pCurrentE->ne;
			pEdge->ts=pCurrentE->ts;	
			pEdge->te=pCurrentE->te;
			pEdge->pNext=NULL;
			AddEt(pEdge);
		}
		ETOrder();	
		pT1=pHeadE;
		if(pT1==NULL)
			return;
		while(pCurrentB->ScanLine>=pT1->yMax)//�±��Ͽ�
		{
			CAET * pAETTEmp=pT1;
			pT1=pT1->pNext;
			delete pAETTEmp;
			pHeadE=pT1;
			if(pHeadE==NULL)
				return;
		}
		if(pT1->pNext!=NULL)
		{
			pT2=pT1;
			pT1=pT2->pNext;
		}
		while(pT1!=NULL)
		{
			if(pCurrentB->ScanLine>=pT1->yMax)//�±��Ͽ�
			{
				CAET* pAETTemp =pT1;
				pT2->pNext=pT1->pNext;
				pT1=pT2->pNext;
				delete pAETTemp;
			}
			else
			{
				pT2=pT1;
				pT1=pT2->pNext;
			}
		}
		CVector na,nb,nf;//na��nb������������ķ�ʸ����nf�������������ķ�ʸ��
		na=Interpolation(pCurrentB->ScanLine,pHeadE->ps.y,pHeadE->pe.y,pHeadE->ns,pHeadE->ne);
		nb=Interpolation(pCurrentB->ScanLine,pHeadE->pNext->ps.y,pHeadE->pNext->pe.y,pHeadE->pNext->ns,pHeadE->pNext->ne);
		CT2 ta,tb,tf;//ta��tb�����������������tf������������������
		ta=Interpolation(pCurrentB->ScanLine,pHeadE->ps.y,pHeadE->pe.y,pHeadE->ts,pHeadE->te);
		tb=Interpolation(pCurrentB->ScanLine,pHeadE->pNext->ps.y,pHeadE->pNext->pe.y,pHeadE->pNext->ts,pHeadE->pNext->te);
		BOOL bInFlag=FALSE;//����������Ա�־����ʼֵΪ�ٱ�ʾ�����ⲿ
		double xb,xe;//ɨ���ߺ���Ч���ཻ����������յ�����
		for(pT1=pHeadE;pT1!=NULL;pT1=pT1->pNext)
		{
			if(FALSE==bInFlag)
			{
				xb=pT1->x;
				z=-(xb*A+pCurrentB->ScanLine*B+D)/C;//z=-(Ax+By+D)/C
				bInFlag=TRUE;
			}
			else
			{
				xe=pT1->x;
				for(double x=xb;x<xe;x++)//����ҿ�
				{
					nf=Interpolation(x,xb,xe,na,nb);
					tf=Interpolation(x,xb,xe,ta,tb);
					CRGB Textureclr=GetTextureColor(tf.u,tf.v);
					pMaterial->SetDiffuse(Textureclr);//��������ɫ��Ϊ���ʵ�������ⷴ����
					pMaterial->SetAmbient(Textureclr);//��������ɫ��Ϊ���ʵĻ����ⷴ����
					CRGB c=pLight->Lighting(ViewPoint,CP3(Round(x),pCurrentB->ScanLine,z),nf,pMaterial);
					if(z<=zBuffer[Round(x)+Width/2][pCurrentB->ScanLine+Height/2])//����²���������С��ԭ����������
					{
					   zBuffer[Round(x)+Width/2][pCurrentB->ScanLine+Height/2]=z;//xy�����������±걣��һ��
					   pDC->SetPixelV(Round(x),pCurrentB->ScanLine,RGB(c.red*255,c.green*255,c.blue*255));
					}
					z+=zStep;
				}
				bInFlag=FALSE;
			}
		}
		for(pT1=pHeadE;pT1!=NULL;pT1=pT1->pNext)//�ߵ�������
			pT1->x=pT1->x+pT1->k;
	}
}

void CZBuffer::AddEt(CAET *pNewEdge)//�ϲ�ET��
{
	CAET *pCE;
	pCE=pHeadE;
	if(pCE==NULL)
	{
		pHeadE=pNewEdge;
		pCE=pHeadE;
	}
	else
	{
		while(pCE->pNext!=NULL)
		{
			pCE=pCE->pNext;
		}
		pCE->pNext=pNewEdge;
	}
}

void CZBuffer::ETOrder()//�߱��ð�������㷨
{
	CAET *pT1,*pT2;
	int Count=1;
	pT1=pHeadE;
	if(pT1==NULL)
		return;
	if(pT1->pNext==NULL)//�����ET��û������ET��
		return;//Ͱ���ֻ��һ���ߣ�����Ҫ����
	while(pT1->pNext!=NULL)//ͳ�Ʊ߽��ĸ���
	{
		Count++;
		pT1=pT1->pNext;
	}
	for(int i=0;i<Count-1;i++)//ð������
	{
		CAET **pPre=&pHeadE;
		pT1=pHeadE;
		for(int j=0;j<Count-1-i;j++)
		{
			pT2=pT1->pNext;
		
			if ((pT1->x>pT2->x)||((pT1->x==pT2->x)&&(pT1->k>pT2->k)))
			{
				pT1->pNext=pT2->pNext;
				pT2->pNext=pT1;
				*pPre=pT2;
				pPre=&(pT2->pNext);//����λ��Ϊ�´α���׼��
			}
			else
			{
				pPre=&(pT1->pNext);
				pT1=pT1->pNext;
			}
		}
	}
}

CRGB CZBuffer::Interpolation(double t,double t1,double t2,CRGB clr1,CRGB clr2)//��ɫ���Բ�ֵ
{
	CRGB color;
	color=(t-t2)/(t1-t2)*clr1+(t-t1)/(t2-t1)*clr2;
	return color;
}

CVector CZBuffer::Interpolation(double t,double t1,double t2,CVector vec1,CVector vec2)//ʸ�����Բ�ֵ
{
	CVector vector;
	vector=(t-t2)/(t1-t2)*vec1+(t-t1)/(t2-t1)*vec2;
	return vector;
}

CT2 CZBuffer::Interpolation(double t,double t1,double t2,CT2 tex1,CT2 tex2)//�������Բ�ֵ
{
	CT2 texture;
	texture=(t-t2)/(t1-t2)*tex1+(t-t1)/(t2-t1)*tex2;
	return texture;
}

CRGB CZBuffer::GetTextureColor(double u,double v)//���������ɫ
{
	if(0==(int(floor(u*8.0))+int(floor(v*8.0)))%2)
		return CRGB(0.1,0.1,0.1);
	else
		return CRGB(0.9,0.9,0.9);
}

void CZBuffer::InitDeepBuffer(int Width,int Height,double Depth)//��ʼ����Ȼ���
{
	this->Width=Width,this->Height=Height;
	zBuffer=new double *[Width];
	for(int i=0;i<Width;i++)
		zBuffer[i]=new double[Height];
	for(i=0;i<Width;i++)//��ʼ����Ȼ���
		for(int j=0;j<Height;j++)
			zBuffer[i][j]=Depth;
}

void CZBuffer::ClearMemory()
{
	DeleteAETChain(pHeadE);
	CBucket *pBucket=pHeadB;
	while (pBucket !=NULL)//���ÿһ��Ͱ
	{
		CBucket * pBucketTemp = pBucket->pNext;
		DeleteAETChain(pBucket->pET);
		delete pBucket;
		pBucket=pBucketTemp;
	}
	pHeadB=NULL;
	pHeadE=NULL;
	if(P!=NULL)
	{
		delete []P;
		P=NULL;
	}
	if(N!=NULL)
	{
		delete []N;
		N=NULL;
	}
	if (T!=NULL)
	{
		delete []T;
		T=NULL;
	}
}

void CZBuffer::DeleteAETChain(CAET* pAET)
{
	while (pAET!=NULL)
	{
		CAET* pAETTemp=pAET->pNext;
		delete pAET;
		pAET=pAETTemp;
	}
}