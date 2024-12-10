#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <SoftwareSerial.h>


#define BLYNK_TEMPLATE_ID "TMPL69O5sfleQ"
#define BLYNK_TEMPLATE_NAME "login lock"
#define BLYNK_AUTH_TOKEN "5JBqwGlfeSmFhcP_KqDBxG31HYxa5qwN"
// #define BLYNK_PRINT Serial
// WiFi credentials
const char* ssid = "FPTU_Student";
const char* password = "12345678";
char auth[] = "5JBqwGlfeSmFhcP_KqDBxG31HYxa5qwN"; // Blynk Auth Token
#include <BlynkSimpleEsp8266.h>
// Google Apps Script ID
const char* GAS_ID = "AKfycbzUyyiRwNf5JO4iJ-qn2rProGRXROTlahm3159_SGVTKrSq3eXcg2Nrn_D6q3OgCrOgOA";

// Server details
const char* host = "script.google.com";
const int httpsPort = 443;

// WiFiClientSecure for TLS connection
WiFiClientSecure client;

// SoftwareSerial pins
#define RX_PIN 4
#define TX_PIN 5
SoftwareSerial espSerial(RX_PIN, TX_PIN);

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Blynk.begin(auth, ssid, password); // Khởi tạo Blynk
  // Set the client to insecure mode
  client.setInsecure();
}

void loop() {
    Blynk.run(); // Chạy Blynk
  if (espSerial.available()) {
    String data = espSerial.readString();
    String col1 = getValue(data, ';',0);
    String col2 = getValue(data, ';', 1);
    String col3 =  getValue(data, ';', 2);
    String col4=getValue(data, ';', 3);
    sendData(col1, col2, col3, col4);
     // Gửi dữ liệu tới Blynk
    Blynk.virtualWrite(V1, col1);
    Blynk.virtualWrite(V2, col2);
    Blynk.virtualWrite(V3, col3);
    Blynk.virtualWrite(V4, col4);
  }
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void sendData(String col1, String col2, String col3, String col4) {
  Serial.print("Connecting to ");
  Serial.println(host);

  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection failed");
    return;
  }

  String url = "/macros/s/" + String(GAS_ID) + "/exec?col1=" + col1 + "&col2=" + col2 + "&col3=" + col3 + "&col4=" + col4;
  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("Request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Headers received");
      break;
    }
  }

  String line = client.readStringUntil('\n');
  Serial.println("Reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("Closing connection");
}
