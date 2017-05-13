// TestView.h : interface of the CTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTVIEW_H__7B09737C_A6B6_4402_B2B8_087738721CF6__INCLUDED_)
#define AFX_TESTVIEW_H__7B09737C_A6B6_4402_B2B8_087738721CF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Face.h"//������ͷ�ļ�
#include "Line.h"//����ֱ��ͷ�ļ�
#include "P3.h"//����ֱ��ͷ�ļ�

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
	void ReadPoint();//���붥�㺯��
	void ReadFace();//�����溯��
	void DoubleBuffer(CDC* pDC);//˫�����ͼ
	void DrawObject(CDC *,CP3 []);//����б�Ȳ�ͼ�߿�ģ��
	void DrawTriView(CDC *,CP3 []);//��������ͼ�߿�ģ��
	void Rotate();//��ת�任
	void TVMatrix();//��������ͼ�任����
	void THMatrix();//���븩��ͼ�任����
	void TWMatrix();//�������ͼ�任����
	void TOMatrix();//����б�Ȳ�任����
	void MultiMatrix(double T[][4]);//������� 
	void DrawOblique(CDC *pDC);//����б�Ȳ�ͼ
	void DrawVView(CDC *pDC );//��������ͼ
	void DrawHView(CDC *pDC);//���Ƹ���ͼ
	void DrawWView(CDC *pDC);//���Ʋ���ͼ
	
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
	CP3 P[6];//��ά����
	CP3 PNew[6];//����ͼ����
	CFace F[5];//���
	double TV[4][4];//����ͼ�任����
	double TH[4][4];//����ͼ�任����
	double TW[4][4];//����ͼ�任����
	double TO[4][4];//б�Ȳ�任����	
// Generated message map functions
protected:
	//{{AFX_MSG(CTestView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
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
