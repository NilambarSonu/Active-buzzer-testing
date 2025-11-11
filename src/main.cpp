#include <Arduino.h>

// --- Configuration ---
#define BUZZER_PIN 7
#define BUZZER_CHANNEL 0
#define BUZZER_RESOLUTION 8
#define SUCCESS_VOLUME 180
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_E5 659
#define NOTE_G5 784
#define NOTE_C6 1047
void playNote(int frequency, int duration) {
  // 1. Set the frequency (pitch)
  ledcSetup(BUZZER_CHANNEL, frequency, BUZZER_RESOLUTION);
  // 2. Play the note at the set volume
  ledcWrite(BUZZER_CHANNEL, SUCCESS_VOLUME);
  // 3. Hold the note
  delay(duration);
  // 4. Stop the sound
  ledcWrite(BUZZER_CHANNEL, 0);
}

// Pattern 1: Fast Chirp (bip-BEEP)
void playSuccess1_Chirp() {
  Serial.println("Playing 1: Chirp");
  playNote(NOTE_G4, 100);
  delay(50);
  playNote(NOTE_C5, 150);
}

// Pattern 2: Simple Rise (C-E-G)
void playSuccess2_SimpleRise() {
  Serial.println("Playing 2: Simple Rise");
  playNote(NOTE_C4, 150);
  delay(50);
  playNote(NOTE_E4, 150);
  delay(50);
  playNote(NOTE_G4, 150);
}

// Pattern 3: Digital Trill (Fast back-and-forth)
void playSuccess3_Trill() {
  Serial.println("Playing 3: Trill");
  for (int i = 0; i < 3; i++) {
    playNote(NOTE_G5, 50);
    delay(25);
    playNote(NOTE_A4, 50);
    delay(25);
  }
}

// Pattern 4: Long Rise (C-E-G-C5)
void playSuccess4_LongRise() {
  Serial.println("Playing 4: Long Rise");
  playNote(NOTE_C4, 120);
  delay(40);
  playNote(NOTE_E4, 120);
  delay(40);
  playNote(NOTE_G4, 120);
  delay(40);
  playNote(NOTE_C5, 200);
}

// Pattern 5: Double Beep (OK / Acknowledge)
void playSuccess5_DoubleBeep() {
  Serial.println("Playing 5: Double Beep");
  playNote(NOTE_C5, 100);
  delay(80);
  playNote(NOTE_C5, 100);
}

// Pattern 6: Short & Sharp (High pitched "Done!")
void playSuccess6_ShortSharp() {
  Serial.println("Playing 6: Short & Sharp");
  playNote(NOTE_C6, 75);
}

// Pattern 7: High-Low (Affirmative)
void playSuccess7_HighLow() {
  Serial.println("Playing 7: High-Low");
  playNote(NOTE_C5, 150);
  delay(50);
  playNote(NOTE_G4, 200);
}

// Pattern 8: Fast 3-Note Scale
void playSuccess8_FastScale() {
  Serial.println("Playing 8: Fast Scale");
  playNote(NOTE_C5, 100);
  delay(10); // No delay for fast sound
  playNote(NOTE_D4, 100);
  delay(10);
  playNote(NOTE_E4, 100);
}

// Pattern 9: Notification (Ding-Dong style)
void playSuccess9_Notification() {
  Serial.println("Playing 9: Notification");
  playNote(NOTE_E5, 150);
  delay(50);
  playNote(NOTE_C5, 150);
}

// Pattern 10: Simple Fanfare
void playSuccess10_Fanfare() {
  Serial.println("Playing 10: Fanfare");
  playNote(NOTE_G4, 100);
  delay(50);
  playNote(NOTE_C5, 100);
  delay(50);
  playNote(NOTE_E5, 200);
}
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("--- Buzzer Test Bench ---");
  Serial.println("Uncomment a pattern in loop() to test it.");
  
  // Attach the pin to the LEDC channel
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
}
void loop() {
  playSuccess1_Chirp();
  // playSuccess2_SimpleRise();
  // playSuccess3_Trill();
  // playSuccess4_LongRise();
  // playSuccess5_DoubleBeep();
  // playSuccess6_ShortSharp();
  // playSuccess7_HighLow();
  // playSuccess8_FastScale();
  // playSuccess9_Notification();
  // playSuccess10_Fanfare();
  Serial.println("Waiting 3 seconds...");
  delay(3000);
}