/* ======================== Documentação do Projeto ======================== */
/**
 * @brief Projeto Jogo da Imitação
 * Autores: Felipe Gonçalves, Guilherme Barretto e Geovanna Sargaço.
 * Data: 06/2025
 * Versão: 1.0
 * Descrição:
 *   Este projeto com Arduino, tem o objetivo de testar a capacidade do 
 *  microcontrolador em "ouvir" e converter em tempo real uma informação 
 *  analógica recebida através de um microfone e destinguir ruídos. 
 *     O segundo objetivo é ser um jogo de adivinhação multijogador, onde
 *  os jogadores devem tentar adivinhar qual o número que o último 
 *  jogador escolheu.
 *     Ajudando a desenvolver a capacidade de reconhecimento de padrões
 *  e de raciocínio lógico.
 *
 *  Funcionalidades:
 *  - 
 *
 *  Hardware Necessário (para cada jogador):
 *  - Placa Arduino e uma fonte de alimentação.
 *  - Protoboard.
 *  - Display LCD 16x2.
 *  - 12x Jumpers.
 *  - LED Receptor e Emissor Infravermelho
 *  - 4x Botões.
 *  - Leds amarelo, verde e vermelho.
 *  - Resistor de 300 ohms.
*/

/* ======================== Inclusão de Bibliotecas ======================== */

// Inclui a biblioteca LiquidCrystal
#include <LiquidCrystal.h>

/* ===================== Definição de Constantes/Macros ==================== */

#define LED_VERDE 10
#define LED_AMARELO 9
#define LED_VERMELHO 8
#define BUZZER 7
#define MICROFONE A0

// Como vai faltar pinos no Arduino, os botões serão ligados aos pinos analógicos.
#define BOTAO_ACAO A2
#define BOTAO_DIMINUIR A3
#define BOTAO_CONFIRMAR A4
#define BOTAO_AUMENTAR A5

/* ==================== Declaração de Variáveis Globais ==================== */

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int modo = 0,
    etapa = 0;

/* ========================= Protótipos de Funções ========================= */

void arduino_falante();
void arduino_ouvinte();
int escolher_numero();
void selecionar_modo();
void resetar();

/* ============================== Função Setup ============================= */

void setup() {
  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);
  pinMode(BOTAO_ACAO, INPUT);
  pinMode(BOTAO_DIMINUIR, INPUT);
  pinMode(BOTAO_CONFIRMAR, INPUT);
  pinMode(BOTAO_AUMENTAR, INPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(MICROFONE, INPUT);

  Serial.begin(9600);
}

/* ============================== Função Loop ============================== */
void loop() {
  switch (etapa) {
    case 0:
      selecionar_modo();
      break;
    case 1:
      if (modo == 0) {
        arduino_falante();
      } else if (modo == 1) {
        arduino_ouvinte();
      }
      break;
  }
}

/* ====================== Desenvolvimento das Funções ====================== */

void selecionar_modo() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Escolha o modo:");
  while (digitalRead(BOTAO_CONFIRMAR) == LOW) {
    lcd.setCursor(0, 1);
    if (digitalRead(BOTAO_AUMENTAR) == HIGH) {
      modo = 1;
    } else if (digitalRead(BOTAO_DIMINUIR) == HIGH) {
      modo = 0;
    }
    if (modo == 1) {
      lcd.print("enviar..>RECEBER");
    } else {
      lcd.print("ENVIAR<..receber");
    }
  }
  etapa = 1;
}

// Função para o arduino falante.
void arduino_falante() {
  int numero = escolher_numero();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enviando...");
  while (digitalRead(BOTAO_ACAO) == LOW) {
    switch (numero) {
      case 1 : 
        irsend.sendNEC(0xFF8877, 32);
        break;
      case 2:
        irsend.sendNEC(0xFF48B7, 32);
        break;
      case 3:
        irsend.sendNEC(0xFFC837, 32);
        break;
      case 4:
        irsend.sendNEC(0xFF28D7, 32);
        break;
      case 5:
        irsend.sendNEC(0xFFA857, 32);
        break;
      case 6:
        irsend.sendNEC(0xFF6897, 32);
        break;
      case 7: 
        irsend.sendNEC(0xFFE817, 32);
        break;
      case 8:
        irsend.sendNEC(0xFF18E7, 32);
        break;
      case 9:
        irsend.sendNEC(0xFF9867, 32);
        break;
    }
  }
}

// Função para o jogador escolher o número.
int escolher_numero() {
  int numero = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Escolha o numero");
  lcd.setCursor(0, 1);
  lcd.print("entre 0 a 10");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Numero: ");
  while (digitalRead(BOTAO_CONFIRMAR) == LOW) {
    lcd.setCursor(0, 1);
    lcd.print(numero);
    if (digitalRead(BOTAO_AUMENTAR) == HIGH && numero < 10) {
      numero++;
      delay(200);
    }
    if (digitalRead(BOTAO_DIMINUIR) == HIGH && numero > 0) {
      numero--;
      delay(200);
    }
  }
  return numero;
}

// Função para o arduino ouvinte.
void arduino_ouvinte() {
  int i = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Decodificando...");
  delay(500);
  while (digitalRead(BOTAO_ACAO) == LOW && i < 4) {
      
      i++;
    }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sinais Reconhecidos");
  delay(1000);
}