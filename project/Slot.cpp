#include "Slot.h"

void Slot::setName(string name)
{
	Name = name;
}

void Slot::setTileNumber(int number)
{
	TileNumber = number;
}

string Slot::getName() const
{
	return Name;
}

int Slot::getTileNumber() const
{
	return TileNumber;
}

Slot::Slot(string name, int number)
{
	setName(name);
	setTileNumber(number);
}

Slot::~Slot()
{

}

ostream& operator<<(ostream& output, Slot& slot)
{
	slot.print(output);
	return output;
}

void Slot::print(ostream& out)
{
	out << "Tile number: " << getTileNumber() << " Tile name: " << getName();
}