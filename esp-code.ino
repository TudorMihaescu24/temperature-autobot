#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

const char* ssid = "The Frat House Robot";
const char* password = "123456789";

#define ENA   4    
#define IN_1  0   
#define IN_2  2    
#define IN_3  12 
#define IN_4  13
#define ENB   15
int currentSpeed = 200;  

#define DHTPIN D1    
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

String command = "";          
ESP8266WebServer server(80);    

void setup() {
  Serial.begin(115200);

  pinMode(ENA, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(ENB, OUTPUT);


  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Access Point IP: ");
  Serial.println(myIP);

  server.on("/", handleRoot);
  server.on("/temperature", handleTemperature);
  server.onNotFound(handleRoot);
  server.begin();
  Serial.println("Web server started");
  
  dht.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  if (server.hasArg("State")) {
    command = server.arg("State");
    processCommand(command);
  }
  server.send(200, "text/html", "Command received");
}

void handleTemperature() {
  Serial.println("Reading DHT sensor...");
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    server.send(500, "text/plain", "Failed to read from DHT sensor");
    return;
  }

  String json = "{\"temperature\":" + String(temperature, 1) + ",\"humidity\":" + String(humidity, 1) + "}";
  server.send(200, "application/json", json);
}


void processCommand(String cmd) {
  if (cmd == "F") {
    goForward();
  } else if (cmd == "B") {
    goBackward();
  } else if (cmd == "L") {
    goLeft();
  } else if (cmd == "R") {
    goRight();
  }
 else {
    stopRobot(); 
  }
}

void goForward() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, currentSpeed);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, currentSpeed);
}

void goBackward() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, currentSpeed);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, currentSpeed);
}

void goLeft() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, currentSpeed);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, currentSpeed);
}

void goRight() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, currentSpeed);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, currentSpeed);
}

void stopRobot() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, 0);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, 0);
}
