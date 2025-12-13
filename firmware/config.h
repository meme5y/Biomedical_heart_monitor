// config.h
// Configuration settings for heart rate monitor

// Pin definitions
#define IR_SENSOR_PIN 2
#define BUZZER_PIN 8
#define LED_PIN 7

// Timing constants
#define READING_INTERVAL 10000  // 10 seconds
#define REFRACTORY_PERIOD 300   // 300ms
#define LED_TIMEOUT 150         // 150ms

// Calculation constants
#define BPM_MULTIPLIER 6        // 10s → BPM conversion
