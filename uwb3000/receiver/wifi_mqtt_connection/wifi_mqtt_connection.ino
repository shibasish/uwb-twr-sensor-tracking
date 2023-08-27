#include <WiFi.h>
#include <PubSubClient.h>

#define ANCHOR_ADD "86:17:5B:D5:A9:9A:E2:9C"

// connection pins
const uint8_t PIN_RST = 9; // reset pin
const uint8_t PIN_IRQ = 2; // irq pin
const uint8_t PIN_SS = SS; 

const char* ssid     = "Vodafone-E319";
const char* password = "3eDBcatn4AQ88qEh";
const char* mqtt_server = "192.168.0.80";

int btnGPIO = 0;
int btnState = false;

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
    Serial.begin(300);

    // SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    delay(10);

    // Set GPIO0 Boot button as input
    pinMode(btnGPIO, INPUT);

    // We start by connecting to a WiFi network
    // To debug, please enable Core Debug Level to Verbose

    Serial.println();
    Serial.print("[WiFi] Connecting to ");
    Serial.println(ssid);
    setup_wifi();

    
    Serial.println("Connecting to broker:");
    client.setServer(mqtt_server, 1883);
    Serial.println("Broker Connected");
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
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
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("my-topic", "hello from device");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  // DW1000Ranging.loop();
  client.loop();
  client.publish("my-topic", "{name: Hello}");
  delay(1000);
//    }
}

void newRange() {
  Serial.print("from: "); Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
  Serial.print("\t Range: "); Serial.print(DW1000Ranging.getDistantDevice()->getRange()); Serial.print(" m");
  Serial.print("\t RX power: "); Serial.print(DW1000Ranging.getDistantDevice()->getRXPower()); Serial.println(" dBm");
}

void newBlink(DW1000Device* device) {
  Serial.print("blink; 1 device added ! -> ");
  Serial.print(" short:");
  Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device* device) {
  Serial.print("delete inactive device: ");
  Serial.println(device->getShortAddress(), HEX);
}