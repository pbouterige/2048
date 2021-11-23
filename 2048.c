#include <stdio.h>
#include <stdlib.h>


typedef struct Partie
{
    int plateau[4][4];
    int score;
    int nb_coup;

}Partie;

Partie * nouvelle_partie(){
    Partie * P = (Partie*)malloc(sizeof(Partie));
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            P->plateau[i][j] = 0;
    P ->score = 0;
    P ->nb_coup = 0;
    return P;
}

void Affiche_Partie(Partie * p){
    puts("");
    printf("score : %d                        coup : %d \n",p->nb_coup,p->score);
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



int main(){
    Partie *nv = nouvelle_partie();
    Affiche_Partie(nv);



    return 0;
}
