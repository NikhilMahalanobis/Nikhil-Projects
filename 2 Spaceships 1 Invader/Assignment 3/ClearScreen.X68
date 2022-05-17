*-----------------------------------------------------------
* Title      :Clear Screen
* Written by :
* Date       :
* Description:A Psuedo Clear Screen function that actually just draws black rectangles over specific parts
*             of the screen. Specifically, it redraws the area of the screen that the players & alien move over
*             and this also holds the function that draws the bitmap background that the score is drawn over
*-----------------------------------------------------------

*Parameters for the bitmap draw from Assignment 2
*We only wanna draw over the score area which is 40x240
BITMAP_CHUNK_TOP_X EQU 0
BITMAP_CHUNK_TOP_Y EQU 0
BITMAP_CHUNK_WIDTH EQU 40
BITMAP_CHUNK_HEIGHT EQU 240
BITMAP_CHUNK_Y_START_POS EQU BITMAP_CHUNK_TOP_Y+BITMAP_CHUNK_HEIGHT
BITMAP_CHUNK_X_END_POS EQU BITMAP_CHUNK_TOP_X+BITMAP_CHUNK_WIDTH
PRINT_TOP_X EQU 0
PRINT_TOP_Y EQU 0
BMP_FILE_SIZE_OFFSET EQU 2
BMP_START_ADDRESS_OFFSET EQU 10
BMP_WIDTH_OFFSET EQU 18
BMP_HEIGHT_OFFSET EQU 22
BMP_BITDEPTH_OFFSET EQU 28

*Commands
OPEN_EXISTING_FILE_TRAP_CODE EQU 51 
OPEN_NEW_FILE_TRAP_CODE EQU 52
READ_FILE_TRAP_CODE EQU 53
DISPLAY_FILE_TRAP_CODE EQU 58
PEN_COLOR_TRAP_CODE EQU 80
DRAW_PIXEL_TRAP_CODE EQU 82
SET_PEN_COLOR_COMMAND equ 80
SET_FILL_COLOR_COMMAND equ 81
DRAW_RECTANGLE_COMMAND equ 87

BLACK equ $00000000

*Function called to draw a black rectangle with X1 = 0, X2 = 640, Y1 = 250, Y2 = 480
*This black rectangle effectivly works as a screen clear, as it will draw over the main area of the screen
*with black, since that's the color of my background, and lets us draw over again.
fakeClearScreen:
    movem.l ALL_REG, -(sp)
    
    move.l #BLACK, d1
    move.b #SET_PEN_COLOR_COMMAND, d0
    trap #15
    move.b #SET_FILL_COLOR_COMMAND, d0
    trap #15
    move.b #DRAW_RECTANGLE_COMMAND, d0
    
    move.l #40, d1
    move.l #0, d2
    move.l #640, d3
    move.l #480, d4
    trap #15
    
    move.l #0, d1
    move.l #250, d2
    move.l #640, d3
    move.l #480, d4
    trap #15
    
    movem.l (sp)+, ALL_REG
    rts
    
*Function from Assignment 2 that draws the bitmap crop
drawBitmap:
;PrintCrop:
;Reads the size of the bmp in bytes, and grabs offset for starting location of pixel array, store both in mem     
    ;First grab the size of the file
     movem.l ALL_REG, -(sp)
     
     lea bmpFile,a1
     lea bmpFileSize, a2
     move.l BMP_FILE_SIZE_OFFSET(a1), d1
     
    ;Size is written in Litten-Endian, so rol -> swap -> rol to get Big-Endian val
     rol.w #8, d1                   
     swap.w d1
     rol.w #8, d1
     move.l d1, (a2)                ;Save Big-Endian value to memory
     
     
     ;Now grab offset for start of pixel data (also Little-Endian, so must convert), and store in memory
     lea bmpPixelOffset, a2
     move.l BMP_START_ADDRESS_OFFSET(a1), d2
     swap.w d2
     rol.w #8, d2
     move.l d2, (a2)

     
;Now grab the Width, Height, Bit Depth, calculate padding, and store them in mem:
     lea bmpFile, a1
     lea bmpFileWidth, a2
     lea bmpFileHeight, a3
     lea bmpFilePadding, a4
     lea bmpBitDepth, a5
     
     move.l BMP_WIDTH_OFFSET(a1), d1
     move.l BMP_HEIGHT_OFFSET(a1), d2
     move.l BMP_BITDEPTH_OFFSET(a1), d3
     
     ;Again, switching from Little-Endian -> Big-Endian
     swap.w d1
     rol.w #8, d1
     swap.w d2
     rol.w #8, d2
     swap.w d3
     rol.w #8, d3
     
     move.w d1, (a2)
     move.w d2, (a3)
     move.w d3, (a5)
     divu #4, d2
     
     swap d2            ;Padding
     move.w d2, (a4)
     
     
;Add pixel offset to starting address of bitmap to start reading pixels
    clr d1
    clr d2
    lea bmpPixelOffset, a2
    move.l (a2), d2
    adda.l d2, a1

;Prepare regs for drawing loop
;d0 - Trap Code
;d1 - 2 purposes; hold value for pixel color, and then hold value of X-coord for drawing
;d2 - Holds value of Y-coord for drawing, initialized to Height, but changes to BITMAP_CHUNK_Y_START_POS for crop
;d3 - Holds the value of the width of the file, used to compare agains the X-coord
;d4 - Since d1 has 2 purposes, we need to save off the position of the X-coord when we reset pixel color, so store into d4 before loop restarts
;d5 - Number of bytes to skip to get to start of crop
;d6 - Number of bytes to skip on each iteration of loop
;d7 - Extra reg to copy values and add/divide

    clr d0
    clr d1
    clr d2
    
    lea bmpFileWidth, a2
    lea bmpFileHeight, a3
    lea bmpFilePadding, a4
    lea bmpBitDepth, a5
    move.w (a2), d3
    move.w (a3), d2
    move.w (a5), d7
    cmpi.w #32, d7                  ;Check the bit depth of the file, depending if 24 or 32 bit, our math slightly changes
    beq BitDepth32

;Branch for 24bit files, will calculate how much of image to skip/crop
BitDepth24:    
    ;This arithmetic will give us the number of bytes to skip to start our crop
    ;Formula is: ((bmpFileHeight - (Bitmap_Chunk_Top_Y+Bitmap_Chunk_Height))*Bitmap_Chunk_Width + Bitmap_Chunk_Top_X
    move.l d2, d5
    sub.l  #BITMAP_CHUNK_Y_START_POS, d5
    mulu d3, d5
    addi.l #BITMAP_CHUNK_TOP_X, d5
    move.l d5, d7
    add.l d7, d5
    add.l d7, d5
    
    adda.l d5, a1
    
    ;This arithmetic will give us the number of bytes to skip during each iteration of the loop
    ;Formual is: (bmpFileWidth - Bitmap_Chunk_Width) * 3
    move.l d3, d6
    subi.l #BITMAP_CHUNK_WIDTH, d6
    move.l d6, d7
    add.l d7, d6
    add.l d7, d6
    
    move.l #(PRINT_TOP_Y+BITMAP_CHUNK_HEIGHT), d2
    move.l #(PRINT_TOP_X+BITMAP_CHUNK_WIDTH), d3
    move.l #PRINT_TOP_X, d4
    
    jmp ContinueRow24

;Branch for 32 bit images, will calculate how much of the image to skip/crop    
BitDepth32:
    ;This arithmetic will give us the number of bytes to skip to start our crop
    ;Formula is: ((bmpFileHeight - (Bitmap_Chunk_Top_Y+Bitmap_Chunk_Height))*Bitmap_Chunk_Width + Bitmap_Chunk_Top_X
    move.l d2, d5
    sub.l  #BITMAP_CHUNK_Y_START_POS, d5
    mulu d3, d5
    addi.l #BITMAP_CHUNK_TOP_X, d5
    move.l d5, d7
    add.l d7, d5
    add.l d7, d5
    add.l d7, d5
    
    adda.l d5, a1
    
    ;This arithmetic will give us the number of bytes to skip during each iteration of the loop
    ;Formual is: (bmpFileWidth - Bitmap_Chunk_Width) * 4
    move.l d3, d6
    subi.l #BITMAP_CHUNK_WIDTH, d6
    move.l d6, d7
    add.l d7, d6
    add.l d7, d6
    add.l d7, d6
    
    move.l #(PRINT_TOP_Y+BITMAP_CHUNK_HEIGHT), d2
    move.l #(PRINT_TOP_X+BITMAP_CHUNK_WIDTH), d3
    move.l #PRINT_TOP_X, d4
  
  
;Loop to print the 32 bit file. 32 bit bitmaps have 4 bytes per pixel, so just move.l
ContinueRow32:
    clr d1 
  
    move.l (a1)+, d1
    ror.l #8, d1
    move.l #PEN_COLOR_TRAP_CODE, d0
    trap #15
    clr d1
    
    ;d1 is being used twice in this loop (pixel color, and draw pixel x coordinate),
    ;so copy X-coord value into d4 every time to save, and move back during each iteration of the loop
    move.l d4, d1
    move.l #DRAW_PIXEL_TRAP_CODE, d0
    trap #15
    
    addi #1, d1                             ;Increment X-coord
    move.l d1, d4                           :Copy X-coord
    cmp.l d1, d3 
    bne ContinueRow32
    move.w (a4), d7                         ;Get file padding, add it to address when we reach end of row
    adda.l d7, a1
    move.l #PRINT_TOP_X, d1                 ;If we reach here, reset X-coord to default value (and copy into d4 to save)
    move.l #PRINT_TOP_X, d4          
    subi.l #1, d2                           ;DECREMENT Y-coord, remember we start at BOTTOM not TOP of screen, so subtract
    adda.l d6, a1
    cmpi.l #PRINT_TOP_Y, d2
    bne ContinueRow32
    
    rts

;Loop to print 24 bit file.then we calculate the crop position based on the input parameters we were given, and the amount of bytes to skip in the pixel array
;as calculated above
ContinueRow24:
    clr d1 
     
    ;Grab the pixel color RGB value from the array. Since this works on 24-bit files, we cannot load in words or longs,
    ;so we must load in the pixel values one byte at a time. After loading in the first byte, we must swap in order to put it into it's correct position
    ;Then for the second byte we must read and roll it left 8 bytes to get THAT color in it's correct position
    move.b (a1)+, d1
    swap d1
    move.b (a1)+, d1
    rol.w #8, d1
    move.b (a1)+, d1
    move.l #PEN_COLOR_TRAP_CODE, d0
    trap #15
    clr d1
    
    ;d1 is being used twice in this loop (pixel color, and draw pixel x coordinate),
    ;so copy X-coord value into d4 every time to save, and move back during each iteration of the loop
    move.l d4, d1
    move.l #DRAW_PIXEL_TRAP_CODE, d0
    trap #15
    
    addi #1, d1                             ;Increment X-coord
    move.l d1, d4                           :Copy X-coord
    cmp.l d1, d3 
    bne ContinueRow24
    move.w (a4), d7                         ;Get file padding, add it to address when we reach end of row
    adda.l d7, a1
    move.l #PRINT_TOP_X, d1                 ;If we reach here, reset X-coord to default value (and copy into d4 to save)
    move.l #PRINT_TOP_X, d4          
    subi.l #1, d2                           ;DECREMENT Y-coord, remember we start at BOTTOM not TOP of screen, so subtract
    adda.l d6, a1
    cmpi.l #PRINT_TOP_Y, d2
    bne ContinueRow24
   
    movem.l (sp)+, ALL_REG 
    rts 
    
 
    
         


;Mem to store the size of img file
bmpFileSize ds.l 1

;Mem to store offset of pixel array location in file
bmpPixelOffset ds.l 1

;Mem to store bitmap file width
bmpFileWidth ds.l 1

;Mem to store bitmap file height
bmpFileHeight ds.l 1

;Mem to store Bit Depth of file
bmpBitDepth ds.l 1

;Mem to store padding for file sizes non-divisible by 4
bmpFilePadding ds.w 1

;Mem to store actual array info of the bitmap, place in same directory as source file
bmpFile INCBIN 'paint.bmp'


*~Font name~Courier~
*~Font size~10~
*~Tab type~1~
*~Tab size~4~
