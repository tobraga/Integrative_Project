//BIBLIOTECAS

//COMUNICAÇÃO ENTRE ARDUINO E A REDE - SHIELD ETHERNET WIZNET W5100
#include <Ethernet.h>

//PROMOVE A CONEXÃO DO ARDUINO UNO COM O SERVIDOR MYSQL E 
//EXECUTA SENTENÇAS DE MANIPULAÇÃO DE BANCO DE DADOS
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

//COORDENA A INTEGRAÇÃO DO SHIELD COM O ARDUINO UNO POR MEIO DO PROTOCOLO DE COMUNICAÇÃO SPI
#include <SPI.h>


//ENTRADA ANALOGICA DO LM
#define LM35 A0
//ARMAZENO O VALOR OBTIDO
int leitura;
//CONVERTE O VLOR OBTIDO
float leituraconvertida

//VETOR PARA CONTER FRASES PARA CONSTRUÇÃO DAS SENTENÇAS P MANIPULAR OS DADOS NO BD
char sentenca[128];
char valortemp[10];

//ENDEREÇO MAC DO ARDUINO UNO
byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//CONFIGURAÇÕES DE REDE
IPAddress server_addr(192,168,25,5);

//CONEXÃO DO ARDUINO COM O SERVIDOR
char user[] = "root";              
char password[] = "";     

//SENTENÇAS DE MANIPULAÇÃO - REGISTRO DE INFORMAÇÕES DO ARDUINO PARA O BD
//NOME DO BANCO: banco_arduino
//NOME DA TABELA: registro

//PREENCHE UMA LINHA DE UMA DETERMINADA TABELA EXISTENTE NO BD
char INSERIR_TEMP[] = "INSERT INTO registrotemp (temperatura) VALUES (%s)";
/*
 * declaramos um vetor de variáveis do tipo char, sem tamanho definido, para que 
 * este armazene a sentença citada anteriormente, de modo que, quando necessário, 
 * o Arduino UNO seja capaz de utilizá-la.
 */
char BANCODEDADOS[] = "USE banco_arduino";

//CRIAÇÃO DOS OBJETOS
//CLIENT: RESPONSAVEL PELA COMUNICAÇÃO VIA ETHERNET NO CODIGO
EthernetClient client;
//CONN: RESPONSAVEL PELA CONEXAO COM O SERVIDOR MYSQL
MySQL_Connection conn((Client *)&client);
    
    
void setup(){ 
  //INICIA COMUNICAÇÃO SERIAL
  Serial.begin(115200);//NUMERO DEVIDO AO LM35
  //CONTINUA EXECUTANDO APOS A CONEXAO SERIAL SER ESTABELECIDA
  while (!Serial);
  //INICIA COMUNICAÇÃO COM O REDE VIA ETHERNET 
  Ethernet.begin(mac_addr);
  //IMPRESSSAO NA PORTA SERIAL
  Serial.println("Conectando...");

  //SAO PASSADOS O ENDEREÇO IP DA MAQUINA, PORTA PARA ACESSO, USUARIO E SENHA
  //SE FOR EXECUTADA CORRETAMENTE -> CONTEUDO SERÁ EXECUTADO
  if (conn.connect(server_addr, 3306, user, password)){
    //ESTABELECE UM ATRASO NA EXECUÇÃO, GARANTINDO A CONEXAO COM O SERVIDOR
    delay(1000);

    //ESSE OBJETO "cur_men"  DO TIPO "MYSQL_CURSOR" - EXECUTA AS SENTENÇAS DE MANIPULAÇÃO DE
    //INFORMAÇÕES NO BD
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

    //FUNÇÃO QUE EXECUTA A VARIAVEL "BANCODEDADOS" RESPONSAVEL PELO "USE BANCO_ARDUINO"
    cur_mem->execute(BANCODEDADOS);
    
    //RECOMENDAÇAO DA BIBLIOTECA LM - P APAGAR A SENTENÇA DE MODA A EVITAR O USO DE MEMORIA
    delete cur_mem;
  }//FIM DO IF
  else{//CASO O IF NAO SEJA SASTIFEITO
    Serial.println("A conexão falhou");
    conn.close();//ENCERRA A TENTATIVA DE CONEXAO
  }//FIM DO ELSE
}//FIM DO SETUP


void loop(){

  //AVIDO DE MANIPULAÇAO NO BANCO DE DADOS SENDO EXECUTADA
  Serial.println("Executando sentença");
  //AQUI QUE VAI ENTRAR A LEITURA DO RFID
  //LEITURA DA PORTA DE ENTRADA ANALOGICA - NA QUAL O SENSOR ESTA CONECTADO 
  leitura = analogRead(LM35);
  //CONVERSAO DA TEMPERATUA
  leituraconvertida = (float(analogRead(LM35))*5/(1023))/0.01;
  
  //CONVERSAO DO FLOAT PARA CHAR
  dtostrf(leituraconvertida, 4, 1, valortemp);

  //CONSTRUIR A SENTENÇA CONTENDO A INSTRUÇAO QUE SERÁ UTILIZADA NA MANIPULAÇAO DOS BD
  //
  sprintf(sentenca, INSERIR_TEMP, valortemp);

  //INCLUSAO  DE UMA LINHA NA TABELA
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(sentenca);
  delete cur_mem;
  delay(2000);
  
}//FIM DO LOOP
