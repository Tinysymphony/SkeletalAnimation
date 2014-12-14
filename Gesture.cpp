// Gesture.cpp : 实现文件
//

#include "stdafx.h"
#include "skeletal animation.h"
#include "Gesture.h"
#include "afxdialogex.h"


// Gesture 对话框
#define WM_SET_GESTURE WM_USER+100//自定义消息
#define WM_OK WM_USER+101//自定义消息
#define WM_DELETE WM_USER+102//自定义消息
#define WM_PREVIOUS_FRAME WM_USER+103//自定义消息
#define WM_NEXT_FRAME WM_USER+104//自定义消息
#define WM_GO WM_USER+105//自定义消息
#define WM_GESTURE_CHANGE 106 //自定义消息，滑动条更新
#define WM_MYCOLOR 107 //自定义消息，背景色改变
#define WM_MYSPEED 108 //自定义消息，速度更改
#define WM_MYCLEAR 109 //自定义消息，清屏

IMPLEMENT_DYNAMIC(Gesture, CDialogEx)

Gesture::Gesture(CWnd* pParent /*=NULL*/)
	: CDialogEx(Gesture::IDD, pParent)
	, Hx(0)
	, Hz(0)
	, luax(0)
	, luaz(0)
	, lfx(0)
	, lfz(0)
	, ruax(0)
	, ruaz(0)
	, rfx(0)
	, rfz(0)
	, ltx(0)
	, ltz(0)
	, lcx(0)
	, lcz(0)
	, rtx(0)
	, rtz(0)
	, rcx(0)
	, rcz(0)
	, cR(0)
	, cG(0)
	, cB(0)
	, m_Radio(-1)
	, frame(1)
	, mySpeed(100)
{
	ASSERT(pParent);
	m_pParent = pParent;

	m_black=RGB(0,0,0);
	m_white=RGB(255,255,255);
	m_red=RGB(255,0,0);

	m_color=RGB(255,255,255);                      // 白色
	m_textcolor=RGB(0,0,0);                 // 黑色文本
	m_brush.CreateSolidBrush(m_color);      // 白色画刷

	//CP1x.SetRange(-360, 360);
	
}

Gesture::~Gesture()
{
}

void Gesture::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITheadx, Hx);
	DDX_Text(pDX, IDC_EDITheadz, Hz);
	DDX_Text(pDX, IDC_EDITleftuparmx, luax);
	DDX_Text(pDX, IDC_EDITleftuparmz, luaz);
	DDX_Text(pDX, IDC_EDITleftforearmx, lfx);
	DDX_Text(pDX, IDC_EDITleftforearmz, lfz);
	DDX_Text(pDX, IDC_EDITrightuparmx, ruax);
	DDX_Text(pDX, IDC_EDITrightuparmz, ruaz);
	DDX_Text(pDX, IDC_EDITrightforearmx, rfx);
	DDX_Text(pDX, IDC_EDITrightforearmz, rfz);
	DDX_Text(pDX, IDC_EDITleftthighx, ltx);
	DDX_Text(pDX, IDC_EDITleftthighz, ltz);
	DDX_Text(pDX, IDC_EDITleftcrusx, lcx);
	DDX_Text(pDX, IDC_EDITleftcrusz, lcz);
	DDX_Text(pDX, IDC_EDITrightthighx, rtx);
	DDX_Text(pDX, IDC_EDITrightthighz, rtz);
	DDX_Text(pDX, IDC_EDITrightcrusx, rcx);
	DDX_Text(pDX, IDC_EDITrightcrusz, rcz);
	DDX_Radio(pDX, IDC_RADIO1, m_Radio);
	DDX_Text(pDX, IDC_EDITframe, frame);
	DDX_Text(pDX, IDC_EDIT2, mySpeed);
	DDX_Control(pDX, IDC_SLIDER1, CP1x);
	DDX_Control(pDX, IDC_SLIDER10, CP1y);
	DDX_Control(pDX, IDC_SLIDER2, CP2x);
	DDX_Control(pDX, IDC_SLIDER11, CP2y);
	DDX_Control(pDX, IDC_SLIDER3, CP3x);
	DDX_Control(pDX, IDC_SLIDER12, CP3y);
	DDX_Control(pDX, IDC_SLIDER4, CP4x);
	DDX_Control(pDX, IDC_SLIDER13, CP4y);
	DDX_Control(pDX, IDC_SLIDER5, CP5x);
	DDX_Control(pDX, IDC_SLIDER14, CP5y);
	DDX_Control(pDX, IDC_SLIDER6, CP6x);
	DDX_Control(pDX, IDC_SLIDER15, CP6y);
	DDX_Control(pDX, IDC_SLIDER7, CP7x);
	DDX_Control(pDX, IDC_SLIDER16, CP7y);
	DDX_Control(pDX, IDC_SLIDER8, CP8x);
	DDX_Control(pDX, IDC_SLIDER17, CP8y);
	DDX_Control(pDX, IDC_SLIDER9, CP9x);
	DDX_Control(pDX, IDC_SLIDER18, CP9y);
	DDX_Control(pDX, IDC_SLIDER19, ColorR);
	DDX_Control(pDX, IDC_SLIDER20, ColorG);
	DDX_Control(pDX, IDC_SLIDER21, ColorB);
	DDX_Control(pDX, IDC_SLIDER22, DSpeed);
	DDX_Control(pDX, IDC_EDIT2, MSpeed);
	DDX_Control(pDX, IDC_EDITheadz, cp12);
	DDX_Control(pDX, IDC_EDITheadx, cp11);
}


BEGIN_MESSAGE_MAP(Gesture, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OK, &Gesture::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_RADIO1, &Gesture::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &Gesture::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &Gesture::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &Gesture::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &Gesture::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, &Gesture::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, &Gesture::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, &Gesture::OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO9, &Gesture::OnBnClickedRadio9)
	ON_BN_CLICKED(IDC_BUTTONpframe, &Gesture::OnBnClickedButtonpframe)
	ON_BN_CLICKED(IDC_BUTTONnframe, &Gesture::OnBnClickedButtonnframe)
	ON_BN_CLICKED(IDC_BUTTONgo, &Gesture::OnBnClickedButtongo)
	ON_BN_CLICKED(IDC_BUTTONOK, &Gesture::OnBnClickedButtonok)
	ON_BN_CLICKED(IDC_BUTTONDELETE, &Gesture::OnBnClickedButtondelete)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(2, &Gesture::OnBnClicked2)
	ON_EN_CHANGE(IDC_EDITheady, &Gesture::OnEnChangeEditheady)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &Gesture::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER10, &Gesture::OnNMCustomdrawSlider10)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &Gesture::OnNMCustomdrawSlider2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER11, &Gesture::OnNMCustomdrawSlider11)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &Gesture::OnNMCustomdrawSlider3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER12, &Gesture::OnNMCustomdrawSlider12)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER4, &Gesture::OnNMCustomdrawSlider4)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER13, &Gesture::OnNMCustomdrawSlider13)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER5, &Gesture::OnNMCustomdrawSlider5)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER14, &Gesture::OnNMCustomdrawSlider14)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER6, &Gesture::OnNMCustomdrawSlider6)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER15, &Gesture::OnNMCustomdrawSlider15)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER7, &Gesture::OnNMCustomdrawSlider7)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER16, &Gesture::OnNMCustomdrawSlider16)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER8, &Gesture::OnNMCustomdrawSlider8)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER17, &Gesture::OnNMCustomdrawSlider17)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER9, &Gesture::OnNMCustomdrawSlider9)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER18, &Gesture::OnNMCustomdrawSlider18)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER19, &Gesture::OnNMCustomdrawSlider19)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER20, &Gesture::OnNMCustomdrawSlider20)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER21, &Gesture::OnNMCustomdrawSlider21)
	ON_EN_CHANGE(IDC_EDITframe, &Gesture::OnEnChangeEditframe)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER22, &Gesture::OnNMCustomdrawSlider22)
	ON_EN_CHANGE(IDC_EDIT2, &Gesture::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDITheadz, &Gesture::OnEnChangeEditheadz)
	ON_BN_CLICKED(IDC_BUTTON1, &Gesture::OnBnClickedButton1)
END_MESSAGE_MAP()

// Gesture 消息处理程序
void Gesture::OnBnClickedButtonOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();//获取编辑框数据
	m_pParent->SendMessage(WM_SET_GESTURE,(WPARAM)this);//发送自定义消息
}

void Gesture::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Radio = 0;
	//AfxMessageBox(L"d");
}


void Gesture::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Radio = 1;
}


void Gesture::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Radio = 2;
}


void Gesture::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Radio = 3;
}


void Gesture::OnBnClickedRadio5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Radio = 4;
}


void Gesture::OnBnClickedRadio6()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Radio = 5;
}


void Gesture::OnBnClickedRadio7()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Radio = 6;
}


void Gesture::OnBnClickedRadio8()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Radio = 7;
}


void Gesture::OnBnClickedRadio9()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Radio = 8;
}


void Gesture::OnBnClickedButtonpframe()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();//获取编辑框数据
	m_pParent->SendMessage(WM_PREVIOUS_FRAME,(WPARAM)this);//发送自定义消息
}


void Gesture::OnBnClickedButtonnframe()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();//获取编辑框数据
	m_pParent->SendMessage(WM_NEXT_FRAME,(WPARAM)this);//发送自定义消息
}


void Gesture::OnBnClickedButtongo()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();//获取编辑框数据
	m_pParent->SendMessage(WM_GO,(WPARAM)this);//发送自定义消息
}


void Gesture::OnBnClickedButtonok()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();//获取编辑框数据
	m_pParent->SendMessage(WM_OK,(WPARAM)this);//发送自定义消息
}


void Gesture::OnBnClickedButtondelete()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();//获取编辑框数据
	m_pParent->SendMessage(WM_DELETE,(WPARAM)this);//发送自定义消息
}


HBRUSH Gesture::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_EDITframe)//第一个编辑框
   {
		pDC->SetBkColor(m_color);    // 输入文本的的底色
		pDC->SetTextColor(m_textcolor);  // 输入文本的颜色
		hbr = (HBRUSH) m_brush;    // 编辑框控件的填充色
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void Gesture::OnBnClicked2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Radio = -1;
	GControl = false;
	this->ShowWindow(SW_HIDE);
}

BOOL Gesture::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	//拖动条初始化
	CP1x.SetRange(-360,360);
	CP1x.SetTicFreq(1);
	CP1x.SetPos(0);

	CP1y.SetRange(-360, 360);
	CP1y.SetTicFreq(1);
	CP1y.SetPos(0);

	CP2x.SetRange(-360, 360);
	CP2x.SetTicFreq(1);
	CP2x.SetPos(0);

	CP2y.SetRange(-360, 360);
	CP2y.SetTicFreq(1);
	CP2y.SetPos(0);

	CP3x.SetRange(-360, 360);
	CP3x.SetTicFreq(1);
	CP3x.SetPos(0);

	CP3y.SetRange(-360, 360);
	CP3y.SetTicFreq(1);
	CP3y.SetPos(0);

	CP4x.SetRange(-360, 360);
	CP4x.SetTicFreq(1);
	CP4x.SetPos(0);

	CP4y.SetRange(-360, 360);
	CP4y.SetTicFreq(1);
	CP4y.SetPos(0);

	CP5x.SetRange(-360, 360);
	CP5x.SetTicFreq(1);
	CP5x.SetPos(0);

	CP5y.SetRange(-360, 360);
	CP5y.SetTicFreq(1);
	CP5y.SetPos(0);

	CP6x.SetRange(-360, 360);
	CP6x.SetTicFreq(1);
	CP6x.SetPos(0);

	CP6y.SetRange(-360, 360);
	CP6y.SetTicFreq(1);
	CP6y.SetPos(0);

	CP7x.SetRange(-360, 360);
	CP7x.SetTicFreq(1);
	CP7x.SetPos(0);

	CP7y.SetRange(-360, 360);
	CP7y.SetTicFreq(1);
	CP7y.SetPos(0);

	CP8x.SetRange(-360, 360);
	CP8x.SetTicFreq(1);
	CP8x.SetPos(0);

	CP8y.SetRange(-360, 360);
	CP8y.SetTicFreq(1);
	CP8y.SetPos(0);

	CP9x.SetRange(-360, 360);
	CP9x.SetTicFreq(1);
	CP9x.SetPos(0);

	CP9y.SetRange(-360, 360);
	CP9y.SetTicFreq(1);
	CP9y.SetPos(0);

	ColorR.SetRange(0,1000);
	ColorR.SetTicFreq(1);
	ColorR.SetPos(0);

	ColorG.SetRange(0, 1000);
	ColorG.SetTicFreq(1);
	ColorG.SetPos(0);

	ColorB.SetRange(0, 1000);
	ColorB.SetTicFreq(1);
	ColorB.SetPos(0);

	DSpeed.SetRange(10, 1000);
	DSpeed.SetTicFreq(1);
	DSpeed.SetPos(100);

	return TRUE;
}

void Gesture::OnEnChangeEditheady()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void Gesture::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	//m_Radio = 0;
	int nPos = CP1x.GetPos();
	if (Hx != (float)nPos)
	{
		m_Radio =0 ;
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(FALSE);
	}
	CString str;
	str.Format(L"%d", nPos);
	SetDlgItemText(IDC_EDITheadx,str);
	m_pParent->SendMessage(WM_GESTURE_CHANGE, (WPARAM)this);
	UpdateData(TRUE);
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider10(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	//m_Radio = 0;
	int nPos = CP1y.GetPos();
	if (Hz != (float)nPos)
	{
		m_Radio = 0;
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(FALSE);
	}
	CString str;
	str.Format(L"%d", nPos);
	SetDlgItemText(IDC_EDITheadz, str);
	m_pParent->SendMessage(WM_GESTURE_CHANGE, (WPARAM)this);
	UpdateData(TRUE);
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int nPos = CP2x.GetPos();
	if (luax != (float)nPos)
	{
		m_Radio = 1;
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(FALSE);
	}
	CString str;
	str.Format(L"%d", nPos);
	SetDlgItemText(IDC_EDITleftuparmx, str);
	m_pParent->SendMessage(WM_GESTURE_CHANGE, (WPARAM)this);
	UpdateData(TRUE);
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider11(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	
	int nPos = CP2y.GetPos();
	if (luaz != (float)nPos)
	{
		m_Radio = 1;
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(FALSE);
	}
	CString str;
	str.Format(L"%d", nPos);
	SetDlgItemText(IDC_EDITleftuparmz, str);
	m_pParent->SendMessage(WM_GESTURE_CHANGE, (WPARAM)this);
	UpdateData(TRUE);
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int nPos = CP3x.GetPos();
	if (lfx != (float)nPos)
	{
		m_Radio = 2;
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(FALSE);
	}
	CString str;
	str.Format(L"%d", nPos);
	SetDlgItemText(IDC_EDITleftforearmx, str);
	m_pParent->SendMessage(WM_GESTURE_CHANGE, (WPARAM)this);
	UpdateData(TRUE);
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider12(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	
	int nPos = CP3y.GetPos();
	if (lfz != (float)nPos)
	{
		m_Radio = 2;
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(FALSE);
	}
	CString str;
	str.Format(L"%d", nPos);
	SetDlgItemText(IDC_EDITleftforearmz, str);
	m_pParent->SendMessage(WM_GESTURE_CHANGE, (WPARAM)this);
	UpdateData(TRUE);
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int nPos = CP4x.GetPos();
	if (ruax != (float)nPos)
	{
		m_Radio = 3;
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(FALSE);
	}
	CString str;
	str.Format(L"%d", nPos);
	SetDlgItemText(IDC_EDITrightuparmx, str);
	m_pParent->SendMessage(WM_GESTURE_CHANGE, (WPARAM)this);
	UpdateData(TRUE);
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider13(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int nPos = CP4y.GetPos();
	if (ruaz != (float)nPos)
	{
		m_Radio = 3;
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(FALSE);
	}
	CString str;
	str.Format(L"%d", nPos);
	SetDlgItemText(IDC_EDITrightuparmz, str);
	m_pParent->SendMessage(WM_GESTURE_CHANGE, (WPARAM)this);
	UpdateData(TRUE);
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int nPos = CP5x.GetPos();
	if (rfx != (float)nPos)
	{
		m_Radio = 4;
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(FALSE);
	}
	CString str;
	str.Format(L"%d", nPos);
	SetDlgItemText(IDC_EDITrightforearmx, str);
	m_pParent->SendMessage(WM_GESTURE_CHANGE, (WPARAM)this);
	UpdateData(TRUE);
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider14(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int nPos = CP5y.GetPos();
	if (rfz != (float)nPos)
	{
		m_Radio = 4;
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(FALSE);
	}
	CString str;
	str.Format(L"%d", nPos);
	SetDlgItemText(IDC_EDITrightforearmz, str);
	m_pParent->SendMessage(WM_GESTURE_CHANGE, (WPARAM)this);
	UpdateData(TRUE);
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int nPos = CP6x.GetPos();
	if (ltx != (float)nPos)
	{
		m_Radio = 5;
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(FALSE);
	}
	CString str;
	str.Format(L"%d", nPos);
	SetDlgItemText(IDC_EDITleftthighx, str);
	m_pParent->SendMessage(WM_GESTURE_CHANGE, (WPARAM)this);
	UpdateData(TRUE);
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider15(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int nPos = CP6y.GetPos();
	if (ltz != (float)nPos)
	{
		m_Radio = 5;
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(FALSE);
	}
	CString str;
	str.Format(L"%d", nPos);
	SetDlgItemText(IDC_EDITleftthighz, str);
	m_pParent->SendMessage(WM_GESTURE_CHANGE, (WPARAM)this);
	UpdateData(TRUE);
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider7(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码

	int nPos = CP7x.GetPos();
	if (lcx != (float)nPos)
	{
		m_Radio = 6;
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(FALSE);
	}
	CString str;
	str.Format(L"%d", nPos);
	SetDlgItemText(IDC_EDITleftcrusx, str);
	m_pParent->SendMessage(WM_GESTURE_CHANGE, (WPARAM)this);
	UpdateData(TRUE);
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider16(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int nPos = CP7y.GetPos();
	if (lcz != (float)nPos)
	{
		m_Radio = 6;
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(FALSE);
	}
	CString str;
	str.Format(L"%d", nPos);
	SetDlgItemText(IDC_EDITleftcrusz, str);
	m_pParent->SendMessage(WM_GESTURE_CHANGE, (WPARAM)this);
	UpdateData(TRUE);
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider8(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int nPos = CP8x.GetPos();
	if (rtx != (float)nPos)
	{
		m_Radio = 7;
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(FALSE);
	}
	CString str;
	str.Format(L"%d", nPos);
	SetDlgItemText(IDC_EDITrightthighx, str);
	m_pParent->SendMessage(WM_GESTURE_CHANGE, (WPARAM)this);
	UpdateData(TRUE);
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider17(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int nPos = CP8y.GetPos();
	if (rtz != (float)nPos)
	{
		m_Radio = 7;
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(FALSE);
	}
	CString str;
	str.Format(L"%d", nPos);
	SetDlgItemText(IDC_EDITrightthighz, str);
	m_pParent->SendMessage(WM_GESTURE_CHANGE, (WPARAM)this);
	UpdateData(TRUE);
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider9(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int nPos = CP9x.GetPos();
	if (rcx != (float)nPos)
	{
		m_Radio = 8;
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(TRUE);
	}
	CString str;
	str.Format(L"%d", nPos);
	SetDlgItemText(IDC_EDITrightcrusx, str);
	m_pParent->SendMessage(WM_GESTURE_CHANGE, (WPARAM)this);
	UpdateData(TRUE);
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider18(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int nPos = CP9y.GetPos();
	if (rcz != (float)nPos)
	{
		m_Radio = 8;
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(TRUE);
	}
	CString str;
	str.Format(L"%d", nPos);
	SetDlgItemText(IDC_EDITrightcrusz, str);
	m_pParent->SendMessage(WM_GESTURE_CHANGE, (WPARAM)this);
	UpdateData(TRUE);
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider19(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	float nPos = (float)ColorR.GetPos()/1000;
	if (nPos != cR)
	{
		cR = nPos;
		m_pParent->SendMessage(WM_MYCOLOR, (WPARAM)this);
	}	
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider20(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	float nPos = (float)ColorG.GetPos() / 1000;
	if (nPos != cG)
	{
		cG = nPos;
		m_pParent->SendMessage(WM_MYCOLOR, (WPARAM)this);
	}
	*pResult = 0;
}


void Gesture::OnNMCustomdrawSlider21(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	float nPos = (float)ColorB.GetPos() / 1000;
	if (nPos != cB)
	{
		cB = nPos;
		m_pParent->SendMessage(WM_MYCOLOR, (WPARAM)this);
	}
	*pResult = 0;
}


void Gesture::OnEnChangeEditframe()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void Gesture::CPupdate()
{
	CP1x.SetPos((int)Hx);
	CP1y.SetPos((int)Hz);
	CP2x.SetPos((int)luax);
	CP2y.SetPos((int)luaz);
	CP3x.SetPos((int)lfx);
	CP3y.SetPos((int)lfz);
	CP4x.SetPos((int)ruax);
	CP4y.SetPos((int)ruaz);
	CP5x.SetPos((int)rfx);
	CP5y.SetPos((int)rfz);
	CP6x.SetPos((int)ltx);
	CP6y.SetPos((int)ltz);
	CP7x.SetPos((int)lcx);
	CP7y.SetPos((int)lcz);
	CP8x.SetPos((int)rtx);
	CP8y.SetPos((int)rtz);
	CP9x.SetPos((int)rcx);
	CP9y.SetPos((int)rcz);
	UpdateData(false);
}

void Gesture::OnNMCustomdrawSlider22(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int nPos = (float)DSpeed.GetPos();
	if (nPos != mySpeed)
	{
		mySpeed = nPos;
		CString str;
		str.Format(L"%d", nPos);
		SetDlgItemText(IDC_EDIT2, str);
		m_pParent->SendMessage(WM_MYSPEED, (WPARAM)this);
		UpdateData(TRUE);
	}
	*pResult = 0;
}


void Gesture::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码


}


void Gesture::OnEnChangeEditheadz()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void Gesture::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	  Hx=0;
	  Hz=0;
	  luax=0;
	  luaz=0;
	  lfx=0;
	  lfz=0;
	  ruax=0;
	  ruaz=0;
	  rfx=0;
	  rfz=0;
	  ltx=0;
	  ltz=0;
	  lcx=0;
	  lcz=0;
	  rtx=0;
	  rtz=0;
	  rcx=0;
	  rcz=0;

	  cR=0;
	  cG=0;
	  cB=0;

	  mySpeed = 100;

	  CP1x.SetPos(0);

	  CP1y.SetPos(0);

	  CP2x.SetPos(0);

	  CP2y.SetPos(0);

	  CP3x.SetPos(0);

	  CP3y.SetPos(0);

	  CP4x.SetPos(0);

	  CP4y.SetPos(0);

	  CP5x.SetPos(0);

	  CP5y.SetPos(0);

	  CP6x.SetPos(0);

	  CP6y.SetPos(0);

	  CP7x.SetPos(0);

	  CP7y.SetPos(0);

	  CP8x.SetPos(0);

	  CP8y.SetPos(0);

	  CP9x.SetPos(0);

	  CP9y.SetPos(0);

	  ColorR.SetPos(0);

	  ColorG.SetPos(0);

	  ColorB.SetPos(0);

	  DSpeed.SetPos(100);

	  UpdateData(false);

	  m_pParent->SendMessage(WM_MYCLEAR, (WPARAM)this);

}
