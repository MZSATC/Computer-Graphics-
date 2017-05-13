// TestView.h : interface of the CTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTVIEW_H__A75FDCFB_621C_4E38_A154_C344803E6372__INCLUDED_)
#define AFX_TESTVIEW_H__A75FDCFB_621C_4E38_A154_C344803E6372__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Face.h"
#include "Vector.h"
#include "ZBuffer.h"
#include "Lighting.h"
#include "Transform.h"

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
	void ReadImage(int);//��������
	void ReadVertex();//������
	void ReadFace();//�������
	void InitParameter();//������ʼ��
	void PerProject(CP3 P);//͸��ͶӰ
	void DoubleBuffer(CDC* pDC);//˫�����ͼ
	void DrawObject(CDC* pDC);//���Ƴ��������
	void ClearImageMemory();//����λͼ�ڴ�
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
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CP3 V[8];//���
	CFace F[6];//���
	double R,Theta,Phi;//�ӵ����û�����ϵ�е�������
	double d;//�Ӿ�
	double Near,Far;//Զ��������
	double k[9];//���㳣��
	CP3 ViewPoint;//�ӵ�������
	CPi3 ScreenP;//��Ļ����ϵ����ά�����
	double Alpha,Beta;//x������ת����,y������ת�½�
	int LightNum;//��Դ����
	CLighting *pLight;//���ջ���
	CMaterial *pMaterial;//�������
	BOOL bPlay;//��������
	COLORREF **Image;//��ά��̬����
	CTransform tran;//�任����
	BITMAP bmp;//BITMAP�ṹ�����
	BYTE *im;
	// Generated message map functions
protected:
	//{{AFX_MSG(CTestView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPlay();
	afx_msg void OnUpdatePlay(CCmdUI* pCmdUI);
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

#endif // !defined(AFX_TESTVIEW_H__A75FDCFB_621C_4E38_A154_C344803E6372__INCLUDED_)
