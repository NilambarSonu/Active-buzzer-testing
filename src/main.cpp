#include <Arduino.h>

// Buzzer Configuration
#define BUZZER_PIN 7
#define BUZZER_CHANNEL 0      // LEDC channel for PWM
#define BUZZER_RESOLUTION 8   // 8-bit resolution (0-255)

// Set volume for the "success" sound (0-255)
// 200 is loud but clear
#define SUCCESS_VOLUME 200

// Define the notes in the melody (C, E, G, High C)
int melody[] = {262, 330, 392, 523};

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("--- Buzzer 'Success Sound' Test ---");
  Serial.println("Hardware: 3-pin Passive Buzzer Module");
  Serial.println("Pin: GPIO " + String(BUZZER_PIN));
  Serial.println("Volume: " + String(SUCCESS_VOLUME));

  // Attach the pin to the LEDC channel
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
}

void playSuccessSound() {
  Serial.println("🔊 Playing Success Sound!");
  
  for (int thisNote = 0; thisNote < 4; thisNote++) {
    // 1. Set the frequency for the current note
    ledcSetup(BUZZER_CHANNEL, melody[thisNote], BUZZER_RESOLUTION);
    
    // 2. Play the note at the set volume
    ledcWrite(BUZZER_CHANNEL, SUCCESS_VOLUME);
    
    // 3. Hold the note
    delay(150);
    
    // 4. Stop the sound (silence between notes)
    ledcWrite(BUZZER_CHANNEL, 0);
    delay(50);
  }
}

void loop() {
  // Play the success sound
  playSuccessSound();
  
  // Wait 3 seconds before playing it again
  Serial.println("Waiting 3 seconds...");
  delay(3000);
}