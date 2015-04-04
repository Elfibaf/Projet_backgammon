#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    /* ============****** VARIABLES ******============ */
    int continuer = 1;
    SDL_Event event;
    
    /* ============****** INITIALISATION  DE LA FENETRE ******============ */
    if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }
    
    /* ============****** CREATION DE LA FENETRE ******============ */
    SDL_Window* pWindow = NULL;
    pWindow = SDL_CreateWindow("Backgammon",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, 1360, 760, SDL_WINDOW_SHOWN);
    if (pWindow == NULL) SDL_ShowSimpleMessageBox(0, "Window init error", SDL_GetError(), pWindow);
    
    /* ============****** GESTION DES IMAGES ******============ */
    SDL_Renderer * renderer = SDL_CreateRenderer(pWindow, -1, 0);
    if (renderer == NULL) SDL_ShowSimpleMessageBox(0, "Renderer init error", SDL_GetError(), pWindow);
    
    SDL_Surface * image = SDL_LoadBMP("plateau.bmp");
    SDL_Surface * jeton_noir = SDL_LoadBMP("jeton_noir.bmp");
    SDL_Surface * jeton_rouge = SDL_LoadBMP("jeton_rouge.bmp");
    
    if (image == NULL) SDL_ShowSimpleMessageBox(0, "Image init error", SDL_GetError(), pWindow);
    if (jeton_noir == NULL) SDL_ShowSimpleMessageBox(0, "Image init error", SDL_GetError(), pWindow);
    if (jeton_rouge == NULL) SDL_ShowSimpleMessageBox(0, "Image init error", SDL_GetError(), pWindow);
    
    
    //Definition de la texture et verification de son existence
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_Texture * texture_jeton_noir = SDL_CreateTextureFromSurface(renderer, jeton_noir);
    SDL_Texture * texture_jeton_rouge = SDL_CreateTextureFromSurface(renderer, jeton_rouge);
    
    if (texture == NULL) SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), pWindow);
    if (texture_jeton_noir == NULL) SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), pWindow);
    if (texture_jeton_rouge == NULL) SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), pWindow);

    /* ============****** RENDU DES JETONS (SITUATION INITIALE) ******============ */
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    
    SDL_Rect r1 = { 1080, 645, 64, 64 }; //Jan 1
    SDL_Rect r2 = { 1080, 581, 64, 64 }; //Jan 1
    
    SDL_Rect n1 = { 722, 645, 64, 64 }; //Jan 6
    SDL_Rect n2 = { 722, 581, 64, 64 }; //Jan 6
    SDL_Rect n3 = { 722, 517, 64, 64 }; //Jan 6
    SDL_Rect n4 = { 722, 453, 64, 64 }; //Jan 6
    SDL_Rect n5 = { 722, 389, 64, 64 }; //Jan 6
    
    SDL_Rect n6 = { 500, 645, 64, 64 }; //Jan 8
    SDL_Rect n7 = { 500, 581, 64, 64 }; //Jan 8
    SDL_Rect n8 = { 500, 517, 64, 64 }; //Jan 8
    
    SDL_Rect r3 = { 212, 645, 64, 64 }; //Jan 12
    SDL_Rect r4 = { 212, 581, 64, 64 }; //Jan 12
    SDL_Rect r5 = { 212, 517, 64, 64 }; //Jan 12
    SDL_Rect r6 = { 212, 453, 64, 64 }; //Jan 12
    SDL_Rect r7 = { 212, 389, 64, 64 }; //Jan 12
    
    SDL_Rect n9 = { 212, 50, 64, 64 }; //Jan 13
    SDL_Rect n10 = { 212, 114, 64, 64 }; //Jan 13
    SDL_Rect n11 = { 212, 178, 64, 64 }; //Jan 13
    SDL_Rect n12 = { 212, 242, 64, 64 }; //Jan 13
    SDL_Rect n13 = { 212, 306, 64, 64 }; //Jan 13
    
    SDL_Rect r8 = { 500, 50, 64, 64 }; //Jan 17
    SDL_Rect r9 = { 500, 114, 64, 64 }; //Jan 17
    SDL_Rect r10 = { 500, 178, 64, 64 }; //Jan 17
    
    SDL_Rect r11 = { 722, 50, 64, 64 }; //Jan 19
    SDL_Rect r12 = { 722, 114, 64, 64 }; //Jan 19
    SDL_Rect r13 = { 722, 178, 64, 64 }; //Jan 19
    SDL_Rect r14 = { 722, 242, 64, 64 }; //Jan 19
    SDL_Rect r15 = { 722, 306, 64, 64 }; //Jan 19
    
    SDL_Rect n14 = { 1080, 50, 64, 64 }; //Jan 24
    SDL_Rect n15 = { 1080, 114, 64, 64 }; //Jan 24
    
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
    
    
    /* ============****** GESTIONNAIRE DES EVENEMENTS DE LA FENETRE ******============ */
    if(pWindow)
    {
        while (continuer == 1)
        {
            SDL_WaitEvent(&event);
            
            switch(event.type)
            {
                case SDL_QUIT:
                    continuer = 0;
                    break;
            }
            apply_surface(0,10, NULL, pWindow);
        }
    }
    else
    {
        fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());
    }
    
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    
    return 0;
}

void draw_surface(int srcX, int srcY, int dstX, int dstY, int width, int height, SDL_Surface *source, SDL_Surface *destination)
{
    SDL_Rect src;
    src.x = srcX;
    src.y = srcY;
    src.w = width;
    src.h = height;
    
    SDL_Rect dst;
    dst.x = dstX;
    dst.y = dstY;
    dst.w = width;
    dst.h = height;
    
    SDL_BlitSurface(source, &src, destination, &dst);
}