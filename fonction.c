#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

int longueur_nombre(int a) {
    int longueur = 1;
    double b = (double)a;
    while ((b /= 10) >= 1.0) longueur++;
    return longueur;
}

void cpyTab(int t1[4][4], int t2[4][4]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) t2[i][j] = t1[i][j];
}

typedef struct Partie {
    int plateau[4][4];
    int score;
    int nb_coup;
    int fused[4][4];
    int precedent[4][4];
    int score_precedent;
} Partie;

int case_libre(Partie* p, int I[16], int J[16]) {
    int case_libre = 0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (p->plateau[i][j] == 0) {
                I[case_libre] = i;
                J[case_libre] = j;
                case_libre++;
            }
    return case_libre;
}

Partie* nouvelle_partie() {
    Partie* P = (Partie*)malloc(sizeof(Partie));
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            P->plateau[i][j] = 0;
            P->fused[i][j] = 0;
        }
    P->score = 0;
    P->score_precedent = 0;
    P->nb_coup = 0;
    int I[16];
    int J[16];
    while (case_libre(P, I, J) > 14) {
        int i = rand() % 4;
        int j = rand() % 4;
        P->plateau[i][j] = 2;
        P->precedent[i][j] = 2;
    }
    return P;
}

Partie* charger_partie() {
    Partie* P = (Partie*)malloc(sizeof(Partie));
    puts("Voici la liste des sauvegares disponible :\n");
    system("ls sauvegarde");
    FILE* f = NULL;
    char* rep;
    char* name = (char*)malloc(40);
    ;
    while (f == NULL) {
        puts("\nVeuillez préciser le nom exacte de votre sauvegarde");
        scanf("%s", name);
        rep = (char*)malloc(15 + strlen(name));
        *rep = '\0';

        strcat(rep, "sauvegarde/");
        strcat(rep, name);
        f = fopen(rep, "r");
        if (f == NULL)
            puts(
                "Désolé nous n'avons pu charger votre sauvegarde. Vérifiez son "
                "écriture ");
    }
    struct stat sb;
    stat(rep, &sb);
    char* c = malloc(sb.st_size);
    int i = -2;
    while (fscanf(f, "%[^\n] ", c) != EOF) {
        sscanf(c, "score : %d", &P->score);
        sscanf(c, "nombre de coups : %d", &P->nb_coup);
        sscanf(c, "| %d || %d || %d || %d |", &P->plateau[i][0],
               &P->plateau[i][1], &P->plateau[i][2], &P->plateau[i][3]);
        i++;
    }
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) P->precedent[i][j] = P->plateau[i][j];
    P->score_precedent = P->score;
    fclose(f);
    while (getchar() != '\n') {
    }
    free(name);
    free(rep);
    free(c);
    return P;
}

void Affiche_Partie(Partie* p) {
    puts("");
    printf("score : %d                             coup : %d \n", p->score,
           p->nb_coup);
    puts("-----------------------------------------------");
    for (int i = 0; i < 4; i++) {
        puts("\n     -------   -------   -------   -------        ");
        puts("    |       | |       | |       | |       |       ");
        printf("    ");
        for (int j = 0; j < 4; j++) {
            if (p->plateau[i][j] < 10)
                printf("|   %d   | ", p->plateau[i][j]);
            else if (p->plateau[i][j] < 100)
                printf("|   %d  | ", p->plateau[i][j]);
            else if (p->plateau[i][j] < 1000)
                printf("|  %d  | ", p->plateau[i][j]);
            else
                printf("|  %d | ", p->plateau[i][j]);
        }
        printf("\n    |       | |       | |       | |       |       ");
    }
    puts("\n     -------   -------   -------   -------        ");
    puts("\n-----------------------------------------------\n");
}

void sauvegarde_partie(Partie* p) {
    puts("Quel nom donnez-vous à votre sauvegarde?");
    char* name = (char*)malloc(40);
    ;
    scanf("%s", name);
    char* rep = (char*)malloc(15 + strlen(name));
    *rep = '\0';

    strcat(rep, "sauvegarde/");
    strcat(rep, name);
    strcat(rep, ".txt");

    FILE* f = fopen(rep, "w+");

    char c = (char)p->score;
    fprintf(f, "score : %d\n", p->score);
    fprintf(f, "nombre de coups : %d\n", p->nb_coup);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) fprintf(f, "| %d |", p->plateau[i][j]);
        putc('\n', f);
    }
    fclose(f);
    free(name);
    free(rep);
}

void nouvelle_case(Partie* p) {
    int I[16], J[16];
    int c = case_libre(p, I, J);
    if (c == 0) return;
    int indice = rand() % c;
    if (rand() % 10 == 9)
        p->plateau[I[indice]][J[indice]] = 4;
    else
        p->plateau[I[indice]][J[indice]] = 2;
}

void reset_fused(Partie* p) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) p->fused[i][j] = 0;
}

int swipe_left(Partie* p) {
    int mouvement = 1, nb_mouvement = 0;
    reset_fused(p);
    while (mouvement) {
        mouvement = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
                if (p->plateau[i][j + 1] != 0 && p->plateau[i][j] == 0) {
                    if (nb_mouvement == 0) {
                        p->score_precedent = p->score;
                        cpyTab(p->plateau, p->precedent);
                    }
                    p->plateau[i][j] = p->plateau[i][j + 1];
                    p->plateau[i][j + 1] = 0;
                    p->fused[i][j] = p->fused[i][j + 1];
                    p->fused[i][j + 1] = 0;
                    mouvement++;
                    nb_mouvement++;
                }
                if (p->plateau[i][j + 1] != 0 &&
                    p->plateau[i][j] == p->plateau[i][j + 1] &&
                    !(p->fused[i][j]) && !(p->fused[i][j + 1])) {
                    if (nb_mouvement == 0) {
                        p->score_precedent = p->score;
                        cpyTab(p->plateau, p->precedent);
                    }
                    p->plateau[i][j] += p->plateau[i][j + 1];
                    p->plateau[i][j + 1] = 0;
                    p->fused[i][j] = 1;
                    mouvement++;
                    nb_mouvement++;
                    p->score += p->plateau[i][j];
                }
            }
        }
    }
    if (nb_mouvement) return 1;
    return 0;
}

int swipe_right(Partie* p) {
    int mouvement = 1, nb_mouvement = 0;
    reset_fused(p);
    while (mouvement) {
        mouvement = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 3; j >= 1; j--) {
                if (p->plateau[i][j - 1] != 0 && p->plateau[i][j] == 0) {
                    if (nb_mouvement == 0) {
                        p->score_precedent = p->score;
                        cpyTab(p->plateau, p->precedent);
                    }
                    p->plateau[i][j] = p->plateau[i][j - 1];
                    p->plateau[i][j - 1] = 0;
                    p->fused[i][j] = p->fused[i][j - 1];
                    p->fused[i][j - 1] = 0;
                    mouvement++;
                    nb_mouvement++;
                }
                if (p->plateau[i][j - 1] != 0 &&
                    p->plateau[i][j - 1] == p->plateau[i][j] &&
                    !(p->fused[i][j]) && !(p->fused[i][j - 1])) {
                    if (nb_mouvement == 0) {
                        p->score_precedent = p->score;
                        cpyTab(p->plateau, p->precedent);
                    }
                    p->plateau[i][j] += p->plateau[i][j - 1];
                    p->plateau[i][j - 1] = 0;
                    p->fused[i][j] = 1;
                    mouvement++;
                    nb_mouvement++;
                    p->score += p->plateau[i][j];
                }
            }
        }
    }
    if (nb_mouvement) return 1;
    return 0;
}

int swipe_down(Partie* p) {
    int mouvement = 1, nb_mouvement = 0;
    reset_fused(p);
    while (mouvement) {
        mouvement = 0;
        for (int j = 0; j < 4; j++) {
            for (int i = 3; i >= 1; i--) {
                if (p->plateau[i - 1][j] != 0 && p->plateau[i][j] == 0) {
                    if (nb_mouvement == 0) {
                        p->score_precedent = p->score;
                        cpyTab(p->plateau, p->precedent);
                    }
                    p->plateau[i][j] = p->plateau[i - 1][j];
                    p->plateau[i - 1][j] = 0;
                    p->fused[i][j] = p->fused[i - 1][j];
                    p->fused[i - 1][j] = 0;
                    mouvement++;
                    nb_mouvement++;
                }
                if (p->plateau[i - 1][j] != 0 &&
                    p->plateau[i - 1][j] == p->plateau[i][j] &&
                    !(p->fused[i][j]) && !(p->fused[i - 1][j])) {
                    if (nb_mouvement == 0) {
                        p->score_precedent = p->score;
                        cpyTab(p->plateau, p->precedent);
                    }
                    p->plateau[i][j] += p->plateau[i - 1][j];
                    p->plateau[i - 1][j] = 0;
                    p->fused[i][j] = 1;
                    mouvement++;
                    nb_mouvement++;
                    p->score += p->plateau[i][j];
                }
            }
        }
    }
    if (nb_mouvement) return 1;
    return 0;
}

int swipe_up(Partie* p) {
    int mouvement = 1, nb_mouvement = 0;
    reset_fused(p);
    while (mouvement) {
        mouvement = 0;
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 3; i++) {
                if (p->plateau[i + 1][j] != 0 && p->plateau[i][j] == 0) {
                    if (nb_mouvement == 0) {
                        p->score_precedent = p->score;
                        cpyTab(p->plateau, p->precedent);
                    }
                    p->plateau[i][j] = p->plateau[i + 1][j];
                    p->plateau[i + 1][j] = 0;
                    p->fused[i][j] = p->fused[i + 1][j];
                    p->fused[i + 1][j] = 0;
                    mouvement++;
                    nb_mouvement++;
                }
                if (p->plateau[i + 1][j] != 0 &&
                    p->plateau[i + 1][j] == p->plateau[i][j] &&
                    !(p->fused[i][j]) && !(p->fused[i + 1][j])) {
                    if (nb_mouvement == 0) {
                        p->score_precedent = p->score;
                        cpyTab(p->plateau, p->precedent);
                    }
                    p->plateau[i][j] += p->plateau[i + 1][j];
                    p->plateau[i + 1][j] = 0;
                    p->fused[i][j] = 1;
                    mouvement++;
                    nb_mouvement++;
                    p->score += p->plateau[i][j];
                }
            }
        }
    }
    if (nb_mouvement) return 1;
    return 0;
}

void End_Of_Game(Partie* p, int* poiteur_FDP, int* test) {
    Partie copie;
    char choix;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            if ((copie.plateau[i][j] = p->plateau[i][j]) == 2048 && *test) {
                Affiche_Partie(p);
                puts("Bravo! vous avez gagné! Voulez-vous continuer ? O\\n\n");
                scanf("%c", &choix);
                while (getchar() != '\n') {
                }
                if (choix == 'O')
                    *test = 0;
                else
                    *poiteur_FDP = 0;
            }
        }
    if (!(swipe_down(&copie) || swipe_up(&copie) || swipe_right(&copie) ||
          swipe_left(&copie))) {
        Affiche_Partie(p);
        puts("\nFin de partie. Plus de mouvement possible.\n");
        printf("score  :  %d\n", p->score);
        printf("nombre de coup  :  %d\n", p->nb_coup);
        sleep(7);
        *poiteur_FDP = 0;
    }
}

void jeu(Partie* p) {
    char direction;
    int fin_de_partie = 1, test = 1;
    while (fin_de_partie) {
        Affiche_Partie(p);
        printf(
            "gauche (g), droite (d), haut(h), bas(b)                         "
            "(quitter (q))\n");
        scanf("%c", &direction);
        while (getchar() != '\n') {
        }
        if (direction == 'g' && swipe_left(p)) {
            p->nb_coup++;
            nouvelle_case(p);
        } else if (direction == 'd' && swipe_right(p)) {
            p->nb_coup++;
            nouvelle_case(p);
        } else if (direction == 'b' && swipe_down(p)) {
            p->nb_coup++;
            nouvelle_case(p);
        } else if (direction == 'h' && swipe_up(p)) {
            p->nb_coup++;
            nouvelle_case(p);
        } else if (direction == 'r') {
            p->score = p->score_precedent;
            cpyTab(p->precedent, p->plateau);
            p->nb_coup--;
        } else if (direction == 'q') {
            while (1) {
                puts(
                    "Voulez-vous sauvegarder votre partie avant de quitter? "
                    "O\\n");
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
    }
}

void AffichageSDL(Partie* p, SDL_Surface* ecran, SDL_Surface* score, SDL_Surface* texte, SDL_Surface** cases, int * max) {
    SDL_Rect positionCase;
    positionCase.x = 30;
    positionCase.y = 80;
    char* nom_img = (char*)malloc(16);
    char* numero = (char*)malloc(10);
    for (int i = 0; i < 4; i++) {
        positionCase.y += 144;
        for (int j = 0; j < 4; j++) {
            positionCase.x += 144;
            if (p->plateau[i][j] > *max){
                *max = p->plateau[i][j];
                *nom_img = '\0';
                strcat(nom_img, "image/");
                sprintf(numero, "%d", p->plateau[i][j]);
                strcat(numero, ".png");
                strcat(nom_img, numero);
                cases[(int)log2(*max)] = IMG_Load(nom_img);
                SDL_BlitSurface(cases[(int)log2(p->plateau[i][j])], NULL, ecran, &positionCase);
            }else{
                if (p->plateau[i][j] == 0)
                    SDL_BlitSurface(cases[0], NULL, ecran, &positionCase);
                else
                    SDL_BlitSurface(cases[(int)log2(p->plateau[i][j])], NULL, ecran, &positionCase);
            }
        }
        positionCase.x = 30;
    }
    positionCase.x = 470;
    positionCase.y = 55;
    SDL_BlitSurface(score, NULL, ecran, &positionCase);

    SDL_Color couleur = {80, 80, 80};
    SDL_Color couleurfond = {186, 173, 165};

    char c_score[20];
    sprintf(c_score, "  score : %d ", p->score);

    TTF_Font *police = TTF_OpenFont("C800.ttf",25);
    texte = TTF_RenderText_Shaded(police, c_score, couleur,couleurfond);
    positionCase.x = 478;
    positionCase.y = 85;
    SDL_BlitSurface(texte, NULL, ecran, &positionCase);

    free(nom_img);
    free(numero);
    TTF_CloseFont(police);
    SDL_Flip(ecran);
}

void End_Of_SDL(Partie* p, int* poiteur_FDP, int* test, int * max, SDL_Surface* ecran,
                SDL_Surface* image, SDL_Surface* texte, SDL_Surface ** cases) {
    Partie *copie = (Partie*)malloc(sizeof(Partie));
    char choix;
    TTF_Font * police = TTF_OpenFont("C800.ttf", 30);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            if ((copie->plateau[i][j] = p->plateau[i][j]) == 2048 && *test) {
                SDL_Event event;
                SDL_Color couleurNoire = {100, 100, 100};
                SDL_Rect position;
                position.x = 150;
                position.y = 200;
                SDL_Surface * score = IMG_Load("image/score.png");
                AffichageSDL(p, ecran, score, texte,cases, max);
                SDL_FreeSurface(score);
                SDL_Delay(1500);
                
                image = IMG_Load("image/plateau.png");
                SDL_BlitSurface(image, NULL, ecran, &position);
               
               
                char phrase[30] = "Bravo! vous avez gagne!";
                texte = TTF_RenderText_Blended(police, phrase, couleurNoire);
                position.x = 200;
                position.y = 350;
                SDL_BlitSurface(texte, NULL, ecran, &position);
                
                *phrase = '\0';
                strcat(phrase,"Voulez vous continuer?");
                texte = TTF_RenderText_Blended(police, phrase, couleurNoire);
                position.x = 205;
                position.y = 450;
                SDL_BlitSurface(texte, NULL, ecran, &position);

                *phrase = '\0';
                strcat(phrase,"oui [o]          [n] non");
                texte = TTF_RenderText_Blended(police, phrase, couleurNoire);
                position.x = 225;
                position.y = 600;
                SDL_BlitSurface(texte, NULL, ecran, &position);
                
                TTF_CloseFont(police);
                free(copie);
                SDL_Flip(ecran);
                while (SDL_WaitEvent(&event)) {
                    if (event.key.keysym.sym == SDLK_o){
                        *test = 0;
                        image = IMG_Load("image/plateau.png");
                        position.x = 150;
                        position.y = 200;
                        SDL_BlitSurface(image, NULL, ecran, &position);
                        SDL_Surface * score = IMG_Load("image/score.png");
                        AffichageSDL(p, ecran, score, texte,cases, max);
                        SDL_FreeSurface(score);
                        return;
                    }else if (event.key.keysym.sym == SDLK_n){
                        *poiteur_FDP = 0;
                        return;
                    }
                }
            }
        }
    if (!(swipe_down(copie) || swipe_up(copie) || swipe_right(copie) ||
          swipe_left(copie))) {
        SDL_Surface * score = IMG_Load("image/score.png");
        AffichageSDL(p, ecran, score, texte,cases, max);
        SDL_FreeSurface(score);
        sleep(1);
        SDL_Color couleurNoire = {100, 100, 100};
        SDL_Rect position;
        position.x = 150;
        position.y = 200;

        image = IMG_Load("image/plateau.png");
        SDL_BlitSurface(image, NULL, ecran, &position);
        
        
        char phrase[15] = "fin de partie";
        texte = TTF_RenderText_Blended(police, phrase, couleurNoire);
        position.x = 320;
        position.y = 350;
        SDL_BlitSurface(texte, NULL, ecran, &position);

        char c_score[8] = "score :";
        texte = TTF_RenderText_Solid(police, c_score, couleurNoire);
        position.x = 390;
        position.y = 450;
        SDL_BlitSurface(texte, NULL, ecran, &position);
        
        *c_score = '\0';
        sprintf(c_score, "%d", p->score);
        texte = TTF_RenderText_Solid(police, c_score, couleurNoire);
        position.x = 470 - 15 * longueur_nombre(p->score);
        position.y = 550;
        SDL_BlitSurface(texte, NULL, ecran, &position);
        
        SDL_Flip(ecran);
        *poiteur_FDP = 0;
        sleep(7);
    }
    TTF_CloseFont(police);
    free(copie);
}

void init(SDL_Surface ** cases, int max){
    char* nom_img = (char*)malloc(16);
    char* numero = (char*)malloc(10);
    cases[0] = IMG_Load("image/0.png");
    for (int i = 1 ; i <= (int)log2(max) ; i++){
        *nom_img = '\0';
        strcat(nom_img, "image/");
        sprintf(numero, "%d", (int)pow(2,i));
        strcat(numero, ".png");
        strcat(nom_img, numero);
        cases[i] = IMG_Load(nom_img);
    }
    free(numero);
    free(nom_img);
}


void jeuSDL(Partie* p, SDL_Surface* ecran, SDL_Surface* image,
            SDL_Surface* texte) {
    SDL_Event event;
    SDL_Rect position;
    SDL_Surface * score = IMG_Load("image/score.png");
    
    SDL_Surface** cases = (SDL_Surface**)malloc(17*sizeof(SDL_Surface*));
    int max = 0 ;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) 
            if (p->plateau[i][j] > max)
                max = p->plateau[i][j];
    init(cases,max);

    SDL_Color couleurNoire = {0, 0, 0}, couleurFond = {248, 239, 222};
    int fin_de_partie = 1, test = 1, a = 1 ;
    
    // Initialisation du temps
    char temps[20] = "";
    int  compteurM = 0, compteurD = 0, compteurS = 0, tempsActuel = 0, tempsPrecedent = 0;
    
    sprintf(temps, "%d : %d%d", compteurM,compteurD,compteurS);
    TTF_Font *police = TTF_OpenFont("C800.ttf", 45);

    texte = TTF_RenderText_Blended(police, temps, couleurNoire);
    position.x = 383;
    position.y = 800;
    SDL_BlitSurface(texte, NULL, ecran, &position); /* Blit du texte */
    AffichageSDL(p, ecran, score, texte, cases, &max);
    while (fin_de_partie) {
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:  // quitte et ferme la fenêtre
                return;
                break;
            case SDL_KEYDOWN:
                if (a && event.key.keysym.sym == SDLK_LEFT && swipe_left(p)) {
                    p->nb_coup++;
                    nouvelle_case(p);
                } else if (a && event.key.keysym.sym == SDLK_RIGHT &&
                           swipe_right(p) && a) {
                    p->nb_coup++;
                    nouvelle_case(p);
                } else if (a && event.key.keysym.sym == SDLK_DOWN &&
                           swipe_down(p) && a) {
                    p->nb_coup++;
                    nouvelle_case(p);
                } else if (a && event.key.keysym.sym == SDLK_UP &&
                           swipe_up(p) && a) {
                    p->nb_coup++;
                    nouvelle_case(p);
                } else if (a && event.key.keysym.sym == SDLK_ESCAPE)
                    return;
                else if (a && event.key.keysym.sym == SDLK_BACKSPACE) {
                    p->score = p->score_precedent;
                    cpyTab(p->precedent, p->plateau);
                }
                AffichageSDL(p, ecran, score, texte, cases, &max);
                (End_Of_SDL(p, &fin_de_partie, &test, &max, ecran, image,
                            texte,cases));
                a = 0;
                break;
            case SDL_KEYUP:
                a = 1;
                break;
        }
        tempsActuel = SDL_GetTicks();

        if (tempsActuel - tempsPrecedent >= 1000) /* Si 100 ms au moins se sont écoulées */
        {
            compteurS += 1; /* On rajoute 1 sec au compteur */
            if (compteurS >= 10) {
                compteurD += 1;
                compteurS = 0;
            }
            if (compteurD >= 6) {
                compteurM += 1;
                compteurD = 0;
            }
            
            sprintf(temps, "%d : %d%d ", compteurM, compteurD, compteurS); /* On écrit dans la chaîne temps" le nouveau temps */
            texte = TTF_RenderText_Shaded(police, temps, couleurNoire, couleurFond); /* On écrit la chaîne temps dans la SDL_Surface */
            SDL_BlitSurface(texte, NULL, ecran, &position); /* Blit du texte */

            tempsPrecedent = tempsActuel; /* On met à jour le tempsPrecedent */
        }

        SDL_Flip(ecran);
    }
    for (int i = 0 ; i <= (int)log2(max) ; i++)
        SDL_FreeSurface(cases[i]);
    free(cases);
    TTF_CloseFont(police);
}

int SDL_launch(Partie* p) {
    srand(time(NULL));
    SDL_Surface* ecran = NULL;
    SDL_Surface* image = NULL;
    SDL_Surface* texte = NULL;
    SDL_Rect position;

    // Démarrage de la SDL (ici : chargement du système vidéo)
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        // Écriture de l'erreur
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n",
                SDL_GetError());
        exit(EXIT_FAILURE);
        // On quitte le programme
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }

    TTF_Font* police = TTF_OpenFont("C800.ttf", 80);

    SDL_Color couleur_2048 = {242, 177, 121};

    // ouvre une fenêtre de taille 800 x 600 en 32 bits / pixel
    ecran = SDL_SetVideoMode(900, 900, 32, SDL_HWSURFACE);

    // donne un titre à la fenêtre
    SDL_WM_SetCaption("2048", NULL);

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 248, 239, 222));

    image = IMG_Load("image/fleche.png");

    position.x = 40;
    position.y = 200;
    SDL_BlitSurface(image, NULL, ecran, &position);

    texte = TTF_RenderText_Blended(police, "Pour jouer :", couleur_2048);

    position.x = 100;
    position.y = 20;

    SDL_BlitSurface(texte, NULL, ecran, &position);

    police = TTF_OpenFont("C800.ttf", 45);

    position.x = 465;
    position.y = 270;

    texte = TTF_RenderText_Blended(police, "Faire glisser", couleur_2048);
    SDL_BlitSurface(texte, NULL, ecran, &position);

    position.x = 530;
    position.y = 330;

    texte = TTF_RenderText_Blended(police, "les cases", couleur_2048);
    SDL_BlitSurface(texte, NULL, ecran, &position);

    image = IMG_Load("image/effacer.png");

    position.x = 65;
    position.y = 500;
    SDL_BlitSurface(image, NULL, ecran, &position);

    position.x = 495;
    position.y = 505;

    texte = TTF_RenderText_Blended(police, "annuler le", couleur_2048);
    SDL_BlitSurface(texte, NULL, ecran, &position);

    position.x = 465;
    position.y = 555;

    texte = TTF_RenderText_Blended(police, "dernier coup", couleur_2048);
    SDL_BlitSurface(texte, NULL, ecran, &position);

    image = IMG_Load("image/echap.png");

    position.x = 165;
    position.y = 700;
    SDL_BlitSurface(image, NULL, ecran, &position);

    position.x = 425;
    position.y = 760;

    texte = TTF_RenderText_Blended(police, "Quitter le jeu", couleur_2048);
    SDL_BlitSurface(texte, NULL, ecran, &position);

    SDL_Flip(ecran);
    sleep(5);
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 248, 239, 222));

    position.x = 160;
    position.y = 75;

    police = TTF_OpenFont("C800.ttf", 80);
    texte = TTF_RenderText_Blended(police, "2048", couleur_2048);

    SDL_BlitSurface(texte, NULL, ecran, &position);

    image = IMG_Load("image/plateau.png");

    position.x = 150;
    position.y = 200;

    SDL_BlitSurface(image, NULL, ecran, &position);
    // Collage de la surface sur l'écran


    jeuSDL(p, ecran, image, texte);

    SDL_FreeSurface(ecran);
    SDL_FreeSurface(image);
    SDL_FreeSurface(texte);
    free(p);

    TTF_CloseFont(police);

    TTF_Quit();
    SDL_Quit();  // Arrêt de la SDL (libération de la mémoire).

    return EXIT_SUCCESS;  // fermeture du programme
}