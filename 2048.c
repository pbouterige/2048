#include "fonction.c"


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


void menu() {
    int choix = 0;
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
            jeu(nv);
            menu();
        }
        else if (choix == 2) //reprend une ancienne partie sauvegardée
        {
            Partie *nv = charger_partie();
            jeu(nv);
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
