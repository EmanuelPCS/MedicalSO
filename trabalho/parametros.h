// Trabalho Pratico SO -> DEIS-ISEC 2021-2022
// Autoria:
// Tânia Guedes - a2020139445@isec.pt
// Emanuel Saraiva - a2019130219@isec.pt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#define LEN 100
#define LENDUP 200


typedef struct Consultas consulta, *pconsulta;

struct Consultas{
    char interveniente[LEN]; //nome do cliente ou medico
    char mensagem[LENDUP]; //mensagens medico <=> utilizador
    int consulta_num; // numero de consulta associada ao balcao
    int estado; 
    pconsulta prox;  
};
