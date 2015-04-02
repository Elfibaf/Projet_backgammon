#include "bot.h"
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <time.h>

void initPlateau(SGameState gameState);
char* generateDices();
int verifCoup(const SGameState * const gameState, const unsigned char dices[2], SMove moves[4], unsigned int *nbMove, unsigned int tries);

//void deroulement_du_jeu()
int main()
{
    //initialisation du g�n�rateur de nombres al�atoire pour la g�n�ration des d�s
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

	if ((lib2=dlopen("bot2.so",RTLD_LAZY)) == NULL)
	{
		//Erreur de chargement de la librairie
		return(-1);
	}

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
	if ((j2InitLibrary=(pfInitLibrary)dlsym(lib2,"InitLibrary")) == NULL)
	{
		return(-1);
	}
	if ((j2StartMatch=(pfStartMatch)dlsym(lib2,"StartMatch")) == NULL)
	{
		return(-1);
	}
	if ((j2StartGame=(pfStartGame)dlsym(lib2,"StartGame")) == NULL)
	{
		return(-1);
	}
	if ((j2EndGame=(pfEndGame)dlsym(lib2,"EndGame")) == NULL)
	{
		return(-1);
	}
	if ((j2EndMatch=(pfEndMatch)dlsym(lib2,"EndMatch")) == NULL)
	{
		return(-1);
	}
	if ((j2DoubleStack=(pfDoubleStack)dlsym(lib2,"DoubleStack")) == NULL)
	{
		return(-1);
	}
	if ((j2TakeDouble=(pfTakeDouble)dlsym(lib2,"TakeDouble")) == NULL)
	{
		return(-1);
	}
	if ((j2PlayTurn=(pfPlayTurn)dlsym(lib2,"PlayTurn")) == NULL)
	{
		return(-1);
	}

	// Initialisation de la librairie
	/*
	faire if pour savoir si c'est un bot ou pas
	=======> avec argv et argc
	*/
	char name[50];
	j1InitLibrary(name);
	j1StartMatch(5);

	//*****// A chaque utilisation de gameState, ne pas oublier de faire une copie de tous les �l�ments (pas fait ici)
	SGameState gameState;
    initPlateau(SGameState gameState) // Initialisation du tableau

    char dices[2] = generateDices(); // G�n�ration des deux d�s

	SMove moves[4];

	unsigned int nbMoves;
	unsigned char dices[2]={1,6};

		//*****// A faire pour chaque jeu
		j1StartGame(BLACK);
		j2StartGame(WHITE);
			//*****// pour chaque joueur, tant que ce n'est pas fini
			if (j1DoubleStack(&gameState))
				j2TakeDouble(&gameState);

			j1PlayTurn(&gameState,dices,moves,&nbMoves,3);
		j1EndGame();

	j1EndMatch();

	dlclose(lib);
	dlclose(lib2);
	return(0);
}

/*
Fonction d'initialisation du tableau
Prend en param�tre un SGameState
*/
void initPlateau(SGameState gameState)
{
    // Initialisation du board
    int i;
    for( i = 0; i < 24; i++ )
    {
        switch(i)
        {
        case 0 :
            gameState.board[i].owner = WHITE;
            gameState.board[i].nbDames = 2;
            break;
        case 5 :
            gameState.board[i].owner = BLACK;
            gameState.board[i].nbDames = 5;
            break;
        case 7 :
            gameState.board[i].owner = BLACK;
            gameState.board[i].nbDames = 3;
            break;
        case 11 :
            gameState.board[i].owner = WHITE;
            gameState.board[i].nbDames = 5;
            break;
        case 12 :
            gameState.board[i].owner = BLACK;
            gameState.board[i].nbDames = 5;
            break;
        case 16 :
            gameState.board[i].owner = WHITE;
            gameState.board[i].nbDames = 3;
            break;
        case 18 :
            gameState.board[i].owner = WHITE;
            gameState.board[i].nbDames = 5;
            break;
        case 23 :
            gameState.board[i].owner = BLACK;
            gameState.board[i].nbDames = 2;
            break;
        default :
            gameState.board[i].owner = NOBODY;
            gameState.board[i].nbDames = 0;
            break;
        }
    }

    // Initialisation du bar
    gameState.bar[WHITE] = 0;
    gameState.bar[BLACK] = 0;

    // Initialisation du out
    gameState.out[WHITE] = 0;
    gameState.out[BLACK] = 0;

    // Initialisation des scores des deux joueurs
    gameState.whiteScore = 0;
    gameState.blackScore = 0;

    // Initialisation du nombre de tours
    gameState.turn = 0;

    // Initialisation de la mise
    gameState.stake = 1;
}


/*
Simulation d'un lanc� de deux d�s
*/
char* generateDices()
{
    char[2] dices;
    dices[0] = (char)rand()%(6);
    dices[1] = (char)rand()%(6);
    return dices;
}

int verifCoup(const SGameState * const gameState, const unsigned char dices[2], SMove moves[4], unsigned int *nbMove, unsigned int tries)
{
    // Je vais faire la fonction tal
    return 0;
}
