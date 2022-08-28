#include "Asset.h"
#include "Player.h"

void Asset::setCity(string city)
{
	City = city;
}

void Asset::setPrice(int price)
{
	Price = price;
}

void Asset::setRent(int rent)
{
	Rent = rent;
}

void Asset::setOwner(Player* owner)
{
	Owner = owner;
}

void Asset::setMortgageYears(int mortgage)
{
	MortgageYears = mortgage;
}

string Asset::getCity() const
{
	return City;
}

int Asset::getPrice() const
{
	return Price;
}

int Asset::getRent() const
{
	return Rent;
}

Player* Asset::getOwner() const
{
	return Owner;
}

int Asset::getMortgageYears() const
{
	return MortgageYears;
}

Asset::Asset(string city, string property, int number, int price, int rent, Player* owner, int mortgage) : Slot(property, number)
{
	setCity(city);
	setPrice(price);
	setRent(rent);
	setOwner(owner);
	setMortgageYears(mortgage);
}

void Asset::print(ostream& output)
{
	Slot::print(output);

	output << " Property group: " << getCity() << " Owner: ";

	if (getOwner())
	{
		output << getOwner()->getName();
	}
	else
	{
		output << "none";
	}
	output << endl;

}

Asset::~Asset()
{
	/* In case in the end some Player* was not deleted */
	delete Owner;
}
