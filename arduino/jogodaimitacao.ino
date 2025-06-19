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
 *  Instalem a IRremote.h
*/

/* ======================== Inclusão de Bibliotecas ======================== */

// Inclui a biblioteca LiquidCrystal
#include <LiquidCrystal.h>
#include "IRremote.h"

/* ===================== Definição de Constantes/Macros ==================== */

#define LED_VERDE 10
#define LED_AMARELO 9
#define LED_VERMELHO 8
#define BUZZER 7
#define MICROFONE A0
#define RECEPTOR 13
#define EMISSOR 3

// Como vai faltar pinos no Arduino, os botões serão ligados aos pinos analógicos.
#define BOTAO_DIMINUIR A3
#define BOTAO_CONFIRMAR A4
#define BOTAO_AUMENTAR A5

/* ==================== Declaração de Variáveis Globais ==================== */

LiquidCrystal lcd(12, 11, 5, 4, 6, 2);
int modo = 0,
    etapa = 0,
    numero_decodificado = 0;
IRsend irsend;
IRrecv irrecv(RECEPTOR);
decode_results results;

/* ========================= Protótipos de Funções ========================= */

void arduino_falante();
void arduino_ouvinte();
int escolher_numero();
int leitura_infravermelho();
void selecionar_modo();
void resetar();
void reproduzir_som(int bit1, int bit2, int bit3, int bit4);

/* ============================== Função Setup ============================= */

void setup() {
  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);
  pinMode(BOTAO_DIMINUIR, INPUT);
  pinMode(BOTAO_CONFIRMAR, INPUT);
  pinMode(BOTAO_AUMENTAR, INPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(MICROFONE, INPUT);
  pinMode(RECEPTOR, INPUT);
  pinMode(EMISSOR, OUTPUT);
  irrecv.enableIRIn(); 
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

void arduino_falante() {
  int numero = escolher_numero();
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enviando...");
  while (digitalRead(BOTAO_CONFIRMAR) == LOW) {
    switch (numero) {
      case 1:  // equivale a 0001
        reproduzir_som(200, 200, 200, 800);
        irsend.sendNEC(0xFF8877, 32);
        break;
      case 2:  // equivale a 0010
        reproduzir_som(200, 200, 800, 200);
        irsend.sendNEC(0xFF48B7, 32);
        break;
      case 3:  // equivale a 0011
        reproduzir_som(200, 200, 800, 800);
        irsend.sendNEC(0xFFC837, 32);
        break;
      case 4:  // equivale a 0100
        reproduzir_som(200, 800, 200, 200);
        irsend.sendNEC(0xFF28D7, 32);
        break;
      case 5:  // equivale a 0101
        reproduzir_som(200, 800, 200, 800);
        irsend.sendNEC(0xFFA857, 32);
        break;
      case 6:  // equivale a 0110
        reproduzir_som(200, 800, 800, 200);
        irsend.sendNEC(0xFF6897, 32);
        break;
      case 7:  // equivale a 0111
        reproduzir_som(200, 800, 800, 800);
        irsend.sendNEC(0xFFE817, 32);
        break;
      case 8:  // equivale a 1000
        reproduzir_som(800, 200, 200, 200);
        irsend.sendNEC(0xFF18E7, 32);
        break;
      case 9:  // equivale a 1001
        reproduzir_som(800, 200, 200, 800);
        irsend.sendNEC(0xFF9867, 32);
        break;
    }
  }
  etapa = 0;
}

void reproduzir_som(int bit1, int bit2, int bit3, int bit4) {
  tone(BUZZER, bit1);
  digitalWrite(LED_AMARELO, HIGH);
  delay(bit1);
  digitalWrite(LED_AMARELO, LOW);
  delay(bit1);
  tone(BUZZER, bit2);
  digitalWrite(LED_AMARELO, HIGH);
  delay(bit2);
  digitalWrite(LED_AMARELO, LOW);
  delay(bit2);
  tone(BUZZER, bit3);
  digitalWrite(LED_AMARELO, HIGH);
  delay(bit3);
  digitalWrite(LED_AMARELO, LOW);
  delay(bit3);
  tone(BUZZER, bit4);
  digitalWrite(LED_AMARELO, HIGH);
  delay(bit4);
  digitalWrite(LED_AMARELO, LOW);
  delay(bit4);

  noTone(BUZZER);
  delay(1000);
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
  int numero_decodificado = 0,
  escolha_do_jogador = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Lendo...");
  while (numero_decodificado == 0) {
      numero_decodificado = leitura_infravermelho();
  }
  escolha_do_jogador = escolher_numero();
  if (escolha_do_jogador == numero_decodificado) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Acertou!");
    digitalWrite(LED_VERDE, HIGH);
    delay(1000);
    digitalWrite(LED_VERDE, LOW);
    delay(1000);
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Errou!");
    digitalWrite(LED_VERMELHO, HIGH);
    delay(1000);
    digitalWrite(LED_VERMELHO, LOW);
    delay(1000);
  }
  delay(1000);
}

int leitura_infravermelho(){
  int numero = 0;
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    if (results.value == 0xFF8877){
      numero = 1;
    } else if (results.value == 0xFF48B7){
      numero = 2;
    } else if (results.value == 0xFFC837){
      numero = 3;
    } else if (results.value == 0xFF28D7){
      numero = 4;
    } else if (results.value == 0xFFA857){
      numero = 5;
    } else if (results.value == 0xFF6897){
      numero = 6;
    } else if (results.value == 0xFFE817){
      numero = 7;
    } else if (results.value == 0xFF18E7){
      numero = 8;
    } else if (results.value == 0xFF9867){
      numero = 9;
    }
  }
  irrecv.resume(); // Recebe o próximo valor
  return numero;
}