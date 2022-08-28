#include "gameEngine.h"

int main()
{
	gameEngine game;
	
	bool validBoard = true;


	/* Bulds deck, board and players */
	
	game.prepareBoard();
	
	while (game.getPlayers().size() > 1)
	{
		try
		{
			game.turn();
		}
		catch (exception e)
		{
			cout << e.what();
		}
		catch (...)
		{
			cout << "unhandled exception";
		}
		
	}
}