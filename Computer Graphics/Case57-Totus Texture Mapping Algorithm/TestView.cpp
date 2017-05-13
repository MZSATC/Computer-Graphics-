// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#define  PI 3.1415926
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
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
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
	R=1000.0,d=800.0,Phi=90.0,Theta=0.0;//�ӵ�λ����ǰ��
	Near=200.0,Far=1200.0;//����������Զ������	
	Alpha=0.0,Beta=0.0;//��ת��
	LightNum=1;//��Դ����
	pLight=new CLighting(LightNum);//һά��Դ��̬����
    pLight->Light[0].SetPosition(0,0,800);//���õ�һ����Դλ������
	for(int i=0;i<LightNum;i++)
	{
		pLight->Light[i].L_Diffuse=CRGB(0.7,0.7,0.7); //��Դ����������ɫ	
		pLight->Light[i].L_Specular=CRGB(0.3,0.3,0.3);//��Դ����߹���ɫ
		pLight->Light[i].L_C0=1.0;//����˥��ϵ��
		pLight->Light[i].L_C1=0.0001;//����˥��ϵ��
		pLight->Light[i].L_C2=0.00000001;//����˥��ϵ��
		pLight->Light[i].L_OnOff=TRUE;//��Դ����	
	}
 	pMaterial=new CMaterial;//һά���ʶ�̬����
	pMaterial->SetAmbient(CRGB(0.247,0.200,0.075));//���ʶԻ������ķ�����
	pMaterial->SetDiffuse(CRGB(0.4,0.4,0.4));//���ʶ��������ķ�����
	pMaterial->SetSpecular(CRGB(1.0,1.0,1.0));//���ʶԾ��淴���ķ�����
	pMaterial->M_Emit=CRGB(0.0,0.0,0.0);//��������ɢ����ɫ
	pMaterial->M_n=20.0;//�߹�ָ��
}

CTestView::~CTestView()
{
	if(pLight!=NULL)
	{
		delete pLight;
		pLight=NULL;
	}
	if(pMaterial!=NULL)
	{
		delete pMaterial;
		pLight=NULL;
	}
	if(V!=NULL)
	{
		delete []V;
		V=NULL;
	}
	if(T!=NULL)
	{
		delete []T;
		T=NULL;
	}
	if(N!=NULL)
	{
		delete []N;
		N=NULL;
	}
	for(int n=0;n<N1;n++)
	{
		delete[] F[n];
		F[n]=NULL;
	}
	delete []F;
	F=NULL;
	for(n=0;n<bmp.bmHeight;n++)
	{
		delete []Image[n];
		Image[n]=NULL;
	}
	delete []Image;
	Image=NULL;
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
	int tAlpha=10,tBeta=10;//�ȷֽǶ�
	int r1=200,r2=80;//Բ���뾶�ͻ�����뾶
	N1=360/tAlpha,N2=360/tBeta;//��Ƭ����ΪN1��N2
	V=new CP3[N1*N2];//���㶯̬����
	T=new CT2[N1*N2];//����̬����
	N=new CVector[N1*N2];//��ʸ����̬����
	for(int i=0;i<N1;i++)
	{
		double tAlpha1=tAlpha*i*PI/180;
		for(int j=0;j<N2;j++)//���㸳ֵ
		{
			double tBeta1=tBeta*j*PI/180;
			V[i*N2+j].x=(r1+r2*sin(tBeta1))*sin(tAlpha1);
			V[i*N2+j].y=r2*cos(tBeta1);
			V[i*N2+j].z=(r1+r2*sin(tBeta1))*cos(tAlpha1);
			//���㶥�������
			T[i*N2+j].u=(2*PI-tAlpha1)/(2*PI)*(bmp.bmWidth-1);
			T[i*N2+j].v=tBeta1/(2*PI)*(bmp.bmHeight-1);
		}
	}
}

void CTestView::CalNormal()//���㷨ʸ��
{
	for(int i=0;i<N1;i++)//����
	{
		for(int j=0;j<N2;j++)//����
		{
			//���㶥���ƽ����ʸ��
			int Beforei=i-1,Afteri=i+1;
			int Beforej=j-1,Afterj=j+1;
			if(0==i) Beforei=N1-1;
			if(0==j) Beforej=N2-1;
			if(N1==Afteri) Afteri=0;
			if(N2==Afterj) Afterj=0;
			CVector vN0,vN1,vN2,vN3,AveN;//�����ĸ���Ƭ�ķ�ʸ����ƽ����ʸ��	
			CVector vEdge01(V[Beforei*N2+Afterj],V[i*N2+Afterj]);
			CVector vEdge02(V[Beforei*N2+Afterj],V[i*N2+j]);
			vN0=Cross(vEdge01,vEdge02);
			CVector vEdge11(V[i*N2+Afterj],V[Afteri*N2+Afterj]);
			CVector vEdge12(V[i*N2+Afterj],V[Afteri*N2+j]);
			vN1=Cross(vEdge11,vEdge12);
			CVector vEdge21(V[i*N2+j],V[Afteri*N2+j]);
			CVector vEdge22(V[i*N2+j],V[Afteri*N2+Beforej]);
			vN2=Cross(vEdge21,vEdge22);
			CVector vEdge31(V[Beforei*N2+j],V[i*N2+j]);
			CVector vEdge32(V[Beforei*N2+j],V[i*N2+Beforej]);
			vN3=Cross(vEdge31,vEdge32);
			AveN=(vN0+vN1+vN2+vN3)/AveN.Mag();//���㷨ʸ����ƽ��ֵ
			N[i*N2+j]=AveN;
		}
	}	
}

void CTestView::ReadFace()//���
{
	F=new CFace *[N1];
	for(int n=0;n<N1;n++)
	{
		F[n]=new CFace[N2];
	}
	for(int i=0;i<N1;i++)
	{
		for(int j=0;j<N2;j++)
		{
			int tempi=i+1;
			int tempj=j+1;
			if(N2==tempj) tempj=0;
			if(N1==tempi) tempi=0;
			F[i][j].SetNum(4);//��ı���
			F[i][j].vI[0]=i*N2+j;//������ıߺ�
			F[i][j].vI[1]=i*N2+tempj;
			F[i][j].vI[2]=tempi*N2+tempj; 
			F[i][j].vI[3]=tempi*N2+j;
		}
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
	ViewPoint.x=R*k[6];
	ViewPoint.y=R*k[4];
	ViewPoint.z=R*k[5];
}

void CTestView::PerProject(CP3 P)//͸�ӱ任
{
	CP3 ViewP;
	ViewP.x=P.x*k[3]-P.z*k[1];//�۲�����ϵ��ά����
	ViewP.y=-P.x*k[8]+P.y*k[2]-P.z*k[7];
	ViewP.z=-P.x*k[6]-P.y*k[4]-P.z*k[5]+R;
	ViewP.c=P.c;
    ScreenP.x=d*ViewP.x/ViewP.z;//��Ļ����ϵ��ά����
	ScreenP.y=Round(d*ViewP.y/ViewP.z);
	ScreenP.z=Far*(1-Near/ViewP.z)/(Far-Near);
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
	//memDC.FillSolidRect(rect,pDC->GetBkColor());//��ԭ���������ͻ����������Ǻ�ɫ
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

void CTestView::DrawObject(CDC *pDC)//����Բ����
{
	CalNormal();
	CZBuffer *zbuf=new CZBuffer;//�����ڴ�
	zbuf->InitDeepBuffer(800,800,1000);//��ʼ����Ȼ�����
	CPi3 Point4[4];//�����ı��ζ�������
	CT2 Texture4[4];//�����ı�����������
	CVector Normal4[4];//�����ı��η�ʸ������
	for(int i=0;i<N1;i++)
	{
		for(int j=0;j<N2;j++)
		{
			CVector ViewVector(V[F[i][j].vI[0]],ViewPoint);//�����ʸ��
			ViewVector=ViewVector.Normalize();//��λ����ʸ��
			F[i][j].SetFaceNormal(V[F[i][j].vI[0]],V[F[i][j].vI[1]],V[F[i][j].vI[2]]);//����С��Ƭ��ʸ��
			F[i][j].fNormal.Normalize();//��λ����ʸ��
			if(Dot(ViewVector,F[i][j].fNormal)>=0)
			{
				for(int m=0;m<F[i][j].vN;m++)
				{
					PerProject(V[F[i][j].vI[m]]);
					Point4[m]=ScreenP;
					Normal4[m]=N[F[i][j].vI[m]];
					Texture4[m]=T[F[i][j].vI[m]];
				}
				if((N2-1)==j)//������������Ľӷ�
				{
					Texture4[1].v=bmp.bmHeight-1;
					Texture4[2].v=bmp.bmHeight-1;
				}
				if ((N1-1)==i)
				{
					Texture4[2].u=1.0;
					Texture4[3].u=1.0;
				}
				zbuf->SetPoint(Point4,Normal4,Texture4,4);//��ʼ��
				zbuf->CreateBucket();//����Ͱ��
				zbuf->CreateEdge();//�����߱�
				zbuf->Phong(pDC,ViewPoint,pLight,pMaterial,Image);//����ı���
				zbuf->ClearMemory();
			}
		}
	}
	delete zbuf;
}

void CTestView::ReadImage()//��������
{
    CBitmap NewBitmap;
	NewBitmap.LoadBitmap(IDB_TEXTURE);//����DDBλͼ
	NewBitmap.GetBitmap(&bmp);//��CBitmap����Ϣ���浽Bitmap�ṹ����
	int nbytesize=bmp.bmWidthBytes*bmp.bmHeight;
	im=new BYTE[nbytesize];
	NewBitmap.GetBitmapBits(nbytesize,(LPVOID)im);
	Image=new COLORREF*[bmp.bmHeight];
	for(int n1=0;n1<bmp.bmHeight;n1++)
		Image[n1]=new COLORREF[bmp.bmWidth];		
	for(n1=0;n1<bmp.bmHeight;n1++)
	{
		for(int n2=0;n2<bmp.bmWidth;n2++)
		{
			int pos=n1*bmp.bmWidthBytes+4*n2;//��ɫ����λ��
			n1=bmp.bmHeight-1-n1;//λͼ�����½������Ͻǻ���
			Image[n1][n2]=RGB(im[pos+2],im[pos+1],im[pos]);
		}
	}
	delete []im;
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
	Alpha=5;
	tran.RotateX(Alpha);
	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//��ֹ����ˢ�� 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
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

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadImage();
	ReadVertex();
	ReadFace();
	tran.SetMat(V,N1*N2);
	tran.RotateX(90);
	InitParameter();
}





