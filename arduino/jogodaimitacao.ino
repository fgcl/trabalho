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
 *  - Buzzer.
 *  - Microfone.
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
int estado = 0;

/* ========================= Protótipos de Funções ========================= */

void arduino_falante();
void arduino_ouvinte();
int escolher_numero();
void reproduzir_som(int byte1, int byte2, int byte3, int byte4);

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
}

/* ============================== Função Loop ============================== */
void loop() {
   // O arduino entra no modo falante.
   if (digitalRead(BOTAO_ACAO) == HIGH) {
      estado = 1;
   }
   // O arduino entra no modo ouvinte.
   if (digitalRead(BOTAO_ACAO) == LOW) {
      estado = 0;
   }
}

/* ====================== Desenvolvimento das Funções ====================== */

// Função para o arduino falante.
void arduino_falante() {
   int numero = escolher_numero();
   while(BOTAO_ACAO == HIGH){
      switch (numero) {
         case 1: // equivale a 0001
            reproduzir_som(200, 200, 200, 800);
            break;
         case 2: // equivale a 0010
            reproduzir_som(200, 200, 800, 200);
            break;
         case 3: // equivale a 0011
            reproduzir_som(200, 200, 800, 800);
            break;
         case 4: // equivale a 0100
            reproduzir_som(200, 800, 200, 200);
            break;
         case 5: // equivale a 0101
            reproduzir_som(200, 800, 200, 800);
            break;
         case 6: // equivale a 0110
            reproduzir_som(200, 800, 800, 200);
            break;
         case 7: // equivale a 0111
            reproduzir_som(200, 800, 800, 800);
            break;
         case 8: // equivale a 1000
            reproduzir_som(800, 200, 200, 200);
            break;
         case 9: // equivale a 1001
            reproduzir_som(800, 200, 200, 800);
            break;
      }
   }
}

void reproduzir_som(int bit1, int bit2, int bit3, int bit4) {
   tone(BUZZER, bit1);
   delay(200);
   tone(BUZZER, bit2);
   delay(200);
   tone(BUZZER, bit3);
   delay(200);
   tone(BUZZER, bit4);
   delay(200);
   noTone(BUZZER);
}

// Função para o jogador escolher o número.
int escolher_numero() {
   int numero = 0;
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Escolha um numero");
   lcd.setCursor(0,1);
   lcd.print("entre 0 e 10");
   delay(1000);
   while (digitalRead(BOTAO_CONFIRMAR) == LOW) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Numero escolhido: ");
      lcd.print(numero);
      if (digitalRead(BOTAO_AUMENTAR) == HIGH && numero < 10) {
         numero++;
      }
      if (digitalRead(BOTAO_DIMINUIR) == HIGH && numero > 0) {
         numero--;
      }
   }
   lcd.clear();
   return numero;
}

// TODO: Refatorar as funções abaixo.
// Função para o arduino ouvinte.
void arduino_ouvinte() {
  int bits[4];
  int i = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Aguardando Sons");
  while (digitalRead(BOTAO_ACAO) == LOW && i < 4) {
    int valor_analogico = analogRead(MICROFONE);
    if (valor_analogico > 500) {
      bits[i] = map(valor_analogico, 500, 1023, 200, 800);
      delay(200);
      i++;
    }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sons Capturados");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Analisando...");
  delay(1000);

  //Serial.print(bits[0]);
  //Serial.print(bits[1]);
  //Serial.print(bits[2]);
  //Serial.print(bits[3]);
  
  verificar_sequencia(bits[0], bits[1], bits[2], bits[3]);
}

void verificar_sequencia(int bit1, int bit2, int bit3, int bit4){
   if(bit1 == 200 && bit2 == 200 && bit3 == 200 && bit4 == 800){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Numero: 1");
      delay(1000);
   } else if(bit1 == 200 && bit2 == 200 && bit3 == 800 && bit4 == 200){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Numero: 2");
      delay(1000);
   }else if(bit1 == 200 && bit2 == 200 && bit3 == 800 && bit4 == 800){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Numero: 3");
      delay(1000);
   }else if(bit1 == 200 && bit2 == 800 && bit3 == 200 && bit4 == 200){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Numero: 4");
      delay(1000);
   }else if(bit1 == 200 && bit2 == 800 && bit3 == 200 && bit4 == 800){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Numero: 5");
      delay(1000);
   }else if(bit1 == 200 && bit2 == 800 && bit3 == 800 && bit4 == 200){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Numero: 6");
      delay(1000);
   }else if(bit1 == 200 && bit2 == 800 && bit3 == 800 && bit4 == 800){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Numero: 7");
      delay(1000);
   }else if(bit1 == 800 && bit2 == 200 && bit3 == 200 && bit4 == 200){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Numero: 8");
      delay(1000);
   }else if(bit1 == 800 && bit2 == 200 && bit3 == 200 && bit4 == 800){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Numero: 9");
      delay(1000);
   }else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Numero Invalido");
      delay(1000);
   }
}
