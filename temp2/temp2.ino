
#include "ESP8266WiFi.h"



const char WEBSITE[] = "api.pushingbox.com"; //pushingbox API server
const String devid = "v26E6332AE413BF8"; //device ID from Pushingbox 

const char* MY_SSID = "352";
const char* MY_PWD =  "bw3kfV^I3BEPX";


void setup()
{
  Serial.begin(9600);
  Serial.print("Connecting to "+*MY_SSID);
  WiFi.begin(MY_SSID, MY_PWD);
  Serial.println("going into wl connect");

  while (WiFi.status() != WL_CONNECTED) //not connected,..waiting to connect
    {
      delay(1000);
      Serial.print(".");
    }
  Serial.println("wl connected");
  Serial.println("");
  Serial.println("Credentials accepted! Connected to wifi\n ");
  Serial.println("");
}


void loop()
{
  
  //Wait between measurements longer then normal to slow donwn
  //google sheet populate, as We dont want to exceed free service quota
  delay(1000*60*60); //10 seconds, (sampling rate vs. service call quota)



  float temperature = 15;
  WiFiClient client;  //Instantiate WiFi object

    //Start or API service using our WiFi Client through PushingBox
    if (client.connect(WEBSITE, 80))
      { 
         client.print("GET /pushingbox?devid=" + devid
       + "&temperature=" + (String) temperature
           );

      client.println(" HTTP/1.1"); 
      client.print("Host: ");
      client.println(WEBSITE);
      client.println("User-Agent: ESP8266/1.0");
      client.println("Connection: close");
      client.println();
      }
}

