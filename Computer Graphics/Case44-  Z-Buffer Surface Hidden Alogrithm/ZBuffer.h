// ZBuffer.h: interface for the CZBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZBUFFER_H__ED93E842_7399_49B8_B579_6A52FBA1289D__INCLUDED_)
#define AFX_ZBUFFER_H__ED93E842_7399_49B8_B579_6A52FBA1289D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Bucket.h"//Ͱͷ�ļ�
#include "Vector.h"//ʸ��ͷ�ļ�

class CZBuffer  
{
public:
	CZBuffer();
    virtual ~CZBuffer();
	void CreateBucket();//����Ͱ
	void CreateEdge();//�߱�
	void AddEt(CAET *);//�ϲ�ET��
	void ETOrder();
	void Gouraud(CDC *);//���
	void InitDeepBuffer(int,int,double);//��ʼ����Ȼ�����
    CRGB Interpolation(double,double,double,CRGB,CRGB);//���Բ�ֵ
	void SetPoint(CPi3 *,int);
	void ClearMemory();//�����ڴ�
	void DeleteAETChain(CAET* pAET);//ɾ���߱�
protected:
	int PNum;//�������
	CPi3 *P;//��������
	CAET *pHeadE,*pCurrentE,*pEdge;//��Ч�߱���ָ��
	CBucket *pCurrentB,*pHeadB;
	double **zBuffer;//����ȳ���
	int Width,Height;//����������
};

#endif // !defined(AFX_ZBUFFER_H__ED93E842_7399_49B8_B579_6A52FBA1289D__INCLUDED_)
