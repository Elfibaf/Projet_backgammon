#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
 
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
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur
        exit(EXIT_FAILURE); // On quitte le programme
    }
 
    screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE); // Ouverture de la fenêtre
	
	if (screen == NULL) // Si l'ouverture s'est mal passée :
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
	
	SDL_WM_SetCaption("Backgammon", NULL);
	//SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 17, 206, 112));

	imageDeFond = SDL_LoadBMP("lac_en_montagne.bmp");
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
