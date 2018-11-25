# TEST_RETURN_CODE = PASS
# allons au ru
.set noreorder
.data
    .asciiz "trnkjerng","test"
    .word 2, -128, 0xff
.text
    lw $28, -30($4)
    XOR $8, $8, $8