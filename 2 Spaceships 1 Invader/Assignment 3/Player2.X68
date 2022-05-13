*-----------------------------------------------------------
* Title      :Player2
* Written by :
* Date       :
* Description:This file handles all of the input, updating, and drawing of the Player entity for the game
*             The player can move left and right (but not off screen), and can press space to shoot a bullet.
*-----------------------------------------------------------

*COMMANDS 
SET_PEN_COLOR_COMMAND equ 80
SET_FILL_COLOR_COMMAND equ 81
DRAW_RECTANGLE_COMMAND equ 87
GET_USER_INPUT_COMMAND equ 19

*The core attributes of the player to help draw them to the screen, and initialize their starting position
PLAYER_WIDTH equ 20
PLAYER_HEIGHT equ 20
PLAYER_TOP_Y_POSITION equ 111440
PLAYER2_STARTING_X_POSITION equ 111140
PLAYER_RIGHT_VELOCITY equ 35
PLAYER_LEFT_VELOCITY equ -35
RIGHT_SIDE_OF_SCREEN equ 640

LEFT_ARROW equ $25
RIGHT_ARROW equ $27
ZERO_KEYPAD equ $2D

GREEN equ $00008000

*Initialize the Player before the gameLoop is called
initPlayer2
    lea player2X1, a0
    lea player2Y1, a1
    lea player2X2, a2
    lea player2Y2, a3
    
    *init x position
    move.l #PLAYER2_STARTING_X_POSITION, (a0)
    move.l #PLAYER2_STARTING_X_POSITION, (a2)
    add.l #PLAYER_WIDTH, (a2)
    
    *set ball y position and height
    move.l #PLAYER_TOP_Y_POSITION<<8, (a1)
    move.l #PLAYER_TOP_Y_POSITION<<8, (a3)
    add.l #PLAYER_HEIGHT, (a3)
    
    *init the number of bullets to 0 since we don't have any yet
    lea numBullets2, a4
    move.l #0, (a4)

    rts

*Called every frame in the game loop, used to get input from the player which updates the position, OR shoots a bullet depending on the key press    
updatePlayer2: 
    movem.l ALL_REG, -(sp)
    
    clr.l d1
    clr.l d2
    clr.l d3
    clr.l d4
    
    lea player2X1, a0
    lea player2Y1, a1
    lea player2X2, a2
    lea player2Y2, a3
    move.l (a0), d7
    move.l (a1), d2
    move.l (a2), d3
    move.l (a3), d4
    
    *If the player presses the '6' key, call the function to move player object right
    move.l #$27, d1
    move.b #GET_USER_INPUT_COMMAND, d0
    trap #15
    move.l d1, d5
    andi.l #$FF, d5
    bne rightPressed
    
    *If the player presses the '4' key, call the function to move player object left
    move.l #$25, d1
    move.b #GET_USER_INPUT_COMMAND, d0
    trap #15
    move.l d1, d5
    andi.l #$FF, d5
    bne leftPressed
    
    *Check to see if space was already pressed the previous frame before. If it was, don't detect input from space this iteration of the update
    *This is because when a button is pressed, Easy68K detects the input for multiple frames, so the bullets were getting spammed too much
    *This gets rid of the spam issue, and forces the user to shoot 1 bullet at a given time
    clr d1
    lea numBullets2, a1
    move.l (a1), d1
    cmpi.l #1, d1
    beq return2
   
    *If the player presses 'Ctrl', call function to shoot bullet
    move.l #$11, d1
    move.b #GET_USER_INPUT_COMMAND, d0
    trap #15
    move.l d1, d5
    andi.l #$FF, d5
    bne firePressed
    
    movem.l (sp)+, ALL_REG
    rts
    
*Called if player presses 'D'
rightPressed:
    *Add right velocity to the current player position to get the new position
    move.l #PLAYER_RIGHT_VELOCITY, d6
    add.l d6, d7
    
    *Compare the new position to our screen bounds. If it's too big, we have to undo the last addition we made
    cmpi.l #(RIGHT_SIDE_OF_SCREEN-PLAYER_WIDTH)<<8, d7
    bge subtractOne2
    
    move.l d7, (a0)
    movem.l (sp)+, ALL_REG
    rts

*Called if player presses 'A'
leftPressed:
    *Add left velocity to current player position to get new position
    move.l #PLAYER_LEFT_VELOCITY, d6
    add.l d6, d7
    
    *Compare new position to screen bounds. If it's too small, undo the last addition we made
    cmpi.l #PLAYER_STARTING_X_POSITION, d7
    ble addOne2
    
    move.l d7, (a0)
    movem.l (sp)+, ALL_REG
    rts

*Called if player presses 'Space'    
firePressed:
    move.l d7, (a0)
    
    *Load the bullet's coordinate table, the number of bullets, and the bulletActive flag
    lea bulletTable2X1, a1
    lea bulletTable2Y1, a2
    lea bulletTable2X2, a3
    lea bulletTable2Y2, a4
    lea numBullets2, a5
    lea bullet2Active, a6
    
    ;Shift to get the X coordinate of the player, which is used to init the X coordinate of the bullet when fired
    move.l d7, d1
    asr.l #8, d1
    move.l d1, d3
    add.l #PLAYER_WIDTH, d3
    
    ;Move the players current position to the Bullet Coordinate tables, so we can init the bullet position relative to the player
    move.l d1, (a1)+
    move.l d2, d5
    asr.l #8, d5
    move.l d5, (a2)+
    move.l d3, (a3)+
    move.l d4, (a4)+
    
    ;Add 1 to number of bullets
    move.l (a5), d6
    addi.l #1, d6
    move.l d6, (a5)
    
    ;Change value of bulletActive flag so we know we're firing a shot
    move.l #1, (a6)
    
    movem.l (sp)+, ALL_REG
    rts

*Called to prevent going too far right off screen
subtractOne2:
    subi.l #PLAYER_RIGHT_VELOCITY, d7
    move.l d7, (a0)
    movem.l (sp)+, ALL_REG
    rts
*Called to rpevent going too far left off screen
addOne2:
    addi.l #-PLAYER_LEFT_VELOCITY, d7
    move.l d7, (a0)
    movem.l (sp)+, ALL_REG
    rts
*Do nothing and return
return2:
    move.l d7, (a0)
    movem.l (sp)+, ALL_REG
    rts

*Using the updated coordinates from the update function, draw the player at their current position  
drawPlayer2:
    movem.l ALL_REG, -(sp)
    
    lea player2X1, a0
    lea player2Y1, a1
    lea player2X2, a2
    lea player2Y2, a3
    move.l (a0), d7
    move.l (a1), d2
    move.l (a2), d3
    move.l (a3), d4
    
    move.l #GREEN, d1
    move.b #SET_PEN_COLOR_COMMAND, d0
    trap #15
    move.b #SET_FILL_COLOR_COMMAND, d0
    trap #15
    move.b #DRAW_RECTANGLE_COMMAND, d0
    
    *Shift to get the value of the X coordinates since we're using fixed point math
    move.l d7, d1
    asr.l #8, d1
    move.l d1, d3
    add.l #PLAYER_WIDTH, d3
    
    *Shift to get the value of the Y coordinates since we're using fixed point math
    asr.l #8, d2
    asr.l #8, d2
    move.l d2, d4
    add.l #PLAYER_HEIGHT, d4
    trap #15
    
    *Reset the players Y position
    ;move.l #PLAYER_TOP_Y_POSITION,d2
    
    *Store the value of the current position of player in table
    
    ;move.l d1, (a1)
    ;move.l d3, (a2)
    ;move.l d2, (a3)
    ;move.l d4, (a4)
   
    movem.l (sp)+, ALL_REG
    
    rts

*Holds the respective coordinates of current player position
player2X1 ds.l 1
player2X2 ds.l 1
player2Y1 ds.l 1
player2Y2 ds.l 1

*Number of active bullets
numBullets2 ds.l 1

*Table to hold the value of the position of the bullet
bulletTable2X1 ds.l 20
bulletTable2X2 ds.l 20
bulletTable2Y1 ds.l 20
bulletTable2Y2 ds.l 20 

*Flag to tell if we have a bullet being fired or not
bullet2Active dc.l 0




*~Font name~Courier~
*~Font size~10~
*~Tab type~1~
*~Tab size~4~
