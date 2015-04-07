#include "gui.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL.h>
 
 
 
 void initHitBoxesTab(Hitbox *hitboxesTab, SDL_Surface* screen)
{

    int i,y1 = 645, y2= 50, width1 = 64, height = 600; // A MODIFIER


    for(i = 0; i < 28; i++){

        hitboxesTab[i].rectHB = (SDL_Rect*) malloc (1*sizeof(SDL_Rect)); // PENSER A LIBERER LE rectHB

        if(i == 0)
        {
            hitboxesTab[i].rectHB->x = 1080;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 1)
        {
            hitboxesTab[i].rectHB->x = 1007;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 2)
        {
            hitboxesTab[i].rectHB->x = 936;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 3)
        {
            hitboxesTab[i].rectHB->x = 865;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 4)
        {
            hitboxesTab[i].rectHB->x = 794;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 5)
        {
            hitboxesTab[i].rectHB->x = 722;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 6)
        {
            hitboxesTab[i].rectHB->x = 573;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 7)
        {
            hitboxesTab[i].rectHB->x = 501;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 8)
        {
            hitboxesTab[i].rectHB->x = 430;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 9)
        {
            hitboxesTab[i].rectHB->x = 359;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 10)
        {
            hitboxesTab[i].rectHB->x = 287;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 11)
        {
            hitboxesTab[i].rectHB->x = 215;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 12)
        {
            hitboxesTab[i].rectHB->x = 215;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 13)
        {
            hitboxesTab[i].rectHB->x = 287;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 14)
        {
            hitboxesTab[i].rectHB->x = 359;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 15)
        {
            hitboxesTab[i].rectHB->x = 430;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 16)
        {
            hitboxesTab[i].rectHB->x = 501;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 17)
        {
            hitboxesTab[i].rectHB->x = 573;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 18)
        {
            hitboxesTab[i].rectHB->x = 722;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 19)
        {
            hitboxesTab[i].rectHB->x = 794;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 20)
        {
            hitboxesTab[i].rectHB->x = 865;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 21)
        {
            hitboxesTab[i].rectHB->x = 936;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 22)
        {
            hitboxesTab[i].rectHB->x = 1007;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 23)
        {
            hitboxesTab[i].rectHB->x = 1080;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }
	//bar BLACK
	else if (i == 24)                               // A COMPLETER
	{
	    hitboxesTab[i].rectHB->x = 490;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
	}
	//bar WHITE
	else if (i == 25)
	{
            hitboxesTab[i].rectHB->x = 490;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
	}
	//out WHITE
	else if (i == 26)
	{
            hitboxesTab[i].rectHB->x = 1045;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
	}
	//out BLACK
	else if (i == 27)
	{
            hitboxesTab[i].rectHB->x = 1045;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
	}

       //SDL_FillRect(screen, hitboxesTab[i].rectHB, SDL_MapRGB(screen->format, 255, 0, 0));

    }

}




int detectClickIntoHitbox(Hitbox *hitboxesTab, int x, int y)
{
    int i;

    for(i = 0; i < 28; i++)
    {
        
        
        // On vérifie si l'abscisse de la hitbox récupérée est comprise entre les coordonnées de la hitbox courante
        if((x >= hitboxesTab[i].rectHB->x) && (x <= hitboxesTab[i].rectHB->x + hitboxesTab[i].rectHB->w))
        {
            if((y >= hitboxesTab[i].rectHB->y) && (y <= hitboxesTab[i].rectHB->y + hitboxesTab[i].rectHB->h))
            {
                //printf("Hitbox numéro : %d atteinte\n", i);
                return i;
            }
        }
    }
    printf("Aucune hitbox atteinte\n");
    return -1;

}



void clickToSMoves(int* indiceHBTab, SMove* moves,unsigned int *nbMoves, Player curPlayer)
{
    if(*nbMoves < 4)
    {
        int i; // normalisation des indices
        for (i = 0; i < 2; i++)
        {
            if(indiceHBTab[i]>=0 && indiceHBTab[i] <= 23) indiceHBTab[i] += 1;
            else if(indiceHBTab[i] == 24 || indiceHBTab[i] == 25) indiceHBTab[i] = 0;
            else if(indiceHBTab[i] == 26 || indiceHBTab[i] == 27) indiceHBTab[i] = 25;

        }

        printf("indiceHBTab[0] %d | indiceHBTab[1] %d | nbMoves %d\n",indiceHBTab[0],indiceHBTab[1], *nbMoves);
        if(curPlayer == WHITE)
        {

            moves[*nbMoves].src_point = indiceHBTab[0];
            moves[*nbMoves].dest_point = indiceHBTab[1];
        }

        else if(curPlayer == BLACK)
        {

            if(indiceHBTab[0]!=0 && indiceHBTab[0]!=25)
            {
                moves[*nbMoves].src_point = 24-indiceHBTab[0]+1;
            }
            else
            {
                moves[*nbMoves].src_point = indiceHBTab[0];
            }

            if(indiceHBTab[1]!=0 && indiceHBTab[1]!=25)
            {
                moves[*nbMoves].dest_point = 24-indiceHBTab[1]+1;
            }
            else
            {
                moves[*nbMoves].dest_point = indiceHBTab[1];
            }

        }

        printf("moves added between %d and %d\n",moves[*nbMoves].src_point,moves[*nbMoves].dest_point);
        *nbMoves += 1; // on incrémente le compteur de moves
        indiceHBTab[0] = -1;
        indiceHBTab[1] = -1;
    }
    else
    {
        printf("erreur segmentation fault out of SMoves[4]");
    }


}
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

 
 
 
 
 
 
 
void pause();
 
int main(int argc, char *argv[])
{
	//pointeur utilisé par SDL pour l'allocation mémoire nécessaire à la fenetre
	SDL_Surface *screen = NULL, *imageDeFond = NULL;; 
    SDL_Rect positionFond;

	positionFond.x = 0;
    positionFond.y = 0;

    if (SDL_Init(SDL_INIT_VIDEO) == -1) // Démarrage de la SDL. Si erreur :
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); 
        exit(EXIT_FAILURE);
    }
 
    screen = SDL_SetVideoMode(1360, 760, 32, SDL_HWSURFACE); // Ouverture de la fenêtre
	
	if (screen == NULL) // Si l'ouverture s'est mal passée :
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
	
	SDL_WM_SetCaption("Backgammon", NULL);

	imageDeFond = SDL_LoadBMP("plateau.bmp");
    /* On blitte par-dessus l'écran */
    SDL_BlitSurface(imageDeFond, NULL, screen, &positionFond);
	
	SDL_Flip(screen);
	
    pause(); // Mise en pause du programme
 	
	SDL_FreeSurface(imageDeFond); //Liberation de la surface
    SDL_Quit(); // Arrêt de la SDL
 
    return EXIT_SUCCESS; // Fermeture du programme
}
 
void pause()
{
    int continuer = 1;
    SDL_Event event;
 
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
        }
    }
}


void setBoardTokens(const SGameState * const state, SDL_Rect noirs[15] , SDL_Rect rouges[15])
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
            for (j = 0; j<state->board[i].nbDames; j++)
            {
                noirs[cptN].x = equivalence_x[i];
                
                if(i<13) noirs[cptN].y = equivalence_y[j];
                else noirs[cptN].y = equivalence_y2[j];
                
                cptN++;
            }
        }
        else
        {
            for (j = 0; j<state->board[i].nbDames; j++)
            {
                rouges[cptR].x = equivalence_x[i];
                
                if(i<13) noirs[cptR].y = equivalence_y[j];
                else noirs[cptR].y = equivalence_y2[j];
                
                cptN ++;
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void init(SDL_renderer renderer, SDL_Window *pWindow, SDL_Surface *image, SDL_Surface *jeton_noir, SDL_Surface *jeton_rouge, SDL_Texture *texture, SDL_Texture *texture_jeton_noir, SDL_Texture *texture_jeton_rouge)
{
    /* ============****** INITIALISATION  DE LA FENETRE ******============ */
    if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
    }
    
    /* ============****** CREATION DE LA FENETRE ******============ */
    pWindow = SDL_CreateWindow("Backgammon",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, 1360, 760, SDL_WINDOW_SHOWN);
    if (pWindow == NULL) SDL_ShowSimpleMessageBox(0, "Window init error", SDL_GetError(), pWindow);
    
    /* ============****** GESTION DES IMAGES ******============ */
    renderer = SDL_CreateRenderer(pWindow, -1, 0);
    if (renderer == NULL) SDL_ShowSimpleMessageBox(0, "Renderer init error", SDL_GetError(), pWindow);
    
    image = SDL_LoadBMP("plateau.bmp");
    jeton_noir = SDL_LoadBMP("jeton_noir.bmp");
    jeton_rouge = SDL_LoadBMP("jeton_rouge.bmp");
    
    if (image == NULL) SDL_ShowSimpleMessageBox(0, "Image init error", SDL_GetError(), pWindow);
    if (jeton_noir == NULL) SDL_ShowSimpleMessageBox(0, "Image init error", SDL_GetError(), pWindow);
    if (jeton_rouge == NULL) SDL_ShowSimpleMessageBox(0, "Image init error", SDL_GetError(), pWindow);
    
    
    //Definition de la texture et verification de son existence
    texture = SDL_CreateTextureFromSurface(renderer, image);
    texture_jeton_noir = SDL_CreateTextureFromSurface(renderer, jeton_noir);
    texture_jeton_rouge = SDL_CreateTextureFromSurface(renderer, jeton_rouge);
    
    if (texture == NULL) SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), pWindow);
    if (texture_jeton_noir == NULL) SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), pWindow);
    if (texture_jeton_rouge == NULL) SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), pWindow);
    
    /* ============****** RENDU DES JETONS (SITUATION INITIALE) ******============ */
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    
    SDL_Rect r1 = { 1080, 645, 64, 64 }; //Jan 1
    SDL_Rect r2 = { 1080, 581, 64, 64 }; //Jan 1
    SDL_Rect r3 = { 212, 645, 64, 64 }; //Jan 12
    SDL_Rect r4 = { 212, 581, 64, 64 }; //Jan 12
    SDL_Rect r5 = { 212, 517, 64, 64 }; //Jan 12
    SDL_Rect r6 = { 212, 453, 64, 64 }; //Jan 12
    SDL_Rect r7 = { 212, 389, 64, 64 }; //Jan 12
    SDL_Rect r8 = { 500, 50, 64, 64 }; //Jan 17
    SDL_Rect r9 = { 500, 114, 64, 64 }; //Jan 17
    SDL_Rect r10 = { 500, 178, 64, 64 }; //Jan 17
    SDL_Rect r11 = { 722, 50, 64, 64 }; //Jan 19
    SDL_Rect r12 = { 722, 114, 64, 64 }; //Jan 19
    SDL_Rect r13 = { 722, 178, 64, 64 }; //Jan 19
    SDL_Rect r14 = { 722, 242, 64, 64 }; //Jan 19
    SDL_Rect r15 = { 722, 306, 64, 64 }; //Jan 19
    
    SDL_Rect rouges[15] = {r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15};
    
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
    
    //Jetons Noirs
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
}

void quitter()
{
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_FreeSurface(jeton_noir);
    SDL_FreeSurface(jeton_rouge);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
}

int main(int argc, char** argv)
{
    init();
    
    /* ============****** GESTIONNAIRE DES EVENEMENTS DE LA FENETRE ******============ */
    if(pWindow)
    {
        while (continuer == 1)
        {
            SDL_PollEvent(&event);
            
            switch(event.type)
            {
                case SDL_QUIT:
                    continuer = 0;
                    break;
            }
        }
        
        SDL_RenderPresent(renderer);
    }
    else
    {
        fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());
    }
    
    quitter();
    
    return 0;
}
