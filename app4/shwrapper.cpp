#include "stdafx.h"

#include "shwrapper.h"
#include <iostream>
using namespace std;

#pragma warning(disable : 4996)

// USER_API参数
extern CThostFtdcMdApi* pUserApi;

// 配置参数
extern char FRONT_ADDR[];
extern TThostFtdcBrokerIDType	BROKER_ID;
extern TThostFtdcInvestorIDType INVESTOR_ID;
extern TThostFtdcPasswordType	PASSWORD;
extern char* ppInstrumentID[];
extern int iInstrumentID;

// 请求编号
extern int iRequestID;

SHWrapper::SHWrapper(CListBox *output)
{
	info = output;
}

void SHWrapper::addListener(DataListener *x)
{
	listener.push_back(x);
}

void SHWrapper::OnRspError(CThostFtdcRspInfoField *pRspInfo,
	int nRequestID, bool bIsLast)
{
	info->AddString(_T("OnRspError"));
	IsErrorRspInfo(pRspInfo);
}

void SHWrapper::OnFrontDisconnected(int nReason)
{
	info->AddString(_T("OnFrontDisconnected"));
	CString res;
	res.Format(_T("Reason = %d"), nReason);
	info->AddString(res);
}

void SHWrapper::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "--->>> " << "OnHeartBeatWarning" << endl;
	cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void SHWrapper::OnFrontConnected()
{
//	info->AddString(_T("OnFrontConnected"));
	///用户登录请求
	ReqUserLogin();
}

void SHWrapper::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.UserID, INVESTOR_ID);
	strcpy(req.Password, PASSWORD);
	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
//	cerr << "--->>> 发送用户登录请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
	CString res;
	res.Format(_T("req login: %s"), (iResult == 0) ? "suc" : "fail");
//	info->AddString(res);
}

void SHWrapper::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
//	info->AddString(_T("OnRspUserLogin"));
	cerr << "--->>> " << "OnRspUserLogin" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///获取当前交易日
		cerr << "--->>> 获取当前交易日 = " << pUserApi->GetTradingDay() << endl;
		// 请求订阅行情
		//SubscribeMarketData(ppInstrumentID);
	}
}

void SHWrapper::SubscribeMarketData(char *stocks[], int count)
{
	int iResult = pUserApi->SubscribeMarketData(stocks, count);
	//cerr << "--->>> 发送行情订阅请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
//	info->AddString(_T("subscribe market data"));
}

void SHWrapper::UnSubscribeMarketData(char *stocks[], int count)
{
	int iResult = pUserApi->UnSubscribeMarketData(stocks, count);
	//cerr << "--->>> 发送行情订阅请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
//	info->AddString(_T("unsubscribe market data"));
}

void SHWrapper::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "OnRspSubMarketData" << endl;
//	info->AddString(_T("OnRspSubMarketData"));

}

void SHWrapper::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "OnRspUnSubMarketData" << endl;
//	info->AddString(_T("OnRspUnSubMarketData"));
}

#include "shwrapper.h"

void SHWrapper::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	cerr << "OnRtnDepthMarketData" << endl;
//	cerr << "最新价 " << pDepthMarketData->LastPrice << endl;
//	cerr << "卖一 " << pDepthMarketData->AskPrice1 << endl;
//	cerr << "买一 " << pDepthMarketData->BidPrice1 << endl;
	CString res;
	res.Format(_T("type %d price %f"), ASK, pDepthMarketData->AskPrice1);
//	info->AddString(res);
	res.Format(_T("type %d price %f"), BID, pDepthMarketData->BidPrice1);
//	info->AddString(res);
//	int nCount = info->GetCount();
//	if (nCount > 0)
//		info->SetCurSel(nCount - 1);

	if (pDepthMarketData->AskPrice1 < 0 && pDepthMarketData->AskPrice1 >= 100000)
		return;
	if (pDepthMarketData->BidPrice1 < 0 && pDepthMarketData->BidPrice1 >= 100000)
		return;

	for (int i = 0; i < listener.size(); i++)
	{
		listener[i]->price(BID, pDepthMarketData->BidPrice1);
		listener[i]->price(ASK, pDepthMarketData->AskPrice1);
	}

}

bool SHWrapper::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}

void SHWrapper::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
	cerr << "OnRtnForQuoteRsp" << endl;
}

void SHWrapper::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "OnRspSubForQuoteRsp" << endl;
}

///取消订阅询价应答
void SHWrapper::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "OnRspUnSubForQuoteRsp" << endl;
}
