#include <SoftwareSerial.h>

SoftwareSerial client(2, 3); //RX, TX

#define BAUD 115200

const int LED_A = 4;
const int LED_B = 5;
const int LED_C = 6;
const int LED_D = 7;
const int BTN_A = 8;
const int BTN_B = 9;
const int BTN_C = 10;
const int BTN_D = 11;

int mode_standalone = true;

int i = 0;
String input = "";

void send_cmd_OK(String cmd, int t) {
  int temp = 0, i = 0;
  /*
  Serial.print("D(");
  while (client.available()) {
    char ch = client.read();
    Serial.print(ch);
  }
  Serial.println(")D");
  */
  while (1) {
    input = "";
    Serial.print(">");
    Serial.println(cmd);
    client.println(cmd); 
//    delay(50);
    while (client.available()) {
      char ch = client.read();
      input += ch;
      if (input.indexOf("OK") >= 0) {
        i = 8;
        break;
      }
      for (int j = 0; j < 10 && !client.available(); j++) {
        Serial.print("{");
        delay(100);
      }
    }
    Serial.print("<[");
    Serial.print(input);
    Serial.println("]>");
    delay(t);
    if (i > 1) {
      break;
    }
    i++;
  }
  if (i == 8) {
    Serial.println("=OK");
  } else {
    Serial.println("=Error");
  }
}

void wifi_init() {
  while (client.available()) {
    char ch = client.read();
  }
  send_cmd_OK("AT+RST", 2000);
  send_cmd_OK("AT", 100);
  send_cmd_OK("AT+CWMODE=1", 100);
  if (mode_standalone) {
    send_cmd_OK("AT+CWJAP=\"ypieee\",\"ypbutton\"", 3000);
  } else {
    }
  Serial.println("Wifi Connected");
  send_cmd_OK("AT+CIFSR", 100);
}

void btn_init() {
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_C, OUTPUT);
  pinMode(LED_D, OUTPUT);
  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);
  pinMode(BTN_C, INPUT_PULLUP);
  pinMode(BTN_D, INPUT_PULLUP);
}

void setup() {
  Serial.begin(BAUD);
  btn_init();
  delay(500);
  mode_standalone = digitalRead(BTN_A) != LOW;
  if (mode_standalone) {
    Serial.println("Using standalone AP.");
  } else {
    Serial.println("Using homell AP.");
  }
  
  client.begin(BAUD);
  wifi_init();
  Serial.println("System Ready..");
//  send_msg("wifi_init");
  delay(2000);
}

// ---------------------------------------------------------------------------------

void send_msg(String msg) {
  if (mode_standalone) {
    send_cmd_OK("AT+CIPSTART=\"TCP\",\"192.168.1.156\",5555", 100);
  } else {
    send_cmd_OK("AT+CIPSTART=\"TCP\",\"192.168.1.102\",5555", 100);
  }
  String cmd = "AT+CIPSEND=";
  cmd += (msg.length() + 2);
  send_cmd_OK(cmd, 100);
  send_cmd_OK(msg, 100);
//  send_cmd_OK("AT+CIPSEND=6", 100);
//  send_cmd_OK("quit", 100);
//  send_cmd_OK("AT+CIPSTATUS", 100);
  send_cmd_OK("AT+CIPCLOSE", 100);
}

void loop() {
  if (digitalRead(BTN_A) == LOW) {
    digitalWrite(LED_A, HIGH);
    send_msg("POST RFID15 ButtonA");
  } else if (digitalRead(BTN_B) == LOW) {
    digitalWrite(LED_B, HIGH);
    send_msg("POST RFID15 ButtonB");
  } else if (digitalRead(BTN_C) == LOW) {
    digitalWrite(LED_C, HIGH);
    send_msg("POST RFID15 ButtonC");
  } else if (digitalRead(BTN_D) == LOW) {
    digitalWrite(LED_D, HIGH);
    send_msg("POST RFID15 ButtonD");
  }
  digitalWrite(LED_A, LOW);
  digitalWrite(LED_B, LOW);
  digitalWrite(LED_C, LOW);
  digitalWrite(LED_D, LOW);
  delay(50);
}


