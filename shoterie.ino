#include <SPI.h>
#include <MFRC522.h>

// pini RC522
#define RST_PIN   22 
#define SS_PIN    21 

const int CMD_PIN1 = 5;
const int CMD_PIN2 = 2;
const int CMD_PIN3 = 4;

byte uid1[7] = { 0xFF, 0x0F, 0x45, 0x58, 0x21, 0x00, 0x00 };
byte uid2[7] = { 0xFF, 0x0F, 0x00, 0xA4, 0x21, 0x00, 0x00 };
byte uid3[7] = { 0xFF, 0x0F, 0xA9, 0x44, 0x21, 0x00, 0x00 };

MFRC522 mfrc522(SS_PIN, RST_PIN);

bool compareUID(byte *uidA, byte *uidB, byte length) {
  for (byte i = 0; i < length; i++) {
    if (uidA[i] != uidB[i]) return false;
  }
  return true;
}

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Scanati un card RFID...");

  pinMode(CMD_PIN1, OUTPUT);
  pinMode(CMD_PIN2, OUTPUT);
  pinMode(CMD_PIN3, OUTPUT);

  digitalWrite(CMD_PIN1, LOW);
  digitalWrite(CMD_PIN2, LOW);
  digitalWrite(CMD_PIN3, LOW);
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("UID tag: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  if (compareUID(mfrc522.uid.uidByte, uid1, 7)) {
    Serial.println("Tag 1 recunoscut - activare pompa 1");
    digitalWrite(CMD_PIN1, HIGH);
    delay(3000);
    digitalWrite(CMD_PIN1, LOW);
    Serial.println("Pompa 1 oprita");
  } else if (compareUID(mfrc522.uid.uidByte, uid2, 7)) {
    Serial.println("Tag 2 recunoscut - activare pompa 2");
    digitalWrite(CMD_PIN2, HIGH);
    delay(3000);
    digitalWrite(CMD_PIN2, LOW);
    Serial.println("Pompa 2 oprita");
  } else if (compareUID(mfrc522.uid.uidByte, uid3, 7)) {
    Serial.println("Tag 3 recunoscut - activare pompa 3");
    digitalWrite(CMD_PIN3, HIGH);
    delay(3000);
    digitalWrite(CMD_PIN3, LOW);
    Serial.println("Pompa 3 oprita");
  } else {
    Serial.println("Tag necunoscut.");
  }

  mfrc522.PICC_HaltA();
  delay(500);
}
