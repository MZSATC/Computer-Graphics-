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
	ON_COMMAND(ID_TLEFT, OnLeft)
	ON_COMMAND(ID_TRIGHT, OnRight)
	ON_COMMAND(ID_TUP, OnUp)
	ON_COMMAND(ID_TDOWN, OnDown)
	ON_COMMAND(ID_SINCREASE, OnIncrease)
	ON_COMMAND(ID_SDECREASE, OnDecrease)
	ON_COMMAND(ID_RCLOCKWISE, OnCounterClockwise)
	ON_COMMAND(ID_RCOUNTERCLOCKWISE, OnClockwise)
	ON_COMMAND(ID_RXAXIS, OnXaxis)
	ON_COMMAND(ID_RYAXIS, OnYaxis)
	ON_COMMAND(ID_RORG, OnOrg)
	ON_COMMAND(ID_SYPLUS, OnYPlus)
	ON_COMMAND(ID_SYNEG, OnYNeg)
	ON_COMMAND(ID_SXPLUS, OnXPlus)
	ON_COMMAND(ID_SXNEG, OnXNeg)
	ON_COMMAND(ID_RESET, OnReset)
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

void CTestView::DoubleBuffer(CDC *pDC)//˫����
{
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
	DrawObject(&memDC);//��memDC����ͼ��
	pDC->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&memDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//���ڴ�memDC�е�λͼ��������ʾpDC��
	memDC.SelectObject(pOldBitmap);//�ָ�λͼ
	NewBitmap.DeleteObject();//ɾ��λͼ
}

void CTestView::ReadPoint()//���
{
	P[0].x=-100;P[0].y=-50;
	P[1].x=100; P[1].y=-50;
	P[2].x=100; P[2].y=50;
	P[3].x=-100;P[3].y=50;
}

void CTestView::DrawObject(CDC *pDC)//����ͼ��
{
	CLine *line=new CLine;//��������ϵ
	line->MoveTo(pDC,0,-rect.Height()/2);//����x��
	line->LineTo(pDC,0,rect.Height()/2);
	line->MoveTo(pDC,-rect.Width()/2,0);//����y��
	line->LineTo(pDC,rect.Width()/2,0);
	DrawPolygon(pDC);
	delete line;
}

void CTestView::DrawPolygon(CDC* pDC)//���ƶ�����߿�ģ��
{
	CLine *line=new CLine;
	CP2 t;
	for(int i=0;i<4;i++)//���ƶ����
	{
		if(0==i)
		{
			line->MoveTo(pDC,P[i]);
			t=P[i];
		}
		else
			line->LineTo(pDC,P[i]);
	}
	line->LineTo(pDC,t);//�պ϶����
	delete line;
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	MessageBox("��ʹ��ͼ�갴ť���ж�ά���α任","��ʾ",MB_OK);
}

void CTestView::OnLeft()//����ƽ�� 
{
	// TODO: Add your command handler code here
	trans.Translate(-10,0);
	Invalidate(FALSE);
}

void CTestView::OnRight()//����ƽ�� 
{
	// TODO: Add your command handler code here
	trans.Translate(10,0);
	Invalidate(FALSE);
}

void CTestView::OnUp()//����ƽ��  
{
	// TODO: Add your command handler code here
	trans.Translate(0,10);
	Invalidate(FALSE);
}

void CTestView::OnDown()//����ƽ�� 
{
	// TODO: Add your command handler code here
    trans.Translate(0,-10);
	Invalidate(FALSE);
}

void CTestView::OnIncrease()//�ȱȷŴ�  
{
	// TODO: Add your command handler code here
    trans.Scale(2,2);
	Invalidate(FALSE);
}

void CTestView::OnDecrease()//�ȱ���С 
{
	// TODO: Add your command handler code here
    trans.Scale(0.5,0.5);
	Invalidate(FALSE);	
}

void CTestView::OnClockwise()//˳ʱ�븴����ת  
{
	// TODO: Add your command handler code here
	CP2 p=(P[0]+P[2])/2;
	trans.Rotate(-30,p);
	Invalidate(FALSE);
}

void CTestView::OnCounterClockwise()//��ʱ�븴����ת 
{
	// TODO: Add your command handler code here
	CP2 p=(P[0]+P[2])/2;
    trans.Rotate(30,p);
	Invalidate(FALSE);		
}

void CTestView::OnXaxis()//����x�ᷴ�� 
{
	// TODO: Add your command handler code here
    trans.ReflectX();
	Invalidate(FALSE);	
}

void CTestView::OnYaxis()//����y�ᷴ�� 
{
	// TODO: Add your command handler code here
    trans.ReflectY();
	Invalidate(FALSE);	
}

void CTestView::OnOrg()//����ԭ�㷴�� 
{
	// TODO: Add your command handler code here
    trans.ReflectOrg();
	Invalidate(FALSE);	
}

void CTestView::OnXPlus()//��x���������  
{
	// TODO: Add your command handler code here
	trans.Shear(0,1);
	Invalidate(FALSE);	
}

void CTestView::OnXNeg()//��x�Ḻ�����
{
	// TODO: Add your command handler code here
	trans.Shear(0,-1);
	Invalidate(FALSE);	
}

void CTestView::OnYPlus()//��y��������� 
{
	// TODO: Add your command handler code here
	trans.Shear(1,0);
	Invalidate(FALSE);	
}

void CTestView::OnYNeg()//��y�Ḻ����� 
{
	// TODO: Add your command handler code here
	trans.Shear(-1,0);
	Invalidate(FALSE);	
}

void CTestView::OnReset()//��λ 
{
	// TODO: Add your command handler code here
	ReadPoint();
	Invalidate(FALSE);	
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadPoint();
	trans.SetMat(P,4);
}
