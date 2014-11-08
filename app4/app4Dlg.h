
// app4Dlg.h : ͷ�ļ�
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

// Capp4Dlg �Ի���
class Capp4Dlg : public CDialogEx
{
// ����
public:
	Capp4Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_APP4_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
