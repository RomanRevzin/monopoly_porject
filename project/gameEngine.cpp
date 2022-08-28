#include "gameEngine.h"

void gameEngine::buildDeck()
{
	/* Dividing an odd integer will provide even amount of cards*/
	if (DECKSIZE % 2 == 1)
	{
		cards.push(rand() % STARTMONEY);
	}

	for (int i = 0; i < DECKSIZE / 2; i++)
	{
		int number = 0;

		while (number == 0) // zero is not allowed
		{
			number = rand() % STARTMONEY + 1;
		}

		cards.push(number); //positive numbers
		cards.push(number - STARTMONEY); //negative numbers
	}
}

void gameEngine::buildBoard() throw (exception)
{
	ifstream boardInfo(BOARDINFOFILENAME);
	string fileLine, instructionType, slotName, cityName;
	int price, rent, i = 0;

	while(boardInfo.good())
	{
		if (boardInfo)
		{
			getline(boardInfo, fileLine);

			char type = fileLine.at(0);

			if (type == 'I')
			{
				type = fileLine.at(2);

				if (type == 'J' || type == 'S' || type == 'T')
				{
					switch (type)
					{
					case 'J':
						instructionType = JAIL;
						break;

					case 'S':
						instructionType = START;
						break;

					case 'T':
						instructionType = TICKET;
						break;
					}
					slotName = fileLine.substr(4);

					this->board.emplace_back(new Instruction(instructionType, slotName, i + 1));
				}
			}
			else if (type == 'P')
			{
				fileLine.erase(0, 2); // cleans slot type symbol and comma
				for (int i = 0; i < 4; i++)
				{
					size_t pos = fileLine.find(',');
					string temp = fileLine.substr(0, pos);
					switch (i)
					{
					case 0:
						cityName = temp;
						break;
					case 1:
						slotName = temp;
						break;
					case 2:
						price = stoi(temp, nullptr, 10);
						break;
					case 3:
						rent = stoi(temp, nullptr, 10);
						break;
					}
					fileLine.erase(0, pos + 1);
				}
				this->board.emplace_back(new Asset(cityName, slotName, i + 1, price, rent));
			}
		}
	}
	boardInfo.close();
}

list<Slot*>& gameEngine::getBoard()
{
	return board;
}

vector<Player*>& gameEngine::getPlayers()
{
	return players;
}

queue<int>& gameEngine::getDeck()
{
	return cards;
}

int gameEngine::getCard()
{
	int temp = cards.front();
	cards.pop();
	cards.push(temp);
	return temp;
}

void gameEngine::prepareBoard()
{
	this->buildDeck();
	this->buildBoard();
	this->addPlayers();
}

void gameEngine::turn()
{
	char choice;
	int dice;

	for (int i = 0; i < getPlayers().size(); i++)
	{
		if (getPlayers().size() == 1)
		{
			Player* winner = players.front();
			cout << "Congratulations to " << winner->getName() << "!" << endl;
			retirePlayer(winner);
			break;
		}

		Player* player = getPlayers()[i];

		if (player->getIfInJail())
		{
			cout << getPlayers()[i]->getName() << " is in jail this turn!" << endl;
			player->setIfInJail(false);
		}
		else
		{
			cout << getPlayers()[i]->getName() << "'s turn. Do you want to proceed?(Y/N): ";
			cin >> choice;
			if (choice == 'N')
			{
				/* End mortgages, cancel estate ownerhip, empty player's list of estates and free player's memory */
				retirePlayer(player);

				/* Deletes member from vector */
				getPlayers().erase(getPlayers().begin() + i);

				i--; // to ensure player after the deleted will make a turn
			}
			else if (choice == 'Y')
			{
				/* Dice roll and move section */
				cout << "Current:" << endl << *player->getPosition();

				srand(time(NULL)); // just to make rand() work
				dice = (rand() % 6) + 1; // 1 - 6
				player->setPosition(move(player, dice));

				cout << "Moved to:" << endl << *player->getPosition();

				/* Check if the player has passed through the whole board */
				Slot* tile = player->getPosition();


				/* If blocks */

				
				/* INSTRUCTION */
				if (typeid(*tile) == typeid(Instruction))
				{
					/* TICKET */
					if(dynamic_cast<Instruction*>(tile)->getType() == TICKET)
					{
						cout << "Before: " << player->getFunds() << endl;
						int cardValue = getCard();
						if (player->isEnoughMoney(cardValue))
						{
							cout << "Ticket value: " << cardValue << endl;
							cout << "After: " << player->getFunds() << endl;
						}
						else
						{
							cout << player->getName() << " lost of oh his money. Gameover!" << endl;
							retirePlayer(player);
							break;
						}
						
					}
					/* JAIL */
					else if (dynamic_cast<Instruction*>(tile)->getType() == JAIL)
					{
						player->setIfInJail(true);
						cout << player->getName() << " is in jail for one turn!" << endl;
					}
				}
				/* ASSET */
				else if (typeid(*tile) == typeid(Asset))
				{
					/* FREE */
					if (dynamic_cast<Asset*>(tile)->getOwner() == nullptr)
					{
						cout << "Are you interested in buying this asset?(Y/N): ";
						cin >> choice;
						if (choice == 'Y')
						{
							if (player->getFunds() > dynamic_cast<Asset*>(tile)->getPrice())
							{
								player->setFunds(player->getFunds() - dynamic_cast<Asset*>(tile)->getPrice());
								player->addRealEsatate(dynamic_cast<Asset*>(tile));
								dynamic_cast<Asset*>(tile)->setOwner(player);
								cout << "Estate was successfully bought!" << endl;
							}
							else
							{
								cout << "Insufficient funds" << endl;
							}
							
						}
						else if (choice != 'N')
						{
							//exception
						}
					}
					/* If not the owner of the asset */
					else if (dynamic_cast<Asset*>(tile)->getOwner() != player)
					{
						/* MORTGAGED */
						if (dynamic_cast<Asset*>(tile)->getMortgageYears() != 0)
						{
							player->isEnoughMoney(-dynamic_cast<Asset*>(tile)->getRent());
						}
						/* OWNED */
						else
						{
							int rent = dynamic_cast<Asset*>(tile)->getRent();
							player->isEnoughMoney(-rent);
							dynamic_cast<Asset*>(tile)->getOwner()->isEnoughMoney(rent);
						}
					}
				}
			}
			else
			{
				cout << "Wrong input" << endl;
				i--;
			}
		}
	}
}

void gameEngine::retirePlayer(Player* player)
{
	for (auto const& estate : player->getRealEstateList())
	{
		/* Frees every mortgaged player's asset */
		player->endMortgage(estate);

		/* Frees the estate to the game*/
		player->endOwnership(estate);
	}

	/* Empties the list without free pointers' memory*/
	player->getRealEstateList().clear();

	/* Frees memory pointed to */
	delete player;

	player = nullptr;

}


Slot* gameEngine::move(Player* player, int dice)
{
	Slot* newPosition = nullptr;
	/*
	since there is no circulators and cyclic containers in STL
	and Boost or CLAG solutions are kinda complex,
	here is a workaround with list iterator,
	which makes the whole thing "cyclic"
	*/
	for (list<Slot*>::iterator it = board.begin(); it!=board.end(); ++it)
	{
		Slot* tile = player->getPosition();

		if (*it == tile)
		{
			for (int i = 0; i < dice; i++)
			{
				if (next(it) == board.end())
				{
					it = board.begin();

					player->isEnoughMoney(STARTMONEY);

					for (Asset* estate : player->getRealEstateList())
					{
						if (estate->getMortgageYears() != 0)
						{
							/* Increase mortgage years by one*/
							estate->setMortgageYears(estate->getMortgageYears() + 1);
						}
					}
				}
				else
				{
					advance(it, 1);
				}
			}
			newPosition = *it;
			break;
		}
	}
	return newPosition;
}

void gameEngine::addPlayers()
{
	string name;
	int num;
	bool validname;

	cout << "How many players are in game?: ";
	cin >> num;
	

	for (int i = 0; i < num; i++)
	{
		validname = false;

		while (!validname)
		{
			try
			{
				cout << "Enter player number " << i + 1 << " name: ";
				cin >> name;
				
				if (find_if(name.begin(), name.end(), (int(*)(int))isdigit) != name.end())
				{
					throw runtime_error(INVALIDPLAYERNAME); // contains digit
				}
				else 
				{
					validname = true;
					players.push_back(new Player(name, STARTMONEY, board.front()));
				}
			}
			catch (const exception e)
			{
				cout << e.what() << endl;
			}
		}
	}
}

gameEngine::~gameEngine()
{
	
	for (list<Slot*>::iterator s = board.begin(); s != board.end(); ++s) 
	{
		delete *s;
	}

}
