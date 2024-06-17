#ifndef OPERACOES_H
#define OPERACOES_H
#include "./estruturas.h"

void erroNum(int err);
void limpar_buffer();
int leapYear(int ano);
void inicializar_doentes(Doente_header *lista_doentes);
int verifica_id(Doente_header *lista_doentes, int id);
void inserirD(Doente_header *lista_doentes);
void apagarD(Doente_header *lista_doentes);
void listarD(Doente_header *lista_doentes);
int inserir_doente_na_lista_aux(aux *lista_aux, Doente *novo_doente);
void listar_doentes_tensao_sup(Doente_header *lista_doentes);
void info_doente(Doente_header *lista_doentes);
int registar_dados(Doente_header *lista_doentes);
int verificarNum(int limMax, int limMin, char str[]);
void erroNome(int err);
int verificarData(Data *dat);
void erroEmail(int err);
int verificarEmail(char *email);
void limpar(Doente_header *lista_doentes);
int verificar_CC(unsigned int lim ,char str2[], char* str);


#endif