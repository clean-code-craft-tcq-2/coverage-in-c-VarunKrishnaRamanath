#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "test/catch.hpp"
#include <cstring.h>
#include "typewise-alert.h"

static TargetDetails Target;
static BatteryCharacter batteryChar;

#define GERMAN	3u

void test_InferBreach(double lowerLimit, double upperLimit)
{
    REQUIRE(inferBreach(lowerLimit - 0.0001, lowerLimit, upperLimit) == TOO_LOW);
    REQUIRE(inferBreach(lowerLimit, lowerLimit, upperLimit) == NORMAL);
    REQUIRE(inferBreach((lowerLimit+upperLimit)/2, lowerLimit, upperLimit) == NORMAL);
    REQUIRE(inferBreach(upperLimit, lowerLimit, upperLimit) == NORMAL);
    REQUIRE(inferBreach(upperLimit + 0.0001, lowerLimit, upperLimit) == TOO_HIGH);
}

void test_classifyTemperatureBreach(void)
{
    unsigned int coolingType;
    double temperatureInC;

    AlertLimit TempAlertLimit[NumOfCoolingTypes] = {
	{0, 35}, // PASSIVE_COOLING
	{0, 45}, // HI_ACTIVE_COOLING
	{0, 40}  // MED_ACTIVE_COOLING
};
    /* Checking the limits for all cooling types*/
    for(coolingType = 0; coolingType < (unsigned int)NumOfCoolingTypes;coolingType++)
    {
        temperatureInC = TempAlertLimit[coolingType].LowerLimit - 1;
        REQUIRE(classifyTemperatureBreach(coolingType, temperatureInC) == TOO_LOW);
        temperatureInC = TempAlertLimit[coolingType].LowerLimit;
        REQUIRE(classifyTemperatureBreach(coolingType, temperatureInC) == NORMAL);
        temperatureInC = TempAlertLimit[coolingType].UpperLimit;
        REQUIRE(classifyTemperatureBreach(coolingType, temperatureInC) == NORMAL);
        temperatureInC = TempAlertLimit[coolingType].UpperLimit + 1;
        REQUIRE(classifyTemperatureBreach(coolingType, temperatureInC) == TOO_HIGH);
    }
}

void test_sendEmail(void)
{
    REQUIRE(strcmp(SendEmail("a.b@c.com", "Whats up?"), "To: a.b@c.com\nWhats up?"));
}

void test_LanguageSupported(void)
{
    REQUIRE(LanguageSupported(ENGLISH) == true);
    REQUIRE(LanguageSupported(KANNADA) == true);
    REQUIRE(LanguageSupported((AlertLanguage)GERMAN) == false);
}

void test_sendToEmailEng (void)
{
    REQUIRE(strcmp(sendToEmail(NORMAL), ""));
    REQUIRE(strcmp(sendToEmail(TOO_HIGH), "To: a.b@c.com\nHi, the temperature is too high\n"));
    REQUIRE(strcmp(sendToEmail(TOO_LOW), "To: a.b@c.com\nHi, the temperature is too low\n"));
}

void test_sendToEmailKan (void)
{
    REQUIRE(strcmp(sendToEmail(NORMAL), ""));
    REQUIRE(strcmp(sendToEmail(TOO_HIGH), "To: bengaluru@kannada.com\nNamaskara, Thaapamaana hechaagide\n"));
    REQUIRE(strcmp(sendToEmail(TOO_LOW), "To: bengaluru@kannada.com\nNamaskara, Thaapamaana kadime ide\n"));
}

void test_sendAlert(TargetDetails Target)
{
    sendAlert(Target, NORMAL);
}

/* Test for InferBreach function*/
TEST_CASE("infers the breach according to limits")
{
    test_InferBreach(20, 30);
}

/* Test for classifyTemperatureBreach function*/
TEST_CASE("verifies the Temperature breaches for given cooling types")
{
    test_classifyTemperatureBreach();
}

/* Test for sendAlert function*/
TEST_CASE("verifies the alert message sent to Email targets in ENGLISH")
{
    Target.Target = TO_EMAIL;
    Target.TargetSettings.EmailDetail.Language = ENGLISH;
    strcpy(Target.TargetSettings.EmailDetail.recepient, "a.b@c.com");

    test_sendAlert(Target);  /* prints the message for alert*/
    test_sendEmail(); /* tests if this function appends the message to client*/
    test_LanguageSupported(); /* checks if the given language is supported. Default ENGLISH*/
    test_sendToEmailEng(); /* returns the message to be printed for given language*/
}

/* Test for sendAlert function*/
TEST_CASE("verifies the alert message sent to another Email targets in KANNADA")
{
    Target.Target = TO_EMAIL;
    Target.TargetSettings.EmailDetail.Language = KANNADA;
    strcpy(Target.TargetSettings.EmailDetail.recepient, "bengaluru@kannada.com");

    test_sendAlert(Target);  /* prints the message for alert*/
    test_sendEmail(); /* tests if this function appends the message to client*/
    test_LanguageSupported(); /* checks if the given language is supported. Default ENGLISH*/
    test_sendToEmailKan(); /* returns the message to be printed for given language*/
}

/* Test for sendAlert function*/
TEST_CASE("verifies the alert message sent to Email targets in unsupportedLanguage")
{
    Target.Target = TO_EMAIL;
    Target.TargetSettings.EmailDetail.Language = (AlertLanguage)GERMAN;
    strcpy(Target.TargetSettings.EmailDetail.recepient, "a.b@c.com");

    test_sendAlert(Target);  /* prints the message for alert*/
    test_sendEmail(); /* tests if this function appends the message to client*/
    test_LanguageSupported(); /* checks if the given language is supported. Default ENGLISH*/
    test_sendToEmailEng(); /* returns the message to be printed for given language*/
}

TEST_CASE("verifies the checkAndAlert function")
{
    strcpy(batteryChar.brand, "BOSCH");
    batteryChar.coolingType = PASSIVE_COOLING;
    Target.Target = TO_EMAIL;
    Target.TargetSettings.EmailDetail.Language = KANNADA;
    strcpy(Target.TargetSettings.EmailDetail.recepient, "bengaluru@kannada.com");

    checkAndAlert(Target, batteryChar, 35);
}
