#ifndef ITEMSLOT_H
#define ITEMSLOT_H

#include "stdafx.h"
#include "datalistener.h"

class ItemSlot: public DataListener
{
public:
	ItemSlot(CStatic *bidPrice, CStatic *askPrice);
	virtual void price(TickType field, double price);
private:
	CStatic *_bidPrice, *_askPrice;

};

#endif