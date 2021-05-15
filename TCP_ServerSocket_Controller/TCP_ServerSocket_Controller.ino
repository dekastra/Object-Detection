#include <ESP8266WiFi.h>

//#define relayPin D1

// Initialize Port 
int port = 8888;  //Port number

//Server connect to WiFi Network
const char *ssid = "YAKIN 7";  //Enter your wifi SSID
const char *password = "yakin777";  //Enter your wifi Password

//Server status flag
//int status = WL_IDLE_STATUS;

// Create WiFi Server listening on the given port 
WiFiServer server(port);


void setup() {
  // Start serial communication with the given baud rate. 
  // NOTE: Remember to set the baud rate in the Serial
  // monitor to the same value.

  Serial.begin(115200);

//  // Check for the presence of the WiFi shield
//  if( WiFi.status() == WL_NO_SHIELD){
//    // If no shield, print message and exit setup.
//    Serial.println("WiFi shield not present");
//    status = WL_NO_SHIELD;
//    return;
//  }

    WiFi.mode(WIFI_STA);
    // Connect to WPA/WPA2 network. 
    WiFi.begin(ssid, password);

    pinMode(D1, OUTPUT);
    digitalWrite(D1, HIGH);


  // Connect to WiFi Network.
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("Connecting to Network named: ");
    Serial.println(ssid);

    // Wait for connection. 
    delay(1000);
    
  }

  // Start the server. 
  server.begin();

  // Print WiFi status.
  printWifiStatus();
  

}

void loop() {
  // Check that we are connected.
  if(WiFi.status() != WL_CONNECTED){
    return;
  }

  // Listen for incoming client requests.
  WiFiClient client = server.available();
  if(!client){
    return;
  }

  Serial.println("Client connected");

  String request = readRequest(&client);
  Serial.println("");
  Serial.println(request);
  executeRequest(&client, &request);

   // Close the connection.
   client.stop();

  Serial.println("Client disconnected");

}

String readRequest(WiFiClient* client){
  String request = "";

  // Loop while the client is connected.
  while(client->connected()){
    // Read available bytes. 
    while(client ->available()){
      // Read a byte.
      char c = client->read();

      //Print the value (for debugging).
      Serial.write(c);

      // Exit loop if end of line. 
      if('\n' == c){
        return request;
      }

      // Add byte to request line.
      request += c;
    }
  }
  return request;
}

void executeRequest(WiFiClient* client, String* request)
{
  char command = readCommand(request);
  int n = readParam(request);
  Serial.println("");
  Serial.print("Request from Client Socket");
  Serial.println(command);
  if ('1' == command)
  {
    Serial.println("Execute Lamp Off");
    digitalWrite(D1,HIGH);
    Serial.println(digitalRead(D1));
    sendResponse(client, String(digitalRead(D1)));
  }
  else if ('0' == command)
  {
     Serial.println("Execute Lamp On");
     digitalWrite(D1,LOW);
     Serial.println(digitalRead(D1));
     sendResponse(client, String(digitalRead(D1)));
  }
}

// Read the command from the request string.
char readCommand(String* request)
{
  String commandString = request->substring(0, 1);
  return commandString.charAt(0);
}

// Read the parameter from the request string.
int readParam(String* request)
{
  // This handles a hex digit 0 to F (0 to 15).
  char buffer[2];
  buffer[0] = request->charAt(1);
  buffer[1] = 0;
  return (int) strtol(buffer, NULL, 16);
}

void sendResponse(WiFiClient* client, String response)
{
  // Send response to client.
  client->println(response);

  // Debug print.
  Serial.println("sendResponse:");
  Serial.println(response);
}

void printWifiStatus(){
  Serial.println("WiFi status"); 

  // Print network name.
  Serial.print(" SSID: ");
  Serial.println(WiFi.SSID());

  // Print WiFi shield IP Address.
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address : ");
  Serial.println(ip);

  // Print WiFi Port 
  Serial.print("Port : ");
  Serial.println(port);

  // Print the signal strength.
  long rssi = WiFi.RSSI();
  Serial.print(" Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
