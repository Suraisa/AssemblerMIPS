# TEST_RETURN_CODE = PASS
# allons au ru
.set noreorder
.data
    .asciiz "trnkjerng","test"
    .word 2, -128, 0xff
.text
test:    lw $28, test
