/* ======================== Documentação do Projeto ======================== */
/**
 * @file main.c
 * @brief Jogo de Memória
 * @authors Felipe Gonçalves, Guilherme Barretto e Geovanna Sargaço.
 * @date 06/2025
 * @version 0.1
 * @license GPL
 * @note Descrição: 
 * É um jogo simples de memorização, onde o jogador deve clicar 
 * nos quadrados revelados anteriormente.
 * 
 * @warning 
 * - Padrão de documentação: Doxygen.
 * - Uso de IA: Autocomplete de código, auxílio na documentação e 
 * para aprendizagem de funções da raylib.
 */

 /* ====================== Diretivas de Processamento ====================== */
/**
 * @brief Inclusão das bibliotecas necessárias para o funcionamento do jogo.
 * 
 * @note Includes:
 * - raylib.h para partes visuais do jogo.
 * - stdio.h 
 * - stdlib.h usando a função rand() para gerar números aleatórios.
 * - time.h usando a função time(NULL) para obter o tempo atual.
 */

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ========================= Protótipos de Funções ========================= */

void janela();
void registro_do_jogador();
int definir_jogador();
int gerador_de_numeros(int i);
int gabarito(int fase);
void fim_de_jogo();
int ranking();

/* =============================== Registros =============================== */
/**
 * @brief Estrutura para armazenar informações de jogadores.
 * 
 * @param nick Apelido do jogador com no máximo 10 caracteres.
 * @param pontos Pontuação acumulada pelo jogador.
 * @param tempo Tempo de jogo.
 * @param fase Fase atual do jogo.
 */

struct registro_de_jogadores
{
    char nick[10];
    int pontos;
    int tempo;
    int fase;
};

/* =========================== Variáveis Globais =========================== */

struct registro_de_jogadores jogador[4]; // Vetor de jogadores max = 4;
int contador_de_jogadores = 0;

/* =========================== Função Principal ============================ */

int main()
{
    srand(time(0));

    return 0;
}

/* ======================= Desenvolvimento de Funções ====================== */
int definir_jogador()
{
    int menor_pontos = 0;
    for(int i = 0; i < 5; i++){
        if(jogador[i].pontos == 0)
        {
            numero_do_jogador = i;
            break; // sai do loop quando encontrar um jogador sem pontos.
        }
        else if(jogador[i].pontos < menor pontos);
        {
            menor_pontos = jogador[i].pontos;
            numero_do_jogador = i;
        }
    }
    return numero_do_jogador;
}

/** 
* @todo: Fazer a parte gráfica do registro do jogador.
* @note: Descrição:
* Está função é para ser executada depois de aparecer o ranking (quando o jogador perder) e na primeira execução.
* Objetivo: Aparecer uma tela pedindo para o jogador digitar um nick. 
*
*/
int registro_do_jogador()
{
    int i = definir_jogador();
    printf("Digite seu nome: ");
    fgets(jogador[i].nick, 10, stdin);
    jogador[i].pontos = 0;
    jogador[i].tempo = 0;
    jogador[i].fase = 3; // Já começa na fase 3, para mostrar 3 quadrados.
    return numero_do_jogador;
}

int gerador_de_numeros(int i)
{
    int n;
    n = rand() % i;
    return n;
}

int gabarito(int fase)
{
    int tamanho_do_tabuleiro, i;
    tamanho_do_tabuleiro = (fase <= 7) ? 4 : (fase <= 13) ? 5 : 6;
    matriz[tamanho_do_tabuleiro][tamanho_do_tabuleiro];
    int linha, coluna, linha_aleatoria, coluna_aleatoria;

    /* Loop para preencher a matriz */
    for(linha = 0; linha<tamanho_do_tabuleiro; linha++)
    {
        for(coluna = 0; coluna<tamanho_do_tabuleiro; coluna++)
        {
            matriz[linha][coluna] = 0;
            printf("%d", matriz[linha][coluna]);
        } /* fim do loop das colunas */
        printf("\n");
    } /* fim do loop das linhas */

    // TODO: Pensar em um jeito de juntar esses dois loops.

    for(i=0; i<fase;)
    {
        linha_aleatoria = gerador_de_numeros(tamanho_do_tabuleiro);
        coluna_aleatoria = gerador_de_numeros(tamanho_do_tabuleiro);
        if (matriz[linha_aleatoria][coluna_aleatoria] == 0)
        {
            matriz[linha_aleatoria][coluna_aleatoria] = 1;
            i++;
        }
    }
} /* fim da função tabuleiro */


void janela()
{
    SetTraceLogLevel(5); // Definir o log como 5 (mostrar somente erros).
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Jogo da Memoria");
    MaximizeWindow();

    /* Definir o ícone da janela */
    Image icon = LoadImage("icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);

    SetTargetFPS(60);

    // Loop principal do jogo
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }

    CloseWindow();
}
