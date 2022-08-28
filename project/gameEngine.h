#pragma once
#include <istream>
#include <fstream>
#include "Player.h"
#include "Instruction.h"
#include <queue>
#include <time.h>
#include <exception>


#define DECKSIZE 100
#define STARTMONEY 350 //used as a range for deck cards values as well
#define BOARDINFOFILENAME "board.txt"
#define JAIL "Jail"
#define START "Start"
#define TICKET "Ticket"

#define INVALIDNUMOFPLAYERS "Exception thrown: invalid number of players"
#define INVALIDPLAYERNAME   "Exception thrown: invalid player name"

class gameEngine
{
	list <Slot*>board;
	vector <Player*> players;
	queue <int> cards;

protected:
	void buildDeck();
	void buildBoard() throw (exception);
	void addPlayers();
public:
	list     <Slot*>& getBoard();
	vector <Player*>& getPlayers();
	queue      <int>& getDeck();

	int getCard();
	void prepareBoard();
	void turn();
	void retirePlayer(Player* player);

	Slot* move(Player* player, int dice);
	~gameEngine();
};



