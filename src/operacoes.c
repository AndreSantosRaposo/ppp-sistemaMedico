#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../lib/ficheiros.h"
#include "../lib/operacoes.h"



//Inicializar a lista com os doentes
void inicializar_doentes(Doente_header *lista_doentes){
    lista_doentes->start = NULL;
}

    //verifica existencia
int verifica_id(Doente_header *lista_doentes, int id){
    Doente *cur=lista_doentes->start;
    while(cur!=NULL){
        if(cur->id_doente==id){
            return 1;
        }
        cur=cur->prox;
    }
    return 0;
}

    //Função que insere o doente na lista
void inserirD(Doente_header *lista_doentes) {
    Doente *novo = (Doente *)malloc(sizeof(Doente));
    Doente *cur = lista_doentes->start;
    Doente *anterior = NULL;
    
    //Pedir o id
    novo->id_doente = verificarNum(0,0,"Insira o ID do doente: ");

    if (!verifica_id(lista_doentes, novo->id_doente)) {
        char *temp = (char *)malloc(sizeof(char)*MAX_TAM);
        int erro;
        int temErro;
    //Pedir o nome do paciente         ---------------------------------
        while(1){
            temErro=0;
            printf("Digite o nome do paciente: ");
            fgets(temp, MAX_TAM, stdin);
            if(temp[strlen(temp)-1]=='\n'){
                temp[strlen(temp)-1]='\0';
            }else{
                limpar_buffer();
            }

            //Remover \n
            if (temp[strlen(temp) - 1] == '\n' && strlen(temp) - 1 !=0) {  //Segunda condição é para evitar segmentations faults
                temp[strlen(temp) - 1] = '\0';
            }

            //Verificar se nome é muito curto
            if (strlen(temp) < 3) {
                erro=1;
                temErro = 1;
            }
            // Verificação de caracteres do nome
            int tem_letras = 0; 
            int i=0;      
            while(temp[i] != '\0'){
                //Se tiver numeros ou simbolo (qualquer caracter que n seja letra, espaço, nova linha ou \0)
                if (!isalpha(temp[i]) && temp[i] != ' ' && temp[i] != '\n' && temp[i] != '\0') {  
                    erro=2;
                    temErro =1;
                    break;
                }
                if (isalpha(temp[i]) && temp[i]!=' ') {
                    tem_letras = 1;
                }
                i++;
            }
            //Se não tiver letras dá erro
            if(tem_letras==0){
                erro=3;
                temErro=1;
            }
            //Se tiver erro recomeçar
            if(temErro==1){
                erroNome(erro);
                continue;
            }else{
                strcpy(novo->nome, temp);
                break;
            }
        }

        //Pedir data de nascimento do paciente ---------------------------------
        Data *dat;
        dat = (Data *)malloc(sizeof(Data));
        printf("Digite a data de nascimento: \n");
        verificarData(dat);                           
        novo->data_nasc.dia = dat->dia;
        novo->data_nasc.mes = dat->mes;
        novo->data_nasc.ano = dat->ano;
        

        //Pedir CC do paciente -----------------------------
        verificar_CC(14, "Digite a identificacao do seu CC (8n-n-xxn): ", novo->CC);
        //Pedir telemovel ---------------------------------
        novo->telemovel = verificarNum(999999999,900000000,"Digite o numero de telemovel: ");
        
        //Pedir email  ------------------------  
        verificarEmail(novo->email);

        
        novo->prox = NULL;
        novo->registos = (Registo_header *)malloc(sizeof(Registo_header));
        novo->registos->start = NULL;
        
        while (cur != NULL && strcmp(cur->nome, novo->nome) < 0) {
            anterior = cur;
            cur = cur->prox;
        }
        
        if (anterior == NULL) {
            novo->prox = lista_doentes->start;
            lista_doentes->start = novo;
        } else {
            anterior->prox = novo;
            novo->prox = cur;
        }
        
        escrever_ficheiro_doentes(novo);
        free(dat);
        free(temp);
    } else {
        printf("O paciente que pretende introduzir já existe\n");
    }
    
}
    //eliminar_doente
void apagarD(Doente_header *lista_doentes){
    int id_a_eliminar;
    listarD(lista_doentes);
    id_a_eliminar = verificarNum(0,0,"Insira o Id desejado: ");    //QQ coisa se não funcionar ver aqui
    if(verifica_id(lista_doentes,id_a_eliminar)){
        Doente *cur = lista_doentes->start;
        Doente *prev = NULL;
        while (cur != NULL){
            if (cur->id_doente == id_a_eliminar){
                if (prev != NULL) { 
                    prev->prox = cur->prox;
                } else {
                    lista_doentes->start = cur->prox; 
                }
                free(cur->registos);
                free(cur);
                apagar_ficheiro_doentes(id_a_eliminar);
                apagar_ficheiro_registos(id_a_eliminar);
                break;
            }
            prev = cur;
            cur = cur->prox;
        }
    }else{
        printf("O ID nã existe\n");
    }
}


//Fazer print dos paceintes em ordem alfabética
void listarD(Doente_header *lista_doentes){
    Doente *cur=lista_doentes->start;
    while(cur!=NULL){
        printf("%d - %s\n", cur->id_doente, cur->nome);
        cur=cur->prox;
    }
}

    //Inserir na lista auxiliar, para organizar por tensão
int inserir_doente_na_lista_aux(aux *lista_aux, Doente *novo) {
    aux *novo_node = (aux *)malloc(sizeof(aux));
    if (novo_node == NULL) {
        printf("Erro ao alocar memória.\n");
        return -1;
    }
    novo_node->start = novo;

    if (lista_aux->start == NULL || novo->registos->start->tensao_max > lista_aux->start->registos->start->tensao_max) {
        novo_node->prox = lista_aux->prox;
        lista_aux->prox = novo_node;
        return -1;
    }

    aux *anterior = lista_aux;
    aux *cur = lista_aux->prox;
    while (cur != NULL && cur->start->registos->start->tensao_max > novo->registos->start->tensao_max) {
        anterior = cur;
        cur = cur->prox;
    }
    novo_node->prox = cur;
    anterior->prox = novo_node;

    return -10;
}

//Função que lista doentes com a tensão superior a um valor dado pelo utilizador
void listar_doentes_tensao_sup(Doente_header *lista_doentes) {
    int tensao_a_analisar;
    tensao_a_analisar = verificarNum(0,0,"Pretende listar os pacientes com tensões máximas superiores a: ");
    aux lista_aux;
    lista_aux.start = NULL;
    lista_aux.prox = NULL;

    Doente *doente_atual = lista_doentes->start;
    while (doente_atual != NULL) {
        if(doente_atual->registos->start !=NULL){
            if (doente_atual->registos->start->tensao_max > tensao_a_analisar) {
                inserir_doente_na_lista_aux(&lista_aux, doente_atual);
            }
        }
        doente_atual = doente_atual->prox;
    }

    aux *aux_atual = lista_aux.prox;
    while (aux_atual != NULL) {
        printf("O paciente %s apresentou as seguintes tensões maiores que %d:\n", aux_atual->start->nome, tensao_a_analisar);
        Registo_info *registo_atual = aux_atual->start->registos->start;
        while (registo_atual != NULL && registo_atual->tensao_max > tensao_a_analisar) {
            printf("- Tensão: %d no dia %d/%d/%d\n", registo_atual->tensao_max, registo_atual->data_res.dia, registo_atual->data_res.mes, registo_atual->data_res.ano);
            registo_atual = registo_atual->prox;
        }
        aux_atual = aux_atual->prox;
    }
}

//Fazer print da informação do doente
void info_doente(Doente_header *lista_doentes) {
    int id;
    id = verificarNum(0,0,"Indroduza o id do paciente: ");

    if (verifica_id(lista_doentes, id)) {
        Doente *current = lista_doentes->start;

        while (current != NULL) {
            if (current->id_doente == id) {
                printf("Doente %d\n", current->id_doente);
                printf("Nome:%s\n", current->nome);
                printf("Nasceu a: %d/%d/%d\n", current->data_nasc.dia, current->data_nasc.mes, current->data_nasc.ano);
                printf("Cartão de cidadão:%s\n", current->CC);
                printf("Numero de telemovel %d\n", current->telemovel);
                printf("Email:%s\n", current->email);
                Registo_info *registro_atual = current->registos->start;
                while (registro_atual != NULL) {
                    printf("No dia %d/%d/%d ", registro_atual->data_res.dia, registro_atual->data_res.mes, registro_atual->data_res.ano);
                    printf("registou uma tensao maxima de %d e minima de %d, ", registro_atual->tensao_max, registro_atual->tensao_min);
                    printf("registou um peso de %d kg e uma altura de %d cm\n", registro_atual->peso, registro_atual->altura);
                    registro_atual = registro_atual->prox;
                }
                break;
            }
            current = current->prox;
        }
    } else {
        printf("O Doente introduzido não existe\n");
    }
}


//Pressão maxima mais alta já registada: 290 mmHg
// ||                 baixa            : 30 mmHg
//Pressão minima mais alta já registada: 250 mmHg
//                    baixa            : 16 mmHg

//Criar registo do paciente
int registar_dados(Doente_header *lista_doentes){
    Registo_info *novo=(Registo_info *)malloc(sizeof(Registo_info));
    Doente *atual_doente = lista_doentes->start;
    Data *data_registo = (Data*)malloc(sizeof(Data));
    if(data_registo==NULL){
        printf("Falha na alocação de memória");
        return -1;
    }
    int id_novo;
    printf("Introduza o novo registo do paciente\n");

    id_novo = verificarNum(0,0,"Introduza o ID:");
    if(verifica_id(lista_doentes,id_novo)){
        //verificar data
        printf("Data do registo: ");
        verificarData(data_registo);
        novo->data_res.dia = data_registo->dia;
        novo->data_res.mes = data_registo->mes;
        novo->data_res.ano = data_registo->ano;


        novo->tensao_max = verificarNum(290,30,"Tensão máxima (mmHg): ");
        novo->tensao_min = verificarNum(250,16,"Tensão mínima (mmHg): ");
        novo->peso = verificarNum(600,1,"Peso (kg):");
        novo->altura = verificarNum(250,30,"Altura (cm): ");

        novo->prox=NULL;

        while (atual_doente != NULL && atual_doente->id_doente != id_novo) {
                atual_doente = atual_doente->prox;
            }
            if (atual_doente != NULL) {
                Registo_info *anterior = NULL;
                Registo_info *atual_registo = atual_doente->registos->start;
                
                while (atual_registo != NULL && novo->tensao_max < atual_registo->tensao_max) {
                    anterior = atual_registo;
                    atual_registo = atual_registo->prox;
                }
                
                
                if (anterior == NULL) {
                    novo->prox = atual_doente->registos->start;
                    atual_doente->registos->start = novo;
                } else {
                    anterior->prox = novo;
                    novo->prox = atual_registo;
                }
            }
            escrever_ficheiro_registos(novo, id_novo);
    }else{
        printf("Erro: ID de paciente não encontrado na lista de pacientes.\n");
                free(novo);
    }
    free(data_registo);
    return -10;
}

//Limpar toda a memória
void limpar(Doente_header *lista_doentes) {
    Doente *cur = lista_doentes->start;
    Doente *prox;
    
    while (cur != NULL) {
        // Liberar todos os registros do doente
        Registo_info *cur_registo = cur->registos->start;
        Registo_info *prox_registo;
        
        while (cur_registo != NULL) {
            prox_registo = cur_registo->prox;
            free(cur_registo);
            cur_registo = prox_registo;
        }
        
        // Liberar a estrutura de cabeçalho dos registros
        free(cur->registos);

        // Avançar para o próximo doente e liberar o atual
        prox = cur->prox;
        free(cur);
        cur = prox;
    }
    
    // Limpar o início da lista de doentes
    lista_doentes->start = NULL;
}

