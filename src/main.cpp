#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

// SD Card Configuration (SPI)
#define SD_CS 10
#define SD_MOSI 11
#define SD_MISO 13
#define SD_SCK 12

// Buzzer Configuration
#define BUZZER_PIN 7
#define BUZZER_CHANNEL 0  // LEDC channel for PWM
#define BUZZER_FREQUENCY 2000  // 2kHz - good frequency for buzzers
#define BUZZER_RESOLUTION 8    // 8-bit resolution (0-255)

void setupBuzzer() {
  Serial.println("🔊 Initializing Buzzer...");
  Serial.println("   Pin: GPIO" + String(BUZZER_PIN));
  Serial.println("   Type: Active Buzzer (3.3-5V)");
  Serial.println("   Module: Robocraze Active Buzzer");
  
  // Configure LEDC for buzzer PWM
  ledcSetup(BUZZER_CHANNEL, BUZZER_FREQUENCY, BUZZER_RESOLUTION);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
  
  Serial.println("✅ Buzzer initialized with PWM");
}

void testBuzzerVolume() {
  Serial.println("\n🎵 Testing Buzzer Volume Levels");
  Serial.println("===============================");
  
  // Test 1: Simple digital on/off (might be quiet)
  Serial.println("1. Testing Digital ON/OFF...");
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);
  delay(500);
  
  // Test 2: PWM at 50% duty cycle
  Serial.println("2. Testing PWM 50% duty cycle...");
  ledcWrite(BUZZER_CHANNEL, 128);  // 50% of 255
  delay(1000);
  ledcWrite(BUZZER_CHANNEL, 0);
  delay(500);
  
  // Test 3: PWM at 100% duty cycle (maximum volume)
  Serial.println("3. Testing PWM 100% duty cycle...");
  ledcWrite(BUZZER_CHANNEL, 255);  // 100% duty cycle
  delay(1000);
  ledcWrite(BUZZER_CHANNEL, 0);
  delay(500);
  
  // Test 4: Different frequencies
  Serial.println("4. Testing different frequencies...");
  
  int frequencies[] = {1000, 1500, 2000, 2500, 3000, 4000};
  for(int i = 0; i < 6; i++) {
    Serial.println("   Frequency: " + String(frequencies[i]) + "Hz");
    ledcSetup(BUZZER_CHANNEL, frequencies[i], BUZZER_RESOLUTION);
    ledcWrite(BUZZER_CHANNEL, 255);
    delay(300);
    ledcWrite(BUZZER_CHANNEL, 0);
    delay(200);
  }
  
  // Reset to optimal frequency
  ledcSetup(BUZZER_CHANNEL, BUZZER_FREQUENCY, BUZZER_RESOLUTION);
}

void playLoudBeep(int duration = 500) {
  // Use PWM at 100% duty cycle for maximum volume
  ledcWrite(BUZZER_CHANNEL, 255);
  delay(duration);
  ledcWrite(BUZZER_CHANNEL, 0);
}

void playBeepPattern(int pattern) {
  switch(pattern) {
    case 1: // Short beep
      Serial.println("🔊 Short beep");
      playLoudBeep(200);
      break;
      
    case 2: // Double beep
      Serial.println("🔊 Double beep");
      playLoudBeep(100);
      delay(100);
      playLoudBeep(100);
      break;
      
    case 3: // Long beep
      Serial.println("🔊 Long beep");
      playLoudBeep(1000);
      break;
      
    case 4: // Siren pattern
      Serial.println("🔊 Siren pattern");
      for(int i = 0; i < 3; i++) {
        // Rising frequency
        for(int freq = 1000; freq <= 3000; freq += 100) {
          ledcSetup(BUZZER_CHANNEL, freq, BUZZER_RESOLUTION);
          ledcWrite(BUZZER_CHANNEL, 255);
          delay(10);
        }
        ledcWrite(BUZZER_CHANNEL, 0);
        delay(100);
      }
      // Reset to default frequency
      ledcSetup(BUZZER_CHANNEL, BUZZER_FREQUENCY, BUZZER_RESOLUTION);
      break;
      
    case 5: // Success melody
      Serial.println("🔊 Success melody");
      int melody[] = {262, 330, 392, 523}; // C, E, G, C
      for(int note = 0; note < 4; note++) {
        ledcSetup(BUZZER_CHANNEL, melody[note], BUZZER_RESOLUTION);
        ledcWrite(BUZZER_CHANNEL, 255);
        delay(150);
        ledcWrite(BUZZER_CHANNEL, 0);
        delay(50);
      }
      ledcSetup(BUZZER_CHANNEL, BUZZER_FREQUENCY, BUZZER_RESOLUTION);
      break;
  }
}

void testBuzzerWithSD() {
  Serial.println("\n📁 Testing Buzzer with SD Card Operations");
  Serial.println("========================================");
  
  // Initialize SD Card
  Serial.println("💾 Initializing SD Card...");
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  
  if (!SD.begin(SD_CS, SPI, 1000000)) {
    Serial.println("❌ SD Card initialization failed!");
    playBeepPattern(2); // Error pattern
    return;
  }
  
  playBeepPattern(1); // Success beep
  Serial.println("✅ SD Card initialized successfully!");
  
  // Create test file with buzzer feedback
  Serial.println("📄 Creating test file...");
  File testFile = SD.open("/buzzer_test.txt", FILE_WRITE);
  if (testFile) {
    testFile.println("Buzzer Volume Test - " + String(millis()));
    testFile.close();
    playBeepPattern(5); // Success melody
    Serial.println("✅ Test file created successfully!");
  } else {
    playBeepPattern(2); // Error pattern
    Serial.println("❌ Failed to create test file");
  }
  
  // List files with buzzer feedback
  Serial.println("📂 Listing files...");
  File root = SD.open("/");
  File file = root.openNextFile();
  int fileCount = 0;
  
  while (file) {
    if (!file.isDirectory()) {
      fileCount++;
      Serial.println("   📄 " + String(file.name()) + " (" + String(file.size()) + " bytes)");
      playLoudBeep(50); // Short beep for each file
      delay(100);
    }
    file = root.openNextFile();
  }
  root.close();
  
  Serial.println("📊 Total files: " + String(fileCount));
  playBeepPattern(3); // Long beep for completion
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("🔊 Robocraze Active Buzzer - Volume Fix Test");
  Serial.println("===========================================");
  Serial.println("Problem: Low sound from active buzzer");
  Serial.println("Solution: Using PWM for maximum volume");
  Serial.println("Pin: GPIO" + String(BUZZER_PIN));
  Serial.println();
  
  // Initialize buzzer with PWM
  setupBuzzer();
  delay(1000);
  
  // Test different volume levels
  testBuzzerVolume();
  delay(1000);
  
  // Test with SD card operations
  testBuzzerWithSD();
  delay(1000);
  
  Serial.println("\n🎉 Buzzer Volume Test Complete!");
  Serial.println("💡 If sound is still low, check:");
  Serial.println("   - Buzzer VCC connected to 5V (not 3.3V)");
  Serial.println("   - Proper ground connection");
  Serial.println("   - Buzzer is active type (not passive)");
  Serial.println("   - No series resistor limiting current");
}

void loop() {
  // Demonstrate different beep patterns
  Serial.println("\n🔄 Demo Mode - Different Beep Patterns");
  
  for(int pattern = 1; pattern <= 5; pattern++) {
    Serial.println("Playing pattern " + String(pattern) + "...");
    playBeepPattern(pattern);
    delay(2000);
  }
  
  // Volume level indicator
  Serial.println("\n🔊 Volume Level Indicator");
  for(int volume = 50; volume <= 255; volume += 50) {
    Serial.println("Volume: " + String(volume) + "/255");
    ledcWrite(BUZZER_CHANNEL, volume);
    delay(500);
    ledcWrite(BUZZER_CHANNEL, 0);
    delay(300);
  }
  
  // Reset to maximum volume
  ledcWrite(BUZZER_CHANNEL, 255);
  delay(3000);
  ledcWrite(BUZZER_CHANNEL, 0);
  
  delay(5000);
}