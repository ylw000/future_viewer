
// app4Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "app4.h"
#include "app4Dlg.h"
#include "afxdialogex.h"
#include "itemslot.h"
#include "resultslot.h"
#include "MyGraph.h"
#include "resource.h"

#include <mmsystem.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Capp4Dlg 对话框



Capp4Dlg::Capp4Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Capp4Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	enableIB = true;
	enableSH = true;
}

void Capp4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Capp4Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_EN_CHANGE(IDC_EDIT1, &Capp4Dlg::OnEnChangeEdit1)
//	ON_BN_CLICKED(IDC_BUTTON2, &Capp4Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &Capp4Dlg::OnBnClickedButton1)
//	ON_BN_CLICKED(IDC_BUTTON3, &Capp4Dlg::OnBnClickedButton3)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_GEN_DATA, &Capp4Dlg::OnBnClickedGenData)
	ON_BN_CLICKED(IDC_BTN_EXG, &Capp4Dlg::OnBnClickedBtnSetexg)
	ON_BN_CLICKED(IDC_BTN_GETSH, &Capp4Dlg::OnBnClickedBtnGetsh)
	ON_BN_CLICKED(IDC_BTN_GETNY, &Capp4Dlg::OnBnClickedBtnGetny)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_CUST_ASK, &Capp4Dlg::OnEnChangeCustAsk)
	ON_EN_CHANGE(IDC_CUST_BID, &Capp4Dlg::OnEnChangeCustBid)
	ON_BN_CLICKED(IDC_STOPALARM, &Capp4Dlg::OnBnClickedStopalarm)
	ON_BN_CLICKED(IDC_BTN_MULT, &Capp4Dlg::OnBnClickedBtnMult)
	ON_BN_CLICKED(IDC_BTN_FORCETT, &Capp4Dlg::OnBnClickedBtnForcett)
END_MESSAGE_MAP()


// Capp4Dlg 消息处理程序

CThostFtdcMdApi* pUserApi;

// 配置参数
//char FRONT_ADDR[] = "tcp://asp-sim2-md1.financial-trading-platform.com:26213";		// 前置地址
char FRONT_ADDR[] = "tcp://180.168.212.75:41213";
TThostFtdcBrokerIDType	BROKER_ID = "2030";				// 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID = "00092";			// 投资者代码
TThostFtdcPasswordType  PASSWORD = "888888";			// 用户密码
char *ppInstrumentID[] = { "au1412" };			// 行情订阅列表
int iInstrumentID = 1;									// 行情订阅数量
// 请求编号
int iRequestID = 0;





BOOL Capp4Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	info = (CListBox *)GetDlgItem(IDC_LIST1);

	//设置字体
	CFont *l_font = new CFont();
	l_font->CreateFont(25, 0, 0, 0, FW_NORMAL,
		FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("arial"));
	((CStatic *)GetDlgItem(IDC_I1A1))->SetFont(l_font);
	((CStatic *)GetDlgItem(IDC_I1B1))->SetFont(l_font);
	((CStatic *)GetDlgItem(IDC_I2A1))->SetFont(l_font);
	((CStatic *)GetDlgItem(IDC_I2B1))->SetFont(l_font);
	((CStatic *)GetDlgItem(IDC_RESULT1))->SetFont(l_font);
	((CStatic *)GetDlgItem(IDC_RESULT2))->SetFont(l_font);

	//设置标题
	SetWindowText(_T("Future Viewer"));

	//设置时钟
	SetTimer(1, 5000, NULL);
	SetTimer(2, 1000, NULL);

	//
	GetDlgItem(IDC_LIMIT1)->SetWindowTextW(_T("-1"));
	GetDlgItem(IDC_LIMIT2)->SetWindowTextW(_T("-1"));

	//
	GetDlgItem(IDC_UID)->SetWindowTextW(_T("1"));

	((CEdit *)GetDlgItem(IDC_UID))->SetReadOnly();

	//
	{
		HANDLE hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT, 0, sizeof(int), _T("Local\\SharedMem"));
		//SetNamedSecurityInfo(SM_NAME, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, 0, 0, (PACL)NULL, NULL);
		int err = GetLastError();
		int *x = (int *)MapViewOfFile(hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
		if (err != 0)
		{
/*			CString er;
			er.Format(_T("%d"), err);
			MessageBox(er);*/
		}
		CString str;
		str.Format(_T("%d"), *x);
		GetDlgItem(IDC_UID)->SetWindowTextW(str);
		(*x)++;
	}

	//Hidden Debug Object
	GetDlgItem(IDC_GEN_DATA)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_LIST1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_INFO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_FORCETT)->ShowWindow(SW_HIDE);

	graph = new CMyGraph(info);
	CRect rect;
	GetDlgItem(IDC_GRAPH)->GetWindowRect(&rect);
	GetDlgItem(IDC_GRAPH)->ShowWindow(SW_HIDE);
	graph->Create(_T("Graph"), WS_CHILD | WS_VISIBLE, rect, this);
	CString str;
	str.Format(_T("%d %d %d %d"), rect.left, rect.right, rect.top, rect.bottom);
//	info->AddString(str);
	graph->ShowWindow(SW_SHOW);
	graph->Invalidate();
	 
	nyGetData = false;
	shGetData = false;

	if (enableIB)
	{
		mw = new MyWrapper((CListBox *)GetDlgItem(IDC_LIST1));
		((CEdit *)GetDlgItem(IDC_NY_SYMBOL))->SetWindowTextW(_T("GC"));
		((CEdit *)GetDlgItem(IDC_NY_EXPIRY))->SetWindowTextW(_T("201412"));
		((CEdit *)GetDlgItem(IDC_NY_MULT))->SetWindowTextW(_T("5000"));

		ItemSlot *t = new ItemSlot((CStatic *)GetDlgItem(IDC_I2B1), (CStatic *)GetDlgItem(IDC_I2A1));
		mw->addListener((DataListener *)t);
	}

	isSetExg = true;
	((CEdit *)GetDlgItem(IDC_EDIT_EXG))->SetWindowTextW(_T("6.1402"));
	((CEdit *)GetDlgItem(IDC_EDIT_EXG))->SetReadOnly();
	isSetMult = true;
	((CEdit *)GetDlgItem(IDC_EDIT_MULT))->SetWindowTextW(_T("1"));
	((CEdit *)GetDlgItem(IDC_EDIT_MULT))->SetReadOnly();

	isForceTradeTime = false;

	if (enableSH)
	{
		sw = new SHWrapper((CListBox *)GetDlgItem(IDC_LIST1));

		((CEdit *)GetDlgItem(IDC_SH_SYMBOL))->SetWindowTextW(_T("au1412"));

		// 初始化UserApi

		ItemSlot *t = new ItemSlot((CStatic *)GetDlgItem(IDC_I1B1), (CStatic *)GetDlgItem(IDC_I1A1));
		sw->addListener(t);

		pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();			// 创建UserApi
		CThostFtdcMdSpi* pUserSpi = sw;
		pUserApi->RegisterSpi(pUserSpi);						// 注册事件类
		pUserApi->RegisterFront(FRONT_ADDR);					// connect
//		pUserApi->Join();
		//	pUserApi->Release();

	}

	rs = new ResultSlot(this);
	mw->addListener(rs);
	sw->addListener(rs);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Capp4Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Capp4Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Capp4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Capp4Dlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void Capp4Dlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	CListBox *lb = (CListBox*)GetDlgItem(IDC_LIST1);
	lb->AddString(_T("abc"));
}


void Capp4Dlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	//connect to NY
	
	CString str;
	GetDlgItem(IDC_UID)->GetWindowTextW(str);
	int id = _ttoi(str);
	mw->connect(id);

	//connect to SH
	static bool x = true;
	if (x)
	{
		pUserApi->Init();
		x = false;
	}
}


void Capp4Dlg::OnBnClickedButton3()
{
//	info->AddString(_T("req be clicked"));
	// TODO:  在此添加控件通知处理程序代码
	CString ny_sym, ny_time, ny_mult;
	((CEdit *)GetDlgItem(IDC_NY_SYMBOL))->GetWindowTextW(ny_sym);
	((CEdit *)GetDlgItem(IDC_NY_EXPIRY))->GetWindowTextW(ny_time);
	((CEdit *)GetDlgItem(IDC_NY_MULT))->GetWindowTextW(ny_mult);
	//	info->AddString(ny_sym);
//	info->AddString(ny_time);
	IBString sym = CT2A(ny_sym);
	IBString exp = CT2A(ny_time); 
	IBString mult = CT2A(ny_mult);
	mw->getData(sym, exp, mult);
}


HBRUSH Capp4Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
/*	if (pWnd->GetDlgCtrlID() == IDC_I1B1)
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(200, 100, 100));
		//pWnd->SetFont
		//HBRUSH b = CreateSolidBrush(RGB(0, 100, 0));
		//return b;
	}*/

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
/*	if (nCtlColor == IDC_EDIT_EXG) 
	{
		pDC->SetBkColor(RGB(255, 255, 255));
		pDC->SetTextColor(RGB(255, 0, 0));
		return (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
	}*/
	return hbr;
}


void Capp4Dlg::OnBnClickedGenData()
{
	// TODO:  在此添加控件通知处理程序代码
	static int i = 100;
	static double last = 30.0;
	for (int x = 0; x < 20; x++)
	{
		//double new_val = last + ((rand() % 20 / 100.0) - 0.1);
		double new_val = last;
		//graph->add_data({ i, (rand() % 200 / 100.0) + 6 });
//		graph->add_data({ i, new_val + 0.2, new_val - 0.2, new_val - 0.1, new_val + 0.1 });
		CString str;
		str.Format(_T("%f"), new_val);
		GetDlgItem(IDC_RESULT1)->SetWindowTextW(str);
		last = new_val;
		i++;
		FakeOnTimer(2, i, new_val);
		FakeOnTimer(2, i, new_val);
		FakeOnTimer(2, i, new_val);
		FakeOnTimer(2, i, new_val);
		FakeOnTimer(2, i, new_val);
		FakeOnTimer(1, i, new_val);
	}
	graph->Invalidate(true);
//	CRect rect;
//	graph->GetClientRect(&rect);
	//this->InvalidateRect(&rect);
//	this->Invalidate();
/*	static int i = 0;
	i++;
	if (i % 2 == 1)
		PlaySound(TEXT("alarm.wav"), NULL, SND_FILENAME | SND_ASYNC);
	else
		PlaySound(NULL, NULL, NULL);*/
	
}


void Capp4Dlg::OnBnClickedBtnSetexg()
{
	// TODO:  在此添加控件通知处理程序代码

	isSetExg = !isSetExg;
	if (isSetExg)
	{
		((CButton *)GetDlgItem(IDC_BTN_EXG))->SetWindowTextW(_T("修改"));
		((CEdit *)GetDlgItem(IDC_EDIT_EXG))->SetReadOnly();
		CString str;
		((CEdit *)GetDlgItem(IDC_EDIT_EXG))->GetWindowTextW(str);
		double cur_ex = _ttof(str);
		rs->setExchange(cur_ex);

		//fake a request to ResultSlot
		rs->price(BID, 0);
		OnEnChangeCustAsk();
	}
	else
	{
		((CButton *)GetDlgItem(IDC_BTN_EXG))->SetWindowTextW(_T("设置"));
		((CEdit *)GetDlgItem(IDC_EDIT_EXG))->SetReadOnly(false);
	}
}


void Capp4Dlg::OnBnClickedBtnGetsh()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!shGetData)
	{
		CString sh_sym;
		((CEdit *)GetDlgItem(IDC_SH_SYMBOL))->GetWindowTextW(sh_sym);
		IBString sym = CT2A(sh_sym);
		char *x[1];
		char *stock = _strdup(sym.c_str());
		x[0] = stock;
		sw->SubscribeMarketData(x, 1);
		free(stock);
		((CButton *)GetDlgItem(IDC_BTN_GETSH))->SetWindowTextW(_T("停止上海数据"));
		((CEdit *)GetDlgItem(IDC_SH_SYMBOL))->SetReadOnly();
	}
	else
	{
		rs->reset();
		((CStatic *)GetDlgItem(IDC_I1A1))->SetWindowTextW(_T("-1"));
		((CStatic *)GetDlgItem(IDC_I1B1))->SetWindowTextW(_T("-1"));

		CString sh_sym;
		((CEdit *)GetDlgItem(IDC_SH_SYMBOL))->GetWindowTextW(sh_sym);
		IBString sym = CT2A(sh_sym);
		char *x[1];
		char *stock = _strdup(sym.c_str());
		x[0] = stock;
		sw->UnSubscribeMarketData(x, 1);
		free(stock);
		((CButton *)GetDlgItem(IDC_BTN_GETSH))->SetWindowTextW(_T("获取上海数据"));
		((CEdit *)GetDlgItem(IDC_SH_SYMBOL))->SetReadOnly(false);
	}
	shGetData = !shGetData;
}


void Capp4Dlg::OnBnClickedBtnGetny()
{
	if (!nyGetData)
	{
//		info->AddString(_T("sh req be clicked"));
		// TODO:  在此添加控件通知处理程序代码
		CString ny_sym, ny_time, ny_mult;
		((CEdit *)GetDlgItem(IDC_NY_SYMBOL))->GetWindowTextW(ny_sym);
		((CEdit *)GetDlgItem(IDC_NY_EXPIRY))->GetWindowTextW(ny_time);
		((CEdit *)GetDlgItem(IDC_NY_MULT))->GetWindowTextW(ny_mult);
		//		info->AddString(ny_sym);
//		info->AddString(ny_time);
		IBString sym = CT2A(ny_sym);
		IBString exp = CT2A(ny_time);
		IBString mult = CT2A(ny_mult);
		mw->getData(sym, exp, mult);
		((CButton *)GetDlgItem(IDC_BTN_GETNY))->SetWindowTextW(_T("停止纽约数据"));
		((CEdit *)GetDlgItem(IDC_NY_SYMBOL))->SetReadOnly();
		((CEdit *)GetDlgItem(IDC_NY_EXPIRY))->SetReadOnly();
		((CEdit *)GetDlgItem(IDC_NY_MULT))->SetReadOnly();
	}
	else
	{
		rs->reset();
		((CStatic *)GetDlgItem(IDC_I2A1))->SetWindowTextW(_T("-1"));
		((CStatic *)GetDlgItem(IDC_I2B1))->SetWindowTextW(_T("-1"));
		mw->cancelData();
		((CButton *)GetDlgItem(IDC_BTN_GETNY))->SetWindowTextW(_T("获取纽约数据"));
		((CEdit *)GetDlgItem(IDC_NY_SYMBOL))->SetReadOnly(false);
		((CEdit *)GetDlgItem(IDC_NY_EXPIRY))->SetReadOnly(false);
		((CEdit *)GetDlgItem(IDC_NY_MULT))->SetReadOnly(false);
	}
	nyGetData = !nyGetData;
}

bool Capp4Dlg::isTradingTime(SYSTEMTIME &st)
{
	if (isForceTradeTime)
		return true;
	//For Test

	/*if (st.wHour == 17 && st.wMinute <= 25)
		return true;
	if (st.wHour == 17 && st.wMinute > 33 && st.wMinute <=50)
		return true;*/
	//
	if (st.wDayOfWeek == 0 || st.wDayOfWeek == 6)
		return false;
	if (st.wHour >= 3 && st.wHour < 9)
		return false;
	if (st.wHour == 2 && st.wMinute >= 30)
		return false;

	if (st.wHour >= 12 && st.wHour < 13)
		return false;
	if (st.wHour == 11 && st.wMinute >= 30)
		return false;

	if (st.wHour == 13 && st.wMinute < 30)
		return false;
	if (st.wHour >= 15 && st.wHour < 21)
		return false;


	return true;
}

void Capp4Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	static int lastMinute = -1;
	static int firstday = -1;
	if (nIDEvent == 1)
	{
		bool new_min = false;

		SYSTEMTIME st;
		CString strTime;
		GetLocalTime(&st);
		static int tt = 1;
		strTime.Format(_T("%2d:%2d:%2d"), st.wHour, st.wMinute, st.wSecond);
		//info->AddString(strTime);
		double maxRes = -1, minRes = 1000000, firstRes = -1, lastRes = -1;
		for (int i = 0; i < minuteData.size(); i++)
		{
			if (firstRes == -1)
				firstRes = minuteData[i];
			if (minuteData[i] > maxRes)
				maxRes = minuteData[i];
			if (minuteData[i] < minRes)
				minRes = minuteData[i];
			lastRes = minuteData[i];
		}
		if (minuteData.size() >0 && st.wMinute != lastMinute)
			new_min = true;
		if (new_min)
		{
			minuteData.clear();
			lastMinute = st.wMinute;
		}

		if (st.wHour == 9 && st.wMinute == 0)
			graph->reset_data();

		//info->AddString(strTime);
		if (maxRes != -1 && isTradingTime(st))
		{
			//info->AddString(_T("added data"));
			if (st.wHour < 9)
				st.wHour += 24;
			int time = (st.wHour - 9) * 60 + st.wMinute;
			if (new_min)
			{
				//info->AddString(_T("added data"));
				graph->add_data({ time, maxRes, minRes, firstRes, lastRes });
			}
			else
			{
				//info->AddString(_T("update data"));
				graph->update_data({ time, maxRes, minRes, firstRes, lastRes});
			}
			graph->Invalidate(true);

		}

	}
	else
	{
		CString str;
		((CStatic *)GetDlgItem(IDC_RESULT1))->GetWindowTextW(str);
		double result = _ttof(str);
		if (result != -1)
			minuteData.push_back(result);
	}

	CDialogEx::OnTimer(nIDEvent);
}


//For Test
void Capp4Dlg::FakeOnTimer(UINT_PTR nIDEvent, int cur_min, double result)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	static int lastMinute = -1;
	if (nIDEvent == 1)
	{
		bool new_min = false;
		double maxRes = -1, minRes = 1000000, firstRes = -1, lastRes = -1;
		for (int i = 0; i < minuteData.size(); i++)
		{
			if (firstRes == -1)
				firstRes = minuteData[i];
			if (minuteData[i] > maxRes)
				maxRes = minuteData[i];
			if (minuteData[i] < minRes)
				minRes = minuteData[i];
			lastRes = minuteData[i];
		}
		if (minuteData.size() >0 && cur_min != lastMinute)
			new_min = true;
		if (new_min)
		{
			minuteData.clear();
			lastMinute = cur_min;
		}
		SYSTEMTIME st;
		st.wDayOfWeek = 3;
		st.wHour = ((cur_min / 60) % 24 + 9) % 24;
		st.wMinute = cur_min % 60;

		if (st.wHour == 9 && st.wMinute == 0)
			graph->reset_data();
		//info->AddString(strTime);
		if (maxRes != -1 && isTradingTime(st))
		{
			//info->AddString(_T("added data"));
			if (st.wHour < 9)
				st.wHour += 24;
			int time = (st.wHour - 9) * 60 + st.wMinute;
			if (new_min)
			{
				//info->AddString(_T("added data"));
				graph->add_data({ cur_min, maxRes, minRes, firstRes, lastRes });
			}
			else
			{
				//info->AddString(_T("update data"));
				graph->update_data({ cur_min, maxRes, minRes, firstRes, lastRes });
			}
			graph->Invalidate(true);

		}

	}
	else
	{
		if (result != -1)
			minuteData.push_back(result);
	}

	//CDialogEx::OnTimer(nIDEvent);
}

void Capp4Dlg::OnEnChangeCustAsk()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString stra, strb;
	((CStatic *)GetDlgItem(IDC_CUST_ASK))->GetWindowTextW(stra);
	((CStatic *)GetDlgItem(IDC_CUST_BID))->GetWindowTextW(strb);
	double a, b;
	a = _ttof(stra);
	b = _ttof(strb);
	double exg = ResultSlot::getExchange();
	double mult = ResultSlot::getMultiplier();
	CString result;
	result.Format(_T("%.3f"), a - b * exg * mult / 31.1035);
	((CStatic *)GetDlgItem(IDC_CUST_RESULT))->SetWindowTextW(result);
}


void Capp4Dlg::OnEnChangeCustBid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString stra, strb;
	((CStatic *)GetDlgItem(IDC_CUST_ASK))->GetWindowTextW(stra);
	((CStatic *)GetDlgItem(IDC_CUST_BID))->GetWindowTextW(strb);
	double a, b;
	a = _ttof(stra);
	b = _ttof(strb);
	double exg = ResultSlot::getExchange();
	double mult = ResultSlot::getMultiplier();
	CString result;
	result.Format(_T("%.3f"), a - b * exg * mult / 31.1035);
	((CStatic *)GetDlgItem(IDC_CUST_RESULT))->SetWindowTextW(result);
}


void Capp4Dlg::OnBnClickedStopalarm()
{
	// TODO:  在此添加控件通知处理程序代码
	static bool isAlarm = false;
	if (!isAlarm)
	{
		rs->setAlarm(true);
		((CEdit *)GetDlgItem(IDC_LIMIT1))->SetReadOnly(true);
		((CEdit *)GetDlgItem(IDC_LIMIT2))->SetReadOnly(true);
		((CButton *)GetDlgItem(IDC_STOPALARM))->SetWindowTextW(_T("停止警报"));
	}
	else
	{
		rs->setAlarm(false);
		PlaySound(NULL, NULL, NULL);
		((CEdit *)GetDlgItem(IDC_LIMIT1))->SetReadOnly(false);
		((CEdit *)GetDlgItem(IDC_LIMIT2))->SetReadOnly(false);
		((CButton *)GetDlgItem(IDC_STOPALARM))->SetWindowTextW(_T("开始警报"));
	}
	isAlarm = !isAlarm;
}


void Capp4Dlg::OnBnClickedBtnMult()
{
	// TODO:  在此添加控件通知处理程序代码
	isSetMult = !isSetMult;
	if (isSetMult)
	{
		((CButton *)GetDlgItem(IDC_BTN_MULT))->SetWindowTextW(_T("修改"));
		((CEdit *)GetDlgItem(IDC_EDIT_MULT))->SetReadOnly();
		CString str;
		((CEdit *)GetDlgItem(IDC_EDIT_MULT))->GetWindowTextW(str);
		double cur_mult = _ttof(str);
		rs->setMultiplier(cur_mult);

		//fake a request to ResultSlot
		rs->price(BID, 0);
		OnEnChangeCustAsk();
	}
	else
	{
		((CButton *)GetDlgItem(IDC_BTN_MULT))->SetWindowTextW(_T("设置"));
		((CEdit *)GetDlgItem(IDC_EDIT_MULT))->SetReadOnly(false);
	}
}


void Capp4Dlg::OnBnClickedBtnForcett()
{
	// TODO:  在此添加控件通知处理程序代码
	isForceTradeTime = !isForceTradeTime;
	if (isForceTradeTime)
		GetDlgItem(IDC_BTN_FORCETT)->SetWindowTextW(_T("NoForceTradeTime"));
	else
		GetDlgItem(IDC_BTN_FORCETT)->SetWindowTextW(_T("ForceTradeTime"));
}
