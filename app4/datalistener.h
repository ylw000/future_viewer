#ifndef DATALISTENER_H
#define DATALISTENER_H

#include "EWrapper.h"

class DataListener
{	
public:
	DataListener();

	virtual void price(TickType field, double price);


};

#endif