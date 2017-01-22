#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <HTTPSRedirect.h>

#define PIN A0

// WiFi
const char *ssid = "352"; 
const char *password = "bw3kfV^I3BEPX";

// host 
const char *host = "script.google.com";
const char* googleRedirHost = "script.googleusercontent.com";
const int httpsPort = 443;
HTTPSRedirect client(httpsPort); 
//WiFiClientSecure client; 
const char* fingerprint = "24:5C:23:54:2A:42:CE:EC:90:84:09:0D:3F:BA:A8:93:E2:17:7B:01"; 
String url = String("/macros/s/") + "AKfycbzziBpRIqOqcgdLYxNmN9z5KxTwvqYwSzdhG9G8r1ee" + "/exec?";
const int dataPostDelay = 900000;   
void setup()
{
  Serial.begin(9600);
  
  pinMode(PIN,INPUT);

  // WiFi >>
  Serial.print("Connect to WiFi ");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");  
  }
  Serial.println();
  Serial.println("WiFi Connected");
  Serial.println("WiFi IP address:");
  Serial.println(WiFi.localIP());
  // WiFi <<

  // Connect to Host >>
  Serial.print(String("Connecting to "));
  Serial.println(host);

  bool flag = false;
  for (int i=0; i<5; i++){
          int retval = client.connect(host, httpsPort);
          if (retval == 1) {
                      flag = true;
                      break;
          }
          else
                  Serial.println("Connection failed. Retrying…");
  }

  // Connection Status, 1 = Connected, 0 is not.
  Serial.println("Connection Status: " + String(client.connected()));
  Serial.flush();
  
  if (!flag){
          Serial.print("Could not connect to server: ");
          Serial.println(host);
          Serial.println("Exiting…!");
          Serial.flush();
          return;
  }

  // Data will still be pushed even certification don’t match.
  if (client.verify(fingerprint, host)) {
          Serial.println("Certificate match.");
  } else {
          Serial.println("Certificate mis-match");
  }  
  
}

void postData(String tag, float value){
    if (!client.connected()){
            Serial.println("Connecting to client again…");
            client.connect(host, httpsPort);
    }
    String urlFinal = url + tag + "=" + String(value);
    client.printRedir(urlFinal, host, googleRedirHost);
}

void loop()
{
    float temp = (analogRead(PIN) * (5000 / 1024.0)) / 10;
    Serial.println(temp);

    // Post these information
    postData("temperature", temp);
   
    delay(dataPostDelay);
}

