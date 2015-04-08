#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dlfcn.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "gui/gui.h"
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


void setBoardTokens(const SGameState * const state, SDL_Rect noirs[15] , SDL_Rect rouges[15])
{
    printf("Calcul des coordonnees des jetons ... \n");
    //Tableau d'equivalence entre les cases et leurs positions en pixels
    int equivalence_x[12] = {1087, 1014, 944, 872, 802, 730, 581, 509, 438, 367, 296, 224};
    
    //Tableau d'equivalence entre la position sur une case et la position en pixel correspondante
    int equivalence_y[6] = {662, 625, 588, 551, 514, 477};
    int equivalence_y2[6] = {50, 87, 124, 161, 198, 235};
    
    //Tableau d'equivalence entre les coordonnees de out et le nombre de jetons blancs dans out
    int equivalence_outB[15] = {604, 593, 582, 571, 560, 549, 538, 527, 516, 505, 494, 483, 472, 461, 451};
    
    //Tableau d'equivalence entre les coordonnees de out et le nombre de jetons noirs dans out
    int equivalence_outN[15] = {107, 118, 129, 140, 151, 162, 173, 184, 195, 206, 217, 228, 239, 250, 261};
    
    //Tableau d'equivalence entre les coordonnees de bar et le nombre de jetons noirs dans bar
    int equivalence_barB[15] = {500, 492, 484, 475, 467, 459, 450, 442, 434, 425, 417, 409, 400, 392, 384};
    
    //Tableau d'equivalence entre les coordonees de bar et le nombre de jetons blancs dans bar
    int equivalence_barN[15] = {210, 218, 226, 235, 243, 251, 260, 268, 276, 285, 293, 301, 310, 318, 326};
    
    
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
				
		//printf("X : %d, Y : %d \n", noirs[cptN].x, noirs[cptN].y);
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
		
		//printf("X : %d, Y : %d \n", rouges[cptR].x, rouges[cptR].y);
                cptR ++;
            }
        }
    }
    
    //Affichage de la sortie des jetons noirs
    if ( state->out[0] != 0 )
    {
      for(i = 0; i < state->out[0]; i++)
      {
	noirs[cptN].x = 1167;
	noirs[cptN].y = equivalence_outN[i];
	cptN++;
      }
    }
    
    
    //Affichage de la sortie des jetons blancs
    if ( state->out[1] != 0 )
    {
      for(i = 0; i < state->out[1]; i++)
      {
	rouges[cptR].x = 1167;
	rouges[cptR].y = equivalence_outB[i];
	cptR++;
      }
    }
    
    //Affichage du bar de jetons noirs
    if ( state->bar[0] != 0 )
    {
      for (i = 0; i < state->bar[0]; i++)
      {
	noirs[cptN].x = 655;
	noirs[cptN].y = equivalence_barN[i];
	cptN++;
      }
    }
    
    //Affichage du bar de jetons noirs
    if ( state->bar[1] != 0 )
    {
      for (i = 0; i < state->bar[1]; i++)
      {
	rouges[cptR].x = 655;
	rouges[cptR].y = equivalence_barB[i];
	cptR++;
      }
    }
    
}

void afficher(SDL_Surface *surfPlateau, SDL_Surface *surfJetonNoir, SDL_Surface *surfJetonBlanc, SDL_Rect noirs[15] , SDL_Rect blancs[15], SDL_Rect *rectPlateau, SDL_Surface *screen)
{
	int i = 0, j = 0;
	printf("Affichage ... \n");
	//Afficher un ecran noir
	SDL_FillRect(screen, NULL, SDL_MapRGB(surfPlateau->format, 0,0,0));

	//Affichage du plateau 
	SDL_BlitSurface(surfPlateau, 0, screen, rectPlateau);

	//Parcours du tableau de jetons noirs et affichage
	for (i=0; i<15;i++)
	{
		//printf("BlitSurface du jeton i = %d \n",i); 
		SDL_BlitSurface(surfJetonNoir, 0, screen, &noirs[i]);
	}

	//Parcours du tableau de jetons blancs et affichage
	for (j=0; j<15;j++)
	{
		//printf("BlitSurface du jeton j = %d \n",j);
		SDL_BlitSurface(surfJetonBlanc, 0, screen, &blancs[j]);
	}
}


//void deroulement_du_jeu()
int main(int argc, char *argv[])
{
  
    /* ============****** VARIABLES ******============ */
    SDL_Event event;

    SDL_Window* pWindow = NULL;
    
    SDL_Surface * surfPlateau;
    SDL_Surface * surfJetonNoir;
    SDL_Surface * surfJetonBlanc;
    SDL_Surface * screen;
    
    bool done = false;
	
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
	
	
	
	
	/*------------------------------------------------------------------------------------------
	 * 											    |
	 * 											    |
	 *  ===================================    GUI    ==========================================
* 												    |
	 * 											    |
	 * -----------------------------------------------------------------------------------------
	 */
	
	
	//Initialisation de la fenêtre
	if (SDL_Init(SDL_INIT_VIDEO) != 0 ) printf("Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());

	
	//Creation de la fenetre
	pWindow = SDL_CreateWindow("Backgammon",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, 1360, 760, SDL_WINDOW_SHOWN);
	if (pWindow == NULL) SDL_ShowSimpleMessageBox(0, "Window init error", SDL_GetError(), pWindow);
	
	
	//Surface de l'ecran
	screen = SDL_GetWindowSurface(pWindow);

	//Chargement des images
	IMG_Init(IMG_INIT_PNG);
	

	surfPlateau = SDL_LoadBMP("plateau.bmp");
	surfJetonNoir = IMG_Load("noir_48.png");
	surfJetonBlanc = IMG_Load("blanc_48.png");

	//Surface SDL servant à contenir l'image du plateau
	SDL_Rect rectPlateau = {0, 0, 1360, 760};
	
	//Surfaces SDL servant à contenir les jetons blancs du plateau
	SDL_Rect b1 = { 1087, 662, 48, 48 }; //Jan 1
	SDL_Rect b2 = { 1087, 625, 48, 48 }; //Jan 1
	SDL_Rect b3 = { 224, 662, 48, 48 }; //Jan 12
	SDL_Rect b4 = { 224, 625, 48, 48 }; //Jan 12
	SDL_Rect b5 = { 224, 588, 48, 48 }; //Jan 12
	SDL_Rect b6 = { 224, 551, 48, 48 }; //Jan 12
	SDL_Rect b7 = { 224, 514, 48, 48 }; //Jan 12
	SDL_Rect b8 = { 509, 50, 48, 48 }; //Jan 17
	SDL_Rect b9 = { 509, 87, 48, 48 }; //Jan 17
	SDL_Rect b10 = { 509, 124, 48, 48 }; //Jan 17
	SDL_Rect b11 = { 730, 50, 48, 48 }; //Jan 19
	SDL_Rect b12 = { 730, 87, 48, 48 }; //Jan 19
	SDL_Rect b13 = { 730, 124, 48, 48 }; //Jan 19
	SDL_Rect b14 = { 730, 161, 48, 48 }; //Jan 19
	SDL_Rect b15 = { 730, 198, 48, 48 }; //Jan 19

	//Tableau de SDL_Rect servant à la mise à jour du plateau et des jetons
	SDL_Rect blancs[15] = {b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15};
	
	
	//Surfaces SDL servant à contenir les jetons noirs du plateau
	SDL_Rect n1 = { 730, 662, 48, 48 }; //Jan 6
	SDL_Rect n2 = { 730, 625, 48, 48 }; //Jan 6
	SDL_Rect n3 = { 730, 588, 48, 48 }; //Jan 6
	SDL_Rect n4 = { 730, 551, 48, 48 }; //Jan 6
	SDL_Rect n5 = { 730, 514, 48, 48 }; //Jan 6
	SDL_Rect n6 = { 509, 662, 48, 48 }; //Jan 8
	SDL_Rect n7 = { 509, 625, 48, 48 }; //Jan 8
	SDL_Rect n8 = { 509, 588, 48, 48 }; //Jan 8
	SDL_Rect n9 = { 224, 50, 48, 48 }; //Jan 13
	SDL_Rect n10 = { 224, 87, 48, 48 }; //Jan 13
	SDL_Rect n11 = { 224, 124, 48, 48 }; //Jan 13
	SDL_Rect n12 = { 224, 161, 48, 48 }; //Jan 13
	SDL_Rect n13 = { 224, 198, 48, 48 }; //Jan 13
	SDL_Rect n14 = { 1087, 50, 48, 48 }; //Jan 24
	SDL_Rect n15 = { 1087, 87, 48, 48 }; //Jan 24
	
	
	//Tableau de SDL_Rect servant à la mise à jour du plateau et des jetons
	SDL_Rect noirs[15] = {n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14, n15};
	
	
	//Collage de l'image du plateau chargée en mémoire plus tot sur la surface du plateau
	SDL_BlitSurface(surfPlateau, 0, screen, &rectPlateau);

	
	//Collage des jetons sur l'interface
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
	
	
	//Collage des jetons sur l'interface
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
	
	
	//Appel a la fonction de mise a jour de l'ecran
	SDL_UpdateWindowSurface(pWindow);
	
	
	Hitbox *hitboxesTab = (Hitbox*) malloc (28*sizeof(Hitbox));
	

	int clicx;
	int clicy;
	int curHB;

	int numHB[2];
	numHB[0] = -1;
	numHB[1] = -1;

	initHitBoxesTab(hitboxesTab, screen);
	
	


	// Tant qu'aucun des joueurs n'a gagné le jeu, on continue à faire des parties
	while( (gameState.whiteScore < goal) && (gameState.blackScore < goal) )
	{
	    
	  j1StartGame(WHITE);
	  j1NbTries = J1_NB_TRIES;
		  
	  j2StartGame(BLACK);
	  j2NbTries = J2_NB_TRIES;
		    
	  // Tant que la partie en cours n'est pas fini
	  while(!done)
	  {
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
	    UpdateGameState(&gameState, moves, nbMoves, WHITE);
			
	    setBoardTokens(&gameState, noirs, blancs);
	    
	    afficher(surfPlateau, surfJetonNoir, surfJetonBlanc, noirs, blancs, &rectPlateau, screen);

	    SDL_UpdateWindowSurface(pWindow);

	    if( WinGame(&gameState, WHITE) )
	    {
		gameState.blackScore++;
		break;
	    }
	    
			/*initHitBoxesTab(hitboxesTab, screen);
			SDL_UpdateWindowSurface(pWindow);*/
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
	    
	    // Gestion des evenements
	    printf("Gestion des evenements \n");
	    while ( SDL_PollEvent(&event) )
	    {  
		printf("Switch case de gestion des evenements... \n");
		switch(event.type)
		{
		    case SDL_WINDOWEVENT:
		      if(event.window.event == SDL_WINDOWEVENT_CLOSE) 
		      {
			printf("SQL_QUIT captured, treating... \n");
			done = true;
		      }
		      break;
		      
		      
		    case SDL_MOUSEBUTTONUP:
		      printf("Clic clic ... \n");
		      clicx = event.button.x;
		      clicy = event.button.y;
		      printf("X = %d \n", clicx);
		      printf("Y = %d \n", clicy);
		      curHB = detectClickIntoHitbox(hitboxesTab, clicx,clicy);
		      printf("\n HITBOX NUMERO  = %d", curHB);


		       if (curHB != -1) {

				    	if (numHB[0] == -1) {
						
						printf("Coucou 0");
						numHB[0] = curHB;

					} else if (numHB[1] == -1) {

						printf("Coucou 1");
						numHB[1] = curHB;
						clickToSMoves(numHB,moves,&nbMoves, WHITE);
											

					}
				     }
			
		      break;
		}
		
	    }
	    
	    printf("Fin de gestion des evenements \n");

	  }
	  if(done) break;
	  printf("Fin Game \n");
	  j1EndGame();
	  
	}
	printf("Fin Match \n");
	j1EndMatch();

	free(hitboxesTab);
	 
	SDL_FreeSurface(screen);
	SDL_FreeSurface(surfPlateau);
	SDL_FreeSurface(surfJetonNoir);
	SDL_FreeSurface(surfJetonBlanc);

	SDL_DestroyWindow(pWindow);

	SDL_Quit();

	dlclose(lib);
	dlclose(lib2);
	
	printf("Fin propre de l'execution \n");
	return(0);
}
