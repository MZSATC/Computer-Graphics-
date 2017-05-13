// Fill.h: interface for the CFill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILL_H__EDD87BAB_FC56_479F_9769_F92E81FDBCAC__INCLUDED_)
#define AFX_FILL_H__EDD87BAB_FC56_479F_9769_F92E81FDBCAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Bucket.h"

class CFill  
{
public:
	CFill();
	virtual ~CFill();
	void SetPoint(CPi2 *p,int);//��ʼ��
	void CreateBucket();//����Ͱ
	void CreateEdge();//�߱�
	void AddET(CAET *);//�ϲ�ET��
	void ETOrder();//ET������
	void Gouraud(CDC *);//�������
 	void ClearMemory();//�����ڴ�
	void DeleteAETChain(CAET* pAET);//ɾ���߱�
protected:
	int     PNum;//�������
	CPi2    *P;//�������궯̬����
	CAET    *pHeadE,*pCurrentE,*pEdge; //��Ч�߱���ָ��
	CBucket *pHeadB,*pCurrentB;        //Ͱ����ָ��
};

#endif // !defined(AFX_FILL_H__EDD87BAB_FC56_479F_9769_F92E81FDBCAC__INCLUDED_)
