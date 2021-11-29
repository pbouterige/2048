#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <math.h>

#include "fonction.c"

void AffichageSDL(Partie* p, SDL_Surface* ecran, SDL_Surface* cases,
                  SDL_Surface* score) {
    SDL_Rect positionCase;
    positionCase.x = 30;
    positionCase.y = 80;
    char* nom_img = (char*)malloc(16);
    char* numero = (char*)malloc(10);
    for (int i = 0; i < 4; i++) {
        positionCase.y += 144;
        for (int j = 0; j < 4; j++) {
            *nom_img = '\0';
            strcat(nom_img, "image/");
            sprintf(numero, "%d", p->plateau[i][j]);
            strcat(numero, ".png");
            strcat(nom_img, numero);
            positionCase.x += 144;
            cases = IMG_Load(nom_img);
            SDL_BlitSurface(cases, NULL, ecran, &positionCase);
        }
        positionCase.x = 30;
    }
    SDL_Flip(ecran);
}

void jeuSDL(Partie* p, SDL_Surface* ecran, SDL_Surface* cases,
            SDL_Surface* score) {
    SDL_Event event;
    int fin_de_partie = 1, test = 1;
    while (fin_de_partie) {
        SDL_WaitEvent(&event);
        AffichageSDL(p, ecran, cases, score);
        switch (event.type) {
            case SDL_QUIT:  // quitte et ferme la fenêtre
                return;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_LEFT && swipe_left(p)) {
                    p->nb_coup++;
                    nouvelle_case(p);
                } else if (event.key.keysym.sym == SDLK_RIGHT &&
                           swipe_right(p)) {
                    p->nb_coup++;
                    nouvelle_case(p);
                } else if (event.key.keysym.sym == SDLK_DOWN && swipe_down(p)) {
                    p->nb_coup++;
                    nouvelle_case(p);
                } else if (event.key.keysym.sym == SDLK_UP && swipe_up(p)) {
                    p->nb_coup++;
                    nouvelle_case(p);
                } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    while (1) {
                        puts(
                            "Voulez-vous sauvegarder votre partie avant de "
                            "quitter? O\\n");
                        char direction;
                        scanf("%c", &direction);
                        while (getchar() != '\n') {
                        }
                        if (direction == 'O') {
                            sauvegarde_partie(p);
                            return;
                        } else if (direction == 'n')
                            return;
                        puts("Désolé je ne vous ai pas compris.");
                    }
                }
                (End_Of_Game(p, &fin_de_partie, &test));
                break;
        }
    }
}

void pauseWin() {
    // cette fonction permet de mettre en pause le programme
    // pour éviter que la fenêtre se ferme aussitôt ouverte
    int continuer = 1;
    SDL_Event event;
    while (continuer) {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:  // quitte et ferme la fenêtre
                continuer = 0;
                break;
            default:  // ne fait rien
                break;
        }
    }
}

int main() {
    srand(time(NULL));
    SDL_Surface* ecran = NULL;
    SDL_Surface* plateau = NULL;
    SDL_Surface* cases = NULL;
    SDL_Surface* icone = NULL;
    SDL_Surface* score = NULL;
    SDL_Rect position;

    // Démarrage de la SDL (ici : chargement du système vidéo)
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        // Écriture de l'erreur
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n",
                SDL_GetError());
        exit(EXIT_FAILURE);
        // On quitte le programme
    }

    icone = IMG_Load("image/icone2.png");
    SDL_WM_SetIcon(icone, NULL);

    // ouvre une fenêtre de taille 800 x 600 en 32 bits / pixel
    ecran = SDL_SetVideoMode(900, 900, 32, SDL_HWSURFACE);

    // donne un titre à la fenêtre
    SDL_WM_SetCaption("2048", NULL);

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 248, 239, 222));

    plateau = IMG_Load("image/plateau.png");
    cases = IMG_Load("image/0.png");

    position.x = 150;  // Les coordonnées de la surface seront (150, 200)
    position.y = 200;

    // Collage de la surface sur l'écran
    SDL_BlitSurface(plateau, NULL, ecran, &position);

    Partie* p = nouvelle_partie();
    jeuSDL(p, ecran, cases, score);

    SDL_FreeSurface(cases);  // Libération de la surface
    SDL_FreeSurface(plateau);
    SDL_FreeSurface(ecran);
    SDL_FreeSurface(icone);
    SDL_FreeSurface(score);
    free(p);

    SDL_Quit();  // Arrêt de la SDL (libération de la mémoire).

    return EXIT_SUCCESS;  // fermeture du programme
}