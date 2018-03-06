#include <LWiFi.h>
#include <stdio.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

char* wifi_ssid = "TP-LINK_09ED";
char* wifi_password = "06188436";

char* mqtt_server = "172.17.28.48";
int mqtt_port = 21883;
char* mqtt_clientID = "ESP8266_1503489186";
char* mqtt_username = "7308624f-5630-48e6-9a82-26bb1754c5ff";
char* mqtt_password = "r:18962a95d258b34826bcc2812aada0ed";

char* mqtt_publish_topic = "qiot/things/admin/ESP8266/R1";
char* mqtt_subscribe_topic = "qiot/things/admin/ESP8266/S1";

WiFiClient linkitClient;
PubSubClient client(linkitClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqtt_clientID,mqtt_username,mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish("qiot/things/admin/ESP8266/led", "{\"value\":\"hallow\"}");
      // ... and resubscribe
      client.subscribe(mqtt_subscribe_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    //snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.print("hello world #");
    Serial.println(value);
    String val = "hallow";
    String msg_a = "{\"value\":\""+ val +"\"}";
    msg_a.toCharArray(msg, 50);
    client.publish(mqtt_publish_topic, msg);
  }
}
