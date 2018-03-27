#include "RTClib.h"

#define GREEN_LED_PIN 2
#define RED_LED_PIN 3
#define BLUE_LED_PIN 4

RTC_DS3231 rtc;

// March 28 is a Monday of non paper week.
const DateTime MARCH_26_2018 = DateTime(2018, 3, 26);
const TimeSpan ONE_DAY = TimeSpan(1, 0, 0, 0);
const int TEN_MINUTES = 600000;

enum Trash {
  PAPER, PLASTIC, DUO, NONE
};

void setup() {
//  Serial.begin(9600);
  
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

  led_off();
  
  if (!rtc.begin()) {
    led_red();
    while (1);
  }

//  adjust_rtc_time();
}

void loop() {
  DateTime today = rtc.now();
  DateTime tomorrow = today + ONE_DAY;

  Trash trash = get_trash(tomorrow);
  
  if (trash != NONE && is_evening(today)) {
    led_on(trash);
  } else {
    led_off();
  }

  delay(TEN_MINUTES);
}

Trash get_trash(DateTime date) {
  bool paper_week = is_paper_week(date);
  
  if (date.dayOfTheWeek() == 1 && paper_week) {
      return PAPER;
  }

  if (date.dayOfTheWeek() == 4) {
    return paper_week ? DUO : PLASTIC;
  }

  return NONE;
}

bool is_paper_week(DateTime date) {
  TimeSpan time_sinse_march_26 = date - MARCH_26_2018;
  int days_sinse_march_26 = time_sinse_march_26.days();
  
  int remainder = days_sinse_march_26 % 14;
  
  return remainder >= 7;
}

void led_on(Trash trash) {
  switch (trash) {
    case PAPER:
      led_red();
      break;

    case PLASTIC:
      led_blue();
      break;

    case DUO:
      led_green();
      break;

    default:
      led_off();
      break;
  }
}

void led_red() {
  digitalWrite(RED_LED_PIN, 1);
  digitalWrite(GREEN_LED_PIN, 0);
  digitalWrite(BLUE_LED_PIN, 0);
}

void led_green() {
  digitalWrite(RED_LED_PIN, 0);
  digitalWrite(GREEN_LED_PIN, 1);
  digitalWrite(BLUE_LED_PIN, 0);
}

void led_blue() {
  digitalWrite(RED_LED_PIN, 0);
  digitalWrite(GREEN_LED_PIN, 0);
  digitalWrite(BLUE_LED_PIN, 1);
}

void led_off() {
  digitalWrite(RED_LED_PIN, 0);
  digitalWrite(GREEN_LED_PIN, 0);
  digitalWrite(BLUE_LED_PIN, 0);
}

bool is_evening(DateTime time) {
  return time.hour() >= 16;
}

void adjust_rtc_time() {
    // Sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    
    // January 21, 2014 at 3am
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}

