#pragma once
#include "Slot.h"
#include "Player.h"

class Player;

class Asset : public Slot
{
	string City;
	int Price;
	int Rent;
	Player* Owner;
	int MortgageYears; // 0 means there is no mortgage

public:
	void setCity(string city);
	void setPrice(int price);
	void setRent(int rent);
	void setOwner(Player* owner);
	void setMortgageYears(int mortgage);

	string  getCity() const;
	int     getPrice() const;
	int     getRent() const;
	Player* getOwner() const;
	int     getMortgageYears() const;

	Asset(string city, string property, int number, int price, int rent, Player* owner = nullptr, int mortgage = 0);
	virtual void print(ostream& out);
	~Asset();

	

	/* No dynamically allocate memory in the object and it's base class, so default base copy constructor and shallow copy is good enough*/
};

