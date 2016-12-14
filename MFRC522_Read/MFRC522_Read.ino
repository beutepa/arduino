/**
  ----------------------------------------------------------------------------
  This is a MFRC522 library example; see https://github.com/miguelbalboa/rfid
  for further details and other examples.

  NOTE: The library file MFRC522.h has a lot of useful info. Please read it.

  Released into the public domain.
  ----------------------------------------------------------------------------
  This sample shows how to read blocks on a MIFARE Classic PICC
  (= card/tag).

  Typical pin layout used:
  -----------------------------------------------------------------------------------------
  MFRC522 Arduino Arduino Arduino Arduino Arduino
  Reader/PCD Uno Mega Nano v3 Leonardo/Micro Pro Micro
  Signal Pin Pin Pin Pin Pin Pin
  -----------------------------------------------------------------------------------------
  RST/Reset RST 9 5 D9 RESET/ICSP-5 RST
  SPI SS SDA(SS) 10 53 D10 10 10
  SPI MOSI MOSI 11 / ICSP-4 51 D11 ICSP-4 16
  SPI MISO MISO 12 / ICSP-1 50 D12 ICSP-1 14
  SPI SCK SCK 13 / ICSP-3 52 D13 ICSP-3 15

*/
#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN 9 // Configurable, see typical pin layout above
#define SS_PIN 10 // Configurable, see typical pin layout above

#define GREEN_LED 2
#define RED_LED 3

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
MFRC522::MIFARE_Key key;

/**
  Initialize.
*/
void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  Serial.begin(9600); // Initialize serial communications with the PC
  while (!Serial); // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  // Prepare the key (used both as key A and as key B)
  // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  Serial.println(F("Scan a MIFARE Classic PICC to demonstrate read."));
  Serial.print(F("Using key (for A and B):"));
  dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
  Serial.println();
}
/**
  Main loop.
*/
void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;
  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
  Serial.print(F("PICC type: "));
  byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
  // Check for compatibility
  if ( piccType != MFRC522::PICC_TYPE_MIFARE_MINI
       && piccType != MFRC522::PICC_TYPE_MIFARE_1K
       && piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("This sample only works with MIFARE Classic cards."));
    return;
  }
  // In this sample we use the second sector,
  // that is: sector #1, covering block #4 up to and including block #7

  // In this sample we are going to read sectors 0 to 4
  // Sector 0 (block 0 - 3)
  // Sector 1 (block 4 - 7)
  // Sector 2 (block 8 - 11)
  // Sector 3 (block 12 - 15)
  // Sector 4 (block 16 - 19)

  byte sector = 1;
  byte blockAddr = 4;

  byte trailerBlock = 7;
  byte status;
  byte buffer[18];
  byte size = sizeof(buffer);
  // Authenticate using key A
  Serial.println(F("Authenticating using key A..."));
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Read data from the block
  Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
  Serial.println(F(" ..."));
  status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }
  Serial.println("byte array:");
  dump_byte_array(buffer, 16); Serial.println();
  Serial.println("characters");
  dump_byte_array_as_characters(buffer, 16); Serial.println();
  Serial.println();

  // Whole sector:
  // Show the whole sector as it currently is
  Serial.println(F("Current data in sector:"));
  mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
  Serial.println();

  // Halt PICC
  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
}
/**
  Helper routine to dump a byte array as hex values to Serial.
*/
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
  digitalWrite(RED_LED, HIGH);
  delay(500);
  digitalWrite(RED_LED, LOW);
}

void dump_byte_array_as_characters(byte *buffer, byte bufferSize) {
  Serial.write((char *)buffer);
  digitalWrite(GREEN_LED, HIGH);
  delay(500);
  digitalWrite(GREEN_LED, LOW);
}
