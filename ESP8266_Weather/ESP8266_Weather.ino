#include <SoftwareSerial.h>

#define SSID "dlink" // insert your SSID
#define PASS "15B98AF0BCA6EE4AB984984FE2" // insert your password
#define LOCATIONID "2925533" // location id
#define DST_IP "188.226.224.148" //api.openweathermap.org
SoftwareSerial dbgSerial(2, 3); // RX, TX for debugging

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(5000);
  dbgSerial.begin(9600); // for debuging
  dbgSerial.println("Init");
  Serial.println("AT+RST"); // reset and test if module is ready
  delay(1000);
  if (Serial.find("ready")) {
    dbgSerial.println("WiFi – Module is ready");
  } else {
    dbgSerial.println("Module doesn’t respond.");
    while (1);
  }
  delay(1000);
  // try to connect to wifi
  boolean connected = false;
  for (int i = 0; i < 5; i++) {
    if (connectWiFi()) {
      connected = true;
      dbgSerial.println("Connected to WiFi…");
      break;
    }
  }
  if (!connected) {
    dbgSerial.println("Coudn’t connect to WiFi.");
    while (1);
  }
  delay(5000);
  Serial.println("AT+CIPMUX=0"); // set to single connection mode
}

void loop()
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += DST_IP;
  cmd += "\",80";
  Serial.println(cmd);
  dbgSerial.println(cmd);
  if (Serial.find("Error")) return;
  cmd = "GET /data/2.5/weather?id=";
  cmd += LOCATIONID;
  cmd += " HTTP/1.0\r\nHost: api.openweathermap.org\r\n\r\n";
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if (Serial.find(">")) {
    dbgSerial.print(">");
  } else {
    Serial.println("AT+CIPCLOSE");
    dbgSerial.println("connection timeout");
    delay(1000);
    return;
  }
  Serial.print(cmd);
  unsigned int i = 0; //timeout counter
  int n = 1; // char counter
  char json[100] = "{";
  while (!Serial.find("\"main\":{")) {} // find the part we are interested in.
  while (i < 60000) {
    if (Serial.available()) {
      char c = Serial.read();
      json[n] = c;
      if (c == '}') break;
      n++;
      i = 0;
    }
    i++;
  }
  dbgSerial.println(json);
  //-//JsonObject root = parser.parse(json);
  //-//double temp = root["temp"];
  //-//double pressure = root["pressure"];
  //-//double humidity = root["humidity"];
  //-//temp -= 273.15; // from kelvin to degree celsius
  //-//dbgSerial.println(temp);
  //-//dbgSerial.println(pressure);
  //-//dbgSerial.println(humidity);
  //-//dbgSerial.println("====");
  delay(600000);
}

boolean connectWiFi()
{
  Serial.println("AT+CWMODE=1");
  String cmd = "AT+CWJAP=\"";
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";
  dbgSerial.println(cmd);
  Serial.println(cmd);
  delay(2000);
  if (Serial.find("OK")) {
    dbgSerial.println("OK, Connected to WiFi.");
    return true;
  } else {
    dbgSerial.println("Can not connect to the WiFi.");
    return false;
  }
}
