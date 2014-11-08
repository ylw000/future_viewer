#include "stdafx.h"
#include "datalistener.h"
#include "itemslot.h"

ItemSlot::ItemSlot(CStatic *bidPrice, CStatic *askPrice)
{
	_bidPrice = bidPrice;
	_askPrice = askPrice;
}

void ItemSlot::price(TickType field, double price)
{
	CString p;
	p.Format(_T("%.3f"), price);
	if (field == BID)
		_bidPrice->SetWindowTextW(p);
	else if (field == ASK)
		_askPrice->SetWindowTextW(p);
}