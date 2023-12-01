// Trabalho Pratico SO -> DEIS-ISEC 2021-2022
// Autoria:
// Tânia Guedes - a2020139445@isec.pt
// Emanuel Saraiva - a2019130219@isec.pt

#ifndef MEDICO_H
#define MEDICO_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define LEN 100

typedef struct Medico medico, *pmedico;

struct Medico{
    char nome_medico[LEN];
    char especialidade[LEN];
    int pid;
    int estado; //ativo ou não ativo
    char nomeRetorno[LEN];
    int pidRetorno;
    char nomeRetornoM[LEN];
    int pidRetornoM;
    pmedico prox; //ponteiro proxima resposta
};

#endif //MEDICO_H
