// this is the MAC Address of the remote ESP server which receives these sensor readings
uint8_t remoteMac[] = {0x5E, 0xCF, 0x7F, 0x2C, 0xD9, 0x90};

#define WIFI_CHANNEL 1
#define SEND_TIMEOUT 245  // 245 millis seconds timeout 

#define MESSAGELEN 10

struct __attribute__((packed)) COMMAND {
  char commandType;
  float payload;
} commandData;

volatile boolean callbackCalled;

void setupEspNow() {
  int i=0;
  WiFi.mode(WIFI_STA); // Station mode for esp-now sensor node
  WiFi.disconnect();

  Serial.printf("This mac: %s, ", WiFi.macAddress().c_str());
  Serial.printf("target mac: %02x%02x%02x%02x%02x%02x", remoteMac[0], remoteMac[1], remoteMac[2], remoteMac[3], remoteMac[4], remoteMac[5]);
  Serial.printf(", channel: %i\n", WIFI_CHANNEL);

  if (esp_now_init() != 0) {
    Serial.println("*** ESP_Now init failed");
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  int addStatus = esp_now_add_peer(remoteMac, ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL, NULL, 0);
  if (addStatus == 0) {
    // Pair success
    Serial.println("Pair success");
  } else {
    Serial.println("Pair failed");
  }

  esp_now_register_send_cb([](uint8_t* mac, uint8_t sendStatus) {
    Serial.printf("send_cb, send done, status = %i\n", sendStatus);
    callbackCalled = true;
  });
  callbackCalled = false;
}

void sendSignalToSlave(char commandType, float payload){
  commandData.commandType = commandType;
  commandData.payload = payload;
  uint8_t bs[sizeof(commandData)];
  memcpy(bs, &commandData, sizeof(commandData));
  esp_now_send(remoteMac, bs, sizeof(commandData));
  // NULL means send to all peers
  /*Serial.print("Time to send: ");
  Serial.print("Overall Time: ");
  Serial.println(millis() - entry1);*/
  //  Serial.print("Size: ");
  //  Serial.println(sizeof(bs));
  // ESP.deepSleep(0);
}
