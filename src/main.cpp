
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

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
}

void loop()
{
  Blynk.run();

  if (startWiggleAt != 0) {
      int turn = (millis() / wigglePeriod) % 2;
      digitalWrite(kMotorAPin, turn == 0 ? HIGH : LOW);
      digitalWrite(kMotorBPin, turn == 1 ? HIGH : LOW);
  }
}

