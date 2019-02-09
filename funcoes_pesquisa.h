#ifndef FUNCOES_PESQUISA_H
#define FUNCOES_PESQUISA_H

//Função para devolver os valores maximo e minimo do intervalo de cor escolhido(Rmin, Rmax, etc...)
int acharIntervalo(int cor, int tol, int min_max) {     //para calcular min -> min_max=0, para calcular max -> min_max=1
    if(min_max == 0) {
        cor = cor - tol;
        if(cor < 0) {
            cor = 0;
        }
    }
    else {
        cor = cor + tol;
        if(cor > 255) {
            cor = 255;
        }
    }
    return(cor);
}

//Função para verificar se pixel atual pertence ao intervalo de cor procurado
int eBlob(int Rmin, int Rmax, int Gmin, int Gmax, int Bmin, int Bmax, int r, int g, int b) {
    if((r >= Rmin) && (r <= Rmax)) {
        if((g >= Gmin) && (g <= Gmax)) {
            if((b >= Bmin) && (b <= Bmax)) {
                return(1);
            }
        }
    }
    return(0);
}

#endif