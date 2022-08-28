#pragma once
#include "Slot.h"
class Instruction : public Slot
{
	string Type;

public:
	void setType(string type);

	string getType() const;

	Instruction(string type, string name, int nubmer);
	virtual void print(ostream& out);
	~Instruction();
	
	/* No dynamically allocate memory in the object and it's base class, so default base copy constructor and shallow copy is good enough*/
};

