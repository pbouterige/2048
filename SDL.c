#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

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
    SDL_Surface *ecran = NULL, *rectangle = NULL, *imageDeFond = NULL;
    SDL_Rect position, positionFond;


    // Démarrage de la SDL (ici : chargement du système vidéo)
    if(SDL_Init(SDL_INIT_VIDEO) == -1) {
        // Écriture de l'erreur
        fprintf(stderr,"Erreur d'initialisation de la SDL : %s\n",SDL_GetError());
        exit(EXIT_FAILURE); 
        // On quitte le programme
    }

    SDL_WM_SetIcon(SDL_LoadBMP("icon1.bmp"), NULL);

    // ouvre une fenêtre de taille 800 x 600 en 32 bits / pixel
    ecran = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);

    // donne un titre à la fenêtre
    SDL_WM_SetCaption("fenetre", NULL);

    // SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 17, 206, 112));

    // Allocation de la surface
    // rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, 220, 180, 32, 0, 0, 0, 0);

    // Remplissage de la surface avec du blanc
    // SDL_FillRect(rectangle, NULL, SDL_MapRGB(ecran->format, 255, 255, 255)); 

    position.x = 0;    // Les coordonnées de la surface seront (0, 0)
    position.y = 0;
    positionFond.x = 0;    // Les coordonnées de la surface seront (0, 0)
    positionFond.y = 0;


    // Collage de la surface sur l'écran
    // SDL_BlitSurface(rectangle, NULL, ecran, &position);
    
    imageDeFond = SDL_LoadBMP("2048.bmp");
    SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);
    
    SDL_Flip(ecran);   // Mise à jour de l'écran
    pause();           // mise en pause du programme

    SDL_FreeSurface(imageDeFond);     // Libération de la surface

    SDL_Quit();        // Arrêt de la SDL (libération de la mémoire).

    return EXIT_SUCCESS; //fermeture du programme
}