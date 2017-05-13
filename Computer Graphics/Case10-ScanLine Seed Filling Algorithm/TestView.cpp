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
	bFill=FALSE;
	SeedClr=RGB(255,0,0);
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

void CTestView::DrawGraph(CDC *pDC)//����ͼ��
{
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);//��ʾDC�Զ�������ϵ
	pDC->SetWindowExt(rect.Width(),rect.Height());
	pDC->SetViewportExt(rect.Width(),-rect.Height());
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);
	CDC memDC;//����һ���ڴ�DC
	memDC.CreateCompatibleDC(pDC);//����һ������ʾDC���ݵ��ڴ�DC
	CBitmap NewBitmap,*pOldBitmap; 
	NewBitmap.LoadBitmap(IDB_BITMAP2);//����Դ�е�����ĺ��ֵ�λͼ
	BITMAP bmpInfo; //����bmpInfo�ṹ��
	NewBitmap.GetBitmap(&bmpInfo);//��ȡλͼ��Ϣ
	pOldBitmap=memDC.SelectObject(&NewBitmap);//��λͼѡ���ڴ�DC
	memDC.SetMapMode(MM_ANISOTROPIC);//�ڴ�DC�Զ�������ϵ
	memDC.SetWindowExt(bmpInfo.bmWidth,bmpInfo.bmHeight);
	memDC.SetViewportExt(bmpInfo.bmWidth,-bmpInfo.bmHeight);
	memDC.SetViewportOrg(bmpInfo.bmWidth/2,bmpInfo.bmHeight/2);
	rect.OffsetRect(-rect.Width()/2,-rect.Height()/2);
	int nX=rect.left+(rect.Width()-bmpInfo.bmWidth)/2;//����λͼ�ڿͻ��������ĵ�
    int nY=rect.top+(rect.Height()-bmpInfo.bmHeight)/2;
	pDC->BitBlt(nX,nY,rect.Width(),rect.Height(),&memDC,-bmpInfo.bmWidth/2,-bmpInfo.bmHeight/2,SRCCOPY);//���ڴ�DC�е�λͼ�������豸DC
	if(bFill)
		CharFill(pDC);//�����ĺ���
	memDC.SelectObject(pOldBitmap);//���ڴ�DC���ͷ�λͼ
	memDC.DeleteDC();//ɾ��memDC
}

void CTestView::CharFill(CDC *pDC)//������亯��
{
	COLORREF BoundaryClr=RGB(0,0,0);//�߽�ɫ
	BOOL bSpanFill;
	pHead=new CStackNode;//����ջ���
	pHead->pNext=NULL;//ջͷ����ָ������Ϊ��
	Push(Seed);//����������ջ
	int x,y,x0=Round(Seed.x),y0=Round(Seed.y);//x��y�����ж�������ͼ�ε�λ�ù�ϵ
	x=x0-1;
	while(pDC->GetPixel(x,y0)!=BoundaryClr && pDC->GetPixel(x,y0)!=SeedClr)//���ж�
	{
		x--;
		if(x<=-rect.Width()/2)
		{
			MessageBox("���Ӳ���ͼ��֮��","����");//����ͻ��������
			return;
		}
	}
	y=y0+1;
	while(pDC->GetPixel(x0,y)!=BoundaryClr && pDC->GetPixel(x0,y)!=SeedClr)//�Ϸ��ж�
	{
		y++;
		if(y>=rect.Height()/2)//����ͻ������϶�
		{
			MessageBox("���Ӳ���ͼ��֮��","����");
			return;
		}
	}
	x=x0+1;
	while(pDC->GetPixel(x,y0)!=BoundaryClr && pDC->GetPixel(x,y0)!=SeedClr)//�ҷ��ж�
	{
		x++;
		if(x>=rect.Width()/2)//����ͻ������Ҷ�
		{
			MessageBox("���Ӳ���ͼ��֮��","����");		
			return;
		}
	}
	y=y0-1;
	while(pDC->GetPixel(x0,y)!=BoundaryClr && pDC->GetPixel(x0,y)!=SeedClr)//�·��ж�
	{
		y--;
		if(y<=-rect.Height()/2)//����ͻ������¶�
		{
			MessageBox("���Ӳ���ͼ��֮��","����");
			return;
		}
	}
	double xleft,xright;//��������������Ҷ�����
	CP2 PopPoint,PointTemp;
	while(pHead->pNext!=NULL)//���ջ��Ϊ��
	{
		Pop(PopPoint);
		if(pDC->GetPixel(Round(PopPoint.x),Round(PopPoint.y))==SeedClr)
			continue;
		PointTemp=PopPoint;
		while(pDC->GetPixel(Round(PointTemp.x),Round(PointTemp.y))!=BoundaryClr && pDC->GetPixel(Round(PointTemp.x),Round(PointTemp.y))!=SeedClr)
		{
			pDC->SetPixelV(Round(PointTemp.x),Round(PointTemp.y),SeedClr);
			PointTemp.x++;
		}
		xright=PointTemp.x-1;
		PointTemp.x=PopPoint.x-1;
		while(pDC->GetPixel(Round(PointTemp.x),Round(PointTemp.y))!=BoundaryClr && pDC->GetPixel(Round(PointTemp.x),Round(PointTemp.y))!=SeedClr)
		{
			pDC->SetPixelV(Round(PointTemp.x),Round(PointTemp.y),SeedClr);
			PointTemp.x--;
		}
		xleft=PointTemp.x+1;
		//������һ��ɨ����
		PointTemp.x=xleft;
		PointTemp.y=PointTemp.y+1;
		while(PointTemp.x<xright)
		{
			bSpanFill=FALSE;
			while(pDC->GetPixel(Round(PointTemp.x),Round(PointTemp.y))!=BoundaryClr && pDC->GetPixel(Round(PointTemp.x),Round(PointTemp.y))!=SeedClr)
			{
				bSpanFill=TRUE;
				PointTemp.x++;
			}
			if(bSpanFill)
			{
				if(PointTemp.x==xright && pDC->GetPixel(Round(PointTemp.x),Round(PointTemp.y))!=BoundaryClr && pDC->GetPixel(Round(PointTemp.x),Round(PointTemp.y))!=SeedClr)
					PopPoint=PointTemp;
				else
				{
					PopPoint.x=PointTemp.x-1;
					PopPoint.y=PointTemp.y;
				}
				Push(PopPoint);
				bSpanFill=FALSE;
			}
			while((pDC->GetPixel(Round(PointTemp.x),Round(PointTemp.y))==BoundaryClr && PointTemp.x<xright) || (pDC->GetPixel(Round(PointTemp.x),Round(PointTemp.y))==SeedClr && PointTemp.x<xright))
			    PointTemp.x++;
		}
		//������һ��ɨ����
		PointTemp.x=xleft;
		PointTemp.y=PointTemp.y-2;
		while(PointTemp.x<xright)
		{
			bSpanFill=FALSE;
			while(pDC->GetPixel(Round(PointTemp.x),Round(PointTemp.y))!=BoundaryClr && pDC->GetPixel(Round(PointTemp.x),Round(PointTemp.y))!=SeedClr)
			{
				bSpanFill=TRUE;
				PointTemp.x++;
			}
			if(bSpanFill)
			{
				if(PointTemp.x==xright && pDC->GetPixel(Round(PointTemp.x),Round(PointTemp.y))!=BoundaryClr && pDC->GetPixel(Round(PointTemp.x),Round(PointTemp.y))!=SeedClr)
					PopPoint=PointTemp;
				else
				{
					PopPoint.x=PointTemp.x-1;
					PopPoint.y=PointTemp.y;
				}
				Push(PopPoint);
				bSpanFill=FALSE;
			}
			while((pDC->GetPixel(Round(PointTemp.x),Round(PointTemp.y))==BoundaryClr && PointTemp.x<xright) || (pDC->GetPixel(Round(PointTemp.x),Round(PointTemp.y))==SeedClr && PointTemp.x<xright))
			    PointTemp.x++;
		}
	}
	delete pHead;
	pHead=NULL;
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

void CTestView::OnLButtonDown(UINT nFlags, CPoint point)//������º��� 
{
	// TODO: Add your message handler code here and/or call default
	Seed=CP2(point.x-rect.Width()/2,rect.Height()/2-point.y);//ѡ������λ��
	Invalidate(FALSE);
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	CColorDialog ccd(SeedClr,CC_SOLIDCOLOR);
	if(IDOK==ccd.DoModal())//������ɫ�Ի���ѡȡ���ɫ
		SeedClr=ccd.GetColor();	
	else
		return;	
	if(IDOK==MessageBox("���ڿ��������ڵ���������!","��ʾ",MB_OKCANCEL))
	    bFill=TRUE;
	else
		return;	
}
