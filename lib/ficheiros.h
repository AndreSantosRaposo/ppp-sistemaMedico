#ifndef FICHEIROS_H
#define FICHEIROS_H
#include "./estruturas.h"

int validar_data(int dia, int mes,int ano);
void ler_ficheiro_doentes(Doente_header *lista_doentes);
void ler_ficheiro_registos(Doente_header *lista_doentes);
void escrever_ficheiro_doentes(Doente *novo);
void escrever_ficheiro_registos(Registo_info *novo, int id_novo_registo);
void apagar_ficheiro_doentes(int id);
void apagar_ficheiro_registos(int id);

#endif