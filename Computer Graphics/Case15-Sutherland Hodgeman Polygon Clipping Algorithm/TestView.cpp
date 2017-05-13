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
#define  InMax   7  //������붥����
#define  OutMax  12 //������������
#define  LEFT    1
#define  RIGHT   2
#define  TOP     3
#define  BOTTOM  4
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
	bDrawRect=FALSE;
	bClip=FALSE;
	OutCount=0;
	RtCount=0;
}

CTestView::~CTestView()
{
	if(In!=NULL)
	{
		delete []In;
		In=NULL;
	}
	if(Out!=NULL)
	{
		delete []Out;
		Out=NULL;
	}
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

void CTestView::ReadPoint()//���
{
	In=new CP2[InMax];//���붥���
	In[0].x=50;  In[0].y=100;
	In[1].x=-150;In[1].y=300;
	In[2].x=-250;In[2].y=50;
	In[3].x=-150;In[3].y=-250;
	In[4].x=0;   In[4].y=-50;
	In[5].x=100; In[5].y=-250;
	In[6].x=300; In[6].y=150;
	Out=new CP2[OutMax];//��������
	for(int i=0;i<OutMax;i++)
		Out[i]=CP2(0,0);
}

void CTestView::DoubleBuffer(CDC *pDC)//˫����
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
	if(RtCount && !bClip)
		DrawWindowRect(&memDC);//���ƴ���
	DrawObject(&memDC,bClip);//���ƶ����
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
	pDC->MoveTo(Round(Rect[0].x),Round(Rect[0].y));
	pDC->LineTo(Round(Rect[1].x),Round(Rect[0].y));
	pDC->LineTo(Round(Rect[1].x),Round(Rect[1].y));
	pDC->LineTo(Round(Rect[0].x),Round(Rect[1].y));
	pDC->LineTo(Round(Rect[0].x),Round(Rect[0].y));
	pDC->SelectObject(pOldPen3);
	NewPen3.DeleteObject();
}

void CTestView::ClipBoundary(CP2 rect0,CP2 rect1)//���ڱ߽縳ֵ����
{
	if(rect0.x>rect1.x)
	{
		Wxl=rect1.x;
		Wxr=rect0.x;
	}
	else
	{
		Wxl=rect0.x;
		Wxr=rect1.x;
	}
	if(rect0.y>rect1.y)
	{
		Wyt=rect0.y;
		Wyb=rect1.y;
	}
	else
	{
		Wyt=rect1.y;
		Wyb=rect0.y;
	}
}

void CTestView::DrawObject(CDC *pDC,BOOL bclip)//���ƶ����
{
	if(!bclip)
	{
		for(int i=0;i<InMax;i++)//���Ʋü�ǰ�Ķ����
		{
			if(0==i)
				pDC->MoveTo(Round(In[i].x),Round(In[i].y));
		    else
		    	pDC->LineTo(Round(In[i].x),Round(In[i].y));
		}
    	pDC->LineTo(Round(In[0].x),Round(In[0].y));
	}
	else
	{
		ClipPolygon(In,InMax,LEFT);
		ClipPolygon(Out,OutCount,RIGHT);
		ClipPolygon(Out,OutCount,BOTTOM);
		ClipPolygon(Out,OutCount,TOP);
		for(int j=0;j<OutCount;j++)//���Ʋü���Ķ����
		{
			if(0==j)
				pDC->MoveTo(Round(Out[j].x),Round(Out[j].y));
		    else
		    	pDC->LineTo(Round(Out[j].x),Round(Out[j].y));
		}
		if(0!=OutCount)
			pDC->LineTo(Round(Out[0].x),Round(Out[0].y));
	}
}

void CTestView::ClipPolygon(CP2 *out,int Length,UINT Boundary)//�ü������
{
	if(0==Length)
		return;
	CP2 *pTemp=new CP2[Length];
	for(int i=0;i<Length;i++)
		pTemp[i]=out[i];
	CP2 p0,p1,p;//p0-��㣬p1-�յ㣬p-����
	OutCount=0;
	p0=pTemp[Length-1];
	for(i=0;i<Length;i++)
	{
		p1=pTemp[i];
		if(Inside(p0,Boundary))//����ڴ�����
		{
			if(Inside(p1,Boundary))//�յ��ڴ�����,�����ڡ���
			{
				Out[OutCount]=p1;//�յ��ڴ�����
				OutCount++;
			}
			else//�����ڡ���
			{
				p=Intersect(p0,p1,Boundary);//�󽻵�
				Out[OutCount]=p;
				OutCount++;
			}
		}
		else if(Inside(p1,Boundary))//�յ��ڴ�����,���������
		{
			p=Intersect(p0,p1,Boundary);//�󽻵�
			Out[OutCount]=p;
			OutCount++;
			Out[OutCount]=p1;
			OutCount++;
		}
		p0=p1;
	}
	delete[] pTemp;
}

BOOL CTestView::Inside(CP2 p,UINT Boundary)//�жϵ��ڴ��ڵ�����
{
	switch(Boundary)
	{
	case LEFT:
		if(p.x>=Wxl)
			return TRUE;
		break;
	case RIGHT:
		if(p.x<=Wxr)
			return TRUE;
		break;
	case TOP:
		if(p.y<=Wyt)
			return TRUE;
		break;
	case BOTTOM:
		if(p.y>=Wyb)
			return TRUE;
		break;
	}
	return FALSE;
}

CP2 CTestView::Intersect(CP2 p0,CP2 p1,UINT Boundary)//�󽻵�
{
	CP2 pTemp;
	double k=(p1.y-p0.y)/(p1.x-p0.x);//ֱ�߶ε�б��
	switch(Boundary)
	{
	case LEFT:
		pTemp.x=Wxl;
		pTemp.y=k*(pTemp.x-p0.x)+p0.y;
		break;
	case RIGHT:		
		pTemp.x=Wxr;
		pTemp.y=k*(pTemp.x-p0.x)+p0.y;
		break;
	case TOP:
		pTemp.y=Wyt;
		pTemp.x=(pTemp.y-p0.y)/k+p0.x;
		break;
	case BOTTOM:
		pTemp.y=Wyb;
		pTemp.x=(pTemp.y-p0.y)/k+p0.x;
		break;
	}
	return pTemp;
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(bDrawRect)
	{	
		if(RtCount<2)
		{
			Rect[RtCount]=Convert(point);
    		RtCount++;
		}	
	}
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!bClip)
	{
		if(RtCount<2)
		{   
			Rect[RtCount]=Convert(point);
			Invalidate(FALSE);
		}
	}

	CView::OnMouseMove(nFlags, point);
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

void CTestView::OnDrawpic()//ͼ�β˵� 
{
	// TODO: Add your command handler code here
	RtCount=0;
	bDrawRect=TRUE;
	bClip=FALSE;
	Rect[0]=Rect[1]=CP2(0,0);
	Invalidate(FALSE);
	MessageBox("�����ƴ��ڣ��������вü�","��ʾ",MB_OKCANCEL);
}

void CTestView::OnClip()//�ü��˵�  
{
	// TODO: Add your command handler code here
	RtCount=0;
	bDrawRect=FALSE;
	bClip=TRUE;
	ClipBoundary(Rect[0],Rect[1]);
	Invalidate(FALSE);
}

void CTestView::OnUpdateClip(CCmdUI* pCmdUI)//����ͼ�갴ť״̬���� 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((RtCount>=2)?TRUE:FALSE);	
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadPoint();
}
