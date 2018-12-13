# Salut

.set

.text
    Lw $t0 , work
    LW $7, 100($6)
    ADDI $14,$0,3
boucle:
    BEQ $t0 , $t1 , byebye
    NOP
    addi $t1 , $t1 , 1
    J boucle
    NOP
    BLT $1,$2,byebye
byebye:
    JAL viteviteauru
    AND $t8, $t6, $zero

# Ça va niquel

.data
work:
    .word 17
    .word informatic
    .asciiz "J'ai envie de dormir..."
    .word 23
    .space 5
    .byte 3
    .word test
    

#Toujours plus

.bss
informatic:
    .space 3
    .work error
