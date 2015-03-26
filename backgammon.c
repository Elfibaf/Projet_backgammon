#include "backgammon.h"
#include <stdio.h>
#include <string.h>

//////////////////////////////////////////////////////////
// Dans la librairie
//

void InitLibrary(char name[50])
{
	printf("InitLibrary\n");
	strcpy(name,"Fabien Picarougne");
}

void StartMatch(const unsigned int target_score)
{
	printf("StartMatch\n");
}

void StartGame(Player p)
{
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
	printf("PlayTurn\n");
}


//////////////////////////////////////////////////////////
// Dans l'exécutable
//

//void deroulement_du_jeu()	
int main()
{
	//*****// à chaque utilisation de gameState, ne pas oublier de faire une copie de tous les éléments (pas fait ici)
	SGameState gameState;
	SMove moves[4];

	//*****// à faire pour chaque joueur
	
	// Chargement de la librairie (chargement des pointeurs de fonctions des fonctions décrites dans "backgammon.h")
	// -> à ermplacer par votre code de chargement
	pfInitLibrary j1InitLibrary=InitLibrary;
	pfStartMatch j1StartMatch=StartMatch;
	pfStartGame j1StartGame=StartGame;
	pfEndGame j1EndGame=EndGame;
	pfEndMatch j1EndMatch=EndMatch;
	pfDoubleStack j1DoubleStack=DoubleStack;
	pfTakeDouble j1TakeDouble=TakeDouble;
	pfPlayTurn j1PlayTurn=PlayTurn;
	// ...
	pfTakeDouble j2TakeDouble=TakeDouble;

	// Initialisation de la librairie
	char name[50];
	j1InitLibrary(name);
	j1StartMatch(5);

	unsigned int nbMoves;
	unsigned char dices[2];

		//*****// à faire pour chaque jeu
		j1StartGame(BLACK);
			//*****// pour chaque joueur, tant que ce n'est pas fini
			if (j1DoubleStack(&gameState))
				j2TakeDouble(&gameState);
			j1PlayTurn(&gameState,dices,moves,&nbMoves,3);
		j1EndGame();

	j1EndMatch();

	return(0);
}
