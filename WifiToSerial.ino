
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


void handleRoot();
void handleDati();
void muovi();
void message();


/* Set these to your desired credentials. */
const char *ssid = "SSID";
const char *password = "Password";

boolean mute = true;

ESP8266WebServer server(80);

MDNSResponder mdns;
String bufferString1 ="";

void setup(){

  Serial.begin(9600);
  serverSetup();
  wifiSetup();

}

void loop(){
    if(Serial.available()>0){
      
      bufferString1= Serial.readString();
      
    }
    server.handleClient();

}

void serverSetup(){
  server.on("/muovi", muovi);
  server.on("/dati", handleDati);
  server.on("/message", message);
  server.on("/", handleRoot);

  server.begin();
  if(!mute){
    Serial.println("HTTP server started");
  }

}



int wifiSetup(){

  int n = WiFi.scanNetworks();
  if(!mute){
    Serial.println(n);
    Serial.println("ricerca SSID");
  }
  boolean connectedToKnown =false;
  for (int i =0; i<n; i++){
    if(!mute){
      Serial.println(i);
      Serial.println(WiFi.SSID(i));
    }
    if(WiFi.SSID(i) == "SSID1"){

      WiFi.begin("SSID1", "PASS1");

      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        if(!mute){
          Serial.print(".");
        }
      }
      if(!mute){
        Serial.println("");
        Serial.println("WiFi connected");

        Serial.println("Server started");

        // Print the IP address
        Serial.println(WiFi.localIP());
      }
      connectedToKnown =true;
    }
    else if(WiFi.SSID(i) == "SSID2"){

      WiFi.begin("SSID2", "PASS2");

      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        if(!mute){
          Serial.print(".");
        }
      }
      if(!mute){
        Serial.println("");
        Serial.println("WiFi connected");

        Serial.println("Server started");

        // Print the IP address
        Serial.println(WiFi.localIP());
      }
      connectedToKnown =true;
    }
    else{

    }
  }
  if(!connectedToKnown){
    if(!mute){
      Serial.println("init soft AP");
    }
    WiFi.softAP(ssid, password);
    //while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
    delay(5000);
    if(!mute){
      Serial.println("done");
    }
    IPAddress myIP = WiFi.softAPIP();
    if(!mute){
      Serial.print("AP IP address: ");
      Serial.println(myIP);

      Serial.println("Server started");
    }
  }
  if (!mdns.begin("esp8266", WiFi.localIP())) {
    if(!mute){     //example address http://esp8266.local/
      Serial.println("Error setting up MDNS responder!");
    }
  }else{
    if(!mute){
      Serial.println("mDNS responder started");
    }
  }
  if(!mute){
    Serial.println(" ");
  }



}
void message(){
  String response = "messaggio";
  String receivedMessage = server.arg("message");
  Serial.println(receivedMessage);
  response= response+receivedMessage+bufferString1;
  server.send(200, "text/html", response);


}
void muovi(){
  String response = "Muovi ricevuto:)";
  server.send(200, "text/html", response);
  String asse = server.arg("asse");
  String distanza = server.arg("distanza");
  Serial.println("3");//muovi
  delay(1000);
  Serial.println("4");//ferma
}

void handleDati(){
  String response = "Hello from ESP8266";
  Serial.print("a");
  server.send(200, "text/html", response);
}

void handleRoot() {

  int sec = millis() % 1000;
  int min = sec % 60;
  int hr = min / 60;
  char text[2000];
  for(int i =0;i<1;i++){
    sprintf(text,"Online from %d:%d:%d", hr, min, sec);
  }
  if(!mute){
    Serial.println("diagnostica fornita.");
  }
  server.send(200, "text/html", text);
}
