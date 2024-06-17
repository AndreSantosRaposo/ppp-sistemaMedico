#ifndef STRUCTS_H
#define STRUCTS_H
#define MAX_TAM 50

typedef struct data{
    int dia;
    int mes;
    int ano;
}Data;

typedef struct registo_info{
    Data data_res;
    int tensao_max;
    int tensao_min;
    int peso;
    int altura;
    struct registo_info *prox;
}Registo_info;

typedef struct registo_header{
    Registo_info *start;
}Registo_header;

typedef struct doente{
    int id_doente;
    char nome [MAX_TAM];    
    Data data_nasc;
    char CC[MAX_TAM];
    int telemovel;
    char email[MAX_TAM];
    Registo_header *registos;
    struct doente *prox;
}Doente;

typedef struct doente_header{
    Doente *start;
}Doente_header;

typedef struct aux{
    Doente *start;
    struct aux *prox;
}aux;

//Meter aux dentro do doente_header, tipo start doente e start aux


#endif