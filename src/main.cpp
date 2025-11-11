#include <Arduino.h>

// Buzzer Configuration
#define BUZZER_PIN 7
#define BUZZER_CHANNEL 0      // LEDC channel for PWM
#define BUZZER_RESOLUTION 8   // 8-bit resolution (0-255)

// YOUR PREFERRED VOLUME (0-255)
// 180 is a great choice, as 255 (100%) can sometimes distort
#define SIREN_VOLUME 180 

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("--- Passive Buzzer Module Siren Test ---");
  Serial.println("Hardware: 3-pin Passive Buzzer Module (VCC, GND, I/O)");
  Serial.println("Pin: GPIO " + String(BUZZER_PIN));
  Serial.println("Method: Frequency sweep (Siren) using ledcSetup()");
  Serial.println("Volume: Set to " + String(SIREN_VOLUME) + "/255");

  // Initial setup for the LEDC channel.
  // The frequency will be changed inside the siren function.
  ledcSetup(BUZZER_CHANNEL, 2000, BUZZER_RESOLUTION);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
  
  // Turn buzzer off to start
  ledcWrite(BUZZER_CHANNEL, 0);
}

void playSiren() {
  Serial.println("🔊 Playing Siren Pattern...");
  
  // This loop repeats the "wail" 3 times
  for(int i = 0; i < 3; i++) {
    
    // Rising frequency (wail up)
    for(int freq = 1000; freq <= 3000; freq += 100) {
      ledcSetup(BUZZER_CHANNEL, freq, BUZZER_RESOLUTION);
      ledcWrite(BUZZER_CHANNEL, SIREN_VOLUME); // Set to your volume
      delay(10); // Short delay for each tone
    }

    // Falling frequency (wail down)
    for(int freq = 3000; freq >= 1000; freq -= 100) {
      ledcSetup(BUZZER_CHANNEL, freq, BUZZER_RESOLUTION);
      ledcWrite(BUZZER_CHANNEL, SIREN_VOLUME); // Set to your volume
      delay(10); // Short delay for each tone
    }
  }
  
  // Turn buzzer off when done
  ledcWrite(BUZZER_CHANNEL, 0);
}

void loop() {
  // Play the siren
  playSiren();
  
  // Wait 5 seconds before playing it again
  Serial.println("Waiting 5 seconds...");
  delay(5000);
}