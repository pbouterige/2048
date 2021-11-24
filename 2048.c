#include "fonction.c"

void jeu(Partie* p){
    char direction;
    int fin_de_partie = 1;
    while (fin_de_partie){
        Affiche_Partie(p);
        printf("direction : ");
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
                puts("voulez-vous sauvegarder votre partie avant de quitter? O\\n");
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
        (End_Of_Game(p, &fin_de_partie));
    }
}


int main(){
    srand(time(NULL));
    Partie *nv = nouvelle_partie();
    jeu(nv);


    return 0;
}
