// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#include "math.h"//��ѧͷ�ļ�
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
void CTestView::ReadPoint()//������ƶ����16����������
{
	P3[0][0].x=20;  P3[0][0].y=0;  P3[0][0].z=200;//P00
	P3[0][1].x=0;   P3[0][1].y=100;P3[0][1].z=150;//P01
	P3[0][2].x=-130;P3[0][2].y=100;P3[0][2].z=50; //P02
	P3[0][3].x=-250;P3[0][3].y=50; P3[0][3].z=0;  //P03

	P3[1][0].x=100; P3[1][0].y=100;P3[1][0].z=150;//P10
	P3[1][1].x=30;  P3[1][1].y=100;P3[1][1].z=100;//p11
	P3[1][2].x=-40; P3[1][2].y=100;P3[1][2].z=50; //p12
	P3[1][3].x=-110;P3[1][3].y=100;P3[1][3].z=0;  //p13

	P3[2][0].x=280; P3[2][0].y=90; P3[2][0].z=140;//P20
	P3[2][1].x=110; P3[2][1].y=120;P3[2][1].z=80; //P21
	P3[2][2].x=30;  P3[2][2].y=130;P3[2][2].z=30; //P22
	P3[2][3].x=-100;P3[2][3].y=150;P3[2][3].z=-50;//P23

	P3[3][0].x=350; P3[3][0].y=30; P3[3][0].z=150;//P30
	P3[3][1].x=200; P3[3][1].y=150;P3[3][1].z=50; //P31
	P3[3][2].x=50;  P3[3][2].y=200;P3[3][2].z=0;  //P32
	P3[3][3].x=0;   P3[3][3].y=100;P3[3][3].z=-70;//P33	
}

void CTestView::DoubleBuffer(CDC* pDC)//˫����
{
	CRect rect;//���� �ͻ�������
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
	DrawCtrlPolygon(&memDC);//���ƿ��ƶ����
	SignCtrPoint(&memDC);//��ע���Ƶ���
	DrawObject(&memDC);//����Bezier����
	pDC->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&memDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//���ڴ�memDC�е�λͼ��������ʾpDC��
	memDC.SelectObject(pOldBitmap);//�ָ�λͼ
	NewBitmap.DeleteObject();//ɾ��λͼ
}

void CTestView::ObliqueProjection()//б�Ȳ�ͶӰ
{
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
		{
			P2[i][j].x=P3[i][j].x-P3[i][j].z/sqrt(2);
			P2[i][j].y=P3[i][j].y-P3[i][j].z/sqrt(2);
		}
}

void CTestView::DrawCtrlPolygon(CDC *pDC)//���ƿ��ƶ����
{
	ReadPoint();
	ObliqueProjection();
	CPen NewPen,*pOldPen;
	NewPen.CreatePen(PS_SOLID,3,RGB(0,0,0));
	pOldPen=pDC->SelectObject(&NewPen);
	for(int i=0;i<4;i++)
	{
		pDC->MoveTo(Round(P2[i][0].x),Round(P2[i][0].y));
		for(int j=1;j<4;j++)
			pDC->LineTo(Round(P2[i][j].x),Round(P2[i][j].y));
	}
	for(int j=0;j<4;j++)
	{
		pDC->MoveTo(Round(P2[0][j].x),Round(P2[0][j].y));
		for(int i=1;i<4;i++)
			pDC->LineTo(Round(P2[i][j].x),Round(P2[i][j].y));
	}
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
}

void CTestView::DrawObject(CDC *pDC)//����˫����Bezier����	
{
	double x,y,u,v,u1,u2,u3,u4,v1,v2,v3,v4;
	double M[4][4];
	M[0][0]=-1;M[0][1]=3; M[0][2]=-3;M[0][3]=1;
	M[1][0]=3; M[1][1]=-6;M[1][2]=3; M[1][3]=0;
	M[2][0]=-3;M[2][1]=3; M[2][2]=0; M[2][3]=0;
	M[3][0]=1; M[3][1]=0; M[3][2]=0; M[3][3]=0;
	LeftMultiMatrix(M,P3);//���־��������ά�����
	TransposeMatrix(M);//����ת�þ���
	RightMultiMatrix(P3,MT);//���־����ҳ���ά�����
	ObliqueProjection();//���ͶӰ
	for(u=0;u<=1;u+=0.04)
		for(v=0;v<=1;v+=0.04)
		{
			u1=u*u*u;u2=u*u;u3=u;u4=1;v1=v*v*v;v2=v*v;v3=v;v4=1;
			x=(u1*P2[0][0].x+u2*P2[1][0].x+u3*P2[2][0].x+u4*P2[3][0].x)*v1
			 +(u1*P2[0][1].x+u2*P2[1][1].x+u3*P2[2][1].x+u4*P2[3][1].x)*v2
			 +(u1*P2[0][2].x+u2*P2[1][2].x+u3*P2[2][2].x+u4*P2[3][2].x)*v3
			 +(u1*P2[0][3].x+u2*P2[1][3].x+u3*P2[2][3].x+u4*P2[3][3].x)*v4;
			y=(u1*P2[0][0].y+u2*P2[1][0].y+u3*P2[2][0].y+u4*P2[3][0].y)*v1
			 +(u1*P2[0][1].y+u2*P2[1][1].y+u3*P2[2][1].y+u4*P2[3][1].y)*v2
			 +(u1*P2[0][2].y+u2*P2[1][2].y+u3*P2[2][2].y+u4*P2[3][2].y)*v3
			 +(u1*P2[0][3].y+u2*P2[1][3].y+u3*P2[2][3].y+u4*P2[3][3].y)*v4;
			if(v==0)
				pDC->MoveTo(Round(x),Round(y));
			else
				pDC->LineTo(Round(x),Round(y));
		}		  
	for(v=0;v<=1;v+=0.04)
		for(u=0;u<=1;u+=0.04)
		{
			u1=u*u*u;u2=u*u;u3=u;u4=1;v1=v*v*v;v2=v*v;v3=v;v4=1;
			x=(u1*P2[0][0].x+u2*P2[1][0].x+u3*P2[2][0].x+u4*P2[3][0].x)*v1
			 +(u1*P2[0][1].x+u2*P2[1][1].x+u3*P2[2][1].x+u4*P2[3][1].x)*v2
			 +(u1*P2[0][2].x+u2*P2[1][2].x+u3*P2[2][2].x+u4*P2[3][2].x)*v3
			 +(u1*P2[0][3].x+u2*P2[1][3].x+u3*P2[2][3].x+u4*P2[3][3].x)*v4;
			y=(u1*P2[0][0].y+u2*P2[1][0].y+u3*P2[2][0].y+u4*P2[3][0].y)*v1
			 +(u1*P2[0][1].y+u2*P2[1][1].y+u3*P2[2][1].y+u4*P2[3][1].y)*v2
			 +(u1*P2[0][2].y+u2*P2[1][2].y+u3*P2[2][2].y+u4*P2[3][2].y)*v3
			 +(u1*P2[0][3].y+u2*P2[1][3].y+u3*P2[2][3].y+u4*P2[3][3].y)*v4;
			if(0==u)
				pDC->MoveTo(Round(x),Round(y));
			else
				pDC->LineTo(Round(x),Round(y));
		}	
}

void CTestView::LeftMultiMatrix(double M0[][4],CP3 P0[][4])//��˾���M*P
{
	CP3 T[4][4];//��ʱ����
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		{	
			T[i][j].x=M0[i][0]*P0[0][j].x+M0[i][1]*P0[1][j].x+M0[i][2]*P0[2][j].x+M0[i][3]*P0[3][j].x;	
			T[i][j].y=M0[i][0]*P0[0][j].y+M0[i][1]*P0[1][j].y+M0[i][2]*P0[2][j].y+M0[i][3]*P0[3][j].y;
			T[i][j].z=M0[i][0]*P0[0][j].z+M0[i][1]*P0[1][j].z+M0[i][2]*P0[2][j].z+M0[i][3]*P0[3][j].z;
		}
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			P3[i][j]=T[i][j];
}

void CTestView::RightMultiMatrix(CP3 P0[][4],double M1[][4])//�ҳ˾���P*M
{
	CP3 T[4][4];//��ʱ����
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		{	
			T[i][j].x=P0[i][0].x*M1[0][j]+P0[i][1].x*M1[1][j]+P0[i][2].x*M1[2][j]+P0[i][3].x*M1[3][j];
			T[i][j].y=P0[i][0].y*M1[0][j]+P0[i][1].y*M1[1][j]+P0[i][2].y*M1[2][j]+P0[i][3].y*M1[3][j];
			T[i][j].z=P0[i][0].z*M1[0][j]+P0[i][1].z*M1[1][j]+P0[i][2].z*M1[2][j]+P0[i][3].z*M1[3][j];
		}
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			P3[i][j]=T[i][j];
}

void CTestView::SignCtrPoint(CDC *pDC)//��ע���Ƶ�ı��
{
	CString str;
    pDC->SetTextColor(RGB(0,0,255));
   	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			str.Format("P%d,%d",i,j);
			pDC->TextOut(Round(P2[i][j].x+10),Round(P2[i][j].y),str);				
		}		
	}
}

void CTestView::TransposeMatrix(double M0[][4])//ת�þ���
{
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			MT[j][i]=M0[i][j];
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadPoint();
}


