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
 * Problemas encontrados: passar uma matriz para outra função. VLA - Variable Length Array
 *
 * @warning Uso de IA: 
 *  Autocomplete de código, auxílio na documentação e cálculos
 * para centralização de elementos graficos.
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
int tela_do_tabuleiro(int fase);
void fim_de_jogo();
int ranking();
void aleatorizar_tabuleiro(int fase, int tamanho, int matriz[][tamanho]);
void limpar_matriz(int tamanho, int matriz[][tamanho]);
int definir_tamanho(int fase);
void desenhar_gabarito(int celulas, int matriz[][celulas], int cordenada);
void desenhar_interacao(int celulas, int prova[][celulas], int cordenada);
void calcular_pontos(int fase, int celulas, int gabarito[][celulas], int prova[][celulas]);
void desenhar_resultado(int celulas, int matriz[][celulas], int prova[][celulas], int cordenada);
void desenhar_contador(int tempo_atual, int tempo_inicio);


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
int indice_do_jogador = 0;
const int largura_da_tela = 500;
const int altura_da_tela = 500;
int cliques = 0;

/* =========================== Função Principal ============================ */

/* Timeline:
- 1. Janela que pede o nick do jogador. (OK)
- 2. Contador de tempo. 
- 2. Mostra o gabarito do tabuleiro na tela por 5 segundos. (OK)
- 3. Pede para o jogador clicar nos quadrados revelados anteriormente. (OK)
- 4. Se o jogador acertar, o quadrado fica verde (OK) e passa para a próxima fase.
- 5. Se o jogador errar, é game over.
- 6. Uma tela de ranking deve aparecer quando o jogador perde.
*/
int main()
{
    // Inicialização da semente para geração de números aleatórios.
    srand(time(0));

    // Inicialização da janela.
    SetTraceLogLevel(5); // Definir o log como 5 (mostrar somente erros).
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(largura_da_tela, altura_da_tela, "Jogo da Memoria");

    // Definir o ícone da janela.
    Image icon = LoadImage("icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);

    return 0;
}

/* ======================= Desenvolvimento de Funções ====================== */

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
    
    /* Area de carregamento do rshape */
    //Texture2D fundo = LoadTexture("fundoInicio.png");  // Carregamento da imagem de fundo da tela de cadastro.

    Image image = LoadImage("fundoInicio.png"); // Carregar imagem na RAM.
    Texture2D fundo = LoadTextureFromImage(image); // Converter para textura, para usar a GPU memory (VRAM)
    UnloadImage(image); // Remover imagem da RAM.

    // gostei de aprender isso de definir antes e depois carregar a textura.
    Rectangle retangulo = {250/2, 250 - 50, 250, 50}; // Define o retangulo para deixar redondo.

    while (!WindowShouldClose())
    {
        // Verifica se o mouse está dentro do retangulo.
        //mouse_no_retangulo = (GetMouseX()>125 && GetMouseX()<375 && GetMouseY()>200 && GetMouseY()<250)?
        (CheckCollisionPointRec(GetMousePosition(), retangulo))?
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
                indice_do_jogador = definir_jogador();
                strcpy(jogador[indice_do_jogador].nick, nome);
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
        DrawTexture(fundo, 0, 0, WHITE); // desenha o fundo.

        DrawText("Digite o seu nome:", 250/2, 180, 20, BLUE); // DrawText("texto", posição_x, posição_y, tamanho_fonte, cor)
        DrawRectangleRounded(retangulo, 0.1, 0, BLACK);

        if (mouse_no_retangulo)
        {
            DrawRectangleRoundedLines(retangulo, 0.1, 0, BLUE);
        }
        else{
            DrawRectangleRoundedLines(retangulo, 0.1, 0, GRAY);
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
 * @author Felipe
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

/**
 * @brief Renderiza e gerencia a tela do tabuleiro de jogo.
 * 
 * Esta função cria um tabuleiro de jogo com tamanho dinâmico baseado na fase atual,
 * gera posições aleatórias para quadrados azuis, e permite que o jogador interaja
 * clicando nos quadrados dentro de um tempo limite.
 * 
 * @param fase Número da fase atual, que determina o tamanho do tabuleiro e número de quadrados
 * @return int Valor de retorno não utilizado (pode ser modificado para retornar resultado do jogo)
 * 
 * @note A função gerencia a lógica de pontuação, renderização e interação do jogo de memória
 * @author Felipe
 * 
 *  TODO: 
 * 1. Definir o indice do jogador que está jogando.
 * 2. Mostrar o tempo
 * 3. Mostrar o nome
 * 4. Interromper a função depois de um tempo.
 * 5. Chamar a função novamente com nova fase.
 */
int tela_do_tabuleiro(int fase)
{
    // fase = qtd_de_quadrados

    // Variáveis da matriz
    int celulas = definir_tamanho(fase);
    int matriz[celulas][celulas];
    int matriz_do_jogador[celulas][celulas];

    limpar_matriz(celulas, matriz);
    limpar_matriz(celulas, matriz_do_jogador);
    aleatorizar_tabuleiro(fase, celulas, matriz);

    // Variáveis gráficas
    char txt_pontos[30];
    const int quadrado_tamanho = 50;
    const int espaco = 5;

    int tempo_inicio = time(NULL);
    const int tempo_limite = 5;
    const int tempo_exibicao_mensagem = 1; // Tempo para exibir "Tempo esgotado!"

    int cordenada = (largura_da_tela - (celulas * quadrado_tamanho) - espaco) / 2;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        sprintf(txt_pontos, "Pontos: %d", jogador[0].pontos);
        DrawText(txt_pontos, 0, 0, 20, BLUE);

        int tempo_atual = time(NULL);
        if (tempo_atual - tempo_inicio < tempo_limite)
        {
            // Fase de exibição do gabarito
            desenhar_contador(tempo_atual, tempo_inicio);
            desenhar_gabarito(celulas, matriz, cordenada);
        }
        else if (tempo_atual - tempo_inicio < tempo_limite_gabarito + tempo_exibicao_mensagem)
        {
            // Fase de "Tempo esgotado!"
            DrawText("Tempo esgotado!", largura_da_tela / 2 - MeasureText("Tempo esgotado!", 30) / 2, GetScreenHeight() / 2 - 15, 30, WHITE);
        }
        else
        {
            // Fase de interação do jogador e exibição do resultado
            if (cliques < fase)
            {
                desenhar_interacao(celulas, matriz_do_jogador, cordenada);
            }
            if (cliques == fase)
            {
                calcular_pontos(fase, celulas, matriz, matriz_do_jogador);
                // O cliques é incrementado aqui para não re-calcular
            }

            // Após a fase de cliques e cálculo de pontos, exibe o resultado final
            if (cliques > fase)
            {
                desenhar_resultado(celulas, matriz, matriz_do_jogador, cordenada);
            }
        }

        EndDrawing();
    }
    CloseWindow();
    return 0; // Adicionado um retorno, pois a função é int
}

void desenhar_gabarito(int celulas, int gabarito[][celulas], int cordenada)
{
    int linha, coluna, x, y;
    for (linha = 0; linha < celulas; linha++)
    {
        for (coluna = 0; coluna < celulas; coluna++)
        {
            Color cor = (gabarito[linha][coluna] == 1) ? BLUE : GRAY;
            // 51 = tamanho do quadrado (50) + espaco (1)
            x = cordenada + coluna * 51;
            y = cordenada + linha * 51;
            DrawRectangle(x, y, 50, 50, cor);
        }
    }
}

// Desenhar o clique do jogador.
void desenhar_interacao(int celulas, int prova[][celulas], int cordenada)
{
    int linha, coluna, x, y;
    int quadrado_tamanho = 50;

    for (int linha = 0; linha < celulas; linha++)
    {
        for (int coluna = 0; coluna < celulas; coluna++)
        {
            Color cor = GRAY;
            if (prova[linha][coluna] == 1)
            {
                cor = GREEN; // Quadrados clicados pelo jogador
            }

            x = cordenada + coluna * 51;
            y = cordenada + linha * 51;

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (GetMouseX() >= x && GetMouseX() <= x + quadrado_tamanho &&
                        GetMouseY() >= y && GetMouseY() <= y + quadrado_tamanho)
                {
                    // Garante que só conta clique se o quadrado não estiver marcado
                    if (prova[linha][coluna] == 0)
                    {
                        cliques++;
                        prova[linha][coluna] = 1;
                    }
                }
            }
            else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
            {
                if (GetMouseX() >= x && GetMouseX() <= x + quadrado_tamanho &&
                        GetMouseY() >= y && GetMouseY() <= y + quadrado_tamanho)
                {
                    if (prova[linha][coluna] == 1)
                    {
                        cliques--;
                        prova[linha][coluna] = 0;
                    }
                }
            }

            DrawRectangle(x, y, quadrado_tamanho, quadrado_tamanho, cor);
        }
    }
}

// Função para calcular os pontos após o término da fase de cliques
void calcular_pontos(int fase, int celulas, int gabarito[][celulas], int prova[][celulas])
{
    // Esta parte do código só deve ser executada uma vez após os cliques terminarem
    for (int linha = 0; linha < celulas; linha++)
    {
        for (int coluna = 0; coluna < celulas; coluna++)
        {
            if (gabarito[linha][coluna] == 1 && prova[linha][coluna] == 1)
            {
                jogador[0].pontos += 100;
            }
        }
    }
    cliques++; // Incrementa para que esta lógica não seja executada novamente
}

// Função para desenhar o resultado final
void desenhar_resultado(int celulas, int gabarito[][celulas], int prova[][celulas], int cordenada)
{
    int linha, coluna, x, y;
    for (linha = 0; linha < celulas; linha++)
    {
        for (coluna = 0; coluna < celulas; coluna++)
        {
            Color cor = GRAY;
            if (gabarito[linha][coluna] == 1 && prova[linha][coluna] == 0)
            {
                cor = BLUE; // Era para ser clicado, mas não foi (erro de omissão)
            }
            else if (gabarito[linha][coluna] == 0 && prova[linha][coluna] == 1)
            {
                cor = RED; // Não era para ser clicado, mas foi (erro de inclusão)
            }
            else if (gabarito[linha][coluna] == 1 && prova[linha][coluna] == 1)
            {
                cor = GREEN; // Acertou
            }
            else if (gabarito[linha][coluna] == 0 && prova[linha][coluna] == 0)
            {
                cor = GRAY; // Acertou não clicando
            }
            x = cordenada + coluna * 51;
            y = cordenada + linha * 51;
            DrawRectangle(x, y, 50, 50, cor);
        }
    }
}

int gerador_de_numeros(int i)
{
    int n;
    n = rand() % i;
    return n;
}

void limpar_matriz(int tamanho, int matriz[][tamanho])
{
    int linha, coluna;
    /* Loop para preencher as matrizes com 0 */
    for(linha = 0; linha<tamanho; linha++)
    {
        for(coluna = 0; coluna<tamanho; coluna++)
        {
            matriz[linha][coluna] = 0;
        } /* fim do loop das colunas */
    } /* fim do loop das linhas */
}

void aleatorizar_tabuleiro(int fase, int tamanho, int matriz[][tamanho])
{
    int linha_aleatoria, coluna_aleatoria, i;
    for(i=0; i<fase;)
    {
        linha_aleatoria = gerador_de_numeros(tamanho);
        coluna_aleatoria = gerador_de_numeros(tamanho);
        if (matriz[linha_aleatoria][coluna_aleatoria] == 0)
        {
            matriz[linha_aleatoria][coluna_aleatoria] = 1;
            i++;
        }
    }
}

int definir_tamanho(int fase){
    int tamanho;
    tamanho = (fase <= 5) ? 4 : (fase <= 10) ? 5 : 6;
    return tamanho;
}

void desenhar_contador(int tempo_atual, int tempo_inicio)
{
    int tempo_decorrido, progresso, countdown;
    tempo_decorrido = tempo_atual - tempo_inicio;
    progresso = tempo_decorrido * 50;
    countdown = 4 - tempo_decorrido;
    DrawRectangle(150, 400, progresso, 20, BLUE);
    DrawRectangleLines(150, 400, 200, 20, DARKGRAY);
    DrawText(TextFormat("%d", countdown), 250, 400, 20, WHITE);
}