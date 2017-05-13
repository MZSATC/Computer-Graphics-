// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "MainFrm.h"
#include "VView.h"//
#include "HView.h"//
#include "WView.h"//
#include "TestDoc.h"
#include "TestView.h"
#include "Transform.h"
#include "Vector.h"
#define PI 3.1415926//Բ����
#include "math.h"//��ѧͷ�ļ�
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
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
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
	int a=80,b=140;//a�����α߳���b�ⳤ
	P[0].x=-a/2;P[0].y=0;          P[0].z=-b/2;//P0
	P[1].x=0;   P[1].y=sqrt(3)/2*a;P[1].z=-b/2;//P1
	P[2].x=a/2; P[2].y=0;          P[2].z=-b/2;//P2
	P[3].x=-a/2;P[3].y=0;          P[3].z=b/2; //P3
	P[4].x=0;   P[4].y=sqrt(3)/2*a;P[4].z=b/2; //P4
	P[5].x=a/2; P[5].y=0;          P[5].z=b/2; //P5	
}

void CTestView::ReadFace()//���
{   
	//��Ķ���������Ķ�������
	F[0].SetNum(4);F[0].vI[0]=0;F[0].vI[1]=3;F[0].vI[2]=4;F[0].vI[3]=1;
	F[1].SetNum(3);F[1].vI[0]=0;F[1].vI[1]=1;F[1].vI[2]=2;
	F[2].SetNum(4);F[2].vI[0]=0;F[2].vI[1]=2;F[2].vI[2]=5;F[2].vI[3]=3;
	F[3].SetNum(4);F[3].vI[0]=1;F[3].vI[1]=4;F[3].vI[2]=5;F[3].vI[3]=2;
	F[4].SetNum(3);F[4].vI[0]=3;F[4].vI[1]=5;F[4].vI[2]=4;
}

void CTestView::Rotate()//��ת�任
{
	CTransform trans;
	trans.SetMat(P,6);
	trans.RotateX(1);
	trans.RotateY(1);
}

void CTestView::DoubleBuffer(CDC* pDC)//˫����
{
	CRect rect;//����ͻ���
	GetClientRect(&rect);//��ÿͻ����Ĵ�С
	CDC memDC[4];
	CDC* pViewDC[4];
	CMainFrame *pFrame=(CMainFrame*)AfxGetMainWnd();//��ÿ��ָ��
	CVView *pVView=(CVView*)pFrame->m_wndSplitter.GetPane(0,0);//�������ͼ����ָ��
	CWView *pWView=(CWView*)pFrame->m_wndSplitter.GetPane(0,1);//��ò���ͼ����ָ��
	CHView *pHView=(CHView*)pFrame->m_wndSplitter.GetPane(1,0);//��ø���ͼ����ָ��
	pViewDC[0]=pDC;//��������ĻDC
	pViewDC[1]=pVView->GetDC();//����ͼ��ĻDC
	pViewDC[2]=pWView->GetDC();//������ĻDC
	pViewDC[3]=pHView->GetDC();//����ͼ��ĻDC
	CBitmap NewBitmap[4],*pOldBitmap[4];//�ڴ��г���ͼ�����ʱλͼ
    for(int i=0;i<4;i++)//�����ڴ�DC
	{	
		pViewDC[i]->SetMapMode(MM_ANISOTROPIC);//pDC�Զ�������ϵ
		pViewDC[i]->SetWindowExt(rect.Width(),rect.Height());//���ô��ڷ�Χ
		pViewDC[i]->SetViewportExt(rect.Width(),-rect.Height());//x��ˮƽ���ң�y��Ǧֱ����
		pViewDC[i]->SetViewportOrg(rect.Width()/2,rect.Height()/2);//��Ļ����Ϊԭ��
		NewBitmap[i].CreateCompatibleBitmap(pViewDC[i],rect.Width(),rect.Height());//��������λͼ
		memDC[i].CreateCompatibleDC(pViewDC[i]);
		pOldBitmap[i]=memDC[i].SelectObject(&NewBitmap[i]);
		memDC[i].FillSolidRect(&rect,pViewDC[i]->GetBkColor());//��ԭ���������ͻ����������Ǻ�ɫ 
		memDC[i].SetMapMode(MM_ANISOTROPIC);//MemDC�Զ�������ϵ
		memDC[i].SetWindowExt(rect.Width(),rect.Height());
		memDC[i].SetViewportExt(rect.Width(),-rect.Height());
		memDC[i].SetViewportOrg(rect.Width()/2,rect.Height()/2);
	}
	rect.OffsetRect(-rect.Width()/2,-rect.Height()/2);
	DrawOblique(&memDC[0]);//����б�Ȳ�ͼ
	DrawVView(&memDC[1]);  //��������ͼ
	DrawWView(&memDC[2]);  //���Ʋ���ͼ
	DrawHView(&memDC[3]);  //���Ƹ���ͼ
	for(i=0;i<4;i++)
	{
		pViewDC[i]->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&memDC[i],-rect.Width()/2,-rect.Height()/2,SRCCOPY);//���ڴ�memDC�е�λͼ��������ʾpDC��
		memDC[i].SelectObject(pOldBitmap[i]);//�ָ�λͼ
		NewBitmap[i].DeleteObject();//ɾ��λͼ
		ReleaseDC(pViewDC[i]);//�ͷ���ͼDC
	}
}

void CTestView::DrawObject(CDC *pDC,CP3 P[])//����б�Ȳ�ͼ�߿�ģ��
{
	for(int nFace=0;nFace<5;nFace++)
	{
		CP2 ScreenP,t;	
		CLine *line=new CLine;
		for(int nVertex=0;nVertex<F[nFace].vN;nVertex++)//����ѭ��
		{
			ScreenP=P[F[nFace].vI[nVertex]];//б�Ȳ�ͶӰ
			if(0==nVertex)
			{
				line->MoveTo(pDC,ScreenP.x,ScreenP.y);
				t=ScreenP;
			}
			else
				line->LineTo(pDC,ScreenP.x,ScreenP.y);
		} 
		line->LineTo(pDC,t.x,t.y);//�պ϶����
		delete line;		
	}
}

void CTestView::DrawTriView(CDC *pDC,CP3 P[])//��������ͼ�߿�ģ��
{
	for(int nFace=0;nFace<5;nFace++)
	{
		CP3 ScreenP,t;	
		CLine *line=new CLine;
		for(int nVertex=0;nVertex<F[nFace].vN;nVertex++)//����ѭ��
		{
			ScreenP=P[F[nFace].vI[nVertex]];
			if(0==nVertex)
			{
				line->MoveTo(pDC,-ScreenP.z,ScreenP.y);
				t=ScreenP;
			}
			else
			{
				line->LineTo(pDC,-ScreenP.z,ScreenP.y);
			}
		}
		line->LineTo(pDC,-t.z,t.y);//�պ϶����
		delete line;
	}
}

void CTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CTransform trans;
	trans.SetMat(P,6);	
	switch(nChar)
	{
	case VK_UP:
		trans.RotateX(1);//�趨����
		break;
	case VK_DOWN:
		trans.RotateX(-1);
		break;
	case VK_LEFT:
		trans.RotateY(-1);
		break;
	case VK_RIGHT:
		trans.RotateY(1);
		break;
	default:
		break;			
	}
	Invalidate(FALSE);		
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTestView::TOMatrix()//б�Ȳ�任����
{
	TO[0][0]=1;         TO[0][1]=0;         TO[0][2]=0;TO[0][3]=0;
	TO[1][0]=0;         TO[1][1]=1;         TO[1][2]=0;TO[1][3]=0;
	TO[2][0]=-1/sqrt(2);TO[2][1]=-1/sqrt(2);TO[2][2]=0;TO[2][3]=0;
	TO[3][0]=0;         TO[3][1]=0;         TO[3][2]=0;TO[3][3]=1;
}

void CTestView::TVMatrix()//����ͼ�任����
{
	TV[0][0]=0;TV[0][1]=0;TV[0][2]=0;TV[0][3]=0;
	TV[1][0]=0;TV[1][1]=1;TV[1][2]=0;TV[1][3]=0;
	TV[2][0]=0;TV[2][1]=0;TV[2][2]=1;TV[2][3]=0;
	TV[3][0]=0;TV[3][1]=0;TV[3][2]=0;TV[3][3]=1;
}

void CTestView::THMatrix()//����ͼ�任����
{
	TH[0][0]=0;TH[0][1]=-1;TH[0][2]=0;TH[0][3]=0;
	TH[1][0]=0;TH[1][1]=0; TH[1][2]=0;TH[1][3]=0;
	TH[2][0]=0;TH[2][1]=0; TH[2][2]=1;TH[2][3]=0;
	TH[3][0]=0;TH[3][1]=0; TH[3][2]=0;TH[3][3]=1;
}

void CTestView::TWMatrix()//����ͼ�任����
{
	TW[0][0]=0;TW[0][1]=0;TW[0][2]=-1;TW[0][3]=0;
	TW[1][0]=0;TW[1][1]=1;TW[1][2]=0; TW[1][3]=0;
	TW[2][0]=0;TW[2][1]=0;TW[2][2]=0; TW[2][3]=0;
	TW[3][0]=0;TW[3][1]=0;TW[3][2]=0; TW[3][3]=1;
}

void CTestView::DrawOblique(CDC *pDC)
{
	pDC->TextOut(-230,130,"����ͼ");
	MultiMatrix(TO);
	DrawObject(pDC,PNew);
}

void CTestView::DrawVView(CDC *pDC)
{
	pDC->TextOut(-230,130,"����ͼ");
	MultiMatrix(TV);
	DrawTriView(pDC,PNew);
}

void CTestView::DrawHView(CDC *pDC)
{
	pDC->TextOut(-230,130,"����ͼ");
	MultiMatrix(TH);
	DrawTriView(pDC,PNew);
}

void CTestView::DrawWView(CDC *pDC)
{
	pDC->TextOut(-230,130,"����ͼ");
	MultiMatrix(TW);
	DrawTriView(pDC,PNew);
}

void CTestView::MultiMatrix(double T[][4])//�����������
{
	for(int i=0;i<6;i++)
	{
		PNew[i].x=P[i].x*T[0][0]+P[i].y*T[1][0]+P[i].z*T[2][0]+P[i].w*T[3][0];
		PNew[i].y=P[i].x*T[0][1]+P[i].y*T[1][1]+P[i].z*T[2][1]+P[i].w*T[3][1];
		PNew[i].z=P[i].x*T[0][2]+P[i].y*T[1][2]+P[i].z*T[2][2]+P[i].w*T[3][2];
		PNew[i].w=P[i].x*T[0][3]+P[i].y*T[1][3]+P[i].z*T[2][3]+P[i].w*T[3][3];
	}		
}

void CTestView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(((CMainFrame*)AfxGetMainWnd())->bPlay)
	{
		Rotate();
		Invalidate(FALSE);;	
	}
	CView::OnTimer(nIDEvent);
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CTestView::OnInitialUpdate()//��ʼ�� 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	TOMatrix();
	TVMatrix();
	THMatrix();
	TWMatrix();
	ReadPoint();
	ReadFace();
	SetTimer(1,50,NULL);//���ö�ʱ��
}
