.data
.text
j       main
sum:
sub     $sp, $sp, 4
sub     $s0, $sp, 8
lw      $t0, -0($sp)
sw      $t0, -0($s0)
lw      $t1, -4($sp)
sw      $t1, -4($s0)
mult    $t0, $t1
mflo    $t2
sw      $t2, -8($s0)
lw      $t0, -8($s0)
sub     $t0, $t0, 0
sub     $t0, $0, $t0
sw      $t0, -12($s0)
lw      $t1, -0($sp)
sw      $t1, -16($s0)
lw      $t2, -4($sp)
sw      $t2, -20($s0)
lw      $t0, -20($s0)
lw      $t1, -16($s0)
sub     $t1, $t1, $t0
sw      $t1, -24($s0)
lw      $t2, -24($s0)
lw      $t0, -12($s0)
add     $t0, $t0, $t2
sw      $t0, -28($s0)
lw      $v0, -28($s0)
jr      $ra
main:
sub     $sp, $sp, 4
sub     $s0, $sp, 12
li      $v0, 5
syscall
sw      $v0, -0($sp)
li      $v0, 5
syscall
sw      $v0, -4($sp)
lw      $t1, -0($sp)
sw      $t1, -4($s0)
sw      $t1, -32($sp)
lw      $t2, -4($sp)
sw      $t2, -8($s0)
sw      $t2, -36($sp)
sub     $sp, $sp, 28
sw      $ra, 0($sp)
jal     sum
add     $sp, $sp, 4
lw      $ra, 0($sp)
add     $sp, $sp, 28
add     $s0, $s0, 28
add     $t0, $0, $v0
sw      $t0, -0($s0)
sw      $t0, -8($sp)
lw      $t1, -8($sp)
sw      $t1, -12($s0)
lw      $a0, -12($s0)
li      $v0, 1
syscall