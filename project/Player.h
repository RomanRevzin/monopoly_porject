#pragma once

#define INTEREST 1.25

#include "Asset.h"
#include <list>


class Asset;

class Player
{
	string Name;
	int Funds;
	Slot* Position;
	list <Asset*> RealEstate;
	bool inJail;
	
public:
	void setName(string name);
	void setFunds(int funds);
	void setPosition(Slot* position);//????
	void addRealEsatate(Asset* newRealEstate);
	void setIfInJail(bool jail);

	string getName() const;
	int    getFunds() const;
	Slot*  getPosition() const;
	list <Asset*>& getRealEstateList();
	Asset* extructRealEstate(); // cannot be const due to stack change
	bool getIfInJail();

	Player(string name, int funds, Slot* position, bool jail = false);

	bool isEnoughMoney(int sum);
	int calculateBuyOut(Asset* estate) const; // return buyout price of mortgaged asset
	void endMortgage(Asset* estate);
	void addMortgage(Asset* estate);
	void endOwnership(Asset* estate);
	
};

