/*************************************/
/*     GEBZE TECHNICAL UNIVERSITY    */ 
/* CSE334 MICROPROCESSORS SPRING 2015*/
/* FINAL PROJECT :HANGMAN IS BACK    */
/*************************************/

#include <hidef.h>      /* common defines and macros */
#include <string.h>
#include "derivative.h"      /* derivative-specific definitions */
#include "lcd.h"

#define enable()  __asm(cli)
#define disable() __asm(sei)
#define endl() put2lcd(0xC0,CMD) // move cursor to 2nd row, 1st column

#define NUMOFWORDS 350
const char my_words[][15]={"TESTS","THEIR","WOULD","ABOUT","THERE","THINK","WHICH","PEOPLE","COULD","OTHER","THESE",
"FIRST","BECAUSE","THING","THOSE", "WOMAN","THROUGH","CHILD","AFTER","SHOULD","WORLD",
"SCHOOL","STILL","THREE","STATE","NEVER","BECOME","BETWEEN","REALLY","SOMETHING","ANOTHER",
"FAMILY","LEAVE","WHILE","STUDENT","GREAT","GROUP","BEGIN","COUNTRY","WHERE","PROBLEM",
"EVERY","START","MIGHT","AMERICAN","AGAINST","PLACE","AGAIN","SURVEY","SUPPOSED","TRADITION",
"WINTER","VILLAGE","SOVIET","REFUSE","SALES","COMMUNICATION","SCREEN","RESIDENT","POTENTIAL","EUROPEAN",
"PRESENCE","INDEPENDENT","DISTRICT","SHAPE","READER","CONTRACT","CROWD","CHRISTIAN","EXPRESS","APARTMENT",
"WILLING","STRENGTH","PREVIOUS","OBVIOUSLY","HORSE","INTERESTED","TARGET","PRISON","GUARD","TERMS",
"DEMAND","REPORTER","DELIVER","VEHICLE","OBSERVE","FLIGHT","FACILITY","UNDERSTANDING","AVERAGE","EMERGE",
"ADVANTAGE","QUICK","LEADERSHIP","POUND","BASIS","BRIGHT","OPERATE","GUEST","SAMPLE","CONTRIBUTE",
"BLOCK","PROTECTION","SETTLE","COLLECT","ADDITIONAL","HIGHLY","IDENTITY","TITLE","MOSTLY","LESSON",
"FAITH","RIVER","PROMOTE","LIVING","COUNT","UNLESS","MARRY","TOMORROW","TECHNIQUE","PRINCIPLE",
"SURVIVE","BORDER","COMPETITION","GATHER","LIMIT","EQUIPMENT","WORTH","ASSOCIATE","CRITIC","ASPECT",
"INSIST","FAILURE","ANNUAL","FRENCH","CHRISTMAS","COMMENT","RESPONSIBLE","AFFAIR","PROCEDURE","REGULAR",
"SPREAD","CHAIRMAN","BASEBALL","IGNORE","BELIEF","DEMONSTRATE","ANYBODY","MURDER","RELIGION","REVIEW",
"EDITOR","ENGAGE","COFFEE","DOCUMENT","SPEED","CROSS","INFLUENCE","ANYWAY","THREATEN","COMMIT",
"FEMALE","YOUTH","AFRAID","QUARTER","BACKGROUND","NATIVE","BROAD","WONDERFUL","APPARENTLY","SLIGHTLY",
"REACTION","TWICE","PERSPECTIVE","GROWING","CONSTRUCTION","INTELLIGENCE","DESTROY","CONNECTION","GRADE","CONTEXT",
"COMMITTEE","MISTAKE","LOCATION","CLOTHES","INDIAN","QUIET","DRESS","PROMISE","AWARE","NEIGHBOR","FUNCTION",
"ACTIVE","EXTEND","CHIEF","COMBINE","BELOW","VOTER","LEARNING","DANGEROUS","REMIND","MORAL",
"UNITED","CATEGORY","RELATIVELY","VICTORY","ACADEMIC","INTERNET","HEALTHY","NEGATIVE","FOLLOWING","HISTORICAL",
"MEDICINE","DEPEND","PHOTO","FINDING","DIRECT","CLASSROOM","CONTACT","JUSTICE","PARTICIPATE","DAILY",
"FAMOUS","EXERCISE", "FLOWER","FAMILIAR","APPROPRIATE","SUPPLY","FULLY","ACTOR","BIRTH","SEARCH",
"DEMOCRACY","EASTERN","PRIMARY","YESTERDAY","CIRCLE","DEVICE","PROGRESS","BOTTOM","ISLAND","EXCHANGE",
"CLEAN","STUDIO","TRAIN","COLLEAGUE","APPLICATION","DAMAGE","PLASTIC","PLATE","OTHERWISE","WRITING",
"ALIVE","EXPRESSION", "NORTHERN","PAINT","RUSSIAN","INSTRUCTION","CLIMB","SWEET","ENGINE","FOURTH",
"EXPAND","IMPORTANCE","METAL", "TICKET","SOFTWARE","DISAPPEAR","CORPORATE","STRANGE","READING","URBAN",
"MENTAL","INCREASINGLY","LUNCH","EDUCATIONAL","CELEBRATION","EFFICIENT","COMPOSITION","SATISFY","BRIEFLY","CARBON",
"CLOSER","CONSUME","SCHEME","CRACK","FREQUENCY","TOBACCO","SURVIVOR","BESIDES","PSYCHOLOGIST","WEALTHY",
"GALAXY","GIVEN","LIMITATION","TRACE","APPOINTMENT","PREFERENCE","METER","EXPLOSION","PUBLICLY","INCREDIBLE",
"FIGHTER","RAPID","ADMISSION","HUNTER","EDUCATE","PAINFUL","FRIENDSHIP","INFANT","CALCULATE","FIFTY",
"PORCH","TENDENCY","UNIFORM","FORMATION","SCHOLARSHIP","RESERVATION","EFFICIENCY","QUALIFY","DERIVE","SCANDAL",
"HELPFUL","IMPRESS","RESEMBLE","PRIVACY","FABRIC","CONTEST","PROPORTION","GUIDELINE","RIFLE","MAINTENANCE",
"CONVICTION","TRICK","ORGANIC","EXAMINATION","PUBLISHER","STRENGTHEN","PROPOSED","SOPHISTICATED","STANDING","ASLEEP",
"TENNIS","NERVE","LEGACY","EXTENSION","SHRUG","BATTERY","ARRIVAL","LEGITIMATE","ORIENTATION","INFLATION"};

unsigned int riderValue=7;
char knownWord[15];
char controlWord[15];
void clrscr();
void delay_1s(unsigned char n);
void startGame(void);
void delay1ms(unsigned int tm);
interrupt(((0x10000-Vtimch5)/2)-1) void rider(void);  
void buzzer(unsigned char x,unsigned char y);
void MSDelay(unsigned int tm);
char getKey(void);
/**************************************************************/
void main(void) {
    disable();
    DDRH=0x00;
    DDRB=0xFF;
    DDRP=0xFF;
    DDRA=0X0F;
    
    /*SERIAL COM */
    SCI0BDL=156;
    SCI0BDH=0;
    SCI0CR1=0X00;
    SCI0CR2=0X0C;    
    
    /*** output compare ch5 for buzzer and LED***/
    TSCR1=0X80;
    TSCR2=0X05;
    TIOS=0x20;
    TCTL1=0x04;
    TFLG1=0x20;
    TIE=TIE | 0x20;  

    enable();
    openlcd(); // Initialize LCD display
    clrscr();
    puts2lcd("HANGMAN IS BACK!"); // Send first line
    endl();
    puts2lcd("Are u ready!!"); // Send first line
    delay_1s(2);
    clrscr();
    puts2lcd("DP0==0 KEYPAD");
    endl();
    puts2lcd("DP0==1 PUTTY");
    
    delay_1s(3);
    startGame();    

    __asm(swi);

}
/*************************************************************/
void MSDelay(unsigned int tm){
  unsigned int i,j;
  for(i=0;i<tm;i++)
    for(j=0;j<300;j++);
}
/*************************************************************/
void delay1ms(unsigned int tm){
  unsigned int i,j;
  for(i=0;i<tm;i++)
    for(j=0;j<4000;j++);
}
/*********************************************************/
void delay_1s(unsigned char n){
    for (;n>0;n--) delay1ms(1000);
}
/********************************************************/

void clrscr(){
    put2lcd(0x01,CMD);
    delay1ms(2);
}
/******************************************************/
void buzzer(unsigned char x,unsigned char y){
  DDRT= 0X04;
  while(x > 0){        
    PTT = PTT | 0X04;
    delay1ms(y);
    PTT = PTT & ~0X04;
    delay1ms(y);
    x--;
  }   
}
/*****************************************************/
char getKey(){
  int row,column;
  const char row_mask[4]={0XFE,0XFD,0XFB,0XF7};
  const char col_mask[4]={0X10,0X20,0X40,0X80};
  const unsigned int keys[4][4]={1,2,3,10,
                                 4,5,6,11,
                                 7,8,9,12,
                                14,0,15,13};
  for(row=0;row<4;++row){
    PORTA=0XFF;
    for(column=0;column<4;++column){
      PORTA=PORTA & row_mask[row];
      if((PORTA & col_mask[column]) == 0){
        delay1ms(20);
        if((PORTA & col_mask[column]) == 0)
          return keys[row][column];
      }
    }
  }
  return -1;
}
/******************************************************************/
void startGame(void){
    unsigned char randomWord=riderValue;
    unsigned int i=0,LEVEL=5,WIN=0,LOSE=0,get=1;
    unsigned char POINT,FLAG,input,KNOWN,FAIL,TOTALRAND=0;
    volatile char ten=-1,one=-1;
    clrscr();
    puts2lcd("Starting Game..");
    delay_1s(1);
    while(1){
      KNOWN=0;
      clrscr();
      puts2lcd("Please Wait");
      endl();
      puts2lcd("Selecting word");
      do{
        randomWord++;
        randomWord=(randomWord)%NUMOFWORDS;
      }while(strlen(my_words[randomWord]) != LEVEL);
      for(i=0;i<LEVEL;++i){
        knownWord[i]='?';
        controlWord[i]=0;
      }
      delay_1s(2);      
      clrscr();
      puts2lcd("Push to GO");
      POINT=0;
      FAIL=6;
      while(FAIL>0){ 
          FLAG=0;
          clrscr();
          puts2lcd(knownWord);
          endl();
          puts2lcd("Remain :");
          put2lcd(FAIL+48,DATA); 
          ten=-1;
          one=-1;
          if((PTH != 0b00000000) && (PTH != 0b00000001)){
             clrscr();
            puts2lcd("DPSWITCH ERROR");
            endl();
            puts2lcd("Select 0 or 1");
            while((PTH != 0b00000000) && (PTH != 0b00000001));
            clrscr();
            put2lcd(PTH+48,DATA);
            puts2lcd(" SELECTED");
            delay_1s(2);
            clrscr();
            puts2lcd(knownWord);
            endl();
            puts2lcd("Remain :");
            put2lcd(FAIL+48,DATA);            
          }if(PTH == 0b00000001){
            while((SCI0SR1 & 0X20) == 0);                
            input=SCI0DRL;

          }else if(PTH == 0b00000000){ 
              do{
                ten=getKey();
              }while(ten == -1);
              puts2lcd("  ");
              put2lcd(ten+48,DATA);
              delay1ms(800);               
              
              do{
                one=getKey();
              }while(one == -1);
              put2lcd(one+48,DATA);
              input=(ten*10)+one;
              delay1ms(800);              
          }
          ten=-1;
          one=-1;  
          TOTALRAND += input;
          for(i=0;i<LEVEL;i++){
            if(my_words[randomWord][i] == input){
              if(controlWord[i] == 0){
                controlWord[i] =1;
                KNOWN++;
                knownWord[i]=input;
                FLAG=1;

              }
            }
          }  
          if(FLAG == 0){
            FAIL--;
            buzzer(200,4);           
          }  
          
          
          if(LEVEL == KNOWN){
            clrscr();
            puts2lcd("YOU WiN");
            endl();
            puts2lcd("MAKE YOUR BEST");
            LEVEL++;
            WIN++;
            for(i=0;i<15;i++){
              buzzer(2,20);
              buzzer(20,2);
            }
            break; 
            
          }
          if(FAIL==0){
            clrscr();
            puts2lcd("YOU LOSE");
            endl();
            puts2lcd("TRY AGAIN");
            LOSE++;
            for(i=0;i<3;i++){
              buzzer(2,20);
              buzzer(4,10);
              buzzer(8,5);
              buzzer(16,2);
              buzzer(20,1);
            }
            break;
          } 
      }
      delay_1s(2);
      clrscr();
      puts2lcd("YOUR WORD IS");

      endl();
      put2lcd('#',DATA);
      puts2lcd(my_words[randomWord]);
      put2lcd('#',DATA);
      delay_1s(4);
            
      clrscr();
      puts2lcd("SCORE:");
      endl();
      puts2lcd("COM:");
      put2lcd(LOSE+48,DATA);
      puts2lcd(" YOU:");
      put2lcd(WIN+48,DATA);
      delay_1s(4);
       
   }
}
/******************************************************/
interrupt(((0x10000-Vtimch5)/2)-1) void rider(void){
  static unsigned char FLAG=0;
  if(riderValue == 0b11100000){
    FLAG=0;
  }else if(riderValue == 0b00000111){
    FLAG=1;
  }
  if(FLAG==0){    
    TC5 -= riderValue;
    riderValue = riderValue >> 1;
    delay_50us(riderValue);
  }else{  
    TC5 += riderValue;
    riderValue = riderValue << 1;
    delay_50us(riderValue);
  }
  
  PORTB=riderValue;
  
  
  TFLG1=0x20;
}
/********************************************************/