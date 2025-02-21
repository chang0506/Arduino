#include <DHT.h>

// 핀 설정
#define DHTPIN 2         // DHT11 센서 핀
#define DHTTYPE DHT11    // DHT11 사용
#define HEATER_PIN 3     // 온열 매트(Heating Pad) 제어 핀

DHT dht(DHTPIN, DHTTYPE);

float targetTemp = 0; // 목표 온도
bool heaterOn = false; // 온열 매트 상태

void setup() {
  Serial.begin(9600);   // 시리얼 모니터용
  dht.begin();          // DHT 센서 시작
  pinMode(HEATER_PIN, OUTPUT); // 온열 매트 핀 출력으로 설정
  digitalWrite(HEATER_PIN, LOW); // 초기 상태는 OFF
  Serial.println("System Ready...");
  Serial.println("Enter command: on/off/setTemp <value>");
  testRelay();
}

void loop() {
  // 시리얼 입력 확인
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();  // 공백 제거
    handleCommand(command); // 명령 처리 함수 호출
  }

  // 현재 온도 측정
  float currentTemp = dht.readTemperature();
  
  // 현재 온도 송신
  Serial.print("CurrentTemp: ");
  Serial.println(currentTemp);
  
  // 목표 온도에 따른 온열 매트 제어
  if (heaterOn) {
    if (currentTemp >= targetTemp) {
      digitalWrite(HEATER_PIN, LOW); // 목표 온도 도달 시 온열 매트 OFF
      heaterOn = false;
      Serial.println("Heater OFF: Target temperature reached");
    }
  }
  else {
    if (currentTemp < targetTemp) {
      digitalWrite(HEATER_PIN, HIGH); // 목표 온도 도달 전에는 온열 매트 ON
      heaterOn = true;
      Serial.println("Heater ON");
    }
  }

  delay(1000);  // 1초마다 업데이트
}
// 릴레이 테스트 함수
void testRelay() {
  digitalWrite(HEATER_PIN, HIGH); // 릴레이 활성화
  Serial.println("Relay ON");
  delay(5000);                     // 5초 대기
  digitalWrite(HEATER_PIN, LOW);  // 릴레이 비활성화
  Serial.println("Relay OFF");
}
// 명령 처리 함수
void handleCommand(String command) {
  if (command == "on") {
    heaterOn = true;
    digitalWrite(HEATER_PIN, HIGH); // 온열 매트 ON
    Serial.println("Heater ON");
  } 
  else if (command == "off") {
    heaterOn = false;
    digitalWrite(HEATER_PIN, LOW);  // 온열 매트 OFF
    Serial.println("Heater OFF");
  } 
  else if (command.startsWith("setTemp")) {
    targetTemp = command.substring(8).toFloat(); // 목표 온도 설정
    Serial.print("Target temperature set to: ");
    Serial.println(targetTemp);
  }
}
