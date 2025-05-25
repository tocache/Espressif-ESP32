/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
  Modificado por Kalun Lau
  24 de mayo del 2025
*********/

// Load Wi-Fi library
#include <WiFi.h>
#include <DHT.h>

//variables para el DHT22
DHT dht_blanco(23, DHT22);
float temperatura = 0;
float humedad = 0;

// Replace with your network credentials
const char* ssid     = "UNI_MIA_B";
const char* password = "0123456789";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output32State = "off";
String output33State = "off";
String output25State = "off";

// Assign output variables to GPIO pins
const int output32 = 32;
const int output33 = 33;
const int output25 = 25;
const int potPin = 34;

// variable for storing the potentiometer value
int potValue = 0;

void setup() {
  Serial.begin(115200);
  dht_blanco.begin();
  // Initialize the output variables as outputs
  pinMode(output32, OUTPUT);
  pinMode(output33, OUTPUT);
  pinMode(output25, OUTPUT);
  
  // Set outputs to LOW
  digitalWrite(output32, LOW);
  digitalWrite(output33, LOW);
  digitalWrite(output25, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password,1,0,4);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  temperatura = dht_blanco.readTemperature();
  humedad = dht_blanco.readHumidity();  
  Serial.printf("Temperatura DHT22: %.2f ºC\n", temperatura);
  Serial.printf("Humedad DHT22: %.2f %RH\n", humedad);
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients
  potValue = analogRead(potPin);
  temperatura = dht_blanco.readTemperature();
  humedad = dht_blanco.readHumidity();  
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /32/on") >= 0) {
              Serial.println("GPIO 32 on");
              output32State = "on";
              digitalWrite(output32, HIGH);
            } else if (header.indexOf("GET /32/off") >= 0) {
              Serial.println("GPIO 32 off");
              output32State = "off";
              digitalWrite(output32, LOW);
            } else if (header.indexOf("GET /33/on") >= 0) {
              Serial.println("GPIO 33 on");
              output33State = "on";
              digitalWrite(output33, HIGH);
            } else if (header.indexOf("GET /33/off") >= 0) {
              Serial.println("GPIO 33 off");
              output33State = "off";
              digitalWrite(output33, LOW);
            } else if (header.indexOf("GET /25/on") >= 0) {
              Serial.println("GPIO 25 on");
              output25State = "on";
              digitalWrite(output25, HIGH);
            } else if (header.indexOf("GET /25/off") >= 0) {
              Serial.println("GPIO 25 off");
              output25State = "off";
              digitalWrite(output25, LOW);
            }
            Serial.print("Canal analogico:");
            Serial.println(potValue);
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>UNI MIA SECCION_B ESP32 Web Server</h1><br>");
            client.println("<h1>&#x2620;</h1><br>");            
            //client.println("<img src=\"https://i.ibb.co/RvdJHHt/upc.jpg\"><br>");
            
            // Display current state, and ON/OFF buttons for GPIO 32  
            client.println("<p>GPIO 32 - State " + output32State + "</p>");
            // If the output32State is off, it displays the ON button       
            if (output32State=="off") {
              client.println("<p><a href=\"/32/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/32/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 33  
            client.println("<p>GPIO 33 - State " + output33State + "</p>");
            // If the output33State is off, it displays the ON button       
            if (output33State=="off") {
              client.println("<p><a href=\"/33/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/33/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 25  
            client.println("<p>GPIO 25 - State " + output25State + "</p>");
            // If the output25State is off, it displays the ON button       
            if (output25State=="off") {
              client.println("<p><a href=\"/25/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/25/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            //Mi codiguito para que se muestre el canal analogico en la página web:
              client.println("<p>Canal Analogico GPIO34:</p>");
              client.println(potValue);
              client.println("<p>Temperatura DHT22 GPIO23</p>");
              client.print(temperatura);
              client.write(0xB0);
              client.println("C");
              client.println("<p>Humedad DHT22 GPIO23</p>");
              client.print(humedad);
              client.println("%RH");
                                      
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}