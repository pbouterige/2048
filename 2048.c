#include "fonction.c"

void menu() {
    int choix = 0;
    char sdl = 0;
    while (choix != 1 && choix != 2 && choix != 3)
    {
        puts("\nDémarrer une nouvelle partie         [1]");
        puts("Continuer une partie en cours        [2]");
        puts("Quitter                              [3]\n");
        scanf("%d", &choix);
        
        if (choix == 1)  //commence une nouvelle partie
        {
            while (getchar() != '\n'){}
            Partie *nv = nouvelle_partie();
            puts(" Voulez vous jouer en SDL ? o/n");
            scanf("%c", &sdl);
            while (getchar() != '\n'){}
            if (sdl == 'o')
                SDL_launch(nv);
            else
                jeu(nv);
            menu();
        }
        else if (choix == 2) //reprend une ancienne partie sauvegardée
        {
            while (getchar() != '\n'){}
            Partie *nv = charger_partie();
            puts(" Voulez vous jouer en SDL ? o/n");
            scanf("%c", &sdl);
            while (getchar() != '\n'){}
            if (sdl == 'n')
                jeu(nv);
            else 
                SDL_launch(nv);
            menu();
        }
        else if (choix == 3) //quitte le jeu
        {
            exit;
        }
        else
        {
            puts("choix incorrect");
            while (getchar() != '\n'){}
        }
    }
}

int main(){
    srand(time(NULL));
    menu();

    return 0;
}
