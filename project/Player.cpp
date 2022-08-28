#include "Player.h"

void Player::setName(string name)
{
	Name = name;
}

void Player::setFunds(int funds)
{
	Funds = funds;
}

void Player::setPosition(Slot* position)
{
	Position = position;
}

void Player::addRealEsatate(Asset* newRealEstate)
{
	RealEstate.push_back(newRealEstate);
}

void Player::setIfInJail(bool jail)
{
	inJail = jail;
}

string Player::getName() const
{
	return Name;
}

int Player::getFunds() const
{
	return Funds;
}

Slot* Player::getPosition() const
{
	return Position;
}

list<Asset*>& Player::getRealEstateList()
{
	return RealEstate;
}

Asset* Player::extructRealEstate()
{
	Asset* temp = RealEstate.front(); // saves the pointer to the first element in list
	RealEstate.pop_front(); // deletes the first element in list
	return temp;
}

bool Player::getIfInJail()
{
	return inJail;
}

Player::Player(string name, int funds, Slot* position, bool jail)
{
	setName(name);
	setFunds(funds);
	setPosition(position);
	setIfInJail(jail);
}

bool Player::isEnoughMoney(int sum) // since card value cannot be equal to zero, no need to throw an exception
{
	bool isEnough = true;

	if (sum > 0)
	{
		setFunds(Funds + sum);

		if (!RealEstate.empty())
		{
			for (Asset* estate : RealEstate) // creates iterator for real RealEstate list
			{
				if (estate->getMortgageYears())
				{
					if (calculateBuyOut(estate) <= Funds)
					{
						endMortgage(estate);
						
					}
				}
			}
		}

	}
	else
	{
		if (-sum < Funds)
		{
			setFunds(Funds + sum);
		}
		else
		{
			isEnough = false;

			for (Asset* estate : RealEstate)
			{

				if (!estate->getMortgageYears()) // checks if the asset is mortgaged
				{
					addMortgage(estate);

					if (-sum < Funds) // once it's true, ends the loop and return true
					{
						setFunds(Funds + sum);
						isEnough = true;
						break;
					}
				}
			}
		}
		
	}// once the return is false it's gameover. no need to add sum to funds

	return isEnough;
}

int Player::calculateBuyOut(Asset* estate) const
{
	return estate->getPrice() * estate->getMortgageYears() * INTEREST;
}

void Player::endMortgage(Asset* estate)
{
	if (estate->getMortgageYears() != 0)
	{
		estate->setMortgageYears(0);
		setFunds(Funds - calculateBuyOut(estate));
	}
}

void Player::addMortgage(Asset* estate)
{
	estate->setMortgageYears(1);
	setFunds(Funds + estate->getPrice());
}

void Player::endOwnership(Asset* estate)
{
	estate->setOwner(nullptr);
}


