/*
-------- CONTROLE DE ACESSO VIA RFID ------------
@tobraga
*/

//Importando as bibliotecas necessárias
#include <SPI.h> //Comunicação SPI
#include <MFRC522.h> //leitor RFID

//Definindo as constantes
#define SS_PIN 10
#define RST_PIN 9

//Definindo os pinos que serão utilizados para ligação do display LCD
String IDtag = ""; //Variável que armazenará o ID da tag

//Vetor responsável por armazenar os IDs das tags cadastradas
String TagsCadastradas[] = {"9d8c4255"}; //ID da tag em formato de cartão
MFRC522 LeitorRFID(SS_PIN, RST_PIN); // Cria uma nova instância para o leitor e passa os pinos como parâmetro

void setup() {
  Serial.begin(9600); // Inicializa a comunicação Serial
  Serial.println("Aproxime seu cartao do leitor...");
  Serial.println();
}

void loop() {
  Leitura(); //Chama a função responsável por fazer a leitura das Tag's
}

void Leitura(){
  SPI.begin(); // Inicializa comunicacao SPI
  LeitorRFID.PCD_Init(); // Inicializa o leitor RFID
  
  
  IDtag = ""; //Inicialmente IDtag deve estar vazia

  // Verifica se existe uma tag presente
  if ( !LeitorRFID.PICC_IsNewCardPresent() || !LeitorRFID.PICC_ReadCardSerial() ) {
    delay(50);
    return;
  }
 
  // Pega o ID da tag através da função LeitorRFID.uid e Armazena o ID na variável IDtag
  for (byte i = 0; i < LeitorRFID.uid.size; i++) {
    IDtag.concat(String(LeitorRFID.uid.uidByte[i], HEX));
  }

  //Compara o valor do ID lido com os IDs armazenados no vetor TagsCadastradas[]
  for (int i = 0; i < (sizeof(TagsCadastradas)/sizeof(String)); i++) {
    if( IDtag.equalsIgnoreCase(TagsCadastradas[i]) ){
      Permitido = true; //Variável Permitido assume valor verdadeiro caso o ID Lido esteja cadastrado
    }
  }

 }//fim Leitura()
