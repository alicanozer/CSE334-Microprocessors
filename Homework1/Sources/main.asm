;*****************************************************************
;* This stationery serves as the framework for a                 *
;* user application (single file, absolute assembly application) *
;* For a more comprehensive program that                         *
;* demonstrates the more advanced functionality of this          *
;* processor, please see the demonstration applications          *
;* located in the examples subdirectory of the                   *
;* Freescale CodeWarrior for the HC12 Program directory          *
;*****************************************************************

; export symbols
            ABSENTRY Entry        ; for absolute assembly: mark this as application entry point  

; Include derivative-specific definitions 
		INCLUDE 'derivative.inc' 

;*****************************************************************
;* CSE 334 Microprocessors                                       *
;* Spring 2015                                                   *
;* Homework 1                                                    *
;* @name  :Alican OZER                                           *
;* @number:111044070                                             *
;* @date  :20/03/2015                                            *
;* DESCRIPTION: 16bit LCM calculator includes BONUS              *
;* you can also run with 16bit numbers                           *
;* replace your numbers with num1 and num2 values                *
;* inputs: num1 and num2                                         *
;* results: x_sum and y_sum are equal to result                  *
;*****************************************************************

; variable/data section  

prog:   equ $4000       ;program code block
data:   equ $1000       ;data block

; code section
        org prog        ;start of program
Entry:
        movw num1,x_sum ;copy for keeping an comparing temp numbers
        movw num2,y_sum ;""""  
        ldd x_sum       ;load to accD for comparing with y_sum   
                
while:        
        cpd y_sum       ;calculate x_sum-y_sum 
        beq while_out   ;if(x_sum=y_sum) LCM is x_sum=y_sum
        bhi sumX_greater;if(x_sum > y_sum)(if N is clear) 

sumY_greater:
        ldd x_sum       ;for summing with num2
        addd num1       ;else accD += num1 
        std x_sum       ;store accD in x_sum for comparing in loop
        bra while       ;loop again
        
sumX_greater:
        ldd y_sum       ;for summing with num2
        addd num2       ;then accD += num2
        std y_sum       ;store accD in y_sum comparing up
        ldd x_sum       ;load to accD for comparing with y_sum
        bra while       ;loop again
        
while_out:
       ;result is already in x_sum and y_sum addresses.        
       ; used for showing 8bit numbers on EmumiS' leds 
       ; ldaa #$FF       ;load accA to FF
       ; staa DDRB       ;set DDRB output
       ; ldaa result     ;load result to accA
       ; staa PORTB      ;store result to LED
        swi             ;sotware interrupt
        
;data define        
        org data        ;start of data block
        num1: dc.w 24   ;first number
        num2: dc.w 40   ;second number
        x_sum: ds.w 1   ;used for temp and shows to RESULT
        y_sum: ds.w 1

;**************************************************************
;*                 Interrupt Vectors                          *
;**************************************************************
            ORG   $FFFE
            DC.W  Entry           ; Reset Vector
