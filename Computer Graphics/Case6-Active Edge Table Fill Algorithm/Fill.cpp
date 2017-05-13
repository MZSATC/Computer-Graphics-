// Fill.cpp: implementation of the CFill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "Fill.h"
#include "math.h"
#define Round(f) int(floor(f+0.5))//��������궨��
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFill::CFill()
{
	PNum=0;
	P=NULL;
	pEdge=NULL;
	pHeadB=NULL;
	pHeadE=NULL;
}

CFill::~CFill()
{
	if(P!=NULL)
	{
		delete[] P;
		P=NULL;
	}
	ClearMemory();
}

void CFill::SetPoint(CPi2 *p,int m)
{
	P=new CPi2[m];//����һά��̬����
	for(int i=0;i<m;i++)
	{
		P[i]=p[i];	
	}
	PNum=m;
}

void CFill::CreateBucket()//����Ͱ��
{
	int yMin,yMax;
	yMin=yMax=P[0].y;
	for(int i=0;i<PNum;i++)//���Ҷ���������ǵ���С�����ɨ����
	{
		if(P[i].y<yMin)
		{
			yMin=P[i].y;//ɨ���ߵ���Сֵ
		}
		if(P[i].y>yMax)
		{
			yMax=P[i].y;//ɨ���ߵ����ֵ
		}
	}
	for(int y=yMin;y<=yMax;y++)
	{
		if(yMin==y)//�����ɨ���ߵ���Сֵ
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

void CFill::CreateEdge()//�����߱�
{
	for(int i=0;i<PNum;i++)
	{
		pCurrentB=pHeadB;
		int j=(i+1)%PNum;//�ߵ���һ�����㣬P[i]��P[j]��Թ��ɱ�
		if(P[i].y<P[j].y)//�ߵ��յ������
		{
			pEdge=new CAET;
			pEdge->x=P[i].x;//����ET���ֵ
			pEdge->yMax=P[j].y;
			pEdge->k=(P[j].x-P[i].x)/(P[j].y-P[i].y);//����1/k
			pEdge->ps=P[i];//�󶨶������ɫ
			pEdge->pe=P[j];
			pEdge->pNext=NULL;
			while(pCurrentB->ScanLine!=P[i].y)//��Ͱ��Ѱ�ҵ�ǰ�ߵ�yMin
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
			pEdge->pNext=NULL;
			while(pCurrentB->ScanLine!=P[j].y)
			{
				pCurrentB=pCurrentB->pNext;
			}
		}
		if(P[i].y!=P[j].y)
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

void CFill::Gouraud(CDC *pDC)//�������
{
	CAET *pT1=NULL,*pT2=NULL;
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
			pEdge->pNext=NULL;
			AddET(pEdge);
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
				CAET *pAETTemp =pT1;
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
		BOOL bInFlag=FALSE;//����������Ա�־����ʼֵΪ�ٱ�ʾ�����ⲿ
		double xb,xe;//ɨ��������Ч���ཻ����������յ�����
		for(pT1=pHeadE;pT1!=NULL;pT1=pT1->pNext)
		{
			if(FALSE==bInFlag)
			{
				xb=pT1->x;
				bInFlag=TRUE;
			}
			else
			{
				xe=pT1->x;
				for(double x=xb;x<xe;x++)//����ҿ�
				{
					pDC->SetPixelV(Round(x),pCurrentB->ScanLine,RGB(pT1->ps.c.red*255,pT1->ps.c.green*255,pT1->ps.c.blue*255));
				}
				bInFlag=FALSE;
			}
		}
		for(pT1=pHeadE;pT1!=NULL;pT1=pT1->pNext)//�ߵ�������
		{
			pT1->x=pT1->x+pT1->k;
		}
	}
}

void CFill::AddET(CAET *pNewEdge)//�ϲ�ET��
{
	CAET *pCE=pHeadE;
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

void CFill::ETOrder()//�߱��ð�������㷨
{
	CAET *pT1,*pT2;
	int Count=1;
	pT1=pHeadE;
	if(pT1==NULL)
	{
		return;
	}
	if(pT1->pNext==NULL)//�����ET��û������ET��
	{
		return;//Ͱ���ֻ��һ���ߣ�����Ҫ����
	}
	while(pT1->pNext!=NULL)//ͳ�Ʊ߽��ĸ���
	{
		Count++;
		pT1=pT1->pNext;
	}
	for(int i=0;i<Count-1;i++)//ð������
	{
		CAET **pPre=&pHeadE;
		pT1=pHeadE;
		for (int j=0;j<Count-1-i;j++)
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

void CFill::ClearMemory()//��ȫɾ������Ͱ��Ͱ�����ӵı�
{
	DeleteAETChain(pHeadE);
	CBucket *pBucket=pHeadB;
	while (pBucket!=NULL)//���ÿһ��Ͱ
	{
		CBucket *pBucketTemp=pBucket->pNext;
		DeleteAETChain(pBucket->pET);
		delete pBucket;
		pBucket=pBucketTemp;
	}
	pHeadB=NULL;
	pHeadE=NULL;
}

void CFill::DeleteAETChain(CAET *pAET)
{
	while (pAET!=NULL)
	{
		CAET *pAETTemp=pAET->pNext;
		delete pAET;
		pAET=pAETTemp;
	}
}
