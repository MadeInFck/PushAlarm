#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Temboo.h>
#include "TembooAccount.h" // Contains Temboo account information
#include <Bounce2.h>

WiFiClient client;

int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 3;   // Maximum number of times the Choreo should be executed

//Define your button
#define ButtonPin D3
Bounce Button = Bounce();

void setup() {
  Serial.begin(115200);
  
  // Try to connect to the local WiFi network
   WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("WiFi: Fail");
    delay(3000);
  }
  Serial.println("");
      Serial.println("WiFi connected");  
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());

//Initialize button
  pinMode(ButtonPin,INPUT);
  digitalWrite(ButtonPin,HIGH);
  Button.attach(ButtonPin);
  Button.interval(5);

  Serial.println("Setup complete.\n");
}

void loop() {
  Serial.println(Button.read());
 if (Button.update()) {
  if (Button.fell()) {
  
        while (numRuns <= maxRuns) {
          Serial.println("Running SendEmail - Run #" + String(numRuns++));
      
          TembooChoreo SendEmailChoreo(client);
      
          // Invoke the Temboo client
          SendEmailChoreo.begin();
      
          // Set Temboo account credentials
          SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
          SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
          SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);
      
          // Set profile to use for execution
          SendEmailChoreo.setProfile("Name of the profile");
      
          // Identify the Choreo to run
          SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");
      
          // Run the Choreo; when results are available, print them to serial
          SendEmailChoreo.run();
      
          while(SendEmailChoreo.available()) {
            char c = SendEmailChoreo.read();
            Serial.print(c);
          }
          SendEmailChoreo.close();
          Serial.println("\nWaiting...\n");
          delay(30000); // wait 30 seconds between SendEmail calls
        }
      
        
    }
 }
 numRuns=1;
}
