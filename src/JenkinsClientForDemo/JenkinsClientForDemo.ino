/*
  Jenkins Client 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0x90, 0xA2, 0xDA, 0x00, 0x86, 0xE5 };

// Enter a name of Jenkins Server
char serverName[] = "example.com";

int numProjects = 1;
String projects[] = {
  "testProject"
};



// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
String result;

void setup() {
  // start the serial library:
  Serial.begin(9600);
  // start the Ethernet connection:

  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);

  Ethernet.begin(mac);
  // give the Ethernet shield a second to initialize:
  Serial.println("initializing...");
  delay(1000);
}

void loop()
{

  boolean failed = false;
  
  for(int i=0;i<numProjects;i++){

    Serial.println("connecting...");
  
    // if you get a connection, report back via serial:
    if (client.connect(serverName, 8080)) {
      Serial.println("connected: "+projects[i]);
      // Make a HTTP request:
      client.println("GET /jenkins/job/"+projects[i]+"/lastCompletedBuild/api/json?tree=result");
      client.println();
    } 
    else {
      Serial.println("connection failed");
    }
  
    delay(1000);
  
    while (client.available()) {     
      char c = client.read();
      result+=c;
    }
  
    Serial.println(result);
  
    if(result.endsWith("{\"result\":\"FAILURE\"}") ||
       result.endsWith("{\"result\":\"UNSTABLE\"}")){
      failed=true;
    } 
    
    result="";
  
    Serial.println("disconnecting.");
    client.stop();
    
    delay(1000);

  }
  
  if(failed){
    digitalWrite(8, HIGH);
  } else {
    digitalWrite(8, LOW);
  }

  delay(50000);
}

