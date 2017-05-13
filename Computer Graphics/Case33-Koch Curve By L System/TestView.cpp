// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"

#include "TestDoc.h"
#include "TestView.h"
#define PI 3.1415926//PI�ĺ궨��
#include "InputDlg.h"//�Ի���
#include "math.h"//��ѧͷ�ļ�
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

void CTestView::Initial(int n)//�ķ�ģ��
{	
	Axiom="F";
	Rule="F+F--F+F";
	NewRule=Axiom;
	NewRuleTemp.Empty();
	int Length=NewRule.GetLength();
	for(int i=1;i<=n;i++)//��n��1��ʼ�滻��n��0ʱ������"F"
	{
		for(int j=0;j<Length;j++)//�����滻
		{
			if(Axiom==NewRule[j])
				NewRuleTemp+=Rule;
			else
				NewRuleTemp+=NewRule[j];
		}
		NewRule=NewRuleTemp;
		NewRuleTemp.Empty();
		Length=NewRule.GetLength();
	}	
}
	
void CTestView::Koch(double theta)//��ͼ����
{
	if(NewRule.IsEmpty ())//�ַ����շ���
		return;
	else 
	{
	    CStateNode Currentnode,Nextnode;
  		Currentnode.x=P0.x;
		Currentnode.y=P0.y;
		Currentnode.alpha=0;
		int Len=NewRule.GetLength();
		pDC->MoveTo(Round(Currentnode.x),Round(Currentnode.y));
		for(int i=0;i<Len;i++)
		{
			switch(NewRule[i])//�����ַ����е�ĳ��λ�õ��ַ�
			{
				case 'F'://ȡ��"F"�ַ��Ĳ���
		 			Nextnode.x=Currentnode.x+d*cos(Currentnode.alpha);
					Nextnode.y=Currentnode.y+d*sin(Currentnode.alpha);
					Nextnode.alpha=Currentnode.alpha;
					pDC->LineTo(Round(Nextnode.x),Round(Nextnode.y));
					Currentnode=Nextnode;
					break ;
				case '+'://ȡ��"+"�ַ��Ĳ���
					Currentnode.alpha=Currentnode.alpha+theta;
					break;
				case '-'://ȡ��"-"�ַ��Ĳ���
					Currentnode.alpha=Currentnode.alpha-theta;
					break;
				default:
					break;
			}
		}
	}
}
 
void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	CInputDlg dlg;
	double Theta;
	if(IDOK==dlg.DoModal())
	{
		n=dlg.m_n;
	    Theta=dlg.m_theta;
	}
	else
		return;
	RedrawWindow();
	pDC=GetDC();
	CRect rect;
	GetClientRect(&rect);	
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(),rect.Height());
	pDC->SetViewportExt(rect.Width(),-rect.Height());
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);
	rect.OffsetRect(-rect.Width()/2,-rect.Height()/2);
	P0.x=-rect.Width()/2.0,P0.y=-rect.Height()/2.0+100;
	P1.x=rect.Width()/2.0,P1.y=-rect.Height()/2.0+100;
	L0=sqrt((P1.x-P0.x)*(P1.x-P0.x)+(P1.y-P0.y)*(P1.y-P0.y));
	Theta=Theta*PI/180;
	d=L0/pow(2*(1+cos(Theta)),n);//����
	Initial(n);	
	Koch(Theta);
	ReleaseDC(pDC);
}
