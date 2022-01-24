
#include "Arduino.h"
#include <WiFi.h>
#include <WiFiClient.h> 
#include <ESP32_FTPClient.h>
#include <ArduinoJson.h>

#define WIFI_SSID "RSense"
#define WIFI_PASS ""

char ftp_server[] = "155.210.150.77";
char ftp_user[]   = "rsense";
char ftp_pass[]   = "rsense";

ESP32_FTPClient ftp (ftp_server,ftp_user,ftp_pass);



void setup()
{
  Serial.begin( 115200 );

  WiFi.begin( WIFI_SSID, WIFI_PASS );
  
  Serial.println("Connecting Wifi...");
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("\nMax Free Heap: ");
  Serial.println(ESP.getMaxAllocHeap());
  Serial.println("");

  //creo el array de char que se enviara como informacion del fichero
  char texto[1024];

  //creo el documento Json
  DynamicJsonDocument doc(1024);

  //asigno valores al json
  doc["sensor"] = "gps";
  doc["time"]   = 135182;
  doc["data"][0] = 48.75;
  doc["data"][1] = 2.30;

  //asigno a "texto" la información del json
  serializeJson(doc, texto);

  ftp.OpenConnection();

  //Change directory
  
  ftp.ChangeWorkDir("/rsense/jorge");

  // Create a new file to use as the download example below:

  // Type x; Donde A corresponde a ASCII, E EBCDIC, I Image, L Local byte Byte size
  ftp.InitFile("Type A");
  
  ftp.NewFile("Json.txt");
  //mando el array de char con la info del json
  ftp.Write(texto);
  ftp.CloseFile();

  //Download the text file or read it
  String response = "";
  ftp.InitFile("Type A");
  ftp.DownloadString("Json.txt", response);
  Serial.println("The file content is: " + response);
  //deserializeJson(response, json);

 

 

  ftp.CloseConnection();
}

void loop()
{

}
