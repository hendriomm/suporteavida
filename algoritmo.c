#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#define MINUTOS 60

#define PRESSAO_ALTA 120 + 120*0.15
#define PRESSAO_BAIXA 120 - 120*0.15
#define MAX_EVENTOS 100
#define MAX_STR_NOME 20

int TempoGlobal = 0;
int PulsoPaciente = 75;     
int PressaoPaciente = 120;  


struct Evento {
    char nomePaciente[MAX_STR_NOME];
    char nomeEvento[30];
    int momentoEvento;
};

struct Evento TabelaPacienteX[MAX_EVENTOS];
int indiceTabela = 0;

int AplicaNoradrenalina = 0;
int AplicaAdenosina = 0;
int AplicaDopamina = 0;
int AplicaFurosemida = 0;
int ControlePressaoPaciente= 0;
int ControlePulsoContador = 0;
int Pulso = 0;

void registrarEvento(const char *nomePaciente, const char *nomeEvento);
void aplicarNoradrenalina(const char *nomePaciente);
void aplicarAdenosina(const char *nomePaciente);
void controlePulso(const char *nomePaciente);
void pulsoPaciente(const char *nomePaciente);
void aplicarDopamina(const char *nomePaciente);
void aplicarFurosemida(const char *nomePaciente);
void controlePressao(const char *nomePaciente);
void pressaoPaciente(const char *nomePaciente);
void EscalonaEstPulso(const char *nomePaciente);
void EscalonaDinPressao(const char *nomePaciente);

void registrarEvento(const char nomePaciente[20], const char nomeEvento[30]) {
    if (indiceTabela < MAX_EVENTOS) {
        strcpy(TabelaPacienteX[indiceTabela].nomePaciente, nomePaciente);
        strcpy(TabelaPacienteX[indiceTabela].nomeEvento, nomeEvento);
        TabelaPacienteX[indiceTabela].momentoEvento = TempoGlobal;
        indiceTabela++;
    } else {
        printf("Tabela de eventos cheia!\n");
    }
}

void pulsoPaciente(const char *nomePaciente) {
    printf("Executando PulsoPaciente\n");
    registrarEvento(nomePaciente, "PulsoPaciente");
    sleep(2 * MINUTOS);
}

void aplicarNoradrenalina(const char *nomePaciente) {
    Pulso -= Pulso * 0.12;
    registrarEvento(nomePaciente, "AplicaNoradrenalina");
    sleep(1 * MINUTOS);
    printf("Aplicando 22 ml de Noradrenalina. Pulso atual: %d\n", PulsoPaciente);
}

void aplicarAdenosina(const char *nomePaciente) {
    Pulso += Pulso * 0.13;
    registrarEvento(nomePaciente, "AplicaAdenosina");
    sleep(1 * MINUTOS);
    printf("Aplicando 23 ml de Adenosina. Pulso atual: %d\n", PulsoPaciente);
}

// Função para gerar valor de pulso verossímil
int obterPulso() {
    // Valores típicos de pulso para um adulto: 50-110 bpm
    return (rand() % 51) + 50; // Retorna um valor entre 50 e 110
}

void controlePulso(const char *nomePaciente) {
    printf("Executando ControlePulso\n");
    registrarEvento(nomePaciente, "ControlePulso");
    Pulso = obterPulso();
    if (ControlePulsoContador > 0) {
        ControlePulsoContador++;
        if (ControlePulsoContador >= 5) {
            ControlePulsoContador = 0;
        }
    }
    sleep(6 * MINUTOS);
}

void aplicarDopamina(const char *nomePaciente) {
    PressaoPaciente-= PressaoPaciente* 0.14;
    ControlePressaoPaciente = TempoGlobal;
    registrarEvento(nomePaciente, "AplicaDopamina");
    sleep(3 * MINUTOS);
    printf("Aplicando 24 ml de Dopamina. Pressão atual: %d\n", PressaoPaciente);
}

void aplicarFurosemida(const char *nomePaciente) {
    PressaoPaciente+= PressaoPaciente* 0.15;
    ControlePressaoPaciente = TempoGlobal;
    registrarEvento(nomePaciente, "AplicaFurosemida");
    sleep(4 * MINUTOS);
    printf("Aplicando 25 ml de Furosemida. Pressão atual: %d\n", PressaoPaciente);
}

// Função para gerar valor de pressão arterial sistólica verossímil, incluindo níveis não saudáveis
int obterPressao() {
    // Valores típicos e não saudáveis de pressão arterial sistólica: 70-180 mmHg
    return (rand() % 111) + 70; // Retorna um valor entre 70 e 180
}

void pressaoPaciente(const char* nomePaciente) {
    sleep(2 * MINUTOS);
    PressaoPaciente = obterPressao();
    registrarEvento(nomePaciente, "PressaoPaciente");
    printf("Pressao aferida: %d.\n", PressaoPaciente);
}

void controlePressao(const char *nomePaciente) {
    sleep(1 * MINUTOS);
    if (ControlePressaoPaciente < TempoGlobal + 40 * MINUTOS);
        return;
    if (PressaoPaciente < PRESSAO_BAIXA)
        aplicarDopamina(nomePaciente);
    else if (PressaoPaciente > PRESSAO_ALTA)
        aplicarFurosemida(nomePaciente);
        
    registrarEvento(nomePaciente, "ControlePressao");
    printf("Controle de pressão realizado.\n");
}


// Escalonador Executivo Cíclico: EscalonaEstPulso
void EscalonaEstPulso(const char *nomePaciente) {
    int ciclo_maior = 10;
    while (1) {
        printf("Iniciando ciclo maior\n");

        // Intervalo de tempo 0-1: PulsoPaciente
        pulsoPaciente(nomePaciente);

        // Intervalo de tempo 2: AplicaNoradrenalina
        if (ControlePulsoContador == 0) {
            aplicarNoradrenalina(nomePaciente);
            ControlePulsoContador = 1;
        }

        // Intervalo de tempo 3: AplicaAdenosina
        if (ControlePulsoContador == 0) {
            aplicarAdenosina(nomePaciente);
            ControlePulsoContador = 1;
        }

        // Intervalo de tempo 4-9: ControlePulso
        controlePulso(nomePaciente);

        printf("Finalizando ciclo maior\n");
        TempoGlobal += ciclo_maior;
    }
}

// Escalonador Dinâmico: EscalonaDinPressao
void EscalonaDinPressao(const char *nomePaciente) {
    while (1) {
        // Alta prioridade: ControlePressao
        controlePressao(nomePaciente);

        // Prioridade média: PressaoPaciente
        pressaoPaciente(nomePaciente);

    }
}

void imprimirTabelaEventos() {
    printf("\n\nTabela de Eventos do PacienteX:\n");
    printf("-------------------------------------------------\n");
    printf("| %-15s | %-25s |\n", "Evento", "Momento");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < indiceTabela; i++) {
        printf("| %-15s | %-25d |\n", TabelaPacienteX[i].nomeEvento, TabelaPacienteX[i].momentoEvento);
    }
    printf("-------------------------------------------------\n");
}

int main() {
    const char *nomePaciente = "PacienteX";

    // Iniciar o escalonador executivo cíclico para monitoramento de pulso
    // Isso rodará indefinidamente
    if (fork() == 0) {
        EscalonaEstPulso(nomePaciente);      
    }

    // Iniciar o escalonador dinâmico para monitoramento de pressão
    // Isso rodará indefinidamente
    if (fork() == 0) {
        EscalonaDinPressao(nomePaciente);
    }

    // Aguardar os processos filhos (escalonadores) completarem
    // Na prática, isso rodará indefinidamente
    wait(NULL);
    wait(NULL);

    imprimirTabelaEventos();
    

    return 0;
}

