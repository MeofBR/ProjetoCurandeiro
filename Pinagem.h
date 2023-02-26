/****************************************Informação do Arquivo********************************************
** Nome do Arquivo:          Pinagem.h
** Data Ultima Modificação:  08-01-19
** Ultima Versão:            Sim
** Descrição:                Biblioteca do controle dos pinos I/O utilizados.                 
**------------------------------------------------------------------------------------------------------
** Data de Criação:     02-03-18     
********************************************************************************************************/

#ifndef Pinagem_h
#define Pinagem_h
#include <Arduino.h>

// Define apelidos para os botoes
#define BT_AVANCA 53
#define BT_VOLTA 49
#define BT_SELECIONA 51

// Leds
#define LED_VERD 47
#define LED_VERM 43
#define LED_AMAR 45

// Define as constantes para os pinos da ponte H e motores
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 4
#define ENA 3
#define ENB A3

//Pino BUZZER
#define PINO_BUZZER A5 
//Delay do buzzer
#define DELAY_BUZZER 80

#endif
