#include "Instruction.h"

void Instruction::setType(string type)
{
	Type = type;
}

string Instruction::getType() const
{
	return Type;
}

Instruction::Instruction(string type, string name, int number) : Slot(name, number)
{
	setType(type);
}

void Instruction::print(ostream& output)
{
	Slot::print(output);
	output << " Instruction type: " << getType() << endl;
}

Instruction::~Instruction()
{
}


