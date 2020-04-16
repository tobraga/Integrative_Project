//O que é necessario?
//Para utilizar o RFID na plataforma do Arduino, é preciso baixar a biblioteca MFRC522

//FUNCIONAMENTO
// Cada etiqueta/tag do Leitor RFID tem a sua propria identificação (UID). Essa identificação que será usada 
//para o controle de acesso

//COMO OCOCRRE
//O RFID lê a tag em um formato da UID de XX YY ZZ WW, como por exemplo: ED 78 03 CA

//PROGRAMA

// RFID com LEITOR LCD -- Comentado as partes do LCD

//Autor: Max Braga
//Projeto Integrador

#include <SPI.h> //PROTOCOLO SPI
#include <MFRC522.h> // ATUA SOBRE A SPI, SIMPLIFICANDO O PROCESSO DE COMUNICAÇÂO
//#include <LiquidCrystal.h>

//PINOS DE ENTRADA E SAIDA
#define SS_PIN 10
#define RST_PIN 9

//DECLARANDO OBJETO QUE REPRESENTARÁ O RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 
//LiquidCrystal lcd(6, 7, 5, 4, 3, 2); 
 
//char st[20];
 
void setup() 
{

  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("----Aproxime o seu cartao/TAG do leitor----");
  Serial.println();
  //Define o número de colunas e linhas do LCD:  
  //lcd.begin(16, 2);  
  //mensageminicial();
}
 
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
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
  
  //PODEMOS TRANSFORMA ESSE PARA SER O ADM
  if (conteudo.substring(1) == "ED 78 03 CA") //UID 1 - Chaveiro
  {
    Serial.println("Ola FILIPEFLOP !");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ola FILIPEFLOP !");
    lcd.setCursor(0,1);
    lcd.print("Acesso liberado!");
    delay(3000);
    mensageminicial();
  }
 
  if (conteudo.substring(1) == "BD 9B 06 7D") //UID 2 - Cartao
  {
    Serial.println("Ola Cartao !");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ola Cartao !");
    lcd.setCursor(0,1);
    lcd.print("Acesso Negado !");
    delay(3000);
    mensageminicial();
  }
} 


/*MENSAGEM INCIAL QUE APARECE NO LCD 
void mensageminicial()
{
  lcd.clear();
  lcd.print(" Aproxime o seu");  
  lcd.setCursor(0,1);
  lcd.print("cartao do leitor");  
}
*/
