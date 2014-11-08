#include "stdafx.h"
#include "mywrapper.h"

MyWrapper::MyWrapper(CListBox *wnd)
{
	client = new EClientSocket(this);
	info = wnd;
//	wnd->AddString(_T("connecting result %d"));
}

MyWrapper::~MyWrapper()
{

}

void MyWrapper::connect(int id)
{
//	info->AddString(_T("connecting..."));
	bool ret = client->eConnect("127.0.0.1", 7496, id, false);
	CString res_str;
	res_str.Format(_T("connecting result %d"), ret);
//	info->AddString(res_str);

}

void MyWrapper::getData(IBString &symbol, IBString &exp_time, IBString &mult)
{
	//int _code = 71410766;
	int _code = 0;
	IBString ticks = "100,101,104,105,106,107,165,221,225,233,236,258,293,294,295,318";
	Contract x;
	x.conId = _code;
	//x.symbol = "GC";
	x.symbol = symbol;
	x.expiry = exp_time;
	x.multiplier = mult;
	CString str; 
	str.Format(_T("symbol %s"), CString(symbol.c_str()));
//	info->AddString(str);
	str.Format(_T("time %s"), CString(exp_time.c_str()));
//	info->AddString(str);
	x.currency = "USD";
	x.exchange = "NYMEX";
	x.primaryExchange = "NYMEX";
	x.secType = "FUT";
	TagValueListSPtr y;
	client->reqMktData(2, x, ticks, false, y);
}

void MyWrapper::cancelData()
{
	client->cancelMktData(2);
}

void MyWrapper::addListener(DataListener *dl)
{
	if (dl != NULL)
	{
		listener.push_back(dl);
	}
}

void MyWrapper::tickPrice(TickerId ddeId, TickType field, double price, int canAutoExecute)
{
	CString pp;
	pp.Format(_T("type %d price %f"), field, price);
//	info->AddString(pp);
	int nCount = info->GetCount();
	if (nCount > 0)
		info->SetCurSel(nCount - 1);
	for (int i = 0; i < listener.size(); i++)
		listener[i]->price(field, price);
}
void MyWrapper::tickSize(TickerId ddeId, TickType field, int size)
{

}
void MyWrapper::tickOptionComputation(TickerId ddeId, TickType field, double impliedVol,
	double delta, double optPrice, double pvDividend,
	double gamma, double vega, double theta, double undPrice)
{

}
void MyWrapper::tickGeneric(TickerId tickerId, TickType tickType, double value)
{

}
void MyWrapper::tickString(TickerId tickerId, TickType tickType, const IBString& value)
{

}
void MyWrapper::tickEFP(TickerId tickerId, TickType tickType, double basisPoints,
	const IBString& formattedBasisPoints, double totalDividends, int holdDays,
	const IBString& futureExpiry, double dividendImpact, double dividendsToExpiry)
{

}
void MyWrapper::orderStatus(OrderId orderId, const IBString &status, int filled, int remaining,
	double avgFillPrice, int permId, int parentId, double lastFillPrice,
	int clientId, const IBString& whyHeld)
{

}
void MyWrapper::openOrder(OrderId orderId, const Contract&, const Order&, const OrderState&)
{

}
void MyWrapper::openOrderEnd()
{

}
void MyWrapper::winError(const IBString &str, int lastError)
{
//	info->AddString(_T("one error come"));
	info->AddString((LPCTSTR)str.c_str());
}
void MyWrapper::connectionClosed()
{

}
void MyWrapper::updateAccountValue(const IBString &key, const IBString &val,
	const IBString &currency, const IBString &accountName)
{

}
void MyWrapper::updatePortfolio(const Contract& contract, int position,
	double marketPrice, double marketValue, double averageCost,
	double unrealizedPNL, double realizedPNL, const IBString &accountName)
{

}
void MyWrapper::updateAccountTime(const IBString &timeStamp)
{

}
void MyWrapper::accountDownloadEnd(const IBString &accountName) {}
void MyWrapper::nextValidId(OrderId orderId){}
void MyWrapper::contractDetails(int reqId, const ContractDetails& contractDetails){}
void MyWrapper::bondContractDetails(int reqId, const ContractDetails& contractDetails){}
void MyWrapper::contractDetailsEnd(int reqId){}
void MyWrapper::execDetails(int reqId, const Contract& contract, const Execution& execution){}
void MyWrapper::execDetailsEnd(int reqId){}

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

void MyWrapper::error(const int id, const int errorCode, const IBString errorString)
{

	info->AddString((LPCTSTR)s2ws(errorString).c_str());
}
void MyWrapper::error(const IBString errorString)
{
	info->AddString((LPCTSTR)s2ws(errorString).c_str());
}
void MyWrapper::updateMktDepth(TickerId id, int position, int operation, int side,
	double price, int size){}
void MyWrapper::updateMktDepthL2(TickerId id, int position, IBString marketMaker, int operation,
	int side, double price, int size){}
void MyWrapper::updateNewsBulletin(int msgId, int msgType, const IBString& newsMessage, const IBString& originExch){}
void MyWrapper::managedAccounts(const IBString& accountsList){}
void MyWrapper::receiveFA(faDataType pFaDataType, const IBString& cxml){}
void MyWrapper::historicalData(TickerId reqId, const IBString& date, double open, double high, double low,
	double close, int volume, int barCount, double WAP, int hasGaps){}
void MyWrapper::scannerParameters(const IBString &xml){}
void MyWrapper::scannerData(int reqId, int rank, const ContractDetails &contractDetails, const IBString &distance,
	const IBString &benchmark, const IBString &projection, const IBString &legsStr){}
void MyWrapper::scannerDataEnd(int reqId){}
void MyWrapper::realtimeBar(TickerId reqId, long time, double open, double high, double low, double close,
	long volume, double wap, int count){}
void MyWrapper::currentTime(long time){}
void MyWrapper::fundamentalData(TickerId reqId, const IBString& data){}
void MyWrapper::deltaNeutralValidation(int reqId, const UnderComp& underComp){}
void MyWrapper::tickSnapshotEnd(int reqId){}
void MyWrapper::marketDataType(TickerId reqId, int marketDataType){}
void MyWrapper::commissionReport(const CommissionReport& commissionReport){}
void MyWrapper::position(const IBString& account, const Contract& contract, int position, double avgCost){}
void MyWrapper::positionEnd(){}
void MyWrapper::accountSummary(int reqId, const IBString& account, const IBString& tag, const IBString& value, const IBString& curency){}
void MyWrapper::accountSummaryEnd(int reqId){}
void MyWrapper::verifyMessageAPI(const IBString& apiData){}
void MyWrapper::verifyCompleted(bool isSuccessful, const IBString& errorText){}
void MyWrapper::displayGroupList(int reqId, const IBString& groups){}
void MyWrapper::displayGroupUpdated(int reqId, const IBString& contractInfo){}