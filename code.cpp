#include <ESP8266WiFi.h>

const char* ssid = "Wifi network's name";
const char* password = "password";

int LOCK = 4; // GPI04 (D2) 
WiFiServer server (80);

void setup() {
    Serial.begin(9600); 
    delay(10);
    pinMode(LOCK, OUTPUT); 
    digitalWrite(LOCK, LOW);

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
    Serial.println("");
    Serial.println("****WiFi doorlock****");

    // Print IP address
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
}


void loop() {
// Check if a client has connected 
    WiFiClient client = server.available(); 
    if (!client) {
        return; 
    }
// Wait until the client sends some data 
    Serial.println("new client");
    while(!client.available()) {
        delay(1);
    }

    // Read the first line of the request
    String request = client.readStringUntil('\r'); 
    Serial.println(request);
    client.flush();
    
    // Match the request
    int value = LOW;
    if (request.indexOf("/LOCK-ON") != -1) {
        digitalWrite(LOCK, HIGH);
        value = HIGH;
    }
    
    if (request.indexOf("/LOCK-OFF") != -1) {
        digitalWrite(LOCK, LOW);
        value = LOW;
    }
    
    // Set LOCK according to the request
    // Return the response
    client.println("HTTP/1.1 200 OK"); 
    client.println("Content-Type: text/html"); 
    client.println(); 
    client.println("<!DOCTYPE HTML>"); 
    client.println("<html>");
    
    client.print("Door is now: ");
    
    if (value == HIGH) { 
        client.print("Open");
    }
    else {
    client.print("Closed");
    }

    client.println("<br><br>");
    client.println("<a href=\"/LOCK-ON\"><button><h1>Turn On</h1></button></a>"); 
    client.println("<a href=\"/LOCK-OFF\"><button><h1>Turn Off</h1></button></a><br/>");
    client.println("</html>");

    delay(1);
    Serial.println("Client disconnected");
    Serial.println("");
}