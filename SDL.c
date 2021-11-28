#include "fonction.c"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <math.h>


void pauseWin(){
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
    SDL_Surface *icone = NULL;
    SDL_Rect position, positionCase;


    // Démarrage de la SDL (ici : chargement du système vidéo)
    if(SDL_Init(SDL_INIT_VIDEO) == -1) {
        // Écriture de l'erreur
        fprintf(stderr,"Erreur d'initialisation de la SDL : %s\n",SDL_GetError());
        exit(EXIT_FAILURE); 
        // On quitte le programme
    }

    icone = IMG_Load("image/icone2.png");
    SDL_WM_SetIcon(icone,NULL);

    // ouvre une fenêtre de taille 800 x 600 en 32 bits / pixel
    ecran = SDL_SetVideoMode(900, 900, 32, SDL_HWSURFACE);    

    // donne un titre à la fenêtre
    SDL_WM_SetCaption("2048", NULL);

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 248, 239, 222));

    plateau = IMG_Load("image/plateau.png");
    cases = IMG_Load("image/0.png");


    position.x = 150;    // Les coordonnées de la surface seront (150, 200)
    position.y = 200;
    positionCase.x = 30;    
    positionCase.y = 80;


    // Collage de la surface sur l'écran
    SDL_BlitSurface(plateau, NULL, ecran, &position);
    

    int k = 1;
    char * nom_img = (char*)malloc(16);
    char * numero = (char*)malloc(10);

    for (int i = 0 ; i < 4 ; i++){
        positionCase.y += 144;
        for (int j = 0 ; j < 4 ; j++){
            *nom_img = '\0';
            strcat(nom_img,"image/");
            int a = (int)pow(2,k);
            sprintf(numero, "%d", a);
            strcat(numero,".png");
            strcat(nom_img,numero);
            printf("%s \n", nom_img);
            k++;
            positionCase.x += 144;
            SDL_BlitSurface(cases, NULL, ecran, &positionCase);
            cases = IMG_Load(nom_img);
        }
        positionCase.x = 30;
    }


    
    SDL_Flip(ecran);   // Mise à jour de l'écran
    pauseWin();           // mise en pause du programme

    SDL_FreeSurface(cases);     // Libération de la surface
    SDL_FreeSurface(plateau); 
    SDL_FreeSurface(ecran); 

    SDL_Quit();        // Arrêt de la SDL (libération de la mémoire).

    return EXIT_SUCCESS; //fermeture du programme
}