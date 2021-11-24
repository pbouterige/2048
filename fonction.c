#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct Partie
{
    int plateau[4][4];
    int score;
    int nb_coup;

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
        for (int j = 0 ; j < 4 ; j++)
            P->plateau[i][j] = 0;
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

void Affiche_Partie(Partie * p){
    puts("");
    printf("score : %d                        coup : %d \n",p->score,p->nb_coup);
    puts("-----------------------------------------\n");
    puts("        ----   ----   ----   ----        ");
    printf("       | %d  | | %d  | | %d  | | %d  |       \n",p->plateau[0][0],p->plateau[0][1],p->plateau[0][2],p->plateau[0][3]);
    puts("        ----   ----   ----   ----        ");
    printf("       | %d  | | %d  | | %d  | | %d  |       \n",p->plateau[1][0],p->plateau[1][1],p->plateau[1][2],p->plateau[1][3]);
    puts("        ----   ----   ----   ----        ");
    printf("       | %d  | | %d  | | %d  | | %d  |       \n",p->plateau[2][0],p->plateau[2][1],p->plateau[2][2],p->plateau[2][3]);
    puts("        ----   ----   ----   ----        ");
    printf("       | %d  | | %d  | | %d  | | %d  |       \n",p->plateau[3][0],p->plateau[3][1],p->plateau[3][2],p->plateau[3][3]);
    puts("        ----   ----   ----   ----        ");
    puts("\n-----------------------------------------\n");

 
}


void sauvegarde_partie(Partie *p){
    puts("Quel nom donnez-vous à votre sauvegarde?");
    char * name = (char *) malloc( 40 );;
    scanf("%s",name);
    char * rep = (char *) malloc( 15 + strlen(name) );

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

int swipe_left(Partie* p){
    int mouvement = 1, nb_mouvement = 0;
    while(mouvement){
        mouvement = 0;
        for (int i = 0 ; i < 4 ; i++){
            for (int j = 0 ; j < 3 ; j++){
                if (p->plateau[i][j+1] != 0 && p->plateau[i][j] == 0){
                    p->plateau[i][j] = p->plateau[i][j+1];
                    p->plateau[i][j+1] = 0;
                    mouvement++;
                    nb_mouvement++;
                }
                if (p->plateau[i][j+1]!=0  && p->plateau[i][j] == p->plateau[i][j+1]){
                    p->plateau[i][j] += p->plateau[i][j+1];
                    p->plateau[i][j+1] = 0;
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
    while(mouvement){
        mouvement = 0;
        for (int i = 0 ; i < 4 ; i++){
            for (int j = 3 ; j >=1 ; j--){
                if (p->plateau[i][j-1]!=0 && p->plateau[i][j] == 0){
                    p->plateau[i][j] = p->plateau[i][j-1];
                    p->plateau[i][j-1] = 0;
                    mouvement++;
                    nb_mouvement++;
                }
                if (p->plateau[i][j-1]!=0  && p->plateau[i][j-1] == p->plateau[i][j]){
                    p->plateau[i][j] += p->plateau[i][j-1];
                    p->plateau[i][j-1] = 0;
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
    while(mouvement){
        mouvement = 0;
        for (int j = 0 ; j < 4 ; j++){
            for (int i = 3 ; i >=1 ; i--){
                if (p->plateau[i-1][j]!=0 && p->plateau[i][j] == 0){
                    p->plateau[i][j] = p->plateau[i-1][j];
                    p->plateau[i-1][j] = 0;
                    mouvement++;
                    nb_mouvement++;
                }
                if (p->plateau[i-1][j]!=0  && p->plateau[i-1][j] == p->plateau[i][j]){
                    p->plateau[i][j] += p->plateau[i-1][j];
                    p->plateau[i-1][j] = 0;
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
    while(mouvement){
        mouvement = 0;
        for (int j = 0 ; j < 4 ; j++){
            for (int i = 0 ; i < 3 ; i++){
                if (p->plateau[i+1][j]!=0 && p->plateau[i][j] == 0){
                    p->plateau[i][j] = p->plateau[i+1][j];
                    p->plateau[i+1][j] = 0;
                    mouvement++;
                    nb_mouvement++;
                }
                if (p->plateau[i+1][j]!=0  && p->plateau[i+1][j] == p->plateau[i][j]){
                    p->plateau[i][j] += p->plateau[i+1][j];
                    p->plateau[i+1][j] = 0;
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

void End_Of_Game(Partie* p, int * poiteur_FDP){

}