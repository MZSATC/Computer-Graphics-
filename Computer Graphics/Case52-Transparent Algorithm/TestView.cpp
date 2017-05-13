// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"

#include "TestDoc.h"
#include "TestView.h"
#include "math.h"
#define Round(f) int(floor(f+0.5))//��������궨��
#define  PI 3.1415926
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
	Alpha=0.0;Beta=0.0;
	LightNum=1;//��Դ����
	pLight=new CLighting(LightNum);//һά��Դ��̬����
    pLight->Light[0].SetPosition(800,800,800);//���õ�һ����Դλ������
	for(int i=0;i<LightNum;i++)
	{
		pLight->Light[i].L_Diffuse=CRGB(1.0,1.0,1.0); //��Դ����������ɫ	
		pLight->Light[i].L_Specular=CRGB(1.0,1.0,1.0);//��Դ����߹���ɫ
		pLight->Light[i].L_C0=1.0;//����˥��ϵ��
		pLight->Light[i].L_C1=0.0000001;//����˥��ϵ��
		pLight->Light[i].L_C2=0.00000001;//����˥��ϵ��
		pLight->Light[i].L_OnOff=TRUE;//��Դ����	
	}
	//��Ĳ���
 	pMaterial=new CMaterial[2];//һά���ʶ�̬����
	pMaterial[0].SetAmbient(CRGB(0.175,0.012,0.012));//���ʶԻ������ķ�����
	pMaterial[0].SetDiffuse(CRGB(0.614,0.041,0.041));//���ʶ��������ķ�����
	pMaterial[0].SetSpecular(CRGB(1.0,1.0,1.0));//���ʶԾ��淴���ķ�����
	pMaterial[0].SetEmit(CRGB(0.2,0.0,0.0));//��������ɢ����ɫ
	pMaterial[0].M_n=30.0;//�߹�ָ��
	//������Ĳ���
	pMaterial[1].SetAmbient(CRGB(0.0,0.8,0.0));//���ʶԻ������ķ�����
	pMaterial[1].SetDiffuse(CRGB(0.0,0.8,0.0));//���ʶԻ�������������ķ��������
	pMaterial[1].SetSpecular(CRGB(1.0,1.0,1.0));//���ʶԾ��淴���ķ�����
	pMaterial[1].SetEmit(CRGB(0.0,0.2,0.0));//��������ɢ����ɫ
	pMaterial[1].M_n=30.0;//�߹�ָ��
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
		delete []pMaterial;
		pMaterial=NULL;
	}
	if(SV!=NULL)
	{
		delete[] SV;
		SV=NULL;
	}
	for(int n=0;n<N1;n++)//ע�⳷������,���к���,�������෴
	{
		delete[] SF[n];
		SF[n]=NULL;
	}
	delete[] SF;
	SF=NULL;
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
void CTestView::ReadSphereVertex()//�������涥������
{
	int gAlpha=4,gBeta=4;//��Ƭ�н�
	N1=180/gAlpha,N2=360/gBeta;//N1Ϊγ������,N2Ϊ��������
	SV=new CP3[(N1-1)*N2+2];//SVΪ��Ķ���
	//γ�ȷ�����ϱ���������"N1��1"���㣬"2"�����ϱ���������
	double gAlpha1,gBeta1,r=100;//rΪ����뾶
	//���㱱��������
	SV[0].x=0,SV[0].y=r,SV[0].z=0;
	//����ѭ�����������ϵĵ�����
	for(int i=0;i<N1-1;i++)
	{
		gAlpha1=(i+1)*gAlpha*PI/180;
		for(int j=0;j<N2;j++)
	    {
			gBeta1=j*gBeta*PI/180;		
			SV[i*N2+j+1].x=r*sin(gAlpha1)*sin(gBeta1);
			SV[i*N2+j+1].y=r*cos(gAlpha1);	
			SV[i*N2+j+1].z=r*sin(gAlpha1)*cos(gBeta1);
		}
	}
	//�����ϼ�������
	SV[(N1-1)*N2+1].x=0,SV[(N1-1)*N2+1].y=-r,SV[(N1-1)*N2+1].z=0;	
}

void CTestView::ReadSphereFace()//����������
{
	//���ö�ά��̬����
	SF=new CFace *[N1];//������
	for(int n=0;n<N1;n++)
		SF[n]=new CFace[N2];//������
	for(int j=0;j<N2;j++)//���챱����������Ƭ
	{
		int tempj=j+1;
		if(tempj==N2) tempj=0;//��Ƭ����β����
		int NorthIndex[3];//������������Ƭ����������
		NorthIndex[0]=0;
		NorthIndex[1]=j+1;
		NorthIndex[2]=tempj+1;
		SF[0][j].SetNum(3);
		for(int k=0;k<SF[0][j].vN;k++)
			SF[0][j].vI[k]=NorthIndex[k];
	}
	for(int i=1;i<N1-1;i++)//���������ı�����Ƭ
	{
		for(int j=0;j<N2;j++)
	    {
			int tempi=i+1;
			int tempj=j+1;
			if(tempj==N2) tempj=0;
			int BodyIndex[4];//�����ı�����Ƭ����������
			BodyIndex[0]=(i-1)*N2+j+1;
			BodyIndex[1]=(tempi-1)*N2+j+1;
			BodyIndex[2]=(tempi-1)*N2+tempj+1;
			BodyIndex[3]=(i-1)*N2+tempj+1;
			SF[i][j].SetNum(4);
			for(int k=0;k<SF[i][j].vN;k++)
				SF[i][j].vI[k]=BodyIndex[k];
		}
	}
	for(j=0;j<N2;j++)//�����ϼ���������Ƭ
	{
		int tempj=j+1;
		if(tempj==N2) tempj=0;
		int SouthIndex[3];//�ϼ���������Ƭ����������
		SouthIndex[0]=(N1-2)*N2+j+1;
		SouthIndex[1]=(N1-1)*N2+1;
		SouthIndex[2]=(N1-2)*N2+tempj+1;
		SF[N1-1][j].SetNum(3);	
		for(int k=0;k<SF[N1-1][j].vN;k++)
			SF[N1-1][j].vI[k]=SouthIndex[k];
	}
}

void CTestView::ReadCubeVertex()//���������嶥������
{
	//�������ά����(x,y,z),������߳�Ϊ2a,tΪ͸����
	double a=160;double t=0.7;
	CV[0].x=-a;CV[0].y=-a;CV[0].z=-a;CV[0].c.alpha=t;
	CV[1].x=+a;CV[1].y=-a;CV[1].z=-a;CV[1].c.alpha=t;
	CV[2].x=+a;CV[2].y=+a;CV[2].z=-a;CV[2].c.alpha=t;
	CV[3].x=-a;CV[3].y=+a;CV[3].z=-a;CV[3].c.alpha=t;
	CV[4].x=-a;CV[4].y=-a;CV[4].z=+a;CV[4].c.alpha=t;
	CV[5].x=+a;CV[5].y=-a;CV[5].z=+a;CV[5].c.alpha=t;
	CV[6].x=+a;CV[6].y=+a;CV[6].z=+a;CV[6].c.alpha=t;
	CV[7].x=-a;CV[7].y=+a;CV[7].z=+a;CV[7].c.alpha=t;
}

void CTestView::ReadCubeFace()//�������������
{
	//��Ķ���������Ķ����������������ɫ
	CF[0].SetNum(4);CF[0].vI[0]=4;CF[0].vI[1]=5;CF[0].vI[2]=6;CF[0].vI[3]=7;
	CF[1].SetNum(4);CF[1].vI[0]=0;CF[1].vI[1]=3;CF[1].vI[2]=2;CF[1].vI[3]=1;
	CF[2].SetNum(4);CF[2].vI[0]=0;CF[2].vI[1]=4;CF[2].vI[2]=7;CF[2].vI[3]=3;
	CF[3].SetNum(4);CF[3].vI[0]=1;CF[3].vI[1]=2;CF[3].vI[2]=6;CF[3].vI[3]=5;
	CF[4].SetNum(4);CF[4].vI[0]=2;CF[4].vI[1]=3;CF[4].vI[2]=7;CF[4].vI[3]=6;
	CF[5].SetNum(4);CF[5].vI[0]=0;CF[5].vI[1]=1;CF[5].vI[2]=5;CF[5].vI[3]=4;
}

void CTestView::InitParameter()
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

void CTestView::PerProject(CP3 P)
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

void CTestView::DrawObject(CDC *pDC)//��������
{
	CZBuffer *zbuf=new CZBuffer;//������Ȼ����ڴ�
	zbuf->InitDeepBuffer(800,800,1000,CRGB(0.0,0.0,0.0));//��ʼ����Ⱥͱ���ɫ
	DrawSphere(pDC,zbuf);
	DrawCube(pDC,zbuf);
	delete zbuf;//�ͷ��ڴ�
}

void CTestView::DrawSphere(CDC *pDC,CZBuffer *zbuf)//��������
{
	CPi3 Point3[3];//�ϱ�����������
	CVector Normal3[3];
	CPi3 Point4[4];//���嶥������
	CVector Normal4[4];
	for(int i=0;i<N1;i++)
	{
		for(int j=0;j<N2;j++)
		{
			CVector ViewVector(SV[SF[i][j].vI[0]],ViewPoint);//�����ʸ��
			ViewVector=ViewVector.Normalize();//��λ����ʸ��
			SF[i][j].SetFaceNormal(SV[SF[i][j].vI[0]],SV[SF[i][j].vI[1]],SV[SF[i][j].vI[2]]);
			SF[i][j].fNormal.Normalize();//��λ����ʸ��
			if(Dot(ViewVector,SF[i][j].fNormal)>=0)//�����޳�
			{
				if(3==SF[i][j].vN)
				{
					for(int m=0;m<SF[i][j].vN;m++)
					{
						PerProject(SV[SF[i][j].vI[m]]);
						Point3[m]=ScreenP;
						Normal3[m]=CVector(SV[SF[i][j].vI[m]]);
					}	
					zbuf->SetPoint(Point3,Normal3,3);//��ʼ��
					zbuf->CreateBucket();//����Ͱ��
					zbuf->CreateEdge();//�����߱�
					zbuf->Phong(pDC,ViewPoint,pLight,pMaterial,SPHERE);//��ɫ�������������
					zbuf->ClearMemory();
				}
				else
				{
					for(int m=0;m<SF[i][j].vN;m++)
					{
						PerProject(SV[SF[i][j].vI[m]]);
						Point4[m]=ScreenP;
						Normal4[m]=CVector(SV[SF[i][j].vI[m]]);
					}
					zbuf->SetPoint(Point4,Normal4,4);//��ʼ��
					zbuf->CreateBucket();//����Ͱ��
					zbuf->CreateEdge();//�����߱�
					zbuf->Phong(pDC,ViewPoint,pLight,&pMaterial[0],SPHERE);//��ɫ��������ı���
					zbuf->ClearMemory();
				}
			}
		}
	}
}

void CTestView::DrawCube(CDC* pDC,CZBuffer *zbuf)//�������������
{
	CPi3 Point[4];//��Ķ�ά��������
	CVector Normal4[4];//��ķ�ʸ��
	for(int nFace=0;nFace<6;nFace++)//��ѭ��
	{
		for(int nVertex=0;nVertex<CF[nFace].vN;nVertex++)//����ѭ��
		{
			PerProject(CV[CF[nFace].vI[nVertex]]);//͸��ͶӰ
			Point[nVertex]=ScreenP;
			Normal4[nVertex]=CF[nFace].fNormal;
		}
		zbuf->SetPoint(Point,Normal4,4);//���ö���
		zbuf->CreateBucket();//����Ͱ��
		zbuf->CreateEdge();//�����߱�
		zbuf->Phong(pDC,ViewPoint,pLight,&pMaterial[1],CUBE);//��ɫ��������ı���
		zbuf->ClearMemory();		
	}
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

void CTestView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	Alpha=5;Beta=5;
	tran.RotateX(Alpha);
	tran.RotateY(Beta);
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
		pCmdUI->SetText("����");
	}		
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadSphereVertex();
	ReadSphereFace();
	ReadCubeVertex();
	ReadCubeFace();
	tran.SetMat(CV,8);
	InitParameter();
}




