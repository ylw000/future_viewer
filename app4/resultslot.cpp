#include "stdafx.h"
#include "resultslot.h"
#include <mmsystem.h>

double ResultSlot::exchange = 6.1402;
double ResultSlot::multiplier = 1;

ResultSlot::ResultSlot(Capp4Dlg *dlg)
{
	this->dlg = dlg;
	isAlarm = false;
}


void ResultSlot::price(TickType, double)
{
	CString str;
//	str.Format(_T("echange: %f"), exchange);
//	((CListBox *)dlg->GetDlgItem(IDC_LIST1))->AddString(str);
	update1();
	update2();
}


void ResultSlot::setExchange(double ex)
{
	exchange = ex;
}

double ResultSlot::getExchange()
{
	return exchange;
}

void ResultSlot::setMultiplier(double val)
{
	multiplier = val;
}

double ResultSlot::getMultiplier()
{
	return multiplier;
}

void ResultSlot::setAlarm(bool x)
{
	isAlarm = x;
}

void ResultSlot::update1()
{
	CString str;
	((CStatic *)dlg->GetDlgItem(IDC_I2B1))->GetWindowTextW(str);
	double ny_bidprice = _ttof(str);
	((CStatic *)dlg->GetDlgItem(IDC_I1A1))->GetWindowTextW(str);
	double sh_askprice = _ttof(str);

	double ex_rate = exchange;

	if (sh_askprice != -1 && ny_bidprice != -1)
	{
		double result = sh_askprice - ny_bidprice * ex_rate * multiplier/ 31.1035;
		str.Format(_T("%.3f"), result);
		((CStatic *)dlg->GetDlgItem(IDC_RESULT1))->SetWindowTextW(str);

		((CStatic *)dlg->GetDlgItem(IDC_LIMIT1))->GetWindowTextW(str);
		double limit = _ttof(str);
		if (isAlarm && limit != -1 && result <= limit)
			PlaySound(_T("alarm.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	else
		((CStatic *)dlg->GetDlgItem(IDC_RESULT1))->SetWindowTextW(_T("-1"));
}

void ResultSlot::update2()
{
	CString str;
	((CStatic *)dlg->GetDlgItem(IDC_I2A1))->GetWindowTextW(str);
	double ny_askrice = _ttof(str);
	((CStatic *)dlg->GetDlgItem(IDC_I1B1))->GetWindowTextW(str);
	double sh_bidprice = _ttof(str);

	double ex_rate = exchange;

	if (sh_bidprice != -1 && ny_askrice != -1)
	{
		double result = sh_bidprice - ny_askrice * ex_rate * multiplier / 31.1035;
		str.Format(_T("%.3f"), result);
		((CStatic *)dlg->GetDlgItem(IDC_RESULT2))->SetWindowTextW(str);
		((CStatic *)dlg->GetDlgItem(IDC_LIMIT2))->GetWindowTextW(str);
		double limit = _ttof(str);
		if (isAlarm && limit != -1 && result >= limit)
			PlaySound(_T("alarm.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	else
		((CStatic *)dlg->GetDlgItem(IDC_RESULT2))->SetWindowTextW(_T("-1"));
}

void ResultSlot::reset()
{
	((CStatic *)dlg->GetDlgItem(IDC_RESULT1))->SetWindowTextW(_T("-1"));
	((CStatic *)dlg->GetDlgItem(IDC_RESULT2))->SetWindowTextW(_T("-1"));
}