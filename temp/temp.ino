/*
 *  HTTPS with follow-redirect example
 *  Created by Sujay S. Phadke, 2016
 *  
 *  Based on the WifiClientSecure example by
 *  Ivan Grokhotkov
 *  *
 *  This example is in public domain.
 */

#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"

// Replace with your network credentials
const char* ssid = "352";
const char* password = "bw3kfV^I3BEPX";

const char* host = "script.google.com";
const char* googleRedirHost = "script.googleusercontent.com";
const char *GScriptId = "AKfycbzziBpRIqOqcgdLYxNmN9z5KxTwvqYwSzdhG9G8r1ee";

const int httpsPort = 443;

// echo | openssl s_client -connect script.google.com:443 |& openssl x509 -fingerprint -noout
// SHA1 fingerprint of the certificate
const char* fingerprint = "CE:27:9E:2C:32:68:B0:40:65:66:EA:83:5F:9A:98:BA:09:39:C5:0D";


// Write to Google Spreadsheet
String url = String("/macros/s/") + GScriptId + "/exec?temperature=13";
// Fetch Google Calendar events for 1 week ahead


void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  // flush() is needed to print the above (connecting...) message reliably, 
  // in case the wireless connection doesn't go through
  Serial.flush();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Use HTTPSRedirect class to create TLS connection
  HTTPSRedirect client(httpsPort);
  Serial.print("Connecting to ");
  Serial.println(host);

  bool flag = false;
  for (int i=0; i<5; i++){
    int retval = client.connect(host, httpsPort);
    if (retval == 1) {
       flag = true;
       break;
    }
    else
      Serial.println("Connection failed. Retrying...");
  }
  
  Serial.flush();
  if (!flag){
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    Serial.println("Exiting...");
    return;
  }
  
  Serial.flush();
  if (client.verify(fingerprint, host)) {
    Serial.println("Certificate match.");
  } else {
    Serial.println("Certificate mis-match");
  }

}

void loop() {
  HTTPSRedirect client(httpsPort);
  if (!client.connected())
    client.connect(host, httpsPort);

  client.printRedir(url, host, googleRedirHost);

  // In my testing on a ESP-01, a delay of less than 1500 resulted 
  // in a crash and reboot after about 50 loop runs.
  delay(5*60*1000);
}

