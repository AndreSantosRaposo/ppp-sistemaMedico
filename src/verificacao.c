#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "../lib/estruturas.h"
#include "../lib/operacoes.h"

//Função de display de erro
void erroNum(int err){
    switch (err){
        case 1:
            printf("Insira apenas números\n");
            break;
        case 2:
            printf("Insira 9 digitos\n");
            break;
        case 3:
            printf("Insira pelo menos um número\n");
            break;
        case 4:
            printf("Numero inválido\n");
            break;
        case 5:
            printf("Insira os caracteres no formato pretendido\n");
    }
}


void limpar_buffer(){
    printf("Limpar o buffer, presione enter:");   //É preciso clicar enter
    int c;
    while((c=getchar())!= '\n' && c!= EOF);
}

//Verificar o input numérico, e se stá dentro dos limites
int verificarNum(int limMax, int limMin, char str[]){
    int temErro;
    int erro;
    double num;
    int n;
    char* temp;
    temp = (char*)malloc(sizeof(char)*MAX_TAM);
    if(temp==NULL){
        printf("Falha na alocação de memória");
        return -1;
    }

    while(1){
        temErro=0;
        erro=0;
        printf("%s",str);
        fgets(temp,MAX_TAM,stdin);

        // Remover o caractere \n para verificar melhor a length da string
        if (temp[strlen(temp) - 1] == '\n' && strlen(temp) - 1 !=0) {  //Segunda condição é para evitar segmentations faults
            temp[strlen(temp) - 1] = '\0';
        }

        int i=0;
        while(temp[i]!='\0'){
            if(!isdigit(temp[i]) && temp[i]!='.'){
                temErro =1;
                erro=1;
                break;
            }  
            i++;  
        }

        if(strlen(temp)<1){
            temErro =1;
            erro = 3;
        }

        //Erros de input
        if(temErro){
            erroNum(erro);
            limpar_buffer();
            continue;
        }

        num = atof(temp);  //Converter para float
        
        n = round(num);

        if((limMax!=0 || limMin!=0) && (num>=limMax || num<=limMin)){
            temErro =1;
            erro =4;
        }

        if(temErro){
            erroNum(erro);
            continue;
        }else{
            break;
        }

    }
    free(temp);
    return n;
}

int verificarData(Data *dat){
    int etapa = 3; // Etapa 1 = dia, 2=mes, 3 =ano;
    int temErro = 0;
    int erro = 0; 
    int num;
    char *temp = malloc(sizeof(char)*15);
    if(temp==NULL){
        printf("Falha na locação de memória");
        return -99;
    }
    
    while (etapa >= 1) {
        erro=0;
        temErro=0;
        switch (etapa) {
            case 1:
                printf("Digite o dia: ");
                break;
            case 2:
                printf("Digite o mes: ");
                break;
            case 3:
                printf("Digite o ano: ");
                break;
        }

        fgets(temp, 15, stdin);
        
        if(temp[strlen(temp)-1]=='\n'){
            temp[strlen(temp)-1]='\0';
        }else{
            limpar_buffer();
        }


        if (strlen(temp) == 0) {
            temErro = 1;
            erro = 3;
        }

        for (int i = 0; temp[i] != '\0'; i++) {
            if (!isdigit(temp[i])) {
                temErro = 1;
                erro = 1;
                break;
            }
        }
        if(temErro){
            erroNum(erro);
            continue;
        }
        num = atoi(temp);

        switch (etapa) {
            case 1:
                if (num > 31 || num < 1) {
                    temErro = 1;
                    erro = 4;
                } else {
                    int diaMax = 31;
                    switch (dat->mes) {
                        case 4:
                        case 6:
                        case 9:
                        case 11:
                        diaMax = 30;
                        break;
                        case 2:
                        if (leapYear(dat->ano)){
                            diaMax = 29;
                        }else{
                            diaMax = 28;
                        }
                        break;
                    }

                    if (num > diaMax) {
                        temErro = 1;
                        erro = 4;
                    }
                }
                break;
            case 2:
                if (num > 12 || num < 1) {
                    temErro = 1;
                    erro = 4;
                }
                break;
            case 3:
                if (num > 2024 || num < 1900) {
                    temErro = 1;
                    erro = 4;
                }
                break;
        }
        
        if(temErro==1){
            erroNum(erro);
            continue;
        }else{
            switch (etapa) {
                case 1:
                    dat->dia = num;
                    break;
                case 2:
                    dat->mes = num;
                    break;
                case 3:
                    dat->ano = num;              
                    break;
            } 
            etapa--;
        }     
    }
    free(temp);
    return -99;
}

int leapYear(int ano) {
    if(ano % 4 == 0){
        if(ano % 100 == 0) {
            return ano % 400 == 0;
        }else{
            return 1;
        }
    }else{
        return 0;
    }
}

void erroNome(int err){
    switch(err){
        case 1:
            printf("Nome muito curto\n");
            break;
        case 2:
            printf("Nome deve conter apenas letras e espaços\n");
            break;
        case 3:
            printf("Nome deve conter letras\n");
            break;
    }
}

void erroEmail(int err){
    switch (err) {
        case 1:
            printf("Digite apenas um numero\n");
            break;
        case 2:
            printf("Numero inválido\n");
            break;
        case 3:
            printf("Email inválido\n");
            break;
        case 4:
            printf("Digite um email válido\n");
            break;
        case 5:
            printf("Email muito curto\n");
        }
}

int verificarEmail(char *email){
    //Escolher o dominio
    char *extensaoC;
    extensaoC = (char*)malloc(sizeof(char)*50);
    if(extensaoC == NULL){
        printf("Falha na alocação de memória \n");
        return -1;
    }

    char *temp;
    temp = (char*)malloc(sizeof(char)*50);
    if(temp == NULL){
        printf("Falha na alocação de memória \n");
        return -1;
    }

    int extensaoN;

    //Pedir o dominio
    while(1){
        //Parte de pedir o dominio
        printf("Escolha o dominio do email digitanto o numero pretendido:\n 1: @gmail.com \n 2: @hotmail.com \n 3: outlook.com\n");
        fgets(extensaoC,50,stdin);   //Permitir mais caracteres para que consiga acionar o erro de mais que um numero posto;

        // Remover o caractere \n para verificar melhor a length da string
        if (extensaoC[strlen(extensaoC) - 1] == '\n' && strlen(extensaoC) - 1 !=0) {  //Segunda condição é para evitar segmentations faults
            extensaoC[strlen(extensaoC) - 1] = '\0';
        }

        int temErro=0;
        int erro;
        int i=0;
        //Verificar se é um numero
        while(!extensaoC[i]!='\0'){
            if(!isdigit(extensaoC)){
                temErro=1;
                erro=1;
            }
        }

        if(strlen(extensaoC)>1){
            erro=1;
            temErro=1;
        }

        if(temErro){        //Tenho que ter este para evitar problemas com a conversão de char para int
            erroEmail(erro);
            continue;
        }

        extensaoN = extensaoC[0] - '0'; // Converter char para int
        //Verificar se o numero está entre 1 e 3
        if(extensaoN>3 || extensaoN<1){
            temErro=1;
            erro=2;
        }
        if(temErro==1){
            erroEmail(erro);
            continue;
        }else{
            break;
        }
    }

    //Pedir a parte do user do email
    while(1){
        printf("Digite o seu email, exeto o dominio (@...): ");
        fgets(temp,50,stdin);
        
        // Remover o caractere \n para verificar melhor a length da string
        if (temp[strlen(temp) - 1] == '\n' && strlen(temp) - 1 !=0) {  //Segunda condição é para evitar segmentations faults
            temp[strlen(temp) - 1] = '\0';
        }

        int temErro=0;
        int erro;
        int i=0;

        while(temp[i]!='\0'){
            if(temp[i]=='@'){
                temErro =1;
                erro=3;
            }
            i++;
        }

        if(strlen(temp)<3){
            temErro=1;
            erro=4;
        }

        if(temErro==1){
            erroEmail(erro);
            continue;
        }else{
            break;
        }
    }
    
    
    switch (extensaoN) {
        case 1:
            strcat(temp, "@gmail.com");
            break;

        case 2:
            strcat(temp, "@hotmail.com");
            break;

        case 3:
            strcat(temp, "@outlook.com");
            break;
    }

    strcpy(email,temp);    //Copiar temp para email


    free(extensaoC);
    free(temp);
    return -10;
}

int verificar_CC(unsigned int lim ,char str2[], char* str){

    int temErro;
    int erro;


    char* temp;
    temp = (char*)malloc(sizeof(char)*MAX_TAM);
    if(temp!=NULL){
        while(1){
            temErro=0;
            erro=0;
            printf("%s",str2);
            fgets(temp,MAX_TAM,stdin);

            // Remover o caractere \n para verificar melhor a length da string
            if (temp[strlen(temp) - 1] == '\n' && strlen(temp) - 1 !=0) {  //Segunda condição é para evitar segmentations faults
                temp[strlen(temp) - 1] = '\0';
            }else{
                limpar_buffer();
            }

            // Verificar que os primeiros 8 caracteres sao numeros
            for(int i=0;i<8;i++){
                if(!isdigit(temp[i])){
                    temErro =1;
                    erro=5;
                    break;
                }  
                
            }
            // Verificar que o utilizador colocou os separadores e os outros cacteres na ordem correta
            if(temp[8]!='-'||temp[10]!='-'||!isdigit(temp[9])||!isalpha(temp[11])||!isalpha(temp[12])||!isdigit(temp[13])){
                temErro =1;
                erro=5;
            }

            if(strlen(temp)<1){
                temErro =1;
                erro = 3;
            }

            //Erros de input
            if(temErro){
                erroNum(erro);
                continue;
            }

            //verificar o tamanho correto da string
            if(strlen(temp)!=lim){
                temErro =1;
                erro =4;
            }

            if(temErro){
                erroNum(erro);
                continue;
            }else{
                break;
            }

        }
        strcpy(str, temp);
        free(temp);
        return -10;
    }else{
        printf("Falha na alocação de memória");
        return -1;
    }

}