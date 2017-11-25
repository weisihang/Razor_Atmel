/**********************************************************************************************************************
File: user_app1.c                                                                

Description:
Provides a Tera-Term driven system to display, read and write an LED command list.

Test1.
Test2 from Engenuics.
Test3.

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:
None.

Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u8 G_au8DebugScanfBuffer[DEBUG_SCANF_BUFFER_SIZE]; /* From debug.c */
extern u8 G_u8DebugScanfCharCount;                        /* From debug.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static u8 UserApp_au8UserInputBuffer[U16_USER_INPUT_BUFFER_SIZE  ];  

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
  for(u16 i = 0; i < U16_USER_INPUT_BUFFER_SIZE  ; i++)
  {
    UserApp_au8UserInputBuffer[i] = 0;
  }
  u8 au8UserApp1Start1[] = "LED program task started\n\r";
  
  /* Turn off the Debug task command processor and announce the task is ready */
  DebugSetPassthrough();
  DebugPrintf(au8UserApp1Start1);
  
    /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_FailedInit;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for input */
static void UserApp1SM_Idle(void)
{
  static u8 u8Message1[] = "\n\r LED Programmming Interface\n\r";
  static u8 u8Message2[] = " Press 1 to Program LED command sequence.\n\r";
  static u8 u8Message3[] = " Press 2 show current USER Program.\n\r";
  static u8 u8menu_Message4[]="Press 3 to clear the USERLIST\n\r";
  
  static u8 u8SeperateLine1[]="***********************************************************************\n\r";
  static u8 u8SeperateLine2[]="-----------------------------------\n\r";
  static u8 u8ExampleMessage1[]="\n\n\rEnter commands as LED-ONTIME-OFFTIME and press Enter\n\r";
  static u8 u8ExampleMessage2[]="Time is in milliseconds,max 100commands\n\r";
  static u8 u8ExampleMessage3[]="LED colors: R, O, Y, G, C, B, P, W\n\r";
  static u8 u8ExampleMessage4[]="Example:R-100-200 (Red on at 100ms and off at 200ms)\n\r";
  static u8 u8ExampleMessage5[]="Press Enter on blank line to end\n\r";
  static u8 u8ExampleMessage6[]=":";
  
  static u8 u8Example1Message1[]="\n\n\rCurrent USER program:\n\r";
  static u8 u8Example1Message2[]="LED  ON TIME   OFF TIME\n\r";
  static u8 u8Example1Message3[]="Command entry complete.\n\r";
  static u8 u8Example1Message4[]="Commands entered:";
  static u8 u8Example1Message5[]="New USER program:\n\n\r";
  
  static u8 u8ScanCounter = 0;
  static u8 u8CharCount;
  static u8 u8Counter2 = 6;
  static u16 u16StartNum;
  static u32 u32EndNumSum = (u32)0;
  static u16 u16EndNumCounter = 6;
  static u32 u32Power = 1;
  static u16 u16ListNum = 1;
  static u8 u8ListIndex = 0;
  
  static u8 u8String[] = "only num after 1\"-\"";
  static u8 u8String2[] = "invalid char for LED";
  static u8 u8String3[] = "incomplete command";
  static u8 u8String4[] = "lack \"-\"";
  static u8 u8String5[] = "space before number";
  static u8 u8String6[] = "invalid last char";
  static u8 u8String7[] = "3 units for 1stnum";
  static u8 u8String8[] = "only num after 2\"-\"";
  static u8 u8String9[] = "how much you have input:";
  static u8 u8String10[] = "start number:";
  static u8 u8String11[] = "end number:";
  
  static bool bInputIsWrong = FALSE;
  static bool bShowMenu = FALSE;
  static bool bStartProgram = FALSE;
  static bool bShowInput = TRUE;
  static bool bStartVerify = FALSE;
  static bool bLedDisplayPrintListLine = TRUE;
  static LedCommandType aeUserList[100];


  if(bShowMenu)
  {
    DebugLineFeed();
    DebugPrintf(u8SeperateLine1);
    DebugPrintf(u8Message1);
    DebugPrintf(u8Message2);
    DebugPrintf(u8Message3);
    DebugPrintf(u8menu_Message4);
    DebugPrintf(u8SeperateLine1);
    DebugLineFeed();
    bShowMenu = FALSE;
  }
  if((!bStartProgram)&&(!bShowInput))
  {
    for(u16 i = 0; i < U16_USER_INPUT_BUFFER_SIZE  ; i++)
    {
      UserApp_au8UserInputBuffer[i] = G_au8DebugScanfBuffer[i]; 
    }
    if(DebugScanf(G_au8DebugScanfBuffer)==1)
    {
      if(UserApp_au8UserInputBuffer[0]== '1')
      {
        DebugLineFeed();
        bStartProgram = TRUE;
        bShowInput = FALSE;
        DebugPrintf(u8ExampleMessage1);
        DebugPrintf(u8ExampleMessage2);
        DebugPrintf(u8ExampleMessage3);
        DebugPrintf(u8ExampleMessage4);
        DebugPrintf(u8ExampleMessage5);   
        DebugPrintNumber(u16ListNum);
        DebugPrintf(u8ExampleMessage6);
      }
      if(UserApp_au8UserInputBuffer[0]=='2')
      {
        bShowInput = TRUE;
        bStartProgram = FALSE;
      }
      if(UserApp_au8UserInputBuffer[0]=='3')
      {
        LedDisplayStartList();
        u8ListIndex=0;
        u16ListNum = 1;
        DebugLineFeed();
        bStartProgram = TRUE;
        bShowInput = FALSE;
        DebugPrintf(u8ExampleMessage1);
        DebugPrintf(u8ExampleMessage2);
        DebugPrintf(u8ExampleMessage3);
        DebugPrintf(u8ExampleMessage4);
        DebugPrintf(u8ExampleMessage5);   
        DebugPrintNumber(u16ListNum);
        DebugPrintf(u8ExampleMessage6);
      }
    }
  }
  if((bShowInput)&&(!bStartProgram))
  {
    DebugPrintf(u8Example1Message1);
    DebugPrintf(u8Example1Message2);
    DebugPrintf(u8SeperateLine2);
    for(u8 i=0 ; i<= u8ListIndex-1; i++)
    {
      LedDisplayPrintListLine(i);
    }
    DebugPrintf(u8SeperateLine2);
    bShowInput = FALSE;
    bShowMenu = TRUE;
    
  }
  if((bStartProgram)&&(!bShowInput))
  {
    for(u16 i = 0; i < U16_USER_INPUT_BUFFER_SIZE ; i++)
    {
      UserApp_au8UserInputBuffer[i] = G_au8DebugScanfBuffer[i]; 
      if(G_au8DebugScanfBuffer[i]=='\r')
      {
        bStartVerify = TRUE;
        i = U16_USER_INPUT_BUFFER_SIZE;
      }
    }
  }
  
  if(bStartVerify)
  {
    while(UserApp_au8UserInputBuffer[u8ScanCounter]!='\r'&&(u8ScanCounter<U16_USER_INPUT_BUFFER_SIZE))
    {
      u8ScanCounter++;
    }
    if(u8ScanCounter==0)
    {
      DebugPrintf(u8Example1Message3);
      DebugLineFeed();
      DebugPrintf(u8Example1Message4);
      DebugPrintNumber(u16ListNum-1);
      DebugLineFeed();
      DebugPrintf(u8Example1Message5);
      DebugPrintf(u8Example1Message2);
      if((u16ListNum-1)==0)
      {
        DebugPrintf("you have entered nothing but \"enter\"");
        bLedDisplayPrintListLine=FALSE;
      }
      if(bLedDisplayPrintListLine)
      {
        DebugPrintf(u8SeperateLine2);
        for(u8 i=0 ; i<= u8ListIndex-1; i++)
        {
          LedDisplayPrintListLine(i);
        }
        DebugPrintf(u8SeperateLine2);
      }
      //LedDisplayStartList();
      bShowMenu = TRUE;
      //u8ListIndex=0;
      u8ScanCounter=0;
      u8CharCount=0;
      u8Counter2=6;
      u16EndNumCounter=6;
      u32EndNumSum=0;
      u32Power=1;
      //u16ListNum = 1;
      bLedDisplayPrintListLine = TRUE;
      bInputIsWrong = FALSE;
      bStartProgram = FALSE;
      bShowInput = FALSE;
      bStartVerify = FALSE;
    }
    if((u8ScanCounter!=U16_USER_INPUT_BUFFER_SIZE)&&(u8ScanCounter!=0))
    {
      DebugScanf(UserApp_au8UserInputBuffer);
      u8CharCount = u8ScanCounter;
      UserApp_au8UserInputBuffer[u8CharCount] = '\0';
      DebugLineFeed();
      DebugPrintf(u8String9);
      DebugPrintNumber(u8CharCount);
      DebugLineFeed();
      
      
      if(u8CharCount<=NUMOFINPUTLEAST)
      {
        DebugPrintf(u8String3); 
        DebugLineFeed();
        bInputIsWrong =TRUE;
      }
      
      if(u8CharCount >= 12)
      {
        DebugPrintf("too long "); 
        DebugLineFeed();
        bInputIsWrong =TRUE;
      }
      
      if(u8CharCount>NUMOFINPUTLEAST&&u8CharCount < 12)
      {
        if((UserApp_au8UserInputBuffer[0]=='R'||UserApp_au8UserInputBuffer[0]=='O'
            ||UserApp_au8UserInputBuffer[0]=='Y'||UserApp_au8UserInputBuffer[0]=='G'
              ||UserApp_au8UserInputBuffer[0]=='C'||UserApp_au8UserInputBuffer[0]=='B' 
                ||UserApp_au8UserInputBuffer[0]=='P'||UserApp_au8UserInputBuffer[0]=='W')&&(!bInputIsWrong))
        {
          ;
        }
        else
        {
          DebugPrintf(u8String2);
          DebugLineFeed();
          bInputIsWrong =TRUE;
        }
        if(UserApp_au8UserInputBuffer[1]!='\055'&&(!bInputIsWrong))
        {
          DebugPrintf(u8String4);
          DebugLineFeed();
          bInputIsWrong =TRUE;
        }
        if((UserApp_au8UserInputBuffer[2]<'\060'||UserApp_au8UserInputBuffer[2]>'\071')&&(!bInputIsWrong))
        {
          if(UserApp_au8UserInputBuffer[2]=='\040')
          {
            DebugPrintf(u8String5);
            DebugLineFeed();
            bInputIsWrong =TRUE;
          }
          else
          {
            DebugPrintf(u8String);
            DebugLineFeed();
            bInputIsWrong =TRUE;
          }
        }
        if((UserApp_au8UserInputBuffer[3]<'\060'||UserApp_au8UserInputBuffer[3]>'\071')&&(!bInputIsWrong))
        {
          DebugPrintf(u8String);
          DebugLineFeed();
          bInputIsWrong =TRUE;
        }
        if((UserApp_au8UserInputBuffer[4]<'\060'||UserApp_au8UserInputBuffer[4]>'\071')&&(!bInputIsWrong))
        {
          DebugPrintf(u8String); 
          DebugLineFeed();
          bInputIsWrong =TRUE;
        }
        if((UserApp_au8UserInputBuffer[5]!='\055')&&(!bInputIsWrong))
        {
          DebugPrintf(u8String7);
          DebugLineFeed();
          bInputIsWrong =TRUE;
        }
        while((!bInputIsWrong)&&(u8Counter2<u8CharCount-1))
        {
          if(UserApp_au8UserInputBuffer[u8Counter2]<'\060'||UserApp_au8UserInputBuffer[u8Counter2]>'\071')
          {
            DebugPrintf(u8String8);
            DebugLineFeed();
            bInputIsWrong =TRUE;
          }
          else
          {
            u8Counter2++;
          }
        }
        if((UserApp_au8UserInputBuffer[u8CharCount-1]<'\060'||UserApp_au8UserInputBuffer[u8CharCount-1]>'\071')&&(!bInputIsWrong))
        {
          DebugPrintf(u8String6); 
          DebugLineFeed();
          bInputIsWrong =TRUE;
        }
      }
      if(bInputIsWrong)
      {
        DebugLineFeed();
        DebugPrintNumber(u16ListNum);
        DebugPrintf(u8ExampleMessage6);
        DebugLineFeed();
      }
      if(!bInputIsWrong)
      {
        u16StartNum = ((int)UserApp_au8UserInputBuffer[2]-48)*100+((int)UserApp_au8UserInputBuffer[3]-48)*10+
          ((int)UserApp_au8UserInputBuffer[4]-48)*1;
        DebugLineFeed();
        DebugPrintf(u8String10);
        DebugPrintNumber(u16StartNum);
        DebugLineFeed();
        while(u16EndNumCounter <= (u8CharCount-1))
        {
          for(u8 i =1; i <= (u8CharCount-1-u16EndNumCounter); i++)
          {
            u32Power = u32Power * 10;
          }
          u32EndNumSum += (((int)UserApp_au8UserInputBuffer[u16EndNumCounter]-48)* u32Power);
          u16EndNumCounter++;
          u32Power = 1;
        }
        DebugPrintf(u8String11);
        DebugPrintNumber(u32EndNumSum);
        if(u32EndNumSum>u16StartNum)
        {
          switch(UserApp_au8UserInputBuffer[0])
          {
          case 'R':
            aeUserList[u8ListIndex].eLED=RED;
            break;
          case 'O':
            aeUserList[u8ListIndex].eLED=ORANGE;
            break;
          case 'Y':
            aeUserList[u8ListIndex].eLED=YELLOW;
            break;
          case 'G':
            aeUserList[u8ListIndex].eLED=GREEN;
            break;
          case 'C':
            aeUserList[u8ListIndex].eLED=CYAN;
            break;
          case 'B':
            aeUserList[u8ListIndex].eLED=BLUE;
            break;
          case 'P':
            aeUserList[u8ListIndex].eLED=PURPLE;
            break;
          case 'W':
            aeUserList[u8ListIndex].eLED=WHITE;
            break;
          }
          aeUserList[u8ListIndex].u32Time = u16StartNum;
          aeUserList[u8ListIndex].bOn = TRUE;
          aeUserList[u8ListIndex].eCurrentRate=LED_PWM_0;
          LedDisplayAddCommand(USER_LIST, &aeUserList[u8ListIndex]);
          u8ListIndex++;
          
          switch(UserApp_au8UserInputBuffer[0])
          {
          case 'R':
            aeUserList[u8ListIndex].eLED=RED;
            break;
          case 'O':
            aeUserList[u8ListIndex].eLED=ORANGE;
            break;
          case 'Y':
            aeUserList[u8ListIndex].eLED=YELLOW;
            break;
          case 'G':
            aeUserList[u8ListIndex].eLED=GREEN;
            break;
          case 'C':
            aeUserList[u8ListIndex].eLED=CYAN;
            break;
          case 'B':
            aeUserList[u8ListIndex].eLED=BLUE;
            break;
          case 'P':
            aeUserList[u8ListIndex].eLED=PURPLE;
            break;
          case 'W':
            aeUserList[u8ListIndex].eLED=WHITE;
            break;
          }
          aeUserList[u8ListIndex].u32Time = u32EndNumSum;
          aeUserList[u8ListIndex].bOn=FALSE;
          aeUserList[u8ListIndex].eCurrentRate=LED_PWM_100;
          LedDisplayAddCommand(USER_LIST, &aeUserList[u8ListIndex]);
          u8ListIndex++;
          
          DebugLineFeed();
          u16ListNum++;
          DebugPrintNumber(u16ListNum);
          DebugPrintf(u8ExampleMessage6);
        }
        else
        {
          DebugLineFeed();
          DebugPrintf("end time must oversize start time");
          DebugLineFeed();
          DebugPrintNumber(u16ListNum);
          DebugPrintf(u8ExampleMessage6);
          DebugLineFeed();
        }
      }
      u8ScanCounter=0;
      u8CharCount=0;
      u8Counter2=6;
      u16EndNumCounter=6;
      u32EndNumSum=0;
      u32Power=1;
      bInputIsWrong = FALSE;
      bShowInput = FALSE;
      bStartVerify = FALSE;
    }
  }
} /* end UserApp1SM_Idle() */
                      
            
#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp1SM_FailedInit(void)          
{
    
} /* end UserApp1SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
