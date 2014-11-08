#ifndef RESULTSLOT_H
#define RESULTSLOT_H

#include "stdafx.h"
#include "datalistener.h"
#include "app4Dlg.h"

class ResultSlot: public DataListener
{
public:
	ResultSlot(Capp4Dlg *dlg);
	virtual void price(TickType, double);
	void update1();
	void update2();

	bool isAlarm;
	void  setAlarm(bool x);
	static void setExchange(double ex);

	static double getExchange();
	static void setMultiplier(double val);
	static double getMultiplier();
	void reset();
private:
	Capp4Dlg *dlg;
	static double exchange;
	static double multiplier;
};

#endif