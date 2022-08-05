#INCLUDE <P16F877A.INC>

RTIE equ 5;TMR0 interruption enable bit
RTIF equ 2;TMR0 interruption flag bit
RBIE equ 3;PortB changing interruption enable bit
RBIF equ 0;PortB changing interruption flag bit
RBPU equ 7;PortB Pull Up resitors enable bit

decades equ 30h;register for left indicator
units equ decades + 1;register for right indicator
tmp equ units + 1
tmp_W equ units + 1;register for saving accumulator's data before
interruption
tmp_STATUS equ tmp_W + 1;register for saving STATUS data before
interruption
flags_register equ tmp_STATUS + 1;buffer for flags

;registers for loops counters
C1 equ flags_register + 1
C2 equ C1 + 1
C3 equ C2 + 1
C4 equ C3 + 1
C5 equ C4 + 1
C6 equ C5 + 1
C7 equ C6 + 1

flag_left_button equ 0; left button's flag
flag_right_button equ 1 ;right button's flag
flag_buzzer equ 2 ;buzzer's flag
left_button equ RB4 ;bit of left button
right_button equ RB5 ;bit of right button
buzzer_bit equ RB3 ;bit of buzzer

org 0 ; start main programm
goto start_programm

org 4
    ;save register
    movwf tmp_W
    movfw STATUS
    movwf tmp_STATUS

    btfsc INTCON,RBIF;checking PortB interruption
    call interruption_RBIF
    btfsc INTCON,RTIF;checking TMR0 interruption
    call interruption_RTIF

    ;recovery registers
    movfw tmp_STATUS
    movwf STATUS
    movfw tmp_W
    retfie

PIC
get_mass_element
    addwf PCL,F
    retlw B'00010000' ;0
    retlw B'01011011' ;1
    retlw B'00001100' ;2
    retlw B'00001001' ;3
    retlw B'01000011' ;4
    retlw B'00100001' ;5
    retlw B'00100000' ;6
    retlw B'00011011' ;7
    retlw B'00000000' ;8
    retlw B'00000001' ;9

interruption_RTIF;whiile button is pushing, increment value for 20 num/sec
    bcf INTCON,RTIF
    btfss flags_register,flag_left_button;check pushing left button
    goto no_add_decades
    movlw .9 ;clear if value > 9
    subwf decades,W
    btfsc STATUS,Z
    goto zeroz_decades
    incf decades,F;increment value
    goto no_add_decades
    zeroz_decades
    clrf decades

    no_add_decades

    btfss flags_register,flag_right_button ;check pushing right button
    goto no_add_units

    movlw .9;clear if value > 9
    subwf units,W
    btfsc STATUS,Z
    goto zeroz_units
    incf units,F;increment value
    goto no_add_units
    zeroz_units
    clrf units

    no_add_units

    movlw .6
    movwf TMR0 ; set timer

    return

interruption_RBIF
    movfw PORTB
    movwf PORTB
    nop
    bcf INTCON,RBIF
    12

    ;setting bits based on button presses
    btfss PORTB,left_button
    bsf flags_register,flag_left_button
    btfsc PORTB,left_button
    bcf flags_register,flag_left_button

    btfss PORTB,right_button
    bsf flags_register,flag_right_button
    btfsc PORTB,right_button
    bcf flags_register,flag_right_button

    return

start_programm

    ;pre-settings
    bsf STATUS,RP0 ;chouse 1 bank of memory

    clrf TRISB ;set PORTB to output
    clrf TRISC ;set PORTC to output

    bsf TRISB,left_button
    bsf TRISB,right_button

    bcf OPTION_REG,PSA
    bcf OPTION_REG,T0CS

    bcf OPTION_REG, RBPU

    bcf STATUS,RP0 ;chouse 0 bank of memory
    clrf PORTC ;clear PORTC
    clrf PORTB ;clear PORTB

    clrf INTCON
    bsf INTCON,GIE
    bsf INTCON,RTIE
    bsf INTCON,RBIE

    clrf decades
    clrf units

    clrf flags_register

    movlw .6
    movwf TMR0 ;set timers

;label main need for infinite loop
main
    call print;print func

    call check;check for equal func

    ;if flag_buzzer was set -> call beep
    btfsc flags_register,flag_buzzer
    call beep;beep func

goto main

check
    movfw decades
    subwf units,W
    btfsc STATUS,Z
    bsf flags_register,flag_buzzer
    btfss STATUS,Z
    bcf flags_register,flag_buzzer
    return
beep ;beep func
    bsf PORTB,buzzer_bit
    movlw .13
    call delay2
    bcf PORTB,buzzer_bit
    movlw .13
    call delay2
return

print

    ;print left indicator
    movfw decades
    call get_mass_element
    clrf PORTC
    movwf PORTC
    bsf PORTC,RC7
    movlw .1
    call delay

    ;print right indicator
    movfw units
    call get_mass_element
    clrf PORTC
    movwf PORTC
    movlw .1
    call delay

    return

;delay with idling
delay
    movwf C1
l1
    movlw .100
    movwf C2
l2
    movlw .166
    movwf C3

    clrwdt
l3
    decfsz C3,F
    goto l3
    decfsz C2,F
    goto l2
    decfsz C1,F
    goto l1
    return

;delay with printing
delay2
    movwf C5
l11
    movlw .100
    movwf C6
l21
    movlw .166
    movwf C7
    clrwdt
l31
    decfsz C7,F
    goto l31
        
    movfw decades
    call get_mass_element
    clrf PORTC
    movwf PORTC
    bsf PORTC,RC7
    clrf PORTC
    movwf PORTC

    decfsz C6,F
    goto l21
    decfsz C5,F
    goto l11
    return

end ;end of programm