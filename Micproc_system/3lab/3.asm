#INCLUDE <P16F877A.INC>

;options and intcon bits

RBIE equ 3;PortB changing interruption enable bit
RBIF equ 0;PortB changing interruption flag bit
RBPU equ 7;PortB Pull Up resitors enable bit

decades equ 30h;register for left indicator
units equ decades + 1 ;register for right indicator
tmp_W equ units + 1;register for saving accumulator's data before
interruption
tmp_STATUS equ tmp_W + 1; register for saving STATUS data before
interruption
flags_register equ tmp_STATUS + 1 ;buffer for flags

;registers for loops counters
C1 equ flags_register + 1
C2 equ C1 + 1
C3 equ C2 + 1
C4 equ C3 + 1
C5 equ C4 + 1
C6 equ C5 + 1
C7 equ C6 + 1

flag_unit equ 0;flag enable unit changing
flag_decades equ 1;flag enable decades changing
flag_func equ 2;flag enable decades and units changing

button_func equ RB7;button name for bit RB7
button_unit equ RB6;button name for bit RB6
button_decades equ RB5;button name for bit RB5

org 0 ; start main programm
    goto start_programm

org 4
    ;save register
    movwf tmp_W ;save accumulator
    movfw STATUS ;save STATUS
    movwf tmp_STATUS
    btfsc INTCON,RBIF; check RB Interruption
    call interruption_RBIF;programm for RB Interruption

    ;recovery registers
    movfw tmp_STATUS
    movwf STATUS
    movfw tmp_W
    retfie

;get_led_for_number
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

interruption_RBIF
    movlw .1
    call delay ;delay for missing button bounce
    movfw PORTB
    movwf PORTB
    nop
    bcf INTCON,RBIF

    ;check press unit button
    btfss PORTB,button_unit
    goto press_but_unit
    end_check_press_but_unit

    ;check press decades button
    btfss PORTB,button_decades
    goto press_but_dec;;;;;;;;;;;;;;;
    end_check_press_but_dec

    ;check press func button
    btfss PORTB,button_func
    goto press_but_func;;;;;;;;;;;;;;;
    end_check_press_but_func

    ;check unpress unit button
    btfsc PORTB, button_unit
    goto endof_unit
    end_check_unpress_but_unit

    ;check unpress decades button
    btfsc PORTB, button_decades
    goto endof_dec
    end_check_unpress_but_decades

    ;check unpress func button
    btfsc PORTB, button_func
    goto endof_func
    end_check_unpress_but_func

    goto endof

    ;if unpress dec button
    endof_dec
    bcf flags_register, flag_decades;allow decades changing
    goto end_check_unpress_but_decades

    endof_unit
    bcf flags_register, flag_unit;allow unit changing
    goto end_check_unpress_but_unit

    endof_func
    bcf flags_register, flag_func;;allow func changing
    goto end_check_unpress_but_func
    endof

    return

press_but_unit
    btfss flags_register, flag_unit
    call addition_units
    bsf flags_register, flag_unit;forbid unit changing
    goto end_check_press_but_unit

press_but_dec
    btfss flags_register, flag_decades
    call addition_decades
    bsf flags_register, flag_decades;forbid decades changing
    goto end_check_press_but_dec

press_but_func
    btfss flags_register, flag_func
    goto addition_func
    end_addition_func
    bsf flags_register, flag_func;forbid func changing
    goto end_check_press_but_func

;for addition func program should
addition_func
    call addition_units;add units
    call addition_decades;add decades twice
    call addition_decades
    goto end_addition_func

    addition_units
    movlw .9 ;max value
    subwf units,W
    btfsc STATUS,Z; if units > 9 then clrf units and end function
    goto no_add
    movlw .1 ;add 1 in units
    addwf units,F
    goto end_add
    no_add
    clrf units
    end_add
    return

addition_decades

    movlw .9 ;max value
    subwf decades,W
    btfsc STATUS,Z; if units > 9 then clrf units and end function
    goto no_add_dec
    movlw .1 ;add 1 in decades
    addwf decades,F
    goto end_add_dec
    no_add_dec
    clrf decades
    end_add_dec
    return

;preprocessing
start_programm
    ;pre-settings
    bsf STATUS,RP0 ;choose 1 bank of memory

    clrf TRISB ;set PORTB to output
    clrf TRISC ;set PORTC to output

    bsf TRISB,button_func ;set bit func button to input
    bsf TRISB,button_unit ;set bit unit button to input
    bsf TRISB,button_decades;set bit decades button to input
    
    bcf OPTION_REG, RBPU

    bcf STATUS,RP0 ;chouse 0 bank of memory
    clrf PORTC ;clear PORTC
    clrf PORTB ;clear PORTB

    ;set interruption
    clrf INTCON
    bsf INTCON,GIE
    bsf INTCON,RBIE

    clrf units
    clrf decades

    clrf flags_register

;main program
main
    ;enable right indicator
    movfw decades
    call get_mass_element
    clrf PORTC
    movwf PORTC
    bsf PORTC,RC7
    movlw .1
    call delay

    ;enable left indicator
    movfw units
    call get_mass_element
    clrf PORTC
    movwf PORTC
    movlw .1
    call delay
goto main

;fun "delay" causes a delay of duration W*10ms
;the microcontroller is idling at this time
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
    
end ;end of program