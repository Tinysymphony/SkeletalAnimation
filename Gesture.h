#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// Gesture 对话框

class Gesture : public CDialogEx
{
	DECLARE_DYNAMIC(Gesture)

public:
	Gesture(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Gesture();
	CWnd* m_pParent;
	CBrush m_brush;//定义画刷
	COLORREF m_color,m_textcolor,m_black,m_white,m_red;//定义颜色结构
// 对话框数据
	enum { IDD = IDD_DIALOG_GESTURE };
	BOOL GControl = false; //控制板开关状态

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	float Hx;
	float Hz;
	float luax;
	float luaz;
	float lfx;
	float lfz;
	float ruax;
	float ruaz;
	float rfx;
	float rfz;
	float ltx;
	float ltz;
	float lcx;
	float lcz;
	float rtx;
	float rtz;
	float rcx;
	float rcz;

	float cR;
	float cG;
	float cB;

	int mySpeed=100;

	void CPupdate();

	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedRadio1();
	int m_Radio;
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedRadio9();
	int frame;
	afx_msg void OnBnClickedButtonpframe();
	afx_msg void OnBnClickedButtonnframe();
	afx_msg void OnBnClickedButtongo();
	afx_msg void OnBnClickedButtonok();
	afx_msg void OnBnClickedButtondelete();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClicked2();
	afx_msg void OnEnChangeEditheady();
	CSliderCtrl CP1x;
	CSliderCtrl CP1y;
	CSliderCtrl CP2x;
	CSliderCtrl CP2y;
	CSliderCtrl CP3x;
	CSliderCtrl CP3y;
	CSliderCtrl CP4x;
	CSliderCtrl CP4y;
	CSliderCtrl CP5x;
	CSliderCtrl CP5y;
	CSliderCtrl CP6x;
	CSliderCtrl CP6y;
	CSliderCtrl CP7x;
	CSliderCtrl CP7y;
	CSliderCtrl CP8x;
	CSliderCtrl CP8y;
	CSliderCtrl CP9x;
	CSliderCtrl CP9y;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider10(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider11(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider12(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider13(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider14(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider15(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider7(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider16(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider8(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider17(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider9(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider18(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl ColorR;
	CSliderCtrl ColorG;
	CSliderCtrl ColorB;
	afx_msg void OnNMCustomdrawSlider19(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider20(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider21(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditframe();
	afx_msg void OnNMCustomdrawSlider22(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl DSpeed;
	CEdit MSpeed;
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEditheadz();
	CEdit cp12;
	CEdit cp11;
	afx_msg void OnBnClickedButton1();
};
