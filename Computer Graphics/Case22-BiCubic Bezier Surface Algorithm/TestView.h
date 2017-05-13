// TestView.h : interface of the CTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTVIEW_H__A75FDCFB_621C_4E38_A154_C344803E6372__INCLUDED_)
#define AFX_TESTVIEW_H__A75FDCFB_621C_4E38_A154_C344803E6372__INCLUDED_
#include "P3.h"//������ά�������
#include "P2.h"//������ά�������
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


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
	void ReadPoint();//������������εĶ���
	void ObliqueProjection();//б�Ȳ�ͶӰ
	void LeftMultiMatrix(double M0[][4],CP3 P0[][4]); //��˾���
	void RightMultiMatrix(CP3 P0[][4],double M0[][4]);//�ҳ˾���
	void TransposeMatrix(double M0[][4]);//����ת��
	void DoubleBuffer(CDC* pDC);//˫�����ͼ
	void DrawObject(CDC* pDC);//����Bezier����
	void DrawCtrlPolygon(CDC* pDC);//���ƿ��ƶ����
	void SignCtrPoint(CDC* pDC);//��ע���Ƶ�
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
	CP3 P3[4][4];//��ά����
	CP2 P2[4][4];//��ά����
	double MT[4][4];//M��ת�þ���
// Generated message map functions
protected:
	//{{AFX_MSG(CTestView)
	afx_msg void OnDrawpic();
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
