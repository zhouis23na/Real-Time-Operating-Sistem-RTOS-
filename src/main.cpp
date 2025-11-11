#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

// ========== OLED ==========
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define I2C_ADDRESS   0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ========== LED ==========
#define LED1 4
#define LED2 5
#define LED3 8

// ========== PUSH BUTTON ==========
#define LED4 13
#define LED5 16
#define BUTTON 17
bool state = false;
TaskHandle_t TaskButton;

// ========== BUZZER ==========
#define BUZZER_PIN 9

// ========== POTENSIO ==========
#define POT_PIN 1

// ========== SERVO ==========
#define SERVO_PIN 10
Servo myServo;

// ========== STEPPER (A4988) ==========
#define STEP_PIN 18
#define DIR_PIN 19
// ENABLE pin A4988 dihubungkan ke GND

// ========== ENCODER ==========
#define ENCODER_PIN_A 36
#define ENCODER_PIN_B 37
volatile long encoderPos = 0;
int lastEncoded = 0;

// ============================
// ===== TASK DEFINITIONS =====
// ============================

// Task 1: OLED Display (Core 0)
void TaskOLED(void *pvParameters) {
  Wire.begin(7, 6); // SDA, SCL
  if (!display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS)) {
    Serial.println("OLED tidak terdeteksi!");
    vTaskDelete(NULL);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 25);
  display.println("Hello");
  display.setCursor(10, 45);
  display.println("World!!!");
  display.display();

  while (true) {
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Task 2: Blink LED (Core 0)
void TaskBlink(void *pvParameters) {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  while (true) {
    digitalWrite(LED1, HIGH); vTaskDelay(300 / portTICK_PERIOD_MS);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH); vTaskDelay(300 / portTICK_PERIOD_MS);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH); vTaskDelay(300 / portTICK_PERIOD_MS);
    digitalWrite(LED3, LOW);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

// Task 3: PushButton (Core 1)
void TaskPushButton(void *pvParameters) {
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  while (true) {
    if (digitalRead(BUTTON) == LOW) {
      state = !state;
      digitalWrite(LED4, state);
      digitalWrite(LED5, !state);
      vTaskDelay(300 / portTICK_PERIOD_MS);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

// Task 4: Buzzer (Core 0)
void TaskBuzzer(void *pvParameters) {
  pinMode(BUZZER_PIN, OUTPUT);
  while (true) {
    tone(BUZZER_PIN, 1000);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    tone(BUZZER_PIN, 1500);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    noTone(BUZZER_PIN);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Task 5: Servo (Core 0)
void TaskServo(void *pvParameters) {
  myServo.attach(SERVO_PIN);
  while (true) {
    for (int pos = 0; pos <= 90; pos += 5) {
      myServo.write(pos);
      vTaskDelay(20 / portTICK_PERIOD_MS);
    }
  }
}

// Task 6: Stepper (Core 0)
void TaskStepper(void *pvParameters) {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  while (true) {
    digitalWrite(DIR_PIN, HIGH); // searah jarum jam
    for (int i = 0; i < 200; i++) {
      digitalWrite(STEP_PIN, HIGH);
      vTaskDelay(2 / portTICK_PERIOD_MS);
      digitalWrite(STEP_PIN, LOW);
      vTaskDelay(2 / portTICK_PERIOD_MS);
    }

    vTaskDelay(500 / portTICK_PERIOD_MS);

    digitalWrite(DIR_PIN, LOW); // berlawanan
    for (int i = 0; i < 200; i++) {
      digitalWrite(STEP_PIN, HIGH);
      vTaskDelay(2 / portTICK_PERIOD_MS);
      digitalWrite(STEP_PIN, LOW);
      vTaskDelay(2 / portTICK_PERIOD_MS);
    }

    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Task 7: Potensio (Core 1)
void TaskPotensio(void *pvParameters) {
  pinMode(POT_PIN, INPUT);
  while (true) {
    int nilai = analogRead(POT_PIN);
    float volt = (nilai / 4095.0) * 3.3;  // ubah ke voltase (0–3.3V)
    Serial.print("Nilai ADC: ");
    Serial.print(nilai);
    Serial.print(" | Tegangan: ");
    Serial.print(volt, 2);
    Serial.println(" V");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Task 8: Encoder (Core 1)
void TaskEncoder(void *pvParameters) {
  while (true) {
    Serial.print("Encoder Position: ");
    Serial.println(encoderPos);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}
// Encoder ISR
void IRAM_ATTR updateEncoder() {
  int MSB = digitalRead(ENCODER_PIN_A);
  int LSB = digitalRead(ENCODER_PIN_B);
  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded << 2) | encoded;

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderPos++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderPos--;

  lastEncoded = encoded;
}

// ============================
// ===== SETUP & LOOP =========
// ============================
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Memulai sistem multitasking ESP32-S3...");

  // Encoder setup
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), updateEncoder, CHANGE);

  // Jalankan semua task
  xTaskCreatePinnedToCore(TaskOLED, "TaskOLED", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskBlink, "TaskBlink", 2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskPushButton, "TaskButton", 4096, NULL, 1, &TaskButton, 1);
  xTaskCreatePinnedToCore(TaskBuzzer, "TaskBuzzer", 2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskServo, "TaskServo", 2048, NULL, 2, NULL, 0);
  xTaskCreatePinnedToCore(TaskStepper, "TaskStepper", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskPotensio, "TaskPotensio", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskEncoder, "TaskEncoder", 2048, NULL, 1, NULL, 1);
}

void loop() {
  Serial.print("Encoder Position: ");
  Serial.println(encoderPos);
  delay(1000);
}
