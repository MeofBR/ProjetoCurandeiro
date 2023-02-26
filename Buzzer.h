/****************************************Informação do Arquivo*******************************************
** Nome do Arquivo:          Buzzer.h
** Data Ultima Modificação:  08-01-19
** Ultima Versão:            Sim
** Descrição:                Biblioteca do Buzzer(Header).                     
**------------------------------------------------------------------------------------------------------
** Data de Criação:     02-03-18     
********************************************************************************************************/

#ifndef Buzzer_h
#define Buzzer_h
#include <Arduino.h>


// Definição da classe Buzzer
class Buzzer
{
  public:
      // Função que é o construtor da classe, pega do buzzer e o delay.
      Buzzer(int pin_buzzer, int dlay_buzzer);

      void task();

      void liga();
  private:
      // Variaveis privadas da classe que armazenam os pinos.
      int pino_buzzer;
      int delay_buzzer;
      // Variaveis de controle de tempo.
      unsigned long tempo_atual_buzzer=0 , time_start_buzzer=0;
      // Controle do acionamento do buzzer.
      short controle_buzzer=0;
      short controle_buzzer1=0;
};

#endif
