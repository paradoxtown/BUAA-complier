.data
enter:  .asciiz "\n"
_str2:  .asciiz "all test:"
_str1:  .asciiz "a"
_str0:  .asciiz " !#$%&'()*+,-./0123456789:;<=>?@abcdefghijklmnopqrstuvwxyz[\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
const_1:        .word 1
const_2:        .word 2
const_3:        .word 3
const_a:        .word 97
const_b:        .word 98
const_c:        .word 99
const_: .word 95
count:  .word 0
num:    .word 0: 10
letter: .word 0: 5
s:      .word 0
_:      .word 0
__:     .word 0
___:    .word 0
__a__:  .word 0
_a_:    .word 0
judge:  .word 0
.text
j       main
get_sum:
sub     $sp, $sp, 4
sub     $s0, $sp, 8
lw      $t0, judge
sw      $t0, -0($s0)
lw      $t1, -0($s0)
add     $t1, $t1, 1
sw      $t1, -4($s0)
la      $a0, judge
sw      $t1, 0($a0)
lw      $t2, count
sw      $t2, -8($s0)
lw      $t0, -8($s0)
add     $t0, $t0, 1
sw      $t0, -12($s0)
la      $a0, count
sw      $t0, 0($a0)
li      $t1, 0
sw      $t1, -4($sp)
lw      $t2, -0($sp)
sw      $t2, -16($s0)
li      $t0, 1
lw      $t1, -16($s0)
bne     $t1, $t0, label0
j       label1
label0:
lw      $t2, -0($sp)
sw      $t2, -24($s0)
lw      $t0, -24($s0)
sub     $t0, $t0, 1
sw      $t0, -28($s0)
sw      $t0, -64($sp)
sub     $sp, $sp, 60
sw      $ra, 0($sp)
jal     get_sum
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 60
add     $s0, $s0, 64
add     $t1, $0, $v0
sw      $t1, -20($s0)
sw      $t1, -4($sp)
lw      $t2, judge
sw      $t2, -32($s0)
lw      $t0, -32($s0)
add     $t0, $t0, 1
sw      $t0, -36($s0)
la      $a0, judge
sw      $t0, 0($a0)
label1:
lw      $t1, -4($sp)
sw      $t1, -40($s0)
lw      $t2, -0($sp)
sw      $t2, -44($s0)
lw      $t0, -44($s0)
lw      $t1, -40($s0)
add     $t1, $t1, $t0
sw      $t1, -48($s0)
lw      $v0, -48($s0)
jr      $ra
count_to_10:
sub     $sp, $sp, 4
sub     $s0, $sp, 0
lw      $t2, judge
sw      $t2, -0($s0)
lw      $t0, -0($s0)
add     $t0, $t0, 1
sw      $t0, -4($s0)
la      $a0, judge
sw      $t0, 0($a0)
li      $t1, 10
sw      $t1, -16($sp)
sub     $sp, $sp, 12
sw      $ra, 0($sp)
jal     get_sum
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 12
add     $s0, $s0, 24
add     $t2, $0, $v0
sw      $t2, -8($s0)
lw      $v0, -8($s0)
jr      $ra
higher:
sub     $sp, $sp, 4
sub     $s0, $sp, 4
li      $v0, 65
jr      $ra
prt_message:
sub     $sp, $sp, 4
sub     $s0, $sp, 0
lw      $t0, judge
sw      $t0, -0($s0)
lw      $t1, -0($s0)
add     $t1, $t1, 1
sw      $t1, -4($s0)
la      $a0, judge
sw      $t1, 0($a0)
la      $a0, _str0
li      $v0, 4
syscall
la      $a0, enter
li      $v0, 4
syscall
li      $v0, 97
jr      $ra
test_global_variable:
sub     $sp, $sp, 4
sub     $s0, $sp, 12
lw      $t2, judge
sw      $t2, -0($s0)
lw      $t0, -0($s0)
add     $t0, $t0, 1
sw      $t0, -4($s0)
la      $a0, judge
sw      $t0, 0($a0)
li      $t1, 0
sw      $t1, -0($sp)
li      $t2, 0
sw      $t2, -4($sp)
li      $t0, 2
la      $a0, count
sw      $t0, 0($a0)
li      $t1, 0
la      $t2, letter
add     $t1, $t1, $t2
li      $t0, 97
sw      $t0, 0($t1)
li      $t1, 4
la      $t2, letter
add     $t1, $t1, $t2
li      $t0, 49
sw      $t0, 0($t1)
li      $t1, 8
la      $t2, letter
add     $t1, $t1, $t2
li      $t0, 95
sw      $t0, 0($t1)
li      $t1, 12
la      $t2, letter
add     $t1, $t1, $t2
li      $t0, 43
sw      $t0, 0($t1)
li      $t1, 16
la      $t2, letter
add     $t1, $t1, $t2
li      $t0, 65
sw      $t0, 0($t1)
lw      $t1, count
sw      $t1, -8($s0)
lw      $a0, -8($s0)
li      $v0, 1
syscall
la      $a0, enter
li      $v0, 4
syscall
la      $t2, letter
add     $t2, $t2, 0
lw      $t2, -0($t2)
sw      $t2, -12($s0)
lw      $a0, -12($s0)
li      $v0, 1
syscall
la      $a0, enter
li      $v0, 4
syscall
lw      $t0, count
sw      $t0, -16($s0)
li      $t1, 2
lw      $t2, -16($s0)
beq     $t2, $t1, label2
j       label3
label2:
lw      $t0, judge
sw      $t0, -20($s0)
lw      $t1, -20($s0)
add     $t1, $t1, 1
sw      $t1, -24($s0)
la      $a0, judge
sw      $t1, 0($a0)
label3:
lw      $t2, count
sw      $t2, -28($s0)
li      $t0, 1
lw      $t1, -28($s0)
beq     $t1, $t0, label4
j       label5
label4:
lw      $t2, judge
sw      $t2, -32($s0)
lw      $t0, -32($s0)
add     $t0, $t0, 1
sw      $t0, -36($s0)
la      $a0, judge
sw      $t0, 0($a0)
li      $t1, 1
sw      $t1, -4($sp)
label5:
lw      $t2, -4($sp)
sw      $t2, -40($s0)
li      $t0, 0
lw      $t1, -40($s0)
beq     $t1, $t0, label6
j       label7
label6:
lw      $t2, judge
sw      $t2, -44($s0)
lw      $t0, -44($s0)
add     $t0, $t0, 1
sw      $t0, -48($s0)
la      $a0, judge
sw      $t0, 0($a0)
label7:
li      $t1, 1
la      $a0, _
sw      $t1, 0($a0)
li      $t2, 2
la      $a0, __
sw      $t2, 0($a0)
li      $t0, 3
la      $a0, ___
sw      $t0, 0($a0)
li      $t1, 4
la      $a0, __a__
sw      $t1, 0($a0)
li      $t2, 5
la      $a0, _a_
sw      $t2, 0($a0)
li      $t0, 0
sw      $t0, -4($sp)
lw      $t1, _
sw      $t1, -52($s0)
li      $t2, 1
lw      $t0, -52($s0)
beq     $t0, $t2, label8
j       label9
label8:
lw      $t1, judge
sw      $t1, -56($s0)
lw      $t2, -56($s0)
add     $t2, $t2, 1
sw      $t2, -60($s0)
la      $a0, judge
sw      $t2, 0($a0)
label9:
lw      $t0, _
sw      $t0, -64($s0)
li      $t1, 0
lw      $t2, -64($s0)
beq     $t2, $t1, label10
j       label11
label10:
li      $t0, 1
sw      $t0, -4($sp)
label11:
lw      $t1, -4($sp)
sw      $t1, -68($s0)
li      $t2, 0
lw      $t0, -68($s0)
beq     $t0, $t2, label12
j       label13
label12:
lw      $t1, judge
sw      $t1, -72($s0)
lw      $t2, -72($s0)
add     $t2, $t2, 1
sw      $t2, -76($s0)
la      $a0, judge
sw      $t2, 0($a0)
label13:
li      $t0, 0
sw      $t0, -4($sp)
lw      $t1, __
sw      $t1, -80($s0)
li      $t2, 2
lw      $t0, -80($s0)
beq     $t0, $t2, label14
j       label15
label14:
lw      $t1, judge
sw      $t1, -84($s0)
lw      $t2, -84($s0)
add     $t2, $t2, 1
sw      $t2, -88($s0)
la      $a0, judge
sw      $t2, 0($a0)
label15:
lw      $t0, __
sw      $t0, -92($s0)
li      $t1, 0
lw      $t2, -92($s0)
beq     $t2, $t1, label16
j       label17
label16:
lw      $t0, judge
sw      $t0, -96($s0)
lw      $t1, -96($s0)
add     $t1, $t1, 1
sw      $t1, -100($s0)
la      $a0, judge
sw      $t1, 0($a0)
li      $t2, 1
sw      $t2, -4($sp)
label17:
lw      $t0, -4($sp)
sw      $t0, -104($s0)
li      $t1, 0
lw      $t2, -104($s0)
beq     $t2, $t1, label18
j       label19
label18:
lw      $t0, judge
sw      $t0, -108($s0)
lw      $t1, -108($s0)
add     $t1, $t1, 1
sw      $t1, -112($s0)
la      $a0, judge
sw      $t1, 0($a0)
label19:
li      $t2, 0
sw      $t2, -4($sp)
lw      $t0, ___
sw      $t0, -116($s0)
li      $t1, 3
lw      $t2, -116($s0)
beq     $t2, $t1, label20
j       label21
label20:
lw      $t0, judge
sw      $t0, -120($s0)
lw      $t1, -120($s0)
add     $t1, $t1, 1
sw      $t1, -124($s0)
la      $a0, judge
sw      $t1, 0($a0)
label21:
lw      $t2, ___
sw      $t2, -128($s0)
li      $t0, 0
lw      $t1, -128($s0)
beq     $t1, $t0, label22
j       label23
label22:
lw      $t2, judge
sw      $t2, -132($s0)
lw      $t0, -132($s0)
add     $t0, $t0, 1
sw      $t0, -136($s0)
la      $a0, judge
sw      $t0, 0($a0)
li      $t1, 1
sw      $t1, -4($sp)
label23:
lw      $t2, -4($sp)
sw      $t2, -140($s0)
li      $t0, 0
lw      $t1, -140($s0)
beq     $t1, $t0, label24
j       label25
label24:
lw      $t2, judge
sw      $t2, -144($s0)
lw      $t0, -144($s0)
add     $t0, $t0, 1
sw      $t0, -148($s0)
la      $a0, judge
sw      $t0, 0($a0)
label25:
li      $t1, 0
sw      $t1, -4($sp)
lw      $t2, __a__
sw      $t2, -152($s0)
li      $t0, 4
lw      $t1, -152($s0)
beq     $t1, $t0, label26
j       label27
label26:
lw      $t2, judge
sw      $t2, -156($s0)
lw      $t0, -156($s0)
add     $t0, $t0, 1
sw      $t0, -160($s0)
la      $a0, judge
sw      $t0, 0($a0)
label27:
lw      $t1, __a__
sw      $t1, -164($s0)
li      $t2, 0
lw      $t0, -164($s0)
beq     $t0, $t2, label28
j       label29
label28:
lw      $t1, judge
sw      $t1, -168($s0)
lw      $t2, -168($s0)
add     $t2, $t2, 1
sw      $t2, -172($s0)
la      $a0, judge
sw      $t2, 0($a0)
li      $t0, 1
sw      $t0, -4($sp)
label29:
lw      $t1, -4($sp)
sw      $t1, -176($s0)
li      $t2, 0
lw      $t0, -176($s0)
beq     $t0, $t2, label30
j       label31
label30:
lw      $t1, judge
sw      $t1, -180($s0)
lw      $t2, -180($s0)
add     $t2, $t2, 1
sw      $t2, -184($s0)
la      $a0, judge
sw      $t2, 0($a0)
label31:
li      $t0, 0
sw      $t0, -4($sp)
lw      $t1, _a_
sw      $t1, -188($s0)
li      $t2, 5
lw      $t0, -188($s0)
beq     $t0, $t2, label32
j       label33
label32:
lw      $t1, judge
sw      $t1, -192($s0)
lw      $t2, -192($s0)
add     $t2, $t2, 1
sw      $t2, -196($s0)
la      $a0, judge
sw      $t2, 0($a0)
label33:
lw      $t0, _a_
sw      $t0, -200($s0)
li      $t1, 0
lw      $t2, -200($s0)
beq     $t2, $t1, label34
j       label35
label34:
lw      $t0, judge
sw      $t0, -204($s0)
lw      $t1, -204($s0)
add     $t1, $t1, 1
sw      $t1, -208($s0)
la      $a0, judge
sw      $t1, 0($a0)
li      $t2, 1
sw      $t2, -4($sp)
label35:
lw      $t0, -4($sp)
sw      $t0, -212($s0)
li      $t1, 0
lw      $t2, -212($s0)
beq     $t2, $t1, label36
j       label37
label36:
lw      $t0, judge
sw      $t0, -216($s0)
lw      $t1, -216($s0)
add     $t1, $t1, 1
sw      $t1, -220($s0)
la      $a0, judge
sw      $t1, 0($a0)
label37:
jr      $ra
test_global_const:
sub     $sp, $sp, 4
sub     $s0, $sp, 20
li      $t2, 100
sw      $t2, -16($sp)
lw      $t0, -16($sp)
sw      $t0, -0($s0)
li      $t1, 100
lw      $t2, -0($s0)
beq     $t2, $t1, label38
j       label39
label38:
lw      $t0, judge
sw      $t0, -4($s0)
lw      $t1, -4($s0)
add     $t1, $t1, 1
sw      $t1, -8($s0)
la      $a0, judge
sw      $t1, 0($a0)
label39:
li      $t2, 0
add     $t2, $t2, 8
sub     $t2, $sp, $t2
li      $t0, 100
sw      $t0, 0($t2)
li      $t1, 4
add     $t1, $t1, 8
sub     $t1, $sp, $t1
li      $t2, 101
sw      $t2, 0($t1)
lw      $t0, -8($sp)
sw      $t0, -12($s0)
li      $t1, 100
lw      $t2, -12($s0)
beq     $t2, $t1, label40
j       label41
label40:
lw      $t0, judge
sw      $t0, -16($s0)
lw      $t1, -16($s0)
add     $t1, $t1, 1
sw      $t1, -20($s0)
la      $a0, judge
sw      $t1, 0($a0)
label41:
lw      $t2, -12($sp)
sw      $t2, -24($s0)
li      $t0, 101
lw      $t1, -24($s0)
beq     $t1, $t0, label42
j       label43
label42:
lw      $t2, judge
sw      $t2, -28($s0)
lw      $t0, -28($s0)
add     $t0, $t0, 1
sw      $t0, -32($s0)
la      $a0, judge
sw      $t0, 0($a0)
label43:
lw      $t1, judge
sw      $t1, -36($s0)
lw      $t2, -36($s0)
add     $t2, $t2, 1
sw      $t2, -40($s0)
la      $a0, judge
sw      $t2, 0($a0)
lw      $t0, const_1
sw      $t0, -44($s0)
li      $t1, 1
lw      $t2, -44($s0)
beq     $t2, $t1, label44
j       label45
label44:
lw      $t0, judge
sw      $t0, -48($s0)
lw      $t1, -48($s0)
add     $t1, $t1, 1
sw      $t1, -52($s0)
la      $a0, judge
sw      $t1, 0($a0)
label45:
lw      $t2, const_a
sw      $t2, -56($s0)
li      $t0, 97
lw      $t1, -56($s0)
beq     $t1, $t0, label46
j       label47
label46:
lw      $t2, judge
sw      $t2, -60($s0)
lw      $t0, -60($s0)
add     $t0, $t0, 1
sw      $t0, -64($s0)
la      $a0, judge
sw      $t0, 0($a0)
label47:
lw      $t1, const_
sw      $t1, -68($s0)
li      $t2, 95
lw      $t0, -68($s0)
beq     $t0, $t2, label48
j       label49
label48:
lw      $t1, judge
sw      $t1, -72($s0)
lw      $t2, -72($s0)
add     $t2, $t2, 1
sw      $t2, -76($s0)
la      $a0, judge
sw      $t2, 0($a0)
label49:
lw      $t0, const_a
sw      $t0, -80($s0)
sw      $t0, -4($sp)
lw      $t1, -4($sp)
sw      $t1, -84($s0)
lw      $t2, const_a
sw      $t2, -88($s0)
lw      $t0, -88($s0)
lw      $t1, -84($s0)
add     $t1, $t1, $t0
sw      $t1, -92($s0)
sw      $t1, -0($sp)
lw      $t2, -0($sp)
sw      $t2, -96($s0)
li      $t0, 194
lw      $t1, -96($s0)
beq     $t1, $t0, label50
j       label51
label50:
lw      $t2, judge
sw      $t2, -100($s0)
lw      $t0, -100($s0)
add     $t0, $t0, 1
sw      $t0, -104($s0)
la      $a0, judge
sw      $t0, 0($a0)
label51:
lw      $t1, const_1
sw      $t1, -108($s0)
li      $t2, 1
lw      $t0, -108($s0)
beq     $t0, $t2, label53
j       label54
label53:
lw      $t1, judge
sw      $t1, -112($s0)
lw      $t2, -112($s0)
add     $t2, $t2, 1
sw      $t2, -116($s0)
la      $a0, judge
sw      $t2, 0($a0)
j       label52
label54:
label52:
lw      $t0, const_1
sw      $t0, -120($s0)
li      $t1, 0
lw      $t2, -120($s0)
beq     $t2, $t1, label56
j       label57
label56:
lw      $t0, judge
sw      $t0, -124($s0)
lw      $t1, -124($s0)
add     $t1, $t1, 1
sw      $t1, -128($s0)
la      $a0, judge
sw      $t1, 0($a0)
j       label55
label57:
label55:
li      $t2, 0
sw      $t2, -0($sp)
label59:
lw      $t0, -0($sp)
sw      $t0, -132($s0)
lw      $t1, const_2
sw      $t1, -136($s0)
lw      $t2, -132($s0)
lw      $t0, -136($s0)
sub     $t0, $t2, $t0
bltz    $t0, label60
j       label58
label60:
lw      $t1, judge
sw      $t1, -140($s0)
lw      $t2, -140($s0)
add     $t2, $t2, 1
sw      $t2, -144($s0)
la      $a0, judge
sw      $t2, 0($a0)
lw      $t0, -0($sp)
sw      $t0, -148($s0)
lw      $t1, -148($s0)
add     $t1, $t1, 1
sw      $t1, -152($s0)
sw      $t1, -0($sp)
j       label59
label58:
jr      $ra
test_brackets:
sub     $sp, $sp, 4
sub     $s0, $sp, 0
lw      $t2, judge
sw      $t2, -0($s0)
lw      $t0, -0($s0)
add     $t0, $t0, 1
sw      $t0, -4($s0)
la      $a0, judge
sw      $t0, 0($a0)
jr      $ra
test_compare_operator:
sub     $sp, $sp, 4
sub     $s0, $sp, 12
li      $t1, 0
sw      $t1, -0($sp)
li      $t2, 1
sw      $t2, -4($sp)
li      $t0, 0
sw      $t0, -8($sp)
lw      $t1, -0($sp)
sw      $t1, -0($s0)
lw      $t2, -4($sp)
sw      $t2, -4($s0)
lw      $t0, -0($s0)
lw      $t1, -4($s0)
sub     $t1, $t0, $t1
bltz    $t1, label61
j       label62
label61:
lw      $t2, judge
sw      $t2, -8($s0)
lw      $t0, -8($s0)
add     $t0, $t0, 1
sw      $t0, -12($s0)
la      $a0, judge
sw      $t0, 0($a0)
label62:
lw      $t1, -4($sp)
sw      $t1, -16($s0)
lw      $t2, -0($sp)
sw      $t2, -20($s0)
lw      $t0, -16($s0)
lw      $t1, -20($s0)
sub     $t1, $t0, $t1
bgtz    $t1, label63
j       label64
label63:
lw      $t2, judge
sw      $t2, -24($s0)
lw      $t0, -24($s0)
add     $t0, $t0, 1
sw      $t0, -28($s0)
la      $a0, judge
sw      $t0, 0($a0)
label64:
lw      $t1, -0($sp)
sw      $t1, -32($s0)
lw      $t2, -4($sp)
sw      $t2, -36($s0)
lw      $t0, -32($s0)
lw      $t1, -36($s0)
beq     $t0, $t1, label65
j       label66
label65:
lw      $t2, judge
sw      $t2, -40($s0)
lw      $t0, -40($s0)
add     $t0, $t0, 1
sw      $t0, -44($s0)
la      $a0, judge
sw      $t0, 0($a0)
li      $t1, 1
sw      $t1, -8($sp)
label66:
lw      $t2, -8($sp)
sw      $t2, -48($s0)
li      $t0, 0
lw      $t1, -48($s0)
beq     $t1, $t0, label67
j       label68
label67:
lw      $t2, judge
sw      $t2, -52($s0)
lw      $t0, -52($s0)
add     $t0, $t0, 1
sw      $t0, -56($s0)
la      $a0, judge
sw      $t0, 0($a0)
label68:
lw      $t1, -0($sp)
sw      $t1, -60($s0)
lw      $t2, -4($sp)
sw      $t2, -64($s0)
lw      $t0, -60($s0)
lw      $t1, -64($s0)
bne     $t0, $t1, label69
j       label70
label69:
lw      $t2, judge
sw      $t2, -68($s0)
lw      $t0, -68($s0)
add     $t0, $t0, 1
sw      $t0, -72($s0)
la      $a0, judge
sw      $t0, 0($a0)
label70:
li      $t1, 0
sw      $t1, -0($sp)
label72:
lw      $t2, -0($sp)
sw      $t2, -76($s0)
li      $t0, 1
lw      $t1, -76($s0)
sub     $t1, $t1, $t0
blez    $t1, label73
j       label71
label73:
lw      $t2, -0($sp)
sw      $t2, -80($s0)
lw      $t0, -4($sp)
sw      $t0, -84($s0)
lw      $t1, -80($s0)
lw      $t2, -84($s0)
sub     $t2, $t1, $t2
blez    $t2, label74
j       label75
label74:
lw      $t0, judge
sw      $t0, -88($s0)
lw      $t1, -88($s0)
add     $t1, $t1, 1
sw      $t1, -92($s0)
la      $a0, judge
sw      $t1, 0($a0)
label75:
lw      $t2, -0($sp)
sw      $t2, -96($s0)
lw      $t0, -96($s0)
add     $t0, $t0, 1
sw      $t0, -100($s0)
sw      $t0, -0($sp)
j       label72
label71:
li      $t1, 0
sw      $t1, -0($sp)
label77:
lw      $t2, -0($sp)
sw      $t2, -104($s0)
li      $t0, 1
lw      $t1, -104($s0)
sub     $t1, $t1, $t0
blez    $t1, label78
j       label76
label78:
lw      $t2, -4($sp)
sw      $t2, -108($s0)
lw      $t0, -0($sp)
sw      $t0, -112($s0)
lw      $t1, -108($s0)
lw      $t2, -112($s0)
sub     $t2, $t1, $t2
bgez    $t2, label79
j       label80
label79:
lw      $t0, judge
sw      $t0, -116($s0)
lw      $t1, -116($s0)
add     $t1, $t1, 1
sw      $t1, -120($s0)
la      $a0, judge
sw      $t1, 0($a0)
label80:
lw      $t2, -0($sp)
sw      $t2, -124($s0)
lw      $t0, -124($s0)
add     $t0, $t0, 1
sw      $t0, -128($s0)
sw      $t0, -0($sp)
j       label77
label76:
jr      $ra
test_expression:
sub     $sp, $sp, 4
sub     $s0, $sp, 32
li      $t1, 0
sw      $t1, -0($sp)
li      $t2, 0
sw      $t2, -4($sp)
li      $t0, 97
sw      $t0, -8($sp)
li      $t1, 98
sw      $t1, -12($sp)
li      $t2, 99
sw      $t2, -16($sp)
lw      $t0, judge
sw      $t0, -0($s0)
lw      $t1, -0($s0)
add     $t1, $t1, 1
sw      $t1, -4($s0)
la      $a0, judge
sw      $t1, 0($a0)
lw      $t2, -0($sp)
sw      $t2, -8($s0)
sw      $t2, -0($sp)
lw      $t0, -0($sp)
sw      $t0, -12($s0)
li      $t1, 0
lw      $t2, -12($s0)
beq     $t2, $t1, label81
j       label82
label81:
lw      $t0, judge
sw      $t0, -16($s0)
lw      $t1, -16($s0)
add     $t1, $t1, 1
sw      $t1, -20($s0)
la      $a0, judge
sw      $t1, 0($a0)
label82:
lw      $t2, -0($sp)
sw      $t2, -24($s0)
lw      $a0, -24($s0)
li      $v0, 1
syscall
la      $a0, enter
li      $v0, 4
syscall
lw      $t0, -0($sp)
sw      $t0, -28($s0)
lw      $t1, -28($s0)
sub     $t1, $t1, 0
sub     $t1, $0, $t1
sw      $t1, -32($s0)
sw      $t1, -0($sp)
lw      $t2, -0($sp)
sw      $t2, -36($s0)
li      $t0, 0
lw      $t1, -36($s0)
beq     $t1, $t0, label83
j       label84
label83:
lw      $t2, judge
sw      $t2, -40($s0)
lw      $t0, -40($s0)
add     $t0, $t0, 1
sw      $t0, -44($s0)
la      $a0, judge
sw      $t0, 0($a0)
label84:
lw      $t1, -0($sp)
sw      $t1, -48($s0)
lw      $a0, -48($s0)
li      $v0, 1
syscall
la      $a0, enter
li      $v0, 4
syscall
lw      $t2, -8($sp)
sw      $t2, -52($s0)
sw      $t2, -0($sp)
lw      $t0, -0($sp)
sw      $t0, -56($s0)
li      $t1, 97
lw      $t2, -56($s0)
beq     $t2, $t1, label85
j       label86
label85:
lw      $t0, judge
sw      $t0, -60($s0)
lw      $t1, -60($s0)
add     $t1, $t1, 1
sw      $t1, -64($s0)
la      $a0, judge
sw      $t1, 0($a0)
label86:
lw      $t2, -0($sp)
sw      $t2, -68($s0)
lw      $a0, -68($s0)
li      $v0, 1
syscall
la      $a0, enter
li      $v0, 4
syscall
lw      $t0, -8($sp)
sw      $t0, -72($s0)
lw      $t1, -72($s0)
sub     $t1, $t1, 0
sub     $t1, $0, $t1
sw      $t1, -76($s0)
sw      $t1, -0($sp)
lw      $t2, -0($sp)
sw      $t2, -80($s0)
li      $t0, 0
sub     $t0, $t0, 97
sw      $t0, -84($s0)
lw      $t1, -80($s0)
lw      $t2, -84($s0)
beq     $t1, $t2, label87
j       label88
label87:
lw      $t0, judge
sw      $t0, -88($s0)
lw      $t1, -88($s0)
add     $t1, $t1, 1
sw      $t1, -92($s0)
la      $a0, judge
sw      $t1, 0($a0)
label88:
lw      $t2, -0($sp)
sw      $t2, -96($s0)
lw      $a0, -96($s0)
li      $v0, 1
syscall
la      $a0, enter
li      $v0, 4
syscall
lw      $t0, const_3
sw      $t0, -100($s0)
sw      $t0, -0($sp)
lw      $t1, -0($sp)
sw      $t1, -104($s0)
li      $t2, 3
lw      $t0, -104($s0)
beq     $t0, $t2, label89
j       label90
label89:
lw      $t1, judge
sw      $t1, -108($s0)
lw      $t2, -108($s0)
add     $t2, $t2, 1
sw      $t2, -112($s0)
la      $a0, judge
sw      $t2, 0($a0)
label90:
lw      $t0, -0($sp)
sw      $t0, -116($s0)
lw      $a0, -116($s0)
li      $v0, 1
syscall
la      $a0, enter
li      $v0, 4
syscall
li      $t1, -1
sw      $t1, -0($sp)
lw      $t2, -0($sp)
sw      $t2, -120($s0)
li      $t0, 0
sub     $t0, $t0, 1
sw      $t0, -124($s0)
lw      $t1, -120($s0)
lw      $t2, -124($s0)
beq     $t1, $t2, label91
j       label92
label91:
lw      $t0, judge
sw      $t0, -128($s0)
lw      $t1, -128($s0)
add     $t1, $t1, 1
sw      $t1, -132($s0)
la      $a0, judge
sw      $t1, 0($a0)
label92:
lw      $t2, -0($sp)
sw      $t2, -136($s0)
lw      $a0, -136($s0)
li      $v0, 1
syscall
la      $a0, enter
li      $v0, 4
syscall
li      $t0, 0
sub     $t0, $t0, -1
sw      $t0, -140($s0)
sw      $t0, -0($sp)
lw      $t1, -0($sp)
sw      $t1, -144($s0)
li      $t2, 1
lw      $t0, -144($s0)
beq     $t0, $t2, label93
j       label94
label93:
lw      $t1, judge
sw      $t1, -148($s0)
lw      $t2, -148($s0)
add     $t2, $t2, 1
sw      $t2, -152($s0)
la      $a0, judge
sw      $t2, 0($a0)
label94:
lw      $t0, -0($sp)
sw      $t0, -156($s0)
lw      $a0, -156($s0)
li      $v0, 1
syscall
la      $a0, enter
li      $v0, 4
syscall
li      $t1, 1
sw      $t1, -0($sp)
lw      $t2, -0($sp)
sw      $t2, -160($s0)
li      $t0, 1
lw      $t1, -160($s0)
beq     $t1, $t0, label95
j       label96
label95:
lw      $t2, judge
sw      $t2, -164($s0)
lw      $t0, -164($s0)
add     $t0, $t0, 1
sw      $t0, -168($s0)
la      $a0, judge
sw      $t0, 0($a0)
label96:
lw      $t1, -0($sp)
sw      $t1, -172($s0)
lw      $a0, -172($s0)
li      $v0, 1
syscall
la      $a0, enter
li      $v0, 4
syscall
li      $t2, 0
sub     $t2, $t2, 1
sw      $t2, -176($s0)
sw      $t2, -0($sp)
lw      $t0, -0($sp)
sw      $t0, -180($s0)
li      $t1, 0
sub     $t1, $t1, 1
sw      $t1, -184($s0)
lw      $t2, -180($s0)
lw      $t0, -184($s0)
beq     $t2, $t0, label97
j       label98
label97:
lw      $t1, judge
sw      $t1, -188($s0)
lw      $t2, -188($s0)
add     $t2, $t2, 1
sw      $t2, -192($s0)
la      $a0, judge
sw      $t2, 0($a0)
label98:
lw      $t0, -0($sp)
sw      $t0, -196($s0)
lw      $a0, -196($s0)
li      $v0, 1
syscall
la      $a0, enter
li      $v0, 4
syscall
li      $t1, 1
sw      $t1, -0($sp)
lw      $t2, -0($sp)
sw      $t2, -200($s0)
li      $t0, 1
lw      $t1, -200($s0)
beq     $t1, $t0, label99
j       label100
label99:
lw      $t2, judge
sw      $t2, -204($s0)
lw      $t0, -204($s0)
add     $t0, $t0, 1
sw      $t0, -208($s0)
la      $a0, judge
sw      $t0, 0($a0)
label100:
lw      $t1, -0($sp)
sw      $t1, -212($s0)
lw      $a0, -212($s0)
li      $v0, 1
syscall
la      $a0, enter
li      $v0, 4
syscall
li      $t2, 3
mul     $t2, $t2, -3
sw      $t2, -216($s0)
sw      $t2, -0($sp)
lw      $t0, -0($sp)
sw      $t0, -220($s0)
li      $t1, 0
sub     $t1, $t1, 9
sw      $t1, -224($s0)
lw      $t2, -220($s0)
lw      $t0, -224($s0)
beq     $t2, $t0, label101
j       label102
label101:
lw      $t1, judge
sw      $t1, -228($s0)
lw      $t2, -228($s0)
add     $t2, $t2, 1
sw      $t2, -232($s0)
la      $a0, judge
sw      $t2, 0($a0)
label102:
lw      $t0, -0($sp)
sw      $t0, -236($s0)
lw      $a0, -236($s0)
li      $v0, 1
syscall
la      $a0, enter
li      $v0, 4
syscall
li      $t1, 5
sw      $t1, -4($sp)
lw      $t2, -4($sp)
sw      $t2, -244($s0)
mul     $t2, $t2, 4
la      $t0, num
add     $t2, $t2, $t0
lw      $t1, 0($t2)
sw      $t1, -240($s0)
lw      $t2, -4($sp)
sw      $t2, -252($s0)
lw      $t0, const_1
sw      $t0, -256($s0)
lw      $t1, -256($s0)
lw      $t2, -252($s0)
add     $t2, $t2, $t1
sw      $t2, -260($s0)
mul     $t2, $t2, 4
la      $t0, num
add     $t2, $t2, $t0
lw      $t1, 0($t2)
sw      $t1, -248($s0)
lw      $t2, -248($s0)
lw      $t0, -240($s0)
add     $t0, $t0, $t2
sw      $t0, -264($s0)
lw      $t1, -264($s0)
add     $t1, $t1, 3
sw      $t1, -268($s0)
sub     $sp, $sp, 440
sw      $ra, 0($sp)
jal     count_to_10
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 440
add     $s0, $s0, 412
add     $t2, $0, $v0
sw      $t2, -272($s0)
lw      $t0, -268($s0)
lw      $t1, -272($s0)
mul     $t1, $t1, $t0
sw      $t1, -276($s0)
lw      $t2, const_3
sw      $t2, -280($s0)
lw      $t1, -276($s0)
lw      $t0, -280($s0)
div     $t1, $t1, $t0
sw      $t1, -284($s0)
lw      $t2, -284($s0)
sub     $t2, $t2, 0
sub     $t2, $0, $t2
sw      $t2, -288($s0)
lw      $t0, -288($s0)
add     $t0, $t0, 97
sw      $t0, -292($s0)
lw      $t1, const_a
sw      $t1, -296($s0)
lw      $t2, -296($s0)
lw      $t0, -292($s0)
add     $t0, $t0, $t2
sw      $t0, -300($s0)
li      $t1, 3
mul     $t1, $t1, 3
sw      $t1, -304($s0)
lw      $t2, -304($s0)
div     $t2, $t2, 4
sw      $t2, -308($s0)
lw      $t0, -308($s0)
lw      $t1, -300($s0)
sub     $t1, $t1, $t0
sw      $t1, -312($s0)
la      $t2, letter
add     $t2, $t2, 0
lw      $t2, -0($t2)
sw      $t2, -316($s0)
lw      $t0, -316($s0)
lw      $t1, -312($s0)
add     $t1, $t1, $t0
sw      $t1, -320($s0)
sw      $t1, -0($sp)
la      $a0, _str1
li      $v0, 4
syscall
la      $a0, enter
li      $v0, 4
syscall
lw      $t2, -0($sp)
sw      $t2, -324($s0)
lw      $a0, -324($s0)
li      $v0, 1
syscall
la      $a0, enter
li      $v0, 4
syscall
lw      $t0, -0($sp)
sw      $t0, -328($s0)
li      $t1, 33
lw      $t2, -328($s0)
beq     $t2, $t1, label103
j       label104
label103:
lw      $t0, judge
sw      $t0, -332($s0)
lw      $t1, -332($s0)
add     $t1, $t1, 1
sw      $t1, -336($s0)
la      $a0, judge
sw      $t1, 0($a0)
label104:
li      $t2, 1
sw      $t2, -0($sp)
li      $t0, 1
sw      $t0, -4($sp)
li      $t1, 97
sw      $t1, -20($sp)
li      $t2, 113
sw      $t2, -24($sp)
lw      $t0, -0($sp)
sw      $t0, -340($s0)
lw      $t1, -4($sp)
sw      $t1, -344($s0)
lw      $t2, -8($sp)
sw      $t2, -348($s0)
lw      $t0, -12($sp)
sw      $t0, -352($s0)
lw      $t1, -16($sp)
sw      $t1, -356($s0)
lw      $t2, -20($sp)
sw      $t2, -360($s0)
lw      $t0, -24($sp)
sw      $t0, -364($s0)
lw      $t1, -360($s0)
lw      $t2, -364($s0)
mul     $t2, $t2, $t1
sw      $t2, -368($s0)
lw      $t0, -368($s0)
lw      $t1, -356($s0)
add     $t1, $t1, $t0
sw      $t1, -372($s0)
lw      $t2, -372($s0)
lw      $t0, -352($s0)
add     $t0, $t0, $t2
sw      $t0, -376($s0)
lw      $t1, -348($s0)
lw      $t2, -376($s0)
mul     $t2, $t2, $t1
sw      $t2, -380($s0)
lw      $t0, -380($s0)
lw      $t1, -344($s0)
add     $t1, $t1, $t0
sw      $t1, -384($s0)
lw      $t2, -340($s0)
lw      $t0, -384($s0)
mul     $t0, $t0, $t2
sw      $t0, -388($s0)
sw      $t0, -0($sp)
lw      $t1, judge
sw      $t1, -392($s0)
lw      $t2, -0($sp)
sw      $t2, -396($s0)
lw      $t0, -396($s0)
div     $t0, $t0, 1024
sw      $t0, -400($s0)
lw      $t1, -400($s0)
lw      $t2, -392($s0)
add     $t2, $t2, $t1
sw      $t2, -404($s0)
la      $a0, judge
sw      $t2, 0($a0)
jr      $ra
test_while:
sub     $sp, $sp, 4
sub     $s0, $sp, 8
li      $t0, 9
sw      $t0, -4($sp)
label106:
lw      $t1, -4($sp)
sw      $t1, -0($s0)
li      $t2, 0
lw      $t0, -0($s0)
sub     $t0, $t0, $t2
bgez    $t0, label107
j       label105
label107:
lw      $t1, -4($sp)
sw      $t1, -4($s0)
li      $t2, 5
lw      $t0, -4($s0)
sub     $t0, $t0, $t2
bgez    $t0, label108
j       label109
label108:
lw      $t1, judge
sw      $t1, -8($s0)
lw      $t2, -8($s0)
add     $t2, $t2, 1
sw      $t2, -12($s0)
la      $a0, judge
sw      $t2, 0($a0)
label109:
lw      $t0, -4($sp)
sw      $t0, -16($s0)
lw      $t1, -16($s0)
sub     $t1, $t1, 1
sw      $t1, -20($s0)
sw      $t1, -4($sp)
j       label106
label105:
li      $t2, 0
sw      $t2, -4($sp)
label111:
lw      $t0, -4($sp)
sw      $t0, -24($s0)
li      $t1, 2
lw      $t2, -24($s0)
sub     $t2, $t2, $t1
blez    $t2, label112
j       label110
label112:
lw      $t0, -4($sp)
sw      $t0, -28($s0)
li      $t1, 0
lw      $t2, -28($s0)
beq     $t2, $t1, label114
j       label115
label114:
lw      $t0, judge
sw      $t0, -32($s0)
lw      $t1, -32($s0)
add     $t1, $t1, 1
sw      $t1, -36($s0)
la      $a0, judge
sw      $t1, 0($a0)
j       label113
label115:
li      $t2, 1
lw      $t0, -28($s0)
beq     $t0, $t2, label116
j       label117
label116:
lw      $t1, judge
sw      $t1, -40($s0)
lw      $t2, -40($s0)
add     $t2, $t2, 1
sw      $t2, -44($s0)
la      $a0, judge
sw      $t2, 0($a0)
j       label113
label117:
lw      $t0, judge
sw      $t0, -48($s0)
lw      $t1, -48($s0)
add     $t1, $t1, 1
sw      $t1, -52($s0)
la      $a0, judge
sw      $t1, 0($a0)
label113:
lw      $t2, -4($sp)
sw      $t2, -56($s0)
lw      $t0, -56($s0)
add     $t0, $t0, 1
sw      $t0, -60($s0)
sw      $t0, -4($sp)
j       label111
label110:
li      $t1, 0
sw      $t1, -4($sp)
li      $t2, 0
sw      $t2, -0($sp)
label119:
lw      $t0, -0($sp)
sw      $t0, -64($s0)
li      $t1, 2
lw      $t2, -64($s0)
sub     $t2, $t2, $t1
blez    $t2, label120
j       label118
label120:
li      $t0, 0
sw      $t0, -4($sp)
label122:
lw      $t1, -4($sp)
sw      $t1, -68($s0)
li      $t2, 2
lw      $t0, -68($s0)
sub     $t0, $t0, $t2
blez    $t0, label123
j       label121
label123:
lw      $t1, judge
sw      $t1, -72($s0)
lw      $t2, -72($s0)
add     $t2, $t2, 1
sw      $t2, -76($s0)
la      $a0, judge
sw      $t2, 0($a0)
lw      $t0, -4($sp)
sw      $t0, -80($s0)
lw      $t1, -80($s0)
add     $t1, $t1, 1
sw      $t1, -84($s0)
sw      $t1, -4($sp)
j       label122
label121:
lw      $t2, -0($sp)
sw      $t2, -88($s0)
lw      $t0, -88($s0)
add     $t0, $t0, 1
sw      $t0, -92($s0)
sw      $t0, -0($sp)
j       label119
label118:
li      $t1, 0
sw      $t1, -4($sp)
li      $t2, 11
sw      $t2, -0($sp)
label125:
lw      $t0, -4($sp)
sw      $t0, -96($s0)
li      $t1, 10
lw      $t2, -96($s0)
sub     $t2, $t2, $t1
bltz    $t2, label126
j       label124
label126:
lw      $t0, -4($sp)
sw      $t0, -100($s0)
lw      $t1, -4($sp)
sw      $t1, -104($s0)
lw      $t2, -0($sp)
sw      $t2, -108($s0)
lw      $t0, -104($s0)
lw      $t1, -108($s0)
mul     $t1, $t1, $t0
sw      $t1, -112($s0)
lw      $t2, -100($s0)
mul     $t2, $t2, 4
la      $t0, num
add     $t2, $t2, $t0
lw      $t1, -112($s0)
sw      $t1, 0($t2)
lw      $t2, judge
sw      $t2, -116($s0)
lw      $t0, -4($sp)
sw      $t0, -124($s0)
mul     $t0, $t0, 4
la      $t1, num
add     $t0, $t0, $t1
lw      $t2, 0($t0)
sw      $t2, -120($s0)
lw      $t0, -120($s0)
lw      $t1, -116($s0)
add     $t1, $t1, $t0
sw      $t1, -128($s0)
la      $a0, judge
sw      $t1, 0($a0)
lw      $t2, -4($sp)
sw      $t2, -132($s0)
lw      $t0, -132($s0)
add     $t0, $t0, 1
sw      $t0, -136($s0)
sw      $t0, -4($sp)
lw      $t1, -0($sp)
sw      $t1, -140($s0)
lw      $t2, -140($s0)
add     $t2, $t2, 2
sw      $t2, -144($s0)
sw      $t2, -0($sp)
j       label125
label124:
jr      $ra
test_if:
sub     $sp, $sp, 4
sub     $s0, $sp, 12
li      $t0, 0
sw      $t0, -0($sp)
li      $t1, 0
sw      $t1, -4($sp)
lw      $t2, judge
sw      $t2, -0($s0)
lw      $t0, -0($s0)
add     $t0, $t0, 1
sw      $t0, -4($s0)
la      $a0, judge
sw      $t0, 0($a0)
lw      $t1, const_1
sw      $t1, -8($s0)
li      $t2, 1
lw      $t0, -8($s0)
beq     $t0, $t2, label127
j       label128
label127:
label128:
lw      $t1, const_1
sw      $t1, -12($s0)
li      $t2, 1
lw      $t0, -12($s0)
beq     $t0, $t2, label129
j       label130
label129:
label130:
lw      $t1, const_1
sw      $t1, -16($s0)
li      $t2, 1
lw      $t0, -16($s0)
beq     $t0, $t2, label131
j       label132
label131:
label132:
li      $t1, 0
sw      $t1, -0($sp)
label134:
lw      $t2, -0($sp)
sw      $t2, -20($s0)
li      $t0, 1
lw      $t1, -20($s0)
sub     $t1, $t1, $t0
blez    $t1, label135
j       label133
label135:
lw      $t2, const_1
sw      $t2, -24($s0)
li      $t0, 1
lw      $t1, -24($s0)
beq     $t1, $t0, label136
j       label137
label136:
lw      $t2, -0($sp)
sw      $t2, -28($s0)
li      $t0, 0
lw      $t1, -28($s0)
beq     $t1, $t0, label138
j       label139
label138:
lw      $t2, judge
sw      $t2, -32($s0)
lw      $t0, -32($s0)
add     $t0, $t0, 1
sw      $t0, -36($s0)
la      $a0, judge
sw      $t0, 0($a0)
li      $t1, 1
sw      $t1, -4($sp)
label139:
lw      $t2, -4($sp)
sw      $t2, -40($s0)
li      $t0, 0
lw      $t1, -40($s0)
beq     $t1, $t0, label140
j       label141
label140:
lw      $t2, judge
sw      $t2, -44($s0)
lw      $t0, -44($s0)
add     $t0, $t0, 1
sw      $t0, -48($s0)
la      $a0, judge
sw      $t0, 0($a0)
label141:
label137:
li      $t1, 0
sw      $t1, -4($sp)
lw      $t2, -0($sp)
sw      $t2, -52($s0)
lw      $t0, -52($s0)
add     $t0, $t0, 1
sw      $t0, -56($s0)
sw      $t0, -0($sp)
j       label134
label133:
li      $t1, 0
sw      $t1, -0($sp)
lw      $t2, const_1
sw      $t2, -60($s0)
li      $t0, 1
lw      $t1, -60($s0)
beq     $t1, $t0, label142
j       label143
label142:
label145:
lw      $t2, -0($sp)
sw      $t2, -64($s0)
li      $t0, 2
lw      $t1, -64($s0)
sub     $t1, $t1, $t0
bltz    $t1, label146
j       label144
label146:
lw      $t2, judge
sw      $t2, -68($s0)
lw      $t0, -68($s0)
add     $t0, $t0, 1
sw      $t0, -72($s0)
la      $a0, judge
sw      $t0, 0($a0)
lw      $t1, -0($sp)
sw      $t1, -76($s0)
lw      $t2, -76($s0)
add     $t2, $t2, 1
sw      $t2, -80($s0)
sw      $t2, -0($sp)
j       label145
label144:
label143:
li      $t0, 0
sw      $t0, -0($sp)
label148:
lw      $t1, -0($sp)
sw      $t1, -84($s0)
li      $t2, 1
lw      $t0, -84($s0)
sub     $t0, $t0, $t2
blez    $t0, label149
j       label147
label149:
lw      $t1, const_1
sw      $t1, -88($s0)
li      $t2, 1
lw      $t0, -88($s0)
beq     $t0, $t2, label150
j       label151
label150:
lw      $t1, -0($sp)
sw      $t1, -92($s0)
li      $t2, 0
lw      $t0, -92($s0)
beq     $t0, $t2, label153
j       label154
label153:
lw      $t1, judge
sw      $t1, -96($s0)
lw      $t2, -96($s0)
add     $t2, $t2, 1
sw      $t2, -100($s0)
la      $a0, judge
sw      $t2, 0($a0)
j       label152
label154:
lw      $t0, judge
sw      $t0, -104($s0)
lw      $t1, -104($s0)
add     $t1, $t1, 1
sw      $t1, -108($s0)
la      $a0, judge
sw      $t1, 0($a0)
label152:
label151:
lw      $t2, -0($sp)
sw      $t2, -112($s0)
lw      $t0, -112($s0)
add     $t0, $t0, 1
sw      $t0, -116($s0)
sw      $t0, -0($sp)
j       label148
label147:
li      $t1, 0
sw      $t1, -4($sp)
li      $t2, 97
sw      $t2, -8($sp)
lw      $t0, -8($sp)
sw      $t0, -120($s0)
li      $t1, 97
lw      $t2, -120($s0)
beq     $t2, $t1, label155
j       label156
label155:
lw      $t0, judge
sw      $t0, -124($s0)
lw      $t1, -124($s0)
add     $t1, $t1, 1
sw      $t1, -128($s0)
la      $a0, judge
sw      $t1, 0($a0)
label156:
lw      $t2, -8($sp)
sw      $t2, -132($s0)
li      $t0, 98
lw      $t1, -132($s0)
beq     $t1, $t0, label157
j       label158
label157:
lw      $t2, judge
sw      $t2, -136($s0)
lw      $t0, -136($s0)
add     $t0, $t0, 1
sw      $t0, -140($s0)
la      $a0, judge
sw      $t0, 0($a0)
li      $t1, 1
sw      $t1, -4($sp)
label158:
lw      $t2, -4($sp)
sw      $t2, -144($s0)
li      $t0, 0
lw      $t1, -144($s0)
beq     $t1, $t0, label159
j       label160
label159:
lw      $t2, judge
sw      $t2, -148($s0)
lw      $t0, -148($s0)
add     $t0, $t0, 1
sw      $t0, -152($s0)
la      $a0, judge
sw      $t0, 0($a0)
label160:
jr      $ra
test_switch:
sub     $sp, $sp, 4
sub     $s0, $sp, 24
li      $t1, 0
sw      $t1, -8($sp)
li      $t2, 0
sw      $t2, -12($sp)
li      $t0, 0
sw      $t0, -0($sp)
li      $t1, 0
sw      $t1, -4($sp)
label162:
lw      $t2, -0($sp)
sw      $t2, -0($s0)
li      $t0, 6
lw      $t1, -0($s0)
sub     $t1, $t1, $t0
blez    $t1, label163
j       label161
label163:
lw      $t2, -0($sp)
sw      $t2, -4($s0)
li      $t0, 0
lw      $t1, -4($s0)
beq     $t1, $t0, label165
j       label166
label165:
lw      $t2, const_1
sw      $t2, -8($s0)
li      $t0, 1
lw      $t1, -8($s0)
beq     $t1, $t0, label167
j       label168
label167:
lw      $t2, judge
sw      $t2, -12($s0)
lw      $t0, -12($s0)
add     $t0, $t0, 1
sw      $t0, -16($s0)
la      $a0, judge
sw      $t0, 0($a0)
lw      $t1, const_3
sw      $t1, -20($s0)
li      $t2, 0
la      $t0, num
add     $t2, $t2, $t0
lw      $t1, -20($s0)
sw      $t1, 0($t2)
lw      $t2, const_3
sw      $t2, -24($s0)
lw      $t0, const_3
sw      $t0, -32($s0)
sw      $t0, -232($sp)
sub     $sp, $sp, 228
sw      $ra, 0($sp)
jal     get_sum
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 228
add     $s0, $s0, 216
add     $t1, $0, $v0
sw      $t1, -28($s0)
lw      $t2, -24($s0)
mul     $t2, $t2, 4
la      $t0, num
add     $t2, $t2, $t0
lw      $t1, -28($s0)
sw      $t1, 0($t2)
label168:
lw      $t2, const_1
sw      $t2, -36($s0)
li      $t0, 0
lw      $t1, -36($s0)
beq     $t1, $t0, label169
j       label170
label169:
lw      $t2, judge
sw      $t2, -40($s0)
lw      $t0, -40($s0)
add     $t0, $t0, 1
sw      $t0, -44($s0)
la      $a0, judge
sw      $t0, 0($a0)
li      $t1, 1
sw      $t1, -8($sp)
label170:
lw      $t2, -8($sp)
sw      $t2, -48($s0)
li      $t0, 0
lw      $t1, -48($s0)
beq     $t1, $t0, label171
j       label172
label171:
lw      $t2, judge
sw      $t2, -52($s0)
lw      $t0, -52($s0)
add     $t0, $t0, 1
sw      $t0, -56($s0)
la      $a0, judge
sw      $t0, 0($a0)
label172:
li      $t1, 1
sw      $t1, -12($sp)
j       label164
label166:
li      $t2, 1
lw      $t0, -4($s0)
beq     $t0, $t2, label173
j       label174
label173:
lw      $t1, judge
sw      $t1, -60($s0)
lw      $t2, -60($s0)
add     $t2, $t2, 1
sw      $t2, -64($s0)
la      $a0, judge
sw      $t2, 0($a0)
li      $t0, 0
sw      $t0, -4($sp)
label176:
lw      $t1, -4($sp)
sw      $t1, -68($s0)
li      $t2, 2
lw      $t0, -68($s0)
sub     $t0, $t0, $t2
blez    $t0, label177
j       label175
label177:
lw      $t1, judge
sw      $t1, -72($s0)
lw      $t2, -72($s0)
add     $t2, $t2, 1
sw      $t2, -76($s0)
la      $a0, judge
sw      $t2, 0($a0)
lw      $t0, -4($sp)
sw      $t0, -80($s0)
lw      $t1, -80($s0)
add     $t1, $t1, 1
sw      $t1, -84($s0)
sw      $t1, -4($sp)
j       label176
label175:
li      $t2, 1
sw      $t2, -12($sp)
j       label164
label174:
li      $t0, 2
lw      $t1, -4($s0)
beq     $t1, $t0, label178
j       label179
label178:
j       label164
label179:
lw      $t2, judge
sw      $t2, -88($s0)
lw      $t0, -88($s0)
add     $t0, $t0, 1
sw      $t0, -92($s0)
la      $a0, judge
sw      $t0, 0($a0)
lw      $t1, -0($sp)
sw      $t1, -96($s0)
lw      $t2, const_3
sw      $t2, -100($s0)
lw      $t1, -96($s0)
lw      $t0, -100($s0)
div     $t1, $t1, $t0
sw      $t1, -104($s0)
li      $t2, 1
lw      $t0, -104($s0)
beq     $t0, $t2, label181
j       label182
label181:
lw      $t1, judge
sw      $t1, -108($s0)
lw      $t2, -108($s0)
add     $t2, $t2, 1
sw      $t2, -112($s0)
la      $a0, judge
sw      $t2, 0($a0)
li      $t0, 1
sw      $t0, -12($sp)
j       label180
label182:
label180:
lw      $t1, -12($sp)
sw      $t1, -116($s0)
li      $t2, 0
lw      $t0, -116($s0)
beq     $t0, $t2, label183
j       label184
label183:
lw      $t1, judge
sw      $t1, -120($s0)
lw      $t2, -120($s0)
add     $t2, $t2, 1
sw      $t2, -124($s0)
la      $a0, judge
sw      $t2, 0($a0)
label184:
li      $t0, 1
sw      $t0, -12($sp)
label164:
lw      $t1, -12($sp)
sw      $t1, -128($s0)
li      $t2, 0
lw      $t0, -128($s0)
beq     $t0, $t2, label185
j       label186
label185:
lw      $t1, judge
sw      $t1, -132($s0)
lw      $t2, -132($s0)
add     $t2, $t2, 1
sw      $t2, -136($s0)
la      $a0, judge
sw      $t2, 0($a0)
label186:
li      $t0, 0
sw      $t0, -12($sp)
lw      $t1, -0($sp)
sw      $t1, -140($s0)
lw      $t2, -140($s0)
add     $t2, $t2, 1
sw      $t2, -144($s0)
sw      $t2, -0($sp)
j       label162
label161:
li      $t0, 0
sw      $t0, -0($sp)
label188:
lw      $t1, -0($sp)
sw      $t1, -148($s0)
li      $t2, 3
lw      $t0, -148($s0)
sub     $t0, $t0, $t2
bltz    $t0, label189
j       label187
label189:
lw      $t1, -0($sp)
sw      $t1, -152($s0)
li      $t2, 0
lw      $t0, -152($s0)
beq     $t0, $t2, label190
j       label191
label190:
li      $t1, 97
sw      $t1, -16($sp)
label191:
lw      $t2, -0($sp)
sw      $t2, -156($s0)
li      $t0, 1
lw      $t1, -156($s0)
beq     $t1, $t0, label192
j       label193
label192:
li      $t2, 98
sw      $t2, -16($sp)
label193:
lw      $t0, -0($sp)
sw      $t0, -160($s0)
li      $t1, 2
lw      $t2, -160($s0)
beq     $t2, $t1, label194
j       label195
label194:
li      $t0, 99
sw      $t0, -16($sp)
label195:
lw      $t1, -16($sp)
sw      $t1, -164($s0)
li      $t2, 97
lw      $t0, -164($s0)
beq     $t0, $t2, label197
j       label198
label197:
lw      $t1, judge
sw      $t1, -168($s0)
lw      $t2, -168($s0)
add     $t2, $t2, 1
sw      $t2, -172($s0)
la      $a0, judge
sw      $t2, 0($a0)
j       label196
label198:
li      $t0, 98
lw      $t1, -164($s0)
beq     $t1, $t0, label199
j       label200
label199:
lw      $t2, judge
sw      $t2, -176($s0)
lw      $t0, -176($s0)
add     $t0, $t0, 1
sw      $t0, -180($s0)
la      $a0, judge
sw      $t0, 0($a0)
j       label196
label200:
lw      $t1, judge
sw      $t1, -184($s0)
lw      $t2, -184($s0)
add     $t2, $t2, 1
sw      $t2, -188($s0)
la      $a0, judge
sw      $t2, 0($a0)
label196:
lw      $t0, -0($sp)
sw      $t0, -192($s0)
lw      $t1, -192($s0)
add     $t1, $t1, 1
sw      $t1, -196($s0)
sw      $t1, -0($sp)
j       label188
label187:
li      $t2, 1
sw      $t2, -0($sp)
lw      $t0, -0($sp)
sw      $t0, -200($s0)
li      $t1, 1
lw      $t2, -200($s0)
beq     $t2, $t1, label202
j       label203
label202:
j       label201
label203:
label201:
jr      $ra
test_function:
sub     $sp, $sp, 4
sub     $s0, $sp, 8
li      $t0, 11
sw      $t0, -0($sp)
li      $t1, 97
sw      $t1, -4($sp)
lw      $t2, judge
sw      $t2, -0($s0)
lw      $t0, -0($s0)
add     $t0, $t0, 1
sw      $t0, -4($s0)
la      $a0, judge
sw      $t0, 0($a0)
lw      $t1, -4($sp)
sw      $t1, -12($s0)
sw      $t1, -36($sp)
sub     $sp, $sp, 32
sw      $ra, 0($sp)
jal     higher
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 32
add     $s0, $s0, 32
add     $t2, $0, $v0
sw      $t2, -8($s0)
sub     $sp, $sp, 32
sw      $ra, 0($sp)
jal     count_to_10
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 32
add     $s0, $s0, 28
add     $t0, $0, $v0
sw      $t0, -16($s0)
lw      $t1, -16($s0)
lw      $t2, -8($s0)
add     $t2, $t2, $t1
sw      $t2, -20($s0)
lw      $a0, -20($s0)
li      $v0, 1
syscall
la      $a0, enter
li      $v0, 4
syscall
sub     $sp, $sp, 32
sw      $ra, 0($sp)
jal     prt_message
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 32
add     $s0, $s0, 28
jr      $ra
test_printf:
sub     $sp, $sp, 4
sub     $s0, $sp, 4
li      $t0, 1
sw      $t0, -0($sp)
lw      $t1, judge
sw      $t1, -0($s0)
lw      $t2, -0($s0)
add     $t2, $t2, 1
sw      $t2, -4($s0)
la      $a0, judge
sw      $t2, 0($a0)
lw      $t0, -0($sp)
sw      $t0, -8($s0)
lw      $a0, -8($s0)
li      $v0, 1
syscall
la      $a0, enter
li      $v0, 4
syscall
li      $t1, 5
sw      $t1, -0($sp)
lw      $t2, -0($sp)
sw      $t2, -16($s0)
mul     $t2, $t2, 4
la      $t0, num
add     $t2, $t2, $t0
lw      $t1, 0($t2)
sw      $t1, -12($s0)
lw      $t2, -0($sp)
sw      $t2, -24($s0)
lw      $t0, const_1
sw      $t0, -28($s0)
lw      $t1, -28($s0)
lw      $t2, -24($s0)
add     $t2, $t2, $t1
sw      $t2, -32($s0)
mul     $t2, $t2, 4
la      $t0, num
add     $t2, $t2, $t0
lw      $t1, 0($t2)
sw      $t1, -20($s0)
lw      $t2, -20($s0)
lw      $t0, -12($s0)
add     $t0, $t0, $t2
sw      $t0, -36($s0)
lw      $t1, -36($s0)
add     $t1, $t1, 3
sw      $t1, -40($s0)
sub     $sp, $sp, 92
sw      $ra, 0($sp)
jal     count_to_10
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 92
add     $s0, $s0, 92
add     $t2, $0, $v0
sw      $t2, -44($s0)
lw      $t0, -40($s0)
lw      $t1, -44($s0)
mul     $t1, $t1, $t0
sw      $t1, -48($s0)
lw      $t2, const_3
sw      $t2, -52($s0)
lw      $t1, -48($s0)
lw      $t0, -52($s0)
div     $t1, $t1, $t0
sw      $t1, -56($s0)
lw      $t2, -56($s0)
sub     $t2, $t2, 0
sub     $t2, $0, $t2
sw      $t2, -60($s0)
lw      $t0, -60($s0)
add     $t0, $t0, 97
sw      $t0, -64($s0)
lw      $t1, const_a
sw      $t1, -68($s0)
lw      $t2, -68($s0)
lw      $t0, -64($s0)
add     $t0, $t0, $t2
sw      $t0, -72($s0)
li      $t1, 3
mul     $t1, $t1, 3
sw      $t1, -76($s0)
lw      $t2, -76($s0)
div     $t2, $t2, 4
sw      $t2, -80($s0)
lw      $t0, -80($s0)
lw      $t1, -72($s0)
sub     $t1, $t1, $t0
sw      $t1, -84($s0)
lw      $a0, -84($s0)
li      $v0, 1
syscall
la      $a0, enter
li      $v0, 4
syscall
jr      $ra
test_scanf:
sub     $sp, $sp, 4
sub     $s0, $sp, 24
lw      $t2, judge
sw      $t2, -0($s0)
lw      $t0, -0($s0)
add     $t0, $t0, 1
sw      $t0, -4($s0)
la      $a0, judge
sw      $t0, 0($a0)
li      $v0, 5
syscall
sw      $v0, -0($sp)
li      $v0, 12
syscall
sw      $v0, -12($sp)
lw      $t1, judge
sw      $t1, -8($s0)
lw      $t2, -0($sp)
sw      $t2, -12($s0)
lw      $t0, -12($s0)
lw      $t1, -8($s0)
add     $t1, $t1, $t0
sw      $t1, -16($s0)
lw      $t2, -12($sp)
sw      $t2, -20($s0)
lw      $t0, -20($s0)
lw      $t1, -16($s0)
add     $t1, $t1, $t0
sw      $t1, -24($s0)
la      $a0, judge
sw      $t1, 0($a0)
li      $v0, 12
syscall
sw      $v0, -12($sp)
lw      $t2, -12($sp)
sw      $t2, -28($s0)
li      $t0, 12
la      $t1, letter
add     $t0, $t0, $t1
lw      $t2, -28($s0)
sw      $t2, 0($t0)
lw      $t0, judge
sw      $t0, -32($s0)
lw      $t1, -12($sp)
sw      $t1, -36($s0)
lw      $t2, -36($s0)
lw      $t0, -32($s0)
add     $t0, $t0, $t2
sw      $t0, -40($s0)
la      $a0, judge
sw      $t0, 0($a0)
li      $v0, 5
syscall
sw      $v0, -0($sp)
lw      $t1, judge
sw      $t1, -44($s0)
lw      $t2, -0($sp)
sw      $t2, -48($s0)
lw      $t0, -48($s0)
lw      $t1, -44($s0)
add     $t1, $t1, $t0
sw      $t1, -52($s0)
la      $a0, judge
sw      $t1, 0($a0)
jr      $ra
test_varib2:
sub     $sp, $sp, 4
sub     $s0, $sp, 0
lw      $t2, judge
sw      $t2, -0($s0)
lw      $t0, -0($s0)
add     $t0, $t0, 1
sw      $t0, -4($s0)
la      $a0, judge
sw      $t0, 0($a0)
lw      $t1, _
sw      $t1, -8($s0)
li      $t2, 1
lw      $t0, -8($s0)
beq     $t0, $t2, label204
j       label205
label204:
lw      $t1, judge
sw      $t1, -12($s0)
lw      $t2, -12($s0)
add     $t2, $t2, 1
sw      $t2, -16($s0)
la      $a0, judge
sw      $t2, 0($a0)
label205:
jr      $ra
test_varib:
sub     $sp, $sp, 4
sub     $s0, $sp, 4
lw      $t0, judge
sw      $t0, -0($s0)
lw      $t1, -0($s0)
add     $t1, $t1, 1
sw      $t1, -4($s0)
la      $a0, judge
sw      $t1, 0($a0)
li      $t2, 100
sw      $t2, -0($sp)
lw      $t0, -0($sp)
sw      $t0, -8($s0)
li      $t1, 100
lw      $t2, -8($s0)
beq     $t2, $t1, label206
j       label207
label206:
lw      $t0, judge
sw      $t0, -12($s0)
lw      $t1, -12($s0)
add     $t1, $t1, 1
sw      $t1, -16($s0)
la      $a0, judge
sw      $t1, 0($a0)
label207:
sub     $sp, $sp, 24
sw      $ra, 0($sp)
jal     test_varib2
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 24
add     $s0, $s0, 24
jr      $ra
test_para4:
sub     $sp, $sp, 4
sub     $s0, $sp, 28
lw      $t2, judge
sw      $t2, -0($s0)
lw      $t0, -0($sp)
sw      $t0, -4($s0)
lw      $t1, -4($s0)
lw      $t2, -0($s0)
add     $t2, $t2, $t1
sw      $t2, -8($s0)
lw      $t0, -4($sp)
sw      $t0, -12($s0)
lw      $t1, -12($s0)
lw      $t2, -8($s0)
add     $t2, $t2, $t1
sw      $t2, -16($s0)
lw      $t0, -8($sp)
sw      $t0, -20($s0)
lw      $t1, -20($s0)
lw      $t2, -16($s0)
add     $t2, $t2, $t1
sw      $t2, -24($s0)
lw      $t0, -12($sp)
sw      $t0, -28($s0)
lw      $t1, -28($s0)
lw      $t2, -24($s0)
add     $t2, $t2, $t1
sw      $t2, -32($s0)
lw      $t0, -16($sp)
sw      $t0, -36($s0)
lw      $t1, -36($s0)
lw      $t2, -32($s0)
add     $t2, $t2, $t1
sw      $t2, -40($s0)
lw      $t0, -20($sp)
sw      $t0, -44($s0)
lw      $t1, -44($s0)
lw      $t2, -40($s0)
add     $t2, $t2, $t1
sw      $t2, -48($s0)
lw      $t0, -24($sp)
sw      $t0, -52($s0)
lw      $t1, -52($s0)
lw      $t2, -48($s0)
add     $t2, $t2, $t1
sw      $t2, -56($s0)
la      $a0, judge
sw      $t2, 0($a0)
jr      $ra
test_para1:
sub     $sp, $sp, 4
sub     $s0, $sp, 8
lw      $t0, judge
sw      $t0, -0($s0)
lw      $t1, -0($s0)
add     $t1, $t1, 1
sw      $t1, -4($s0)
la      $a0, judge
sw      $t1, 0($a0)
li      $v0, 1
jr      $ra
test_para2:
sub     $sp, $sp, 4
sub     $s0, $sp, 8
lw      $t2, judge
sw      $t2, -0($s0)
lw      $t0, -0($s0)
add     $t0, $t0, 1
sw      $t0, -4($s0)
la      $a0, judge
sw      $t0, 0($a0)
li      $v0, 97
jr      $ra
test_para3:
sub     $sp, $sp, 4
sub     $s0, $sp, 8
lw      $t1, judge
sw      $t1, -0($s0)
lw      $t2, -0($s0)
add     $t2, $t2, 1
sw      $t2, -4($s0)
la      $a0, judge
sw      $t2, 0($a0)
jr      $ra
test_para:
sub     $sp, $sp, 4
sub     $s0, $sp, 0
lw      $t0, judge
sw      $t0, -0($s0)
lw      $t1, -0($s0)
add     $t1, $t1, 1
sw      $t1, -4($s0)
la      $a0, judge
sw      $t1, 0($a0)
li      $t2, 1
sw      $t2, -12($sp)
li      $t0, 2
sw      $t0, -16($sp)
sub     $sp, $sp, 8
sw      $ra, 0($sp)
jal     test_para1
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 8
add     $s0, $s0, 20
li      $t1, 97
sw      $t1, -12($sp)
li      $t2, 98
sw      $t2, -16($sp)
sub     $sp, $sp, 8
sw      $ra, 0($sp)
jal     test_para2
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 8
add     $s0, $s0, 20
li      $t0, 97
sw      $t0, -12($sp)
li      $t1, 1
sw      $t1, -16($sp)
sub     $sp, $sp, 8
sw      $ra, 0($sp)
jal     test_para3
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 8
add     $s0, $s0, 20
li      $t2, 1
sw      $t2, -12($sp)
li      $t0, 2
sw      $t0, -16($sp)
li      $t1, 3
sw      $t1, -20($sp)
li      $t2, 4
sw      $t2, -24($sp)
li      $t0, 5
sw      $t0, -28($sp)
li      $t1, 6
sw      $t1, -32($sp)
li      $t2, 7
sw      $t2, -36($sp)
sub     $sp, $sp, 8
sw      $ra, 0($sp)
jal     test_para4
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 8
add     $s0, $s0, 40
jr      $ra
main:
sub     $sp, $sp, 4
sub     $s0, $sp, 4
li      $t0, 0
la      $a0, judge
sw      $t0, 0($a0)
lw      $t1, judge
sw      $t1, -0($s0)
lw      $t2, -0($s0)
add     $t2, $t2, 1
sw      $t2, -4($s0)
la      $a0, judge
sw      $t2, 0($a0)
li      $t0, 0
sw      $t0, -0($sp)
sub     $sp, $sp, 16
sw      $ra, 0($sp)
jal     test_global_variable
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 16
add     $s0, $s0, 28
sub     $sp, $sp, 16
sw      $ra, 0($sp)
jal     test_global_const
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 16
add     $s0, $s0, 36
sub     $sp, $sp, 16
sw      $ra, 0($sp)
jal     test_brackets
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 16
add     $s0, $s0, 16
sub     $sp, $sp, 16
sw      $ra, 0($sp)
jal     test_compare_operator
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 16
add     $s0, $s0, 28
sub     $sp, $sp, 16
sw      $ra, 0($sp)
jal     test_expression
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 16
add     $s0, $s0, 48
sub     $sp, $sp, 16
sw      $ra, 0($sp)
jal     test_while
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 16
add     $s0, $s0, 24
sub     $sp, $sp, 16
sw      $ra, 0($sp)
jal     test_if
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 16
add     $s0, $s0, 28
sub     $sp, $sp, 16
sw      $ra, 0($sp)
jal     test_switch
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 16
add     $s0, $s0, 40
sub     $sp, $sp, 16
sw      $ra, 0($sp)
jal     test_function
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 16
add     $s0, $s0, 24
sub     $sp, $sp, 16
sw      $ra, 0($sp)
jal     test_printf
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 16
add     $s0, $s0, 20
sub     $sp, $sp, 16
sw      $ra, 0($sp)
jal     test_scanf
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 16
add     $s0, $s0, 40
sub     $sp, $sp, 16
sw      $ra, 0($sp)
jal     test_varib
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 16
add     $s0, $s0, 20
sub     $sp, $sp, 16
sw      $ra, 0($sp)
jal     test_para
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 16
add     $s0, $s0, 16
la      $a0, _str2
li      $v0, 4
syscall
la      $a0, enter
li      $v0, 4
syscall
lw      $t1, judge
sw      $t1, -8($s0)
lw      $a0, -8($s0)
li      $v0, 1
syscall
la      $a0, enter
li      $v0, 4
syscall
j       end
end: