/****************************************Informação do Arquivo********************************************
** Nome do Arquivo:          Buzzer.cpp
** Data Ultima Modificação:  08-01-19
** Ultima Versão:            Sim
** Descrição:                Biblioteca do Buzzer.                     
**------------------------------------------------------------------------------------------------------
** Data de Criação:     02-03-18     
********************************************************************************************************/

#include "Buzzer.h"

/*********************************************************************************************************
** Nome da Função:       Buzzer
** Descrição:            Função que é o construtor da classe, pega do buzzer e o delay.
** Parametro:            Pino do buzzer e delay.
** Valor de retorno:     Não.
*********************************************************************************************************/
Buzzer::Buzzer(int pin_buzzer, int dlay_buzzer)
{ 
  // Define o pino do modulo buzzer como saida e o deixa em estado desligado.
  pinMode(pin_buzzer,OUTPUT);
  digitalWrite(pin_buzzer,LOW);

  // Transfere o valor dos pinos para variaveis privadas da classe.
  pino_buzzer = pin_buzzer;
  delay_buzzer = dlay_buzzer;
}

/*********************************************************************************************************
** Nome da Função:      task
** Descrição:    	      Tarefa que faz o processo de emitir o som do buzzer.
** Parametro:           Não.
** Valor de retorno:    Não.
*********************************************************************************************************/
void Buzzer::task()
{
  // Maquina de estado para efetuar o efeito sonoro do buzzer
  switch(controle_buzzer)
  {
    // Liga o buzzer.
    case 1:
          // Liga o buzzer.
          digitalWrite(pino_buzzer,HIGH);  
          time_start_buzzer = millis();            
          controle_buzzer++; 
          break;
    // Quando controle_buzzer igual a 2.      
    case 2:
          // Aguarda o tempo de DELAY_BUZZER para desligar o buzzer.
          tempo_atual_buzzer= millis();
          if((tempo_atual_buzzer-time_start_buzzer) > delay_buzzer)
          {
              // Desliga pino do buzzer.
              digitalWrite(pino_buzzer,LOW);
              controle_buzzer=0;
          } 
          break;      
  }
}

/*********************************************************************************************************
** Nome da Função:      liga
** Descrição:           Método que inicia o processo do efeito sonoro do buzzer, setando a variavel,
*                       controle_buzzer como 1
** Parametro:           Não
** Valor de retorno:    Não
*********************************************************************************************************/
void Buzzer::liga()
{
  // Seta a variavel controle_buzzer como 1, assim iniciando o processo do efeito sonoro.
  controle_buzzer = 1;
}
