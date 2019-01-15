#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "estruturas.h"
#include "funcoes_pesquisa.h"
#include "funcoes_listas.h"


int main() {
    static const char filename[] = "imgs.txt"; 
    char str[50];
    char name_img[50];
    char dim_img[50];
    char *token;  //para strtok
    int i, j, lin, col, c, dim, val, x, y, count, count_lin, count_col;
    int *blopSpecs;
    int user_r, user_g, user_b, user_d, Rmin, Rmax, Gmin, Gmax, Bmin, Bmax;     //para função acharIntervalo()
    int totLinhas, totColunas, totCanais;

    Result *L = NULL, *temp;

    Pixel **px = NULL;

    Porler *PL = NULL, *p_temp;

    FILE *fpointer = fopen(filename, "r");

    if(fpointer == NULL) {
        printf("ERRO AO ABRIR O FICHEIRO\n");
        return(-1);
    }
    
    //Input da cor a procurar
    printf("Indique a cor a procurar\n");
    printf("Valor R: ");
    scanf("%d", &user_r);
    printf("Valor G: ");
    scanf("%d", &user_g);
    printf("Valor B: ");
    scanf("%d", &user_b);
    printf("Intervalo de tolerancia: ");
    scanf("%d", &user_d); 
    
    //guardar intervalos de tolerancia dos 3 canais RGB
    Rmin = acharIntervalo(user_r, user_d, 0);
    Rmax = acharIntervalo(user_r, user_d, 1);
    Gmin = acharIntervalo(user_g, user_d, 0);
    Gmax = acharIntervalo(user_g, user_d, 1);
    Bmin = acharIntervalo(user_b, user_d, 0);
    Bmax = acharIntervalo(user_b, user_d, 1);

    //cada iteração lê uma nova imagem
    while(fgets(name_img, 50, fpointer) != NULL) {
        name_img[strcspn(name_img, "\n")] = 0;      //remove '\n' do  nome

        //ler as dimensões da imagem
        fgets(dim_img, 50, fpointer);
        
        //cast e armazenamento das dimensões da imagem
        token = strtok(dim_img, " ");
        totLinhas = atoi(token);
        
        token = strtok(NULL, " ");
        totColunas = atoi(token);
        
        token = strtok(NULL, " ");
        totCanais = atoi(token);

        //criar matriz para guardar pixeis
        px = (Pixel**)malloc(totLinhas*sizeof(Pixel*));
        for(i=0;i<totLinhas;i++) {
            px[i] = (Pixel*)malloc(totColunas*sizeof(Pixel));
        }

        //leitura dos pixeis
        lin = 0;
        col = 0;
        dim = totLinhas*totColunas*totCanais;   //total de linhas a serem lidas para a imagem atual

        for(i=0,c=0;i<dim;i++,c++) {            //c = canal atual (R = 0, G = 1, B = 2)
            val = atoi(fgets(str, 50, fpointer));       
            
            if(c == 0) {
                px[lin][col].R = val;           //guardar valor R no pixel atual
            }
            if(c == 1) {
                px[lin][col].G = val;           //guardar valor G no pixel atual
            }
            if(c == 2) {
                px[lin][col].B = val;           //guardar valor B no pixel atual
                px[lin][col].estado = 1;        //guardar o estado do pixel atual como 1 (não pesquisado)
                px[lin][col].add_lista = 1;
                c = -1;                         //voltar ao canal R(= 0)  NOTA: c = -1 porque depois no for() faz c++ e fica c = 0
                if(col == totColunas-1) {       //condição caso indice chegue à última coluna reinicia-se o contador das colunas e começa uma nova linha 
                    col = 0;
                    lin ++;
                }
                else {
                    col ++;
                }
            }
        }
        
        // inicialização das variáveis para a pesquisa
        count = 0;
        count_lin = 0;
        count_col = 0;
        
        //Pesquisa
        for(i=0;i<totLinhas;i++) {
            for(j=0;j<totColunas;j++) {
                if(px[i][j].estado == 1) {                                                                      //estado=1 pixel ainda não foi verificado
                    if(eBlob(Rmin, Rmax, Gmin, Gmax, Bmin, Bmax, px[i][j].R, px[i][j].G, px[i][j].B) == 1) {    //eBlob()=1 pixel dentro do intervalo de cor, else fora do intervalo
                        // fase 1 - encontrar novo Blob 
                    
                        x = i;
                        y = j;
                        count ++;
                        count_lin += x;
                        count_col += y;
                        px[x][y].estado = 0;

                        if(y != 0) {
                            if(px[x][y-1].estado == 1) {
                                if(eBlob(Rmin, Rmax, Gmin, Gmax, Bmin, Bmax, px[x][y-1].R, px[x][y-1].G, px[x][y-1].B) == 1) {
                                    //insert first de (x, y-1) na lista_por_correr
                                   if(px[x][y-1].add_lista == 1) {
                                        p_temp = p_makeNode(x,y-1);
                                        PL = p_insertFirst(PL, p_temp);
                                        px[x][y-1].add_lista = 0;
                                   }
                                }
                            }
                        }
                        if(y != totColunas-1) {
                            if(px[x][y+1].estado == 1) {
                                if(eBlob(Rmin, Rmax, Gmin, Gmax, Bmin, Bmax, px[x][y+1].R, px[x][y+1].G, px[x][y+1].B) == 1) {
                                    //insert first de (x, y+1) na lista_por_correr
                                    if(px[x][y+1].add_lista == 1) {
                                        p_temp = p_makeNode(x,y+1);
                                        PL = p_insertFirst(PL, p_temp);
                                        px[x][y+1].add_lista = 0;
                                    }
                                }
                            }
                        }
                        if(x != 0) {
                            if(px[x-1][y].estado == 1) {
                                if(eBlob(Rmin, Rmax, Gmin, Gmax, Bmin, Bmax, px[x-1][y].R, px[x-1][y].G, px[x-1][y].B) == 1) {
                                    //insert first de (x-1, y) na lista_por_correr
                                    if(px[x-1][y].add_lista == 1) {
                                        p_temp = p_makeNode(x-1,y);
                                        PL = p_insertFirst(PL, p_temp);
                                        px[x-1][y].add_lista = 0;
                                }
                            }
                        }
                        if(x != totLinhas-1) {
                            if(px[x+1][y].estado == 1) {
                                    }
                                if(eBlob(Rmin, Rmax, Gmin, Gmax, Bmin, Bmax, px[x+1][y].R, px[x+1][y].G, px[x+1][y].B) == 1) {
                                    //insert first de (x+1, y) na lista_por_correr
                                    if(px[x+1][y].add_lista == 1) {
                                        p_temp = p_makeNode(x+1,y);
                                        PL = p_insertFirst(PL, p_temp);
                                        px[x+1][y].add_lista = 0;
                                    }
                                }
                            }
                        }
                        // fase 2 - entrar em loop
                        while(PL != NULL) {
                            //remove first da lista_por_correr
                            x = PL->x;
                            y = PL->y;
                            PL = removeFirst(PL);

                            count ++;
                            count_lin += x;
                            count_col += y;
                            px[x][y].estado = 0;

                            if(y != 0) {
                                if(px[x][y-1].estado == 1) {
                                    if(eBlob(Rmin, Rmax, Gmin, Gmax, Bmin, Bmax, px[x][y-1].R, px[x][y-1].G, px[x][y-1].B) == 1) {
                                        //insert first de (x, y-1) na lista_por_correr
                                        if(px[x][y-1].add_lista == 1) {
                                            p_temp = p_makeNode(x,y-1);
                                            PL = p_insertFirst(PL, p_temp);
                                            px[x][y-1].add_lista = 0;
                                        }
                                    }
                                }
                            }
                            if(y != totColunas-1) {
                                if(px[x][y+1].estado == 1) {
                                    if(eBlob(Rmin, Rmax, Gmin, Gmax, Bmin, Bmax, px[x][y+1].R, px[x][y+1].G, px[x][y+1].B) == 1) {
                                        //insert first de (x, y+1) na lista_por_correr
                                        if(px[x][y+1].add_lista == 1) {
                                            p_temp = p_makeNode(x,y+1);
                                            PL = p_insertFirst(PL, p_temp);
                                            px[x][y+1].add_lista = 0;
                                        }
                                    }
                                }
                            }
                            if(x != 0) {
                                if(px[x-1][y].estado == 1) {
                                    if(eBlob(Rmin, Rmax, Gmin, Gmax, Bmin, Bmax, px[x-1][y].R, px[x-1][y].G, px[x-1][y].B) == 1) {
                                        //insert first de (x-1, y) na lista_por_correr
                                        if(px[x-1][y].add_lista == 1) {
                                            p_temp = p_makeNode(x-1,y);
                                            PL = p_insertFirst(PL, p_temp);
                                            px[x-1][y].add_lista = 0;
                                        }
                                    }
                                }
                            }
                            if(x != totLinhas-1) {
                                if(px[x+1][y].estado == 1) {
                                    if(eBlob(Rmin, Rmax, Gmin, Gmax, Bmin, Bmax, px[x+1][y].R, px[x+1][y].G, px[x+1][y].B) == 1) {
                                        //insert first de (x+1, y) na lista_por_correr
                                        if(px[x+1][y].add_lista == 1) {
                                            p_temp = p_makeNode(x+1,y);
                                            PL = p_insertFirst(PL, p_temp);
                                            px[x+1][y].add_lista = 0;
                                        }
                                    }
                                }
                            }
                        }
                        // armazenamento de resultados
                        temp = makeNode(name_img, (count_lin/count), (count_col/count), count);
                        L = insertOrder(L, temp);
                        count = 0;
                        count_lin = 0;
                        count_col = 0;
                    }
                }
            }
        }
        
        //no fim da pesquisa libertar a matriz
        for(i=0;i<totLinhas;i++) {
            free(px[i]);
        }
        
    }

    //output de resultados
    printf("\n");
    printf("RESULTADOS ORDENADOS\n");
    while(L != NULL) {
        printf("%s (%d,%d): %d pixeis\n", L->nome, L->x, L->y, L->size);
        L = L->nseg;
    }

    fclose(fpointer);

    return(0);
}
