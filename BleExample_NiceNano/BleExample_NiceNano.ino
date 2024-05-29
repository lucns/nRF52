#include <bluefruit.h>
#include <Retard.h>

#define RED_LED 24

BLEUart bleuart;
Retard everyOne(INTERVAL_1S);

void setup() {
  Serial.begin(9600);
  while (/*!Serial || */millis() < 100); // !Serial freezes while usb port is open

  pinMode(RED_LED, OUTPUT); // Red led on pin 24
  digitalWrite(RED_LED, HIGH);

  Bluefruit.autoConnLed(true);
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
  Bluefruit.begin();
  Bluefruit.setTxPower(8); // power in dbm
  Bluefruit.setName("BLE Lucas");
  Bluefruit.Periph.setConnectCallback(onConnected);
  Bluefruit.Periph.setDisconnectCallback(onDisconnect);

  bleuart.begin();
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(bleuart);
  Bluefruit.ScanResponse.addName();
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244); // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30); // seconds
  Bluefruit.Advertising.start(0);

  everyOne.cancel();
  Serial.println("Ready");
}

void loop() {
  if (everyOne.gate()) {
    everyOne.reset();
    String s = String(millis());
    bleuart.write(s.c_str(), s.length());
  }

  if (bleuart.available()) {
    Serial.print("Received:");
    while (bleuart.available()) {
      char c = (char) bleuart.read();
      Serial.print(c);
      if (c == 'a') digitalToggle(RED_LED);
    }
    Serial.println();
  }
}

void onConnected(uint16_t nameLength) {
  everyOne.reset();
  char name[32] = {0};
  BLEConnection* connection = Bluefruit.Connection(nameLength);
  connection->getPeerName(name, sizeof(name));
  Serial.print("Connected to ");
  Serial.print(name);
  Serial.println("!");
}

void onDisconnect(uint16_t nameLength, uint8_t reason) {
  Serial.print("Disonnected!");
  everyOne.cancel();
}
