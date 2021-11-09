#include <WiFi.h>
#include "time.h"

const char* ssid       = "SURFACEJORGE 2228";
const char* password   = "p0@926G8";


const char* ntpServer = "europe.pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

int displayOn = 0;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

//ipconfig servidor
const IPAddress serverIP(192,168,137,1); // La dirección que desea visitar
uint16_t serverPort = 21;         // Número de puerto del servidor

WiFiClient client;

void setup()
{
  Serial.begin(115200);
  
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");

  Serial.print("IP Address:");
  Serial.println(WiFi.localIP());
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
//  WiFi.disconnect(true);
//  WiFi.mode(WIFI_OFF);

  

  
}

void loop()
{

 Serial.println("Intenta acceder al servidor");
    if (client.connect(serverIP, serverPort)) // Intenta acceder a la dirección de destino
    {
      while (client.connected() || client.available()) // Si está conectado o se han recibido datos no leídos
        {
          if (client.available()) // Si hay datos para leer
            {     
              String line = client.readStringUntil('\n'); // Leer datos a nueva línea

              Serial.println(line);
              
              if(line.equals("start")){
                displayOn = 1;
                Serial.println("true");
              }
              else //if(line == "stop"){
                displayOn = 0;
                Serial.println("false");
              //}
          
            
            }

            if(displayOn == 1){
              delay(1000);
              printLocalTime();
              //client.write(printLocalTime());
            }
        }
    }
    
    else
    {
        Serial.println("Acceso fallido");
        //client.stop(); // Cerrar el cliente
    }


    

  
}
