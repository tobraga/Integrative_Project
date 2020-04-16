    #include <Ethernet.h>
    #include <MySQL_Connection.h>
    #include <MySQL_Cursor.h>
    #include <SPI.h>
    #define LM35 A0
    int leitura;
    float leituraconvertida
    char sentenca[128];
    char valortemp[10];
    byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    IPAddress server_addr(192,168,25,5);  
    char user[] = "arduino";              
    char password[] = "arduino";        
    char INSERIR_TEMP[] = "INSERT INTO registrotemp (temperatura) VALUES (%s)";
    char BANCODEDADOS[] = "USE banco_arduino";
    EthernetClient client;
    MySQL_Connection conn((Client *)&client);
    
    
    void setup() 
    { 
       Serial.begin(115200);
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
       leitura = analogRead(LM35);
       leituraconvertida = (float(analogRead(LM35))*5/(1023))/0.01;
       dtostrf(leituraconvertida, 4, 1, valortemp);
       sprintf(sentenca, INSERIR_TEMP, valortemp);
       
       MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
       cur_mem->execute(sentenca);
       delete cur_mem;
       delay(2000);
    }
