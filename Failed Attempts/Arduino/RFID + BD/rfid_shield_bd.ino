//bibliotecas 
#include <Ethernet.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 7
#define RST_PIN 9
char st[20];
MFRC522 mfrc522(SS_PIN, RST_PIN);


String leitura;
//float leituraconvertida

char sentenca[128];
char valortag[10];

//shield
byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server_addr(192,168,0,36);

//login bd
char user[] = "arduino";              
char password[] = "arduino";


char INSERIR_TEMP[] = "INSERT INTO registrotemp (tag) VALUES (%s)";
char BANCODEDADOS[] = "USE banco_arduino";
EthernetClient client;
MySQL_Connection conn((Client *)&client);
void setup() 
{ 
   Serial.begin(115200);
   SPI.begin();
   mfrc522.PCD_Init();
   Serial.println("Aproxime o seu cartao/TAG do leitor");
   Serial.println();
   
   while (!Serial); 
   Ethernet.begin(mac_addr);
   Serial.println("Conectando...");
   if (conn.connect(server_addr, 3306, user, password)) 
   {
      delay(1000);
      
      MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
      cur_mem->execute(BANCODEDADOS);
      delete cur_mem;
   }
   else
   {
      Serial.println("A conexão falhou");
      conn.close();
   }
}
void loop() 
{
   Serial.println("Executando sentença");
   // Busca novos cartões 
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Seleciona um catão a ser lido
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Mostra ID na serial
  Serial.print("ID da tag:");
  String conteudo = "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
   
  leitura = conteudo; 
   
   //leitura = analogRead(LM35);
   //leituraconvertida = (float(analogRead(LM35))*5/(1023))/0.01;
   
   dtostrf(leitura, 11, 0, valortag);
   sprintf(sentenca, INSERIR_TEMP, valortag);
   
   MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
   cur_mem->execute(sentenca);
   delete cur_mem;
   delay(2000);
}
