// TestView.h : interface of the CTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTVIEW_H__7B09737C_A6B6_4402_B2B8_087738721CF6__INCLUDED_)
#define AFX_TESTVIEW_H__7B09737C_A6B6_4402_B2B8_087738721CF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Pi2.h"
#include "Face.h"
#include "Vector.h"
#include "Lighting.h"
#include "Material.h"
#include "Transform.h"
#include "ZBuffer.h"

class CTestView : public CView
{
protected: // create from serialization only
	CTestView();
	DECLARE_DYNCREATE(CTestView)

// Attributes
public:
	CTestDoc* GetDocument();

// Operations
public:
	void ReadVertex();//���붥���
	void ReadFace();//�������
	void InitParameter();//�任����
	void PerProject(CP3 P);//͸��ͶӰ
	void DoubleBuffer(CDC* pDC);//˫����
	void DrawObject(CDC* pDC);//��������
	void CalVertexColor();//�������Ĺ�����ɫ
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	double R,Theta,Phi;//�ӵ����û�����ϵ�е�������
	double d;//�Ӿ�
	double Near,Far;//Զ��������
	double k[9];
	CP3 ViewPoint;//�ӵ�������λ��
	CPi3 ScreenP;//��Ļ����ϵ����ά�����
	CP3 *V;//��Ķ���һά����
	CFace **F;//��Ķ�ά����
	double Alpha,Beta;//x������ת����,y������ת�½�
	int N1,N2;//N1Ϊγ������,N2Ϊ��������
	int	 LightNum;//��Դ����
	CLighting *pLight;//���ջ���
	CMaterial *pMaterial;//�������
	CTransform tran;//�任����	
	// Generated message map functions
protected:
	//{{AFX_MSG(CTestView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TestView.cpp
inline CTestDoc* CTestView::GetDocument()
   { return (CTestDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTVIEW_H__7B09737C_A6B6_4402_B2B8_087738721CF6__INCLUDED_)
