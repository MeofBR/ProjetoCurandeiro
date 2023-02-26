/****************************************Informação do Arquivo********************************************
** Nome do Arquivo:          Display.h
** Data Ultima Modificação:  08-01-19
** Ultima Versão:            Sim
** Descrição:                Biblioteca do controle das mensagens exibidas na Display(Header).                     
**------------------------------------------------------------------------------------------------------
** Data de Criação:     02-03-18     
********************************************************************************************************/

#ifndef Display_h
#define Display_h
#include <Arduino.h>

/* Estruturas */
struct Msg_tela
{
  String MENU = "MENU";
};

//Pinos do Display LCD.
#define LCD_RS 10
#define LCD_E 9
#define LCD_D4 8
#define LCD_D5 7
#define LCD_D6 6
#define LCD_D7 5

#define NORMAL 0
#define CENTRALIZADO 1
#define DIREITA 2

// Definição da classe ControleIluminacao
class Display
{
  public:
      // Função que é o construtor da classe.
      Display(int linha, int coluna);
      
      // Método que limpa a tela.
      void limpa_tela(); 
      
      // Método que escreve na tela.
      void escreve(String mensagem, int tipo, int linha, int coluna);    
    
      // Método que escreve na tela.
      void escreve_tela(int tipo, String mensagem, int linha, int coluna, String mensagem1, int linha1, int coluna1, String mensagem2, int linha2, int coluna2, String mensagem3, int linha3, int coluna3); 
      
      // Método que escreve na tela um caractere especial.
      void escreve_char_especial(int Char, int linha, int coluna);  
      
      // Método que faz o setup do display.
      void begin(int linhas, int colunas);  
  
      // Método que ativa o blink.
      void blink(int linha, int coluna);  
  
      // Seta o cursor.
      void seta_cursor(int linha, int coluna);
     
      // Escreve simples.
      void escreve_simples(String mensagem);
      
      // Método que desativa o blink.
      void noBlink();     
      
  private:  
      // Quantidades de linhas no display.
      int qtd_linhas_display = 0;
};

#endif
