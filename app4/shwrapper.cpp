#include "stdafx.h"

#include "shwrapper.h"
#include <iostream>
using namespace std;

#pragma warning(disable : 4996)

// USER_API����
extern CThostFtdcMdApi* pUserApi;

// ���ò���
extern char FRONT_ADDR[];
extern TThostFtdcBrokerIDType	BROKER_ID;
extern TThostFtdcInvestorIDType INVESTOR_ID;
extern TThostFtdcPasswordType	PASSWORD;
extern char* ppInstrumentID[];
extern int iInstrumentID;

// ������
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
	///�û���¼����
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
//	cerr << "--->>> �����û���¼����: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
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
		///��ȡ��ǰ������
		cerr << "--->>> ��ȡ��ǰ������ = " << pUserApi->GetTradingDay() << endl;
		// ����������
		//SubscribeMarketData(ppInstrumentID);
	}
}

void SHWrapper::SubscribeMarketData(char *stocks[], int count)
{
	int iResult = pUserApi->SubscribeMarketData(stocks, count);
	//cerr << "--->>> �������鶩������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
//	info->AddString(_T("subscribe market data"));
}

void SHWrapper::UnSubscribeMarketData(char *stocks[], int count)
{
	int iResult = pUserApi->UnSubscribeMarketData(stocks, count);
	//cerr << "--->>> �������鶩������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
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
//	cerr << "���¼� " << pDepthMarketData->LastPrice << endl;
//	cerr << "��һ " << pDepthMarketData->AskPrice1 << endl;
//	cerr << "��һ " << pDepthMarketData->BidPrice1 << endl;
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
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
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

///ȡ������ѯ��Ӧ��
void SHWrapper::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "OnRspUnSubForQuoteRsp" << endl;
}
