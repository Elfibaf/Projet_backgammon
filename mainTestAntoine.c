#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dlfcn.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "arbitrage.h"
#include "bot.h"

#define DEFAULT_GOAL 5
#define J1_NB_TRIES 3
#define J2_NB_TRIES 3


//****************************
//
//Fichier pour le test du bot 
//
//****************************


void setBoardTokens(const SGameState * const state, SDL_Rect noirs[15] , SDL_Rect rouges[15], SDL_Renderer *renderer, SDL_Texture *texture_jeton_noir, SDL_Texture *texture_jeton_rouge)
{
    //Tableau d'equivalence entre les cases et leurs positions en pixels
    int equivalence_x[12] = {1080, 1022, 962, 902, 842, 722, 558, 500, 385, 327, 270, 212};
    
    //Tableau d'equivalence entre la position sur une case et la position en pixel correspondante
    int equivalence_y[5] = {645, 581, 517, 453, 389};
    int equivalence_y2[5] = {50, 114, 178, 242, 306};
    
    int i = 0, j = 0, cptN = 0, cptR = 0;
    
    //Parcours du SGameState
    for (i = 0; i<24; i++)
    {
        if (state->board[i].owner == 0)
        {
            for (j = 0; j < state->board[i].nbDames; j++)
            {
				if (i<12) noirs[cptN].x = equivalence_x[i];
	            else noirs[cptN].x = equivalence_x[24-(i+1)];
                
                if(i<12) noirs[cptN].y = equivalence_y[j];
                else noirs[cptN].y = equivalence_y2[j];

                cptN++;
            }
        }
        else
        {
            for (j = 0; j<state->board[i].nbDames; j++)
            {
				if(i<12) rouges[cptR].x = equivalence_x[i];
				else rouges[cptR].x = equivalence_x[24-(i+1)];
                
                if(i<12) rouges[cptR].y = equivalence_y[j];
				else rouges[cptR].y = equivalence_y2[j];

                cptR ++;
            }
        }
    }
}

void afficher(SDL_Surface *surfPlateau, SDL_Surface *surfJetonNoir, SDL_Surface *surfJetonBlanc, SDL_Rect noirs[15] , SDL_Rect blancs[15], SDL_Rect *rectPlateau, SDL_Surface *screen)
{
	int i = 0, j = 0;

	//Afficher un ecran noir
	SDL_FillRect(surfPlateau, NULL, SDL_MapRGB(surfPlateau->format, 0,0,0));

	//Affichage du plateau 
	SDL_BlitSurface(surfPlateau, 0, screen, rectPlateau);

	//Parcours du tableau de jetons noirs et affichage
	for (i=0; i<15;i++)
	{
		SDL_BlitSurface(surfJetonNoir, 0, screen, &noirs[i]);
	}

	//Parcours du tableau de jetons blancs et affichage
	for (j=0; i<15;i++)
	{
		SDL_BlitSurface(surfJetonBlanc, 0, screen, &blancs[j]);
	}
}


//void deroulement_du_jeu()
int main(int argc, char *argv[])
{
	/* ============****** VARIABLES ******============ */
	int continuer = 1;
	SDL_Event event;

	SDL_Renderer * renderer;
	SDL_Window* pWindow = NULL;
	
	SDL_Surface * surfPlateau;
	SDL_Surface * surfJetonNoir;
	SDL_Surface * surfJetonBlanc;
	SDL_Surface * screen;

	SDL_Texture * texture;
	SDL_Texture * texture_jeton_noir;
	SDL_Texture * texture_jeton_rouge;

	
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
	
	char name[50];
	j1InitLibrary(name);
	j1StartMatch(goal);

	SGameState gameState, copyGameState;
	InitPlateau(&gameState); // Initialisation du tableau
	
	unsigned char dices[2];

	unsigned int nbMoves; // Le nombre de coup possibles que peut faire le joueur
	SMove moves[4]; // Tableau de mouvements
	
	unsigned int j1NbTries, j2NbTries; // variables pour gérer le nombre d'essais de chaque joueur
	
	/* ============****** INITIALISATION  DE LA FENETRE ******============ */
	if (SDL_Init(SDL_INIT_VIDEO) != 0 )
	{
		fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
	}

	/* ============****** CREATION DE LA FENETRE ******============ */
	pWindow = SDL_CreateWindow("Backgammon",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, 1360, 760, SDL_WINDOW_SHOWN);
	if (pWindow == NULL) SDL_ShowSimpleMessageBox(0, "Window init error", SDL_GetError(), pWindow);
	screen = SDL_GetWindowSurface(pWindow);

	/* ============****** GESTION DES IMAGES ******============ */
	IMG_Init(IMG_INIT_PNG);
	
	//renderer = SDL_CreateRenderer(pWindow, -1, 0);
	//if (renderer == NULL) SDL_ShowSimpleMessageBox(0, "Renderer init error", SDL_GetError(), pWindow);
	

	surfPlateau = SDL_LoadBMP("plateau.bmp");
	surfJetonNoir = IMG_Load("noir.png");
	surfJetonBlanc = IMG_Load("blanc.png");
	//jeton_noir = SDL_LoadBMP("jeton_noir.bmp");
	//jeton_rouge = SDL_LoadBMP("jeton_rouge.bmp");

	//if (image == NULL) SDL_ShowSimpleMessageBox(0, "Image init error", SDL_GetError(), pWindow);
	//if (jeton_noir == NULL) SDL_ShowSimpleMessageBox(0, "Image init error", SDL_GetError(), pWindow);
	//if (jeton_rouge == NULL) SDL_ShowSimpleMessageBox(0, "Image init error", SDL_GetError(), pWindow);


	//Definition de la texture et verification de son existence
	//texture = SDL_CreateTextureFromSurface(renderer, image);
	//texture_jeton_noir = SDL_CreateTextureFromSurface(renderer, jeton_noir);
	//texture_jeton_rouge = SDL_CreateTextureFromSurface(renderer, jeton_rouge);

	//if (texture == NULL) SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), pWindow);
	//if (texture_jeton_noir == NULL) SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), pWindow);
	//if (texture_jeton_rouge == NULL) SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), pWindow);

	/* ============****** RENDU DES JETONS (SITUATION INITIALE) ******============ */
	//SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_Rect rectPlateau = {0, 0, 1360, 760};

	SDL_Rect b1 = { 1080, 645, 64, 64 }; //Jan 1
	SDL_Rect b2 = { 1080, 581, 64, 64 }; //Jan 1
	SDL_Rect b3 = { 212, 645, 64, 64 }; //Jan 12
	SDL_Rect b4 = { 212, 581, 64, 64 }; //Jan 12
	SDL_Rect b5 = { 212, 517, 64, 64 }; //Jan 12
	SDL_Rect b6 = { 212, 453, 64, 64 }; //Jan 12
	SDL_Rect b7 = { 212, 389, 64, 64 }; //Jan 12
	SDL_Rect b8 = { 500, 50, 64, 64 }; //Jan 17
	SDL_Rect b9 = { 500, 114, 64, 64 }; //Jan 17
	SDL_Rect b10 = { 500, 178, 64, 64 }; //Jan 17
	SDL_Rect b11 = { 722, 50, 64, 64 }; //Jan 19
	SDL_Rect b12 = { 722, 114, 64, 64 }; //Jan 19
	SDL_Rect b13 = { 722, 178, 64, 64 }; //Jan 19
	SDL_Rect b14 = { 722, 242, 64, 64 }; //Jan 19
	SDL_Rect b15 = { 722, 306, 64, 64 }; //Jan 19

	SDL_Rect blancs[15] = {b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15};

	SDL_Rect n1 = { 722, 645, 64, 64 }; //Jan 6
	SDL_Rect n2 = { 722, 581, 64, 64 }; //Jan 6
	SDL_Rect n3 = { 722, 517, 64, 64 }; //Jan 6
	SDL_Rect n4 = { 722, 453, 64, 64 }; //Jan 6
	SDL_Rect n5 = { 722, 389, 64, 64 }; //Jan 6
	SDL_Rect n6 = { 500, 645, 64, 64 }; //Jan 8
	SDL_Rect n7 = { 500, 581, 64, 64 }; //Jan 8
	SDL_Rect n8 = { 500, 517, 64, 64 }; //Jan 8
	SDL_Rect n9 = { 212, 50, 64, 64 }; //Jan 13
	SDL_Rect n10 = { 212, 114, 64, 64 }; //Jan 13
	SDL_Rect n11 = { 212, 178, 64, 64 }; //Jan 13
	SDL_Rect n12 = { 212, 242, 64, 64 }; //Jan 13
	SDL_Rect n13 = { 212, 306, 64, 64 }; //Jan 13
	SDL_Rect n14 = { 1080, 50, 64, 64 }; //Jan 24
	SDL_Rect n15 = { 1080, 114, 64, 64 }; //Jan 24

	SDL_Rect noirs[15] = {n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14, n15};
	
	SDL_BlitSurface(surfPlateau, 0, screen, &rectPlateau);

	SDL_BlitSurface(surfJetonNoir, NULL, screen, &n1);
	SDL_BlitSurface(surfJetonNoir, NULL, screen, &n2);
	SDL_BlitSurface(surfJetonNoir, NULL, screen, &n3);
	SDL_BlitSurface(surfJetonNoir, NULL, screen, &n4);
	SDL_BlitSurface(surfJetonNoir, NULL, screen, &n5);
	SDL_BlitSurface(surfJetonNoir, NULL, screen, &n6);
	SDL_BlitSurface(surfJetonNoir, NULL, screen, &n7);
	SDL_BlitSurface(surfJetonNoir, NULL, screen, &n8);
	SDL_BlitSurface(surfJetonNoir, NULL, screen, &n9);
	SDL_BlitSurface(surfJetonNoir, NULL, screen, &n10);
	SDL_BlitSurface(surfJetonNoir, NULL, screen, &n11);
	SDL_BlitSurface(surfJetonNoir, NULL, screen, &n12);
	SDL_BlitSurface(surfJetonNoir, NULL, screen, &n13);
	SDL_BlitSurface(surfJetonNoir, NULL, screen, &n14);
	SDL_BlitSurface(surfJetonNoir, NULL, screen, &n15);
	
	SDL_BlitSurface(surfJetonBlanc, NULL, screen, &b1);
	SDL_BlitSurface(surfJetonBlanc, NULL, screen, &b2);
	SDL_BlitSurface(surfJetonBlanc, NULL, screen, &b2);
	SDL_BlitSurface(surfJetonBlanc, NULL, screen, &b3);
	SDL_BlitSurface(surfJetonBlanc, NULL, screen, &b4);
	SDL_BlitSurface(surfJetonBlanc, NULL, screen, &b5);
	SDL_BlitSurface(surfJetonBlanc, NULL, screen, &b6);
	SDL_BlitSurface(surfJetonBlanc, NULL, screen, &b7);
	SDL_BlitSurface(surfJetonBlanc, NULL, screen, &b8);
	SDL_BlitSurface(surfJetonBlanc, NULL, screen, &b9);
	SDL_BlitSurface(surfJetonBlanc, NULL, screen, &b10);
	SDL_BlitSurface(surfJetonBlanc, NULL, screen, &b11);
	SDL_BlitSurface(surfJetonBlanc, NULL, screen, &b12);
	SDL_BlitSurface(surfJetonBlanc, NULL, screen, &b13);
	SDL_BlitSurface(surfJetonBlanc, NULL, screen, &b14);
	SDL_BlitSurface(surfJetonBlanc, NULL, screen, &b15);
	
	SDL_UpdateWindowSurface(pWindow);

	//Jetons Noirs
/*
	SDL_RenderCopy(renderer, texture_jeton_noir, NULL, &n1);
	SDL_RenderCopy(renderer, texture_jeton_noir, NULL, &n2);
	SDL_RenderCopy(renderer, texture_jeton_noir, NULL, &n3);
	SDL_RenderCopy(renderer, texture_jeton_noir, NULL, &n4);
	SDL_RenderCopy(renderer, texture_jeton_noir, NULL, &n5);
	SDL_RenderCopy(renderer, texture_jeton_noir, NULL, &n6);
	SDL_RenderCopy(renderer, texture_jeton_noir, NULL, &n7);
	SDL_RenderCopy(renderer, texture_jeton_noir, NULL, &n8);
	SDL_RenderCopy(renderer, texture_jeton_noir, NULL, &n9);
	SDL_RenderCopy(renderer, texture_jeton_noir, NULL, &n10);
	SDL_RenderCopy(renderer, texture_jeton_noir, NULL, &n11);
	SDL_RenderCopy(renderer, texture_jeton_noir, NULL, &n12);
	SDL_RenderCopy(renderer, texture_jeton_noir, NULL, &n13);
	SDL_RenderCopy(renderer, texture_jeton_noir, NULL, &n14);
	SDL_RenderCopy(renderer, texture_jeton_noir, NULL, &n15);

	//Jetons rouges
	SDL_RenderCopy(renderer, texture_jeton_rouge, NULL, &r1);
	SDL_RenderCopy(renderer, texture_jeton_rouge, NULL, &r2);
	SDL_RenderCopy(renderer, texture_jeton_rouge, NULL, &r3);
	SDL_RenderCopy(renderer, texture_jeton_rouge, NULL, &r4);
	SDL_RenderCopy(renderer, texture_jeton_rouge, NULL, &r5);
	SDL_RenderCopy(renderer, texture_jeton_rouge, NULL, &r6);
	SDL_RenderCopy(renderer, texture_jeton_rouge, NULL, &r7);
	SDL_RenderCopy(renderer, texture_jeton_rouge, NULL, &r8);
	SDL_RenderCopy(renderer, texture_jeton_rouge, NULL, &r9);
	SDL_RenderCopy(renderer, texture_jeton_rouge, NULL, &r10);
	SDL_RenderCopy(renderer, texture_jeton_rouge, NULL, &r11);
	SDL_RenderCopy(renderer, texture_jeton_rouge, NULL, &r12);
	SDL_RenderCopy(renderer, texture_jeton_rouge, NULL, &r13);
	SDL_RenderCopy(renderer, texture_jeton_rouge, NULL, &r14);
	SDL_RenderCopy(renderer, texture_jeton_rouge, NULL, &r15);

	SDL_RenderPresent(renderer);
*/

    // Tant qu'aucun des joueurs n'a gagné le jeu, on continue à faire des parties
    while( (gameState.whiteScore < goal) && (gameState.blackScore < goal) )
    {
        j1StartGame(WHITE);
		j1NbTries = J1_NB_TRIES;
		
		j2StartGame(BLACK);
		j2NbTries = J2_NB_TRIES;
		
        // Tant que la partie en cours n'est pas fini
    	
		printf("Debut de la boucle de gestion des evenements");
        while (continuer == 1)
        {
            SDL_WaitEvent(&event);
            
            switch(event.type)
            {
                case SDL_QUIT:
                    continuer = 0;
                    break;
            }
            
			gameState.turn++; // Mise à jour du nombre de tour de la partie en cours
    
            // **********************************************************
            // TOUR DU PREMIER JOUEUR (WHITE)
            // **********************************************************            
            /*if(j1DoubleStack(&gameState))
            {
                j2TakeDouble(&gameState);
            }
            
            GenerateDices(dices); // Génération des deux dés
            
            copyGameState = gameState;
            
            j1PlayTurn(&copyGameState, dices, moves, &nbMoves, j1NbTries); // Le joueur 1 joue
            
            copyGameState = gameState; // Re-copie pour envoyer une copie du tableau avant de valider les changements (si valide)
            
            if ( IsValid(&copyGameState, dices, moves, nbMoves, WHITE) ) // Vérification des coups
            {
            	ModifPlateau(&gameState, moves, nbMoves, WHITE); // Mise à jour du plateau
            	
            	if( WinGame(&gameState, WHITE) ) // On regarde si le joueur à gagner
		        {
		            gameState.whiteScore++;
		            break;
		        }
            }
            else // Les coups n'étaient pas valides
            {
            	j1NbTries--; // On décremente le nombre d'essais restant
            	if( j1NbTries == 0 ) // Si le joueur n'a plus d'essais, il perd automatiquement
            	{
            		gameState.blackScore++;
		            break;
            	}
            }*/
            GenerateDices(dices);
            //dices[0]=1;
            //dices[1]=4;
            if(j1DoubleStack(&gameState))
            {
                j2TakeDouble(&gameState);
            }
            j1PlayTurn(&gameState,dices,moves,&nbMoves,3);
            ModifPlateau(&gameState, moves, nbMoves, WHITE);
			
			setBoardTokens(&gameState, noirs, blancs, renderer, texture_jeton_noir, texture_jeton_rouge);
			afficher(surfPlateau, surfJetonNoir, surfJetonBlanc, noirs, blancs, &rectPlateau, screen);

            if( WinGame(&gameState, WHITE) )
            {
                gameState.blackScore++;
                break;
            }
            
         
			getchar();
            // **********************************************************
            // TOUR DU DEUXIEME JOUEUR (BLACK)
            // **********************************************************
            
            /*
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
			
			
	        //SDL_RenderPresent(renderer);

		    }
		    else
		    {
		        fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());
		    }
		}
		j1EndGame();
    }
	j1EndMatch();

	SDL_DestroyTexture(texture);

	SDL_FreeSurface(surfPlateau);
	SDL_FreeSurface(surfJetonNoir);
	SDL_FreeSurface(surfJetonBlanc);
	SDL_FreeSurface(screen);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(pWindow);
	IMG_Quit();
	SDL_Quit();

	dlclose(lib);
	dlclose(lib2);
	return(0);
}
