#include "typewise-alert.h"
#include <stdio.h>

#define GREATER_THAN_UPPPERLIMIT(value, upperLimit)		(value > upperLimit)
#define LESSER_THAN_LOWERLIMIT(value, lowerLimit)		(value < lowerLimit)

const AlertLimit TemperatureAlertLimit[NumOfCoolingTypes] = {
	{0, 35}, // PASSIVE_COOLING
	{0, 45}, // HI_ACTIVE_COOLING
	{0, 40}  // MED_ACTIVE_COOLING
};


BreachType inferBreach(double value, double lowerLimit, double upperLimit)
{
  BreachType TempBreach = NORMAL;

  if(LESSER_THAN_LOWERLIMIT(value, lowerLimit))
  {
    TempBreach = TOO_LOW;
  }
  if(GREATER_THAN_UPPPERLIMIT(value, upperLimit))
  {
    TempBreach = TOO_HIGH;
  }
  return TempBreach;
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
  int lowerLimit = TemperatureAlertLimit[coolingType].LowerLimit;
  int upperLimit = TemperatureAlertLimit[coolingType].LowerLimit;

  return inferBreach(temperatureInC, lowerLimit, upperLimit);
}

void checkAndAlert(TargetDetails Target, BatteryCharacter batteryChar, double temperatureInC) {

  BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);

  sendAlert(Target, breachType);
}
