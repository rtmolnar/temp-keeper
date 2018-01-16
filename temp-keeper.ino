#include <ESP8266WiFi.h>
#include <SimpleDHT.h>


int pinDHT11 = 2;
SimpleDHT11 dht11;
const char* ssid = "vodafone0048";
const char* password = "968TGC3UN55D6S";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare GPIO2
  
  pinMode(12, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(15, OUTPUT);

  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {

  delay(1000);

  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    //Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }

  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" H");


  // Set GPIO2 according to the request
  if(temperature > 24){
    Serial.println("too high");
    digitalWrite(12, LOW);
    digitalWrite(5, LOW);
    digitalWrite(15, HIGH);
  } else if(temperature < 19){
    Serial.println("too low");
    digitalWrite(12, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(15, LOW);
  }else{
    Serial.println("normal");
    digitalWrite(12, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(15, LOW);
  }

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
    while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  //int val;
  //if (req.indexOf("/gpio/0") != -1)
  //  val = 0;
  //else if (req.indexOf("/gpio /1") != -1)
  //  val = 1;
  //else {
  //  Serial.println("invalid request");
  //  client.stop();
  //  return;
  //}

  // Set GPIO2 according to the request
  //digitalWrite(2, val);
  
  //client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r";
  //s += (val)?"high":"low";
  s += "<h1>TEMPERATURE = "; 
  s += temperature;
  s += "</h1><br>";
  s += "<h1>HUMIDITY = ";
  s += humidity;
  s += "</h1></html>\n";
  
  

  // Send the response to the client
  client.print(s);
  delay(1000);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

