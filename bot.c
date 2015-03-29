//// Fichier pour la stratégie ////
#include "bot.h"
#include <stdio.h>
#include <string.h>

//////////////////////////////////////////////////////////
// Dans la librairie
//

typedef struct {
	Player color;
	unsigned int targetScore;
	
} SBotInfo;

SBotInfo bot;


void InitLibrary(char name[50])
{
	printf("InitLibrary\n");
	strcpy(name,"Ballz");
}

void StartMatch(const unsigned int target_score)
{
	bot.targetScore = target_score;
	printf("StartMatch\n");
}

void StartGame(Player p)
{
	
	bot.color = p;
	
	printf("StartGame\n");
}

void EndGame()
{
	printf("EndGame\n");
}

void EndMatch()
{
	printf("EndMatch\n");
}

int DoubleStack(const SGameState * const gameState)
{
	printf("DoubleStack\n");
	return(0);
}

int TakeDouble(const SGameState * const gameState)
{
	printf("TakeDouble\n");
	return(0);
}

void PlayTurn(const SGameState * const gameState, const unsigned char dices[2], SMove moves[4], unsigned int *nbMove, unsigned int tries)
{
	/*int i;
	for(i=0;i<24;i++)
	{
		if (gameState->board[i].owner == )
	}*/
	
	printf("bot : %d\n",(int)bot.color);
	
	printf("PlayTurn\n");
}
