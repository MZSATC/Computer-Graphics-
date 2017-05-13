// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#define PI 3.1415926//Բ����
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
	ON_WM_RBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDM_PLAY, OnPlay)
	ON_UPDATE_COMMAND_UI(IDM_PLAY, OnUpdatePlay)
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
	R=800.0;d=1000;Phi=90;Theta=0;
	Alpha=0.0;Beta=0.0;
	V=NULL;F=NULL;
}

CTestView::~CTestView()
{
	if(V!=NULL)
	{
		delete[] V;
		V=NULL;
	}
	for(int n=0;n<N2+2;n++)//ע�⳷������,���к���,�������෴
	{
		delete[] F[n];
		F[n]=NULL;
	}
	delete[] F;
	F=NULL;
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
	r=80;//Բ������뾶
	h=300;//Բ���ĸ�
	int cTheta=10;//����н�
	int cNum=30;//������
	N1=360/cTheta;//N1����������
	N2=Round(h/cNum);//N2Ϊ����������
	V=new CP3[N1*(N2+1)+2];
	double cTheta1,cNum1;
	V[0].x=0;V[0].y=0;V[0].z=0;//��������
	for(int i=0;i<N2+1;i++)//����
	{	
		cNum1=i*cNum;
		for(int j=0;j<N1;j++)//����
		{
			cTheta1=j*cTheta*PI/180;
			V[i*N1+j+1].x=r*cos(cTheta1);
			V[i*N1+j+1].y=cNum1;	
			V[i*N1+j+1].z=r*sin(cTheta1);
		}
	}
	V[N1*(N2+1)+1].x=0;V[N1*(N2+1)+1].y=h;V[N1*(N2+1)+1].z=0;//��������	
}

void CTestView::ReadFace()//���
{
	//���ö�ά��̬����
	F=new CFace *[N2+2];//����
	for(int n=0;n<N2+2;n++)
		F[n]=new CFace[N1];//����
	for(int j=0;j<N1;j++)//����ײ���������Ƭ
	{
		int tempj=j+1;
		if(tempj==N1) tempj=0;//��Ƭ����β����
		int BottomIndex[3];//�ײ���������Ƭ����������
		BottomIndex[0]=0;
		BottomIndex[1]=j+1;
		BottomIndex[2]=tempj+1;
		F[0][j].SetNum(3);
		for(int k=0;k<F[0][j].vN;k++)//��Ƭ�ж��������
			F[0][j].vI[k]=BottomIndex[k];
	}
	for(int i=1;i<=N2;i++)//����Բ�����ı�����Ƭ
	{
		for(int j=0;j<N1;j++)
	    {
			int tempi=i+1;
			int tempj=j+1;
			if(N1==tempj) tempj=0;
			int BodyIndex[4];//Բ�����ı�����Ƭ����������
			BodyIndex[0]=(i-1)*N1+j+1;
			BodyIndex[1]=(tempi-1)*N1+j+1;
			BodyIndex[2]=(tempi-1)*N1+tempj+1;
			BodyIndex[3]=(i-1)*N1+tempj+1;
			F[i][j].SetNum(4);
			for(int k=0;k<F[i][j].vN;k++)
				F[i][j].vI[k]=BodyIndex[k];
		}
	}
	for(j=0;j<N1;j++)//���춥����������Ƭ
	{
		int tempj=j+1;
		if(tempj==N1) tempj=0;
		int TopIndex[3];//������������Ƭ����������
		TopIndex[0]=N1*(N2+1)+1;
		TopIndex[1]=N1*N2+tempj+1;
		TopIndex[2]=N1*N2+j+1;
		F[N2+1][j].SetNum(3);	
		for(int k=0;k<F[N2+1][j].vN;k++)
			F[N2+1][j].vI[k]=TopIndex[k];
	}
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
	ViewP.x=P.x*k[3]-P.z*k[1];//�۲�����ϵ��ά����
	ViewP.y=-P.x*k[8]+P.y*k[2]-P.z*k[7];
	ViewP.z=-P.x*k[6]-P.y*k[4]-P.z*k[5]+R;
    ScreenP.x=d*ViewP.x/ViewP.z;//��Ļ��ά����ϵ
	ScreenP.y=d*ViewP.y/ViewP.z;
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

void CTestView::DrawObject(CDC* pDC)//����Բ��������
{
	CLine *line=new CLine;
    CP2 Point3[3],t3;//��������涥������
	CP2 Point4[4],t4;//���涥������
	for(int i=0;i<N2+2;i++)//N2+2
	{
		for(int j=0;j<N1;j++)
		{
			CVector ViewVector(V[F[i][j].vI[0]],ViewPoint);//���������
			ViewVector=ViewVector.Normalize();//��λ��������
			F[i][j].SetFaceNormal(V[F[i][j].vI[0]],V[F[i][j].vI[1]],V[F[i][j].vI[2]]);	
			F[i][j].fNormal.Normalize();//��λ����ʸ��
			if(Dot(ViewVector,F[i][j].fNormal)>=0)//�����޳�
			{
				if(3==F[i][j].vN)//��������Ƭ
				{
					for(int m=0;m<F[i][j].vN;m++)
					{
						PerProject(V[F[i][j].vI[m]]);
						Point3[m]=ScreenP;						
					}			
					for(int n=0;n<3;n++)
					{
						if(0==n)
						{
							line->MoveTo(pDC,Point3[n]);
							t3=Point3[n];
						}
						else
							line->LineTo(pDC,Point3[n]);
					}
					line->LineTo(pDC,t3);//�պ϶����
				}
				else//�ı�����Ƭ
				{
					for(int m=0;m<F[i][j].vN;m++)
					{
						PerProject(V[F[i][j].vI[m]]);
						Point4[m]=ScreenP;						
					}
					
					for(int n=0;n<4;n++)
					{
						if(0==n)
						{
							line->MoveTo(pDC,Point4[n]);
							t4=Point4[n];
						}
						else
						{
							line->LineTo(pDC,Point4[n]);
						}		
					}
					line->LineTo(pDC,t4);//�պ϶����
				}
			} 	
		}	
	}
	delete line;
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point)//����������
{
	// TODO: Add your message handler code here and/or call default
	R=R+100;
	InitParameter();
	Invalidate(FALSE);
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnRButtonDblClk(UINT nFlags, CPoint point)//����Ҽ����� 
{
	// TODO: Add your message handler code here and/or call default
	R=R-100;
	InitParameter();
	Invalidate(FALSE);
	CView::OnRButtonDblClk(nFlags, point);
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

void CTestView::OnPlay() 
{
	// TODO: Add your command handler code here
	bPlay=bPlay?FALSE:TRUE;
	if(bPlay)//���ö�ʱ��
		SetTimer(1,150,NULL);	
	else
		KillTimer(1);
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

void CTestView::OnUpdatePlay(CCmdUI* pCmdUI) 
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
	tran.SetMat(V,N1*(N2+1)+2);
	tran.Translate(0,-h/2,0);
	InitParameter();
}




