//// Fichier pour l'interface ////
#include "bot.h"
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

//void deroulement_du_jeu()	
int main()
{
	//*****// � chaque utilisation de gameState, ne pas oublier de faire une copie de tous les �l�ments (pas fait ici)
	SGameState gameState;
	SMove moves[4];

	//*****// � faire pour chaque joueur
	
	// Chargement de la librairie (chargement des pointeurs de fonctions des fonctions d�crites dans "backgammon.h")
	
	void *lib;
	
	
	pfInitLibrary j1InitLibrary;
	pfStartMatch j1StartMatch;
	pfStartGame j1StartGame;
	pfEndGame j1EndGame;
	pfEndMatch j1EndMatch;
	pfDoubleStack j1DoubleStack;
	pfTakeDouble j1TakeDouble;
	pfPlayTurn j1PlayTurn;
	pfInitLibrary j2InitLibrary;
	pfStartMatch j2StartMatch;
	pfStartGame j2StartGame;
	pfEndGame j2EndGame;
	pfEndMatch j2EndMatch;
	pfDoubleStack j2DoubleStack;
	pfTakeDouble j2TakeDouble;
	pfPlayTurn j2PlayTurn;
	
	if ((lib=dlopen("bot.so",RTLD_LAZY)) == NULL)
	{
		//Erreur de chargement de la librairie
		return(-1);
	}
	
	if ((j1InitLibrary=(pfInitLibrary)dlsym(lib,"InitLibrary")) == NULL)
	{
		return(-1);
	}
	if ((j1StartMatch=(pfStartMatch)dlsym(lib,"StartMatch")) == NULL)
	{
		return(-1);
	}
	if ((j1StartGame=(pfStartGame)dlsym(lib,"StartGame")) == NULL)
	{
		return(-1);
	}
	
	if ((j1EndGame=(pfEndGame)dlsym(lib,"EndGame")) == NULL)
	{
		return(-1);
	}
	if ((j1EndMatch=(pfEndMatch)dlsym(lib,"EndMatch")) == NULL)
	{
		return(-1);
	}
	if ((j1DoubleStack=(pfDoubleStack)dlsym(lib,"DoubleStack")) == NULL)
	{
		return(-1);
	}
	if ((j1TakeDouble=(pfTakeDouble)dlsym(lib,"TakeDouble")) == NULL)
	{
		return(-1);
	}
	if ((j1PlayTurn=(pfPlayTurn)dlsym(lib,"PlayTurn")) == NULL)
	{
		return(-1);
	}
	if ((j2InitLibrary=(pfInitLibrary)dlsym(lib,"InitLibrary")) == NULL)
	{
		return(-1);
	}
	if ((j2StartMatch=(pfStartMatch)dlsym(lib,"StartMatch")) == NULL)
	{
		return(-1);
	}
	if ((j2StartGame=(pfStartGame)dlsym(lib,"StartGame")) == NULL)
	{
		return(-1);
	}
	if ((j2EndGame=(pfEndGame)dlsym(lib,"EndGame")) == NULL)
	{
		return(-1);
	}
	if ((j2EndMatch=(pfEndMatch)dlsym(lib,"EndMatch")) == NULL)
	{
		return(-1);
	}
	if ((j2DoubleStack=(pfDoubleStack)dlsym(lib,"DoubleStack")) == NULL)
	{
		return(-1);
	}
	if ((j2TakeDouble=(pfTakeDouble)dlsym(lib,"TakeDouble")) == NULL)
	{
		return(-1);
	}
	if ((j2PlayTurn=(pfPlayTurn)dlsym(lib,"PlayTurn")) == NULL)
	{
		return(-1);
	}
	
	// Initialisation de la librairie
	char name[50];
	j1InitLibrary(name);
	j1StartMatch(5);
	
	unsigned int nbMoves;
	unsigned char dices[2];

		//*****// � faire pour chaque jeu
		j1StartGame(BLACK);
			//*****// pour chaque joueur, tant que ce n'est pas fini
			if (j1DoubleStack(&gameState))
				j2TakeDouble(&gameState);
			j1PlayTurn(&gameState,dices,moves,&nbMoves,3);
		j1EndGame();

	j1EndMatch();
	
	dlclose(lib);

	return(0);
}