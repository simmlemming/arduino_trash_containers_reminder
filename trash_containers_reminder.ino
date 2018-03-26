#include "RTClib.h"

#define GREEN_LED_PIN 2
#define RED_LED_PIN 3
#define BLUE_LED_PIN 4

RTC_DS3231 rtc;
DateTime mar26 = DateTime(2018, 3, 26);
int days_in_month[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

enum Trash {
  PAPER, PLASTIC, DUO, NONE
};

void setup() {
  Serial.begin(9600);
  
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

  digitalWrite(RED_LED_PIN, 0);
  digitalWrite(GREEN_LED_PIN, 0);
  digitalWrite(BLUE_LED_PIN, 0);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  adjust_rtc_time();

//  DateTime now = rtc.now();
//  Serial.print(now.hour());
//  Serial.print(":");
//  Serial.println(now.minute());

//  Serial.print("month = ");
//  Serial.println(now.month());
  //Serial.println(now.dayOfTheWeek());

  DateTime then = DateTime(2019, 10, 24);
//  bool is_papier = is_paper_week(then);
//  Serial.print("is paper = ");
//  Serial.println(is_papier);
  Serial.print("trash = ");
  Serial.println(get_trash(then));
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
  TimeSpan s = date - mar26;

  int days = s.days();

  while (days >= 14) {
    days -= 14;
  }

  return days >= 7;
}

int week_of_year(int month, int day_of_month) {
  int days_passed = 0;
  for (int i = 0; i < month; i++) {
    days_passed += days_in_month[i];
  }

  return (days_passed + day_of_month) / 7;
}

void loop() {
//  led_red();
//  delay(1000);
//  
//  led_green();
//  delay(1000);
//  
//  led_blue();
//  delay(1000);
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

void adjust_rtc_time() {
    // Sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    
    // January 21, 2014 at 3am
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}

