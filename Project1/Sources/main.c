/**
* CSE 334 MICROPROCESSORS
* PROJECT 1
* 04/05/2015
* ALICAN OZER
**/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define enable()  __asm(cli)
#define disable() __asm(sei)

#define WORD_SIZE 25
const char hex2seven[]={0X3F,0X06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,
                        0X7F,0X6F,0X77,0X7C,0X58,0X5E,0X79,0X71};
                           /*_    *a    b    c    d    e    f    g    h    i   l */
const char harfler_seven[]={0x08,0x5f,0x7c,0x58,0x5e,0x79,0x71,0x7D,0x76,0x10,0x38,
0X6E,0x73,0X50,0x5c,0x6d,0X1C,0x78,0X5B,0X54};
/*Y   p    R   O    S     U     T   Z    N */
char gir[]="GIR_";
char bilinen[]="____";
char kontrol[]={0,0,0,0};
const char my_words[][4]={"ABLA","DAYI","HAHA","PALA","YALA","BABA","DEDE","BEBE","LALA","PARA","HELA","SOBA","BACA","SORU",
"ANNE","ANLA","CANO","DANA","POLO","GOLF","EDIZ","EDEP","BELA","FENA","YALI"};
  

unsigned char WORD=6,PUAN=0,DEVAM=1,DENEME=0,BILINEN_7=0,SONUC_7=0,PUAN_7=0,GIR_7=0;
unsigned int randomCounter=0;


void buzzer(void);
void getInputControl(void);
void delay(unsigned int tm);
void MSDelay(unsigned int tm);
char ascii2seven(char ascii);
interrupt(((0x10000-Vrti)/2)-1) void REALTIMEI(void);  

/*******************************/
void main(void) {
    unsigned int count; 
    disable();

    DDRB=0xff;
    DDRP=0xff;
    DDRH=0x00;
    PORTB=0;
    
    RTICTL=0x24;
    CRGINT=0x80;
    CRGFLG=0x80;

    enable();
      
   
  while(1){
       WORD=(randomCounter+7)%WORD_SIZE;   
       bilinen[0]='_';bilinen[1]='_';bilinen[2]='_';bilinen[3]='_';
       kontrol[0]=0;kontrol[1]=0;kontrol[2]=0;kontrol[3]=0;
       gir[0]='G';gir[1]='I';gir[2]='R';gir[3]='_';
       PUAN=0;DENEME=0;DEVAM=1;
       for(count=0; (count<6 && DEVAM); ++count){ 
         BILINEN_7=1;SONUC_7=0;PUAN_7=0;GIR_7=0;   
         delay(1000);
         getInputControl();
       }
    }  
}
/*********/
void buzzer(void){
  unsigned char x;
  DDRT= 0X20;
    for(x=0; x<100; x++){        
      PTT = PTT | 0X20;
      MSDelay(20);
      PTT = PTT & ~0X20;
      MSDelay(20);
    }   
}
/*************/
void getInputControl(){
    unsigned char input,i=0,bildi=0;
    BILINEN_7=0;SONUC_7=0;PUAN_7=0;GIR_7=1;
    
    delay(5000);
    DDRP=0xFF;
    DDRH=0x00;    //dp input

    input=PTH;     //read from dip switch
    randomCounter += input;
    
    DENEME++;
    for(i=0;i<4;i++){
        if(my_words[WORD][i] == input){
          if(kontrol[i] == 0){
            kontrol[i] =1;
            PUAN=PUAN+10;
            bilinen[i]=input;
            bildi=1;
          }
        }
    }
                
    if(bildi == 0){     
        buzzer();MSDelay(30); 
        buzzer();MSDelay(30);
        buzzer();
        
    } 
    if(PUAN == 40){       

        BILINEN_7=0;SONUC_7=1;PUAN_7=0;GIR_7=0;
        delay(2000);
        
        gir[0]='G';gir[1]='O';gir[2]='O';gir[3]='L';        
        BILINEN_7=0;SONUC_7=0;PUAN_7=0;GIR_7=1;         
        delay(2000);
        
        gir[0]='S';gir[1]='C';gir[2]='R';gir[3]='_';
        delay(2000);
        
        BILINEN_7=0;SONUC_7=0;PUAN_7=1;GIR_7=0;
        DEVAM=0;
        
        delay(5000);
        
    }else if(DENEME>5){
        BILINEN_7=0;SONUC_7=1;PUAN_7=0;GIR_7=0;
        delay(2000);
        
        gir[0]='L';gir[1]='O';gir[2]='S';gir[3]='T'; 
        BILINEN_7=0;SONUC_7=0;PUAN_7=0;GIR_7=1;         
        delay(2000);
               
        gir[0]='S';gir[1]='C';gir[2]='R';gir[3]='_';
        delay(2000);
        
        BILINEN_7=0;SONUC_7=0;PUAN_7=1; GIR_7=0;
        DEVAM=0;
        
        delay(5000);   
    }             
}

/***************************/
void delay(unsigned int tm){
  volatile unsigned int j,k;
    for(j=0;j<tm;++j)
      for(k=0; k<2000; ++k);
} 
/*************/
void MSDelay(unsigned int tm){
  unsigned int i,j;
  for(i=0;i<tm;i++)
    for(j=0;j<300;j++);
}
/************/
char ascii2seven(char ascii){
    if(ascii==65){return harfler_seven[1];//a        
    }else if(ascii==66){return harfler_seven[2];//b    
    }else if(ascii==67){return harfler_seven[3];//c    
    }else if(ascii==68){return harfler_seven[4];//d    
    }else if(ascii==69){return harfler_seven[5];//e    
    }else if(ascii==70){return harfler_seven[6];//f    
    }else if(ascii==71){return harfler_seven[7];//g    
    }else if(ascii==72){return harfler_seven[8];//h    
    }else if(ascii==73){return harfler_seven[9];//ý    
    }else if(ascii==76){return harfler_seven[10];//l    
    }else if(ascii==89){return harfler_seven[11];//y    
    }else if(ascii==80){return harfler_seven[12];//P
    }else if(ascii==82){return harfler_seven[13];//R 
    }else if(ascii==79){return harfler_seven[14];//O
    }else if(ascii==83){return harfler_seven[15];//S        
    }else if(ascii==85){return harfler_seven[16];//U 
    }else if(ascii==84){return harfler_seven[17];//T
    }else if(ascii==90){return harfler_seven[18];//Z
    }else if(ascii==78){return harfler_seven[19];//N
    }else return harfler_seven[0];//_
}
/*******************/
interrupt(((0x10000-Vrti)/2)-1) void REALTIMEI(void){
    static unsigned char nibble=0;
    if(BILINEN_7 == 1){
        switch(nibble){
            case 0: PTP = 0x0E; //1110
                PORTB = ascii2seven(bilinen[nibble]);
                break;
            case 1: PTP = 0x0D; //1101
                PORTB = ascii2seven(bilinen[nibble]);
                break;
            case 2: PTP = 0x0B; //1011
                PORTB = ascii2seven(bilinen[nibble]);
                break;
            case 3: PTP = 0x07; //0111
                PORTB = ascii2seven(bilinen[nibble]);
                break; 
            }
            nibble = (nibble + 1) % 4;
            CRGFLG = 0x80;           
    }else if(SONUC_7 == 1){
        switch(nibble){
            case 0: PTP = 0x0E; //1110
                PORTB = ascii2seven(my_words[WORD][nibble]);
                break;
            case 1: PTP = 0x0D; //1101
                PORTB = ascii2seven(my_words[WORD][nibble]);
                break;
            case 2: PTP = 0x0B; //1011
                PORTB = ascii2seven(my_words[WORD][nibble]);
                break;
            case 3: PTP = 0x07; //0111
                PORTB = ascii2seven(my_words[WORD][nibble]);
                break; 
            }
            nibble = (nibble + 1) % 4;
            CRGFLG = 0x80;           
    }else if(GIR_7 == 1){
        switch(nibble){
            case 0: PTP = 0x0E; //1110
                PORTB = ascii2seven(gir[nibble]);
                break;
            case 1: PTP = 0x0D; //1101
                PORTB = ascii2seven(gir[nibble]);
                break;
            case 2: PTP = 0x0B; //1011
                PORTB = ascii2seven(gir[nibble]);
                break;
            case 3: PTP = 0x07; //0111
                PORTB = ascii2seven(gir[nibble]);
                break; 
            }
            nibble = (nibble + 1) % 4;
            CRGFLG = 0x80;           
    }else if(PUAN_7 == 1){
        switch(nibble){
            case 0: PTP = 0x0E; //1110
                PORTB = hex2seven[(PUAN>>12)&0x0F];
                break;
            case 1: PTP = 0x0D; //1101
                PORTB = hex2seven[(PUAN>>8)&0x0F];
                break;
            case 2: PTP = 0x0B; //1011
                PORTB = hex2seven[(PUAN>>4)&0x0F];
                break;
            case 3: PTP = 0x07; //0111
                PORTB = hex2seven[(PUAN)&0x0F];
                break; 
            }
            nibble = (nibble + 1) % 4;
            CRGFLG = 0x80;           
    }
}       
/***********************************************************/