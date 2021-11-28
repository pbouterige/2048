#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


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
    while (f==NULL){
        puts("\nVeuillez préciser le nom exacte de votre sauvegarde");
        char * name = (char *) malloc( 40 );;
        scanf("%s",name);
        rep = (char *) malloc( 15 + strlen(name) );

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

}

int nouvelle_case(Partie *p){
    int I[16] , J[16];
    int c = case_libre(p,I,J);
    if (c == 0)
        return 0;
    int indice = rand()%c;
    if (rand()%10 == 9)
        p->plateau[I[indice]][J[indice]] = 4;
    else
        p->plateau[I[indice]][J[indice]] = 2;
    return 1;
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