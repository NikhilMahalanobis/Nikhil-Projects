*-----------------------------------------------------------
* Title      : 7 Segment LED
* Written by :
* Date       :
* Description: This file handles the update and drawing of a 7 segment LED which displays the current score,
*              and also handles the function calls to detect the end condition for our game
*-----------------------------------------------------------

*Commands
ALL_DATA_REG reg d0-d7
PRINT_TEXT_COMMAND equ 14
SET_PEN_COLOR_COMMAND equ 80
SET_FILL_COLOR_COMMAND equ 81
DRAW_RECTANGLE_COMMAND equ 87

*Core attributes to the LED, including coordinates of where segments need to be placed,
*as well as offsets for accessing the segment table, and the win score.
LEFT_X_COORDINATE equ 20
RIGHT_X_COORDINATE equ 30
TOP_Y_COORDINATE equ 30
MIDDLE_Y_COORDINATE equ 40
BOTTOM_Y_COORDINATE equ 50
COORDINATE_OFFSET equ 6
PRINT_OFFSET equ 15
WIN_SCORE equ 10

TEST_NUMBER equ 57   
 
WHITE equ $00FFFFFF   

initLED:
    movem.l ALL_REG, -(sp)
    
    lea coordinateTableOffset, a0
    lea printOffset, a1
    lea printFlag, a2
    lea player1Score, a3
    lea player2Score, a4
    lea alienDownVelocity, a5
    
    move.b #COORDINATE_OFFSET, (a0)
    move.b #0, (a1)
    move.b #0, (a2)
    move.l #0, (a3)
    move.l #0, (a4)
    move.l #7, (a5)
    
    movem.l (sp)+, ALL_REG
    rts
*Called every frame of the game loop to check the current score, and draw it to the scren
updateScore:
    movem.l ALL_REG, -(sp)
  
    clr.l d3
    clr.l d4
    
    *Load the current score that we need to draw, as well as the tables that hold the 
    *bitmask for our 7 segment LED, and the coordinates for each of the segments
    lea player1Score, a0
    move.l (a0), d5
    lea sevenSegmentTable, a0
    lea coordinateTableOffset, a1
    lea coordinateTableX1, a2
    lea coordinateTableY1, a3
    lea coordinateTableX2, a4
    lea coordinateTableY2, a5
    lea printOffset, a6
    
;d0 - trap codes
;d5 - number to print
;d6 - single digit to print
;d3 - value of segment to print
;d4 - AND register  
*Function called to read the current ones digit of the number we want to print  
readDigit:
    *Set value of the coordinate offset used to index segment coordinate table
    move.b #COORDINATE_OFFSET, (a1)
    
    *Divide the current score by 10, then swap to get the remainder
    *This remainder will give us the one's digit we need to draw
    divu #10, d5
    swap d5
    move.w d5, d6
    
    *Using the digit itself as an offset, go into the segment table to find out which number needs to be drawn
    move.b (a0, d6), d3
    move.l d3, d4

*Loop to start the drawing of segments
loop:    
    *And 1 to see if the lowest bit is set, to see which segment needs to be drawn
    *If the bit is set to 0, segment must not be drawn, and we must shift down one
    andi.l #1, d4
    beq shift
  
*Loop that actually draws given segment   
drawSegment:
    movem.l ALL_DATA_REG, -(sp)
    
    move.l #SET_PEN_COLOR_COMMAND, d0
    move.l #WHITE, d1
    trap #15
    
    clr d0
    clr.l d1
    clr.l d2
    clr.l d3
    clr.l d4
    
    *If we reach here, we know which digit to draw, which segment to draw, so get the coordinates of each segment to draw
    *from the Segment coordinate table
    move.b (a1), d0
    move.b (a2, d0), d1
    add.b (a6), d1
    move.b (a3, d0), d2
    move.b (a4, d0), d3
    add.b (a6), d3
    move.b (a5, d0), d4
    
    move.l #DRAW_RECTANGLE_COMMAND, d0
    trap #15
    
    movem.l (sp)+, ALL_DATA_REG
    
*Function to shift the bitmask to see which segments of digit to draw
shift:
    *Shift bitmask by 1 since we didn't draw last segment
    lsr #1, d3
    
    *Check if the value after shift is 0. If so, we're done this digit, so move onto the next one.
    cmpi.l #0, d3
    beq upperSwap
    
    *If reach here, we still have segments left to draw
    *Move the current segment back into the reg, and go back to the drawing loop 
    move.l d3, d4
    subi.b #1, (a1)
    bra loop  
 
*Function to swap the upper and lower words of the number we wish to draw   
upperSwap:
    *Load the print flag, add 1, and check if the value is 2
    *If so, we printed all digits, so stop printing the score
    lea printFlag, a6
    addi.b #1, (a6)
    cmpi.b #2, (a6)
    beq stopPrinting
    
    *If we reach here, reset the coordinate offset,
    *and swap the value in d5, since before we had the ones digit, now we need the tens digit
    move.b #COORDINATE_OFFSET, (a1)
    swap d5
    move.w d5, d6
    
    *Set the offset for printing the tens digit, since the tens digit needs to be offset to the left of the ones digit
    lea printOffset, a6
    subi.b #PRINT_OFFSET, (a6)
       
    move.b (a0, d6), d3
    move.l d3, d4
    bra loop
 
*Function to stop printing the score   
stopPrinting:
    *Reset the print offset, and return 
    move.b #0, (a6)
    lea printOffset, a6
    move.b #0, (a6)
    
    movem.l (sp)+,ALL_REG
    rts

*Function to check score to see if player has set win condition
checkP1WinScore:
    movem.l ALL_REG, -(sp)
    
    lea player1Score, a0
    cmpi.l #WIN_SCORE, (a0)
    
    movem.l (sp)+, ALL_REG 
    rts

*Function to check score to see if player has set lose condition
checkP2WinScore:
    movem.l ALL_REG, -(sp)
    
    lea player2Score, a0
    cmpi.l #WIN_SCORE, (a0)
    
    movem.l (sp)+, ALL_REG
    rts
   
*Function to print win message to screen if the player wins. 
printP1ScreenWin:
    
    lea player1String, a1
    move.l #PRINT_TEXT_COMMAND, d0
    trap #15
    
    rts
       
*Function to print lose message to screen if the player loses.
printP2ScreenWin:
    lea player2String, a1
    move.l #PRINT_TEXT_COMMAND, d0
    trap #15
    
    rts
    
*Table that holds the equivalent hex values of the binary bitmasks that make up each digit
*Values gotten from class lecturn, then calculated 
*Table indexed so the digit to draw is also the index of the bitmask value
sevenSegmentTable:  dc.b $7E    ;0 = ABCDEF = 7E
                    dc.b $30    ;1 = BC = 30
                    dc.b $6D    ;2 = ABDEG = 6D
                    dc.b $79    ;3 = ABCDG = 79
                    dc.b $33    ;4 = BCFG = 33
                    dc.b $5B    ;5 = ACDFG = 5B
                    dc.b $5F    ;6 = ACDEFG = 5F
                    dc.b $70    ;7 = ABC = 70
                    dc.b $7F    ;8 = ABCDEFG = 7F
                    dc.b $7B    ;9 = ABCDFG = 7B
    

*Tables to hold each X and Y coordinate for each induvidual segment
*This table is also indexed to match the value of each digit
coordinateTableX1:  dc.b LEFT_X_COORDINATE    ;A
                    dc.b RIGHT_X_COORDINATE   ;B
                    dc.b RIGHT_X_COORDINATE   ;C
                    dc.b LEFT_X_COORDINATE    ;D
                    dc.b LEFT_X_COORDINATE    ;E
                    dc.b LEFT_X_COORDINATE    ;F
                    dc.b LEFT_X_COORDINATE    ;G 
   

coordinateTableX2:  dc.b RIGHT_X_COORDINATE   ;A
                    dc.b RIGHT_X_COORDINATE   ;B
                    dc.b RIGHT_X_COORDINATE   ;C
                    dc.b RIGHT_X_COORDINATE   ;D
                    dc.b LEFT_X_COORDINATE    ;E
                    dc.b LEFT_X_COORDINATE    ;F
                    dc.b RIGHT_X_COORDINATE   ;G

coordinateTableY1:  dc.b TOP_Y_COORDINATE     ;A
                    dc.b TOP_Y_COORDINATE     ;B
                    dc.b MIDDLE_Y_COORDINATE  ;C
                    dc.b BOTTOM_Y_COORDINATE  ;D
                    dc.b BOTTOM_Y_COORDINATE  ;E
                    dc.b MIDDLE_Y_COORDINATE  ;F
                    dc.b MIDDLE_Y_COORDINATE  ;G

coordinateTableY2:  dc.b TOP_Y_COORDINATE     ;A
                    dc.b MIDDLE_Y_COORDINATE  ;B
                    dc.b BOTTOM_Y_COORDINATE  ;C
                    dc.b BOTTOM_Y_COORDINATE  ;D
                    dc.b MIDDLE_Y_COORDINATE  ;E
                    dc.b TOP_Y_COORDINATE     ;F
                    dc.b MIDDLE_Y_COORDINATE  ;G  

*Offset for indexing the segment coordinate table
coordinateTableOffset dc.b 6

*Offset for printing the tens digit
printOffset dc.b 0

*Flag for determining if we finished printing
printFlag dc.b 0

*Win screen message
player1String dc.b 'Player 1 Wins! Press R to play again.',0

*Lose screen message
player2String dc.b 'Player 2 Wins! Press R to play again',0


morePadding dc.l 0













*~Font name~Courier~
*~Font size~10~
*~Tab type~1~
*~Tab size~4~
