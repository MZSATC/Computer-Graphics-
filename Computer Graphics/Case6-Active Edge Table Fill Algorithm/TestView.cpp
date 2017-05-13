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
	DrawGraph(pDC);
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

void CTestView::ReadPoint()//���
{
	P[0].x=50;  P[0].y=100;
	P[1].x=-150;P[1].y=300;
	P[2].x=-250;P[2].y=50;
	P[3].x=-150;P[3].y=-250;
	P[4].x=0;   P[4].y=-50;
	P[5].x=100; P[5].y=-250;
	P[6].x=300; P[6].y=150;
}

void CTestView::DrawGraph(CDC *pDC)//����ͼ��
{
	CRect rect;                                         //����ͻ���
	GetClientRect(&rect);                               //��ÿͻ����Ĵ�С
   	pDC->SetMapMode(MM_ANISOTROPIC);                    //�Զ�������ϵ
	pDC->SetWindowExt(rect.Width(),rect.Height());      //���ô��ڱ���
	pDC->SetViewportExt(rect.Width(),-rect.Height());   //����������������x��ˮƽ���ң�y�ᴹֱ����
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);//���ÿͻ�������Ϊ����ϵԭ��
	rect.OffsetRect(-rect.Width()/2,-rect.Height()/2);  //������ͻ����غ�
	if(!bFill)
		DrawPolygon(pDC);//���ƶ����
	else
		FillPolygon(pDC);//�������
}

void CTestView::DrawPolygon(CDC *pDC)//���ƶ���α߽�
{
	CLine *line=new CLine;
	CP2 t;
	for(int i=0;i<7;i++)//���ƶ����
	{
		if(i==0)
		{
			line->MoveTo(pDC,P[i]);
			t=P[i];
		}
		else
		{
			line->LineTo(pDC,P[i]);
		}		
	}
	line->LineTo(pDC,t);//�պ϶����
	delete line;
}

void CTestView::FillPolygon(CDC *pDC)//�������
{
	for(int i=0;i<7;i++)//ת���������꣬y����ȡΪ����
	{
		P1[i].x=P[i].x;
		P1[i].y=Round(P[i].y);
		P1[i].c=CRGB(bRed/255.0,bGreen/255.0,bBlue/255.0);
	}
	CFill *fill=new CFill;      //��̬�����ڴ� 
	fill->SetPoint(P1,7);       //��ʼ��Fill����
	fill->CreateBucket();       //����Ͱ��
	fill->CreateEdge();         //�����߱�
    fill->Gouraud(pDC);         //�������
	delete fill;                //�����ڴ�
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	COLORREF GetClr=RGB(0,0,0);//��ɫ����ɫ
	CColorDialog ccd(GetClr,CC_SOLIDCOLOR);
	if(IDOK==ccd.DoModal())//������ɫ�Ի���ѡȡ���ɫ
		GetClr=ccd.GetColor();	
	else
		return;
	bRed=GetRValue(GetClr);  //��ȡ��ɫ����
	bGreen=GetGValue(GetClr);//��ȡ��ɫ����
	bBlue=GetBValue(GetClr); //��ȡ��ɫ����
	bFill=TRUE;
	Invalidate();	
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	bFill=FALSE;
	ReadPoint();
}
