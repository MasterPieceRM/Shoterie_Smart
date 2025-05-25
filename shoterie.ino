#include <SPI.h>
#include <MFRC522.h>

// pini RC522
#define RST_PIN   22 
#define SS_PIN    21 

// pinul de comandă (merge pe baza TIP41C prin 1k)
const int CMD_PIN = 5;

// UID-ul “valid” (4 octeți, în HEX)
byte validUID[4] = { 0xDE, 0xAD, 0xBE, 0xEF };

// obiecte SPI + MFRC522
MFRC522 mfrc522(SS_PIN, RST_PIN);

bool isValidTag() {
  if (mfrc522.uid.size != sizeof(validUID)) return false;
  for (byte i = 0; i < sizeof(validUID); i++) {
    if (mfrc522.uid.uidByte[i] != validUID[i]) return false;
  }
  return true;
}

void setup() {
  Serial.begin(115200);
  SPI.begin();            
  mfrc522.PCD_Init();     
  Serial.println("Scanati un card RFID...");

  pinMode(CMD_PIN, OUTPUT);
  digitalWrite(CMD_PIN, LOW);
}

void loop() {
  // dacă nu e niciun tag nou, ieșim
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // afișăm UID
  Serial.print("UID tag :");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  Serial.println("Tag valid — porneste pompa 5s");
  digitalWrite(CMD_PIN, HIGH);   // activează baza tranzistorului → pompa pornește
  delay(5000);                   // așteaptă 2 secunde
  digitalWrite(CMD_PIN, LOW);    // oprește pompa
  Serial.println("Pompa oprita");


  // terminate session
  mfrc522.PICC_HaltA();
  // opțional: mic delay ca să nu “citească” de mai multe ori
  delay(500);
}
