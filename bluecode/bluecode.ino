#include <SoftwareSerial.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define RELAY_PIN 3

SoftwareSerial BTSerial(10, 11);

DHT dht(DHTPIN, DHTTYPE);

float setTemp = 20;
bool heaterOn = false;

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  Serial.println("System Ready...");
}

void loop() {
  if (BTSerial.available()) {
    String command = BTSerial.readStringUntil('\n');
    command.trim();
    handleCommand(command);
  }

  float currentTemp = dht.readTemperature();
  BTSerial.print(currentTemp);
  delay(1000);
  if (setTemp > 20 && heaterOn) {
    if (currentTemp < setTemp) {
      digitalWrite(RELAY_PIN, HIGH);
      Serial.println("가열중..");
    } else {
      digitalWrite(RELAY_PIN, LOW);
      Serial.println("목표온도 도달 장판꺼짐.");
    }
  }
}

void handleCommand(String command) {
  if (command == "ON") {
    heaterOn = true;
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("장판 on");
  } 
  else if (command == "OFF") {
    heaterOn = false;
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("장판 off");
  } 
  else if (command.startsWith("setTemp")) {
    String tempStr = command.substring(7);
    setTemp = tempStr.toFloat();
    Serial.print("setTemp는: ");
    Serial.println(setTemp);
  }
  else if (command == "RESET"){
    setTemp = 20;
    Serial.print("setTemp초기화");
  }
}
