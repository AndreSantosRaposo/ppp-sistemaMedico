#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../lib/ficheiros.h"




//Ler o ficheiro dos doentes
void ler_ficheiro_doentes(Doente_header *lista_doentes) {
    FILE *file = fopen("doentes.txt", "r");
    char linha[MAX_TAM];
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de doentes.\n");
        return;
    }
    
    Doente *novo = NULL;
    Doente *atual = lista_doentes->start;
    Doente *anterior = NULL;
    
    while (fgets(linha, sizeof(linha), file) != NULL) {
        if (strcmp(linha, "\n") == 0) {
            continue;
        }
        
        novo = (Doente *)malloc(sizeof(Doente));
        if (novo == NULL) {
            printf("Erro ao alocar memória para o novo doente.\n");
            fclose(file);
            return;
        }
        
        if (sscanf(linha, "%d", &novo->id_doente) == 1) {
            fgets(novo->nome, MAX_TAM, file);
            novo->nome[strcspn(novo->nome, "\n")] = '\0';
            
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "%d/%d/%d", &novo->data_nasc.dia, &novo->data_nasc.mes, &novo->data_nasc.ano);

            
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "%s", novo->CC);
            
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "%d", &novo->telemovel);
            
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "%s", novo->email);
            
            novo->registos = (Registo_header *)malloc(sizeof(Registo_header)); 
            novo->registos->start = NULL; 
            
            novo->prox = NULL;

            while (atual != NULL && strcmp(atual->nome, novo->nome) < 0) {
                anterior = atual;
                atual = atual->prox;
            }
          
            if (anterior == NULL) {
                novo->prox = lista_doentes->start;
                lista_doentes->start = novo;
            } else {
                anterior->prox = novo;
                novo->prox = atual;
            }
            
            atual = lista_doentes->start;
            anterior = NULL;
        }
    }
    fclose(file);

}

//Ler o ficheiro dos registos
void ler_ficheiro_registos(Doente_header *lista_doentes) {
    FILE *file = fopen("registos.txt", "r");
    char linha[MAX_TAM];
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de registros.\n");
        return;
    }
    
    Registo_info *novo_registo = NULL;
    Doente *atual_doente = lista_doentes->start;
    
    while (fgets(linha, sizeof(linha), file) != NULL) {
        if (strcmp(linha, "\n") == 0 || strcmp(linha, "\r\n") == 0) {
            continue;
        }
        
        novo_registo = (Registo_info *)malloc(sizeof(Registo_info));
        
        if (novo_registo == NULL) {
            printf("Erro ao alocar memória para o novo registro.\n");
            fclose(file);
            return;
        }
        int id_registo;

        if (sscanf(linha, "%d", &id_registo) == 1) {
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "%d/%d/%d", &novo_registo->data_res.dia, &novo_registo->data_res.mes, &novo_registo->data_res.ano);
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "%d", &novo_registo->tensao_max);
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "%d", &novo_registo->tensao_min);
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "%d", &novo_registo->peso);
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "%d", &novo_registo->altura);
            novo_registo->prox = NULL;
            
            while (atual_doente != NULL && atual_doente->id_doente != id_registo) {
                atual_doente = atual_doente->prox;
            }
            
            if (atual_doente != NULL) {
                Registo_info *anterior = NULL;
                Registo_info *atual_registo = atual_doente->registos->start;
                
                               
                while (atual_registo != NULL && novo_registo->tensao_max < atual_registo->tensao_max) {
                    anterior = atual_registo;
                    atual_registo = atual_registo->prox;
                }
                
                if (anterior == NULL) {
                    novo_registo->prox = atual_doente->registos->start;
                    atual_doente->registos->start = novo_registo;
                } else {
                    anterior->prox = novo_registo;
                    novo_registo->prox = atual_registo;
                }
            } else {
                printf("Erro: ID de paciente não encontrado na lista de pacientes.\n");
                free(novo_registo);
            }
        }
        atual_doente = lista_doentes->start;
    }
    fclose(file);
}


//Adicionar doente ao ficheiro
void escrever_ficheiro_doentes(Doente *novo){
    FILE *file = fopen("doentes.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }
    fprintf(file,"%d\n", novo->id_doente);
    fprintf(file,"%s\n", novo->nome);
    fprintf(file,"%d/%d/%d\n", novo->data_nasc.dia,novo->data_nasc.mes,novo->data_nasc.ano);
    fprintf(file,"%s\n", novo->CC);
    fprintf(file,"%d\n", novo->telemovel);
    fprintf(file,"%s\n", novo->email);
    fclose(file);
}

//Adicionar registo ao ficheiro
void escrever_ficheiro_registos(Registo_info *novo, int id_novo_registo){
    FILE *file = fopen("registos.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }
    fprintf(file,"%d\n", id_novo_registo);
    fprintf(file,"%d/%d/%d\n", novo->data_res.dia,novo->data_res.mes,novo->data_res.ano);
    fprintf(file,"%d\n", novo->tensao_max);
    fprintf(file,"%d\n", novo->tensao_min);
    fprintf(file,"%d\n", novo->peso);
    fprintf(file,"%d\n", novo->altura);
    fclose(file);
}

//Limpar o doente do ficheiro e consequentemente do "sistema"
void apagar_ficheiro_doentes(int id) {
    FILE *arquivo_leitura = fopen("doentes.txt", "r");
    FILE *arquivo_escrita = fopen("temp.txt", "w");
    int id_atual;
    char linha[MAX_TAM];
    int id_encontrado = 0;


    if (arquivo_leitura == NULL) {
        perror("Erro ao abrir o arquivo de leitura");
        exit(1);
    }

    if (arquivo_escrita == NULL) {
        perror("Erro ao abrir o arquivo de escrita");
        fclose(arquivo_leitura);
        exit(1);
    }

    
    while (fgets(linha, sizeof(linha), arquivo_leitura) != NULL) {
        if (sscanf(linha, "%d", &id_atual) == 1) {
            if (id_atual == id) {
                id_encontrado = 1;
                // Pular as próximas 5 linhas (nome, data de nascimento, CC, telemovel, email)
                for (int i = 0; i < 5; i++) {
                    if (fgets(linha, sizeof(linha), arquivo_leitura) == NULL) {
                        printf("Erro: Formato de arquivo inválido.\n");
                        fclose(arquivo_leitura);
                        fclose(arquivo_escrita);
                        remove("temp.txt");
                        return;
                    }
                }
            } else {
                fprintf(arquivo_escrita, "%s", linha);
            }
        } else {
            fprintf(arquivo_escrita, "%s", linha);
        }
    }

    fclose(arquivo_leitura);
    fclose(arquivo_escrita);

    if (id_encontrado) {
        remove("doentes.txt");
        rename("temp.txt", "doentes.txt");
    } else {
        printf("ID %d não encontrado no arquivo.\n", id);
        remove("temp.txt");
    }
}



//Limpar o registo do doente do ficheiro e consequentemente do "sistema"
void apagar_ficheiro_registos(int id) {
    FILE *arquivo_leitura = fopen("registos.txt", "r");
    FILE *arquivo_escrita = fopen("temp.txt", "w");
    int id_atual;
    char linha[MAX_TAM];
    int id_encontrado = 0;

    if (arquivo_leitura == NULL) {
        perror("Erro ao abrir o arquivo de leitura");
        exit(1);
    }

    if (arquivo_escrita == NULL) {
        perror("Erro ao abrir o arquivo de escrita");
        fclose(arquivo_leitura);
        exit(1);
    }

    while (fgets(linha, sizeof(linha), arquivo_leitura) != NULL) {
        if (sscanf(linha, "%d", &id_atual) == 1) {
            if (id_atual == id) {
                id_encontrado = 1;
                // Pular as próximas 5 linhas (data do registro, tensao_max, tensao_min, peso, altura)
                for (int i = 0; i < 5; i++) {
                    if (fgets(linha, sizeof(linha), arquivo_leitura) == NULL) {
                        printf("Erro: Formato de arquivo inválido.\n");
                        fclose(arquivo_leitura);
                        fclose(arquivo_escrita);
                        remove("temp.txt");
                        return;
                    }
                }
            } else {
                fprintf(arquivo_escrita, "%s", linha);
            }
        } else {
            fprintf(arquivo_escrita, "%s", linha);
        }
    }

    fclose(arquivo_leitura);
    fclose(arquivo_escrita);

    if (id_encontrado) {
        remove("registos.txt");
        rename("temp.txt", "registos.txt");
    } else {
        printf("ID %d não encontrado no arquivo.\n", id);
        remove("temp.txt");
    }
}

