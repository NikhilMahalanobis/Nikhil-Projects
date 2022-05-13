*-----------------------------------------------------------
* Title      : Bullet
* Written by :
* Date       :
* Description: This file handles all of the updating and drawing of the Bullet entity in the game
*              This file checks if any bullets are being fired, and if so, to move and draw them across the screen
*              Attribute of the bullet can be altered here as well
*-----------------------------------------------------------

ALL_REG reg D0-D7/A0-A6

;Trap code commands for Bullet
SET_PEN_COLOR_COMMAND equ 80
SET_FILL_COLOR_COMMAND equ 81
DRAW_CIRCLE_COMMAND equ 88
GET_USER_INPUT_COMMAND equ 19

;EQU values for player height/width, starting position, boundaries, and velocity
BULLET_WIDTH equ 16
BULLET_HEIGHT equ 16
BULLET_TOP_Y_POSITION equ 111400
BULLET_STARTING_X_POSITION equ 40
BULLET_VELOCITY equ 45
BULLET_ACCELERATION equ 1
TOP_SIDE_OF_SCREEN equ 0

PURPLE equ $00800080

*Checks the current number of bullets being fired for the gameLoop
checkNumBullets:  
    movem.l ALL_REG, -(sp)
  
    *Compare numBullets to 0. If not 0, then we must update/draw bullets
    lea numBullets, a1
    move.l (a1), d5
    cmpi.l #0, d5
    
    movem.l (sp)+, ALL_REG
    rts
 
*Updates the position of the bullet after being fired   
*Since the bullet only moves vertically, we only mess w/ the Y coordinates
updateBullet:
    movem.l ALL_REG, -(sp)
    
    *Get current Y coordinate of the bullet, and add velocity to change position
    lea bulletTableY1, a1
    lea bulletTableX1, a2
    move.l #BULLET_VELOCITY, d5
    move.l (a1), d6
    sub.l d5,d6

   *Comparison to make sure we don't go beyond the bounds of the top of the screen
   *If we do, delete the bulelt
    cmpi.l #(TOP_SIDE_OF_SCREEN)<<8, d6
    ble deleteBullet
    
    cmpi.l #45, (a2)
    ble deleteBullet
    
*Called to return from the deleteBullet function
returnFromDelete:
    move.l d6, (a1)+
        
    movem.l (sp)+,ALL_REG
    
    rts 
            
*Draw the Bullet at it's current updated position
drawBullet:
    movem.l ALL_REG, -(sp)
    
    move.l #PURPLE, d1
    move.b #SET_PEN_COLOR_COMMAND, d0
    trap #15
    move.b #SET_FILL_COLOR_COMMAND, d0
    trap #15
    
    *Load the bullet's coordinates from the table, shift accordingly due to fixed point,
    *and draw the bullet at the current positio
    move.b #DRAW_CIRCLE_COMMAND, d0
    lea bulletTableX1, a1
    lea bulletTableY1, a2
    lea bulletTableX2, a3
    lea bulletTableY2, a4    
    move.l (a1), d1
    move.l (a2), d2
    asr.l #8, d2
    move.l d2, d4
    add.l #BULLET_HEIGHT, d4
    move.l d1, d3
    add.l #BULLET_WIDTH, d3
    trap #15
    

    movem.l (sp)+, ALL_REG
    rts

*Called to delete the Bullet
deleteBullet:
    *All we have to do to 'delete' the bullet is decrease the value of numBullets
    *This effectively stops the bullet from being updated/printed
    lea numBullets, a3
    subi.l #1, (a3)
    ;move.l #1, (a2)
    
    bra returnFromDelete

*Velocity of bullet
bulletVelocity dc.l 10











    










*~Font name~Courier~
*~Font size~10~
*~Tab type~1~
*~Tab size~4~
