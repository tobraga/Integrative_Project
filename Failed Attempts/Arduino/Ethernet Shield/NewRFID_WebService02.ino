#include <SPI.h>
#include <Ethernet.h>
#include <MFRC522.h> //leitor RFID

//Definindo as constantes
#define SS_PIN 10
#define RST_PIN 9

//Definindo os pinos que serão utilizados para ligação do display LCD
String IDtag = ""; //Variável que armazenará o ID da tag

//Vetor responsável por armazenar os IDs das tags cadastradas
String TagsCadastradas[] = {"9d8c4255"}; //ID da tag em formato de cartão
MFRC522 LeitorRFID(SS_PIN, RST_PIN); // Cria uma nova instância para o leitor e passa os pinos como parâmetro


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 25, 16); 
EthernetServer server(80);     

void setup() 
{
    Ethernet.begin(mac, ip);  
    server.begin();

	Serial.begin(9600); // Inicializa a comunicação Serial
	Serial.println("Aproxime seu cartao do leitor...");
	Serial.println();	
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
                    client.println("<h1><font color=#4279c7>Servidor Web do Vida de Sil&iacute;cio</font></h1>");
                    client.println("<hr/>");
                    client.println("<h1>Temperatura</h1>");
                    leitura_tag(client);   
                    client.println("<br/>");
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

void leitura_tag(EthernetClient client_aux)
{
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
  
  client_aux_println("TAG Cadastrada: ");
  client_aux.println("<font color=#FF0000>");
  client_aux.println(IDtag);
  client_aux.println("------");
  client_aux.println("</font>");

 }
