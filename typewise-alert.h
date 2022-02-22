#pragma once
#include <stdbool.h>
#include <string.h>

typedef enum {
  PASSIVE_COOLING = 0,
  HI_ACTIVE_COOLING,
  MED_ACTIVE_COOLING,
  NumOfCoolingTypes
} CoolingType;

typedef enum {
    ENGLISH = 0,
    KANNADA = 1,
    MaxNumOfSupportedLanguages
} AlertLanguage;

typedef enum {
  NORMAL,
  TOO_LOW,
  TOO_HIGH,
  NumOfBreachTypes
} BreachType;

typedef struct{
    double LowerLimit;
    double UpperLimit;
}AlertLimit;

typedef enum {
  TO_CONTROLLER = 0,
  TO_EMAIL,
  MaxNumOfAlertTargets
} AlertTarget;

typedef struct {
  CoolingType coolingType;
  char brand[48];
} BatteryCharacter;

typedef struct{
    char recepient[100];    /* mandatory to fill when the target is Email*/
    AlertLanguage Language; /* mandatory to fill when the target is Email*/
}EmailTargetDetails;

typedef union{
    unsigned short header;  /* mandatory to fill when the target is Controller*/
    EmailTargetDetails EmailDetail;
}TargetSpec;

typedef struct{
    AlertTarget Target;
    TargetSpec TargetSettings;
} TargetDetails;


/* FUNCTIONS */
void checkAndAlert(TargetDetails Target, BatteryCharacter batteryChar, double temperatureInC);
BreachType inferBreach(double value, double lowerLimit, double upperLimit);
BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC);
void sendAlert(TargetDetails Target, BreachType breachType);
char* sendToEmail(BreachType breachType);
char* sendToController(BreachType breachType);
bool LanguageSupported(AlertLanguage Language);
char* SendEmail(const char* recepient, const char* message);
