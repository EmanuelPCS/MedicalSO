// Trabalho Pratico SO -> DEIS-ISEC 2021-2022
// Autoria:
// Tânia Guedes - a2020139445@isec.pt
// Emanuel Saraiva - a2019130219@isec.pt

#ifndef BALCAO_H
#define BALCAO_H


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <ctype.h>
#include "medico.h"
#include "cliente.h"

#define LEN 100
#define LENDUP 200
#define HALFLEN 50
#define FIFO_SRV "canal"
#define FIFO_SRVMED "canalMedico"
#define FIFO_CLI "cli%d"
#define FIFO_MED "med%d"
#define FIFO_CLICMED "clicmed%d"
#define FIFO_MEDCCLI "medccli%d"

int varAmbienteMAXCLIENTES();
int varAmbienteMAXMEDICOS();

typedef struct Balcao balcao, *pbalcao;

struct Balcao{ //main que faz os forks
    int cliente_num;
    int medico_num;
    int consulta_num; 
    int consulta_status;// "em espera" ou "em consulta" ou "em consulta terminada"
    int prioridade;
    //char nome_cliente[LEN]; //nome do cliente
    char nome_medico[LEN]; //nome do medico
    //char sintoma[LENDUP]; //sintoma dado pelo cliente
    char especialidade[HALFLEN]; // especialidade (atribuida pelo classificador)
    int MAXMEDICOS;
    int MAXCLIENTES;
    pcliente client;// ponteiro para um cliente
    pmedico medic;// ponteiro para um jogo

    pbalcao prox;
};

typedef struct PacoteCliente pacoteCliente, *ppacoteCliente;

struct PacoteCliente{ 
    char nome[LEN];
    char sintoma[LEN];//texto
    int estado; //ativo ou não ativo
    int pid;
    char especialidade[LEN];
    int prioridade;
    int pidRetorno;
    char nomeRetorno[LEN];
    char msg[LEN];
    int numClientesFrente;
    char msgAdicionalNumCliFrente[LEN];
    int numMedicosEspecialidadeSistema;
    
    ppacoteCliente prox;
};


typedef struct PacoteMedico pacoteMedico, *ppacoteMedico;

struct PacoteMedico{ 
    char nome[LEN];
    char especialidade[LEN];
    int pid;
    int estado; //ativo ou não ativo
    char nomeRetorno[LEN];
    int pidRetorno;
    char msg[LEN];
    char msgAlarme[LEN];
    char msgControlC[LEN];
    int flagAlarme;
    
    ppacoteMedico prox;
};



typedef struct{
	int continua;
	int *filaOrt;
	int *filaOft;
	int *filaEst;
	int *filaNeu;
	int *filaGer;
	struct Cliente *cliTDATA;
	int tempo;
	//pthread_mutex_t *trinco;
}TDATA;


#endif //BALCAO_H

