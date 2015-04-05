#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dlfcn.h>

#include "arbitrage.h"
#include "bot.h"

//void deroulement_du_jeu()
int main()
{
    //initialisation du générateur de nombres aléatoire pour la génération des dés
    srand(time(NULL));

	// Chargement de la librairie (chargement des pointeurs de fonctions des fonctions d�crites dans "backgammon.h")

	void *lib,*lib2;

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

	if ((lib2=dlopen("bot2.so",RTLD_LAZY)) == NULL)
	{
		//Erreur de chargement de la librairie
		return(-1);
	}

	if( ((j1InitLibrary=(pfInitLibrary)dlsym(lib,"InitLibrary")) == NULL)
	|| ((j1StartMatch=(pfStartMatch)dlsym(lib,"StartMatch")) == NULL)
	|| ((j1StartGame=(pfStartGame)dlsym(lib,"StartGame")) == NULL)
	|| ((j1EndGame=(pfEndGame)dlsym(lib,"EndGame")) == NULL)
	|| ((j1EndMatch=(pfEndMatch)dlsym(lib,"EndMatch")) == NULL)
	|| ((j1DoubleStack=(pfDoubleStack)dlsym(lib,"DoubleStack")) == NULL)
	|| ((j1TakeDouble=(pfTakeDouble)dlsym(lib,"TakeDouble")) == NULL)
	|| ((j1PlayTurn=(pfPlayTurn)dlsym(lib,"PlayTurn")) == NULL) )
	{
		return(-1);
	}

	if( ((j2InitLibrary=(pfInitLibrary)dlsym(lib2,"InitLibrary")) == NULL)
	|| ((j2StartMatch=(pfStartMatch)dlsym(lib2,"StartMatch")) == NULL)
	|| ((j2StartGame=(pfStartGame)dlsym(lib2,"StartGame")) == NULL)
	|| ((j2EndGame=(pfEndGame)dlsym(lib2,"EndGame")) == NULL)
	|| ((j2EndMatch=(pfEndMatch)dlsym(lib2,"EndMatch")) == NULL)
	|| ((j2DoubleStack=(pfDoubleStack)dlsym(lib2,"DoubleStack")) == NULL)
	|| ((j2TakeDouble=(pfTakeDouble)dlsym(lib2,"TakeDouble")) == NULL)
	|| ((j2PlayTurn=(pfPlayTurn)dlsym(lib2,"PlayTurn")) == NULL) )
	{
		return(-1);
	}

	// Initialisation de la librairie
	/*
	faire if pour savoir si c'est un bot ou pas
	=======> avec argv et argc
	*/
	
	int goal = 5; // Le score a obtenir pour gagner la partie
	
	char name[50];
	j1InitLibrary(name);
	j1StartMatch(goal);

	SGameState gameState;
	InitPlateau(&gameState); // Initialisation du tableau
	
	unsigned char dices[2];

	unsigned int nbMoves; // Le nombre de coup possibles que peut faire le joueur
	SMove moves[4]; // Tableau de mouvements
	
	unsigned int j1NbTries, j2NbTries; // Initialisation du nombre d'erreurs possibles
	
    // Tant qu'aucun des joueurs n'a gagné le jeu, on continue à faire des parties
    while( (gameState.whiteScore < goal) && (gameState.blackScore < goal) )
    {
        j1StartGame(WHITE);
		j1NbTries = 3;
		
		j2StartGame(BLACK);
		j2NbTries = 3;
		
        // Tant que la partie en cours n'est pas fini
        while(1)
        {
            // Mise à jour du nombre de tour de la partie en cours
            gameState.turn++;

            // Tour du premier joueur
            if(j1DoubleStack(&gameState))
            {
                j2TakeDouble(&gameState);
            }
            GenerateDices(dices); // Génération des deux dés
            j1PlayTurn(&gameState, dices, moves, &nbMoves, &j1NbTries);
            ///////////////////////////////////////////////////////////////////////////IsValid()
            ModifPlateau(&gameState, moves, &nbMoves, WHITE);
            if( WinGame(&gameState, WHITE) )
            {
                gameState.whiteScore++;
                break;
            }
			getchar();
            /*// Tour du deuxième joueur
            if(j2DoubleStack(&gameState))
            {
                j1TakeDouble(&gameState);
            }
            j2PlayTurn(&gameState,dices,moves,&nbMoves,3);
            if( WinGame(&gameState, BLACK) )
            {
                gameState.blackScore++;
                break;
            }*/

		}

		j1EndGame();
    }

	j1EndMatch();

	dlclose(lib);
	dlclose(lib2);
	return(0);
}
