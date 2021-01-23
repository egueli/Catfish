
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ArduinoOTA.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
// char auth[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

// Your WiFi credentials.
// Set password to "" for open networks.
// char ssid[] = "************";
// char pass[] = "************";
#include "credentials.h"

unsigned long startWiggleAt;
unsigned long wigglePeriod = 300;

const uint8_t kMotorAPin = 0;
const uint8_t kMotorBPin = 2;

WidgetLCD lcd(V3);

BLYNK_WRITE(V0)
{
    wigglePeriod = param.asLong();
}

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt();
  if (pinValue == 1) {
      startWiggleAt = millis();
  } else {
      startWiggleAt = 0;
      digitalWrite(kMotorAPin, LOW);
      digitalWrite(kMotorBPin, LOW);
  }
}

void otaSetup();

void setup()
{
  // Debug console
  Serial.begin(9600);

  pinMode(kMotorAPin, OUTPUT);
  digitalWrite(kMotorAPin, LOW);
  pinMode(kMotorBPin, OUTPUT);
  digitalWrite(kMotorBPin, LOW);

  Blynk.begin(auth, ssid, pass);
  lcd.print(0, 0, "IP:" + WiFi.localIP().toString());

  otaSetup();
}

void loop()
{
  Blynk.run();

  if (startWiggleAt != 0) {
      int turn = (millis() / wigglePeriod) % 2;
      digitalWrite(kMotorAPin, turn == 0 ? HIGH : LOW);
      digitalWrite(kMotorBPin, turn == 1 ? HIGH : LOW);
  }

  ArduinoOTA.handle();
}

void otaSetup() {
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    Blynk.disconnect();

    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}