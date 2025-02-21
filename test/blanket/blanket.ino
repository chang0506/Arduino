#include <SoftwareSerial.h>
#include <DHT.h>

// 핀 설정
#define DHTPIN 2         // DHT11 센서 핀
#define DHTTYPE DHT11    // DHT11 사용
#define HEATER_PIN 3     // 온열 매트(Heating Pad) 제어 핀

SoftwareSerial BTSerial(10, 11); // HM-10 모듈 RX(10), TX(11) 연결

DHT dht(DHTPIN, DHTTYPE);

float targetTemp = 0; // 목표 온도
bool heaterOn = false; // 온열 매트 상태

void setup() {
  Serial.begin(9600);   // 시리얼 모니터용
  BTSerial.begin(9600); // 블루투스 통신 시작
  dht.begin();          // DHT 센서 시작
  pinMode(HEATER_PIN, OUTPUT); // 온열 매트 핀 출력으로 설정
  digitalWrite(HEATER_PIN, LOW); // 초기 상태는 OFF
  Serial.println("System Ready...");
}

void loop() {
  // 블루투스 데이터 수신 확인
  if (BTSerial.available()) {
    String command = BTSerial.readStringUntil('\n');
    command.trim();  // 공백 제거
    handleCommand(command); // 명령 처리 함수 호출
  }

  // 현재 온도 측정
  float currentTemp = dht.readTemperature();
  
  // 현재 온도 송신
  BTSerial.print("CurrentTemp: ");
  BTSerial.println(currentTemp);
  delay(1000);  // 온도 값을 1초마다 업데이트

  // 목표 온도에 따른 온열 매트 제어
  if (heaterOn && currentTemp >= targetTemp) {
    digitalWrite(HEATER_PIN, LOW); // 목표 온도 도달 시 온열 매트 OFF
    heaterOn = false;
    BTSerial.println("Heater OFF: Target temperature reached");
  }
}

// 명령 처리 함수
void handleCommand(String command) {
  if (command == "on") {
    heaterOn = true;
    digitalWrite(HEATER_PIN, HIGH); // 온열 매트 ON
    BTSerial.println("Heater ON");
  } 
  else if (command == "off") {
    heaterOn = false;
    digitalWrite(HEATER_PIN, LOW);  // 온열 매트 OFF
    BTSerial.println("Heater OFF");
  } 
  else if (command.startsWith("setTemp")) {
    targetTemp = command.substring(8).toFloat(); // 목표 온도 설정
    BTSerial.print("Target temperature set to: ");
    BTSerial.println(targetTemp);
  }
}
