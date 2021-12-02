#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <math.h>


int longueur_nombre(int a){
    int longueur = 1;
    double b = (double)a;
    while ((b /= 10) > 1.0)
        longueur++;
    return longueur;
}


typedef struct Partie
{
    int plateau[4][4];
    int score;
    int nb_coup;
    int fused[4][4];

}Partie;

int case_libre(Partie *p,int I[16], int J[16]){
    int case_libre = 0;
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (p->plateau[i][j] == 0){
                I[case_libre] = i;
                J[case_libre] = j;
                case_libre ++;    
            }
    return case_libre;
}

Partie * nouvelle_partie(){
    Partie * P = (Partie*)malloc(sizeof(Partie));
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++){
            P->plateau[i][j] = 0;
            P->fused[i][j] = 0;
        }
    P ->score = 0;
    P ->nb_coup = 0;
    int I[16];
    int J[16];
    while (case_libre(P, I, J) > 14){
        int i = rand()%4;
        int j = rand()%4;
        P->plateau[i][j] = 2;
    }
    return P;
}

Partie * charger_partie(){
    Partie * P = (Partie*)malloc(sizeof(Partie));
    puts("Voici la liste des sauvegares disponible :\n");
    system("ls sauvegarde");
    FILE * f = NULL;
    char* rep;
    char * name = (char *) malloc( 40 );;
    while (f==NULL){
        puts("\nVeuillez préciser le nom exacte de votre sauvegarde");
        scanf("%s",name);
        rep = (char *) malloc( 15 + strlen(name) );
        *rep = '\0';

        strcat( rep, "sauvegarde/" );
        strcat( rep, name );
        f = fopen(rep,"r");
        if (f == NULL)
            puts("Désolé nous n'avons pu charger votre sauvegarde. Vérifiez son écriture ");
    }
    struct stat sb;
	stat(rep, &sb);
    char *c = malloc(sb.st_size);
    int i = -2;
    while(fscanf(f,"%[^\n] ",c) != EOF){
        sscanf(c, "score : %d",&P->score);
        sscanf(c, "nombre de coups : %d",&P->nb_coup);
        sscanf(c, "| %d || %d || %d || %d |",&P->plateau[i][0],&P->plateau[i][1],&P->plateau[i][2],&P->plateau[i][3]);
        i++;
    }
    fclose(f);
    while (getchar() != '\n') {}
    free(name);
    free(rep);
    free(c);
    return P;
}

void Affiche_Partie(Partie * p){
    puts("");
    printf("score : %d                             coup : %d \n",p->score,p->nb_coup);
    puts("-----------------------------------------------");
    for (int i = 0 ; i < 4 ; i++){
        puts("\n     -------   -------   -------   -------        ");
        puts("    |       | |       | |       | |       |       ");
        printf("    ");
        for (int j = 0 ; j < 4 ; j++){
            if(p->plateau[i][j] <10)
                printf("|   %d   | ",p->plateau[i][j]);
            else if(p->plateau[i][j] <100)
                printf("|   %d  | ",p->plateau[i][j]);
            else if(p->plateau[i][j] <1000)
                printf("|  %d  | ",p->plateau[i][j]);
            else
                printf("|  %d | ",p->plateau[i][j]);
            
        }
        printf("\n    |       | |       | |       | |       |       ");
    }
    puts("\n     -------   -------   -------   -------        ");
    puts("\n-----------------------------------------------\n");

 
}


void sauvegarde_partie(Partie *p){
    puts("Quel nom donnez-vous à votre sauvegarde?");
    char * name = (char *) malloc( 40 );;
    scanf("%s",name);
    char * rep = (char *) malloc( 15 + strlen(name) );
    *rep = '\0';

    strcat( rep, "sauvegarde/" );
    strcat( rep, name );
    strcat( rep, ".txt" );

    FILE * f = fopen(rep,"w+");
    
    
    char c = (char)p->score;
    fprintf(f,"score : %d\n",p->score);
    fprintf(f,"nombre de coups : %d\n",p->nb_coup);
    for (int i = 0 ; i < 4 ; i ++){
        for (int j = 0 ; j < 4 ; j++)
            fprintf(f,"| %d |",p->plateau[i][j]);
        putc('\n',f);
    }
    fclose(f);
    free(name);
    free(rep);
}

void nouvelle_case(Partie *p){
    int I[16] , J[16];
    int c = case_libre(p,I,J);
    if (c == 0)
        return ;
    int indice = rand()%c;
    if (rand()%10 == 9)
        p->plateau[I[indice]][J[indice]] = 4;
    else
        p->plateau[I[indice]][J[indice]] = 2;
}

void reset_fused(Partie* p){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            p->fused[i][j] = 0;
}


int swipe_left(Partie* p){
    int mouvement = 1, nb_mouvement = 0;
    reset_fused(p);
    while(mouvement){
        mouvement = 0;
        for (int i = 0 ; i < 4 ; i++){
            for (int j = 0 ; j < 3 ; j++){
                if (p->plateau[i][j+1] != 0 && p->plateau[i][j] == 0){
                    p->plateau[i][j] = p->plateau[i][j+1];
                    p->plateau[i][j+1] = 0;
                    p->fused[i][j] = p->fused[i][j+1];
                    p->fused[i][j+1] = 0;
                    mouvement++;
                    nb_mouvement++;
                }
                if (p->plateau[i][j+1]!=0  && p->plateau[i][j] == p->plateau[i][j+1] && !(p->fused[i][j]) && !(p->fused[i][j+1])){
                    p->plateau[i][j] += p->plateau[i][j+1];
                    p->plateau[i][j+1] = 0;
                    p->fused[i][j] = 1;
                    mouvement++;
                    nb_mouvement++;
                    p->score += p->plateau[i][j];
                }
            }
        }

    }
    if (nb_mouvement)
        return 1;
    return 0;
}

int swipe_right(Partie* p){
    int mouvement = 1, nb_mouvement = 0;
    reset_fused(p);
    while(mouvement){
        mouvement = 0;
        for (int i = 0 ; i < 4 ; i++){
            for (int j = 3 ; j >=1 ; j--){
                if (p->plateau[i][j-1]!=0 && p->plateau[i][j] == 0){
                    p->plateau[i][j] = p->plateau[i][j-1];
                    p->plateau[i][j-1] = 0;
                    p->fused[i][j] = p->fused[i][j-1];
                    p->fused[i][j-1] = 0;
                    mouvement++;
                    nb_mouvement++;
                }
                if (p->plateau[i][j-1]!=0  && p->plateau[i][j-1] == p->plateau[i][j] && !(p->fused[i][j]) && !(p->fused[i][j-1])){
                    p->plateau[i][j] += p->plateau[i][j-1];
                    p->plateau[i][j-1] = 0;
                    p->fused[i][j] = 1;
                    mouvement++;
                    nb_mouvement++;
                    p->score += p->plateau[i][j];
                }
            }
        }

    }
    if (nb_mouvement)
        return 1;
    return 0;
}

int swipe_down(Partie* p){
    int mouvement = 1, nb_mouvement = 0;
    reset_fused(p);
    while(mouvement){
        mouvement = 0;
        for (int j = 0 ; j < 4 ; j++){
            for (int i = 3 ; i >=1 ; i--){
                if (p->plateau[i-1][j]!=0 && p->plateau[i][j] == 0){
                    p->plateau[i][j] = p->plateau[i-1][j];
                    p->plateau[i-1][j] = 0;
                    p->fused[i][j] = p->fused[i-1][j];
                    p->fused[i-1][j] = 0;
                    mouvement++;
                    nb_mouvement++;
                }
                if (p->plateau[i-1][j]!=0  && p->plateau[i-1][j] == p->plateau[i][j] && !(p->fused[i][j]) && !(p->fused[i-1][j])){
                    p->plateau[i][j] += p->plateau[i-1][j];
                    p->plateau[i-1][j] = 0;
                    p->fused[i][j] = 1;
                    mouvement++;
                    nb_mouvement++;
                    p->score += p->plateau[i][j];
                }
            }
        }

    }
    if (nb_mouvement)
        return 1;
    return 0;
}

int swipe_up(Partie* p){
    int mouvement = 1, nb_mouvement = 0;
    reset_fused(p);
    while(mouvement){
        mouvement = 0;
        for (int j = 0 ; j < 4 ; j++){
            for (int i = 0 ; i < 3 ; i++){
                if (p->plateau[i+1][j]!=0 && p->plateau[i][j] == 0){
                    p->plateau[i][j] = p->plateau[i+1][j];
                    p->plateau[i+1][j] = 0;
                    p->fused[i][j] = p->fused[i+1][j];
                    p->fused[i+1][j] = 0;
                    mouvement++;
                    nb_mouvement++;
                }
                if (p->plateau[i+1][j]!=0  && p->plateau[i+1][j] == p->plateau[i][j] && !(p->fused[i][j]) && !(p->fused[i+1][j])){
                    p->plateau[i][j] += p->plateau[i+1][j];
                    p->plateau[i+1][j] = 0;
                    p->fused[i][j] = 1;
                    mouvement++;
                    nb_mouvement++;
                    p->score += p->plateau[i][j];
                }
            }
        }

    }
    if (nb_mouvement)
        return 1;
    return 0;
}

void End_Of_Game(Partie* p, int * poiteur_FDP, int *test){
    Partie copie;
    char choix;
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++){
            if ((copie.plateau[i][j] = p->plateau[i][j]) == 2048 && *test){
                Affiche_Partie(p);
                puts("Bravo! vous avez gagné! Voulez-vous continuer ? O\\n\n");
                scanf("%c",&choix);
                while (getchar() != '\n') {}
                if (choix == 'O')
                    *test = 0;
                else 
                    *poiteur_FDP = 0;
            }
        }
    if (!(swipe_down(&copie) || swipe_up(&copie) || swipe_right(&copie) || swipe_left(&copie))){
        Affiche_Partie(p);
        puts("\nFin de partie. Plus de mouvement possible.\n");
        printf("score  :  %d\n",p->score);
        printf("nombre de coup  :  %d\n",p->nb_coup);
        sleep (7);
        *poiteur_FDP = 0;
    }
}

void jeu(Partie* p){
    char direction;
    int fin_de_partie = 1, test = 1;
    while (fin_de_partie){
        Affiche_Partie(p);
        printf("gauche (g), droite (d), haut(h), bas(b)                         (quitter (q))\n");
        scanf("%c",&direction);
        while (getchar() != '\n') {}
        if(direction == 'g' && swipe_left(p)){
            p->nb_coup ++;
            nouvelle_case(p);
        }
        else if(direction == 'd' && swipe_right(p)){
            p->nb_coup ++;
            nouvelle_case(p);
        }
        else if(direction == 'b' && swipe_down(p)){
            p->nb_coup ++;
            nouvelle_case(p);
        }
        else if(direction == 'h' && swipe_up(p)){
            p->nb_coup ++;
            nouvelle_case(p);
        }
        else if(direction == 'q'){
            while(1){
                puts("Voulez-vous sauvegarder votre partie avant de quitter? O\\n");
                scanf("%c",&direction);
                while (getchar() != '\n') {}
                if (direction == 'O'){
                    sauvegarde_partie(p);
                    return ;
                }
                else if (direction == 'n')
                    return;
                puts("Désolé je ne vous ai pas compris.");

            }
        }
        (End_Of_Game(p, &fin_de_partie,&test));
    }
}

void AffichageSDL(Partie* p, SDL_Surface* ecran, SDL_Surface* cases,
                  SDL_Surface* score, TTF_Font* police, SDL_Surface* texte,
                  char temps[], int tempsActuel, int tempsPrecedent,
                  int compteur) {
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
    positionCase.x = 470;
    positionCase.y = 55;
    score = IMG_Load("image/score.png");
    SDL_BlitSurface(score, NULL, ecran, &positionCase);

    SDL_Color couleur = {80, 80, 80};
    SDL_Color couleurNoire = {0, 0, 0}, couleurBlanche = {255, 255, 255};
    char c_score[8];
    sprintf(c_score, "%d", p->score);

    texte = TTF_RenderText_Solid(police, c_score, couleur);
    positionCase.x = 600;
    positionCase.y = 72;
    SDL_BlitSurface(texte, NULL, ecran, &positionCase);

    tempsActuel = SDL_GetTicks();
    if (tempsActuel - tempsPrecedent >=
        100) /* Si 100 ms au moins se sont écoulées */
    {
        compteur += 100; /* On rajoute 100 ms au compteur */
        sprintf(
            temps, "Temps : %d",
            compteur); /* On écrit dans la chaîne "temps" le nouveau temps */
        SDL_FreeSurface(texte); /* On supprime la surface précédente */
        texte = TTF_RenderText_Shaded(
            police, temps, couleurNoire,
            couleurBlanche); /* On écrit la chaîne temps dans la SDL_Surface */
        tempsPrecedent = tempsActuel; /* On met à jour le tempsPrecedent */
    }

    positionCase.x = 377;
    positionCase.y = 800;
    SDL_BlitSurface(texte, NULL, ecran, &positionCase); /* Blit du texte */
    SDL_Flip(ecran);
}

void End_Of_SDL(Partie* p, int* poiteur_FDP, int* test, SDL_Surface* ecran,
                SDL_Surface* cases, SDL_Surface* score, TTF_Font* police,
                SDL_Surface* texte, char temps[], int tempsActuel,
                int tempsPrecedent, int compteur) {
    Partie copie;
    char choix;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            if ((copie.plateau[i][j] = p->plateau[i][j]) == 2048 && *test) {
                AffichageSDL(p, ecran, cases, score, police, texte, temps,
                             tempsActuel, tempsPrecedent, compteur);
                sleep(2);
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
        AffichageSDL(p, ecran, cases, score, police, texte, temps, tempsActuel,
                     tempsPrecedent, compteur);
        sleep(1);
        SDL_Surface* plateau = IMG_Load("image/game_over.png");
        SDL_Rect position;
        position.x = 150;
        position.y = 200;
        SDL_BlitSurface(plateau, NULL, ecran, &position);
        SDL_Color couleurNoire = {100, 100, 100};
        char c_score[8];
        sprintf(c_score, "%d", p->score);
        texte = TTF_RenderText_Solid(police, c_score, couleurNoire);
        position.x = 430 - 10*longueur_nombre(p->score);
        position.y = 550;
        SDL_BlitSurface(texte, NULL, ecran, &position);
        SDL_Flip(ecran);
        *poiteur_FDP = 0;
        sleep(7);
    }
}

void jeuSDL(Partie* p, SDL_Surface* ecran, SDL_Surface* cases,
            SDL_Surface* score, TTF_Font* police, SDL_Surface* texte,
            char temps[], int tempsActuel, int tempsPrecedent, int compteur) {
    SDL_Event event;
    int fin_de_partie = 1, test = 1;
    AffichageSDL(p, ecran, cases, score, police, texte, temps, tempsActuel,
                     tempsPrecedent, compteur);
    while (fin_de_partie) {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:  // quitte et ferme la fenêtre
                exit;
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
                } else if (event.key.keysym.sym == SDLK_ESCAPE)
                    exit;
                AffichageSDL(p, ecran, cases, score, police, texte, temps, tempsActuel,
                     tempsPrecedent, compteur);
                (End_Of_SDL(p, &fin_de_partie, &test, ecran, cases, score,
                            police, texte, temps, tempsActuel, tempsPrecedent,
                            compteur));
                break;
        }
    }
}


int SDL_launch(Partie* p) {
    srand(time(NULL));
    SDL_Surface* ecran = NULL;
    SDL_Surface* plateau = NULL;
    SDL_Surface* cases = NULL;
    SDL_Surface* icone = NULL;
    SDL_Surface* score = NULL;
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

    TTF_Font* police = NULL;
    police = TTF_OpenFont("C800.ttf", 80);

    SDL_Color couleur_2048 = {227, 157, 66};

    texte = TTF_RenderText_Blended(police, "2048", couleur_2048);

    icone = IMG_Load("image/icone2.png");
    SDL_WM_SetIcon(icone, NULL);

    // ouvre une fenêtre de taille 800 x 600 en 32 bits / pixel
    ecran = SDL_SetVideoMode(900, 900, 32, SDL_HWSURFACE);

    // donne un titre à la fenêtre
    SDL_WM_SetCaption("2048", NULL);

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 248, 239, 222));

    position.x = 160;
    position.y = 75;

    SDL_BlitSurface(texte, NULL, ecran, &position);

    plateau = IMG_Load("image/plateau.png");
    cases = IMG_Load("image/0.png");

    position.x = 150;
    position.y = 200;

    // Collage de la surface sur l'écran
    SDL_BlitSurface(plateau, NULL, ecran, &position);

    police = TTF_OpenFont("C800.ttf", 45);
    int tempsActuel = 0, tempsPrecedent = 0, compteur = 0;
    char temps[20] = "";

    // Initialisation du temps
    tempsActuel = SDL_GetTicks();
    sprintf(temps, "%d", compteur);
    police = TTF_OpenFont("C800.ttf", 45);

    jeuSDL(p, ecran, cases, score, police, texte, temps, tempsActuel,
           tempsPrecedent, compteur);

    SDL_FreeSurface(cases);  // Libération de la surface
    SDL_FreeSurface(plateau);
    SDL_FreeSurface(ecran);
    SDL_FreeSurface(icone);
    SDL_FreeSurface(score);
    SDL_FreeSurface(texte);
    free(p);

    TTF_CloseFont(police);

    TTF_Quit();
    SDL_Quit();  // Arrêt de la SDL (libération de la mémoire).
    

    return EXIT_SUCCESS;  // fermeture du programme
}