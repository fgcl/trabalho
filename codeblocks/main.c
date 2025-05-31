/* ======================== Documentação do Projeto ======================== */
/**
 * @file main.c
 * @brief Jogo de Memória
 * @authors Felipe Gonçalves, Guilherme Barretto e Geovanna Sargaço.
 * @date 06/2025
 * @version 0.1
 * @license GPL
 *
 * @note Descrição:
 * É um jogo simples de memorização, onde o jogador deve clicar
 * nos quadrados revelados anteriormente.
 *
 * @warning
 * Uso de IA: Autocomplete de código, auxílio na documentação e
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

void tela_de_cadastro();
void tela_de_contagem();
int registro_do_jogador();
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

typedef struct registo_de_jogadores
{
    char nick[10];
    int pontos;
    int tempo;
    int fase;
} registro;

/* =========================== Variáveis Globais =========================== */

registro jogador[4]; // Vetor de jogadores max = 4;
int contador_de_jogadores = 0;

/* =========================== Função Principal ============================ */

int main()
{
    // Inicialização da semente para geração de números aleatórios.
    srand(time(0));

    // Inicialização da janela.
    SetTraceLogLevel(5); // Definir o log como 5 (mostrar somente erros).
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(500, 500, "Jogo da Memoria");

    // Definir o ícone da janela.
    Image icon = LoadImage("icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);

    return 0;
}

/* ======================= Desenvolvimento de Funções ====================== */

/* Timeline:
- 1. Janela que pede o nick do jogador.
- 2. Mostra o gabarito do tabuleiro na tela por 5 segundos.
- 3. Pede para o jogador clicar nos quadrados revelados anteriormente.
- 4. Se o jogador acertar, o quadrado fica verde e passa para a próxima fase.
- 5. Se o jogador errar, é game over.
- 6. Uma tela de ranking deve aparecer quando o jogador perde.
*/

/**
 * @brief Janela de cadastro do nome do jogador.
 * 
 * @note:
 * Está função cria uma janela interativa para o jogador digitar o seu nome:
 * - Verificando se o nome tem mais de 3 letras e menos de 10.
 * - Exibindo as letras em tempo real.
 * - Pode-se usar o backspace para apagar as letras.
 * Ao presssionar enter, chama a função tela_de_contagem().
 *
 * @author Felipe Gonçalves
 *
 * TODO: Substituir o jogador[0] por um jogador vazio com a função definir_jogador()
 */

void tela_de_cadastro()
{
    int tecla = 0, indice_do_nome = 0;
    char nome[10] = "\0";
    int mouse_no_retangulo = 0;

    while (!WindowShouldClose())
    {
        // verifica se o mouse está dentro do retangulo.
        mouse_no_retangulo = (GetMouseX()>125 && GetMouseX()<375 && GetMouseY()>200 && GetMouseY()<250)?
        1: 0;
        if (mouse_no_retangulo)
        {
            SetMouseCursor(MOUSE_CURSOR_IBEAM); //Transforma o icone do mouse em um I.
            tecla = GetCharPressed();

            while (tecla>0)
            {
                if ((tecla >= 32) && (tecla <= 125) && indice_do_nome<10)
                {
                    nome[indice_do_nome] = tecla;
                    indice_do_nome++;
                    nome[indice_do_nome] = '\0';
                }
                tecla = GetCharPressed();
            }
            if (IsKeyPressed(KEY_ENTER) && indice_do_nome>3)
            {
                strcpy(jogador[0].nick, nome);
                tela_de_contagem();
                //printf("Nome escolhido: %s\n", nome);

            }
            else if(IsKeyPressed(KEY_BACKSPACE) && indice_do_nome>0)
            {
                indice_do_nome--;
                //printf("Apagado a letra: %c\n", nome[indice_do_nome]);
                nome[indice_do_nome] = '\0';

            }
        }
        else
        {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT); // Volta o ícone do mouse para o padrão.
        }

        // Desenha na tela.
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Digite o seu nome:", 250/2, 180, 20, BLUE); // DrawText("texto", posição_x, posição_y, tamanho_fonte, cor)
        DrawRectangle(250/2, 250 - 50, 250, 50, BLACK); // DrawRectangle(posição_x, posição_y, largura, altura, cor)

        if (mouse_no_retangulo)
        {
            DrawRectangleLines(250/2, 250 - 50, 250, 50, BLUE);
        }
        else{
            DrawRectangleLines(250/2, 250 - 50, 250, 50, GRAY);
        }

        DrawText(nome, 250/2 + 5, 200 + 8, 40, RED);
        EndDrawing();
    }
    CloseWindow();
}

/**
 * @brief Substituidor de pontos.
 * Esta função é chamada, para definir o jogador que terá o seu score substituído:
 * - Prioriza um vetor de jogador não preenchido.
 * - Substitui o jogador com menor pontuação.
 *
 * @return int Indice do vetor do registro (0-4)
 *
 * @author Felipe Gonçalves
 */
int definir_jogador()
{
    int menor_pontos = 0;
    int numero_do_jogador;
    for(int i = 0; i < 5; i++){
        if(jogador[i].pontos == 0)
        {
            numero_do_jogador = i;
            break; // sai do loop quando encontrar um jogador sem pontos.
        }
        else if(jogador[i].pontos < menor_pontos);
        {
            menor_pontos = jogador[i].pontos;
            numero_do_jogador = i;
        }
    }
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
    int matriz[tamanho_do_tabuleiro][tamanho_do_tabuleiro];
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
