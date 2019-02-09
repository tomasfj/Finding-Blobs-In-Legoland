#ifndef FUNCOES_LISTAS_H
#define FUNCOES_LISTAS_H

Result *makeNode(char *nome, int x, int y, int size) {
    Result *nv;
    nv = (Result*)malloc(sizeof(Result));
    
    strcpy(nv->nome, nome);
    nv->x = x;
    nv->y = y;
    nv->size = size;
    nv->nseg = NULL;
    nv->nant = NULL;
    
    return(nv);
}

Porler *p_makeNode(int x, int y) {
    Porler *nv;
    nv = (Porler*)malloc(sizeof(Porler));

    nv->x = x;
    nv->y = y;
    nv->nseg = NULL;

    return(nv);
}

Result *insertFirst(Result *L, Result *nv) {
    nv->nseg = L;
    nv->nant = NULL;
    L->nant = nv;

    return(nv);
}

Result *insertLast(Result *L, Result *nv) {
    Result *aux = L;
    
    if(aux == NULL)
        return(nv);

    while(aux->nseg != NULL) {
        aux = aux->nseg;
    }

    aux->nseg = nv;
    nv->nant = aux;

    return(L);
}

// adicionar posições da lista de elementos por correr
Porler *p_insertFirst(Porler *L, Porler *nv) {
    (*nv).nseg = L;

    return(nv);
}
// eliminar posições da lista de elementos por correr
Porler *removeFirst(Porler *L) {
    if(L == NULL)       //Verificação caso L esteja vazia
        return(L);

    Porler *aux = L;

    aux = (*aux).nseg;  //avançar aux para a segunda posição
    
    free(L);       //liberta-se o primeiro elemento

    return(aux);
}


Result *insertOrder(Result *L, Result *nv) {
    Result *aux = L;
    
    while(aux != NULL) {
        if(aux->size < nv->size)
            break;
        aux = aux->nseg;
    }
    
    if(aux == NULL)
        return(insertLast(L, nv));

    if(aux->nant == NULL) {
        return(insertFirst(L, nv));
    }

    nv->nseg = aux;
    nv->nant = aux->nant;
    aux->nant = nv;
    nv->nant->nseg = nv;
    
    return(L);
}

#endif