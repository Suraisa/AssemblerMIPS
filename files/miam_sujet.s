# TEST_RETURN_CODE = PASS
# allons au ru
.set noreorder
.text
    Lw $t0 , lunchtime
    LW $6, -200($7)
    ADDI $t1,$zero,8
boucle:
    BEQ $t0 , $t1 , byebye
    NOP
    addi $t1 , $t1 , 1
    J boucle
    NOP
    BLT $1,$2,byebye
byebye:
    JAL viteviteauru

.data
lunchtime: 
    .word 12
    .word menu
    .asciiz "ils : \"au ru!\""
.bss 
menu:
    .space 24
