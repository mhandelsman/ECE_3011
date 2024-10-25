#include "SDCall.h"

// Pin Definitions
#define SD_CS 6               // SD card chip select pin
#define SPI_MISO 19           // SPI MISO (DO)
#define SPI_MOSI 23           // SPI MOSI (DI)
#define SPI_SCK 18            // SPI Clock (SCK)

void test_func() {
    // Initialize SPI for SD card
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

    // Initialize the SD card
    if (!SD.begin(SD_CS)) {
        Serial.println("Card Mount Failed");
        return;
    }

    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        Serial.println("No SD card attached");
        return;
    }

    Serial.println("SD card initialized successfully.");
    
    // List files on the SD card (for testing purposes)
    File root = SD.open("/");
    File file = root.openNextFile();
    
    while (file) {
        Serial.print("File: ");
        Serial.print(file.name());
        Serial.print("\tSize: ");
        Serial.println(file.size());
        file = root.openNextFile();
    }
}
