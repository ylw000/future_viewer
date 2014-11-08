
// app4Dlg.h : 头文件
//

#pragma once

#include "stdafx.h"
#include "Resource.h"
#include "EClientSocket.h"
#include "TwsSocketClient.h"
#include "EWrapper.h"
#include "mywrapper.h"
#include "shwrapper.h"
#include "MyGraph.h"
//#include "resultslot.h"

class ResultSlot;

// Capp4Dlg 对话框
class Capp4Dlg : public CDialogEx
{
// 构造
public:
	Capp4Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_APP4_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();

public:
	MyWrapper *mw;
	SHWrapper *sw;
	CListBox *info;
	ResultSlot *rs;

	bool enableIB, enableSH;
	CMyGraph *graph;

	bool isSetExg;
	bool isSetMult;
	bool isForceTradeTime;

	bool nyGetData, shGetData;
	std::vector<double> minuteData;

	bool isTradingTime(SYSTEMTIME &st);
	void FakeOnTimer(UINT_PTR nIDEvent, int cur_min, double result);
public:
	afx_msg void OnBnClickedButton3();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedGenData();
	afx_msg void OnBnClickedBtnSetexg();
	afx_msg void OnBnClickedBtnGetsh();
	afx_msg void OnBnClickedBtnGetny();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeCustAsk();
	afx_msg void OnEnChangeCustBid();
	afx_msg void OnBnClickedStopalarm();
	afx_msg void OnBnClickedBtnMult();
	afx_msg void OnBnClickedBtnForcett();
};
