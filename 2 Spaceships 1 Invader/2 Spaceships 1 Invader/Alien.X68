*-----------------------------------------------------------
* Title      : Alien
* Written by :
* Date       :
* Description: This file handles the initialization, updating, interaction, and drawing of the alien 
*              The alien is spawned at the top of the screen at a random X position, and will move vertically
*              down the screen until it collides with a bullet, or reaches the bottom of the screen
*-----------------------------------------------------------

*Commands
GET_TIME_COMMAND equ 8
SET_PEN_COLOR_COMMAND equ 80
FILL_COLOR_COMMAND equ 81
GET_PIXEL_COLOR_COMMAND equ 83
DRAW_RECTANGLE_COMMAND equ 87

*Core alien attributes including width, height, velocity, acceleration, screen bounds
ALIEN_WIDTH equ 14
ALIEN_HEIGHT equ 14
ALIEN_TOP_Y_POSITION equ 11130
ALIEN_START_X_POSITION equ 100
ALIEN_RIGHT_VELOCITY equ 5
ALIEN_LEFT_VELOCITY equ -5
ALIEN_DOWN_ACCELERATION equ 4
BOTTOM_SIDE_OF_SCREEN equ 500
SUM_OF_RADII equ 15
SUM_OF_RADII_SQUARED equ 225

RED equ $000000FF
RANDOM_MASK equ $FFFFFF

*Initialize the alien before we start the game loop, and when it gets destroyed by a bulelt
initAlien:
    *Get a random spawn point
    jsr generateRandomSpawnPoint
    
    *Init Y coordinate
    move.l #ALIEN_TOP_Y_POSITION, d2
    
    *Store the init values of the Alien starting position in it's table
    lea alienTableX1, a0
    lea alienTableY1, a1
    lea alienTableX2, a2
    lea alienTableY2, a3
    move.l d7, (a0)
    move.l d7, (a2)
    move.l d2, (a1)
    move.l d2, (a3)
    addi.l #ALIEN_WIDTH, (a2)
    addi.l #ALIEN_HEIGHT, (a3)
    
    rts
 
*Updates the alien every frame of the game loop. Since it only moves vertically down, only change Y position   
updateAlien:
    movem.l ALL_REG, -(sp)
    
    ;Get our alien coordinate table ready, and move them to registers
    lea alienTableX1, a0
    lea alienTableY1, a1
    lea alienTableX2, a2
    lea alienTableY2, a3
    lea alienDownVelocity, a6
    move.l (a0), d1
    move.l (a1), d2
    move.l (a2), d3
    move.l (a3), d4
    
    *Add velocity to current Y coordinate to update position
    add.l (a6), d2
    
    *Compare to see if Alien reached bottom of the screen or not
    cmpi.l #(BOTTOM_SIDE_OF_SCREEN-ALIEN_HEIGHT)<<8, d2
    bge deleteAlien
    
    *If reached here, Alien didn't reach bottom, so store coordinates for next iteration of loop
    ;move.l d1, (a0)
    move.l d2, (a1)
    movem.l (sp)+, ALL_REG
    rts

*Function to handle 'deleting' the Alien, and setting Lose condition
deleteAlien:
    *If we get here, the alien made it to the bottom of screen, so set Lose flag for the gameLoop
    ;lea LoseFlag, a0
    ;addi.l #1, (a0) 
    
    jsr initAlien
    movem.l (sp)+, ALL_REG
    rts
   
*Function to draw Alien at the current updated position 
drawAlien:
    movem.l ALL_REG, -(sp)
    
    move.l #RED, d1
    move.b #SET_PEN_COLOR_COMMAND, d0
    trap #15
    move.b #SET_FILL_COLOR_COMMAND, d0
    trap #15
    
    *Get the current values from the Alien coordinate table, move to registers,
    *shift accordingly since we're using fixed point math, and draw it to the screen
    lea alienTableX1, a0
    lea alienTableY1, a1
    lea alienTableX2, a2
    lea alienTableY2, a3
    move.b #DRAW_CIRCLE_COMMAND, d0
    move.l (a0), d1
    move.l (a1), d2
    asr.l #8, d2
    move.l d1, d3
    move.l d2, d4
    add.l #ALIEN_HEIGHT, d4
    add.l #ALIEN_WIDTH, d3
    trap #15
    
    move.l d1, (a0)
    movem.l (sp)+, ALL_REG
    rts

*Function called every frame to detect if there's collision between an Alien and a Bullet
detectCollision:
    movem.l ALL_REG, -(sp)
    
    *If the number of Bullets is currently 0, we don't have any collision to detect, so just skip this call
    lea numBullets, a1
    move.l (a1), d1
    cmpi.l #0, d1
    beq checkBullet2    
    
    *Get the current X/Y position of the Bullet and the Alien, and put them into reg
    lea bulletTableX1, a0
    lea bulletTableY1, a1
    lea alienTableX1, a2
    lea alienTableY1, a3
    
    *The given coordinates are at the Top-Left of the given shape.
    *We need the coordinates at the CENTER of the shape to detect collision,
    *so simply add half of the width to the X, and half of the height to the Y to get the Center coordinates
    move.l (a0), d1
    add.l #(BULLET_WIDTH/2), d1
    move.l (a1), d2
    asr.l #8, d2
    add.l #(BULLET_HEIGHT/2), d2
     
    *Getting center of Alien 
    move.l (a2), d3
    add.l #(ALIEN_WIDTH/2), d3
    move.l (a3), d4
    asr.l #8, d4
    add.l #(ALIEN_HEIGHT/2), d4
    
    *To detect collision between two circles, we must compare the distance between their centers
    *to the sum of their radii. If this distacnce is smaller than the sum of their radii, then the 2 circles are colliding.
    *The formula we need is: sqrt((x1-x2)^2 + (y1-y2)^2) <= SUM_OF_RADII
    sub.l d1, d3
    muls d3, d3
    sub.l d2, d4
    muls d4, d4
    add.l d4, d3
    
    *Since hard to get the sqaure root, just square both sides to get easier calculation
    *New formula is: (x1-x2)^2 + (y1-y2)^2 <= SUM_OF_RADII_SQUARED
    *If we have a collision, then delete the Alien
    cmpi.l #SUM_OF_RADII_SQUARED, d3
    ble delete

checkBullet2:    
    lea numBullets2, a1
    move.l (a1), d1
    cmpi.l #0, d1
    beq skip
    
    *If we reach here, time to detect collision for Player 2!
    lea bulletTable2X1, a0
    lea bulletTable2Y1, a1
    lea alienTableX1, a2
    lea alienTableY1, a3
    
    *Get the center of Bullet 2
    move.l (a0), d1
    add.l #(BULLET_WIDTH/2), d1
    move.l (a1), d2
    asr.l #8, d2
    add.l #(BULLET_HEIGHT/2), d2
    
    *Now get center of alien again
    move.l (a2), d3
    add.l #(ALIEN_WIDTH/2), d3
    move.l (a3), d4
    asr.l #8, d4
    add.l #(ALIEN_HEIGHT/2), d4
    
    *To detect collision between two circles, we must compare the distance between their centers
    *to the sum of their radii. If this distacnce is smaller than the sum of their radii, then the 2 circles are colliding.
    *The formula we need is: sqrt((x1-x2)^2 + (y1-y2)^2) <= SUM_OF_RADII
    sub.l d1, d3
    muls d3, d3
    sub.l d2, d4
    muls d4, d4
    add.l d4, d3
    
    *Since hard to get the sqaure root, just square both sides to get easier calculation
    *New formula is: (x1-x2)^2 + (y1-y2)^2 <= SUM_OF_RADII_SQUARED
    *If we have a collision, then delete the Alien
    cmpi.l #SUM_OF_RADII_SQUARED, d3
    ble delete2
    
    movem.l (sp)+, ALL_REG
    rts
    
*Called when the Alien and Bullet collide
delete:
    ;Delete Bullet
    lea numBullets, a0
    subi.l #1, (a0)
    
    ;Increase velocity of alien on next spawn
    lea alienDownVelocity, a0
    addi.l #ALIEN_DOWN_ACCELERATION, (a0)
    
    ;Increase score
    lea player1Score, a0
    addi.l #1, (a0)
    
    ;Respawn the Alien
    jsr initAlien
    
    movem.l (sp)+, ALL_REG
    
    *At the same time that we delete the bullet & and alien,
    *we need to draw the bitmap bg behind the score, then update the scores
    jsr drawBitmap
    jsr updateScore
    jsr updateScore2
    rts

*Called when Alien and Bullet 2 collide
delete2:
    lea numBullets2, a0
    subi.l #1, (a0)
    
    lea alienDownVelocity, a0
    addi.l #1, (a0)
    
    lea player2Score, a0
    addi.l #1, (a0)
    
    jsr initAlien
    
    movem.l (sp)+, ALL_REG
    
    *At the same time that we delete the bullet & and alien,
    *we need to draw the bitmap bg behind the score, then update the scores
    jsr drawBitmap
    jsr updateScore
    jsr updateScore2
    rts
    
*Called to skip collision detection
skip:
    movem.l (sp)+, ALL_REG,
    rts

*Random number generator seeded with time that generates spawn position of Alien    
generateRandomSpawnPoint:
    clr d7
    
    *Get current time for seed
    move.b #GET_TIME_COMMAND, d0
    trap #15
    
    *Take the seed, and it with a mask to get a random value, and divide that value by 600 (number determined by size of the screen)
    andi.l #$FFFFFF, d1
    divu #540, d1
    *Swaping gives us the remainder, and since we divided by 600, we will get a remainder from 0-599
    *Thus, we effectively get a random number generator from the range of 0-599, which is then moved to be used as the init position of an Alien on spawn
    swap d1
    addi.w #60, d1
    move.w d1, d7
    
    rts
    
*Velocity of Alien
alienDownVelocity dc.l 7    

*Table to hold current updated position of Alien coordinates
alienTableX1: ds.l 20
alienTableX2: ds.l 20
alienTableY1: ds.l 20
alienTableY2: ds.l 20

*Holds value of the score
player1Score dc.l 0

player2Score dc.l 0

*Flag to be set if we detect lose condition
LoseFlag dc.l 0














*~Font name~Courier~
*~Font size~10~
*~Tab type~1~
*~Tab size~4~
