#INCLUDE <P16F877A.INC>
C1 equ 20h; C1 now points at memory cell at 20h
C2 equ 21h; C2 now points at memory cell at 21h
C3 equ 22h; C3 now points at memory cell at 22h

ORG 0 ; Текущий адрес в ПЗУ - 0

    bsf STATUS,RP0; Select memory page 1
    clrf TRISB; Set port B to output
    clrf TRISD; Set port D to output
    bcf STATUS,RP0; Return to page 0
    clrf PORTD; Set zero at port D pins
    clrf PORTB; Set zero at port B pins
    bsf PORTB,RB6; Turn on bit 6 at port B
    bsf PORTB,RB7; Turn on bit 7 at port B

main; Start of main programme

    clrf PORTD; Set zero at port D pins

    clrwdt; Clear WatchDog Timer
    btfss PORTB,RB7; If 7th bit of PORTB is 1, skip next line
    goto checkBut1; Go to handler of first button clicked

    btfss PORTB,RB6; Skip next line if 6th bit of PORTB is 1
    goto checkBut2; Go to handler of second button pressed
goto main; Loop to beginning of main program

checkBut1; Start of first handler
    clrwdt; Clear WatchDog Timer
    btfss PORTB,RB6; Skip next line if 6th bit of PORTB is 1
    goto work; Run handler when both buttons are pressed

    movlw 0xAA; Move 0b10101010 to accumulator
    movwf PORTD; Move 0xAA to PORTD (Light odd lights)

    btfsc PORTB,RB7; If Button 1 is still pressed, skip next line
    goto main; Return to main program
goto checkBut1; Loop to beginning of handler

checkBut2; Start of second handler
    clrwdt; Clear WatchDog Timer
    btfss PORTB,RB7; Skip next line if 7th bit of PORTB is 1
    goto work; Run handler when both buttons are pressed

    movlw 0x55; Move 0b01010101 to accumulator
    movwf PORTD; Move 0x55 to PORTD (Light even lights)
    btfsc PORTB,RB6; If Button 2 is still pressed, skip next line
    goto main; Return to main program
goto checkBut2; Loop to beginning of handler

work; Start of handler, when both buttons are pressed
    clrwdt; Clear WatchDog Timer
    movlw 0xAA; Move 0b10101010 to accumulator
    movwf PORTD; Move 0xAA to PORTD (Light odd lights)
    btfsc PORTB,RB6; If Button 2 is still pressed, skip next line
    goto main; Return to main program
    btfsc PORTB,RB7; If Button 1 is still pressed, skip next line
    goto main; Return to main program
    call delay; Make MC sleep for 0.5 secs
    movlw 0x55; Move 0b01010101 to accumulator
    movwf PORTD; Move 0x55 to PORTD (Light odd lights)
    btfsc PORTB,RB6; If Button 2 is still pressed, skip next line
    goto main; Return to main program
    btfsc PORTB,RB7; If Button 1 is still pressed, skip next line
    goto main; Return to main program
    call delay; Make MC sleep for 0.5 secs

goto work; Loop to beginning of handler

delay; Subprogram for delay
    movlw .50; Move 50 to Accumulator
    movwf C1; Move 50 from accumulator to 20h
l1; First loop
    movlw .100; Move 100 to accumulator
    movwf C2; Move 100 from accumulator to 21h
l2; Second loop
    movlw .166; Move 166 to accumulator
    movwf C3; Move 166 from accumulator to 22h
l3; Third loop
    decfsz C3,F; Decrease C3, if C3=0, skip next line
    goto l3; Loop back to l3
    decfsz C2,F; Decrease C2, if C2=0, skip next line
    goto l2; Loop back to l2;
    decfsz C1,F; Decrease C1, if C1=0, skip next line
    goto l1; Loop back to l1;
    return; Return to place of call
end; End of program