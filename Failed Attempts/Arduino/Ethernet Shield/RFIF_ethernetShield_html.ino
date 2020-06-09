#include <SPI.h>
#include <Ethernet.h>
#include <MFRC522.h> // ATUA SOBRE A SPI, SIMPLIFICANDO O PROCESSO DE COMUNICAÇÂO

//PINOS DE ENTRADA E SAIDA
#define SS_PIN 10
#define RST_PIN 9

//DECLARANDO OBJETO QUE REPRESENTARÁ O RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 25, 16);  //-> trocar IP
EthernetServer server(80);     

void setup(){
    Serial.begin(9600);   // Inicia a serial
    SPI.begin();      // Inicia  SPI bus
    mfrc522.PCD_Init();   // Inicia MFRC522
    
    Ethernet.begin(mac, ip);  
    server.begin();           
} 


void loop() {
    EthernetClient client = server.available();  
    if (client) 
    {    
        boolean currentLineIsBlank = true;
        while (client.connected()) 
        {
            if (client.available()) 
            {   
                char c = client.read(); 
                
                if (c == '\n' && currentLineIsBlank) {
                  
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println("Refresh: 2");
                    client.println();
                    
                    client.println("<!DOCTYPE html>");
                    client.println("<html>");
                    
                       client.println("<head>");
                          client.println("<title>Servidor Web VDS</title>");
                       client.println("</head>");
                       
                       client.println("<body>");
                          client.println("<h1 style=\"color:#4279c7\">Servidor Web do Vida de Sil&iacute;cio</h1>");
                          client.println("<hr/>");
                          client.println("<h1>Aproxime seu Cartão/TAG do leitor </h1>");
                         
                          // Look for new cards
                          if ( ! mfrc522.PICC_IsNewCardPresent()){
                            return;
                          }
                          // Select one of the cards
                          if ( ! mfrc522.PICC_ReadCardSerial()){
                            return;
                          }
                          client.println("<h2>UID da tag: </h2>");
                          String conteudo = "";
                         
                         byte letra;
                         for (byte i = 0; i < mfrc522.uid.size; i++){
                            Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                            Serial.print(mfrc522.uid.uidByte[i], HEX);
                            conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
                            conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
                         }
                         client.println("<h2>Mensagem: </h2>");
                         conteudo.toUpperCase();
                         
                         
                        client.println("</body>");
                     client.println("</html>");
                     break;
                     }
                     if (c == '\n') 
                     {    
                     currentLineIsBlank = true;
                     } 
                     else if (c != '\r') 
                     {
                        currentLineIsBlank = false;
                     }
                 } 
              }  
        delay(1);      
        client.stop(); 
    } 
} 
