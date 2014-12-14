
// skeletal animationView.h : CskeletalanimationView ��Ľӿ�
//
#include <gl.h>
#include <glu.h>
#include <glaux.h>
#include "windows.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "glm.h"
#include "framenode.h"
#include "Gesture.h"
#include <mmsystem.h>


#define T(x) (model->triangles[(x)])
#define fsin(x) (float)sin(x)
#define fcos(x) (float)cos(x)
#define fasin(x) (float)asin(x)
#define facos(x) (float)acos(x)
#define fsqrt(x) (float)sqrt(x)

#pragma once


class CskeletalanimationView : public CView
{
protected: // create from serialization only
	CskeletalanimationView();
	DECLARE_DYNCREATE(CskeletalanimationView)

// Attributes
public:
	CskeletalanimationDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
private:
	bool m_leftButtonDown;
	int spin;/*��̨��ת����*/
	BOOL m_play;/*�Ƿ񲥷�*/
	bool render_select;/*����ѡ����ʰȡ����*/
	GLuint select_buf[512];/*ѡ����ʰȡ���ػ���*/
	void displayTop( GLenum mode);/*��ʾ����ͼ*/
	void displayBeside( GLenum mode);/*��ʾ����ͼ*/
	void displayFront(GLenum mode);/*��ʾǰ��ͼ*/
	void SRecover();
	void BGM();
	
	GLfloat light_position[4] ;
    GLMmodel *head;/*ͷģ��ָ�����*/
    GLMmodel *body;/*����ģ��ָ�����*/
    GLMmodel *leftdatui;/*����ģ��ָ�����*/
	GLMmodel *rightdatui;/*����ģ��ָ�����*/
	GLMmodel *leftxiaotui;/*��С��ģ��ָ�����*/
	GLMmodel *rightxiaotui;/*��С��ģ��ָ�����*/
	GLMmodel *leftfoot;/*���ģ��ָ�����*/
	GLMmodel *rightfoot;/*�ҽ�ģ��ָ�����*/
	GLMmodel *leftbi;/*���ģ��ָ�����*/
	GLMmodel *rightbi;/*�ұ�ģ��ָ�����*/
	GLMmodel *leftzhou;/*����ģ��ָ�����*/
	GLMmodel *rightzhou;/*����ģ��ָ�����*/
	GLMmodel *righthand;/*����ģ��ָ�����*/
	GLMmodel *lefthand;/*����ģ��ָ�����*/

	//����ɫ
	GLfloat RGBA;
	GLfloat RGBB;
	GLfloat RGBC;


	int h;/*�ͻ����Ŀ��*/
	int w;/*�ͻ����ĳ���*/

	int frameTime = 100;

	int Pshift=0; //shift��ݼ��л��޸ĵ�λ��

	//�����ƶ��ź�
	bool Mup=false;
	bool Mdown = false;
	bool Mleft = false;
	bool Mright = false;

	bool CBDown = false;//��¼ctrl���İ���

	float check(float xy); //���Ʋ�����-360~360��

	Gesture* pDialog;//�Ի���

	void display();/*��ʾ����*/
	void myinit();/*��ʼ������*/
	bool CreateOpenGLContext();/*������ͼ������*/
	bool SetupPixelFormat();/*�������ظ�ʽ*/
	HGLRC m_hRC;/*��ͼ��������*/
	HDC m_hDC;/*�豸��������*/
	virtual ~CskeletalanimationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:	
	long m_leftDownPos[2];//���ԭʼ�������
	float m_xRotation;//X������ת�Ƕ�
	float m_yRotation;//Z������ת�Ƕ�
	GLfloat z_Pos=-2.0;
	GLfloat scaleMolNow;/*�Ŵ�ϵ��*/
	GLint scaleMol;/*�Ŵ����*/
	GLfloat left_right;/*����ƽ�ƿ���*/
	GLfloat up_down;/*����ƽ�ƿ���*/

	GLfloat scaling;  // ���ι������ű���
	GLfloat Rscaling;   //��¼�ܷŴ���


    bool scaleMode;/*����Ŵ�ģʽ���Ʋ���*/
	bool ok;/*����ඥ��ͼ���Ʋ���*/

	GLfloat rotate_left_right;/*������ת����*/
	GLfloat rotate_up_down;/*������ת����*/
	
	float hx,hy,hz;
	float luax,luay,luaz;
	float ruax,ruay,ruaz;
	float lfx,lfy,lfz;
	float rfx,rfy,rfz;
	float ltx,lty,ltz;
	float rtx,rty,rtz;
	float lcx,lcy,lcz;
	float rcx,rcy,rcz;

	//{{AFX_MSG(CMyView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHelpOperation();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnWindowOpen();
	afx_msg void OnUpdateWindowOpen(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnControlClosetb();
	afx_msg void OnControlOpentb();
	afx_msg void OnControlRecover();
	afx_msg void OnControlPlay();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateControlPlay(CCmdUI* pCmdUI);
	afx_msg void OnHelpNumber();
	afx_msg void OnUpdateControlScale(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnSetGesture(WPARAM wP,LPARAM lP);
	afx_msg LRESULT OnGestureChange(WPARAM wP, LPARAM lP);
	afx_msg LRESULT OnPreviousFrame(WPARAM wP,LPARAM lP);
	afx_msg LRESULT OnNextFrame(WPARAM wP,LPARAM lP);
	afx_msg LRESULT OnGo(WPARAM wP,LPARAM lP);
	afx_msg LRESULT OnOK(WPARAM wP,LPARAM lP);
	afx_msg LRESULT OnDelete(WPARAM wP,LPARAM lP);
	afx_msg LRESULT OnSetBGC(WPARAM wP, LPARAM lP);
	afx_msg LRESULT OnSpeedChange(WPARAM wP, LPARAM lP);
	afx_msg LRESULT OnClear(WPARAM wP, LPARAM lP);


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGesture();
	int frame;
	int maxframe;
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	framenode* certain;
	framenode* previous;
	framenode* listhead;

	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // skeletal animationView.cpp �еĵ��԰汾
inline CskeletalanimationDoc* CskeletalanimationView::GetDocument() const
   { return reinterpret_cast<CskeletalanimationDoc*>(m_pDocument); }
#endif

