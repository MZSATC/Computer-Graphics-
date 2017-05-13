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
/////////////////////////////////////////////////////////////////////////////
// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
	//{{AFX_MSG_MAP(CTestView)
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDM_ZOOMOUT, OnZoomout)
	ON_COMMAND(IDM_ZOOMIN, OnZoomin)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
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
	nRHWidth=100;
	nRHHeight=100;
	nScale=2;
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
	Diamond(&memDC,FALSE);//���ƽ��ʯ����ͼ
	DrawRect(&memDC,nScale);//���ƷŴ�
	Diamond(&memDC,TRUE);//���Ʋü������еĽ��ʯ
	pDC->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&memDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//���ڴ�memDC�е�λͼ��������ʾpDC��
	memDC.SelectObject(pOldBitmap);//�ָ�λͼ
	NewBitmap.DeleteObject();//ɾ��λͼ
}

void CTestView::DrawRect(CDC *pDC,int nScale)//���Ʋü�����
{
	pDC->Rectangle(Round(nRCenter.x-nRHWidth),Round(nRCenter.y+nRHHeight),
		Round(nRCenter.x+nRHWidth),Round(nRCenter.y-nRHHeight));
}

void CTestView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	nRCenter=Convert(point);//���ָ��Ϊ�Ŵ�����
	Invalidate(FALSE);
	CView::OnMouseMove(nFlags, point);
}

BOOL CTestView::LBLineClip()//Liang-Barsky�ü�����
{
	double tmax,tmin,dx,dy;
	dx=P[1].x-P[0].x;dy=P[1].y-P[0].y;tmax=0.0,tmin=1.0;
	double wxl=nRCenter.x-nRHWidth/nScale;//���ڵ���߽�
	double wxr=nRCenter.x+nRHWidth/nScale;//���ڵ��ұ߽�
	double wyb=nRCenter.y-nRHHeight/nScale;//���ڵ��±߽�
	double wyt=nRCenter.y+nRHHeight/nScale;//���ڵ��ϱ߽�
	//���ڱ߽�����ҡ��¡���˳��ü�ֱ��
	if(ClipTest(-dx,P[0].x-wxl,tmax,tmin))//n��1,��߽�u1������x��v1��x0��wxl
	{
		if(ClipTest(dx,wxr-P[0].x,tmax,tmin))//n��2���ұ߽�u2����x��v2��wxr��x0
		{			
			if(ClipTest(-dy,P[0].y-wyb,tmax,tmin))//n��3���±߽�u3������y��v3��y0-wyb
			{
				if(ClipTest(dy,wyt-P[0].y,tmax,tmin))//n��4���ϱ߽�u4����y��v4��wyt-y0
				{
					if(tmin<1.0)//�ж�ֱ�ߵ��յ�
					{
						P[1].x=P[0].x+tmin*dx;//���¼���ֱ���յ�����
						P[1].y=P[0].y+tmin*dy;//x��x0��t(x1��x0)��ʽ
					}
					if(tmax>0.0)//�ж�ֱ�ߵ����
					{
						P[0].x=P[0].x+tmax*dx;//���¼���ֱ���������
						P[0].y=P[0].y+tmax*dy;//x��x0��t(x1��x0)��ʽ
					}
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CTestView::ClipTest(double u,double v,double &tmax,double &tmin)//�ü����Ժ���
{
	double t;
	BOOL ReturnValue=TRUE;
	if(u<0.0)//�Ӳü����ڵ��ⲿ���ڲ�,������㴦��tmax
	{
		t=v/u;
		if(t>tmin)
			ReturnValue=FALSE;
		else if(t>tmax)
			tmax=t;
	}
	else
	{
		if(u>0.0)//�Ӳü����ڵ��ڲ����ⲿ�������յ㴦��tmin
		{
			t=v/u;
			if(t<tmax)
				ReturnValue=FALSE;
			else if(t<tmin)
				tmin=t;
		}
		else//ƽ���ڴ��ڱ߽��ֱ��
		{
			if(v<0.0)//ֱ���ڴ������ֱ��ɾ��
				ReturnValue=FALSE;
		}
	}
	return(ReturnValue);
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

void CTestView::Diamond(CDC *pDC,BOOL bclip)
{
	CPen NewPen,*pOldPen;//���廭��
	if(bclip)
		NewPen.CreatePen(PS_SOLID,nScale,RGB(0,0,0));//����nScale��Ȼ���
	else
		NewPen.CreatePen(PS_SOLID,1,RGB(0,0,0));//����1���ؿ�Ȼ���
	pOldPen=pDC->SelectObject(&NewPen);//����ɫ����ѡ���豸������
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
			if(!bclip)
			{
				pDC->MoveTo(Round(V[i].x),Round(V[i].y));
				pDC->LineTo(Round(V[j].x),Round(V[j].y));
			}
			else
			{		
				P[0]=V[i];P[1]=V[j];
				if(LBLineClip())					
				{
					pDC->MoveTo(ZoomX(Round(P[0].x)),ZoomY(Round(P[0].y)));
					pDC->LineTo(ZoomX(Round(P[1].x)),ZoomY(Round(P[1].y)));					
				}
			}	
		}
	}
	delete []V; 
	pDC->SelectObject(pOldPen);//�ָ��豸������ԭ����
	NewPen.DeleteObject();//ɾ���ѳ�����״̬���»��� 
}

int CTestView::ZoomX(int x)
{
	return Round((x-nRCenter.x)*nScale+nRCenter.x);
}

int CTestView::ZoomY(int y)
{
	return Round((y-nRCenter.y)*nScale+nRCenter.y);
}

void CTestView::OnZoomin()//�Ŵ�ͼ�β˵�
{
	// TODO: Add your command handler code here
	if(nScale<=5)
		nScale=nScale+1;
	else
		MessageBox("�Ŵ���̫��");
	Invalidate(FALSE);
}

void CTestView::OnZoomout()//��Сͼ�β˵� 
{
	// TODO: Add your command handler code here
	if(nScale>=2)
		nScale=nScale-1;
	else
		MessageBox("�Ŵ���̫С");
	Invalidate(FALSE);
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(nScale<=5)
		nScale=nScale+1;
	else
		MessageBox("�Ŵ���̫��");	
	Invalidate(FALSE);
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(nScale>=2)
		nScale=nScale-1;
	else
		MessageBox("�Ŵ���̫С");
	Invalidate(FALSE);
	CView::OnRButtonDown(nFlags, point);
}
