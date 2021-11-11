/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

// Replace with your network credentials
const char* ssid = "pini";
const char* password = "123456_Aa";

const char* ntpServer = "europe.pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

    

String printLocalTime()
{  
  struct tm timeinfo;
  String tiempo_actual = "";
  
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return "";
  }
  tiempo_actual = String(timeinfo.tm_hour) + ":" +
                  String(timeinfo.tm_min) + ":" +
                  String(timeinfo.tm_sec) + "\n";
                  
  return tiempo_actual;
}



// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  if(var == "Tiempo"){
    return printLocalTime();
  }
  return String();
}

String processor1(const String& var){
  Serial.println(var);
  if(var == "Tiempo"){  
       
   return String(0) + ":" + String(0) + ":" + String(0) + "\n";
   
  }
  return String();
}


 
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);


  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to rest time
    server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request){  
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to rest time
    server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){  
    request->send(SPIFFS, "/index.html", String(), false, processor1);
  });

  // Start server
  server.begin();

  
}
 
void loop(){

  
  
}
