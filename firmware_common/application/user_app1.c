/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */
extern u8 G_au8DebugScanfBuffer[];  /* From debug.c */
extern u8 G_u8DebugScanfCharCount;  /* From debug.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static u8 UserApp_au8MyName[] = "Sihang Wei";     
static u8 UserApp_au8LCD1[] = "Your choice?";   
static u8 UserApp_au8LCD2[] = "B0:DISPLAY";
static u8 UserApp_au8LCD3[] = "B1:DEBUG";
static u8 UserApp_au8LCD4[] = "B3:MENU";
static u8 UserApp_au8LCD5[] = "B0:LEFT";
static u8 UserApp_au8LCD6[] = "B1:RIGHT";
static u8 UserApp_au8LCD7[] = "INPUT MESSAGES";
static u8 UserApp_au8LCD8[] = "PRESS\"ENTER\"";
static u8 UserApp_CursorPosition;
static u8 UserApp_au8UserInputBuffer[U16_USER_INPUT_BUFFER_SIZE];  

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
  //LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, UserApp_au8LCD1);
  LCDMessage(LINE1_START_ADDR+13,UserApp_au8LCD4);
  LCDMessage(LINE2_START_ADDR,UserApp_au8LCD2); 
  LCDMessage(LINE2_START_ADDR+12,UserApp_au8LCD3);  
  UserApp_CursorPosition = LINE1_START_ADDR;
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(RED);
  for(u16 i = 0; i < U16_USER_INPUT_BUFFER_SIZE  ; i++)
  {
    UserApp_au8UserInputBuffer[i] = 0;
  }


 
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
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
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  static u16 u16MovementCounter = 0;
  static bool bMoveLeft = FALSE;
  static bool bMoveRight = FALSE;
  static bool bexcuationOfMove = FALSE;
  static bool bDebug = FALSE;
  static u8 u8ArrayCount = 0;
  static u8 u8CharCount = 0;
  
  if(!bexcuationOfMove&&!bMoveLeft&&!bMoveRight&&!bDebug)
  {
    if(WasButtonPressed(BUTTON0))
    {
      ButtonAcknowledge(BUTTON0);
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR, UserApp_au8MyName);
      LCDMessage(LINE2_START_ADDR, UserApp_au8LCD5);
      LCDMessage(LINE2_START_ADDR+8, UserApp_au8LCD6);
      bexcuationOfMove = TRUE;
    }
    if(WasButtonPressed(BUTTON1))
    {
      ButtonAcknowledge(BUTTON1);
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR, UserApp_au8LCD7);
      LCDMessage(LINE2_START_ADDR, UserApp_au8LCD8);
      LCDMessage(LINE2_START_ADDR+13, UserApp_au8LCD4);
      bDebug = TRUE;
    /*  if(G_au8DebugScanfBuffer[u8ArrayCount] != '\n')
      {
        UserApp_au8UserInputBuffer[u8ArrayCount] = G_au8DebugScanfBuffer[u8ArrayCount];
        u8ArrayCount++;
      }
      u8ArrayCount=0;
      u8CharCount = DebugScanf(UserApp_au8UserInputBuffer);
      UserApp_au8UserInputBuffer[u8CharCount] = '\0';
      if(u8CharCount<=20)
      {
        LCDCommand(LCD_CLEAR_CMD);
        LCDMessage/(LINE1_START_ADDR, UserApp_au8UserInputBuffer);
      }*/
    }
  }
  if(bexcuationOfMove&&!bMoveLeft&&!bMoveRight&&!bDebug)
  {
    if(WasButtonPressed(BUTTON0))
    {
      ButtonAcknowledge(BUTTON0);
      bMoveLeft = TRUE;
    }
    if(WasButtonPressed(BUTTON1))
    {
      ButtonAcknowledge(BUTTON1);
      bMoveRight = TRUE;
    }
  }
  if(bMoveRight)
  {
    PWMAudioOn(BUZZER1);
    if(u16MovementCounter==1000)
    {
      if(UserApp_CursorPosition>LINE1_END_ADDR)
      {
        UserApp_CursorPosition=LINE1_START_ADDR;
      }
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR+UserApp_CursorPosition, UserApp_au8MyName);
      LCDMessage(LINE2_START_ADDR, UserApp_au8LCD4);
      LCDMessage(LINE2_START_ADDR+8, UserApp_au8LCD5);
      u16MovementCounter=0;
      UserApp_CursorPosition++;
    }
    else
    {
      u16MovementCounter++;
    } 
    if(WasButtonPressed(BUTTON0))
    {
      LedOff(WHITE);
      LedOff(PURPLE);
      LedOff(BLUE);
      LedOff(CYAN);
      LedOff(GREEN);
      LedOff(YELLOW);
      LedOff(ORANGE);
      LedOff(RED);
      ButtonAcknowledge(BUTTON0);
      bMoveRight = FALSE;
      bMoveLeft = TRUE;
      u16MovementCounter = 0;
    }
    if((UserApp_CursorPosition==LINE1_START_ADDR)||(UserApp_CursorPosition>LINE1_END_ADDR))
    {
      PWMAudioSetFrequency(BUZZER1, 1000);
      LedOn(WHITE);
      LedOff(RED);
    }
    if(UserApp_CursorPosition==2&&u16MovementCounter==500)
    {
      PWMAudioSetFrequency(BUZZER1, 875);
      LedOn(PURPLE);
      LedOff(WHITE);
    }
    if(UserApp_CursorPosition==5)
    {
      PWMAudioSetFrequency(BUZZER1, 750);
      LedOn(BLUE);
      LedOff(PURPLE);
    }
    if(UserApp_CursorPosition==7&&u16MovementCounter==500)
    {
      PWMAudioSetFrequency(BUZZER1, 625);
      LedOn(CYAN);
      LedOff(BLUE);
    }
    if(UserApp_CursorPosition==10)
    {
      PWMAudioSetFrequency(BUZZER1, 500);
      LedOn(GREEN);
      LedOff(CYAN);
    }
    if(UserApp_CursorPosition==12&&u16MovementCounter==500)
    {
      PWMAudioSetFrequency(BUZZER1, 375);
      LedOn(YELLOW);
      LedOff(GREEN);
    }
    if(UserApp_CursorPosition==15)
    {
      PWMAudioSetFrequency(BUZZER1, 250);
      LedOn(ORANGE);
      LedOff(YELLOW);
    }
    if(UserApp_CursorPosition==17&&u16MovementCounter==500)
    {
      PWMAudioSetFrequency(BUZZER1, 125);
      LedOn(RED);
      LedOff(ORANGE);
    } 
  }
  
  if(bMoveLeft)
  {
    PWMAudioOn(BUZZER1);
    PWMAudioSetFrequency(BUZZER1, 500);
    if(u16MovementCounter==1000)
    {
      if(UserApp_CursorPosition==LINE1_START_ADDR)
      {
        UserApp_CursorPosition=LINE1_END_ADDR;
      }
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(UserApp_CursorPosition, UserApp_au8MyName);
      LCDMessage(LINE2_START_ADDR, UserApp_au8LCD4);
      LCDMessage(LINE2_START_ADDR+8, UserApp_au8LCD6);
      u16MovementCounter=0;
      UserApp_CursorPosition--;
    }
    else
    {
      u16MovementCounter++;
    } 
    if(WasButtonPressed(BUTTON1))
    {
      LedOff(WHITE);
      LedOff(PURPLE);
      LedOff(BLUE);
      LedOff(CYAN);
      LedOff(GREEN);
      LedOff(YELLOW);
      LedOff(ORANGE);
      LedOff(RED);
      ButtonAcknowledge(BUTTON1);
      bMoveLeft = FALSE;
      bMoveRight = TRUE;
      u16MovementCounter = 0;
    }
    if(UserApp_CursorPosition==LINE1_START_ADDR)
    {
      PWMAudioSetFrequency(BUZZER1, 125);
      LedOn(RED);
      LedOff(WHITE);
    }
    if(UserApp_CursorPosition==17&&u16MovementCounter==500)
    {
      PWMAudioSetFrequency(BUZZER1, 250);
      LedOn(ORANGE);
      LedOff(RED);
    }
    if(UserApp_CursorPosition==15)
    {
      PWMAudioSetFrequency(BUZZER1, 375);
      LedOn(YELLOW);
      LedOff(ORANGE);
    }
    if(UserApp_CursorPosition==12&&u16MovementCounter==500)
    {
      PWMAudioSetFrequency(BUZZER1, 500);
      LedOn(GREEN);
      LedOff(YELLOW);
    }
    if(UserApp_CursorPosition==10)
    {
      PWMAudioSetFrequency(BUZZER1, 625);
      LedOn(CYAN);
      LedOff(GREEN);
    }
    if(UserApp_CursorPosition==7&&u16MovementCounter==500)
    {
      PWMAudioSetFrequency(BUZZER1, 750);
      LedOn(BLUE);
      LedOff(CYAN);
    }
    if(UserApp_CursorPosition==5)
    {
      PWMAudioSetFrequency(BUZZER1, 875);
      LedOn(PURPLE);
      LedOff(BLUE);
    }
    if(UserApp_CursorPosition==2&&u16MovementCounter==500)
    {
      PWMAudioSetFrequency(BUZZER1, 1000);
      LedOn(WHITE);
      LedOff(PURPLE);
    } 
  }
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, UserApp_au8LCD1);
    LCDMessage(LINE1_START_ADDR+13,UserApp_au8LCD4);
    LCDMessage(LINE2_START_ADDR,UserApp_au8LCD2); 
    LCDMessage(LINE2_START_ADDR+12,UserApp_au8LCD3);  
    UserApp_CursorPosition = LINE1_START_ADDR;
    bMoveLeft = FALSE;
    bMoveRight = FALSE;
    bexcuationOfMove = FALSE;
    LedOff(WHITE);
    LedOff(PURPLE);
    LedOff(BLUE);
    LedOff(CYAN);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(ORANGE);
    LedOff(RED); 
    PWMAudioOff(BUZZER1);
    bDebug = FALSE;
  }
  if(bDebug)
  {
    while(G_au8DebugScanfBuffer[u8ArrayCount] != '\n')
    {
      UserApp_au8UserInputBuffer[u8ArrayCount] = G_au8DebugScanfBuffer[u8ArrayCount];
      u8ArrayCount++;
    }
    u8ArrayCount=0;
    u8CharCount = DebugScanf(UserApp_au8UserInputBuffer);
    UserApp_au8UserInputBuffer[u8CharCount] = '\0';
    if(u8CharCount<=20)
    {
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR, UserApp_au8UserInputBuffer);
    }
  }
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
