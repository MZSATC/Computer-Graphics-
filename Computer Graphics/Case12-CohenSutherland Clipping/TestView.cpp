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
	Wxl=-300;Wyt=100;Wxr=300;Wyb=-100;
	PtCount=0;
	bDrawLine=FALSE;
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
	DrawWindowRect(&memDC);//���ƴ���
	if(PtCount>=1)
	{
		memDC.MoveTo(Round(P[0].x),Round(P[0].y));
		memDC.LineTo(Round(P[1].x),Round(P[1].y));			
	}
	pDC->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&memDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//���ڴ�memDC�е�λͼ��������ʾpDC��
	memDC.SelectObject(pOldBitmap);//�ָ�λͼ
	NewBitmap.DeleteObject();//ɾ��λͼ
}

void CTestView::DrawWindowRect(CDC* pDC)//���Ʋü�����
{
	// TODO: Add your message handler code here and/or call default
	pDC->SetTextColor(RGB(128,0,0));
	pDC->TextOut(-10,Wyt+20,"����");
	CPen NewPen3,*pOldPen3;//����3�����ؿ�ȵĻ���
	NewPen3.CreatePen(PS_SOLID,3,RGB(0,128,0));
	pOldPen3=pDC->SelectObject(&NewPen3);
    pDC->Rectangle(Wxl,Wyt,Wxr,Wyb);
	pDC->SelectObject(pOldPen3);
	NewPen3.DeleteObject();
}

void CTestView::Cohen()//Cohen-Sutherland�㷨
{
	CP2 p;//��������
	EnCode(P[0]);//������
	EnCode(P[1]);//�յ����
	while(P[0].rc!=0 || P[1].rc!=0)//��������һ�������ڴ���֮������
	{
		if((P[0].rc & P[1].rc)!=0)//����֮
		{
			PtCount=0;
			return;
		}
		if(0==P[0].rc)//ȷ��P[0]λ�ڴ���֮��
		{
			CP2 Temp;
			Temp=P[0];
			P[0]=P[1];
			P[1]=Temp;
		}
		UINT RC=P[0].rc;
		double k=(P[1].y-P[0].y)/(P[1].x-P[0].x);//ֱ�߶ε�б��
		//���ڱ߽簴���ҡ��¡��ϵ�˳��ü�ֱ�߶�
		if(RC & LEFT)//P[0]��λ�ڴ��ڵ����
		{
			p.x=Wxl;//���㽻��y����
			p.y=k*(p.x-P[0].x)+P[0].y;
		}
		else if(RC & RIGHT)//P[0]��λ�ڴ��ڵ��Ҳ�
		{
			p.x=Wxr;//���㽻��y����
			p.y=k*(p.x-P[0].x)+P[0].y;
		}
		else if(RC & BOTTOM)//P[0]��λ�ڴ��ڵ��²�
		{
			p.y=Wyb;//���㽻��x����
			p.x=(p.y-P[0].y)/k+P[0].x;
		}			
		else if(RC & TOP)//P[0]��λ�ڴ��ڵ��ϲ�
		{
			p.y=Wyt;//���㽻��x����
			p.x=(p.y-P[0].y)/k+P[0].x;
		}
		EnCode(p);
		P[0]=p;
	}
}

void CTestView::EnCode(CP2 &pt)//�˵���뺯��
{
	pt.rc=0;
	if(pt.x<Wxl)
		pt.rc=pt.rc|LEFT;
	else if(pt.x>Wxr)
		pt.rc=pt.rc|RIGHT;
	if(pt.y<Wyb)
		pt.rc=pt.rc|BOTTOM;
	else if(pt.y>Wyt)
		pt.rc=pt.rc|TOP;
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(bDrawLine)
	{	if(PtCount<2)
		{
			P[PtCount]=Convert(point);
			PtCount++;
		}
	}
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(bDrawLine)
	{
		if(PtCount<2)
		{   
			P[PtCount]=Convert(point);
			Invalidate(FALSE);
		}
	}
	CView::OnMouseMove(nFlags, point);
}

CP2 CTestView::Convert(CPoint point)//�豸����ϵ���Զ�������ϵת��
{
	CRect rect;
	GetClientRect(&rect);
	CP2 ptemp;
	ptemp.x=point.x-rect.Width()/2;
	ptemp.y=rect.Height()/2-point.y;
	return ptemp;
}

void CTestView::OnDrawpic()//ͼ�β˵�  
{
	// TODO: Add your command handler code here
	PtCount=0;
	bDrawLine=TRUE;
	MessageBox("��껭�ߣ������ü�","��ʾ",MB_OKCANCEL);
	Invalidate(FALSE);
}

void CTestView::OnClip()//�ü��˵� 
{
	// TODO: Add your command handler code here
	Cohen();
	bDrawLine=FALSE;
	Invalidate(FALSE);
}

void CTestView::OnUpdateClip(CCmdUI* pCmdUI)//����ͼ�갴ť״̬���� 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((PtCount>=2)?TRUE:FALSE);
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//��ֹ�������� 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}


