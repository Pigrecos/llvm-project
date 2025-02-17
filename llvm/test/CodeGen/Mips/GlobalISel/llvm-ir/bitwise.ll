; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc  -O0 -mtriple=mipsel-linux-gnu -global-isel  -verify-machineinstrs %s -o -| FileCheck %s -check-prefixes=MIPS32

define i1 @and_i1(i1 %a, i1 %b) {
; MIPS32-LABEL: and_i1:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    and $2, $5, $4
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %and = and i1 %b, %a
  ret i1 %and
}

define i8 @and_i8(i8 %a, i8 %b) {
; MIPS32-LABEL: and_i8:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    and $2, $5, $4
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %and = and i8 %b, %a
  ret i8 %and
}

define i16 @and_i16(i16 %a, i16 %b) {
; MIPS32-LABEL: and_i16:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    and $2, $5, $4
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %and = and i16 %b, %a
  ret i16 %and
}

define i32 @and_i32(i32 %a, i32 %b) {
; MIPS32-LABEL: and_i32:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    and $2, $5, $4
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %and = and i32 %b, %a
  ret i32 %and
}

define i64 @and_i64(i64 %a, i64 %b) {
; MIPS32-LABEL: and_i64:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    and $2, $6, $4
; MIPS32-NEXT:    and $3, $7, $5
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %and = and i64 %b, %a
  ret i64 %and
}

define i1 @or_i1(i1 %a, i1 %b) {
; MIPS32-LABEL: or_i1:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    or $2, $5, $4
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %or = or i1 %b, %a
  ret i1 %or
}

define i8 @or_i8(i8 %a, i8 %b) {
; MIPS32-LABEL: or_i8:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    or $2, $5, $4
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %or = or i8 %b, %a
  ret i8 %or
}

define i16 @or_i16(i16 %a, i16 %b) {
; MIPS32-LABEL: or_i16:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    or $2, $5, $4
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %or = or i16 %b, %a
  ret i16 %or
}

define i32 @or_i32(i32 %a, i32 %b) {
; MIPS32-LABEL: or_i32:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    or $2, $5, $4
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %or = or i32 %b, %a
  ret i32 %or
}

define i64 @or_i64(i64 %a, i64 %b) {
; MIPS32-LABEL: or_i64:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    or $2, $6, $4
; MIPS32-NEXT:    or $3, $7, $5
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %or = or i64 %b, %a
  ret i64 %or
}

define i1 @xor_i1(i1 %a, i1 %b) {
; MIPS32-LABEL: xor_i1:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    xor $2, $5, $4
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %xor = xor i1 %b, %a
  ret i1 %xor
}

define i8 @xor_i8(i8 %a, i8 %b) {
; MIPS32-LABEL: xor_i8:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    xor $2, $5, $4
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %xor = xor i8 %b, %a
  ret i8 %xor
}

define i16 @xor_i16(i16 %a, i16 %b) {
; MIPS32-LABEL: xor_i16:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    xor $2, $5, $4
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %xor = xor i16 %b, %a
  ret i16 %xor
}

define i32 @xor_i32(i32 %a, i32 %b) {
; MIPS32-LABEL: xor_i32:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    xor $2, $5, $4
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %xor = xor i32 %b, %a
  ret i32 %xor
}

define i64 @xor_i64(i64 %a, i64 %b) {
; MIPS32-LABEL: xor_i64:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    xor $2, $6, $4
; MIPS32-NEXT:    xor $3, $7, $5
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %xor = xor i64 %b, %a
  ret i64 %xor
}

define i32 @shl(i32 %a) {
; MIPS32-LABEL: shl:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    sll $2, $4, 1
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %shl = shl i32 %a, 1
  ret i32 %shl
}

define i32 @ashr(i32 %a) {
; MIPS32-LABEL: ashr:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    sra $2, $4, 1
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %shr = ashr i32 %a, 1
  ret i32 %shr
}

define i32 @lshr(i32 %a) {
; MIPS32-LABEL: lshr:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    srl $2, $4, 1
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %shr = lshr i32 %a, 1
  ret i32 %shr
}

define i32 @shlv(i32 %a, i32 %b) {
; MIPS32-LABEL: shlv:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    sllv $2, $4, $5
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %shl = shl i32 %a, %b
  ret i32 %shl
}

define i32 @ashrv(i32 %a, i32 %b) {
; MIPS32-LABEL: ashrv:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    srav $2, $4, $5
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %shr = ashr i32 %a, %b
  ret i32 %shr
}

define i32 @lshrv(i32 %a, i32 %b) {
; MIPS32-LABEL: lshrv:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    srlv $2, $4, $5
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %shr = lshr i32 %a, %b
  ret i32 %shr
}

define  i16 @shl_i16(i16 %a) {
; MIPS32-LABEL: shl_i16:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    ori $1, $zero, 2
; MIPS32-NEXT:    ori $2, $zero, 65535
; MIPS32-NEXT:    and $1, $1, $2
; MIPS32-NEXT:    sllv $2, $4, $1
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %shl = shl i16 %a, 2
  ret i16 %shl
}

define i8 @ashr_i8(i8 %a) {
; MIPS32-LABEL: ashr_i8:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    ori $1, $zero, 2
; MIPS32-NEXT:    ori $2, $zero, 255
; MIPS32-NEXT:    and $1, $1, $2
; MIPS32-NEXT:    sll $2, $4, 24
; MIPS32-NEXT:    sra $2, $2, 24
; MIPS32-NEXT:    srav $2, $2, $1
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %0 = ashr i8 %a, 2
  ret i8 %0
}

define i16 @lshr_i16(i16 %a) {
; MIPS32-LABEL: lshr_i16:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    ori $1, $zero, 2
; MIPS32-NEXT:    ori $2, $zero, 65535
; MIPS32-NEXT:    and $1, $1, $2
; MIPS32-NEXT:    and $2, $4, $2
; MIPS32-NEXT:    srlv $2, $2, $1
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %0 = lshr i16 %a, 2
  ret i16 %0
}

define i64 @shl_i64(i64 %a, i64 %b) {
; MIPS32-LABEL: shl_i64:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    addiu $sp, $sp, -8
; MIPS32-NEXT:    .cfi_def_cfa_offset 8
; MIPS32-NEXT:    ori $1, $zero, 32
; MIPS32-NEXT:    subu $2, $6, $1
; MIPS32-NEXT:    subu $3, $1, $6
; MIPS32-NEXT:    ori $8, $zero, 0
; MIPS32-NEXT:    sltu $1, $6, $1
; MIPS32-NEXT:    xor $9, $6, $8
; MIPS32-NEXT:    sltiu $9, $9, 1
; MIPS32-NEXT:    sllv $10, $4, $6
; MIPS32-NEXT:    srlv $3, $4, $3
; MIPS32-NEXT:    sllv $6, $5, $6
; MIPS32-NEXT:    or $3, $3, $6
; MIPS32-NEXT:    sllv $2, $4, $2
; MIPS32-NEXT:    ori $4, $zero, 1
; MIPS32-NEXT:    and $6, $1, $4
; MIPS32-NEXT:    movn $8, $10, $6
; MIPS32-NEXT:    and $1, $1, $4
; MIPS32-NEXT:    movn $2, $3, $1
; MIPS32-NEXT:    and $1, $9, $4
; MIPS32-NEXT:    movn $2, $5, $1
; MIPS32-NEXT:    sw $2, 4($sp) # 4-byte Folded Spill
; MIPS32-NEXT:    move $2, $8
; MIPS32-NEXT:    lw $3, 4($sp) # 4-byte Folded Reload
; MIPS32-NEXT:    addiu $sp, $sp, 8
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %shl = shl i64 %a, %b
  ret i64 %shl
}

define i64 @ashl_i64(i64 %a, i64 %b) {
; MIPS32-LABEL: ashl_i64:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    ori $1, $zero, 32
; MIPS32-NEXT:    subu $2, $6, $1
; MIPS32-NEXT:    subu $3, $1, $6
; MIPS32-NEXT:    ori $8, $zero, 0
; MIPS32-NEXT:    sltu $1, $6, $1
; MIPS32-NEXT:    xor $8, $6, $8
; MIPS32-NEXT:    sltiu $8, $8, 1
; MIPS32-NEXT:    srav $9, $5, $6
; MIPS32-NEXT:    srlv $6, $4, $6
; MIPS32-NEXT:    sllv $3, $5, $3
; MIPS32-NEXT:    or $3, $6, $3
; MIPS32-NEXT:    sra $6, $5, 31
; MIPS32-NEXT:    srav $2, $5, $2
; MIPS32-NEXT:    ori $5, $zero, 1
; MIPS32-NEXT:    and $10, $1, $5
; MIPS32-NEXT:    movn $2, $3, $10
; MIPS32-NEXT:    and $3, $8, $5
; MIPS32-NEXT:    movn $2, $4, $3
; MIPS32-NEXT:    and $1, $1, $5
; MIPS32-NEXT:    movn $6, $9, $1
; MIPS32-NEXT:    move $3, $6
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %shr = ashr i64 %a, %b
  ret i64 %shr
}

define i64 @lshr_i64(i64 %a, i64 %b) {
; MIPS32-LABEL: lshr_i64:
; MIPS32:       # %bb.0: # %entry
; MIPS32-NEXT:    ori $1, $zero, 32
; MIPS32-NEXT:    subu $2, $6, $1
; MIPS32-NEXT:    subu $3, $1, $6
; MIPS32-NEXT:    ori $8, $zero, 0
; MIPS32-NEXT:    sltu $1, $6, $1
; MIPS32-NEXT:    xor $9, $6, $8
; MIPS32-NEXT:    sltiu $9, $9, 1
; MIPS32-NEXT:    srlv $10, $5, $6
; MIPS32-NEXT:    srlv $6, $4, $6
; MIPS32-NEXT:    sllv $3, $5, $3
; MIPS32-NEXT:    or $3, $6, $3
; MIPS32-NEXT:    srlv $2, $5, $2
; MIPS32-NEXT:    ori $5, $zero, 1
; MIPS32-NEXT:    and $6, $1, $5
; MIPS32-NEXT:    movn $2, $3, $6
; MIPS32-NEXT:    and $3, $9, $5
; MIPS32-NEXT:    movn $2, $4, $3
; MIPS32-NEXT:    and $1, $1, $5
; MIPS32-NEXT:    movn $8, $10, $1
; MIPS32-NEXT:    move $3, $8
; MIPS32-NEXT:    jr $ra
; MIPS32-NEXT:    nop
entry:
  %shr = lshr i64 %a, %b
  ret i64 %shr
}
