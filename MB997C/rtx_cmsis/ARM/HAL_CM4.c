/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    HAL_CM4.C
 *      Purpose: Hardware Abstraction Layer for Cortex-M4
 *      Rev.:    V4.20
 *----------------------------------------------------------------------------
 *
 * Copyright (c) 1999-2009 KEIL, 2009-2012 ARM Germany GmbH
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  - Neither the name of ARM  nor the names of its contributors may be used 
 *    to endorse or promote products derived from this software without 
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/

#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_HAL_CM.h"
#include "rt_Task.h"
#include "rt_MemBox.h"


/*----------------------------------------------------------------------------
 *      Functions
 *---------------------------------------------------------------------------*/


/*--------------------------- rt_set_PSP ------------------------------------*/

__asm void rt_set_PSP (U32 stack) {
        MSR     PSP,R0
        BX      LR
}


/*--------------------------- os_set_env ------------------------------------*/

__asm void os_set_env (void) {
   /* Switch to Unprivileged/Privileged Thread mode, use PSP. */
        MOV     R0,SP                   ; PSP = MSP
        MSR     PSP,R0
        LDR     R0,=__cpp(&os_flags)
        LDRB    R0,[R0]
        LSLS    R0,#31
        MOVNE   R0,#0x02                ; Privileged Thread mode, use PSP
        MOVEQ   R0,#0x03                ; Unprivileged Thread mode, use PSP
        MSR     CONTROL,R0
        BX      LR

        ALIGN
}


/*--------------------------- _alloc_box ------------------------------------*/

__asm void *_alloc_box (void *box_mem) {
   /* Function wrapper for Unprivileged/Privileged mode. */
        LDR     R12,=__cpp(rt_alloc_box)
        MRS     R3,IPSR
        LSLS    R3,#24
        BXNE    R12
        MRS     R3,CONTROL
        LSLS    R3,#31
        BXEQ    R12
        SVC     0
        BX      LR

        ALIGN
}


/*--------------------------- _free_box -------------------------------------*/

__asm int _free_box (void *box_mem, void *box) {
   /* Function wrapper for Unprivileged/Privileged mode. */
        LDR     R12,=__cpp(rt_free_box)
        MRS     R3,IPSR
        LSLS    R3,#24
        BXNE    R12
        MRS     R3,CONTROL
        LSLS    R3,#31
        BXEQ    R12
        SVC     0
        BX      LR

        ALIGN
}


/*-------------------------- SVC_Handler ------------------------------------*/

__asm void SVC_Handler (void) {
        PRESERVE8

        IMPORT  SVC_Count
        IMPORT  SVC_Table
        IMPORT  rt_stk_check

        MRS     R0,PSP                  ; Read PSP
        LDR     R1,[R0,#24]             ; Read Saved PC from Stack
        LDRB    R1,[R1,#-2]             ; Load SVC Number
        CMP     R1,#0
        BNE     SVC_User

        LDM     R0,{R0-R3,R12}          ; Read R0-R3,R12 from stack
        PUSH    {R4,LR}                 ; Save EXC_RETURN
        BLX     R12                     ; Call SVC Function 
        POP     {R4,LR}                 ; Restore EXC_RETURN

        MRS     R12,PSP                 ; Read PSP
        STM     R12,{R0-R2}             ; Store return values

        LDR     R3,=__cpp(&os_tsk)
        LDM     R3,{R1,R2}              ; os_tsk.run, os_tsk.new
        CMP     R1,R2
        BXEQ    LR                      ; RETI, no task switch

        CBZ     R1,SVC_Next             ; Runtask deleted?
        TST     LR,#0x10                ; is it extended frame?
        VSTMDBEQ R12!,{S16-S31}         ; yes, stack also VFP hi-regs
        MOVEQ   R0,#0x01                ; os_tsk->stack_frame val
        MOVNE   R0,#0x00
        STRB    R0,[R1,#TCB_STACKF]     ; os_tsk.run->stack_frame = val
        STMDB   R12!,{R4-R11}           ; Save Old context
        STR     R12,[R1,#TCB_TSTACK]    ; Update os_tsk.run->tsk_stack

        PUSH    {R2,R3}
        BL      rt_stk_check            ; Check for Stack overflow
        POP     {R2,R3}

SVC_Next
        STR     R2,[R3]                 ; os_tsk.run = os_tsk.new

        LDR     R12,[R2,#TCB_TSTACK]    ; os_tsk.new->tsk_stack
        LDMIA   R12!,{R4-R11}           ; Restore New Context
        LDRB    R0,[R2,#TCB_STACKF]     ; Stack Frame
        CMP     R0,#0                   ; Basic/Extended Stack Frame
        VLDMIANE R12!,{S16-S31}         ; restore VFP hi-registers
        MVNNE   LR,#:NOT:0xFFFFFFED     ; set EXC_RETURN value
        MVNEQ   LR,#:NOT:0xFFFFFFFD
        MSR     PSP,R12                 ; Write PSP

SVC_Exit
        BX      LR

        /*------------------- User SVC ------------------------------*/

SVC_User
        PUSH    {R4,LR}                 ; Save Registers
        LDR     R2,=SVC_Count
        LDR     R2,[R2]
        CMP     R1,R2
        BHI     SVC_Done                ; Overflow

        LDR     R4,=SVC_Table-4
        LDR     R4,[R4,R1,LSL #2]       ; Load SVC Function Address

        LDM     R0,{R0-R3,R12}          ; Read R0-R3,R12 from stack
        BLX     R4                      ; Call SVC Function

        MRS     R12,PSP
        STM     R12,{R0-R3}             ; Function return values
SVC_Done
        POP     {R4,PC}                 ; RETI

        ALIGN
}


/*-------------------------- Sys_Handler ------------------------------------*/

__asm void Sys_Handler (void) {
        PRESERVE8

        EXPORT  SysTick_Handler
        EXPORT  PendSV_Handler

PendSV_Handler
        PUSH    {R4,LR}                 ; Save EXC_RETURN
        BL      __cpp(rt_pop_req)

Sys_Switch
        POP     {R4,LR}                 ; Restore EXC_RETURN

        LDR     R3,=__cpp(&os_tsk)
        LDM     R3,{R1,R2}              ; os_tsk.run, os_tsk.new
        CMP     R1,R2
        BXEQ    LR                      ; RETI, no task switch

        MRS     R12,PSP                 ; Read PSP
        TST     LR,#0x10                ; is it extended frame?
        VSTMDBEQ R12!,{S16-S31}         ; yes, stack also VFP hi-regs
        MOVEQ   R0,#0x01                ; os_tsk->stack_frame val
        MOVNE   R0,#0x00
        STRB    R0,[R1,#TCB_STACKF]     ; os_tsk.run->stack_frame = val
        STMDB   R12!,{R4-R11}           ; Save Old context
        STR     R12,[R1,#TCB_TSTACK]    ; Update os_tsk.run->tsk_stack

        PUSH    {R2,R3}
        BL      rt_stk_check            ; Check for Stack overflow
        POP     {R2,R3}

        STR     R2,[R3]                 ; os_tsk.run = os_tsk.new

        LDR     R12,[R2,#TCB_TSTACK]    ; os_tsk.new->tsk_stack
        LDMIA   R12!,{R4-R11}           ; Restore New Context
        LDRB    R0,[R2,#TCB_STACKF]     ; Stack Frame
        CMP     R0,#0                   ; Basic/Extended Stack Frame
        VLDMIANE R12!,{S16-S31}         ; restore VFP hi-regs
        MVNNE   LR,#:NOT:0xFFFFFFED     ; set EXC_RETURN value
        MVNEQ   LR,#:NOT:0xFFFFFFFD
        MSR     PSP,R12                 ; Write PSP

Sys_Exit
        BX      LR                      ; Return to Thread Mode

SysTick_Handler
        PUSH    {R4,LR}                 ; Save EXC_RETURN
        BL      __cpp(rt_systick)
        B       Sys_Switch

        ALIGN
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

