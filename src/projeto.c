#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../lib/ficheiros.h"
#include "../lib/operacoes.h"



//Menu inicial
void inicio(Doente_header *lista_doentes){
    int opcao;
    do {
        printf("Insira o que quer fazer: \n");
        printf("1. Introduzir dados de um novo paciente\n 2. Eliminar um paciente existente.\n 3. Listar todos os pacientes por ordem alfabética\n 4. Listar os pacientes com tensões máximas acima de um determinado valor\n");
        printf("5. Apresentar toda a informação de um determinado paciente\n 6. Registar as tensões, o peso e a altura de um determinado paciente num determinado dia\n 7. Sair da aplicação\n");
        printf("\nEscolha uma opção: ");
        

        opcao = verificarNum(8,0," "); //Verificar se numero está entre 7 e 1 ,(inclusivo)


        if (opcao < 1 || opcao > 7) {
            printf("Opcao invalida. Tente novamente.\n");
            continue;
        }
        switch (opcao) {
            case 1:
                inserirD(lista_doentes);
                break;
            case 2:
                apagarD(lista_doentes);
                break;
            case 3:
                listarD(lista_doentes);
                break;
            case 4:
                listar_doentes_tensao_sup(lista_doentes);
                break;
            case 5:
                info_doente(lista_doentes);
                break;
            case 6:
                registar_dados(lista_doentes);
                break;
            case 7:
            limpar(lista_doentes);
                break;
                default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
            }
    } while (opcao != 7);

    
}

int main(){
    Doente_header *lista_doentes = (Doente_header *)malloc(sizeof(Doente_header));
    inicializar_doentes(lista_doentes);
    ler_ficheiro_doentes(lista_doentes);
    ler_ficheiro_registos(lista_doentes);
    inicio(lista_doentes);
    free(lista_doentes);
}