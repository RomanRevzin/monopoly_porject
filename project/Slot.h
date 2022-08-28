#pragma once
#include <iostream>
#include <string>
using namespace std;

class Slot
{
	string Name;
	int TileNumber;

public:
	void setName(string name);
	void setTileNumber(int number);
	string getName() const;
	int getTileNumber() const;
	Slot(string name, int number);
	friend ostream& operator<<(ostream& output, Slot& slot);
	virtual void print(ostream& out);
	virtual ~Slot() = 0;
};

