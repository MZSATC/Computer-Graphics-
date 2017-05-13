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

void CTestView::DrawPolygon(CDC *pDC) 
{
	for(int i=0;i<7;i++)                         //�������α߽�
	{
		if(P[i].x>MaxX)
			MaxX=P[i].x;
		if(P[i].x<MinX)
			MinX=P[i].x;
		if(P[i].y>MaxY)
			MaxY=P[i].y;
		if(P[i].y<MinY)
			MinY=P[i].y;
	}
	CLine *line=new CLine;
	CP2 t;
	for(i=0;i<7;i++)                          //���ƶ����
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
	line->LineTo(pDC,t);                        //�պ϶����
	line->MoveTo(pDC,CP2(MinX,MinY));           //���ư�Χ��
	line->LineTo(pDC,CP2(MinX,MaxY));
	line->LineTo(pDC,CP2(MaxX,MaxY));
	line->LineTo(pDC,CP2(MaxX,MinY));
	line->LineTo(pDC,CP2(MinX,MinY));	
	delete line;
}

void CTestView::FillPolygon(CDC *pDC) 
{
	COLORREF BClr=RGB(255,255,255);//����ɫ
	COLORREF FClr=GetClr;//���ɫ
	int ymin,ymax;//�ߵ���Сyֵ�����yֵ
	double x,y,k;//x,y��ǰ�㣬kб�ʵĵ���
	for(int i=0;i<7;i++)//ѭ����������б�
	{
		int j=(i+1)%7;
		k=(P[i].x-P[j].x)/(P[i].y-P[j].y);//����1/k
		if(P[i].y<P[j].y)//�õ�ÿ����y�����ֵ����Сֵ
		{
			ymin=Round(P[i].y);
			ymax=Round(P[j].y);
			x=P[i].x;//�õ�x|ymin
		}
		else
		{
			ymin=Round(P[j].y);
			ymax=Round(P[i].y);
			x=P[j].x;
		}
		for(y=ymin;y<ymax;y++)//��ÿһ����ѭ��ɨ����
		{
			for(int m=Round(x);m<MaxX;m++)       //��ÿһ��ɨ������ߵĽ�����Ҳ�����ѭ��
			{
				if(FClr==pDC->GetPixel(m,Round(y)))//��������ɫ
					pDC->SetPixelV(m,Round(y),BClr);//��Ϊ����ɫ
				else
					pDC->SetPixelV(m,Round(y),FClr);//��Ϊ���ɫ	
			}
			x+=k;                                   //������һ��ɨ���ߵ�x�������
		}
	}
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	CColorDialog ccd(GetClr,CC_SOLIDCOLOR);
	if(IDOK==ccd.DoModal())//������ɫ�Ի���ѡȡ���ɫ
		GetClr=ccd.GetColor();	
	else
		return;
	bFill=TRUE;
	Invalidate(FALSE);	
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	bFill=FALSE;
	ReadPoint();
	GetClr=RGB(0,0,0);
	MinX=MaxX=P[0].x;
	MinY=MaxY=P[0].y;
}
