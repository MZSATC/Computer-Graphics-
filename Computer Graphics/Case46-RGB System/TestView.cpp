// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"

#include "TestDoc.h"
#include "TestView.h"
#include "math.h"//��ѧͷ�ļ�
#define Round(d) int(floor(d+0.5))//��������궨��
#define PI 3.1415926
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
	ON_UPDATE_COMMAND_UI(IDM_DRAWPIC, OnUpdateDrawpic)
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
	bPlay=FALSE;
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
	CRect rect;
	GetClientRect(&rect);	
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(),rect.Height());
	pDC->SetViewportExt(rect.Width(),-rect.Height());
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);
	rect.OffsetRect(-rect.Width()/2,-rect.Height()/2);
	BoundaryClr=RGB(1,1,1);
	if(bPlay)
		CMYModel(pDC);
	else
		RGBModel(pDC);
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

void CTestView::RGBModel(CDC *pDC)
{
	AfxGetMainWnd()->SetWindowText("����46:RGB��ɫϵͳ");
	int r=130;//Բ�İ뾶
	int a=200;//�ȱ������εı߳�
	int h=Round(a*sin(60*PI/180));//�ȱ������εĸ�
	CPoint vr(0,2*h/3);//��ԲԲ��
	CPoint vg(-a/2,-h/3);//��ԲԲ��
	CPoint vb(a/2,-h/3);//��ԲԲ��

	CPen NewPen,*OldPen;//�ڱ�
	NewPen.CreatePen(PS_SOLID,1,BoundaryClr);
	OldPen=pDC->SelectObject(&NewPen);
	CBrush NewBrush,*pOldBrush;
	pOldBrush=(CBrush*)pDC->SelectStockObject(NULL_BRUSH);//ѡ��͸����ˢ
	pDC->Ellipse(vr.x-r,vr.y-r,vr.x+r,vr.y+r);
	pDC->Ellipse(vg.x-r,vg.y-r,vg.x+r,vg.y+r);
	pDC->Ellipse(vb.x-r,vb.y-r,vb.x+r,vb.y+r);
	pDC->SelectObject(OldPen);
	NewPen.DeleteObject();	
	pDC->SelectObject(pOldBrush);
	
	CBrush NewBrushR,*OldBrushR;//��ɫ
	NewBrushR.CreateSolidBrush(RGB(255,0,0));
	OldBrushR=pDC->SelectObject(&NewBrushR);
	pDC->FloodFill(vr.x,vr.y,BoundaryClr);
	pDC->SelectObject(OldBrushR);
	NewBrushR.DeleteObject();

	CBrush NewBrushG,*OldBrushG;//��ɫ
	NewBrushG.CreateSolidBrush(RGB(0,255,0));
	OldBrushG=pDC->SelectObject(&NewBrushG);
	pDC->FloodFill(vg.x,vg.y,BoundaryClr);
	pDC->SelectObject(OldBrushG);
	NewBrushG.DeleteObject();

	CBrush NewBrushB,*OldBrushB;//��ɫ
	NewBrushB.CreateSolidBrush(RGB(0,0,255));
	OldBrushB=pDC->SelectObject(&NewBrushB);
	pDC->FloodFill(vb.x,vb.y,BoundaryClr);
	pDC->SelectObject(OldBrushB);
	NewBrushB.DeleteObject();
	
	CBrush NewBrushRGB,*OldBrushRGB;//��ɫ����ɫ����ɫ
	NewBrushRGB.CreateSolidBrush(RGB(255,0,0)|RGB(0,255,0)|RGB(0,0,255));
	OldBrushRGB=pDC->SelectObject(&NewBrushRGB);
	pDC->FloodFill(0,0,BoundaryClr);
	pDC->SelectObject(OldBrushRGB);
	NewBrushRGB.DeleteObject();

	CBrush NewBrushRB,*OldBrushRB;//��ɫ����ɫ
	NewBrushRB.CreateSolidBrush(RGB(255,0,0)|RGB(0,255,0));
	OldBrushRB=pDC->SelectObject(&NewBrushRB);
	pDC->FloodFill(-a/5,0,BoundaryClr);
	pDC->SelectObject(OldBrushRB);
	NewBrushRB.DeleteObject();

	CBrush NewBrushRG,*OldBrushRG;//��ɫ����ɫ
	NewBrushRG.CreateSolidBrush(RGB(255,0,0)|RGB(0,0,255));
	OldBrushRG=pDC->SelectObject(&NewBrushRG);
	pDC->FloodFill(a/5,0,BoundaryClr);
	pDC->SelectObject(OldBrushRG);
	NewBrushRG.DeleteObject();

	CBrush NewBrushBG,*OldBrushBG;//��ɫ����ɫ
	NewBrushBG.CreateSolidBrush(RGB(0,255,0)|RGB(0,0,255));
	OldBrushBG=pDC->SelectObject(&NewBrushBG);
	pDC->FloodFill(0,-a/5,BoundaryClr);
	pDC->SelectObject(OldBrushBG);
	NewBrushBG.DeleteObject();
}

void CTestView::CMYModel(CDC *pDC)
{
	AfxGetMainWnd()->SetWindowText("����46:CMY��ɫϵͳ");
	int r=130;//Բ�İ뾶
	int a=200;//�ȱ������εı߳�
	int h=Round(a*sin(60*PI/180));//�ȱ������εĸ�
	CPoint vm(0,2*h/3);//Ʒ��ԲԲ��
	CPoint vy(-a/2,-h/3);//��ɫԲԲ��
	CPoint vc(a/2,-h/3);//��ɫԲԲ��

	CPen NewPen,*OldPen;//�ڱ�
	NewPen.CreatePen(PS_SOLID,1,BoundaryClr);
	OldPen=pDC->SelectObject(&NewPen);
	CBrush NewBrush,*pOldBrush;
	pOldBrush=(CBrush*)pDC->SelectStockObject(NULL_BRUSH);//ѡ��͸����ˢ
	pDC->Ellipse(vm.x-r,vm.y-r,vm.x+r,vm.y+r);
	pDC->Ellipse(vy.x-r,vy.y-r,vy.x+r,vy.y+r);
	pDC->Ellipse(vc.x-r,vc.y-r,vc.x+r,vc.y+r);
	pDC->SelectObject(OldPen);
	NewPen.DeleteObject();	
	pDC->SelectObject(pOldBrush);

	CBrush NewBrushM,*OldBrushM;//Ʒ��
	NewBrushM.CreateSolidBrush(RGB(255,0,255));
	OldBrushM=pDC->SelectObject(&NewBrushM);
	pDC->FloodFill(vm.x,vm.y,BoundaryClr);
	pDC->SelectObject(OldBrushM);
	NewBrushM.DeleteObject();

	CBrush NewBrushY,*OldBrushY;//��ɫ
	NewBrushY.CreateSolidBrush(RGB(255,255,0));
	OldBrushY=pDC->SelectObject(&NewBrushY);
	pDC->FloodFill(vy.x,vy.y,BoundaryClr);
	pDC->SelectObject(OldBrushY);
	NewBrushY.DeleteObject();

	CBrush NewBrushC,*OldBrushC;//��ɫ
	NewBrushC.CreateSolidBrush(RGB(0,255,255));
	OldBrushC=pDC->SelectObject(&NewBrushC);
	pDC->FloodFill(vc.x,vc.y,BoundaryClr);
	pDC->SelectObject(OldBrushC);
	NewBrushC.DeleteObject();
	
	CBrush NewBrushCMY,*OldBrushCMY;//��ɫ
	NewBrushCMY.CreateSolidBrush(RGB(0,255,255)&RGB(255,0,255)&RGB(255,255,0));
	OldBrushCMY=pDC->SelectObject(&NewBrushCMY);
	pDC->FloodFill(0,0,BoundaryClr);
	pDC->SelectObject(OldBrushCMY);
	NewBrushCMY.DeleteObject();

	CBrush NewBrushMY,*OldBrushMY;//��ɫ
	NewBrushMY.CreateSolidBrush(RGB(255,0,255)&RGB(255,255,0));
	OldBrushMY=pDC->SelectObject(&NewBrushMY);
	pDC->FloodFill(-a/5,0,BoundaryClr);
	pDC->SelectObject(OldBrushMY);
	NewBrushMY.DeleteObject();

	CBrush NewBrushMC,*OldBrushMC;//��ɫ
	NewBrushMC.CreateSolidBrush(RGB(255,0,255)&RGB(0,255,255));
	OldBrushMC=pDC->SelectObject(&NewBrushMC);
	pDC->FloodFill(a/5,0,BoundaryClr);
	pDC->SelectObject(OldBrushMC);
	NewBrushMC.DeleteObject();

	CBrush NewBrushYC,*OldBrushYC;//��ɫ
	NewBrushYC.CreateSolidBrush(RGB(255,255,0)&RGB(0,255,255));
	OldBrushYC=pDC->SelectObject(&NewBrushYC);
	pDC->FloodFill(0,-a/5,BoundaryClr);
	pDC->SelectObject(OldBrushYC);
	NewBrushYC.DeleteObject();
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	bPlay=bPlay?FALSE:TRUE;
	Invalidate(FALSE);
}

void CTestView::OnUpdateDrawpic(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(bPlay)
	{
		pCmdUI->SetCheck(TRUE);
		pCmdUI->SetText("CMY");
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->SetText("RGB");
	}
}
