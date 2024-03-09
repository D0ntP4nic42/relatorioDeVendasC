#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

//dados gerais
typedef struct{
    int id;
    char idEquipe[3];
    int idVenda;
    char cargo[8];
    char nome[100];
    float venda;
} dados;

//dados apenas de vendedores
typedef struct{
    char nome[100];
    char cargo[9];
    int id;
    int idEquipe;
    float totalVendas;
    float maiorVenda;
    float comissao;
} pessoa;

//gerenciamento de equipes
typedef struct{
    int id;
    float totalVendas;
    char gerente[100];
    float comissaoGerente;
} equipes;

int main(){
    setlocale(LC_ALL, "Portuguese_Brazil");

    dados *vendas = malloc(sizeof(dados));
    pessoa *vendedor = malloc(sizeof(pessoa));
    char linha[1024];
    int cont = 0, qtdEquipes = 0;
    float vendasEmpresa = 0, maiorVendaEmpresa = 0;


    //armazenamento de dados brutos
    while(fgets(linha, sizeof(linha), stdin) != NULL){
        vendas = realloc(vendas, sizeof(dados) * (1 + cont));

        //id
        char *token = strtok(linha, ",");
        if(token != NULL){
            vendas[cont].id = atoi(token);
        }

        //nome
        token = strtok(NULL, ",");
        if(token != NULL){
            strcpy(vendas[cont].nome, token);
        }
        
        //cargo
        token = strtok(NULL, ",");
        if(token != NULL){
            strcpy(vendas[cont].cargo, token);
        }

        //idEquipe
        token = strtok(NULL, ",");
        if(token != NULL){
            strcpy(vendas[cont].idEquipe, token);
        }

        //idEquipe de gerentes estão com \n no fim!
        if(strcmp(vendas[cont].cargo,"gerente") == 0){
            vendas[cont].idEquipe[2] = '\0';
            qtdEquipes++;
        } else {
            //idVenda
            token = strtok(NULL, ",");
            if(token != NULL){
                vendas[cont].idVenda = atoi(token);
            }

            //valorVenda
            token = strtok(NULL, ",");
            if(token != NULL){
                vendas[cont].venda = atof(token);

                //Vendas totais
                vendasEmpresa = vendas[cont].venda + vendasEmpresa;
            }
        }  
        
        cont++;
    }

    //Gerentes em primeiro dá problema aqui conserte seu vagabundo
    //vendas por equipe, equipe 0 = equipe 1...
    equipes vendasEquipes[qtdEquipes];
    //o primeiro for vai percorrer o num de equipes
    for(int numEquipe = 0; numEquipe < qtdEquipes; numEquipe++){
        //o segundo vai percorrer os vendedores
        for(int numVendedor = 0; numVendedor < cont; numVendedor++){
            //compara a equipe que está sendo analisada com a equipe de cada vendedor
            if(atoi(vendas[numVendedor].idEquipe) == numEquipe + 1 && strcmp(vendas[numVendedor].cargo, "gerente") != 0){
                vendasEquipes[numEquipe].id = numEquipe + 1;
                vendasEquipes[numEquipe].totalVendas += vendas[numVendedor].venda;            
            }
        }
    }


    //ordenar equipe e salvar indices para não perder o numero da equipe pós ordenação
    for(int i = 0; i < qtdEquipes - 1; i++) {
        int maior = i;
        for(int j = i + 1; j < qtdEquipes; j++) {
            if (vendasEquipes[j].totalVendas > vendasEquipes[maior].totalVendas){
                maior = j;
            } 
        }
        equipes aux = vendasEquipes[i];
        vendasEquipes[i] = vendasEquipes[maior];
        vendasEquipes[maior] = aux;
    }

    //colocar gerentes em suas equipes
    for(int numVendas = 0; numVendas < cont; numVendas++){
        if(strcmp(vendas[numVendas].cargo, "gerente") == 0){
            for(int numEquipes = 0; numEquipes < qtdEquipes; numEquipes++){
                if(atoi(vendas[numVendas].idEquipe) == vendasEquipes[numEquipes].id){
                    strcpy(vendasEquipes[numEquipes].gerente, vendas[numVendas].nome);
                    vendasEquipes[numEquipes].comissaoGerente = vendasEquipes[numEquipes].totalVendas * 0.05;  
                }
            }
        }
    }
    
    vendedor = malloc(sizeof(pessoa));

    //organizar dados
    int vendedoresIndice = 0; 
    for(int i = 0; i < cont; i++){
        vendedor = realloc(vendedor, sizeof(pessoa) * (vendedoresIndice + 1));

        int jaAddFlag = -1;
        for(int j = 0; j <= vendedoresIndice; j++){
            //vendedor já add
            if(vendas[i].id == vendedor[j].id || strcmp(vendas[i].cargo, "gerente") == 0){
                jaAddFlag = 1;
                
                if(vendas[i].venda > vendedor[j].maiorVenda){
                    vendedor[j].maiorVenda = vendas[i].venda;
                }
                vendedor[j].totalVendas += vendas[i].venda;

                if(strcmp(vendedor[j].cargo, "junior") == 0){
                    vendedor[j].comissao = vendedor[j].totalVendas * 0.01;
                } else if(strcmp(vendedor[j].cargo, "pleno") == 0){
                    vendedor[j].comissao = vendedor[j].totalVendas * 0.02;
                } else {
                    vendedor[j].comissao = vendedor[j].totalVendas * 0.03;
                }

                if(vendas[i].venda > maiorVendaEmpresa){
                    maiorVendaEmpresa = vendas[i].venda;
                }
            }
        }

        //Vendedor n adicionado ainda
        if(jaAddFlag == -1){
            vendedor[vendedoresIndice].id = vendas[i].id;
            strcpy(vendedor[vendedoresIndice].nome, vendas[i].nome);
            strcpy(vendedor[vendedoresIndice].cargo, vendas[i].cargo);
            vendedor[vendedoresIndice].totalVendas = vendas[i].venda;
            vendedor[vendedoresIndice].maiorVenda = vendas[i].venda;
            vendedor[vendedoresIndice].idEquipe = atoi(vendas[i].idEquipe);
            vendedor[vendedoresIndice].idEquipe = atoi(vendas[i].idEquipe);

            if(strcmp(vendedor[vendedoresIndice].cargo, "junior") == 0){
                vendedor[vendedoresIndice].comissao = vendas[i].venda * 0.01;
            } else if(strcmp(vendedor[vendedoresIndice].cargo, "pleno") == 0){
                vendedor[vendedoresIndice].comissao = vendas[i].venda * 0.02;
            } else if(strcmp(vendedor[vendedoresIndice].cargo, "senior") == 0){
                vendedor[vendedoresIndice].comissao = vendas[i].venda * 0.03;
            }

            if(vendas[i].venda > maiorVendaEmpresa){
                maiorVendaEmpresa = vendas[i].venda;
                
            }

            vendedoresIndice++;
        } 
    }

    //ordenar vendedores
    for(int i = 0; i < vendedoresIndice - 1; i++){
        int maior = i;
        for(int j = i + 1; j < vendedoresIndice; j++){
            if (vendedor[j].totalVendas > vendedor[maior].totalVendas){
                maior = j;
            } 
        }
        pessoa aux = vendedor[i];
        vendedor[i] = vendedor[maior];
        vendedor[maior] = aux;
    }

    //Maior nome de vendedor
    int tamMaiorNome = 0;
    for(int i = 0; i < vendedoresIndice; i++){
        if(strlen(vendedor[i].nome) > tamMaiorNome){
            tamMaiorNome = strlen(vendedor[i].nome);
        }
    }


    printf("Relatório de Vendas\n");
    printf("Total de Vendas da Empresa: R$%'.2f\n", vendasEmpresa);
    printf("Total de Vendas por Equipe (do maior para o menor):\n");
    for(int numEquipe = 0; numEquipe < qtdEquipes; numEquipe++){
            printf("Equipe %02d (Gerente: %s) - Vendas: R$%'.2f - ", vendasEquipes[numEquipe].id, vendasEquipes[numEquipe].gerente, vendasEquipes[numEquipe].totalVendas);
            printf("Comissão - R$%'.3f\n", vendasEquipes[numEquipe].comissaoGerente);
        }
    printf("Gerente da Equipe Vencedora: %s\n", vendasEquipes[0].gerente);
    printf("Melhor Vendedor: %s\n\n", vendedor[0].nome);

    printf("Lista de Vendedores(por vendas decrescentes): \n");
    printf("Nome");   
    for(int i = 0; i < tamMaiorNome; i++){
        printf(" ");                                   
    }                                
    printf("Cargo   Equipe  Total de Vendas        Maior Venda     Comissão\n");
    for(int i = 0; i < vendedoresIndice; i++){
        while(strlen(vendedor[i].nome) < tamMaiorNome + 4){
            strcat(vendedor[i].nome, " ");
        }
        while(strlen(vendedor[i].cargo) < 8){
            strcat(vendedor[i].cargo, " ");
        }
        //caso seja o primeiro ele n pula a linha
        if(i == 0){
            printf("%s%s%-8.02d%-23.2f%-16.2f%.2f", vendedor[i].nome, vendedor[i].cargo, vendedor[i].idEquipe, vendedor[i].totalVendas, vendedor[i].maiorVenda, vendedor[i].comissao);           
        } else {
            printf("\n%s%s%-8.02d%-23.2f%-16.2f%.2f", vendedor[i].nome, vendedor[i].cargo, vendedor[i].idEquipe, vendedor[i].totalVendas, vendedor[i].maiorVenda, vendedor[i].comissao);           
        }
    }        
}