// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#include "math.h"
#define Round(d) int(floor(d+0.5))//��������궨��
#define PI 3.1415926//Բ����
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
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDM_PLAY, OnPlay)
	ON_UPDATE_COMMAND_UI(IDM_PLAY, OnUpdatePlay)
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
	bPlay=FALSE;
	R=1000.0;d=900.0;Phi=45.0;Theta=45.0;
	Alpha=0.0;Beta=0.0;
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

void CTestView::ReadVertex()//���
{
	//�������ά����(x,y,z),������߳�Ϊ2a
	double a=150;	
	V[0].x=-a;V[0].y=-a;V[0].z=-a;V[0].c=CRGB(0.0,0.0,0.0);
	V[1].x=+a;V[1].y=-a;V[1].z=-a;V[1].c=CRGB(1.0,0.0,0.0);
	V[2].x=+a;V[2].y=+a;V[2].z=-a;V[2].c=CRGB(1.0,1.0,0.0);
	V[3].x=-a;V[3].y=+a;V[3].z=-a;V[3].c=CRGB(0.0,1.0,0.0);
	V[4].x=-a;V[4].y=-a;V[4].z=+a;V[4].c=CRGB(0.0,0.0,1.0);
	V[5].x=+a;V[5].y=-a;V[5].z=+a;V[5].c=CRGB(1.0,0.0,1.0);
	V[6].x=+a;V[6].y=+a;V[6].z=+a;V[6].c=CRGB(1.0,1.0,1.0);
	V[7].x=-a;V[7].y=+a;V[7].z=+a;V[7].c=CRGB(0.0,1.0,1.0);
}

void CTestView::ReadFace()//���
{
	//��Ķ���������Ķ�������
	F[0].SetNum(4);F[0].vI[0]=4;F[0].vI[1]=5;F[0].vI[2]=6;F[0].vI[3]=7;//ǰ��
	F[1].SetNum(4);F[1].vI[0]=0;F[1].vI[1]=3;F[1].vI[2]=2;F[1].vI[3]=1;//����
	F[2].SetNum(4);F[2].vI[0]=0;F[2].vI[1]=4;F[2].vI[2]=7;F[2].vI[3]=3;//����
	F[3].SetNum(4);F[3].vI[0]=1;F[3].vI[1]=2;F[3].vI[2]=6;F[3].vI[3]=5;//����
	F[4].SetNum(4);F[4].vI[0]=2;F[4].vI[1]=3;F[4].vI[2]=7;F[4].vI[3]=6;//����
	F[5].SetNum(4);F[5].vI[0]=0;F[5].vI[1]=1;F[5].vI[2]=5;F[5].vI[3]=4;//����
}

void CTestView::InitParameter()//͸�ӱ任������ʼ��
{	
	k[1]=sin(PI*Theta/180);
	k[2]=sin(PI*Phi/180);
	k[3]=cos(PI*Theta/180);
	k[4]=cos(PI*Phi/180);
	k[5]=k[2]*k[3];
	k[6]=k[2]*k[1];
	k[7]=k[4]*k[3];
	k[8]=k[4]*k[1];
	ViewPoint.x=R*k[6];//�û�����ϵ���ӵ�������
	ViewPoint.y=R*k[4];
	ViewPoint.z=R*k[5];
}

void CTestView::PerProject(CP3 P)//͸�ӱ任
{
	CP3 ViewP;
	ViewP.x=k[3]*P.x-k[1]*P.z;//�۲�����ϵ��ά����
	ViewP.y=-k[8]*P.x+k[2]*P.y-k[7]*P.z;
	ViewP.z=-k[6]*P.x-k[4]*P.y-k[5]*P.z+R;
	ViewP.c=P.c;
    ScreenP.x=d*ViewP.x/ViewP.z;//��Ļ����ϵ��ά����
	ScreenP.y=d*ViewP.y/ViewP.z;
	ScreenP.c=ViewP.c;
}

void CTestView::DoubleBuffer(CDC* pDC)//˫����
{
	CRect rect;//����ͻ�������
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

void CTestView::DrawObject(CDC* pDC)//�������������
{
	CPi2 Point[4];//͸��ͶӰ����Ķ�ά��������
	for(int nFace=0;nFace<6;nFace++)//��ѭ��
	{	
		CVector ViewVector(V[F[nFace].vI[0]],ViewPoint);//�����ʸ��
		ViewVector=ViewVector.Normalize();//��λ����ʸ��
		F[nFace].SetFaceNormal(V[F[nFace].vI[0]],V[F[nFace].vI[1]],V[F[nFace].vI[2]]);
		F[nFace].fNormal.Normalize();//��λ����ʸ��
		if(Dot(ViewVector,F[nFace].fNormal)>=0)//�����޳�
		{
			for(int nVertex=0;nVertex<F[nFace].vN;nVertex++)//����ѭ��
			{
				PerProject(V[F[nFace].vI[nVertex]]);//͸��ͶӰ
				Point[nVertex].x=ScreenP.x;
				Point[nVertex].y=Round(ScreenP.y);
				Point[nVertex].c=ScreenP.c;
			}
			CFill *fill=new CFill;//��̬�����ڴ� 
			fill->SetPoint(Point,4);//���ö���
			fill->CreateBucket();//����Ͱ��
			fill->CreateEdge();//�����߱�
			fill->Gouraud(pDC);//�����Ƭ
			delete fill;//�����ڴ�
		}
	}
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point)//����������
{
	// TODO: Add your message handler code here and/or call default
	R=R+100;
	InitParameter();
	Invalidate(FALSE);
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	R=R-100;
	InitParameter();
	Invalidate(FALSE);
	CView::OnRButtonDown(nFlags, point);
}

void CTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(!bPlay)
	{
		switch(nChar)
		{
		case VK_UP:
			Alpha=-5;
			tran.RotateX(Alpha);
			break;
		case VK_DOWN:
			Alpha=5;
			tran.RotateX(Alpha);
			break;
		case VK_LEFT:
			Beta=-5;
			tran.RotateY(Beta);
			break;
		case VK_RIGHT:
			Beta=5;
			tran.RotateY(Beta);
			break;
		default:
			break;			
		}
		Invalidate(FALSE);		
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTestView::OnTimer(UINT nIDEvent)//����ʱ�亯�� 
{
	// TODO: Add your message handler code here and/or call default
	Alpha=5;Beta=5;
	tran.RotateX(Alpha);
	tran.RotateY(Beta);
	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}

void CTestView::OnPlay() //�˵�����
{
	// TODO: Add your command handler code here
	bPlay=bPlay?FALSE:TRUE;
	if(bPlay)//���ö�ʱ��
		SetTimer(1,150,NULL);	
	else
		KillTimer(1);
}

void CTestView::OnUpdatePlay(CCmdUI* pCmdUI) //�˵����ƺ���
{
	// TODO: Add your command update UI handler code here
	if(bPlay)
	{
		pCmdUI->SetCheck(TRUE);
		pCmdUI->SetText("ֹͣ");
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->SetText("��ʼ");
	}
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadVertex();
	ReadFace();
	tran.SetMat(V,8);
	InitParameter();
}



