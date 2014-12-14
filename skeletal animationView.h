
// skeletal animationView.h : CskeletalanimationView 类的接口
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
	int spin;/*舞台旋转参数*/
	BOOL m_play;/*是否播放*/
	bool render_select;/*进入选择与拾取控制*/
	GLuint select_buf[512];/*选择与拾取返回缓存*/
	void displayTop( GLenum mode);/*显示顶视图*/
	void displayBeside( GLenum mode);/*显示侧视图*/
	void displayFront(GLenum mode);/*显示前景图*/
	void SRecover();
	void BGM();
	
	GLfloat light_position[4] ;
    GLMmodel *head;/*头模型指针对象*/
    GLMmodel *body;/*身体模型指针对象*/
    GLMmodel *leftdatui;/*身体模型指针对象*/
	GLMmodel *rightdatui;/*身体模型指针对象*/
	GLMmodel *leftxiaotui;/*左小腿模型指针对象*/
	GLMmodel *rightxiaotui;/*右小腿模型指针对象*/
	GLMmodel *leftfoot;/*左脚模型指针对象*/
	GLMmodel *rightfoot;/*右脚模型指针对象*/
	GLMmodel *leftbi;/*左臂模型指针对象*/
	GLMmodel *rightbi;/*右臂模型指针对象*/
	GLMmodel *leftzhou;/*左肘模型指针对象*/
	GLMmodel *rightzhou;/*右肘模型指针对象*/
	GLMmodel *righthand;/*右手模型指针对象*/
	GLMmodel *lefthand;/*左手模型指针对象*/

	//背景色
	GLfloat RGBA;
	GLfloat RGBB;
	GLfloat RGBC;


	int h;/*客户区的宽度*/
	int w;/*客户区的长度*/

	int frameTime = 100;

	int Pshift=0; //shift快捷键切换修改的位置

	//键盘移动信号
	bool Mup=false;
	bool Mdown = false;
	bool Mleft = false;
	bool Mright = false;

	bool CBDown = false;//记录ctrl键的按下

	float check(float xy); //控制参数在-360~360内

	Gesture* pDialog;//对话框

	void display();/*显示函数*/
	void myinit();/*初始化函数*/
	bool CreateOpenGLContext();/*创建绘图描述表*/
	bool SetupPixelFormat();/*设置像素格式*/
	HGLRC m_hRC;/*绘图描述表句柄*/
	HDC m_hDC;/*设备描述表句柄*/
	virtual ~CskeletalanimationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:	
	long m_leftDownPos[2];//存放原始鼠标坐标
	float m_xRotation;//X方向旋转角度
	float m_yRotation;//Z方向旋转角度
	GLfloat z_Pos=-2.0;
	GLfloat scaleMolNow;/*放大系数*/
	GLint scaleMol;/*放大次数*/
	GLfloat left_right;/*左右平移控制*/
	GLfloat up_down;/*上下平移控制*/

	GLfloat scaling;  // 单次滚轮缩放倍数
	GLfloat Rscaling;   //记录总放大倍数


    bool scaleMode;/*进入放大模式控制参数*/
	bool ok;/*进入侧顶视图控制参数*/

	GLfloat rotate_left_right;/*左右旋转控制*/
	GLfloat rotate_up_down;/*上下旋转控制*/
	
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

#ifndef _DEBUG  // skeletal animationView.cpp 中的调试版本
inline CskeletalanimationDoc* CskeletalanimationView::GetDocument() const
   { return reinterpret_cast<CskeletalanimationDoc*>(m_pDocument); }
#endif

