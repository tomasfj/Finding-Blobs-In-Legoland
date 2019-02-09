typedef struct {
    int R, G, B;
    int estado;  //1 quando guardado, 0 quando for achado durante a pesquisa 
    int add_lista;
}Pixel; 

typedef struct RESULT {
    char nome[100];
    int x, y, size;
    struct RESULT *nseg;
    struct RESULT *nant;
}Result;

typedef struct PORLER {
    int x, y;
    struct PORLER *nseg;
}Porler;

