#include<stdio.h>
#include "typewise-alert.h"
#include "Language.h"

static TargetDetails AlertTargetDetails;

char* (*TargetForAlertFunPtr[MaxNumOfAlertTargets])(BreachType) = {
sendToController,
sendToEmail
};

char* SendEmail(const char* recepient, const char* message)
{
    static char messageToPrintEmail[500];
    sprintf(messageToPrintEmail, "To: %s\n%s", recepient, message);

    return messageToPrintEmail;
}

bool LanguageSupported(AlertLanguage Language)
{
    int i;
    bool LangSupported = false;
    for(i = 0; i < MaxNumOfSupportedLanguages; i++)
    {
        if(Language == i)
        {
            LangSupported = true;
            break;
        }
    }
    return LangSupported;
}

char* sendToController(BreachType breachType) {
    static char messageToPrintController[500];
    sprintf(messageToPrintController, "%x : %x\n", AlertTargetDetails.TargetSettings.header, breachType);

    return messageToPrintController;
}

char* sendToEmail(BreachType breachType)
{
  const char* recepient = AlertTargetDetails.TargetSettings.EmailDetail.recepient;
  static char PrintMsg[500] = "";

  if(breachType != NORMAL)
  {
      if(!LanguageSupported(AlertTargetDetails.TargetSettings.EmailDetail.Language))
      {
          AlertTargetDetails.TargetSettings.EmailDetail.Language = ENGLISH;
      }
    strcpy(PrintMsg, SendEmail(recepient, TempAlertMessage[AlertTargetDetails.TargetSettings.EmailDetail.Language][breachType]));
  }
  return PrintMsg;
}

void sendAlert(TargetDetails Target, BreachType breachType){

    AlertTargetDetails = Target;

    printf("%s", TargetForAlertFunPtr[AlertTargetDetails.Target](breachType));
}
