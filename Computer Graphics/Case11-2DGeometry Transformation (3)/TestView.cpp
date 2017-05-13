// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#include "math.h"
#define Round(d) int(floor(d+0.5))//��������궨��
#define PI 3.141592
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define LEFT   1   //����:0001
#define RIGHT  2   //����:0010
#define BOTTOM 4   //����:0100
#define TOP    8   //����:1000
/////////////////////////////////////////////////////////////////////////////
// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
	//{{AFX_MSG_MAP(CTestView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDM_DRAWPIC, OnDrawpic)
	ON_COMMAND(IDM_CLIP, OnClip)
	ON_UPDATE_COMMAND_UI(IDM_CLIP, OnUpdateClip)
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
	PtCount=0;
	bDrawRect=FALSE;
	bClip=FALSE;
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
	DoubleBuffer(pDC);
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

void CTestView::DoubleBuffer(CDC* pDC)//˫����
{
	CRect rect;//����ͻ���
	GetClientRect(&rect);//��ÿͻ����Ĵ�С
	pDC->SetMapMode(MM_ANISOTROPIC);//pDC�Զ�������ϵ
	pDC->SetWindowExt(rect.Width(),rect.Height());//���ô��ڷ�Χ
	pDC->SetViewportExt(rect.Width(),-rect.Height());//����������Χ,x��ˮƽ���ң�y�ᴹֱ����
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);//�ͻ�������Ϊԭ��
	CDC memDC;//�ڴ�DC
	memDC.CreateCompatibleDC(pDC);//����һ������ʾpDC���ݵ��ڴ�memDC
	CBitmap NewBitmap,*pOldBitmap;//�ڴ��г��ص���ʱλͼ 
	NewBitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());//��������λͼ 
	pOldBitmap=memDC.SelectObject(&NewBitmap);//������λͼѡ��memDC 
	memDC.FillSolidRect(rect,pDC->GetBkColor());//��ԭ���������ͻ����������Ǻ�ɫ
	memDC.SetMapMode(MM_ANISOTROPIC);//memDC�Զ�������ϵ
	memDC.SetWindowExt(rect.Width(),rect.Height());
	memDC.SetViewportExt(rect.Width(),-rect.Height());
	memDC.SetViewportOrg(rect.Width()/2,rect.Height()/2);
	rect.OffsetRect(-rect.Width()/2,-rect.Height()/2);
	if(PtCount && !bClip)
		DrawWindowRect(&memDC);//���ƴ���
	Diamond(&memDC);
	pDC->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&memDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//���ڴ�memDC�е�λͼ��������ʾpDC��
	memDC.SelectObject(pOldBitmap);//�ָ�λͼ
	NewBitmap.DeleteObject();//ɾ��λͼ
}


void CTestView::DrawWindowRect(CDC* pDC)//���Ʋü�����
{
	// TODO: Add your message handler code here and/or call default
	CPen NewPen3,*pOldPen3;//����3�����ؿ�ȵĻ���
	NewPen3.CreatePen(PS_SOLID,3,RGB(0,128,0));
	pOldPen3=pDC->SelectObject(&NewPen3);
    pDC->Rectangle(Round(Rect[0].x),Round(Rect[0].y),Round(Rect[1].x),Round(Rect[1].y));
	pDC->SelectObject(pOldPen3);
	NewPen3.DeleteObject();
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(bDrawRect)
	{	if(PtCount<2)
		{
			Rect[PtCount]=Convert(point);
			PtCount++;
		}		
	}
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!bClip)
	{
		if(PtCount<2)
		{   
			Rect[PtCount]=Convert(point);
			Invalidate(FALSE);
		}
	}
	CView::OnMouseMove(nFlags, point);
}

BOOL CTestView::Cohen()//�е�ָ��㷨
{
	EnCode(P[0]);//������
	EnCode(P[1]);//�յ����
	while(P[0].rc!=0 || P[1].rc!=0)//��������һ�������ڴ���֮������
	{
		if((P[0].rc & P[1].rc)!=0)//����֮
		{
			PtCount=0;
			return FALSE;
		}
		if(0==P[0].rc)//ȷ��P[0]λ�ڴ���֮��
		{
			CP2 Temp;
			Temp=P[0];
			P[0]=P[1];
			P[1]=Temp;
		}
		MidClip(P[0],P[1]);
	}
	return TRUE;
}

void CTestView::MidClip(CP2 p0,CP2 p1)//�е�ָ��㷨
{
	CP2 p;//�е�����	
	p=(p0+p1)/2;
	EnCode(p);
	while(fabs(p.x-p0.x)>1e-6||fabs(p.y-p0.y)>1e-6)//�жϽ���
	{
		if(0==p.rc)//�е�Ҳ�ڴ����ڣ�������P1��
			p1=p;
		else//��������P0��
			p0=p;
		p=(p0+p1)/2;
		EnCode(p);
	}
	P[0]=p;
}

void CTestView::EnCode(CP2 &pt)//�˵���뺯��
{
	pt.rc=0;
	if(pt.x<Rect[0].x)
	{
		pt.rc=pt.rc|LEFT;
	}
	else if(pt.x>Rect[1].x)
	{
		pt.rc=pt.rc|RIGHT;
	}
	if(pt.y<Rect[1].y)
	{
		pt.rc=pt.rc|BOTTOM;
	}
	else if(pt.y>Rect[0].y)
	{
		pt.rc=pt.rc|TOP;
	}
}

CP2 CTestView::Convert(CPoint point)//����ϵ�任 
{
	CRect rect;
	GetClientRect(&rect);
	CP2 ptemp;
	ptemp.x=point.x-rect.Width()/2;
	ptemp.y=rect.Height()/2-point.y;
	return ptemp;
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//��ֹ�������� 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CTestView::Diamond(CDC *pDC)
{
	double thta;//thtaΪԲ�ĵȷֽ�
	int n=30;//����ȷֵ����
	V=new CP2[n];
	double r=300;//����Բ�İ뾶
	thta=2*PI/n;
	for(int i=0;i<n;i++)//����ȷֵ�����
	{
		V[i].x=r*cos(i*thta);
		V[i].y=r*sin(i*thta);
	}
	for(i=0;i<=n-2;i++)//���θ����ӵȷֵ�
	{
		for(int j=i+1;j<=n-1;j++)
		{
			if(!bClip)
			{
				pDC->MoveTo(Round(V[i].x),Round(V[i].y));
				pDC->LineTo(Round(V[j].x),Round(V[j].y));
			}
			else
			{		
				P[0]=V[i];P[1]=V[j];//�Խ��ʯ��ÿ��ֱ�߽��вü�
				if(Cohen())					
				{
					pDC->MoveTo(Round(P[0].x),Round(P[0].y));
					pDC->LineTo(Round(P[1].x),Round(P[1].y));
					
				}
			}	
		}
	}
	delete []V;
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	PtCount=0;
	bClip=FALSE;
	bDrawRect=TRUE;
	Invalidate(FALSE);	
	MessageBox("�����ƴ��ڣ��������вü�","��ʾ",MB_OKCANCEL);	
}

void CTestView::OnClip() 
{
	// TODO: Add your command handler code here
	bClip=TRUE;
	Invalidate(FALSE);
}

void CTestView::OnUpdateClip(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((PtCount>=2)?TRUE:FALSE);		
}
