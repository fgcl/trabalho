/* ======================== Documentação do Projeto ======================== */
/**
* @brief Projeto Decifra Bits
Autores: Felipe Gonçalves, Guilherme Barretto, Geovanna Sargaço
Data: 06/2025
Versão: 1.0
Descrição:
   Este projeto com Arduino, tem o objetivo de testar a capacidade do 
   microcontrolador em "ouvir" e converter em tempo real uma informação 
   analógica recebida através de um microfone e destinguir ruídos. 
   O segundo objetivo é ser um jogo de adivinhação multijogador, onde
   os jogadores devem tentar adivinhar qual o número que o último 
   jogador escolheu.
   Ajudando a desenvolver a capacidade de reconhecimento de padrões
   e de raciocínio lógico.

   Funcionalidades:
   - 

   Hardware Necessário (para cada jogador):
   - Placa Arduino e uma fonte de alimentação.
   - Protoboard.
   - Display LCD 16x2.
   - 12x Jumpers.
   - Buzzer.
   - Microfone.
   - 4x Botões.
   - Leds amarelo, verde e vermelho.
   - Resistor de 300 ohms.
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
#define BOTAO_ACAO A2
#define BOTAO_DIMINUIR A3
#define BOTAO_CONFIRMAR A4
#define BOTAO_AUMENTAR A5

/* ==================== Declaração de Variáveis Globais ==================== */

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
estado = 0;

/* ========================= Protótipos de Funções ========================= */

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
   while (digitalRead(BOTAO_CONFIRMAR) == HIGH) {
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