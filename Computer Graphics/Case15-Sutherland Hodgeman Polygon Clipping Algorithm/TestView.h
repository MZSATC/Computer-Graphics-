// TestView.h : interface of the CTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTVIEW_H__7B09737C_A6B6_4402_B2B8_087738721CF6__INCLUDED_)
#define AFX_TESTVIEW_H__7B09737C_A6B6_4402_B2B8_087738721CF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "P2.h"//��������

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
	void DoubleBuffer(CDC *);//˫����
	void ReadPoint();//������
	void DrawWindowRect(CDC *);//���Ʋü�����
	void DrawObject(CDC *,BOOL);//���ƶ����
	void ClipBoundary(CP2 ,CP2);//���ڱ߽縳ֵ����
	void ClipPolygon(CP2 *,int ,UINT);//�ü������
	BOOL Inside(CP2 ,UINT);//���ڴ��ڱ߽����жϺ���
	CP2  Intersect(CP2 p0,CP2 p1,UINT Boundary);//���㽻�㺯��
	CP2  Convert(CPoint point);//����ϵת��
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
	CP2 *In,*Out;//�ü�ǰ��Ķ�������
	CP2 Rect[2];//���ڶ�������
	int OutCount;//�ü���Ķ��������ж������
    int RtCount;//���ڶ������
	double	Wxl,Wxr,Wyb,Wyt;//���ô��ڱ߽�
	BOOL bDrawRect;//�Ƿ�������ƴ���
	BOOL bClip;//�Ƿ�����ü�
// Generated message map functions
protected:
	//{{AFX_MSG(CTestView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDrawpic();
	afx_msg void OnClip();
	afx_msg void OnUpdateClip(CCmdUI* pCmdUI);
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
