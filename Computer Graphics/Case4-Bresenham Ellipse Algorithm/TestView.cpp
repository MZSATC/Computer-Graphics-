// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#include "math.h"
#define Round(d) int(floor(d+0.5))//��������궨��
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
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
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
	GetClientRect(&rect);//��ÿͻ������εĴ�С
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

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	p0.x=point.x;
	p0.y=point.y;
	p0.x=p0.x-rect.Width()/2;                           //�豸����ϵ���Զ�������ϵϵת��
	p0.y=rect.Height()/2-p0.y;
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	p1.x=point.x;
	p1.y=point.y;
	CDC *pDC=GetDC();                                   //�����豸������ָ��
	pDC->SetMapMode(MM_ANISOTROPIC);                    //�Զ�������ϵ
	pDC->SetWindowExt(rect.Width(),rect.Height());      //���ô��ڱ���
	pDC->SetViewportExt(rect.Width(),-rect.Height());   //����������������x��ˮƽ���ң�y�ᴹֱ����
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);//���ÿͻ�������Ϊ����ϵԭ��
	rect.OffsetRect(-rect.Width()/2,-rect.Height()/2);  //������ͻ����غ�
	p1.x=p1.x-rect.Width()/2;
	p1.y=rect.Height()/2-p1.y;
	MBEllipse(pDC);
	ReleaseDC(pDC);
	CView::OnLButtonUp(nFlags, point);
}

void CTestView::MBEllipse(CDC *pDC)//��Բ�е�Bresenham�㷨
{
	double x,y,d1,d2,a,b;
	a=fabs(p1.x-p0.x)/2;
	b=fabs(p1.y-p0.y)/2;
	x=0;y=b;
	d1=b*b+a*a*(-b+0.25);
	EllipsePoint(x,y,pDC);
	while(b*b*(x+1)<a*a*(y-0.5))//��ԲAC����
	{
		if (d1<0)
		{
			d1+=b*b*(2*x+3);
		}
		else
		{
			d1+=b*b*(2*x+3)+a*a*(-2*y+2);
			y--;
		}
		x++;
		EllipsePoint(x,y,pDC);
	}
	d2=b*b*(x+0.5)*(x+0.5)+a*a*(y-1)*(y-1)-a*a*b*b;//��ԲCB����
	while(y>0)
	{
		if (d2<0)
		{
			d2+=b*b*(2*x+2)+a*a*(-2*y+3);
			x++;
		}
		else
		{
			d2+=a*a*(-2*y+3);
		}
		y--;
		EllipsePoint(x,y,pDC);
	}
}

void CTestView::EllipsePoint(double x, double y,CDC *pDC)//�ķַ�����Բ�Ӻ���
{
	CP2 pc=CP2((p0.x+p1.x)/2.0,(p0.y+p1.y)/2.0);        //��Բ��������
	COLORREF  clr=RGB(0,0,255);                         //������Բ����ɫ
	pDC->SetPixelV(Round(x+pc.x),Round(y+pc.y),clr);
	pDC->SetPixelV(Round(-x+pc.x),Round(y+pc.y),clr);
	pDC->SetPixelV(Round(x+pc.x),Round(-y+pc.y),clr);
	pDC->SetPixelV(Round(-x+pc.x),Round(-y+pc.y),clr);
	pDC->MoveTo(Round(p0.x),Round(p0.y));
	//pDC->LineTo(Round(p1.x),Round(p0.y));
	//pDC->LineTo(Round(p1.x),Round(p1.y));
	//pDC->LineTo(Round(p0.x),Round(p1.y));
	//pDC->LineTo(Round(p0.x),Round(p0.y));
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	MessageBox("�밴����������ͼ��","��ʾ",MB_ICONEXCLAMATION|MB_OK);
	RedrawWindow();
}

