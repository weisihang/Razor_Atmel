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


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
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
  extern u8 G_au8DebugScanfBuffer[];  /* From debug.c */
  extern u8 G_u8DebugScanfCharCount;  /* From debug.c */
  static u8 u8MyName[]="WeiSihang"; 
  static u8 u8BufferMessage[]   = "\n\rBuffer contents:\n\r";
  static u8 u8EmptyMessage[]="\n\rEMPTY!";
  static u8 u8OutPut[]="\n\r*****\n\r";
  static u8 u8OutPutNum=0;
  static u8 u8NumNameMessage[] = "\n\rTimes of your name's appearances in buffer: ";
  static u16 u16NameCounter=0;
  static u32 u32TimeCounter=0;
  static u8 u8CharCount;
  static u8 u8CountOfStar=0;
  static u8 u8CompareCount=0;
  static u8 u8Flag;
  static u16 u16RecordOfNameCounter;
  static bool bFlagOfName = FALSE;
  
  
  if(u32TimeCounter==100000)//Every ten seconds check the buffer and reset the buffer
  {
    for(u16 i = 0; i < U16_USER_INPUT_BUFFER_SIZE;i++)
    {
      UserApp_au8UserInputBuffer[i] = G_au8DebugScanfBuffer[i];// use another array to restore
      //the strings and print it out
    }
    /* Read the buffer and print the contents */
    u8CharCount = DebugScanf(UserApp_au8UserInputBuffer);//to check out the length of the strings
    UserApp_au8UserInputBuffer[u8CharCount] = '\0';
    while(UserApp_au8UserInputBuffer[u8CompareCount] != '\0')
    {
      if(UserApp_au8UserInputBuffer[u8CompareCount]=='W')
      {
        bFlagOfName = TRUE;
        u8Flag=u8CompareCount;// set a flag showing the begening of the compare programme
      }
      if(bFlagOfName)
      {
        if((u8CompareCount-u8Flag)<8)
        {
          if(UserApp_au8UserInputBuffer[u8CompareCount]!=u8MyName[u8CompareCount-u8Flag])
          {
            bFlagOfName = FALSE;// compare the strings with the array restoring the name
          }
        }
        if((u8CompareCount-u8Flag)==8)
        {
          u16NameCounter++;
          bFlagOfName = FALSE;// if results of the comparison is true, then adds the counter;
        }
      }
      u8CompareCount++;
    }
    DebugPrintf(u8BufferMessage);
    if(u8CharCount > 0)
    {
      DebugPrintf(UserApp_au8UserInputBuffer);
    }
    else
    {
      DebugPrintf(u8EmptyMessage);
    }
    DebugPrintf(u8NumNameMessage);
    DebugPrintf(u8OutPut);
    u16RecordOfNameCounter=u16NameCounter;
    while(u16NameCounter>1)//to determine the digits of the numbers
    {
      u16NameCounter=u16NameCounter/10;
      u8OutPutNum++;
    }
    if(u16NameCounter==0)
    {
      u8OutPutNum=1;
    }
    for(u8 i = 0;i<(5-u8OutPutNum)/2;i++)
    {
      DebugPrintf("*");
      u8CountOfStar++;
    }
    DebugPrintNumber(u16RecordOfNameCounter);
    for(u8 i = 0;i<(5-u8CountOfStar-u8OutPutNum);i++)
    {
      DebugPrintf("*");
    }// to adjust the output of the number
    DebugPrintf(u8OutPut);
    DebugLineFeed();
    u16NameCounter=0;
    u8CompareCount=0;
    u32TimeCounter=0;
    u8OutPutNum=0;
    u8CountOfStar=0;
  }
  else
  {
    u32TimeCounter++;
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
