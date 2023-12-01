// Trabalho Pratico SO -> DEIS-ISEC 2021-2022
// Autoria:
// Tânia Guedes - a2020139445@isec.pt
// Emanuel Saraiva - a2019130219@isec.pt

#ifndef CLIENTE_H
#define CLIENTE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define LEN 100

typedef struct Cliente cliente, *pcliente;

struct Cliente{
    char nome_cliente[LEN];
    char sintoma[LEN];
    //int num_cliente;
    int estado; //ativo ou não ativo
    int pid;
    char especialidade[LEN];
    int prioridade;
    char nomeRetornoC[LEN];
    int pidRetornoC;
    pcliente prox; 
};

#endif //CLIENTE_H
