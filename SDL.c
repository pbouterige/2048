#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


void pause(){
// cette fonction permet de mettre en pause le programme
// pour éviter que la fenêtre se ferme aussitôt ouverte
    int continuer = 1;
    SDL_Event event;
    while( continuer ) {
        SDL_WaitEvent(&event);
        switch ( event.type ) {
            case SDL_QUIT:    // quitte et ferme la fenêtre
                continuer = 0;
                break;
            default:  // ne fait rien
                break;
        }
    }
}


int main(){
    SDL_Surface *ecran = NULL;
    SDL_Surface *plateau = NULL;
    SDL_Surface *cases = NULL;
    SDL_Rect position, positionCase;


    // Démarrage de la SDL (ici : chargement du système vidéo)
    if(SDL_Init(SDL_INIT_VIDEO) == -1) {
        // Écriture de l'erreur
        fprintf(stderr,"Erreur d'initialisation de la SDL : %s\n",SDL_GetError());
        exit(EXIT_FAILURE); 
        // On quitte le programme
    }


    // ouvre une fenêtre de taille 800 x 600 en 32 bits / pixel
    ecran = SDL_SetVideoMode(900, 900, 32, SDL_HWSURFACE);    

    // donne un titre à la fenêtre
    SDL_WM_SetCaption("2048", NULL);

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 248, 239, 222));

    plateau = SDL_CreateRGBSurface(SDL_HWSURFACE, 600, 600, 32, 0, 0, 0, 0);
    cases = SDL_CreateRGBSurface(SDL_HWSURFACE, 120, 120, 32, 0, 0, 0, 0);

    // Remplissage du plateau
    SDL_FillRect(plateau, NULL, SDL_MapRGB(ecran->format, 145, 124, 111)); 
    SDL_FillRect(cases, NULL, SDL_MapRGB(ecran->format, 187, 174, 166)); 

    position.x = 150;    // Les coordonnées de la surface seront (0, 0)
    position.y = 200;
    positionCase.x = 30;    // Les coordonnées de la surface seront (0, 0)
    positionCase.y = 80;


    // Collage de la surface sur l'écran
    SDL_BlitSurface(plateau, NULL, ecran, &position);
    
    for (int i = 0 ; i < 4 ; i++){
        positionCase.x += 144;
        for (int j = 0 ; j < 4 ; j++){
            positionCase.y += 144;
            SDL_BlitSurface(cases, NULL, ecran, &positionCase);
        }
        positionCase.y = 80;
    }


    
    SDL_Flip(ecran);   // Mise à jour de l'écran
    pause();           // mise en pause du programme

    SDL_FreeSurface(cases);     // Libération de la surface
    SDL_FreeSurface(plateau); 
    SDL_FreeSurface(ecran); 

    SDL_Quit();        // Arrêt de la SDL (libération de la mémoire).

    return EXIT_SUCCESS; //fermeture du programme
}