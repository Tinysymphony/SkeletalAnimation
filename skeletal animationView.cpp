
// skeletal animationView.cpp : CskeletalanimationView ���ʵ��
//

#include "stdafx.h"

#ifndef SHARED_HANDLERS
#include "skeletal animation.h"
#endif

#include "skeletal animationDoc.h"
#include "skeletal animationView.h"
#include "Gesture.h"
#include "FrameList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BUFSIZE 512
#define WM_SET_GESTURE WM_USER+100//�Զ�����Ϣ
#define WM_OK WM_USER+101//�Զ�����Ϣ
#define WM_DELETE WM_USER+102//�Զ�����Ϣ
#define WM_PREVIOUS_FRAME WM_USER+103//�Զ�����Ϣ
#define WM_NEXT_FRAME WM_USER+104//�Զ�����Ϣ
#define WM_GO WM_USER+105//�Զ�����Ϣ
#define WM_GESTURE_CHANGE 106 //����������
#define WM_MYCOLOR 107 //�Զ�����Ϣ������ɫ�ı�
#define WM_MYSPEED 108 //�Զ�����Ϣ���ٶȸ���
#define WM_MYCLEAR 109 //�Զ�����Ϣ������


// CskeletalanimationView

IMPLEMENT_DYNCREATE(CskeletalanimationView, CView)

BEGIN_MESSAGE_MAP(CskeletalanimationView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_WINDOW_OPEN, OnWindowOpen)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_OPEN, OnUpdateWindowOpen)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_CONTROL_RECOVER, OnControlRecover)
	ON_COMMAND(ID_CONTROL_PLAY, OnControlPlay)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_CONTROL_PLAY, OnUpdateControlPlay)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_SCALE, OnUpdateControlScale)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_SET_GESTURE, OnSetGesture)
	ON_MESSAGE(WM_GESTURE_CHANGE, OnGestureChange)
	ON_MESSAGE(WM_PREVIOUS_FRAME, OnPreviousFrame)
	ON_MESSAGE(WM_MYCLEAR, OnClear)
	ON_MESSAGE(WM_NEXT_FRAME, OnNextFrame)
	ON_MESSAGE(WM_GO, OnGo)
	ON_MESSAGE(WM_OK, OnOK)
	ON_MESSAGE(WM_DELETE, OnDelete)
	ON_MESSAGE(WM_MYCOLOR, OnSetBGC)
	ON_MESSAGE(WM_MYSPEED, OnSpeedChange)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_GESTURE, &CskeletalanimationView::OnGesture)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEHWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// CskeletalanimationView ����/����

CskeletalanimationView::CskeletalanimationView(): frame(1)
												,maxframe(1)
{
	// TODO: �ڴ˴���ӹ������
	rotate_up_down = 0;
	rotate_left_right = 0;
	ok = false;
	render_select = false;
	m_play = false;
	scaleMol=1;
    left_right=0;
    up_down=0;
	scaleMolNow=2.0;
	scaleMode = false;
	Rscaling = 1.0;
	spin = 0;
	m_xRotation = 0.0;
	m_yRotation = 0.0;
	m_leftButtonDown = false;

	hx=0,hy=0,hz=0;
	luax=0,luay=0,luaz=0;
	ruax=0,ruay=0,ruaz=0;
	lfx=0,lfy=0,lfz=0;
	rfx=0,rfy=0,rfz=0;
	ltx=0,lty=0,ltz=0;
	rtx=0,rty=0,rtz=0;
	lcx=0,lcy=0,lcz=0;
	rcx=0,rcy=0,rcz=0;

	RGBA = 0;
	RGBB = 0;
	RGBC = 0;

	//BGM();
	
}

void CskeletalanimationView::BGM()
{
	wchar_t   buf[128];

	//use   mciSendCommand   
	MCI_OPEN_PARMS   mciOpen;
	MCIERROR   mciError;
	//mciOpen.lpstrDeviceType   =   (LPCTSTR)MCI_ALL_DEVICE_ID;   
	//mciOpen.lpstrDeviceType   =   "waveaudio";   //ֻ�ܲ���.wav�ļ�   
	//mciOpen.lpstrDeviceType   =   "avivideo";     //*.avi   
	mciOpen.lpstrDeviceType = L"mpegvideo";
	//mciOpen.lpstrDeviceType   =   "sequencer";   
	mciOpen.lpstrElementName = L"C:\\Users\\Tiny\\Desktop\\skeletal animation\\skeletal animation\\res\\TE.mp3";
	//mciOpen.lpstrElementName   =   "e:\\movie\\first.avi";   
	mciError = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)&mciOpen);
	if (mciError)
	{
		mciGetErrorString(mciError, buf, 128);
		MessageBox(L"send MCI_PLAY command failed", L"ERROR");
		return;
	}
	UINT   DeviceID = mciOpen.wDeviceID;
	MCI_PLAY_PARMS   mciPlay;

	//mciError   =   mciSendCommand(DeviceID,MCI_PLAY,0   ,(DWORD)&mciPlay);  
	//MCI_FROM | MCI_TO | 
	mciError = mciSendCommand(DeviceID, MCI_PLAY, MCI_WAIT,
		(DWORD)(LPMCI_PLAY_PARMS)&mciPlay);  //MCI_DGV_PLAY_REPEAT, Ҫ #include "Digitalv.h"
	if (mciError)
	{
		mciGetErrorString(mciError, buf, 128);
		MessageBox(L"send MCI_PLAY command failed", L"ERROR");
		return;
	}

}

CskeletalanimationView::~CskeletalanimationView()
{
}


BOOL CskeletalanimationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	pDialog = new Gesture(this);		

	if(pDialog != NULL)
	{
		BOOL ret = pDialog->Create(IDD_DIALOG_GESTURE,this);				//������ģ̬�Ի���
		if(!ret)   											//����ʧ��
			AfxMessageBox(_T("�����Ի���ʧ��"));	
	}
	else
		AfxMessageBox(_T("�Ի��������ʧ��"));

	return CView::PreCreateWindow(cs);
}


void CskeletalanimationView::OnDraw(CDC* /*pDC*/)
{
	CskeletalanimationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	display();/*��ʾȫ����ͼ����*/
}


int CskeletalanimationView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO: Add your specialized creation code here
	if( SetupPixelFormat() == false )/*�������ظ�ʽ*/
	{
		return 0;
	}
//	AfxMessageBox( "Test" );
	if( CreateOpenGLContext() == false )/*������ͼ������*/
	{
		return 0;
	}
	myinit();/*��ʼ��*/
	return 0;
}


void CskeletalanimationView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	if( wglGetCurrentContext() != NULL )/*��ǰ�������Ƿ����豸��������*/
	{
		wglMakeCurrent( NULL, NULL );/*�����������ȡ�������*/
	}

	if( m_hRC )/*��ͼ�������Ƿ�Ϊ��*/
	{
		wglDeleteContext( m_hRC );/*�����Ϊ�գ������ÿ�*/
		m_hRC = NULL;
	}	
}


BOOL CskeletalanimationView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return true;/*��Ϊ�ֶ�ˢ����Ļ*/
}

/*���ڴ�С�ı�*/
void CskeletalanimationView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	w=cx;
	h=cy;
//	AfxMessageBox( "Test" );
/*	glViewport( 0, 0, cx, cy );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
//	glFrustum( -1.0, 1.0, -1.0, 1.0, 1.5, 20.0 );
	gluPerspective( 60.0, (GLfloat)w/(GLfloat)h, 1.0, 500.0 );
	if( w <= h )
	{
		glOrtho( -1.5, 1.5, -1.5*(GLdouble)h/(GLdouble)w, 1.5*(GLdouble)h/(GLdouble)w, -10.0, 10.0 );
	}
	else
	{
		glOrtho( -1.5*(GLdouble)w/(GLdouble)h, 1.5*(GLdouble)w/(GLdouble)h, -1.5, 1.5, -10.0, 10.0 );
	}
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef( 0.0, 0.0, -5.5 );
	Invalidate();*/	
}


bool CskeletalanimationView::SetupPixelFormat()
{
	HWND hWnd = GetSafeHwnd();
	m_hDC = ::GetDC( hWnd );/*�����豸������*/


	static PIXELFORMATDESCRIPTOR pfd =/*���ظ�ʽ�ṹ��ʼ��*/
	{
		sizeof( PIXELFORMATDESCRIPTOR ),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		 0, 0, 0, 0, 0, 0,               // color bits ignored
        0,                              // no alpha buffer
        0,                              // shift bit ignored
        0,                              // no accumulation buffer
        0, 0, 0, 0,                     // accum bits ignored
        16,                             // 16-bit z-buffer
        0,                              // no stencil buffer
        0,                              // no auxiliary buffer
        PFD_MAIN_PLANE,                 // main layer
        0,                              // reserved
        0, 0, 0                         // layer masks ignored
    };

	int m_PixelFormat;
	m_PixelFormat = ::ChoosePixelFormat( m_hDC, &pfd );/*ѡ�����豸��������ص����ظ�ʽ����*/
   
    SetPixelFormat(m_hDC, m_PixelFormat, &pfd );/*�������ظ�ʽ*/

	return true;
}


bool CskeletalanimationView::CreateOpenGLContext()
{
	m_hRC = ::wglCreateContext( m_hDC );/*������ͼ������*/
//	AfxMessageBox( "Test" );
	if( m_hRC == NULL )
	{
		return false;
	}
 //   AfxMessageBox( "Test" );
	if( wglMakeCurrent( m_hDC, m_hRC )==false)/*�豸���������ͼ�������Ƿ�����ɹ�*/
	{
		return false;
	}
//	AfxMessageBox( "Test" );

	return true;
}


void CskeletalanimationView::myinit()
{
	glClearColor( 0.0, 0.0, 0.0, 1.0 );/*����������ɫ*/
	glClearDepth( 1.0 );/*������Ȼ���*/
	glShadeModel( GL_SMOOTH );/*ƽ����ɫ*/

	righthand=glmReadOBJ("����.obj");/*��ʼ����ģ��ָ��*/
	head = glmReadOBJ("ͷ.obj");/*��ʼͷģ��ָ��*/
	body = glmReadOBJ("����.obj");/*��ʼ����ģ��ָ��*/
	leftdatui= glmReadOBJ("�����.obj");/*��ʼ�����ģ��ָ��*/
	rightdatui=glmReadOBJ("�Ҵ���.obj");/*��ʼ�Ҵ���ģ��ָ��*/
	rightxiaotui=glmReadOBJ("��С��.obj");/*��ʼ��С��ģ��ָ��*/
    leftxiaotui=glmReadOBJ("��С��.obj");/*��ʼ��С��ģ��ָ��*/
	leftfoot=glmReadOBJ("���.obj");/*��ʼ���ģ��ָ��*/
    rightfoot=glmReadOBJ("�ҽ�.obj");/*��ʼ�ҽ�ģ��ָ��*/
	leftbi=glmReadOBJ("���.obj");/*��ʼ���ģ��ָ��*/
	rightbi=glmReadOBJ("�ұ�.obj");/*��ʼ�ұ�ģ��ָ��*/
	leftzhou=glmReadOBJ("����.obj");/*��ʼ����ģ��ָ��*/
	rightzhou=glmReadOBJ("����.obj");/*��ʼ����ģ��ָ��*/
    lefthand=glmReadOBJ("����.obj");/*��ʼ����ģ��ָ��*/
   

	/*��ģ�ͷŴ��ı�*/
	glmScale(head,4.0f);
	glmScale(body,4.0f);
	glmScale(leftdatui,4.0f);
	glmScale( rightdatui,4.0f );
	glmScale( leftxiaotui,4.0f );
	glmScale(  rightxiaotui,4.0f );
	glmScale( leftfoot,4.0f );
	glmScale( rightfoot,4.0f );
	glmScale(leftbi,4.0f );
	glmScale( rightbi,4.0f );
	glmScale( leftzhou,4.0f );
	glmScale( rightzhou,4.0f );
    glmScale( lefthand,4.0f );
	glmScale( righthand,4.0f );

	GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	glLightfv( GL_LIGHT0, GL_SPECULAR, light_ambient );/*���û�������ɫ*/
	glEnable( GL_LIGHTING );/*��������*/
	glEnable( GL_LIGHT0 );/*������Դ0*/
	glEnable( GL_DEPTH_TEST );/*������Ȳ���*/
	glEnable( GL_LESS );
}

/*��ʾ����*/
void CskeletalanimationView::display()
{
//	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if( ok )/*�ඥ��ͼ*/
    {

		glEnable( GL_SCISSOR_TEST );/*�������ò���*/
		glScissor( 0, 0, w/2, h );/*��һ����ʾ�����СΪ�����ͻ��������*/
		glClearColor( RGBA, RGBB, RGBC, 1.0 );/*���õ�һ���ͻ�����������ɫ*/
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );/*�����ɫ�������Ȼ���*/
		glDisable( GL_SCISSOR_TEST );/*�رռ��ò���*/
		glViewport( 0, 0, w/2, h );/*�����ӿ�*/
		glMatrixMode( GL_PROJECTION );//����ͶӰģʽ
		glLoadIdentity();/*��յ�ǰ����*/
		if( (w/2) <= h )/*����ͶӰ��ʽ*/
		{
			glOrtho( -1.5, 1.5, -1.5*(GLdouble)h/(GLdouble)(w/2), 1.5*(GLdouble)h/(GLdouble)(w/2), -10.0, 10.0 );
		}
		else
		{
			glOrtho( -1.5*(GLdouble)(w/2)/(GLdouble)h, 1.5*(GLdouble)(w/2)/(GLdouble)h, -1.5, 1.5, -10.0, 10.0 );
		}
		glMatrixMode( GL_MODELVIEW );/*�˳�ͶӰģʽ��������ͼģʽ*/
		glLoadIdentity();
		displayFront( GL_RENDER );


		glEnable( GL_SCISSOR_TEST );/*�������ò���*/
		glScissor( w/2, 0, w/2, h/2 );/*�ڶ�������Ĵ�СΪ�Ұ벿�ֵ��ϰ벿��*/
		glClearColor( RGBA+0.05, RGBB+0.05,RGBC+0.05, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glDisable( GL_SCISSOR_TEST );
		glViewport( w/2, 0, w/2, h/2);
		glMatrixMode( GL_PROJECTION );//����ͶӰģʽ
		glLoadIdentity();/*��յ�ǰ����*/
		if( (w) <= h )/*����ͶӰ��ʽ*/
		{
			glOrtho( -1.5, 1.5, -1.5*(GLdouble)h/(GLdouble)(w), 1.5*(GLdouble)h/(GLdouble)(w), -10.0, 10.0 );
		}
		else
		{
			glOrtho( -1.5*(GLdouble)(w)/(GLdouble)h, 1.5*(GLdouble)(w)/(GLdouble)h, -1.5, 1.5, -10.0, 10.0 );
		}
		glMatrixMode( GL_MODELVIEW );/*�˳�ͶӰģʽ��������ͼģʽ*/
		glLoadIdentity();
	    displayTop(GL_RENDER);

		
		glEnable( GL_SCISSOR_TEST );
		glScissor( w/2, h/2, w/2, h/2 );/*�������ͻ������Ұ벿�ֵ��°벿��*/
		glClearColor( RGBA-0.05,RGBB-0.05, RGBC-0.05, 1.0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glDisable( GL_SCISSOR_TEST );
		glViewport( w/2, h/2, w/2, h/2);
		glMatrixMode( GL_PROJECTION );//����ͶӰģʽ
		glLoadIdentity();/*��յ�ǰ����*/
		if( (w) <= h )/*����ͶӰ��ʽ*/
		{
			glOrtho( -1.5, 1.5, -1.5*(GLdouble)h/(GLdouble)(w), 1.5*(GLdouble)h/(GLdouble)(w), -10.0, 10.0 );
		}
		else
		{
			glOrtho( -1.5*(GLdouble)(w)/(GLdouble)h, 1.5*(GLdouble)(w)/(GLdouble)h, -1.5, 1.5, -10.0, 10.0 );
		}
		glMatrixMode( GL_MODELVIEW );/*�˳�ͶӰģʽ��������ͼģʽ*/
		glLoadIdentity();
		displayBeside(GL_RENDER);
		

	}
	else/*ȫ����ͼ��*/
	{

		glClearColor( RGBA, RGBB, RGBC, 0.0 );
		glClearDepth( 1.0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glMatrixMode( GL_PROJECTION );//����ͶӰģʽ

		glLoadIdentity();

		

		if( (w) <= h )
		{
			glOrtho( -1.5, 1.5, -1.5*(GLdouble)h/(GLdouble)(w), 1.5*(GLdouble)h/(GLdouble)(w), -10.0, 10.0 );
		}
		else
		{
			glOrtho( -1.5*(GLdouble)(w)/(GLdouble)h, 1.5*(GLdouble)(w)/(GLdouble)h, -1.5, 1.5, -10.0, 10.0 );
		}
//		glFrustum( -1.0, 1.0, -1.0, 1.0, 1.5, 20.0 );

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		glViewport( 0, 0, w, h );
	/////////////////////////////////////////////////////////////

		displayFront(GL_RENDER);

	}
 
	//////////////////////////////////////////////////////////////////
	glFlush();/*ǿ����ʾģ��*/
	SwapBuffers( m_hDC );/*����ǰ�󻺴�*/

}

/*��ʾǰ��ͼ*/
void CskeletalanimationView::displayFront( GLenum mode )
{
	glPushMatrix( );//
	glLoadIdentity();

	glTranslated( 0.0, 0.0, -5.0 ); /*��ģ������ƽ��5����λ*/
    glRotated( -90, 1.0, 0.0, 0.0 );/*��ģ����X��˳ʱ����ת90��*/



	if( !ok )
	{
		glTranslated( 0.0, 0.0, up_down); /*����ƽ��*/
	    glTranslated( left_right, 0.0, 0.0); /*����ƽ��*/
	} 
	glRotated( m_xRotation, 0.0, 0.0, 1.0 );
	glRotated( m_yRotation, 1.0, 0.0, 0.0 );

    glRotated(rotate_left_right, 0.0, 0.0, 1.0 );/*������ת*/
	glRotated( rotate_up_down, 1.0, 0.0, 0.0 );/*������ת*/
    glTranslated(0.0, 0.0, -1.3*Rscaling);/*����ƽ��ģ��*/
	
	glPushMatrix();/*���ƿ�ʼ*/
	
	glTranslated( 0.0, 0.0, 2.514*Rscaling );
    glRotated( hx, 1, 0, 0 );/*��ͷ*/
	glRotated( hy, 0, 1, 0 );
	glRotated( hz, 0, 0, 1 );
	glTranslated( 0.0, 0.0, -2.514*Rscaling );
	
	if( mode == GL_SELECT )
	{
		glLoadName( 1 );
	}
	glmDraw( head, GLM_SMOOTH | GLM_COLOR );/*����ͷģ��*/	
    
	glPopMatrix();/*ͷ���ƽ���*/
	
	if( mode == GL_SELECT )
	{
		glLoadName( 2 );
	}

	glmDraw( body, GLM_SMOOTH | GLM_COLOR );/*��������*/
    
	glPushMatrix();/*���ȿ�ʼ����*/
	glTranslated( 0.12*Rscaling, 0.0, 1.51*Rscaling );
    glRotated(ltx, 1, 0, 0 );/*�������ת*/
	glRotated(lty, 0, 1, 0 );
	glRotated(ltz, 0, 0, 1 );
	glTranslated( -0.12*Rscaling, 0.0, -1.51*Rscaling );
	
	if( mode == GL_SELECT )
	{
		glLoadName( 3 );
	}
   	glmDraw(leftdatui, GLM_SMOOTH | GLM_COLOR );
	glPushMatrix();/*��С�ȿ�ʼ����*/
	glTranslated( 0.164*Rscaling, -0.07*Rscaling, 0.81*Rscaling );
    glRotated(lcx, 1, 0, 0 );/*��С����ת*/
	glRotated(lcy, 0, 1, 0 );
	glRotated(lcz, 0, 0, 1 );
	glTranslated( -0.164*Rscaling, 0.07*Rscaling, -0.81 *Rscaling);

	if( mode == GL_SELECT )
	{
		glLoadName( 4 );
	}
    glmDraw(leftxiaotui, GLM_SMOOTH | GLM_COLOR );
    glPushMatrix();/*��ſ�ʼ����*/

	if( mode == GL_SELECT )
	{
		glLoadName( 5 );
	}
    glmDraw(leftfoot, GLM_SMOOTH | GLM_COLOR ); 
    glPopMatrix();/*��Ż��ƽ���*/

	glPopMatrix();/*��С�Ȼ��ƽ���*/

	glPopMatrix();/*����Ȼ��ƽ���*/

	glPushMatrix();/*���ȿ�ʼ����*/
	glTranslated( -0.12*Rscaling, 0.0, 1.51*Rscaling );
    glRotated(rtx, 1, 0, 0 );/*������ת*/
	glRotated(rty, 0, 1, 0 );
	glRotated(rtz, 0, 0, 1 );
	glTranslated( 0.12*Rscaling, 0.0, -1.51 *Rscaling);

	if( mode == GL_SELECT )
	{
		glLoadName( 6 );
	}
    glmDraw(rightdatui, GLM_SMOOTH | GLM_COLOR );
	glPushMatrix();/*��С�ȿ�ʼ����*/
	glTranslated( -0.164*Rscaling, -0.07*Rscaling, 0.81 *Rscaling);
    glRotated(rcx, 1, 0, 0 );/*��С����ת*/
	glRotated(rcy, 0, 1, 0 );
	glRotated(rcz, 0, 0, 1 );
	glTranslated( 0.164*Rscaling, 0.07*Rscaling, -0.81*Rscaling );

	if( mode == GL_SELECT )
	{
		glLoadName( 7 );
	}
    glmDraw(rightxiaotui, GLM_SMOOTH | GLM_COLOR );
    glPushMatrix();/*�ҽſ�ʼ����*/

	if( mode == GL_SELECT )
	{
		glLoadName( 8 );
	}
    glmDraw(rightfoot, GLM_SMOOTH | GLM_COLOR ); 
    glPopMatrix();/*�ҽŻ��ƽ���*/
    glPopMatrix();/*��С�Ȼ��ƽ���*/
	glPopMatrix();/*�Ҵ��Ȼ��ƽ���*/


	glPushMatrix();/*��ۿ�ʼ����*/ 

	glTranslated(0.25*Rscaling, 0.05*Rscaling, 2.23*Rscaling);
	glRotated(luax, 1, 0, 0 );/*�����ת*/
	glRotated(luay, 0, 1, 0 );
	glRotated(luaz, 0, 0, 1 );
	glTranslated(-0.25*Rscaling, -0.05*Rscaling, -2.23*Rscaling); 
	
	if( mode == GL_SELECT )
	{
		glLoadName( 9 );
	}
	//glPushMatrix();/*�����������ת30��*/
	//glTranslated( 0.25*Rscaling, 0.0, 2.24*Rscaling );
	//glRotated( 30, 0.0, 1.0, 0.0 );
	//glTranslated( -0.25*Rscaling, 0.0, -2.24*Rscaling );
	glmDraw(leftbi, GLM_SMOOTH | GLM_COLOR );
	
    glPushMatrix();/*���⿪ʼ����*/
    glTranslated(0.50*Rscaling, 0.05*Rscaling, 1.90*Rscaling);
    glRotated(lfx, 1, 0, 0);/*������ת*/
	glRotated(lfy, 0, 1, 0);
	glRotated(lfz, 0, 0, 1);
	glTranslated(-0.50*Rscaling, -0.05*Rscaling, -1.90*Rscaling);

	if( mode == GL_SELECT )
	{
		glLoadName( 10 );
	}
	glmDraw(leftzhou, GLM_SMOOTH | GLM_COLOR );
    glPushMatrix();/*���ֿ�ʼ����*/

	if( mode == GL_SELECT )
	{
		glLoadName( 11 );
	}
    glmDraw(lefthand, GLM_SMOOTH | GLM_COLOR ); 
    glPopMatrix();/*���ֻ��ƽ���*/
    glPopMatrix();/*������ƽ���*/
    //glPopMatrix();/*��ۻ��ƽ���*/
	glPopMatrix();



	glPushMatrix();/*�ұۿ�ʼ����*/
	glTranslated(-0.25, 0.05*Rscaling, 2.23*Rscaling);
	glRotated(ruax, 1, 0, 0 );/*�ұ���ת*/
	glRotated(ruay, 0, 1, 0 );
	glRotated(ruaz, 0, 0, 1 );
	glTranslated(0.25, -0.05*Rscaling, -2.23*Rscaling);

	if( mode == GL_SELECT )
	{
		glLoadName( 12 );
	}
    glmDraw(rightbi, GLM_SMOOTH | GLM_COLOR );
	glPushMatrix();/*���⿪ʼ����*/
	glTranslated(-0.5*Rscaling, 0.05*Rscaling, 1.90*Rscaling);
	glRotated(rfx, 1, 0, 0 );/*������ת*/
	glRotated(rfy, 0, 1, 0 );
	glRotated(rfz, 0, 0, 1 );
	glTranslated(0.5*Rscaling, -0.05*Rscaling, -1.90*Rscaling);

	if( mode == GL_SELECT )
	{
		glLoadName( 13 );
	}
    glmDraw(rightzhou, GLM_SMOOTH | GLM_COLOR );
    glPushMatrix();/*���ֿ�ʼ����*/
	if( mode == GL_SELECT )
	{
		glLoadName( 14 );
	}
    glmDraw(righthand, GLM_SMOOTH | GLM_COLOR ); 
    glPopMatrix();/*���ֻ��ƽ���*/
    glPopMatrix();/*������ƽ���*/
    glPopMatrix();/*�ұۻ��ƽ���*/
	glPopMatrix();/*ȫ�����ƽ���*/

}

//���Ͻ�
void CskeletalanimationView::displayBeside( GLenum mode )
{
	CskeletalanimationDoc* pDoc=GetDocument();
	glPushMatrix( );//
	
	glTranslated( 0.0, 0.0, -5.0 ); /*��ģ������ƽ��5����λ*/

	glRotated( -90, 1.0, 0.0, 0.0 );/*��ģ����X��˳ʱ����ת90��*/
	glRotated( m_xRotation, 0.0, 0.0, 1.0 );
	glRotated( m_yRotation, 1.0, 0.0, 0.0 );

    glRotated(rotate_left_right, 0.0, 0.0, 1.0 );/*������ת*/
	glRotated( rotate_up_down, 1.0, 0.0, 0.0 );/*������ת*/
    glTranslated(0.0, 0.0, -1.3*Rscaling);/*����ƽ��ģ��*/
   
	
	glPushMatrix();/*���ƿ�ʼ*/
	
	glTranslated( 0.0, 0.0, 2.514*Rscaling );
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).Hx, 1, 0, 0 );/*��ͷ*/
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).Hy, 0, 1, 0 );
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).Hz, 0, 0, 1 );
	glTranslated( 0.0, 0.0, -2.514*Rscaling );
	
	if( mode == GL_SELECT )
	{
		glLoadName( 1 );
	}
	glmDraw( head, GLM_SMOOTH | GLM_COLOR );/*����ͷģ��*/	
    
	glPopMatrix();/*ͷ���ƽ���*/
	
	if( mode == GL_SELECT )
	{
		glLoadName( 2 );
	}

	glmDraw( body, GLM_SMOOTH | GLM_COLOR );/*��������*/
    
	
	glPushMatrix();/*���ȿ�ʼ����*/
	glTranslated( 0.12*Rscaling, 0.0, 1.51*Rscaling );
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).ltx, 1, 0, 0 );
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).lty, 0, 1, 0 );
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).ltz, 0, 0, 1 );
	glTranslated( -0.12*Rscaling, 0.0, -1.51*Rscaling );

	
	if( mode == GL_SELECT )
	{
		glLoadName( 3 );
	}
   	glmDraw(leftdatui, GLM_SMOOTH | GLM_COLOR );
	glPushMatrix();/*��С�ȿ�ʼ����*/
	glTranslated( 0.164*Rscaling, -0.07*Rscaling, 0.81*Rscaling );
    glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).lcx, 1, 0, 0 );
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).lcy, 0, 1, 0 );
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).lcz, 0, 0, 1 );/*��С����ת*/
	glTranslated( -0.164*Rscaling, 0.07*Rscaling, -0.81 *Rscaling);

	if( mode == GL_SELECT )
	{
		glLoadName( 4 );
	}
    glmDraw(leftxiaotui, GLM_SMOOTH | GLM_COLOR );
    glPushMatrix();/*��ſ�ʼ����*/

	if( mode == GL_SELECT )
	{
		glLoadName( 5 );
	}
    glmDraw(leftfoot, GLM_SMOOTH | GLM_COLOR ); 
    glPopMatrix();/*��Ż��ƽ���*/

	glPopMatrix();/*��С�Ȼ��ƽ���*/

	glPopMatrix();/*����Ȼ��ƽ���*/

	glPushMatrix();/*���ȿ�ʼ����*/
	glTranslated( -0.12*Rscaling, 0.0, 1.51*Rscaling );
    glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).rtx, 1, 0, 0 );
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).rty, 0, 1, 0 );
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).rtz, 0, 0, 1 );/*������ת*/
	glTranslated( 0.12*Rscaling, 0.0, -1.51 *Rscaling);

	if( mode == GL_SELECT )
	{
		glLoadName( 6 );
	}
    glmDraw(rightdatui, GLM_SMOOTH | GLM_COLOR );
	glPushMatrix();/*��С�ȹ�ʼ����*/
	glTranslated( -0.164*Rscaling, -0.07*Rscaling, 0.81 *Rscaling);
    glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).rcx, 1, 0, 0 );
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).rcy, 0, 1, 0 );
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).rcz, 0, 0, 1 );/*��С����ת*/
	glTranslated( 0.164*Rscaling, 0.07*Rscaling, -0.81*Rscaling );

	if( mode == GL_SELECT )
	{
		glLoadName( 7 );
	}
    glmDraw(rightxiaotui, GLM_SMOOTH | GLM_COLOR );
    glPushMatrix();/*�ҽſ�ʼ����*/

	if( mode == GL_SELECT )
	{
		glLoadName( 8 );
	}
    glmDraw(rightfoot, GLM_SMOOTH | GLM_COLOR ); 
    glPopMatrix();/*�ҽŻ��ƽ���*/
    glPopMatrix();/*��С�Ȼ��ƽ���*/
	glPopMatrix();/*�Ҵ��Ȼ��ƽ���*/


	glPushMatrix();/*��ۿ�ʼ����*/

	glTranslated(0.25*Rscaling, 0.05*Rscaling, 2.23*Rscaling);
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).luax, 1, 0, 0 );
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).luay, 0, 1, 0 );
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).luaz, 0, 0, 1 );/*�����ת*/
	glTranslated(-0.25*Rscaling, -0.05*Rscaling, -2.23*Rscaling); 
	
	if( mode == GL_SELECT )
	{
		glLoadName( 9 );
	}
	glmDraw(leftbi, GLM_SMOOTH | GLM_COLOR );
	
    glPushMatrix();/*���⿪ʼ����*/
    glTranslated(0.50*Rscaling, 0.05*Rscaling, 1.90*Rscaling);
    glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).lfx, 1, 0, 0 );
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).lfy, 0, 1, 0 );
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).lfz, 0, 0, 1 );/*������ת*/
	glTranslated(-0.50*Rscaling, -0.05*Rscaling, -1.90*Rscaling);

	if( mode == GL_SELECT )
	{
		glLoadName( 10 );
	}
	glmDraw(leftzhou, GLM_SMOOTH | GLM_COLOR );
    glPushMatrix();/*���ֿ�ʼ����*/

	if( mode == GL_SELECT )
	{
		glLoadName( 11 );
	}
    glmDraw(lefthand, GLM_SMOOTH | GLM_COLOR ); 
    glPopMatrix();/*���ֻ��ƽ���*/
    glPopMatrix();/*������ƽ���*/
	glPopMatrix();


	glPushMatrix();/*�ұۿ�ʼ����*/
	glTranslated(-0.25, 0.05*Rscaling, 2.23*Rscaling);
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).ruax, 1, 0, 0 );
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).ruay, 0, 1, 0 );
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).ruaz, 0, 0, 1 );/*�ұ���ת*/
	glTranslated(0.25, -0.05*Rscaling, -2.23*Rscaling);

	if( mode == GL_SELECT )
	{
		glLoadName( 12 );
	}
    glmDraw(rightbi, GLM_SMOOTH | GLM_COLOR );
	glPushMatrix();/*���⿪ʼ����*/
	glTranslated(-0.5*Rscaling, 0.05*Rscaling, 1.90*Rscaling);
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).rfx, 1, 0, 0 );
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).rfy, 0, 1, 0 );
	glRotated( pDoc->framelist.find(frame-10<1?1:frame-10).rfz, 0, 0, 1 );/*������ת*/
	glTranslated(0.5*Rscaling, -0.05*Rscaling, -1.90*Rscaling);

	if( mode == GL_SELECT )
	{
		glLoadName( 13 );
	}
    glmDraw(rightzhou, GLM_SMOOTH | GLM_COLOR );
    glPushMatrix();/*���ֿ�ʼ����*/
	if( mode == GL_SELECT )
	{
		glLoadName( 14 );
	}
    glmDraw(righthand, GLM_SMOOTH | GLM_COLOR ); 
    glPopMatrix();/*���ֻ��ƽ���*/
    glPopMatrix();/*������ƽ���*/
    glPopMatrix();/*�ұۻ��ƽ���*/
	glPopMatrix();/*ȫ�����ƽ���*/

}

//���½�
void CskeletalanimationView::displayTop( GLenum mode)
{
	CskeletalanimationDoc* pDoc = GetDocument();
	glPushMatrix();//

	glTranslated(0.0, 0.0, -5.0); /*��ģ������ƽ��5����λ*/

	glRotated(-90, 1.0, 0.0, 0.0);/*��ģ����X��˳ʱ����ת90��*/
	glRotated(m_xRotation, 0.0, 0.0, 1.0);
	glRotated(m_yRotation, 1.0, 0.0, 0.0);
	glRotated(rotate_left_right, 0.0, 0.0, 1.0);/*������ת*/
	glRotated(rotate_up_down, 1.0, 0.0, 0.0);/*������ת*/
	glTranslated(0.0, 0.0, -1.3*Rscaling);/*����ƽ��ģ��*/


	glPushMatrix();/*���ƿ�ʼ*/

	glTranslated(0.0, 0.0, 2.514*Rscaling);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).Hx, 1, 0, 0);/*��ͷ*/
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).Hy, 0, 1, 0);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).Hz, 0, 0, 1);
	glTranslated(0.0, 0.0, -2.514*Rscaling);

	if (mode == GL_SELECT)
	{
		glLoadName(1);
	}
	glmDraw(head, GLM_SMOOTH | GLM_COLOR);/*����ͷģ��*/

	glPopMatrix();/*ͷ���ƽ���*/

	if (mode == GL_SELECT)
	{
		glLoadName(2);
	}

	glmDraw(body, GLM_SMOOTH | GLM_COLOR);/*��������*/


	glPushMatrix();/*���ȿ�ʼ����*/
	glTranslated(0.12*Rscaling, 0.0, 1.51*Rscaling);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).ltx, 1, 0, 0);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).lty, 0, 1, 0);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).ltz, 0, 0, 1);
	glTranslated(-0.12*Rscaling, 0.0, -1.51*Rscaling);


	if (mode == GL_SELECT)
	{
		glLoadName(3);
	}
	glmDraw(leftdatui, GLM_SMOOTH | GLM_COLOR);
	glPushMatrix();/*��С�ȿ�ʼ����*/
	glTranslated(0.164*Rscaling, -0.07*Rscaling, 0.81*Rscaling);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).lcx, 1, 0, 0);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).lcy, 0, 1, 0);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).lcz, 0, 0, 1);/*��С����ת*/
	glTranslated(-0.164*Rscaling, 0.07*Rscaling, -0.81 *Rscaling);

	if (mode == GL_SELECT)
	{
		glLoadName(4);
	}
	glmDraw(leftxiaotui, GLM_SMOOTH | GLM_COLOR);
	glPushMatrix();/*��ſ�ʼ����*/

	if (mode == GL_SELECT)
	{
		glLoadName(5);
	}
	glmDraw(leftfoot, GLM_SMOOTH | GLM_COLOR);
	glPopMatrix();/*��Ż��ƽ���*/

	glPopMatrix();/*��С�Ȼ��ƽ���*/

	glPopMatrix();/*����Ȼ��ƽ���*/

	glPushMatrix();/*���ȿ�ʼ����*/
	glTranslated(-0.12*Rscaling, 0.0, 1.51*Rscaling);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).rtx, 1, 0, 0);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).rty, 0, 1, 0);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).rtz, 0, 0, 1);/*������ת*/
	glTranslated(0.12*Rscaling, 0.0, -1.51 *Rscaling);

	if (mode == GL_SELECT)
	{
		glLoadName(6);
	}
	glmDraw(rightdatui, GLM_SMOOTH | GLM_COLOR);
	glPushMatrix();/*��С�ȹ�ʼ����*/
	glTranslated(-0.164*Rscaling, -0.07*Rscaling, 0.81 *Rscaling);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).rcx, 1, 0, 0);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).rcy, 0, 1, 0);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).rcz, 0, 0, 1);/*��С����ת*/
	glTranslated(0.164*Rscaling, 0.07*Rscaling, -0.81*Rscaling);

	if (mode == GL_SELECT)
	{
		glLoadName(7);
	}
	glmDraw(rightxiaotui, GLM_SMOOTH | GLM_COLOR);
	glPushMatrix();/*�ҽſ�ʼ����*/

	if (mode == GL_SELECT)
	{
		glLoadName(8);
	}
	glmDraw(rightfoot, GLM_SMOOTH | GLM_COLOR);
	glPopMatrix();/*�ҽŻ��ƽ���*/
	glPopMatrix();/*��С�Ȼ��ƽ���*/
	glPopMatrix();/*�Ҵ��Ȼ��ƽ���*/


	glPushMatrix();/*��ۿ�ʼ����*/

	glTranslated(0.25*Rscaling, 0.05*Rscaling, 2.23*Rscaling);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).luax, 1, 0, 0);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).luay, 0, 1, 0);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).luaz, 0, 0, 1);/*�����ת*/
	glTranslated(-0.25*Rscaling, -0.05*Rscaling, -2.23*Rscaling);

	if (mode == GL_SELECT)
	{
		glLoadName(9);
	}
	glmDraw(leftbi, GLM_SMOOTH | GLM_COLOR);

	glPushMatrix();/*���⿪ʼ����*/
	glTranslated(0.50*Rscaling, 0.05*Rscaling, 1.90*Rscaling);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).lfx, 1, 0, 0);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).lfy, 0, 1, 0);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).lfz, 0, 0, 1);/*������ת*/
	glTranslated(-0.50*Rscaling, -0.05*Rscaling, -1.90*Rscaling);

	if (mode == GL_SELECT)
	{
		glLoadName(10);
	}
	glmDraw(leftzhou, GLM_SMOOTH | GLM_COLOR);
	glPushMatrix();/*���ֿ�ʼ����*/

	if (mode == GL_SELECT)
	{
		glLoadName(11);
	}
	glmDraw(lefthand, GLM_SMOOTH | GLM_COLOR);
	glPopMatrix();/*���ֻ��ƽ���*/
	glPopMatrix();/*������ƽ���*/
	glPopMatrix();


	glPushMatrix();/*�ұۿ�ʼ����*/
	glTranslated(-0.25, 0.05*Rscaling, 2.23*Rscaling);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).ruax, 1, 0, 0);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).ruay, 0, 1, 0);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).ruaz, 0, 0, 1);/*�ұ���ת*/
	glTranslated(0.25, -0.05*Rscaling, -2.23*Rscaling);

	if (mode == GL_SELECT)
	{
		glLoadName(12);
	}
	glmDraw(rightbi, GLM_SMOOTH | GLM_COLOR);
	glPushMatrix();/*���⿪ʼ����*/
	glTranslated(-0.5*Rscaling, 0.05*Rscaling, 1.90*Rscaling);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).rfx, 1, 0, 0);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).rfy, 0, 1, 0);
	glRotated(pDoc->framelist.find(frame+10>maxframe?maxframe:frame+10).rfz, 0, 0, 1);/*������ת*/
	glTranslated(0.5*Rscaling, -0.05*Rscaling, -1.90*Rscaling);

	if (mode == GL_SELECT)
	{
		glLoadName(13);
	}
	glmDraw(rightzhou, GLM_SMOOTH | GLM_COLOR);
	glPushMatrix();/*���ֿ�ʼ����*/
	if (mode == GL_SELECT)
	{
		glLoadName(14);
	}
	glmDraw(righthand, GLM_SMOOTH | GLM_COLOR);
	glPopMatrix();/*���ֻ��ƽ���*/
	glPopMatrix();/*������ƽ���*/
	glPopMatrix();/*�ұۻ��ƽ���*/
	glPopMatrix();/*ȫ�����ƽ���*/

}

void CskeletalanimationView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CMenu menu;
	menu.LoadMenu( IDR_POPUP_EDIT );/*���뵯���˵�*/
	menu.GetSubMenu(0)->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
		                                point.x, point.y, this );
/*	if( menu.GetSubMenu(0)->EnableMenuItem( ID_CONTROL_OPENTB, MF_ENABLED ) == -1)
	{
		AfxMessageBox("-1");
	}
	else if( menu.GetSubMenu(0)->EnableMenuItem( ID_CONTROL_OPENTB, MF_ENABLED ) == (MF_ENABLED) )
	{
		AfxMessageBox("OK");
	}*/
//	menu.GetSubMenu(0)->EnableMenuItem( ID_CONTROL_VIEW, MF_DISABLED );
}

/*�ඥ��ͼ����*/
void CskeletalanimationView::OnWindowOpen() 
{
	// TODO: Add your command handler code here
	ok = !ok;
	scaleMode = false;
		
	GLfloat recoverMol = Rscaling;	
	if( Rscaling != 1 )/*�жϷŴ����*/
	{
		glmScale( head, (float)1/recoverMol );
		glmScale( body, (float)1/recoverMol );
		glmScale( leftdatui, (float)1/recoverMol );
		glmScale( rightdatui, (float)1/recoverMol );
		glmScale( leftxiaotui, (float)1/recoverMol );
		glmScale( rightxiaotui, (float)1/recoverMol );
		glmScale( leftfoot, (float)1/recoverMol );
		glmScale( rightfoot, (float)1/recoverMol );
		glmScale( leftbi, (float)1/recoverMol );
		glmScale( rightbi, (float)1/recoverMol );
		glmScale( leftzhou, (float)1/recoverMol );
		glmScale( rightzhou, (float)1/recoverMol );
		glmScale( lefthand, (float)1/recoverMol );
		glmScale( righthand, (float)1/recoverMol );
	}
	Rscaling= 1;/*���Ŵ�ϵ������Ϊ1*/
		
	Invalidate();	/*ˢ��*/
}

/*�ඥ��ͼ�˵����²���*/
void CskeletalanimationView::OnUpdateWindowOpen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( ok );/*�ڲඥ��ͼ�˵�ǰ���ϱ��*/

	Invalidate();	
}


void CskeletalanimationView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	m_leftButtonDown = true;/*������±�־*/

	m_leftDownPos[0] = point.x;/*ȡ������ԭʼX����*/
	m_leftDownPos[1] = point.y;/*ȡ������ԭʼZ����*/

	CView::OnLButtonDown(nFlags, point);

	Invalidate();
}

/*�ӽǸ�ԭ*/
void CskeletalanimationView::OnControlRecover() 
{
	// TODO: Add your command handler code here
	rotate_up_down = 0;
	rotate_left_right = 0;
	up_down=0;
	left_right=0;
	scaleMode = false;
	m_xRotation = 0;
	m_yRotation = 0;

	/*��ģ�ͻ�ԭΪԭʼ��С*/
	glmScale(head,(float)1/Rscaling);
	glmScale(body,(float)1/Rscaling);
	glmScale(leftdatui,(float)1/Rscaling);
	glmScale( rightdatui,(float)1/Rscaling );
	glmScale( leftxiaotui,(float)1/Rscaling );
	glmScale(  rightxiaotui,(float)1/Rscaling );
	glmScale( leftfoot,(float)1/Rscaling );
	glmScale( rightfoot,(float)1/Rscaling );
	glmScale(leftbi,(float)1/Rscaling);
	glmScale( rightbi,(float)1/Rscaling );
	glmScale( leftzhou,(float)1/Rscaling );
	glmScale( rightzhou,(float)1/Rscaling );
    glmScale( lefthand,(float)1/Rscaling );
	glmScale( righthand,(float)1/Rscaling );

	Rscaling = 1;
	//scaleMol = 1;
	//render_select = false;

	Invalidate();
	
}

//ȫ����ԭ�ɳ�ʼ
void CskeletalanimationView::SRecover()
{
	OnControlRecover(); //�ص���ʼ�ӽ�

	pDialog->Hx = 0, pDialog->Hz = 0;
	pDialog->luax = 0, pDialog->luaz = 0;
	pDialog->ruax = 0, pDialog->ruaz = 0;
	pDialog->lfx = 0, pDialog->lfz = 0;
	pDialog->rfx = 0, pDialog->rfz = 0;
	pDialog->ltx = 0, pDialog->ltz = 0;
	pDialog->rtx = 0, pDialog->rtz = 0;
	pDialog->lcx = 0, pDialog->lcz = 0;
	pDialog->rcx = 0, pDialog->rcz = 0;

	pDialog->UpdateData(false);//���¿ؼ�

	hx = 0, hy = 0, hz = 0;
	luax = 0, luay = 0, luaz = 0;
	ruax = 0, ruay = 0, ruaz = 0;
	lfx = 0, lfy = 0, lfz = 0;
	rfx = 0, rfy = 0, rfz = 0;
	ltx = 0, lty = 0, ltz = 0;
	rtx = 0, rty = 0, rtz = 0;
	lcx = 0, lcy = 0, lcz = 0;
	rcx = 0, rcy = 0, rcz = 0;


	pDialog->CP1x.SetPos(hx);
	pDialog->CP1y.SetPos(hz);

	pDialog->CP2x.SetPos(luax);
	pDialog->CP2y.SetPos(luaz);

	pDialog->CP3x.SetPos(lfx);
	pDialog->CP3y.SetPos(lfz);

	pDialog->CP4x.SetPos(ruax);
	pDialog->CP4y.SetPos(ruaz);

	pDialog->CP5x.SetPos(rfx);
	pDialog->CP5y.SetPos(rfz);

	pDialog->CP6x.SetPos(ltx);
	pDialog->CP6y.SetPos(ltz);

	pDialog->CP7x.SetPos(lcx);
	pDialog->CP7y.SetPos(lcz);

	pDialog->CP8x.SetPos(rtx);
	pDialog->CP8y.SetPos(rtz);

	pDialog->CP9x.SetPos(rcx);
	pDialog->CP9y.SetPos(rcz);

	pDialog->DSpeed.SetPos(100);

	pDialog->mySpeed = 100;

	Invalidate(TRUE);

}

LRESULT CskeletalanimationView::OnClear(WPARAM wP, LPARAM lP)
{
	SRecover();
	return TRUE;
}

#ifdef _DEBUG
void CskeletalanimationView::AssertValid() const
{
	CView::AssertValid();
}
void CskeletalanimationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
CskeletalanimationDoc* CskeletalanimationView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CskeletalanimationDoc)));
	return (CskeletalanimationDoc*)m_pDocument;
}
#endif //_DEBUG




/*���ſ��ƺ���*/
void CskeletalanimationView::OnControlPlay() 
{
	// TODO: Add your command handler code here
	CskeletalanimationDoc* pDoc=GetDocument();
	int t_frame;
	/*pDoc->anime = new framenode[maxframe];*/
	pDialog->m_Radio=-1;
	certain = pDoc->framelist.head->next;
	previous = pDoc->framelist.head;
	
	m_play = !m_play;	
		
	if( m_play )/*�ж��Ƿ���������״̬*/
	{
		for(t_frame = 1;t_frame<=maxframe;t_frame++)
		{
			framenode *NewNode = new framenode;
			NewNode->m_frameno = t_frame;

			NewNode->Hx = previous->Hx + (t_frame-previous->m_frameno)*(certain->Hx - previous->Hx)/(certain->m_frameno - previous->m_frameno);
			NewNode->Hy = previous->Hy + (t_frame-previous->m_frameno)*(certain->Hy - previous->Hy)/(certain->m_frameno - previous->m_frameno);
			NewNode->Hz = previous->Hz + (t_frame-previous->m_frameno)*(certain->Hz - previous->Hz)/(certain->m_frameno - previous->m_frameno);

			NewNode->lcx = previous->lcx + (t_frame-previous->m_frameno)*(certain->lcx - previous->lcx)/(certain->m_frameno - previous->m_frameno);
			NewNode->lcy = previous->lcy + (t_frame-previous->m_frameno)*(certain->lcy - previous->lcy)/(certain->m_frameno - previous->m_frameno);
			NewNode->lcz = previous->lcz + (t_frame-previous->m_frameno)*(certain->lcz - previous->lcz)/(certain->m_frameno - previous->m_frameno);

			NewNode->lfx = previous->lfx + (t_frame-previous->m_frameno)*(certain->lfx - previous->lfx)/(certain->m_frameno - previous->m_frameno);
			NewNode->lfy = previous->lfy + (t_frame-previous->m_frameno)*(certain->lfy - previous->lfy)/(certain->m_frameno - previous->m_frameno);
			NewNode->lfz = previous->lfz + (t_frame-previous->m_frameno)*(certain->lfz - previous->lfz)/(certain->m_frameno - previous->m_frameno);

			NewNode->ltx = previous->ltx + (t_frame-previous->m_frameno)*(certain->ltx - previous->ltx)/(certain->m_frameno - previous->m_frameno);
			NewNode->lty = previous->lty + (t_frame-previous->m_frameno)*(certain->lty - previous->lty)/(certain->m_frameno - previous->m_frameno);
			NewNode->ltz = previous->ltz + (t_frame-previous->m_frameno)*(certain->ltz - previous->ltz)/(certain->m_frameno - previous->m_frameno);

			NewNode->luax = previous->luax + (t_frame-previous->m_frameno)*(certain->luax - previous->luax)/(certain->m_frameno - previous->m_frameno);
			NewNode->luay = previous->luay + (t_frame-previous->m_frameno)*(certain->luay - previous->luay)/(certain->m_frameno - previous->m_frameno);
			NewNode->luaz = previous->luaz + (t_frame-previous->m_frameno)*(certain->luaz - previous->luaz)/(certain->m_frameno - previous->m_frameno);

			NewNode->rcx = previous->rcx + (t_frame-previous->m_frameno)*(certain->rcx - previous->rcx)/(certain->m_frameno - previous->m_frameno);
			NewNode->rcy = previous->rcy + (t_frame-previous->m_frameno)*(certain->rcy - previous->rcy)/(certain->m_frameno - previous->m_frameno);
			NewNode->rcz = previous->rcz + (t_frame-previous->m_frameno)*(certain->rcz - previous->rcz)/(certain->m_frameno - previous->m_frameno);

			NewNode->rfx = previous->rfx + (t_frame-previous->m_frameno)*(certain->rfx - previous->rfx)/(certain->m_frameno - previous->m_frameno);
			NewNode->rfy = previous->rfy + (t_frame-previous->m_frameno)*(certain->rfy - previous->rfy)/(certain->m_frameno - previous->m_frameno);
			NewNode->rfz = previous->rfz + (t_frame-previous->m_frameno)*(certain->rfz - previous->rfz)/(certain->m_frameno - previous->m_frameno);

			NewNode->rtx = previous->rtx + (t_frame-previous->m_frameno)*(certain->rtx - previous->rtx)/(certain->m_frameno - previous->m_frameno);
			NewNode->rty = previous->rty + (t_frame-previous->m_frameno)*(certain->rty - previous->rty)/(certain->m_frameno - previous->m_frameno);
			NewNode->rtz = previous->rtz + (t_frame-previous->m_frameno)*(certain->rtz - previous->rtz)/(certain->m_frameno - previous->m_frameno);

			NewNode->ruax = previous->ruax + (t_frame-previous->m_frameno)*(certain->ruax - previous->ruax)/(certain->m_frameno - previous->m_frameno);
			NewNode->ruay = previous->ruay + (t_frame-previous->m_frameno)*(certain->ruay - previous->ruay)/(certain->m_frameno - previous->m_frameno);
			NewNode->ruaz = previous->ruaz + (t_frame-previous->m_frameno)*(certain->ruaz - previous->ruaz)/(certain->m_frameno - previous->m_frameno);

			pDoc->anime.add(NewNode);

			if(t_frame == certain->m_frameno)
			{
				previous = previous->next;
				certain = certain->next;
			}
		}
		listhead = pDoc->anime.head;
		certain = listhead;
		SetTimer( 1, frameTime, NULL );/*�ǣ���ʼ�Զ���ת*/
		pDialog->ShowWindow(SW_HIDE);
	}
	else
	{
		pDoc->anime.end = pDoc->anime.head;
		for(t_frame = 1;t_frame<=maxframe;t_frame++)
		{
			pDoc->anime.Delete(t_frame);
		}
		KillTimer( 1 );/*����رղ���״̬*/
		pDialog->ShowWindow(SW_SHOW);
	}
}

/*��ʱ����Ӧ����*/
void CskeletalanimationView::OnTimer(UINT nIDEvent) 
{
	CskeletalanimationDoc* pDoc=GetDocument();
	// TODO: Add your message handler code here and/or call default
	switch( nIDEvent )
	{
	case 0:
		break;
	case 1:/*�ǲ���״̬ʱ*/
		hx=certain->Hx;
		hy=certain->Hy;
		hz=certain->Hz;

		luax=certain->luax;
		luay=certain->luay;
		luaz=certain->luaz;

		lfx=certain->lfx;
		lfy=certain->lfy;
		lfz=certain->lfz;

		ruax=certain->ruax;
		ruay=certain->ruay;
		ruaz=certain->ruaz;

		rfx=certain->rfx;
		rfy=certain->rfy;
		rfz=certain->rfz;

		ltx=certain->ltx;
		lty=certain->lty;
		ltz=certain->ltz;

		lcx=certain->lcx;
		lcy=certain->lcy;
		lcz=certain->lcz;

		rtx=certain->rtx;
		rty=certain->rty;
		rtz=certain->rtz;

		rcx=certain->rcx;
		rcy=certain->rcy;
		rcz=certain->rcz;

		certain = certain->next;
		if(certain == NULL)
			certain = listhead;
		Invalidate();
		break;
	default:
		{}
	}
	CView::OnTimer(nIDEvent);
}

/*����״̬�˵��Ͱ�ť���²�������*/
void CskeletalanimationView::OnUpdateControlPlay(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_play );/*���ò˵��Ͱ�ť״̬*/	
}

/*�Ŵ�˵�����*/
void CskeletalanimationView::OnUpdateControlScale(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( scaleMode );
	
}

void CskeletalanimationView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if (CBDown == true && m_leftButtonDown == true)
	{
		m_xRotation -= (float)(m_leftDownPos[0] - point.x) / 3.0f;
		m_yRotation += (float)(m_leftDownPos[1] - point.y) / 3.0f;
		m_leftDownPos[0] = point.x;
		m_leftDownPos[1] = point.y;
		Invalidate();
	}

	if (m_leftButtonDown )
	{
		switch (pDialog->m_Radio)
		{
		case -1:
				m_xRotation -= (float)(m_leftDownPos[0] - point.x) / 3.0f;
				m_yRotation += (float)(m_leftDownPos[1] - point.y) / 3.0f;
				m_leftDownPos[0] = point.x;
				m_leftDownPos[1] = point.y;
				Invalidate();
				break;
		case 0:
				hz -= (float)(m_leftDownPos[0] - point.x) / 3.0f;
				hx += (float)(m_leftDownPos[1] - point.y) / 3.0f;
				m_leftDownPos[0] = point.x;
				m_leftDownPos[1] = point.y;
				hz = check(hz);
				hx = check(hx);
				pDialog->CP1x.SetPos(hx);
				pDialog->CP1y.SetPos(hz);
				pDialog->Hz = hz;
				pDialog->Hx = hx;
				pDialog->UpdateData(false);//���¿ؼ�
				Invalidate();
				break;
		case 1:
				luaz -= (float)(m_leftDownPos[0] - point.x) / 3.0f;
				luax += (float)(m_leftDownPos[1] - point.y) / 3.0f;
				m_leftDownPos[0] = point.x;
				m_leftDownPos[1] = point.y;
				luaz = check(luaz);
				luax = check(luax);
				pDialog->CP2x.SetPos(luax);
				pDialog->CP2y.SetPos(luaz);
				pDialog->luaz = luaz;
				pDialog->luax = luax;
				pDialog->UpdateData(false);//���¿ؼ�
				Invalidate();
				break;
		case 2:
				lfz -= (float)(m_leftDownPos[0] - point.x) / 3.0f;
				lfx += (float)(m_leftDownPos[1] - point.y) / 3.0f;
				m_leftDownPos[0] = point.x;
				m_leftDownPos[1] = point.y;
				lfz = check(lfz);
				lfx = check(lfx);
				pDialog->CP3x.SetPos(lfx);
				pDialog->CP3y.SetPos(lfz);
				pDialog->lfz = lfz;
				pDialog->lfx = lfx;
				pDialog->UpdateData(false);//���¿ؼ�
				Invalidate();
				break;
		case 3:
				ruaz -= (float)(m_leftDownPos[0] - point.x) / 3.0f;
				ruax += (float)(m_leftDownPos[1] - point.y) / 3.0f;
				m_leftDownPos[0] = point.x;
				m_leftDownPos[1] = point.y;
				ruaz = check(ruaz);
				ruax = check(ruax);
				pDialog->CP4x.SetPos(ruax);
				pDialog->CP4y.SetPos(ruaz);
				pDialog->ruaz = ruaz;
				pDialog->ruax = ruax;
				pDialog->UpdateData(false);//���¿ؼ�
				Invalidate();
				break;
		case 4:
				rfz -= (float)(m_leftDownPos[0] - point.x) / 3.0f;
				rfx += (float)(m_leftDownPos[1] - point.y) / 3.0f;
				m_leftDownPos[0] = point.x;
				m_leftDownPos[1] = point.y;
				rfz = check(rfz);
				rfx = check(rfx);
				pDialog->CP5x.SetPos(rfx);
				pDialog->CP5y.SetPos(rfz);
				pDialog->rfz = rfz;
				pDialog->rfx = rfx;
				pDialog->UpdateData(false);//���¿ؼ�
				Invalidate();
				break;
		case 5:
				ltz -= (float)(m_leftDownPos[0] - point.x) / 3.0f;
				ltx += (float)(m_leftDownPos[1] - point.y) / 3.0f;
				m_leftDownPos[0] = point.x;
				m_leftDownPos[1] = point.y;
				ltz = check(ltz);
				ltx = check(ltx);
				pDialog->CP6x.SetPos(ltx);
				pDialog->CP6y.SetPos(ltz);
				pDialog->ltz = ltz;
				pDialog->ltx = ltx;
				pDialog->UpdateData(false);//���¿ؼ�
				Invalidate();
				break;
		case 6:
				lcz -= (float)(m_leftDownPos[0] - point.x) / 3.0f;
				lcx += (float)(m_leftDownPos[1] - point.y) / 3.0f;
				m_leftDownPos[0] = point.x;
				m_leftDownPos[1] = point.y;
				lcz = check(lcz);
				lcx = check(lcx);
				pDialog->CP7x.SetPos(lcx);
				pDialog->CP7y.SetPos(lcz);
				pDialog->lcz = lcz;
				pDialog->lcx = lcx;
				pDialog->UpdateData(false);//���¿ؼ�
				Invalidate();
				break;
		case 7:
				rtz -= (float)(m_leftDownPos[0] - point.x) / 3.0f;
				rtx += (float)(m_leftDownPos[1] - point.y) / 3.0f;
				m_leftDownPos[0] = point.x;
				m_leftDownPos[1] = point.y;
				rtz = check(rtz);
				rtx = check(rtx);
				pDialog->CP8x.SetPos(rtx);
				pDialog->CP8y.SetPos(rtz);
				pDialog->rtz = rtz;
				pDialog->rtx = rtx;
				pDialog->UpdateData(false);//���¿ؼ�
				Invalidate();
				break;
		case 8:
				rcz -= (float)(m_leftDownPos[0] - point.x) / 3.0f;
				rcx += (float)(m_leftDownPos[1] - point.y) / 3.0f;
				m_leftDownPos[0] = point.x;
				m_leftDownPos[1] = point.y;
				rcz = check(rcz);
				rcx = check(rcx);
				pDialog->CP9x.SetPos(rcx);
				pDialog->CP9y.SetPos(rcz);
				pDialog->rcz = rcz;
				pDialog->rcx = rcx;
				pDialog->UpdateData(false);//���¿ؼ�
				Invalidate();
				break;
		default:break;
		}
	}
	CView::OnMouseMove(nFlags, point);
}

void CskeletalanimationView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_leftButtonDown = false;
	m_leftDownPos[0] = 0;
	m_leftDownPos[1] = 0;

	CView::OnLButtonUp(nFlags, point);
}

void CskeletalanimationView::OnGesture()
{
	// TODO: �ڴ���������������
	pDialog->ShowWindow(SW_HIDE);						//���ضԻ���

	//��ȡ��ǰ�����������Ի���Ĳ�����
	pDialog->Hx=hx,pDialog->Hz=hz;
	pDialog->luax=luax,pDialog->luaz=luaz;
	pDialog->ruax=ruax,pDialog->ruaz=ruaz;
	pDialog->lfx=lfx,pDialog->lfz=lfz;
	pDialog->rfx=rfx,pDialog->rfz=rfz;
	pDialog->ltx=ltx,pDialog->ltz=ltz;
	pDialog->rtx=rtx,pDialog->rtz=rtz;
	pDialog->lcx=lcx,pDialog->lcz=lcz;
	pDialog->rcx=rcx,pDialog->rcz=rcz;

	pDialog->UpdateData(false);//���¿ؼ�
	pDialog->ShowWindow(SW_SHOW);						//��ʾ�Ի���
	::SetWindowPos( pDialog->m_hWnd , HWND_TOPMOST , NULL , NULL , NULL , NULL , SWP_NOSIZE | SWP_NOMOVE );//
}

LRESULT CskeletalanimationView::OnSetGesture(WPARAM wP,LPARAM lP)
{
	CskeletalanimationDoc* pDoc=GetDocument();//��ȡdocָ��
	framenode *NewNode = new framenode;
	NewNode->m_frameno = pDialog->frame;
	NewNode->is_key_frame = true;

	if(maxframe < pDialog->frame)
		maxframe = pDialog->frame;

	NewNode->Hx = hx = pDialog->Hx;
	NewNode->Hz = hz = pDialog->Hz;
	
	NewNode->luax = luax = pDialog->luax;
	NewNode->luaz = luaz = pDialog->luaz;

	NewNode->lfx = lfx = pDialog->lfx;
	NewNode->lfz = lfz = pDialog->lfz;

	NewNode->ruax = ruax = pDialog->ruax;
	NewNode->ruaz = ruaz = pDialog->ruaz;

	NewNode->rfx = rfx = pDialog->rfx;
	NewNode->rfz = rfz = pDialog->rfz;

	NewNode->ltx = ltx = pDialog->ltx;
	NewNode->ltz = ltz = pDialog->ltz;

	NewNode->lcx = lcx = pDialog->lcx;
	NewNode->lcz = lcz = pDialog->lcz;

	NewNode->rtx = rtx = pDialog->rtx;
	NewNode->rtz = rtz = pDialog->rtz;

	NewNode->rcx = rcx = pDialog->rcx;
	NewNode->rcz = rcz = pDialog->rcz;

	pDoc->framelist.insert(NewNode);
	pDoc->framelist.totalframe = maxframe;

	pDialog->m_color = pDialog->m_red;
	pDialog->m_textcolor = pDialog->m_black;

	pDialog->CP1x.SetPos(hx);
	pDialog->CP1y.SetPos(hz);

	pDialog->CP2x.SetPos(luax);
	pDialog->CP2y.SetPos(luaz);

	pDialog->CP3x.SetPos(lfx);
	pDialog->CP3y.SetPos(lfz);

	pDialog->CP4x.SetPos(ruax);
	pDialog->CP4y.SetPos(ruaz);

	pDialog->CP5x.SetPos(rfx);
	pDialog->CP5y.SetPos(rfz);

	pDialog->CP6x.SetPos(ltx);
	pDialog->CP6y.SetPos(ltz);

	pDialog->CP7x.SetPos(lcx);
	pDialog->CP7y.SetPos(lcz);

	pDialog->CP8x.SetPos(rtx);
	pDialog->CP8y.SetPos(rtz);

	pDialog->CP9x.SetPos(rcx);
	pDialog->CP9y.SetPos(rcz);

	UpdateData(false);
	Invalidate(TRUE);
	
	return TRUE;
}

LRESULT CskeletalanimationView::OnGestureChange(WPARAM wP, LPARAM lP)
{

	 hx = pDialog->Hx;
	 hz = pDialog->Hz;

	 luax = pDialog->luax;
	 luaz = pDialog->luaz;

	 lfx = pDialog->lfx;
	 lfz = pDialog->lfz;

	 ruax = pDialog->ruax;
	 ruaz = pDialog->ruaz;

	 rfx = pDialog->rfx;
	 rfz = pDialog->rfz;

	 ltx = pDialog->ltx;
	 ltz = pDialog->ltz;

	 lcx = pDialog->lcx;
	 lcz = pDialog->lcz;

	 rtx = pDialog->rtx;
	 rtz = pDialog->rtz;

	 rcx = pDialog->rcx;
	 rcz = pDialog->rcz;

	Invalidate(TRUE);
	return TRUE;
}

BOOL CskeletalanimationView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//��ʵ�������Ϊ���ĵķŴ��ƶ�

	if (CBDown==true )
	{
		if (zDelta == 120)
			scaling = 1.1;
		else if (zDelta == -120)
			scaling = 0.94;

		Rscaling *= scaling;

		glmScale(head, scaling);
		glmScale(body, scaling);
		glmScale(leftdatui, scaling);
		glmScale(rightdatui, scaling);
		glmScale(leftxiaotui, scaling);
		glmScale(rightxiaotui, scaling);
		glmScale(leftfoot, scaling);
		glmScale(rightfoot, scaling);
		glmScale(leftbi, scaling);
		glmScale(rightbi, scaling);
		glmScale(leftzhou, scaling);
		glmScale(rightzhou, scaling);
		glmScale(lefthand, scaling);
		glmScale(righthand, scaling);

		Invalidate();
	}
	else if (pDialog->GControl == true && CBDown == false)
	{
		Pshift = pDialog->m_Radio;
		if (zDelta == 120)
		{
			if (Pshift == 0)
				Pshift = 8;
			else
				Pshift--;
		}
		else if (zDelta == -120)
		{
			if (Pshift == 8)
				Pshift = 0;
			else
				Pshift++;
		}

		switch (Pshift)
		{
		case 0:
			pDialog->OnBnClickedRadio1();
			break;
		case 1:
			pDialog->OnBnClickedRadio2();
			break;
		case 2:
			pDialog->OnBnClickedRadio3();
			break;
		case 3:
			pDialog->OnBnClickedRadio4();
			break;
		case 4:
			pDialog->OnBnClickedRadio5();
			break;
		case 5:
			pDialog->OnBnClickedRadio6();
			break;
		case 6:
			pDialog->OnBnClickedRadio7();
			break;
		case 7:
			pDialog->OnBnClickedRadio8();
			break;
		case 8:
			pDialog->OnBnClickedRadio9();
			break;
		}
		pDialog->m_Radio = Pshift;
		pDialog->UpdateData(false);
	}


	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

LRESULT CskeletalanimationView::OnPreviousFrame(WPARAM wP,LPARAM lP)
{
	CskeletalanimationDoc* pDoc=GetDocument();

	int frameNow = pDialog->frame;

	frameNow--;

	if (frameNow == 0)
	{
		AfxMessageBox(L"�Ѿ�����һ֡��", MB_OK | MB_SYSTEMMODAL);
		return TRUE;
	}

	//frame--;
	//if(frame<0){
	//	frame = 0;
	//	pDialog->frame=0;
	//}

	framenode WantToFind = pDoc->framelist.find(frameNow);

	pDialog->Hx = hx = WantToFind.Hx;
	pDialog->Hz = hz = WantToFind.Hz;

	pDialog->luax=luax=WantToFind.luax;
	pDialog->luaz=luaz=WantToFind.luaz;

	pDialog->lfx=lfx=WantToFind.lfx;
	pDialog->lfz=lfz=WantToFind.lfz;

	pDialog->ruax=ruax=WantToFind.ruax;
	pDialog->ruaz=ruaz=WantToFind.ruaz;

	pDialog->rfx=rfx=WantToFind.rfx;
	pDialog->rfz=rfz=WantToFind.rfz;

	pDialog->ltx=ltx=WantToFind.ltx;
	pDialog->ltz=ltz=WantToFind.ltz;

	pDialog->lcx=lcx=WantToFind.lcx;
	pDialog->lcz=lcz=WantToFind.lcz;

	pDialog->rtx=rtx=WantToFind.rtx;
	pDialog->rtz=rtz=WantToFind.rtz;

	pDialog->rcx=rcx=WantToFind.rcx;
	pDialog->rcz=rcz=WantToFind.rcz;

	//pDialog->CPupdate();

	pDialog->CP1x.SetPos(hx);
	pDialog->CP1y.SetPos(hz);

	pDialog->CP2x.SetPos(luax);
	pDialog->CP2y.SetPos(luaz);

	pDialog->CP3x.SetPos(lfx);
	pDialog->CP3y.SetPos(lfz);

	pDialog->CP4x.SetPos(ruax);
	pDialog->CP4y.SetPos(ruaz);

	pDialog->CP5x.SetPos(rfx);
	pDialog->CP5y.SetPos(rfz);

	pDialog->CP6x.SetPos(ltx);
	pDialog->CP6y.SetPos(ltz);

	pDialog->CP7x.SetPos(lcx);
	pDialog->CP7y.SetPos(lcz);

	pDialog->CP8x.SetPos(rtx);
	pDialog->CP8y.SetPos(rtz);

	pDialog->CP9x.SetPos(rcx);
	pDialog->CP9y.SetPos(rcz);

	pDialog->frame = frameNow;

	if(WantToFind.is_key_frame)
	{
		pDialog->m_color = pDialog->m_red;
		pDialog->m_textcolor = pDialog->m_black;
	//	pDialog->m_brush = pDialog->m_red;
	}
	else
	{
		pDialog->m_color = pDialog->m_white;
		pDialog->m_textcolor = pDialog->m_black;
	}

	pDialog->UpdateData(false);//���¿ؼ�
	Invalidate(TRUE);
	return true;
}

LRESULT CskeletalanimationView::OnNextFrame(WPARAM wP,LPARAM lP)
{
	CskeletalanimationDoc* pDoc=GetDocument();

	int frameNow = pDialog->frame;

	frameNow++;

	if (frameNow> maxframe)
	{
		AfxMessageBox(L"�Ѿ������һ֡��", MB_OK | MB_SYSTEMMODAL);
		return TRUE;
	}

	//frame++;

	framenode WantToFind = pDoc->framelist.find(frameNow);

	pDialog->Hx = hx = WantToFind.Hx;
	pDialog->Hz = hz = WantToFind.Hz;

	pDialog->luax=luax=WantToFind.luax;
	pDialog->luaz=luaz=WantToFind.luaz;

	pDialog->lfx=lfx=WantToFind.lfx;
	pDialog->lfz=lfz=WantToFind.lfz;

	pDialog->ruax=ruax=WantToFind.ruax;
	pDialog->ruaz=ruaz=WantToFind.ruaz;

	pDialog->rfx=rfx=WantToFind.rfx;
	pDialog->rfz=rfz=WantToFind.rfz;

	pDialog->ltx=ltx=WantToFind.ltx;
	pDialog->ltz=ltz=WantToFind.ltz;

	pDialog->lcx=lcx=WantToFind.lcx;
	pDialog->lcz=lcz=WantToFind.lcz;

	pDialog->rtx=rtx=WantToFind.rtx;
	pDialog->rtz=rtz=WantToFind.rtz;

	pDialog->rcx=rcx=WantToFind.rcx;
	pDialog->rcz=rcz=WantToFind.rcz;

	//pDialog->CPupdate();


	pDialog->CP1x.SetPos(hx);
	pDialog->CP1y.SetPos(hz);

	pDialog->CP2x.SetPos(luax);
	pDialog->CP2y.SetPos(luaz);

	pDialog->CP3x.SetPos(lfx);
	pDialog->CP3y.SetPos(lfz);

	pDialog->CP4x.SetPos(ruax);
	pDialog->CP4y.SetPos(ruaz);

	pDialog->CP5x.SetPos(rfx);
	pDialog->CP5y.SetPos(rfz);

	pDialog->CP6x.SetPos(ltx);
	pDialog->CP6y.SetPos(ltz);

	pDialog->CP7x.SetPos(lcx);
	pDialog->CP7y.SetPos(lcz);

	pDialog->CP8x.SetPos(rtx);
	pDialog->CP8y.SetPos(rtz);

	pDialog->CP9x.SetPos(rcx);
	pDialog->CP9y.SetPos(rcz);

	pDialog->frame = frameNow;

	//CString str;
	//str.Format(L"%d",(int)lfz);
	//AfxMessageBox(str);


	if(WantToFind.is_key_frame)
	{
		pDialog->m_color = pDialog->m_red;
		pDialog->m_textcolor = pDialog->m_black;
	//	pDialog->m_brush = pDialog->m_red;
	}
	else
	{
		pDialog->m_color = pDialog->m_white;
		pDialog->m_textcolor = pDialog->m_black;
	}

	pDialog->UpdateData(false);//���¿ؼ�
	Invalidate(TRUE);
	return true;
}

LRESULT CskeletalanimationView::OnGo(WPARAM wP,LPARAM lP)
{
	CskeletalanimationDoc* pDoc=GetDocument();

	if (pDialog->frame > maxframe || pDialog->frame < 1)
	{
		AfxMessageBox(L"�޴˹ؼ�֡��", MB_OK | MB_SYSTEMMODAL);
		return TRUE;
	}

	frame = pDialog->frame;
	framenode WantToFind = pDoc->framelist.find(frame);

	pDialog->Hx = hx = WantToFind.Hx;
	pDialog->Hz = hz = WantToFind.Hz;

	pDialog->luax=luax=WantToFind.luax;
	pDialog->luaz=luaz=WantToFind.luaz;

	pDialog->lfx=lfx=WantToFind.lfx;
	pDialog->lfz=lfz=WantToFind.lfz;

	pDialog->ruax=ruax=WantToFind.ruax;
	pDialog->ruaz=ruaz=WantToFind.ruaz;

	pDialog->rfx=rfx=WantToFind.rfx;
	pDialog->rfz=rfz=WantToFind.rfz;

	pDialog->ltx=ltx=WantToFind.ltx;
	pDialog->ltz=ltz=WantToFind.ltz;

	pDialog->lcx=lcx=WantToFind.lcx;
	pDialog->lcz=lcz=WantToFind.lcz;

	pDialog->rtx=rtx=WantToFind.rtx;
	pDialog->rtz=rtz=WantToFind.rtz;

	pDialog->rcx=rcx=WantToFind.rcx;
	pDialog->rcz=rcz=WantToFind.rcz;
	
	//pDialog->CPupdate();

	pDialog->CP1x.SetPos(hx);
	pDialog->CP1y.SetPos(hz);

	pDialog->CP2x.SetPos(luax);
	pDialog->CP2y.SetPos(luaz);

	pDialog->CP3x.SetPos(lfx);
	pDialog->CP3y.SetPos(lfz);

	pDialog->CP4x.SetPos(ruax);
	pDialog->CP4y.SetPos(ruaz);

	pDialog->CP5x.SetPos(rfx);
	pDialog->CP5y.SetPos(rfz);

	pDialog->CP6x.SetPos(ltx);
	pDialog->CP6y.SetPos(ltz);

	pDialog->CP7x.SetPos(lcx);
	pDialog->CP7y.SetPos(lcz);

	pDialog->CP8x.SetPos(rtx);
	pDialog->CP8y.SetPos(rtz);

	pDialog->CP9x.SetPos(rcx);
	pDialog->CP9y.SetPos(rcz);

	if(WantToFind.is_key_frame)
	{
		pDialog->m_color = pDialog->m_red;
		pDialog->m_textcolor = pDialog->m_black;
	//	pDialog->m_brush = pDialog->m_red;
	}
	else
	{
		pDialog->m_color = pDialog->m_white;
		pDialog->m_textcolor = pDialog->m_black;
	}



	pDialog->UpdateData(false);//���¿ؼ�
	Invalidate(TRUE);

	//CString str;
	//str.Format(L"%d", (int)rcz);
	//AfxMessageBox(str, MB_OK | MB_SYSTEMMODAL);

	return true;
}

LRESULT CskeletalanimationView::OnOK(WPARAM wP,LPARAM lP)
{
	CskeletalanimationDoc* pDoc=GetDocument();//��ȡdocָ��

	hx = pDialog->Hx;
	hz = pDialog->Hz;
	
	luax = pDialog->luax;
	luaz = pDialog->luaz;

	lfx = pDialog->lfx;
	lfz = pDialog->lfz;

	ruax = pDialog->ruax;
	ruaz = pDialog->ruaz;

	rfx = pDialog->rfx;
	rfz = pDialog->rfz;

	ltx = pDialog->ltx;
	ltz = pDialog->ltz;

	lcx = pDialog->lcx;
	lcz = pDialog->lcz;

	rtx = pDialog->rtx;
	rtz = pDialog->rtz;


	rcx = pDialog->rcx;
	rcz = pDialog->rcz;

	Invalidate(TRUE);
	
	return TRUE;
}

LRESULT CskeletalanimationView::OnDelete(WPARAM wP,LPARAM lP)
{
	CskeletalanimationDoc* pDoc=GetDocument();//��ȡdocָ��

	int frameNow = pDialog->frame;

	if (pDialog->frame != 1 )
	{
		OnPreviousFrame(wP,lP);
	}

	pDoc->framelist.Delete(frameNow);

	pDialog->UpdateData(false);

	Invalidate(TRUE);
	
	return TRUE;
}


/*����м�������Ӧ�ӽǸ�ԭ����*/
void CskeletalanimationView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (pDialog->GControl == false)
	{
		OnGesture();
		pDialog->GControl = true;
	}
	else
	{
		pDialog->OnBnClicked2();
		pDialog->GControl = false;
	}
	CView::OnMButtonDown(nFlags, point);
}


void CskeletalanimationView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nChar)
	{
	case VK_UP:
		Mup = true;
		break;
	case VK_DOWN:
		Mdown = true;
		break;
	case VK_LEFT:
		Mleft = true;
		break;
	case VK_RIGHT:
		Mright = true;
		break;
	case VK_ESCAPE:
		SRecover();
	case VK_SPACE:
		OnControlRecover();
		break;
	case VK_TAB:
		OnWindowOpen();
		break;
	case VK_CONTROL:
		CBDown = true;
		break;
	case VK_END:
		if (pDialog->GControl == false)
		{
			OnGesture();
			pDialog->GControl = true;
		}
		else
		{
			pDialog->OnBnClicked2();
			pDialog->GControl = false;
		}
		break;
	default:break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CskeletalanimationView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nChar == VK_CONTROL)
		CBDown = false;

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

float CskeletalanimationView::check(float xy)
{
	if (xy >= 360)
		xy -= 360;
	if (xy <= -360)
		xy += 360;
	return xy;
}

LRESULT CskeletalanimationView::OnSetBGC(WPARAM wP, LPARAM lP)
{
	RGBA = pDialog->cR;
	RGBB = pDialog->cG;
	RGBC = pDialog->cB;
	Invalidate(TRUE);
	return TRUE;
}

LRESULT CskeletalanimationView::OnSpeedChange(WPARAM wP, LPARAM lP)
{
	frameTime = pDialog->mySpeed;
	Invalidate(TRUE);
	return TRUE;
}
