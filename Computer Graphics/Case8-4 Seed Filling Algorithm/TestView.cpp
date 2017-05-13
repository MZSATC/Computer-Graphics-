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
	ON_WM_LBUTTONDOWN()
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
	COLORREF BoundaryClr=RGB(0,0,0);//�߽�ɫ
	COLORREF PixelClr;//��ǰ���ص���ɫ
	pHead=new CStackNode;//����ջͷ���
	pHead->pNext=NULL;//ջͷ����ָ������Ϊ��
	Push(Seed);	//����������ջ
	int x,y,x0=Round(Seed.x),y0=Round(Seed.y);//x��y�����ж�������ͼ�ε�λ�ù�ϵ
	x=x0-1;
	while(BoundaryClr!=pDC->GetPixel(x,y0) && SeedClr!=pDC->GetPixel(x,y0))//���ж�
	{
		x--;
		if(x<=-rect.Width()/2)//����ͻ��������
		{
			MessageBox("���Ӳ���ͼ��֮��","����");//������ִ˶Ի��򵯳�������ע��
			return;
		}
	}
	y=y0+1;
	while(BoundaryClr!=pDC->GetPixel(x0,y) && SeedClr!=pDC->GetPixel(x0,y))//�Ϸ��ж�
	{
		y++;
		if(y>=rect.Height()/2)//����ͻ������϶�
		{
			MessageBox("���Ӳ���ͼ��֮��","����");//������ִ˶Ի��򵯳�������ע��
			return;
		}
	}
	x=x0+1;
	while(BoundaryClr!=pDC->GetPixel(x,y0) && SeedClr!=pDC->GetPixel(x,y0))//�ҷ��ж�
	{
		x++;
		if(x>=rect.Width()/2)//����ͻ������Ҷ�
		{
			MessageBox("���Ӳ���ͼ��֮��","����");//������ִ˶Ի��򵯳�������ע��		
			return;
		}
	}
	y=y0-1;
	while(BoundaryClr!=pDC->GetPixel(x0,y) && SeedClr!=pDC->GetPixel(x0,y))//�·��ж�
	{
		y--;
		if(y<=-rect.Height()/2)//����ͻ������¶�
		{
			MessageBox("���Ӳ���ͼ��֮��","����");//������ִ˶Ի��򵯳�������ע��
			return;
		}
	}
	while(NULL!=pHead->pNext)//���ջ��Ϊ��
	{
		CP2 PopPoint;
		Pop(PopPoint);
		if(SeedClr==pDC->GetPixel(Round(PopPoint.x),Round(PopPoint.y)))
			continue;//����
		pDC->SetPixelV(Round(PopPoint.x),Round(PopPoint.y),SeedClr);
		PointLeft.x=PopPoint.x-1;//������ջ����������
		PointLeft.y=PopPoint.y;
		PixelClr=pDC->GetPixel(Round(PointLeft.x),Round(PointLeft.y));
		if(BoundaryClr!=PixelClr && SeedClr!=PixelClr)//���Ǳ߽�ɫ����δ�ó����ɫ
			Push(PointLeft);//��������ջ
		PointTop.x=PopPoint.x;
		PointTop.y=PopPoint.y+1;//������ջ�����Ϸ�����
		PixelClr=pDC->GetPixel(Round(PointTop.x),Round(PointTop.y));
		if(BoundaryClr!=PixelClr && SeedClr!=PixelClr)
			Push(PointTop);	//�Ϸ�������ջ
		PointRight.x=PopPoint.x+1;//������ջ�����ҷ�����
		PointRight.y=PopPoint.y;
		PixelClr=pDC->GetPixel(Round(PointRight.x),Round(PointRight.y));
		if(BoundaryClr!=PixelClr && SeedClr!=PixelClr)
			Push(PointRight);//�ҷ�������ջ	
		PointBottom.x=PopPoint.x;
		PointBottom.y=PopPoint.y-1;//������ջ�����·�����
		PixelClr=pDC->GetPixel(Round(PointBottom.x),Round(PointBottom.y));
		if(BoundaryClr!=PixelClr && SeedClr!=PixelClr)
			Push(PointBottom);//�·�������ջ
	}
	pDC->TextOut(rect.left+50,rect.bottom-20,"������");
	delete pHead;
	pHead = NULL;
}

void CTestView::Push(CP2 point)//��ջ����
{
	pTop=new CStackNode;
	pTop->PixelPoint=point;
	pTop->pNext=pHead->pNext;
	pHead->pNext=pTop;
}

void CTestView::Pop(CP2 &point)//��ջ����
{
	if(pHead->pNext!=NULL)
	{
		pTop=pHead->pNext;
		pHead->pNext=pTop->pNext;
		point=pTop->PixelPoint;
		delete pTop;
	}
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	CColorDialog ccd(SeedClr,CC_SOLIDCOLOR);
	if(IDOK==ccd.DoModal())//������ɫ�Ի���ѡȡ���ɫ
		SeedClr=ccd.GetColor();	
	else
		return;	
	if(IDOK==MessageBox("���������ڲ�ѡȡ���ӵ�","��ʾ",MB_OKCANCEL))
	    bFill=TRUE;
	else
		return;	
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(TRUE==bFill)
	{
		Seed=CP2(point.x-rect.Width()/2,rect.Height()/2-point.y);//ѡ������λ��
		Invalidate(FALSE);
	}
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	bFill=FALSE;
	SeedClr=RGB(0,0,255);
	ReadPoint();
}


