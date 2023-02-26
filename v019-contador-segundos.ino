/****************************************Informação do Arquivo*************************************************
** Nome do Arquivo:          projeto_curandeiro.ino versão: 8.
** Data Ultima Modificação:  22-02-18
** Ultima Versão:            Sim
** Descrição:                Software controlador do sistema da máquina "Curandeiro".
**------------------------------------------------------------------------------------------------------
** Data de Criação:     22-12-18       
********************************************************************************************************/

/**********************************************************************************
** Observações:
** - Compilar utilizando versão com suporte a Serial HW com 9bits.
** - Alterar o tamanho do buffer RX de 64 para 128 bytes
**   <Arduino IDE>\hardware\arduino\avr\cores\arduino\HardwareSerial.h
**   "#define SERIAL_RX_BUFFER_SIZE 64" para "#define SERIAL_RX_BUFFER_SIZE 128"
***********************************************************************************/

/* Bibliotecas. */
#include <LiquidCrystal.h>
#include "Pinagem.h"
#include "Buzzer.h"
#include "Display.h"

/* Definição de versão do software e hardware. */
#define V_SFT "1.19"
#define V_HDW 1

/* Debug Serial. */
//#define DEBUG

/* Constantes */
// Dimensao display.
#define LINHAS 20
#define COLUNAS 4

// Constantes funcao bt_press
#define AVANCAR 1
#define VOLTAR 2
#define SELECIONA 3

// Ativo inativo
#define ATIVO 1
#define INATIVO 0

// Configurações de tempo em segundos.
#define TEMPO_LIMPAR_MODELO 900 // (15 min)
#define TEMPO_CURAR_MODELO 600 // (10min)
#define TEMPO_LIMPAR_TEMPORARIO 1200 // (20min)
#define TEMPO_CURAR_TEMPORARIO 900 // (15 min)
#define TEMPO_LIMPAR_FUNDICAO 600 // (10min)
#define TEMPO_CURAR_FUNDICAO 180 // (3min)
#define TEMPO_LIMPAR_BITESPLINT 600 // (10min)
#define TEMPO_CURAR_BITESPLINT 300 // (50min)
#define TEMPO_LIMPAR_GUIA_CIRURGICO 60 // (1min)
#define TEMPO_CURAR_GUIA_CIRURGICO 30 // (30s)

// Estados task operacao.
#define INICIALIZACAO 0
#define AGUARDA_INICIALIZACAO 1
#define MENU_PRINCIPAL 2
#define SELECAO_MENU_PRINCIPAL 3
#define FUNCAO_MODELO 4
#define AGUARDA_LIMPEZA 5
#define AGUARDA_CURAR 6
#define FINALIZA_OPERACAO 7
#define FUNCAO_TEMPORARIO 8
#define AGUARDA_LIMPEZA_TEMPORARIO 9
#define AGUARDA_CURAR_TEMPORARIO 10
#define FINALIZA_OPERACAO_TEMPORARIO 11
#define FUNCAO_FUNDICAO 12
#define AGUARDA_LIMPEZA_FUNDICAO 13
#define AGUARDA_CURAR_FUNDICAO 14
#define FINALIZA_OPERACAO_FUNDICAO 15
#define FUNCAO_BITESPLINT 16
#define AGUARDA_LIMPEZA_BITESPLINT 17
#define AGUARDA_CURAR_BITESPLINT 18
#define FINALIZA_OPERACAO_BITESPLINT 19
#define FUNCAO_GUIA_CIRURGICO 20
#define AGUARDA_LIMPEZA_GUIA_CIRURGICO 21
#define AGUARDA_CURAR_GUIA_CIRURGICO 22
#define FINALIZA_OPERACAO_GUIA_CIRURGICO 23

// Menu principal.
#define QTD_MENU_PRINCIPAL 4

// Constantes operacoes.
#define MODELO 0
#define TEMPORARIO 1
#define FUNDICAO 2
#define BITE_SPLINT 3
#define GUIA_CIRURGICO 4

// Constantes booleanas
#define TRUE 1
#define FALSE 2

/* Variáveis */
// Barra
int contador_barra_progressiva = 0;
// Contador porcentagem.
int contador_porcentagem = 0;
// Contador ativo
int contador_ativo = 0;
// Controle da MS de operacao.
int estado_operacao = 0;
// Posicao do menu principal.
int posicao_menu_principal = 0;
// Tempo de espera.
long tempo_espera = 0;
// Selecao menu principal.
int selecao = 0;
// Contador segundos.
int contador_segundos = 0;
// Contador 10 porcento.
int contador_dez_porcento = 0;
// Em operacao
int em_operacao = 0;

// Temporizadores.
long tempo_inicio_inicializacao = millis(), tempo_atual_inicializacao = millis();
long tempo_inicio_barra_progressiva = millis(), tempo_atual_barra_progressiva = millis();
long tempo_inicio_contador_porcentagem = millis(), tempo_atual_contador_porcentagem = millis();
long tempo_inicio_bt = millis(), tempo_atual_bt = millis();
long tempo_inicio_buzzer = millis(), tempo_atual_buzzer = millis();
long tempo_inicio_lcd = millis(), tempo_atual_lcd = millis();
long tempo_inicio_operacao = millis(), tempo_atual_operacao = millis();
long tempo_inicio_contador_segundos = millis(), tempo_atual_contador_segundos = millis();
long time_start_processamento = millis(), tempo_atual_processamento = millis();

/* Instancias */
// Buzzer
Buzzer buzzer(PINO_BUZZER,DELAY_BUZZER);
// Display LCD.
Display display_lcd(LINHAS,COLUNAS);

/*********************************************************************************************************
** Nome da Função:       setup
** Descrição:            Setup principal da main, onde é feita a inicialização das portas seriais/pinos/
                         configurações,etc.
** Parametro:            Não.
** Valor de retorno:     Não.
*********************************************************************************************************/
void setup()
{
  // Iinicia serial principal de comunicação.
  Serial.begin(115200);
  // Funcao de inicializacao de pinos e estruturas.
  inicializacao();   
}

/*********************************************************************************************************
** Nome da Função:       inicializacao
** Descrição:            Funcao que define valor inicial dos parametros, inicia memoria e le os dados 
                         necessarios.
** Parametro:            Não.
** Valor de retorno:     Não.
*********************************************************************************************************/
void inicializacao()
{
  // Inicialização do display lcd.
  display_lcd.begin(LINHAS, COLUNAS);
  // Inicializa pinos.
  inicia_pinos();  
}

/*********************************************************************************************************
** Nome da Função:       inicia_pinos
** Descrição:            Funcao que define os pinos de I/O.
** Parametro:            Não.
** Valor de retorno:     Não.
*********************************************************************************************************/
void inicia_pinos()
{
  // Seta os botoes de interação.
  pinMode (BT_AVANCA, INPUT_PULLUP);
  pinMode (BT_VOLTA, INPUT_PULLUP);
  pinMode (BT_SELECIONA, INPUT_PULLUP);
  // Seta os pinos da ponte H e motores como output.
  pinMode (IN1,OUTPUT); 
  pinMode (IN2,OUTPUT); 
  pinMode (IN3,OUTPUT); 
  digitalWrite (IN1,HIGH); 
  digitalWrite (IN2,HIGH);
  digitalWrite (IN3,HIGH);
  pinMode (LED_VERM,OUTPUT); 
  pinMode (LED_AMAR,OUTPUT); 
  pinMode (LED_VERD,OUTPUT); 
  digitalWrite (LED_VERM,LOW); 
  digitalWrite (LED_AMAR,LOW);
  digitalWrite (LED_VERD,LOW);
  pinMode (ENA,OUTPUT);
  pinMode (ENB,OUTPUT);
}

/*********************************************************************************************************
** Nome da Função:       reinicia_lcd
** Descrição:            Reinicia display lcd.
** Parametro:            Não.
** Valor de retorno:     Não.
*********************************************************************************************************/
void reinicia_lcd()
{
  // A cada 10s faz o lcd reiniciar para evitar bugs.
  tempo_atual_lcd = millis();
  if((tempo_atual_lcd-tempo_inicio_lcd) > 10000)
  {
    // Verifica se esta no menu principal.
    if ( estado_operacao == SELECAO_MENU_PRINCIPAL )
    {
      Serial.println("REINICIOU LCD");
      // Inicialização do display lcd.
      display_lcd.begin(LINHAS, COLUNAS); 
      delay(50);       
      estado_operacao = MENU_PRINCIPAL;
    } 
    // Reinicia temporizador.
    tempo_inicio_lcd = millis();       
  }
}

/*********************************************************************************************************
** Nome da Função:       led_processamento
** Descrição:            Tarefa faz o led de processamento piscar.
** Parametro:            Não.
** Valor de retorno:     Não.
*********************************************************************************************************/
void led_processamento()
{
  // A cada 500ms liga e desliga o led.
  tempo_atual_processamento = millis();
  if((tempo_atual_processamento-time_start_processamento) > 500)
  {    
    // Se led estiver ligado.  
    if ( digitalRead(LED_VERD) == HIGH )  
      // Desliga o led.    
      digitalWrite(LED_VERD,LOW);
    // Se desligado.  
    else
      // Liga o led.
      digitalWrite(LED_VERD,HIGH);
   
    // Reseta temporizador.
    time_start_processamento = tempo_atual_processamento;      
  }
}

/*********************************************************************************************************
** Nome da Função:       loop
** Descrição:            Loop principal do sistema, onde todas tarefas são executadas.
** Parametro:            Não.
** Valor de retorno:     Não.
*********************************************************************************************************/
void loop() 
{  
  // Cancela operacao.
  cancela_operacao();
  
  // Reinicia o lcd.
  reinicia_lcd();

  // Máquina de estados principal de interação.
  task_operacao();

  // Tarefa que verifica se é para emitir o efeito sonoro do beep.
  buzzer.task();

  // Tarefa que faz piscar o led para mostrar processamento
  led_processamento();
}

/*********************************************************************************************************
** Nome da Função:       task_venda
** Descrição:            Tarefa responsavel pela interação do usuario com a selecao do produto, visualização
                         do credito.
** Parametro:            Não.
** Valor de retorno:     Não.
*********************************************************************************************************/
void task_operacao()
{
  // Maquina de estados da operacao.
  switch(estado_operacao)
  {
    // Case principal de venda, com a intereção com o usuario.
    case INICIALIZACAO:
      display_lcd.limpa_tela();
      display_lcd.escreve("INICIANDO", CENTRALIZADO, 0, 1);
      display_lcd.escreve("  AGUARDE    V" +(String)V_SFT, NORMAL, 0, 2);
      // Inicia temporizador.
      tempo_inicio_inicializacao = millis();
      // Continua para proximo estado.
      estado_operacao = AGUARDA_INICIALIZACAO;
      break;  
    // Aguarda inicializacao.
    case AGUARDA_INICIALIZACAO:
      // Aguarda passar 2 segundos.
      tempo_atual_inicializacao = millis();
      if((tempo_atual_inicializacao-tempo_inicio_inicializacao) > 1000 )
      {    
        digitalWrite(LED_VERM,LOW);
        digitalWrite(LED_AMAR,HIGH);
        display_lcd.escreve(".", NORMAL, 9, 2);
      }
      // Aguarda passar 2 segundos.
      tempo_atual_inicializacao = millis();
      if((tempo_atual_inicializacao-tempo_inicio_inicializacao) > 2000 )
      {    
        digitalWrite(LED_VERM,HIGH);
        digitalWrite(LED_AMAR,LOW);
        display_lcd.escreve(".", NORMAL, 10, 2);
      }
      // Aguarda passar 2 segundos.
      tempo_atual_inicializacao = millis();
      if((tempo_atual_inicializacao-tempo_inicio_inicializacao) > 3000 )
      {    
        digitalWrite(LED_VERM,HIGH);
        digitalWrite(LED_AMAR,HIGH);
        display_lcd.escreve(".", NORMAL, 11, 2);
      }
      // Aguarda passar 2 segundos.
      tempo_atual_inicializacao = millis();
      if((tempo_atual_inicializacao-tempo_inicio_inicializacao) > 4000 )
      {    
        digitalWrite(LED_VERM,LOW);
        digitalWrite(LED_AMAR,LOW);
        // Continua para proximo estado.
        estado_operacao = MENU_PRINCIPAL;
      }
      break;
    // Menu principal.
    case MENU_PRINCIPAL:
      // Chama o menu principal.
      menu_principal();
      // Continua para proximo estado.
      estado_operacao = SELECAO_MENU_PRINCIPAL;
      break;  
    // Percorre o menu atraves dos botoes de interação.   
    case SELECAO_MENU_PRINCIPAL: 
      // Pega se algum botao foi pressionado.
      selecao = bt_press();

      if ( selecao != 0 )
      {
        // De acordo com o botao pressionado movimenta o menu.     
        switch(selecao)
        {
          // Se botao avanar pressionado.
          case AVANCAR:
            if ( posicao_menu_principal < QTD_MENU_PRINCIPAL )
              posicao_menu_principal++;
            else
              posicao_menu_principal = 0;
  
            // Volta para mostrar nova selecao.
            estado_operacao = MENU_PRINCIPAL;    
            break;          
          // Se botao voltar pressionado.  
          case VOLTAR:
            if ( posicao_menu_principal > 0 )
              posicao_menu_principal--;
            else
              posicao_menu_principal = QTD_MENU_PRINCIPAL;
  
            // Volta para mostrar nova selecao.
            estado_operacao = MENU_PRINCIPAL;    
            break;
          // Se botao seleciona pressionado.  
          case SELECIONA:
            // Verifica qual opcao foi selecionada.
            switch(posicao_menu_principal)
            {        
               case MODELO: 
                  Serial.println("SELECINOU MODELO");
                  display_lcd.limpa_tela();
                  display_lcd.escreve("INICIANDO", CENTRALIZADO, 0, 1);
                  display_lcd.escreve("MODELO...", CENTRALIZADO, 0, 2);
                  // Seta o tempo de espera para 2 segundos.
                  seta_tempo_espera(2,0);
                  // Seta em operacao como ativo.
                  em_operacao = ATIVO;
                  // Continua para inicializacao do processo "MODELO".
                  estado_operacao = FUNCAO_MODELO;
                  break; 
               case TEMPORARIO:
                  Serial.println("SELECINOU TEMPORARIO");
                  display_lcd.limpa_tela();
                  display_lcd.escreve("INICIANDO", CENTRALIZADO, 0, 1);
                  display_lcd.escreve("TEMPORARIO...", CENTRALIZADO, 0, 2);
                  // Seta o tempo de espera para 2 segundos.
                  seta_tempo_espera(2,0);
                  // Seta em operacao como ativo.
                  em_operacao = ATIVO;
                  // Continua para inicializacao do processo "TEMPORARIO".
                  estado_operacao = FUNCAO_TEMPORARIO;
                  break;  
               case FUNDICAO:
                  Serial.println("SELECINOU FUNDICAO");
                  display_lcd.limpa_tela();
                  display_lcd.escreve("INICIANDO", CENTRALIZADO, 0, 1);
                  display_lcd.escreve("FUNDICAO...", CENTRALIZADO, 0, 2);
                  // Seta o tempo de espera para 2 segundos.
                  seta_tempo_espera(2,0);
                  // Seta em operacao como ativo.
                  em_operacao = ATIVO;
                  // Continua para inicializacao do processo "TEMPORARIO".
                  estado_operacao = FUNCAO_FUNDICAO;
                  break; 
                case BITE_SPLINT:
                  Serial.println("SELECINOU BITE_SPLINT");
                  display_lcd.limpa_tela();
                  display_lcd.escreve("INICIANDO", CENTRALIZADO, 0, 1);
                  display_lcd.escreve("BITE_SPLINT...", CENTRALIZADO, 0, 2);
                  // Seta o tempo de espera para 2 segundos.
                  seta_tempo_espera(2,0);
                  // Seta em operacao como ativo.
                  em_operacao = ATIVO;
                  // Continua para inicializacao do processo "TEMPORARIO".
                  estado_operacao = FUNCAO_BITESPLINT;
                  break;  
                case GUIA_CIRURGICO:
                  Serial.println("SELECINOU GUIA_CIRURGICO");
                  display_lcd.limpa_tela();
                  display_lcd.escreve("INICIANDO", CENTRALIZADO, 0, 1);
                  display_lcd.escreve("GUIA_CIRURGICO...", CENTRALIZADO, 0, 2);
                  // Seta o tempo de espera para 2 segundos.
                  seta_tempo_espera(2,0);
                  // Seta em operacao como ativo.
                  em_operacao = ATIVO;
                  // Continua para inicializacao do processo "TEMPORARIO".
                  estado_operacao = FUNCAO_GUIA_CIRURGICO;
                  break;   
            } 
            break;  
        }
      }
      break;
   // Inicio do processo "MODELO".   
   case FUNCAO_MODELO: 
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {    
        // Chama funcão limpar, para acionar os componentes.
        limpar();
        // Reinicia display lcd.
        display_lcd.begin(LINHAS, COLUNAS); 
        delay(100);
        // Mostra mensagem na tela.
        display_lcd.limpa_tela();
        // Mostra mensagem de limpando.
        display_lcd.escreve("LIMPANDO", CENTRALIZADO, 0, 0);
        // Seta o tempo de espera para 2 segundos.
        seta_tempo_espera(TEMPO_LIMPAR_MODELO,1);
        // Continua para proximo estado.
        estado_operacao = AGUARDA_LIMPEZA;
      }
      break;
   // Aguarda limpeza do processo e inicia processo de curar. 
   case AGUARDA_LIMPEZA:
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {     
        // Desliga os reles.
        desliga_motores();
        delay(100);
        // Chama funcão curar, para acionar os componentes.
        curar();
        // Reinicia display lcd.
        display_lcd.begin(LINHAS, COLUNAS); 
        delay(100);
        // Mostra mensagem na tela.
        display_lcd.limpa_tela();
        // Mostra mensagem de curando.
        display_lcd.escreve("CURANDO", CENTRALIZADO, 0, 0);        
        // Seta o tempo de espera para 2 segundos.
        seta_tempo_espera(TEMPO_CURAR_MODELO,1);
        // Continua para proximo estado.
        estado_operacao = AGUARDA_CURAR;
      }
      break;
   // Aguarda curar do processo e inicia processo de curar. 
   case AGUARDA_CURAR:
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {     
        // Desliga todos motores.
        desliga_motores();
        // Reinicia display lcd.
        display_lcd.begin(LINHAS, COLUNAS); 
        delay(100);
        // Seta o tempo de espera para 2 segundos.
        seta_tempo_espera(4,0);
        // Mostra mensagem de finalizado.
        display_lcd.limpa_tela();
        display_lcd.escreve("MODELO", CENTRALIZADO, 0, 1);
        display_lcd.escreve("FINALIZADO", CENTRALIZADO, 0, 2);
        // Continua para proximo estado.
        estado_operacao = FINALIZA_OPERACAO;
      }
      break; 
   // Aguarda tempo de visualizacao da mensagem e retorna ao menu principal.
   case FINALIZA_OPERACAO:
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {        
        // Volta para menu principal.
        estado_operacao = MENU_PRINCIPAL;
      }
      break;
   // Inicio do processo "TEMPORARIO".   
   case FUNCAO_TEMPORARIO: 
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {    
        // Chama funcão limpar, para acionar os componentes.
        limpar();
        // Reinicia display lcd.
        display_lcd.begin(LINHAS, COLUNAS); 
        delay(100);
        // Mostra mensagem na tela.
        display_lcd.limpa_tela();
        // Mostra mensagem de limpando.
        display_lcd.escreve("LIMPANDO", CENTRALIZADO, 0, 0);
        // Seta o tempo de espera para 2 segundos.
        seta_tempo_espera(TEMPO_LIMPAR_TEMPORARIO,1);
        // Continua para proximo estado.
        estado_operacao = AGUARDA_LIMPEZA_TEMPORARIO;
      }
      break;
   // Aguarda limpeza do processo e inicia processo de curar. 
   case AGUARDA_LIMPEZA_TEMPORARIO:
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {     
        // Desliga os reles.
        desliga_motores();
        delay(100);
        // Chama funcão curar, para acionar os componentes.
        curar();
        // Reinicia display lcd.
        display_lcd.begin(LINHAS, COLUNAS); 
        delay(100);
        // Mostra mensagem na tela.
        display_lcd.limpa_tela();
        // Mostra mensagem de curando.
        display_lcd.escreve("CURANDO", CENTRALIZADO, 0, 0);        
        // Seta o tempo de espera para 2 segundos.
        seta_tempo_espera(TEMPO_CURAR_TEMPORARIO,1);
        // Continua para proximo estado.
        estado_operacao = AGUARDA_CURAR_TEMPORARIO;
      }
      break;
   // Aguarda curar do processo e inicia processo de curar. 
   case AGUARDA_CURAR_TEMPORARIO:
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {     
        // Desliga todos motores.
        desliga_motores();
        // Reinicia display lcd.
        display_lcd.begin(LINHAS, COLUNAS); 
        delay(100);
        // Seta o tempo de espera para 2 segundos.
        seta_tempo_espera(4,0);
        // Mostra mensagem de finalizado.
        display_lcd.limpa_tela();
        display_lcd.escreve("TEMPORARIO", CENTRALIZADO, 0, 1);
        display_lcd.escreve("FINALIZADO", CENTRALIZADO, 0, 2);
        // Continua para proximo estado.
        estado_operacao = FINALIZA_OPERACAO_TEMPORARIO;
      }
      break; 
   // Aguarda tempo de visualizacao da mensagem e retorna ao menu principal.
   case FINALIZA_OPERACAO_TEMPORARIO:
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {        
        // Volta para menu principal.
        estado_operacao = MENU_PRINCIPAL;
      }
      break;  
   // Inicio do processo "FUNDICAO".   
   case FUNCAO_FUNDICAO: 
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {    
        // Chama funcão limpar, para acionar os componentes.
        limpar();
        // Reinicia display lcd.
        display_lcd.begin(LINHAS, COLUNAS); 
        delay(100);
        // Mostra mensagem na tela.
        display_lcd.limpa_tela();
        // Mostra mensagem de limpando.
        display_lcd.escreve("LIMPANDO", CENTRALIZADO, 0, 0);
        // Seta o tempo de espera para 2 segundos.
        seta_tempo_espera(TEMPO_LIMPAR_FUNDICAO,1);
        // Continua para proximo estado.
        estado_operacao = AGUARDA_LIMPEZA_FUNDICAO;
      }
      break;
   // Aguarda limpeza do processo e inicia processo de curar. 
   case AGUARDA_LIMPEZA_FUNDICAO:
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {     
        // Desliga os reles.
        desliga_motores();
        delay(100);
        // Chama funcão curar, para acionar os componentes.
        curar();
        // Reinicia display lcd.
        display_lcd.begin(LINHAS, COLUNAS); 
        delay(100);
        // Mostra mensagem na tela.
        display_lcd.limpa_tela();
        // Mostra mensagem de curando.
        display_lcd.escreve("CURANDO", CENTRALIZADO, 0, 0);        
        // Seta o tempo de espera para 2 segundos.
        seta_tempo_espera(TEMPO_CURAR_FUNDICAO,1);
        // Continua para proximo estado.
        estado_operacao = AGUARDA_CURAR_FUNDICAO;
      }
      break;
   // Aguarda curar do processo e inicia processo de curar. 
   case AGUARDA_CURAR_FUNDICAO:
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {     
        // Desliga todos motores.
        desliga_motores();
        // Reinicia display lcd.
        display_lcd.begin(LINHAS, COLUNAS); 
        delay(100);
        // Seta o tempo de espera para 2 segundos.
        seta_tempo_espera(4,0);
        // Mostra mensagem de finalizado.
        display_lcd.limpa_tela();
        display_lcd.escreve("FUNDICAO", CENTRALIZADO, 0, 1);
        display_lcd.escreve("FINALIZADO", CENTRALIZADO, 0, 2);
        // Continua para proximo estado.
        estado_operacao = FINALIZA_OPERACAO_FUNDICAO;
      }
      break; 
   // Aguarda tempo de visualizacao da mensagem e retorna ao menu principal.
   case FINALIZA_OPERACAO_FUNDICAO:
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {        
        // Volta para menu principal.
        estado_operacao = MENU_PRINCIPAL;
      }
      break;  
   // Inicio do processo "BITESPLINT".   
   case FUNCAO_BITESPLINT: 
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {    
        // Chama funcão limpar, para acionar os componentes.
        limpar();
        // Reinicia display lcd.
        display_lcd.begin(LINHAS, COLUNAS); 
        delay(100);
        // Mostra mensagem na tela.
        display_lcd.limpa_tela();
        // Mostra mensagem de limpando.
        display_lcd.escreve("LIMPANDO", CENTRALIZADO, 0, 0);
        // Seta o tempo de espera para 2 segundos.
        seta_tempo_espera(TEMPO_LIMPAR_BITESPLINT,1);
        // Continua para proximo estado.
        estado_operacao = AGUARDA_LIMPEZA_BITESPLINT;
      }
      break;
   // Aguarda limpeza do processo e inicia processo de curar. 
   case AGUARDA_LIMPEZA_BITESPLINT:
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {     
        // Desliga os reles.
        desliga_motores();
        delay(100);
        // Chama funcão curar, para acionar os componentes.
        curar();
        // Reinicia display lcd.
        display_lcd.begin(LINHAS, COLUNAS); 
        delay(100);
        // Mostra mensagem na tela.
        display_lcd.limpa_tela();
        // Mostra mensagem de curando.
        display_lcd.escreve("CURANDO", CENTRALIZADO, 0, 0);        
        // Seta o tempo de espera para 2 segundos.
        seta_tempo_espera(TEMPO_CURAR_BITESPLINT,1);
        // Continua para proximo estado.
        estado_operacao = AGUARDA_CURAR_BITESPLINT;
      }
      break;
   // Aguarda curar do processo e inicia processo de curar. 
   case AGUARDA_CURAR_BITESPLINT:
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {     
        // Desliga todos motores.
        desliga_motores();
        // Reinicia display lcd.
        display_lcd.begin(LINHAS, COLUNAS); 
        delay(100);
        // Seta o tempo de espera para 2 segundos.
        seta_tempo_espera(4,0);
        // Mostra mensagem de finalizado.
        display_lcd.limpa_tela();
        display_lcd.escreve("BITESPLINT", CENTRALIZADO, 0, 1);
        display_lcd.escreve("FINALIZADO", CENTRALIZADO, 0, 2);
        // Continua para proximo estado.
        estado_operacao = FINALIZA_OPERACAO_BITESPLINT;
      }
      break; 
   // Aguarda tempo de visualizacao da mensagem e retorna ao menu principal.
   case FINALIZA_OPERACAO_BITESPLINT:
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {        
        // Volta para menu principal.
        estado_operacao = MENU_PRINCIPAL;
      }
      break;    
   // Inicio do processo "GUIA_CIRURGICO".   
   case FUNCAO_GUIA_CIRURGICO: 
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {    
        // Chama funcão limpar, para acionar os componentes.
        limpar();
        // Reinicia display lcd.
        display_lcd.begin(LINHAS, COLUNAS); 
        delay(100);
        // Mostra mensagem na tela.
        display_lcd.limpa_tela();
        // Mostra mensagem de limpando.
        display_lcd.escreve("LIMPANDO", CENTRALIZADO, 0, 0);
        // Seta o tempo de espera para 2 segundos.
        seta_tempo_espera(TEMPO_LIMPAR_GUIA_CIRURGICO,1);
        // Continua para proximo estado.
        estado_operacao = AGUARDA_LIMPEZA_GUIA_CIRURGICO;
      }
      break;
   // Aguarda limpeza do processo e inicia processo de curar. 
   case AGUARDA_LIMPEZA_GUIA_CIRURGICO:
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {     
        // Desliga os reles.
        desliga_motores();
        delay(100);
        // Chama funcão curar, para acionar os componentes.
        curar();
        // Reinicia display lcd.
        display_lcd.begin(LINHAS, COLUNAS); 
        delay(100);
        // Mostra mensagem na tela.
        display_lcd.limpa_tela();
        // Mostra mensagem de curando.
        display_lcd.escreve("CURANDO", CENTRALIZADO, 0, 0);        
        // Seta o tempo de espera para 2 segundos.
        seta_tempo_espera(TEMPO_CURAR_GUIA_CIRURGICO,1);
        // Continua para proximo estado.
        estado_operacao = AGUARDA_CURAR_GUIA_CIRURGICO;
      }
      break;
   // Aguarda curar do processo e inicia processo de curar. 
   case AGUARDA_CURAR_GUIA_CIRURGICO:
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {     
        // Desliga todos motores.
        desliga_motores();
        // Reinicia display lcd.
        display_lcd.begin(LINHAS, COLUNAS); 
        delay(100);
        // Seta o tempo de espera para 2 segundos.
        seta_tempo_espera(4,0);
        // Mostra mensagem de finalizado.
        display_lcd.limpa_tela();
        display_lcd.escreve("GUIA CIRURGICO", CENTRALIZADO, 0, 1);
        display_lcd.escreve("FINALIZADO", CENTRALIZADO, 0, 2);
        // Continua para proximo estado.
        estado_operacao = FINALIZA_OPERACAO_GUIA_CIRURGICO;
      }
      break; 
   // Aguarda tempo de visualizacao da mensagem e retorna ao menu principal.
   case FINALIZA_OPERACAO_GUIA_CIRURGICO:
      // Aguarda passar o tempo determinado.
      if ( aguarda_tempo_espera() )
      {        
        // Volta para menu principal.
        estado_operacao = MENU_PRINCIPAL;
      }
      break;    
  }
}

/*********************************************************************************************************
** Nome da Função:       seta_tempo_espera
** Descrição:            Seta tempo de espera em segundos.
** Parametro:            tempo = tempo em segundos de espera.
** Valor de retorno:     Não.
*********************************************************************************************************/
void seta_tempo_espera(long tempo, int contador_atv)
{  
  // Seta variavel global contador.
  contador_ativo = contador_atv;
  // Inicia temporizador.
  tempo_inicio_operacao = millis();  
  // Seta o tempo de espera.
  tempo_espera = tempo*1000;  
  Serial.println("TEMPO ESPERA: " + (String)tempo_espera);  
  // Verifica se o contador esta ativo.
  if ( contador_ativo )
    // Seta parametros para contagem regressiva
    seta_contador(); 
}

/*********************************************************************************************************
** Nome da Função:       aguarda_tempo_espera
** Descrição:            Aguarda tempo de espera em segundos.
** Parametro:            Não
** Valor de retorno:     1 = sucesso, 0 - aguardar.
*********************************************************************************************************/
int aguarda_tempo_espera()
{
  // Verifica se o contador esta ativo.
  if ( contador_ativo )
    // Faz a contagem regressiva em segundos e mostra na tela.
    contador_regressivo();
  // Aguarda o tempo para informar a função.
  tempo_atual_operacao = millis();
  if((tempo_atual_operacao-tempo_inicio_operacao) > tempo_espera )
  { 
    return 1;
  }else return 0;
}

/*********************************************************************************************************
** Nome da Função:       seta_contador
** Descrição:            Seta parametros para contagem regressiva em segundos e mostra na tela.
** Parametro:            Não
** Valor de retorno:     Não
*********************************************************************************************************/
void seta_contador()
{
  // Verifica se o tempo é maior que 10s
    if ( ((tempo_espera/1000)-contador_segundos) >= 60  )
    {
      // Verifica se o tempo é maior que 10s
      if ( ((tempo_espera/1000)/60)-(contador_segundos/60) >= 10  )
        // Mostra na tela contador regressivo.
        display_lcd.escreve((String)(((tempo_espera/1000)/60)-(contador_segundos/60)) + "m", NORMAL, 17, 0);
      else  
        // Mostra na tela contador regressivo.
        display_lcd.escreve(" " + (String)(((tempo_espera/1000)/60)-(contador_segundos/60)) + "m", NORMAL, 17, 0); 
  }else
  {
    // Verifica se o tempo é maior que 10s
    if ( (tempo_espera/1000) >= 10  )
      // Mostra na tela contador regressivo.
      display_lcd.escreve((String)(tempo_espera/1000) + "s", NORMAL, 17, 0);
    else  
    // Mostra na tela contador regressivo.
      display_lcd.escreve(" " + (String)(tempo_espera/1000) + "s", NORMAL, 17, 0); 
  }

  // Entrar
  display_lcd.escreve("  [          ]    0%", NORMAL, 0, 2);     
  // Inicia temporizador que conta minutos.
  tempo_inicio_contador_segundos = millis();
  // Contador segundos.
  contador_segundos = 1;
  // Seta contador da barra progressiva.
  contador_barra_progressiva = 3;
  // Contador porcentagem.
  contador_porcentagem = 0;
  // Contador 10 porcento.
  contador_dez_porcento = 0;
}

/*********************************************************************************************************
** Nome da Função:       contador_regressivo
** Descrição:            Faz a contagem regressiva em segundos e mostra na tela.
** Parametro:            Não
** Valor de retorno:     Não
*********************************************************************************************************/
void contador_regressivo()
{
  // Mostra segundos restantes na tela.
  tempo_atual_contador_porcentagem = millis();
  if((tempo_atual_contador_porcentagem-tempo_inicio_contador_porcentagem) > (tempo_espera/100) )
  {   
    // Incrementa contador.
    contador_porcentagem++;
    if ( contador_porcentagem < 10 )
      // Imprime um quadrado na barra progressiva.
      display_lcd.escreve((String)contador_porcentagem + "%", NORMAL, 18, 2);   
    else
      // Imprime um quadrado na barra progressiva.
     display_lcd.escreve((String)contador_porcentagem + "%", NORMAL, 17, 2);   
    // Contador 10 %.
    contador_dez_porcento++;
    if ( contador_dez_porcento == 10 )
    {
      // Imprime um quadrado na barra progressiva.
      display_lcd.escreve_char_especial(8, contador_barra_progressiva, 2); 
      // Reseta contador.
      contador_dez_porcento = 0;      
      // Incrementa contador.
      contador_barra_progressiva++;
    }
    // Reinicia temporizador.
    tempo_inicio_contador_porcentagem = millis();
  }
  // Mostra segundos restantes na tela.
  tempo_atual_buzzer = millis();
  if((tempo_atual_buzzer-tempo_inicio_buzzer) > 100 && ((tempo_espera/1000)-contador_segundos) == 0 )
  {
    // Liga o buzzer.
    buzzer.liga();
    // Mostra segundos restantes na tela.
    tempo_inicio_buzzer = millis();  
  }

  // Mostra segundos restantes na tela.
  tempo_atual_buzzer = millis();
  if((tempo_atual_buzzer-tempo_inicio_buzzer) > 300 && ((tempo_espera/1000)-contador_segundos) == 1 )
  {
    // Liga o buzzer.
    buzzer.liga();
    // Mostra segundos restantes na tela.
    tempo_inicio_buzzer = millis();  
  }

  // Mostra segundos restantes na tela.
  tempo_atual_buzzer = millis();
  if((tempo_atual_buzzer-tempo_inicio_buzzer) > 400 && ((tempo_espera/1000)-contador_segundos) == 2 )
  {
    // Liga o buzzer.
    buzzer.liga();
    // Mostra segundos restantes na tela.
    tempo_inicio_buzzer = millis();  
  }
  // Mostra segundos restantes na tela.
  tempo_atual_buzzer = millis();
  if((tempo_atual_buzzer-tempo_inicio_buzzer) > 500 && ((tempo_espera/1000)-contador_segundos) == 3 )
  {
    // Liga o buzzer.
    buzzer.liga();
    // Mostra segundos restantes na tela.
    tempo_inicio_buzzer = millis();  
  }
  // Mostra segundos restantes na tela.
  tempo_atual_buzzer = millis();
  if((tempo_atual_buzzer-tempo_inicio_buzzer) > 600 && ((tempo_espera/1000)-contador_segundos) == 4 )
  {
    // Liga o buzzer.
    buzzer.liga();
    // Mostra segundos restantes na tela.
    tempo_inicio_buzzer = millis();  
  }
  
  // Mostra segundos restantes na tela.
  tempo_atual_contador_segundos = millis();
  if((tempo_atual_contador_segundos-tempo_inicio_contador_segundos) > 1000 )
  {
    // Verifica se o tempo é maior que 10s
    if ( ((tempo_espera/1000)-contador_segundos) >= 60  )
    {
      // Verifica se o tempo é maior que 10s
      if ( ((tempo_espera/1000)/60)-(contador_segundos/60) >= 10  )
        // Mostra na tela contador regressivo.
        display_lcd.escreve((String)(((tempo_espera/1000)/60)-(contador_segundos/60)) + "m", NORMAL, 17, 0);
      else  
        // Mostra na tela contador regressivo.
        display_lcd.escreve(" " + (String)(((tempo_espera/1000)/60)-(contador_segundos/60)) + "m", NORMAL, 17, 0); 
    }else
    {
      // Verifica se o tempo é maior que 10s
      if ( ((tempo_espera/1000)-contador_segundos) >= 10  )
        // Mostra na tela contador regressivo.
        display_lcd.escreve((String)((tempo_espera/1000)-contador_segundos) + "s", NORMAL, 17, 0);
      else  
        // Mostra na tela contador regressivo.
        display_lcd.escreve(" " + (String)((tempo_espera/1000)-contador_segundos) + "s", NORMAL, 17, 0);  
    }
    
    if ( ((tempo_espera/1000)-contador_segundos) == 5 )
      // Liga o buzzer.
      buzzer.liga();
      
    // Incrementa contador segundos.
    contador_segundos++;
    // Reinicia temporizador.
    tempo_inicio_contador_segundos = millis();
  }
}

/*********************************************************************************************************
** Nome da Função:       limpar
** Descrição:            Ativa os dispositivos necessarios para realizar a limpeza.
** Parametro:            Não
** Valor de retorno:     Não.
*********************************************************************************************************/
void limpar()
{
  // Liga o pino IN1
  digitalWrite(IN1, LOW);
}

/*********************************************************************************************************
** Nome da Função:       curar
** Descrição:            Ativa os dispositivos necessarios para realizar a cura.
** Parametro:            Não
** Valor de retorno:     Não.
*********************************************************************************************************/
void curar()
{
  // Liga o pino IN2
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
}

/*********************************************************************************************************
** Nome da Função:       desliga_motores
** Descrição:            Desliga todos motores de operação.
** Parametro:            Não
** Valor de retorno:     Não.
*********************************************************************************************************/
void desliga_motores()
{
  // Desliga o motor do tanque.
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
}

/*********************************************************************************************************
** Nome da Função:       menu_principal
** Descrição:            Menu principal de servico, onde contem todos menus.
** Parametro:            Não
** Valor de retorno:     Não.
*********************************************************************************************************/
void menu_principal()
{
  switch(posicao_menu_principal)
  {        
     case MODELO:
        display_lcd.limpa_tela();
        display_lcd.escreve("PROGRAMA: ", CENTRALIZADO, 0, 0);
        display_lcd.escreve_char_especial(4, 0, 2);   
        display_lcd.escreve("MODELO", NORMAL, 2, 2); 
        break; 
     case TEMPORARIO:
        display_lcd.limpa_tela();
        display_lcd.escreve("PROGRAMA: ", CENTRALIZADO, 0, 0);
        display_lcd.escreve_char_especial(4, 0, 2);   
        display_lcd.escreve("TEMPORARIO", NORMAL, 2, 2); 
        break;  
     case FUNDICAO:
        display_lcd.limpa_tela();
        display_lcd.escreve("PROGRAMA: ", CENTRALIZADO, 0, 0);
        display_lcd.escreve_char_especial(4, 0, 2);   
        display_lcd.escreve("FUNDICAO", NORMAL, 2, 2); 
        break; 
      case BITE_SPLINT:
        display_lcd.limpa_tela();
        display_lcd.escreve("PROGRAMA: ", CENTRALIZADO, 0, 0);
        display_lcd.escreve_char_especial(4, 0, 2);   
        display_lcd.escreve("BITE SPLINT", NORMAL, 2, 2); 
        break;  
      case GUIA_CIRURGICO:
        display_lcd.limpa_tela();
        display_lcd.escreve("PROGRAMA: ", CENTRALIZADO, 0, 0);
        display_lcd.escreve_char_especial(4, 0, 2);   
        display_lcd.escreve("GUIA CIRURGICO", NORMAL, 2, 2); 
        break;   
  }   
}

/*********************************************************************************************************
** Nome da Função:       cancela_operacao
** Descrição:            Cancela a operacao
** Parametro:            Não
** Valor de retorno:     Não
*********************************************************************************************************/
void cancela_operacao()
{
  // Se em operacao.
  if ( em_operacao )
  {
    // Pega se algum botao foi pressionado.
    selecao = bt_press();

    if ( selecao != 0 )
    {
      // De acordo com o botao pressionado movimenta o menu.     
      switch(selecao)
      {      
        // Se botao voltar pressionado.  
        case VOLTAR:
          // Seta em operacao como inativo.
          em_operacao = INATIVO;
          display_lcd.limpa_tela();
          display_lcd.escreve("FINALIZANDO ", CENTRALIZADO, 0, 1);
          display_lcd.escreve("OPERACAO, AGUARDE...", CENTRALIZADO, 0, 2);          
          // Desliga todos motores.
          desliga_motores();
          // Aguarda 1,5 segundos.
          delay(1500);
          // Volta para menu principal.
          estado_operacao = MENU_PRINCIPAL;
          break; 
      }
    }
  }
}

/*********************************************************************************************************
** Nome da Função:       bt_press
** Descrição:            Funcao para verificar se algum botao foi pressionado.
** Parametro:            Não
** Valor de retorno:     1 - Avancar. 2 - Voltar. 3 - SELECIONA.
*********************************************************************************************************/
int bt_press()
{
  // A cada 10s faz o lcd reiniciar para evitar bugs.
  tempo_atual_bt = millis();
  if((tempo_atual_bt-tempo_inicio_bt) > 300)
  {
    // Se botao avancar pressionado.
    if(digitalRead(BT_AVANCA))
    {
      // Inicia contador para contagem dos botoes.
      tempo_inicio_bt = millis();
      // Liga o buzzer.
      buzzer.liga();
      return AVANCAR;
    }
    // Se botao voltar pressionado.
    if(digitalRead(BT_VOLTA))
    {
      // Inicia contador para contagem dos botoes.
      tempo_inicio_bt = millis();
      // Liga o buzzer.
      buzzer.liga();
      return VOLTAR;
    }
    // Se botao seleciona pressionado.
    if(digitalRead(BT_SELECIONA))
    {
      // Inicia contador para contagem dos botoes.
      tempo_inicio_bt = millis();
      // Liga o buzzer.
      buzzer.liga();
      return SELECIONA;
    }
  }
  return 0;
}

        
