// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"

#include "TestDoc.h"
#include "TestView.h"
#include "InputDlg.h"//�Ի���ͷ�ļ�
#include "math.h"//��ѧͷ�ļ�
#define Round(d) int(floor(d+0.5))//��������궨��
#define dx d*cos(PI/4) //б�ߵ�ˮƽͶӰ����
#define dy d*sin(PI/4) //б�ߵĴ�ֱͶӰ����
#define PI 3.1415926
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
	//{{AFX_MSG_MAP(CTestView)
	ON_COMMAND(IDM_DRAWPIC, OnDrawpic)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestView construction/destruction

CTestView::CTestView()
{
	// TODO: add construction code here
	pDC=NULL;
}

CTestView::~CTestView()
{
}

BOOL CTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestView drawing

void CTestView::OnDraw(CDC* pDC)
{
	CTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTestView printing

BOOL CTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTestView diagnostics

#ifdef _DEBUG
void CTestView::AssertValid() const
{
	CView::AssertValid();
}

void CTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestDoc* CTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDoc)));
	return (CTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestView message handlers

void CTestView::Sponge(int n,CP2 p, double d)//���ຯ��
{
	if(0==n)
	{
		CubicFront(p,d);//���������塰ǰ�桱����
		CubicTop(p,d);  //���������塰���桱����
		CubicRight(p,d);//���������塰���桱����
		return;
	}
	d=d/3;
	Sponge(n-1,CP2(p.x-2*d+2*dx,p.y-2*d+2*dy),d);//���1
	Sponge(n-1,CP2(p.x-d+2*dx,p.y-2*d+2*dy),d);//���2
	Sponge(n-1,CP2(p.x+2*dx,p.y-2*d+2*dy),d);//���3
	Sponge(n-1,CP2(p.x-2*d+dx,p.y-2*d+dy),d);//���4
	Sponge(n-1,CP2(p.x+dx,p.y-2*d+dy),d);//���5
	Sponge(n-1,CP2(p.x-2*d,p.y-2*d),d);//���6
	Sponge(n-1,CP2(p.x-d,p.y-2*d),d);//���7
	Sponge(n-1,CP2(p.x,p.y-2*d),d);//���8
	Sponge(n-1,CP2(p.x-2*d+2*dx,p.y-d+2*dy),d);//���9
	Sponge(n-1,CP2(p.x+2*dx,p.y-d+2*dy),d);//���10
	Sponge(n-1,CP2(p.x-2*d,p.y-d),d);//���11
	Sponge(n-1,CP2(p.x,p.y-d),d);//���12
	Sponge(n-1,CP2(p.x-2*d+2*dx,p.y+2*dy),d);//���13
	Sponge(n-1,CP2(p.x-d+2*dx,p.y+2*dy),d);//���14
	Sponge(n-1,CP2(p.x+2*dx,p.y+2*dy),d);//���15
	Sponge(n-1,CP2(p.x-2*d+dx,p.y+dy),d);//���16
	Sponge(n-1,CP2(p.x+dx,p.y+dy),d);//���17
	Sponge(n-1,CP2(p.x-2*d,p.y),d);//���18
	Sponge(n-1,CP2(p.x-d,p.y),d);//���19
	Sponge(n-1,CP2(p.x,p.y),d);//���20	
}

void CTestView::CubicFront(CP2 p, double d)//���������塰ǰ�桱����
{
	CBrush brushFront;
	brushFront.CreateSolidBrush(RGB(254,173,139));
	CBrush *pbrushOld=pDC->SelectObject(&brushFront);
	CP2 front[4];
	front[0]=CP2(p.x,p.y);
	front[1]=CP2(p.x-d,p.y);
	front[2]=CP2(p.x-d,p.y-d);
	front[3]=CP2(p.x,p.y-d);
	pDC->BeginPath();
	pDC->MoveTo(Round(front[0].x),Round(front[0].y));
	pDC->LineTo(Round(front[1].x),Round(front[1].y));
	pDC->LineTo(Round(front[2].x),Round(front[2].y));
	pDC->LineTo(Round(front[3].x),Round(front[3].y));
	pDC->LineTo(Round(front[0].x),Round(front[0].y));
	pDC->EndPath();
	pDC->FillPath();
	pDC->SelectObject(pbrushOld);
	brushFront.DeleteObject();
}

void CTestView::CubicTop(CP2 p, double d)//���������塰���桱����
{
	CBrush brushTop;
	brushTop.CreateSolidBrush(RGB(223,122,79));
	CBrush *pbrushOld=pDC->SelectObject(&brushTop);
	CP2 top[4];
	top[0]=CP2(p.x,p.y);
	top[1]=CP2(p.x+dx,p.y+dx);
	top[2]=CP2(p.x-d+dx,p.y+dy);
	top[3]=CP2(p.x-d,p.y);
	pDC->BeginPath();
	pDC->MoveTo(Round(top[0].x),Round(top[0].y));
	pDC->LineTo(Round(top[1].x),Round(top[1].y));
	pDC->LineTo(Round(top[2].x),Round(top[2].y));
	pDC->LineTo(Round(top[3].x),Round(top[3].y));
	pDC->LineTo(Round(top[0].x),Round(top[0].y));
	pDC->EndPath();
	pDC->FillPath();
	pDC->SelectObject(pbrushOld);
	brushTop.DeleteObject();
}

void CTestView::CubicRight(CP2 p, double d)//���������塰���桱����
{
	CBrush brushRight;
	brushRight.CreateSolidBrush(RGB(177,66,66));
	CBrush *pbrushOld=pDC->SelectObject(&brushRight);
	CP2 right[4];
	right[0]=CP2(p.x,p.y);
	right[1]=CP2(p.x,p.y-d);
	right[2]=CP2(p.x+dx,p.y-d+dy);
	right[3]=CP2(p.x+dx,p.y+dy);
	pDC->BeginPath();
	pDC->MoveTo(Round(right[0].x),Round(right[0].y));
	pDC->LineTo(Round(right[1].x),Round(right[1].y));
	pDC->LineTo(Round(right[2].x),Round(right[2].y));
	pDC->LineTo(Round(right[3].x),Round(right[3].y));
	pDC->LineTo(Round(right[0].x),Round(right[0].y));
	pDC->EndPath();
	pDC->FillPath();
	pDC->SelectObject(pbrushOld);
	brushRight.DeleteObject();
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	CInputDlg dlg;
	if(IDOK==dlg.DoModal())
		n=dlg.m_n;
	else
		return;
	pDC=GetDC();
	CRect rect;
	GetClientRect(&rect);	
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(),rect.Height());
	pDC->SetViewportExt(rect.Width(),-rect.Height());
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);
	rect.OffsetRect(-rect.Width()/2,-rect.Height()/2);
	RedrawWindow();
	CP2 P;//�������
	double d;//������߳�
	d=300.0;
	P=CP2(40,40);
	Sponge(n,P,d);
	ReleaseDC(pDC);
}
