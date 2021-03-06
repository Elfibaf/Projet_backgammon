#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dlfcn.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "gui/gui.h"
#include "arbitrage.h"
#include "api.h"

#define DEFAULT_GOAL 5
#define J1_NB_TRIES 3
#define J2_NB_TRIES 3


//****************************
//
//Fichier pour le test du bot 
//
//****************************


//void deroulement_du_jeu()
int main(int argc, char *argv[])
{
    //Variables
    SDL_Event event;

    SDL_Window* pWindow = NULL;
    
    SDL_Surface * surfPlateau;
    SDL_Surface * surfJetonNoir;
    SDL_Surface * surfJetonBlanc;
    SDL_Surface * screen;
    SDL_Surface * des;
    SDL_Surface * scoreWhite;
    SDL_Surface * scoreBlack;
    SDL_Surface * titleWhite;
    SDL_Surface * titleBlack;
    SDL_Surface * mise;
    
    //Fonts
    TTF_Font *font;
    TTF_Font *font1;
    
    //Definition de la couleur de la police
    SDL_Color colorFont = {0, 0, 0};
	
    
    //Surface accueillant l'affichage des des
    SDL_Rect rectDes = {140, 350, 0, 0};
    
    //Surface d'affichage des scores
    SDL_Rect rectTitleBlack = {130, 180, 0, 0};
    SDL_Rect rectTitleWhite = {130, 480, 0, 0};
    
    SDL_Rect rectScoreBlack = {155, 200, 0, 0};
    SDL_Rect rectScoreWhite = {155, 500, 0, 0};
    
    //Surface d'affichage de la mise
    SDL_Rect rectMise = {1183, 368, 0, 0};
    
    //Surface SDL servant à contenir l'image du plateau
    SDL_Rect rectPlateau;
    
    //Surfaces SDL servant à contenir les jetons blancs du plateau
    SDL_Rect b1,b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15;

    //Tableau de SDL_Rect servant à la mise à jour du plateau et des jetons
    SDL_Rect blancs[15] = {b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15};
    
    //Surfaces SDL servant à contenir les jetons noirs du plateau
    SDL_Rect n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14, n15; 
    
    //Tableau de SDL_Rect servant à la mise à jour du plateau et des jetons
    SDL_Rect noirs[15] = {n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14, n15};
      
    
    //Chaine de caractère correspondant à l'affichage du dé
    char stringDes[10];
    
    //Chaine de caractère représentant le score White
    char stringScoreWhite[20];
    
    //Chaine de caractère représentant le score Black
    char stringScoreBlack[20];
    
    //Chaine de caractere de la mise
    char stringMise[10];
    
    bool done = false;
	
    //initialisation du générateur de nombres aléatoire pour la génération des dés
    srand(time(NULL));
    
    int goal; // Le score a obtenir pour gagner la partie
    char mode[20],nomBot1[50],nomBot2[50]; //Mode de jeu + nom des librairies à charger
    
    
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
    		strcpy(nomBot1,argv[2]);
            strcpy(mode,"ManVsBot");
    		
    		printf("Vous avez choisi le mode de jeu : HUMAIN VS IA - Jeu en %d points\n", goal);
    	}
    	else if (argc == 4)
    	{
    		strcpy(nomBot1,argv[2]);
    	    strcpy(nomBot2,argv[3]);
    	    strcpy(mode,"BotvsBot");
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
	

    printf("MODE : %s\n",mode);
    if((strcmp(mode,"ManVsBot")== 0) || (strcmp(mode,"BotvsBot") == 0))
    {
        
	    
	    if ((lib=dlopen(nomBot1,RTLD_LAZY)) == NULL)
	    {
		    printf("Erreur de chargement de la librairie\n");
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
	        printf("Erreur de chargement des pointeurs de fonction\n");
		    return(-1);
	    }
	    
	    //Initialisation de la librairie
	    char name1[50];
	    j1InitLibrary(name1);
	    j1StartMatch(goal);
	        
	    if (strcmp(mode,"BotvsBot") == 0)
	    {
        	if ((lib2=dlopen(nomBot2,RTLD_LAZY)) == NULL)
            {
	            printf("Erreur de chargement de la librairie\n");
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
                printf("Erreur de chargement des pointeurs de fonction\n");
	            return(-1);
            }
	        //Initialisation de la librairie
	        char name2[50];
	        j2InitLibrary(name2);
	        j2StartMatch(goal);

	    }
	    
    }

	SGameState gameState, copyGameState;
	
	
	unsigned char dices[2];

	unsigned int nbMoves; // Le nombre de coup possibles que peut faire le joueur
	SMove moves[4]; // Tableau de mouvements
	
	unsigned int j1NbTries, j2NbTries; // variables pour gérer le nombre d'essais de chaque joueur
	int doubleJ1, doubleJ2; // Variables pour vérifier qui possede le videau
	
	/*------------------------------------------------------------------------------------------
	 * 											    |
	 * 											    |
	 *  ===================================    GUI    ==========================================
* 												    |
	 * 											    |
	 * -----------------------------------------------------------------------------------------
	 */

	
	//Initialisation de la fenêtre
	if ( SDL_Init(SDL_INIT_VIDEO) != 0 ) printf("Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
	
	//Initialisation de TTF
	if ( TTF_Init() == -1) printf("Echec de l'initialisation de TTF (%s) \n", TTF_GetError());
	
	//Creation de la fenetre
	pWindow = SDL_CreateWindow("Backgammon",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, 1360, 760, SDL_WINDOW_SHOWN);
	if (pWindow == NULL) SDL_ShowSimpleMessageBox(0, "Window init error", SDL_GetError(), pWindow);
	
	//Surface de l'ecran
	screen = SDL_GetWindowSurface(pWindow);

	//Chargement des images
	IMG_Init(IMG_INIT_PNG);
	
	//Chargement en memoire des images utilisees dans la GUI
	surfPlateau = SDL_LoadBMP("plateau.bmp");
	surfJetonNoir = IMG_Load("noir_48.png");
	surfJetonBlanc = IMG_Load("blanc_48_new.png");
	
	
	//Definition de la police à utiliser pour l'affichage des des
	font = TTF_OpenFont("stocky.ttf", 25);
	font1 = TTF_OpenFont("stocky.ttf", 18);
	
	if(font == NULL) printf("Erreur d'ouverture  de la police (%s)", TTF_GetError());
	if(font1 == NULL) printf("Erreur d'ouverture  de la police (%s)", TTF_GetError());
	
	//Rendu du texte a afficher
	des = TTF_RenderText_Blended(font, " Des 1 : Des 2 ", colorFont);
	if(des == NULL) printf("Erreur de rendu du texte du des (%s)", TTF_GetError());
	
	titleWhite = TTF_RenderText_Blended(font1, "Score White : ", colorFont);
	if(titleWhite == NULL) printf("Erreur de rendu du texte du score (%s)", TTF_GetError());
	
	titleBlack = TTF_RenderText_Blended(font1, "Score Black : ", colorFont);
	if(titleBlack == NULL) printf("Erreur de rendu du texte du score (%s)", TTF_GetError());
	
	scoreWhite = TTF_RenderText_Blended(font, "0", colorFont);
	if(scoreWhite == NULL) printf("Erreur de rendu du texte du score (%s)", TTF_GetError());
	
	scoreBlack = TTF_RenderText_Blended(font, "0", colorFont);
	if(scoreBlack == NULL) printf("Erreur de rendu du texte du score (%s)", TTF_GetError());
	
	mise = TTF_RenderText_Blended(font, "0", colorFont);
	if(mise == NULL) printf("Erreur de rendu du texte de la mise (%s)", TTF_GetError());
	//Affichage du score
	afficherScore(titleBlack, titleWhite, scoreBlack, scoreWhite, &rectScoreBlack, &rectScoreWhite, &rectTitleBlack, &rectTitleWhite, stringScoreBlack, stringScoreWhite, colorFont, font1, screen, gameState.whiteScore, gameState.blackScore);
	
	//Appel a la fonction de mise a jour de l'ecran
	SDL_UpdateWindowSurface(pWindow);
	
	
	// Initialisation des scores des deux joueurs
	gameState.whiteScore = 0;
	gameState.blackScore = 0;
	InitPlateau(&gameState); // Initialisation du tableau
	  
	//Affichage du plateau initial
	setBoardTokens(&gameState, noirs, blancs, &rectPlateau);
	afficher(surfPlateau, surfJetonNoir, surfJetonBlanc, noirs, blancs, &rectPlateau, &rectDes, screen);
	afficherDes(des, &rectDes, dices, stringDes, colorFont, font, screen);
	afficherMise(mise, &rectMise, stringMise, colorFont, font, screen, gameState.stake);
	
	//Appel a la fonction de mise a jour de l'ecran
	SDL_UpdateWindowSurface(pWindow);
	
	//Allocation mémoire au tableau qui contiendra les 28 hitbox
	Hitbox *hitboxesTab = (Hitbox*) malloc (28*sizeof(Hitbox));

	// Entiers qui contiendront les coordonnées du clic
	int clicx;
	int clicy;
	
	// Entier qui contiendra la hitbox touchée par un clic
	int curHB;
	
	// Entier qui contiendra le nombre de moves qu'un joueur pourra faire à tel tour, utile dans la gestion des clics
	int cpt;

	// Tableau de deux entiers qui contiendra la hitbox "source" et la hitbox "destination" touchées par deux clics consécutifs
	int numHB[2];
	numHB[0] = -1; // On initialiste à -1, valeur qui sera utilisée si aucun clic "source" ou "destination" n'a été fait pour le moment
	numHB[1] = -1;

	// Appel de la fonction qui permet d'initialiser les coordonnées des hitbox
	initHitBoxesTab(hitboxesTab, screen);
	
	


	// Tant qu'aucun des joueurs n'a gagné le jeu, on continue à faire des parties
	while( (gameState.whiteScore < goal) && (gameState.blackScore < goal))
	{
	    InitPlateau(&gameState); // Initialisation du tableau
	  
	    //Affichage du plateau initial
	    setBoardTokens(&gameState, noirs, blancs, &rectPlateau);
	    afficher(surfPlateau, surfJetonNoir, surfJetonBlanc, noirs, blancs, &rectPlateau, &rectDes, screen);
	
	    //Appel a la fonction de mise a jour de l'ecran
	    SDL_UpdateWindowSurface(pWindow);
	      
	    //Initialisation des bots pour une game
	    if((strcmp(mode,"ManVsBot")== 0) || (strcmp(mode,"BotvsBot") == 0))
		{
			j1StartGame(WHITE);
			if(strcmp(mode,"BotvsBot") == 0)
			{
			    j2StartGame(BLACK);
			}
		}
	 
	  
	    Player player; //Joueur courant
	    
	    // En début de manche les 2 joueurs ont le videau
	    doubleJ1 = 1;
	    doubleJ2 = 1;
	    
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
	  
	    // Tant que la partie en cours n'est pas fini
	    while((! WinGame(&gameState, (1-player))) && (!done))
	    {

	      gameState.turn++; // Mise à jour du nombre de tours de la partie en cours
	      GenerateDices(dices); // Generation des 2 dés
	      
	      
	      //Affichage du de avant que le joueur joue
	      afficherDes(des, &rectDes, dices, stringDes, colorFont, font, screen);
	      afficherScore(titleBlack, titleWhite, scoreBlack, scoreWhite, &rectScoreBlack, &rectScoreWhite, &rectTitleBlack, &rectTitleWhite, stringScoreBlack, stringScoreWhite, colorFont, font1, screen, gameState.whiteScore, gameState.blackScore);
	      afficherMise(mise, &rectMise, stringMise, colorFont, font, screen, gameState.stake);
	      
	      //Mise a jour de l'affichage
	      SDL_UpdateWindowSurface(pWindow);
	      
	      if ((player == WHITE) && ((strcmp(mode,"ManVsBot")== 0) || (strcmp(mode,"BotvsBot") == 0)))
	      {
			  if ((j1DoubleStack(&gameState)) &&(doubleJ1 == 1))
			  {
			      if(!j2TakeDouble(&gameState))
			      {
					gameState.blackScore += gameState.stake;
					break;
			      }
			      else
			      {
					  doubleJ1=0;
					  doubleJ2=1;
					  gameState.stake = gameState.stake*2;
			      }
			    
			  }
		  
		  j1PlayTurn(&gameState, dices, moves, &nbMoves, j1NbTries);
		  
		  copyGameState = gameState;		        
		  if (CheckTurn(&copyGameState, dices, moves, nbMoves, player)) // Vérification des coups
		  {
		      UpdateAllMove(&gameState, moves, nbMoves, player); // Mise à jour du jeu
		      
		      //Calcul des coordonnees des jetons
		      setBoardTokens(&gameState, noirs, blancs, &rectPlateau);

		      //Affichage des jetons et du plateau
		      afficher(surfPlateau, surfJetonNoir, surfJetonBlanc, noirs, blancs, &rectPlateau, &rectDes, screen);
		      
		      //Affichage des des et du score
		      afficherDes(des, &rectDes, dices, stringDes, colorFont, font, screen);
		      afficherScore(titleBlack, titleWhite, scoreBlack, scoreWhite, &rectScoreBlack, &rectScoreWhite, &rectTitleBlack, &rectTitleWhite, stringScoreBlack, stringScoreWhite, colorFont, font1, screen, gameState.whiteScore, gameState.blackScore);
		      afficherMise(mise, &rectMise, stringMise, colorFont, font, screen, gameState.stake);
		      
		      //Mise a jour de l'affichage de l'interface
		      SDL_UpdateWindowSurface(pWindow);
		      
		      if (WinGame(&gameState, WHITE)) // On regarde si le joueur à gagner la partie
		      {
			  	gameState.whiteScore += gameState.stake;
		      }
		  }
		  else // Les coups n'étaient pas valides
		  {
		      j1NbTries--; // On décremente le nombre d'essais restant
		      if (j1NbTries == 0) // Si le joueur n'a plus d'essais, il perd automatiquement
		      {
			  gameState.blackScore+= gameState.stake;
		      }
		  }
		  
		  	player = BLACK; // Mise à jour du joueur	
	      }
    	
	      else if((player == BLACK) && (strcmp(mode,"BotvsBot") == 0))
	      {
		  if ((j2DoubleStack(&gameState)) && (doubleJ2 == 1))
		  {
		      if(!j1TakeDouble(&gameState))
		      {
			  gameState.whiteScore += gameState.stake;
			      break;
		      }
		      else
		      {
			  doubleJ1=1;
			  doubleJ2=0;
			  gameState.stake = gameState.stake*2;
		      }
		  }
		  
		  j2PlayTurn(&gameState, dices, moves, &nbMoves, j2NbTries);
		      
		  copyGameState = gameState;
		  if (CheckTurn(&copyGameState, dices, moves, nbMoves, player)) // Vérification des coups
		  {
		      UpdateAllMove(&gameState, moves, nbMoves, player); // Mise à jour du jeu
		      
		      //Calcul des coordonnees des jetons
		      setBoardTokens(&gameState, noirs, blancs, &rectPlateau);
		      
		      //Affichage des jetons et du plateau
		      afficher(surfPlateau, surfJetonNoir, surfJetonBlanc, noirs, blancs, &rectPlateau, &rectDes, screen);
		      
		      //Affichage des des et du score
		      afficherDes(des, &rectDes, dices, stringDes, colorFont, font, screen);
		      afficherScore(titleBlack, titleWhite, scoreBlack, scoreWhite, &rectScoreBlack, &rectScoreWhite, &rectTitleBlack, &rectTitleWhite, stringScoreBlack, stringScoreWhite, colorFont, font1, screen, gameState.whiteScore, gameState.blackScore);
		      afficherMise(mise, &rectMise, stringMise, colorFont, font, screen, gameState.stake);
		      
		      //Mise a jour de l'affichage de l'interface
		      SDL_UpdateWindowSurface(pWindow);
			  
		      if (WinGame(&gameState, player)) // On regarde si le joueur à gagner la partie
		      {
			  gameState.blackScore+=gameState.stake;
		      }
		  }
		  else // Les coups n'étaient pas valides
		  {
		      j2NbTries--; // On décremente le nombre d'essais restant
		      if (j2NbTries == 0) // Si le joueur n'a plus d'essais, il perd automatiquement
		      {
			      gameState.whiteScore+=gameState.stake;
		      }
		  }
		  
		  player = WHITE; // Mise à jour du joueur
	      }
	    
	    
	      SDL_Delay(500);

	      
	      // Gestion des evenements
	      printf("Gestion des evenements \n");
	      while ( SDL_PollEvent(&event) )
	      {  
		switch(event.type)
		{
		    case SDL_WINDOWEVENT:
		      if(event.window.event == SDL_WINDOWEVENT_CLOSE) 
		      {
			  printf("SQL_QUIT captured, treating... \n");
			  done = true;
		      }
		      break;
			
			
			
			// Cas du clic, lorsque l'utilisateur clique sur le plateau de jeu, on rentre dans ce case
		    case SDL_MOUSEBUTTONUP:
		      
		      //printf("Clic clic ... \n"); TEST
		      
		      // On récupère les coordonnées du clic de l'utilisateur
		      clicx = event.button.x;
		      clicy = event.button.y;
		      //printf("X = %d \n", clicx); TEST
		      //printf("Y = %d \n", clicy);
		      
		      
		      // On récupère grâce à la fonction la hitbox touchée par l'utilisateur (c'est à dire  quelle case du board il a touché)  via son clic
		      curHB = detectClickIntoHitbox(hitboxesTab, clicx,clicy);
		      //printf("\n HITBOX NUMERO  = %d", curHB); TEST


              // Si la hitbox récupérée existe   
		      if (curHB != -1) {


						// Si le clic est un "clic source"
				    	if (numHB[0] == -1) {
						
							// printf("Clic source"); TEST
						
							//On met la hitbox touchée dans le case du tableau contenant la hitbox source
							numHB[0] = curHB;
	
	
	
	
							/* TRAITEMENT AFFICHAGE POSSIBILITES DE COUP POUR LE JOUEUR 
							// Si les dés sont égaux, alors chaque dé va être utilisé deux fois, sinon une fois
							if (dices[0] == dices[1]) {
	
								cpt = 2;
	
							} else {
	
								cpt =1;
	
							}
	
							// On affiche les hitbox touchables pour chaque dé, et on le fait une autre fois si les dés sont égaux
							for (i =1; i <= cpt; i++) {
								
								SDL_FillRect(screen, hitboxesTab[curHB+(i*dices[0])].rectHB, SDL_MapRGB(screen->format, 255, 0, 0));
								SDL_FillRect(screen, hitboxesTab[curHB+(i*dices[1])].rectHB, SDL_MapRGB(screen->format, 255, 0, 0));
								SDL_UpdateWindowSurface(pWindow);
								getchar();
							} */


						// Si le clic est un "clic destination"
						} else if (numHB[1] == -1) {
	
							//printf("Clic destination"); TEST
							
							
							//On met la hitbox touchée dans le case du tableau contenant la hitbox destination
							numHB[1] = curHB;
	
							
							// Conditionnelle permettant de savoir combien de moves peut faire le joueur, et stocke ce nombre dans la variable cpt
							if (dices[0] == dices[1]) {
	
								cpt = 4; // 4 moves maximum si les dés sont égaux
	
							} else {
	
								cpt = 2; // 2 sinon
	 
							}
							
							
							// Puisque l'on vient d'ajouter un "clic destination", on utilise donc cette fonction qui "transforme" ces deux clics en un Move
							clickToSMoves(numHB,moves, &nbMoves, WHITE, cpt);
							//printf("NBMOVES = %d\n", nbMoves); TEST
							
							// On met à jour le gameState en ajoutant et en faisant les modfications à faire sur le Board avec le nouveau Move juste créé
							UpdateOneMove(&gameState,moves[nbMoves-1], BLACK); 
							
							//On met à jour l'affichage des jetons et du plateau
							setBoardTokens(&gameState, noirs, blancs, &rectPlateau);
							afficher(surfPlateau, surfJetonNoir, surfJetonBlanc, noirs, blancs, &rectPlateau, &rectDes, screen);
							//getchar(); TEST, permettait de pouvoir bien regarder les différents résultats de tests puisque ça faisait une pause	
							
							
										

						}
				}
		      break;
		}
		  
	      }
	      printf("Fin de gestion des evenements \n");

	  }
	  
	  if(done) break;
	  
	  //Cleaning the printed elements
	  sprintf(stringDes, " ");
    
	  des = TTF_RenderText_Blended(font, stringDes, colorFont);
	  
	  SDL_BlitSurface(des, 0, screen, &rectDes);
	  
	  SDL_UpdateWindowSurface(pWindow);
	  
	  printf("Fin Game \n");
	  j1EndGame();
	  j2EndGame();
	}
	
	printf("Fin Match \n");
	j1EndMatch();
	j2EndMatch();
	
	// Libération de la mémoire utilisée par le tableau d'hitbox
	free(hitboxesTab);
	 
	SDL_FreeSurface(screen);
	SDL_FreeSurface(surfPlateau);
	SDL_FreeSurface(surfJetonNoir);
	SDL_FreeSurface(surfJetonBlanc);
	SDL_FreeSurface(des);
	
	TTF_CloseFont(font);
	
	SDL_DestroyWindow(pWindow);
	
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	dlclose(lib);
	dlclose(lib2);
	
	printf("Fin propre de l'execution \n");
	return(0);
}
