#include <LiquidCrystal_I2C.h>
#define HRS_ANALOG A1
#define HRS_DIGITAL 2
#define BUZZER_PIN 8
#define BT_ANALOG A0
#define BT_DIGITAL 2
#define OL_ANALOG A2
#define OL_DIGITAL 2
#define BR_ANALOG A3
#define BR_DIGITAL 2



int bpm = 120;
unsigned long interval;  // Interval between buzzes in milliseconds
unsigned long previousMillis = 0;
bool buzzerState = false;

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(HRS_ANALOG, INPUT);
  pinMode(HRS_DIGITAL, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(BT_ANALOG, INPUT);
  pinMode(BT_DIGITAL, INPUT);

  pinMode(OL_ANALOG, INPUT);
  pinMode(OL_DIGITAL, INPUT);

  pinMode(BR_ANALOG, INPUT);
  pinMode(BR_DIGITAL, INPUT);
  


  lcd.init();
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
  int HRS_ANALOG_X = analogRead(HRS_ANALOG);
  int HRS_scaledValue = map(HRS_ANALOG_X, 0, 1023, 0, 200);

  int BT_ANALOG_X = analogRead(BT_ANALOG);  // Read analog input (0 to 1023)
  float BT_scaledValue = (BT_ANALOG_X / 1023.0) * (45.0 - 20.0) + 20.0;

  int OL_ANALOG_X = analogRead(OL_ANALOG);
  int OL_scaledValue = map(OL_ANALOG_X, 0, 1023, 0, 100);

  int BR_ANALOG_X = analogRead(BR_ANALOG);
  int BR_scaledValue = map(BR_ANALOG_X, 0, 1023, 0, 30);
  

  lcd.setCursor(0, 0);
  lcd.print("Pulso: ");
  lcd.print(String(HRS_scaledValue)+" BPM");
  delay(5);

  lcd.setCursor(0, 1);
  lcd.print("Temperatura: ");
  lcd.print(String(BT_scaledValue)+" C");
  delay(5);

  lcd.setCursor(0, 2);
  lcd.print("Oxigeno: ");
  lcd.print(String(OL_scaledValue)+" %");
  delay(5);

  lcd.setCursor(0, 3);
  lcd.print("Respiracion: ");
  lcd.print(String(BR_scaledValue)+" brPM");
  delay(5);


  setBPM(HRS_scaledValue);

  unsigned long currentMillis = millis();
  if (bpm == 0) {
    // Continuous tone if BPM is 0
    tone(BUZZER_PIN, 1000);
  } else if (bpm < 60 || bpm > 120) {
    // Faster tone for BPM outside 60-120 range
    interval = 300;  // Set interval for faster tone (e.g., 300ms)
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      toggleBuzzer();
    }
  } else {
    // Normal BPM-based interval
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      toggleBuzzer();
    }
  }

}

void setBPM(int newBPM) {
  bpm = newBPM;
  if (bpm > 0) {
    interval = 60000 / bpm;  // Convert BPM to interval in milliseconds
  } else {
    interval = 0;  // Continuous tone, no interval
  }
  noTone(BUZZER_PIN);  // Ensure buzzer is off initially
  buzzerState = false;  // Reset buzzer state
  //Serial.print("BPM set to: ");
  //Serial.println(bpm);
  //Serial.print("Interval set to: ");
  //Serial.println(interval);
}

void toggleBuzzer() {
  if (buzzerState) {
    noTone(BUZZER_PIN);  // Turn the buzzer off
  } else {
    tone(BUZZER_PIN, 1000, 100);  // Turn the buzzer on with a frequency of 1000 Hz
  }
  buzzerState = !buzzerState;
}
