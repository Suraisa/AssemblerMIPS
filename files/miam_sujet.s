# TEST_RETURN_CODE = PASS
# allons au ru
.set noreorder
.data
    .asciiz "testzorgnkejrnkjerng","test"
   .byte 2, 2, 0x12
.text
    lw $28,$v1