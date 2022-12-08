#include <Arduino.h>
#include <HTTPClient.h>


const char* ssid = "Roborn2.4G";
const char* password = "Cyberport@811";

int port = 1234;
WiFiServer server(port);
String header;

String ledStat = "off";

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while(WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    Serial.print(".");

  }
  
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.println(port);
  digitalWrite(LED_BUILTIN, LOW);
  server.begin();
}



void send_byte(uint8_t * ptr_arr){
  Serial.print("bytearr:");
  for (int i = 0; i < 8; i++) {
		printf("%x ",  * (ptr_arr + i));
	}
  Serial.println(16, HEX);
}
void loop() {
  WiFiClient client = server.available();
  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client."); 
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /on") >= 0) {
              client.println("onnn");
              Serial.println("LED on");
              ledStat = "on";
              digitalWrite(LED_BUILTIN, HIGH);
            }else if(header.indexOf("GET /off") >= 0) {
                client.println("offff");
                Serial.println("LED off");
                ledStat = "off";
                digitalWrite(LED_BUILTIN, LOW);
            }else if(header.indexOf("GET /cmd=") >= 0) {
                client.println("command received");
                Serial.print("header is ");
                Serial.println(header);

                std::vector<byte> vec  { 0x92, 0, 0, 0, 0x78, 0, 0 ,0};
                uint8_t *ptr_arr = &vec[0];
                //send_byte(ptr_arr);
            }
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