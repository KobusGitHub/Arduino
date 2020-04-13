
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "DO_F62B";
const char* password = "jonnyb55";

const char* host = "maker.ifttt.com";
const int httpsPort = 443;
const int API_TIMEOUT = 10000;  //keep it long if you want to receive headers from client

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // WiFiClientSecure client;
  BearSSL::WiFiClientSecure client;

  client.setInsecure();
  client.setTimeout(API_TIMEOUT);

  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

 

  String url = "/trigger/esppush/with/key/hKz5fItk_n_tyrrV7W5mystovzU3-tXUhwLJd1hxLe2";
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');

  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
}

void loop() {
}
