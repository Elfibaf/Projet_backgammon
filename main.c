#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dlfcn.h>

#include "arbitrage.h"
#include "bot.h"

#define DEFAULT_GOAL 5
#define J1_NB_TRIES 3
#define J2_NB_TRIES 3

//void deroulement_du_jeu()
int main(int argc, char *argv[])
{
    //initialisation du générateur de nombres aléatoire pour la génération des dés
    srand(time(NULL));
    
    int goal; // Le score a obtenir pour gagner la partie
    
    if (argc == 1)
    {
    	goal = DEFAULT_GOAL; // Si l'utilisateur ne rentre pas de paramètres
    	printf("Vous avez choisi le mode de jeu : HUMAIN VS HUMAIN - Jeu en %d points (parametres par défaut)\n", goal);
    }
    else
    {
    	long res;
    	char *ptr;
    	res = strtol(argv[1], &ptr, 10);
    	goal = (int)res; // On stocke dans goal le nombre de points pour gagner la partie
    	
    	if (argc == 2)
    	{
    		printf("Vous avez choisi le mode de jeu : HUMAIN VS HUMAIN - Jeu en %d points\n", goal);
    	}
    	else if (argc == 3)
    	{
    		printf("Vous avez choisi le mode de jeu : HUMAIN VS IA - Jeu en %d points\n", goal);
    	}
    	else if (argc == 4)
    	{
    		printf("Vous avez choisi le mode de jeu : IA VS IA - Jeu en %d points\n", goal);
    	}
		else
		{
			printf("Vous avez mis beaucoup trop de paramètres !\n");
			printf("./nomDuProgramme [nbPointsPourGagner] [librairie1] [librairie2]\n");
			return EXIT_FAILURE;
		}
	} 

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

	if ( ((j1InitLibrary=(pfInitLibrary)dlsym(lib,"InitLibrary")) == NULL)
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

	if ( ((j2InitLibrary=(pfInitLibrary)dlsym(lib2,"InitLibrary")) == NULL)
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
	
	char name1[50];
	j1InitLibrary(name1);
	j1StartMatch(goal);
	
	char name2[50];
	j2InitLibrary(name2);
	j2StartMatch(goal);

	SGameState gameState, copyGameState;
	InitPlateau(&gameState); // Initialisation du tableau
	
	unsigned char dices[2];

	unsigned int nbMoves;
	SMove moves[4];
	
	unsigned int j1NbTries, j2NbTries; // variables pour gérer le nombre d'essais de chaque joueur
	
	
    while ( (gameState.whiteScore < goal) && (gameState.blackScore < goal) ) // Tant qu'aucun des joueurs n'a gagné le jeu
    {
    	j1StartGame(WHITE);
		j2StartGame(BLACK);
		
		Player player;
    	if (gameState.turn == 0)
    	{
    		// Pour savoir qui commence, on lance deux dés, si le premier est plus petit que le deuxième,
    		// le joueur 1 est le WHITE
    		// sinon c'est le joueur 2
    		do
    		{
    			GenerateDices(dices); // Génération des deux dés
    			if (dices[0] < dices[1])
    			{
    				player = WHITE;
    			}
    			else
    			{
    				player = BLACK;
    			}
    		} while (dices[0] == dices[1]);
    		
    		// On détermine le nombre d'essais possibles
    		j1NbTries = J1_NB_TRIES;
    		j2NbTries = J2_NB_TRIES;
    	}
		
        while ( ! WinGame(&gameState, player) ) // Tant que la partie en cours n'est pas fini
        {
        	if (player == WHITE)
        	{
        	
        	}
        	gameState.turn++; // Mise à jour du nombre de tour de la partie en cours
        	GenerateDices(dices); // Génération des deux dés
        	
        	if (player == WHITE)
        	{
        		if (j1DoubleStack(&gameState))
		        {
		            j2TakeDouble(&gameState);
		        }
		        
		        j1PlayTurn(&copyGameState, dices, moves, &nbMoves, j1NbTries);
		        
		        copyGameState = gameState;		        
		        if (CheckTurn(&copyGameState, dices, moves, nbMoves, player)) // Vérification des coups
		        {
		        	UpdateGameState(gameState, moves, nbMoves, player); // Mise à jour du jeu
		        	if (WinGame(&gameState, WHITE)) // On regarde si le joueur à gagner la partie
				    {
				        gameState.whiteScore++;
				    }
		        }
		        else // Les coups n'étaient pas valides
		        {
		        	j1NbTries--; // On décremente le nombre d'essais restant
		        	if (j1NbTries == 0) // Si le joueur n'a plus d'essais, il perd automatiquement
		        	{
		        		gameState.blackScore++;
		        	}
		        }
		        
		        player = BLACK; // Mise à jour du joueur	
        	}
        	else
        	{
        		if (j2DoubleStack(&gameState))
		        {
		            j1TakeDouble(&gameState);
		        }
		        
		        j2PlayTurn(&gameState, dices, moves, &nbMoves, j2NbTries);
		        
		        copyGameState = gameState;
		        if (CheckTurn(&copyGameState, dices, moves, nbMoves, player)) // Vérification des coups
		        {
		        	UpdateGameState(gameState, moves, nbMoves, player); // Mise à jour du jeu
		        	if (WinGame(&gameState, player)) // On regarde si le joueur à gagner la partie
				    {
				        gameState.blackScore++;
				    }
		        }
		        else // Les coups n'étaient pas valides
		        {
		        	j2NbTries--; // On décremente le nombre d'essais restant
		        	if (j2NbTries == 0) // Si le joueur n'a plus d'essais, il perd automatiquement
		        	{
		        		gameState.whiteScore++;
		        	}
		        }
		        
		        player = WHITE; // Mise à jour du joueur
        	}
            

		}

		j1EndGame();
		j2EndGame();
    }

	j1EndMatch();
	j2EndMatch();

	dlclose(lib);
	dlclose(lib2);
	return(0);
}
