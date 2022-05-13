*-----------------------------------------------------------
* Title      : Interactive Assembly Assignment
* Written by : Nikhil Mahalanobis
* Date       : 10/18/2021
* Description:  This Interactive Assembly Assignment is a "game" where the player must shoot the aliens 
*               heading towards them a certain amount of times (15) before the alien reaches the bottom of the screen. 
*               The alien spawns in a random position along the top side of the screen, and every time one is destroyed,
*               acceleration is applied and the velocity increases every iteration. 
*               
*               This program is made up of several other files that manage the different parts of the game including:
*               Player.x68 - File that handles the creation, input, and drawing of the player
*               Bullet.x68 - File that handles the creation, movement, and drawing of the bullet the player can shoot
*               Alien.x68 - File that handels the creation, movement, drawing, of the Alien the player has to shoot, as well as keeping track of score
*               LED.x68 - File that handles the drawing and updating of a seven segment LED, that indicates the current score to the player, which also affects the win/lose condition
*
*
* CONTROLS: A/D - Move left/right respectively
*           Space - Shoot bullet 
*
*
* OBJECTIVE: Don't let the Alien get to the end of the screen!
*-----------------------------------------------------------
    
    
    ORG    $1000

    INCLUDE "Player.x68"
    INCLUDE "Bullet.x68"
    INCLUDE "Alien.x68"
    INCLUDE "LED.x68"
    INCLUDE "Player2.x68"
    INCLUDE "Bullet2.x68"
    INCLUDE "LED2.x68"
    INCLUDE "ClearScreen.x68"
    INCLUDE "IntroScreen.x68"
    
START:                  

DRAWING_MODE_TRAPCODE equ 92
DOUBLE_BUFFER_MODE equ 17
REPAINT_SCREEN_TRAPCODE equ 94
CLEAR_SCREEN_COMMAND equ 11
CLEAR_SCREEN_MAGIC_VAL equ $FF00

    jsr drawIntro
drawStartScreen:
    
    move.l #'R', d1
    move.b #GET_USER_INPUT_COMMAND, d0
    trap #15
    move.l d1, d2
    andi.l #$FF, d2
    bne initGameLoop
    bra drawStartScreen

initGameLoop:
    jsr clearScreen
    move.b #DRAWING_MODE_TRAPCODE, d0
    move.b #DOUBLE_BUFFER_MODE, d1
    trap #15
    
    ;Initialize the Alien and Player entities
    ;Also, draw the bitmap and draw the inital scores over it
    jsr initAlien
    jsr initPlayer
    jsr initPlayer2
    jsr initLED
    jsr drawBitmap
    jsr updateScore
    jsr updateScore2
gameLoop:
    ;jsr clearScreen
    jsr fakeClearScreen
    
    ;First update, then draw the player
    jsr updatePlayer    
    jsr drawPlayer
    
    ;Update/draw player 2
    jsr updatePlayer2
    jsr drawPlayer2
        
    ;Get the number of bullets that are currently being fired.
    ;If there's 0 bullets, we can skip the Bullet update/draw
    jsr checkNumBullets
    beq bulletLoop2
    
bulletLoop:
    ;If we reach here, draw and update a bullet
    jsr updateBullet
    jsr drawBullet
    
bulletLoop2:
    *Now check if player 2 has fired any bullets, and update
    jsr checkNumBullets2
    beq continue
    
    jsr updateBullet2
    jsr drawBullet2
    
;If we have 0 bullets, skip update/draw for them    
continue:
    ;Update, then draw the Alien
    jsr updateAlien
    jsr drawAlien
    
    ;Call to detect collision between the bullet and the Alien
    jsr detectCollision
    
    ;Call to update the score every time an Alien is defeated
    ;jsr updateScore
    ;jsr updateScore2
    jsr swapBuffers
    
    ;Call to check if the player has reached win condition
    jsr checkP1WinScore
    beq exitGameLoopWinP1
    
    ;Call to check if player has reached lose condition
    jsr checkP2WinScore
    beq exitGameLoopWinP2
    
    ;If we reach here, restart the game loop
    bra gameLoop

    ;If the player wins, clear the screen, print the message, swap buffers, then halt
exitGameLoopWinP1:
    jsr clearScreen
    jsr printP1ScreenWin
    jsr swapBuffers
    jsr restartGameLoop
    
    SIMHALT
    
    ;Same as if player wins, but instead print the lose message
exitGameLoopWinP2:
    jsr clearScreen
    jsr printP2ScreenWin
    jsr swapBuffers
    jsr restartGameLoop
    
    SIMHALT
   
restartGameLoop:
    clr.l d0
    clr.l d1
    clr.l d2
    
    move.l #'R', d1
    move.b #GET_USER_INPUT_COMMAND, d0
    trap #15
    move.l d1, d2
    andi.l #$FF, d2
    bne restartGame
    bra restartGameLoop
 
restartGame:
   jmp initGameLoop
   
swapBuffers:
    move.b #REPAINT_SCREEN_TRAPCODE, d0
    trap #15
    rts    
    
clearScreen:
    movem.l ALL_REG,-(sp)

    move.b #CLEAR_SCREEN_COMMAND, d0
    move.l #CLEAR_SCREEN_MAGIC_VAL, d1
    trap #15
    
    movem.l (sp)+,ALL_REG

    rts


    END    START       
























*~Font name~Courier~
*~Font size~10~
*~Tab type~1~
*~Tab size~4~
