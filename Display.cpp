/****************************************Informação do Arquivo********************************************
** Nome do Arquivo:          Display.cpp
** Data Ultima Modificação:  08-01-19
** Ultima Versão:            Sim
** Descrição:                Biblioteca do controle das mensagens exibidas na Display(Header).                     
**------------------------------------------------------------------------------------------------------
** Data de Criação:     02-03-18     
********************************************************************************************************/

#include "Arduino.h"
#include "Display.h"
#include "LiquidCrystal.h"

// Desenho Voltar do display
byte voltar[8] = {                                    
  B00001,
  B00001,
  B00001,
  B00101,
  B01001,
  B11111,
  B01000,
  B00100
};

// Desenho seta do display
byte seta[8] = {
  B00000,
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000
};

// Desenho do grau da temperatura.
byte grau[8] = {
  B00110,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000,
  B00000,
  B00000
};

// Desenho bloco vazio.
byte vazio[8] = {
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111
};

// Desenho bloco cheio.
byte cheio[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

// Desenho de checado.
byte checado[8] = {
  B00000,
  B00000,
  B00001,
  B00010,
  B10100,
  B01000,
  B00000,
  B00000
};  

// Definicoes do display 20x4
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7); 

/*********************************************************************************************************
** Nome da Função:       Display
** Descrição:            Função que é o construtor da classe.
** Parametro:            Não.
** Valor de retorno:     Não.
*********************************************************************************************************/
Display::Display(int linhas, int colunas)
{    
  lcd.begin(linhas, colunas); 
}

/*********************************************************************************************************
** Nome da Função:      display_begin
** Descrição:           Método que faz o setup do display.
** Parametro:           Não
** Valor de retorno:    Não
*********************************************************************************************************/
void Display::begin(int linhas, int colunas)
{  
  // Display, caracteres especiais
  lcd.createChar(3,voltar);         
  lcd.createChar(4,seta);
  lcd.createChar(5,grau);
  lcd.createChar(7,vazio);
  lcd.createChar(8,cheio);
  lcd.createChar(9,checado);
  lcd.begin(20, 4); 
  
  // Armazena valor da quantidade de linhas do display;
  qtd_linhas_display = linhas;
}

/*********************************************************************************************************
** Nome da Função:      limpa_tela
** Descrição:           Método que limpa a tela.
** Parametro:           Não
** Valor de retorno:    Não
*********************************************************************************************************/
void Display::limpa_tela()
{
  lcd.clear();
}

/*********************************************************************************************************
** Nome da Função:      limpa_tela
** Descrição:           Método que limpa a tela.
** Parametro:           Não
** Valor de retorno:    Não
*********************************************************************************************************/
void Display::seta_cursor(int linha, int coluna)
{
  lcd.setCursor(linha,coluna);
}

/*********************************************************************************************************
** Nome da Função:      limpa_tela
** Descrição:           Método que limpa a tela.
** Parametro:           Não
** Valor de retorno:    Não
*********************************************************************************************************/
void Display::escreve_simples(String mensagem)
{
  lcd.print(mensagem);
}

/*********************************************************************************************************
** Nome da Função:      escreve
** Descrição:           Método que escreve na tela.
** Parametro:           Não
** Valor de retorno:    Não
*********************************************************************************************************/
void Display::escreve(String mensagem, int tipo = NORMAL, int linha = 0, int coluna = 0)
{
  int qtd_char = 0;
  switch ( tipo )
  {
    case NORMAL:
              lcd.setCursor(linha,coluna);
              lcd.print(mensagem);
              break;
    case CENTRALIZADO:
              qtd_char = mensagem.length();
              linha = (qtd_linhas_display - qtd_char)/2;
              lcd.setCursor(linha,coluna);
              lcd.print(mensagem);
              break;
    case DIREITA:
              qtd_char = mensagem.length();
              linha = (qtd_linhas_display - qtd_char);
              Serial.println(linha);
              lcd.setCursor(linha,coluna);
              lcd.print(mensagem);
              break;            
  }  
}

/*********************************************************************************************************
** Nome da Função:      escreve_tela
** Descrição:           Método que escreve na tela.
** Parametro:           Não
** Valor de retorno:    Não
*********************************************************************************************************/
void Display::escreve_tela(int tipo, String mensagem, int linha, int coluna, String mensagem1, int linha1, int coluna1, String mensagem2, int linha2, int coluna2, String mensagem3, int linha3, int coluna3)
{
  int qtd_char = 0;
  
  switch ( tipo )
  {
    case NORMAL:
              lcd.setCursor(linha,coluna);
              lcd.print(mensagem);
              lcd.setCursor(linha1,coluna1);
              lcd.print(mensagem1);
              lcd.setCursor(linha2,coluna2);
              lcd.print(mensagem2);
              lcd.setCursor(linha3,coluna3);
              lcd.print(mensagem3);
              break;
    case CENTRALIZADO:
              lcd.clear();
              qtd_char = mensagem.length();
              linha = (qtd_linhas_display - qtd_char)/2;
              lcd.setCursor(linha,coluna);
              lcd.print(mensagem);
              qtd_char = mensagem1.length();
              linha1 = (qtd_linhas_display - qtd_char)/2;
              lcd.setCursor(linha1,coluna1);
              lcd.print(mensagem1);
              qtd_char = mensagem2.length();
              linha2 = (qtd_linhas_display - qtd_char)/2;
              lcd.setCursor(linha2,coluna2);
              lcd.print(mensagem2);
              qtd_char = mensagem3.length();
              linha3 = (qtd_linhas_display - qtd_char)/2;
              lcd.setCursor(linha3,coluna3);
              lcd.print(mensagem3);
              break;    
     case DIREITA:
              lcd.clear();
              qtd_char = mensagem.length();
              linha = (qtd_linhas_display - qtd_char);
              lcd.setCursor(linha,coluna);
              lcd.print(mensagem);
              qtd_char = mensagem1.length();
              linha1 = (qtd_linhas_display - qtd_char);
              lcd.setCursor(linha1,coluna1);
              lcd.print(mensagem1);
              qtd_char = mensagem2.length();
              linha2 = (qtd_linhas_display - qtd_char);
              lcd.setCursor(linha2,coluna2);
              lcd.print(mensagem2);
              qtd_char = mensagem3.length();
              linha3 = (qtd_linhas_display - qtd_char);
              lcd.setCursor(linha3,coluna3);
              lcd.print(mensagem3);
              break;         
  }
  
}

/*********************************************************************************************************
** Nome da Função:      escreve
** Descrição:           Método que escreve na tela.
** Parametro:           Não
** Valor de retorno:    Não
*********************************************************************************************************/
void Display::escreve_char_especial(int Char, int linha, int coluna)
{
  lcd.setCursor(linha,coluna);
  lcd.write(Char);
}


/*********************************************************************************************************
** Nome da Função:      blink
** Descrição:           Método que ativa o blink.
** Parametro:           Não
** Valor de retorno:    Não
*********************************************************************************************************/
void Display::blink(int linha, int coluna)
{
  lcd.blink();
  lcd.setCursor(linha,coluna);
}

/*********************************************************************************************************
** Nome da Função:      noBlink
** Descrição:           Método que desativa o blink.
** Parametro:           Não
** Valor de retorno:    Não
*********************************************************************************************************/
void Display::noBlink()
{
  // 
  lcd.noBlink();
}
