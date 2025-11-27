#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// ============================================================================
// --- Constantes Globais ---
// ============================================================================
#define NUM_TERRITORIOS 5
#define TAM_NOME 50
#define TAM_COR 20
#define COR_JOGADOR "Azul"   // Definimos que o usuário joga com o Azul
#define COR_INIMIGO "Vermelho"

// ============================================================================
// --- Estrutura de Dados ---
// ============================================================================
typedef struct {
    char nome[TAM_NOME];
    char corExercito[TAM_COR];
    int numTropas;
} Territorio;

// ============================================================================
// --- Protótipos das Funções ---
// ============================================================================

// Setup e Memória
Territorio* alocarMapa();
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio* mapa);

// Interface
void exibirMenuPrincipal();
// Uso de 'const' aqui garante que exibir o mapa não altera dados acidentalmente
void exibirMapa(const Territorio* mapa); 
void exibirMissao(int idMissao);
void limparBufferEntrada();

// Lógica do Jogo
void faseDeAtaque(Territorio* mapa, char* corJogador);
void simularAtaque(Territorio* atacante, Territorio* defensor);
int sortearMissao();
// Uso de 'const' pois verificar a vitória apenas lê o estado do mapa
int verificarVitoria(const Territorio* mapa, char* corJogador, int idMissao);

// ============================================================================
// --- Função Principal (main) ---
// ============================================================================
int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL)); // Semente aleatória

    // 1. Configuração Inicial
    Territorio* mapa = alocarMapa();
    if (mapa == NULL) {
        printf("Erro fatal: Falha na alocacao de memoria.\n");
        return 1;
    }

    inicializarTerritorios(mapa);
    
    char corJogador[TAM_COR] = COR_JOGADOR;
    int idMissao = sortearMissao();
    int opcao;
    int venceu = 0;

    // 2. Laço Principal do Jogo
    do {
        // Limpa tela (funciona em Windows/Linux de forma simples)
        system("cls || clear"); 

        printf("=== 🌍 WAR: ESTRATEGIA EM C 🌍 ===\n");
        printf("Comandante do Exercito: %s\n", corJogador);
        exibirMissao(idMissao);
        printf("\n--- Estado do Mundo ---\n");
        exibirMapa(mapa);
        
        exibirMenuPrincipal();
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, corJogador);
                break;
            case 2:
                venceu = verificarVitoria(mapa, corJogador, idMissao);
                if (venceu) {
                    printf("\n🏆 PARABENS! Voce cumpriu sua missao e VENCEU o jogo!\n");
                    opcao = 0; // Força saída do loop
                } else {
                    printf("\n⚠️ Missao ainda incompleta. Continue lutando!\n");
                }
                break;
            case 0:
                printf("Encerrando operacoes militares...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }

        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }

    } while (opcao != 0);

    // 3. Limpeza
    liberarMemoria(mapa);
    return 0;
}

// ============================================================================
// --- Implementação das Funções ---
// ============================================================================

// Aloca o vetor de structs dinamicamente
Territorio* alocarMapa() {
    // calloc inicializa tudo com zero, útil para evitar lixo de memória
    return (Territorio*) calloc(NUM_TERRITORIOS, sizeof(Territorio));
}

// Configura o cenário inicial (Passagem por Referência via ponteiro)
void inicializarTerritorios(Territorio* mapa) {
    // Configuração manual para fins de teste
    strcpy(mapa[0].nome, "Brasil");
    strcpy(mapa[0].corExercito, COR_JOGADOR);
    mapa[0].numTropas = 5;

    strcpy(mapa[1].nome, "Argentina");
    strcpy(mapa[1].corExercito, COR_INIMIGO); // Inimigo
    mapa[1].numTropas = 2;

    strcpy(mapa[2].nome, "Argelia");
    strcpy(mapa[2].corExercito, COR_INIMIGO);
    mapa[2].numTropas = 2;

    strcpy(mapa[3].nome, "Egito");
    strcpy(mapa[3].corExercito, "Neutro");
    mapa[3].numTropas = 1;

    strcpy(mapa[4].nome, "Reino Unido");
    strcpy(mapa[4].corExercito, COR_INIMIGO);
    mapa[4].numTropas = 4;
}

void liberarMemoria(Territorio* mapa) {
    free(mapa);
    printf("Memoria liberada com sucesso.\n");
}

void exibirMenuPrincipal() {
    printf("\n--- QG de Comando ---\n");
    printf("1. Atacar Territorio\n");
    printf("2. Verificar Status da Missao\n");
    printf("0. Sair do Jogo\n");
    printf("Sua ordem: ");
}

// Usa 'const' para garantir integridade dos dados (apenas leitura)
void exibirMapa(const Territorio* mapa) {
    printf("%-5s | %-15s | %-10s | %s\n", "ID", "Territorio", "Dono", "Tropas");
    printf("----------------------------------------------------\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-5d | %-15s | %-10s | %d\n", 
            i, mapa[i].nome, mapa[i].corExercito, mapa[i].numTropas);
    }
}

int sortearMissao() {
    // Retorna 1 ou 2
    return (rand() % 2) + 1;
}

void exibirMissao(int idMissao) {
    printf("\n📜 SUA MISSAO SECRETA: ");
    if (idMissao == 1) {
        printf("Dominar TOTALMENTE o exercito %s.\n", COR_INIMIGO);
    } else {
        printf("Conquistar pelo menos 4 territorios quaisquer.\n");
    }
}

// Gerencia a entrada do usuário para o ataque
void faseDeAtaque(Territorio* mapa, char* corJogador) {
    int origem, destino;

    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Informe o ID do territorio atacante (Seu): ");
    scanf("%d", &origem);
    
    printf("Informe o ID do territorio alvo (Inimigo): ");
    scanf("%d", &destino);

    // Validações básicas de índice
    if (origem < 0 || origem >= NUM_TERRITORIOS || destino < 0 || destino >= NUM_TERRITORIOS) {
        printf("🚫 IDs invalidos!\n");
        return;
    }

    // Validações de regra de negócio
    if (strcmp(mapa[origem].corExercito, corJogador) != 0) {
        printf("🚫 Voce so pode atacar com seus proprios territorios!\n");
        return;
    }
    if (strcmp(mapa[destino].corExercito, corJogador) == 0) {
        printf("🚫 Fogo amigo! Voce nao pode atacar a si mesmo.\n");
        return;
    }
    if (mapa[origem].numTropas <= 1) {
        printf("🚫 Tropas insuficientes para atacar (Minimo: 2).\n");
        return;
    }

    // Passa os endereços de memória dos territórios específicos
    simularAtaque(&mapa[origem], &mapa[destino]);
}

// Lógica matemática da batalha (Passagem por Referência para modificar tropas)
void simularAtaque(Territorio* atacante, Territorio* defensor) {
    printf("\n⚔️ Batalha em curso: %s vs %s...\n", atacante->nome, defensor->nome);
    
    // Simulação simplificada de dados (1 dado de ataque vs 1 dado de defesa)
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    printf("🎲 Ataque rolou: %d  |  🛡️ Defesa rolou: %d\n", dadoAtaque, dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        printf("💥 Vitoria do Ataque! Defensor perde 1 tropa.\n");
        defensor->numTropas--;

        // Verifica conquista
        if (defensor->numTropas == 0) {
            printf("🏳️ TERRITORIO CONQUISTADO! %s agora pertence ao exercito %s!\n", 
                   defensor->nome, atacante->corExercito);
            
            // Troca o dono
            strcpy(defensor->corExercito, atacante->corExercito);
            
            // Move 1 tropa para ocupar
            defensor->numTropas = 1;
            atacante->numTropas--;
        }
    } else {
        printf("🛡️ Defesa segurou! Atacante perde 1 tropa.\n");
        atacante->numTropas--;
    }
}

// Verifica se a missão foi cumprida (apenas leitura do mapa)
int verificarVitoria(const Territorio* mapa, char* corJogador, int idMissao) {
    int territoriosDominados = 0;
    int inimigosVivos = 0;

    // Varredura única do mapa para coletar estatísticas
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        if (strcmp(mapa[i].corExercito, corJogador) == 0) {
            territoriosDominados++;
        }
        if (strcmp(mapa[i].corExercito, COR_INIMIGO) == 0) {
            inimigosVivos++;
        }
    }

    if (idMissao == 1) {
        // Missão 1: Destruir exército vermelho
        if (inimigosVivos == 0) return 1;
    } 
    else if (idMissao == 2) {
        // Missão 2: Conquistar 4 territórios
        if (territoriosDominados >= 4) return 1;
    }

    return 0; // Ainda não venceu
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
