/**************************************************
 *
 * Internal assembler library CFI handling.
 *
 * Copyright 2010 IAR Systems AB.
 *
 * $Revision: 2299 $
 *
 **************************************************/

#ifndef CFI_INTERNAL_INC
#define CFI_INTERNAL_INC

#if __CODE_MODEL__ == __SMALL_CODE_MODEL__
#define XCFI_RET_ADDR_SIZE 2
#elif __CODE_MODEL__ == __MEDIUM_CODE_MODEL__
#define XCFI_RET_ADDR_SIZE 3
#elif __CODE_MODEL__ == __LARGE_CODE_MODEL__
#define XCFI_RET_ADDR_SIZE 3
#else
#error "Unknown code model"
#endif

        CFI Names cfiNamesInternal
        CFI StackFrame CFA SP DATA
        CFI Resource A:8, XL:8, XH:8, YL:8, YH:8, SP:16, CC:8
        CFI Resource PC:24, PCL:8, PCH:8, PCE:8
        CFI Resource ?b0:8, ?b1:8, ?b2:8, ?b3:8, ?b4:8, ?b5:8, ?b6:8, ?b7:8
        CFI Resource ?b8:8, ?b9:8, ?b10:8, ?b11:8, ?b12:8, ?b13:8, ?b14:8
        CFI Resource ?b15:8
        CFI ResourceParts PC PCE, PCH, PCL
        CFI EndNames cfiNamesInternal

XCFI_COMMON_HEADER:      MACRO

        CFI CodeAlign 1
        CFI DataAlign 1
        CFI ReturnAddress PC CODE
#if __CODE_MODEL__ == __SMALL_CODE_MODEL__
        CFI CFA SP+2
        CFI PC Concat
        CFI PCL Frame(CFA, 0)
        CFI PCH Frame(CFA, -1)
        CFI PCE SameValue
#elif __CODE_MODEL__ == __MEDIUM_CODE_MODEL__
        CFI CFA SP+3
        CFI PC Frame(CFA, -2)
        CFI PCL Undefined
        CFI PCH Undefined
        CFI PCE Undefined
#elif __CODE_MODEL__ == __LARGE_CODE_MODEL__
        CFI CFA SP+3
        CFI PC Frame(CFA, -2)
        CFI PCL Undefined
        CFI PCH Undefined
        CFI PCE Undefined
#else
#error "Unknown code model"
#endif

        ENDM

XCFI_BEGIN:     MACRO
        CFI     Block cfiBlock_\1 Using \2
        CFI     NoFunction
XCFI_OFFSET:    SET -XCFI_RET_ADDR_SIZE
        ENDM

XCFI_END:       MACRO
        CFI     EndBlock cfiBlock_\1
        ENDM

XCFI_ADJUST_SP: MACRO
XCFI_OFFSET:    SET XCFI_OFFSET + \1
#ifdef XCFI_NO_FRAME_CHECK
        CFI     CFA SUB(SP, LITERAL(XCFI_OFFSET))
#else
        CFI     CFA SP + -XCFI_OFFSET
#endif
        ENDM

#define XCFI_CFA_EXPR(expr) CFA ADD(SUB(SP,LITERAL(XCFI_OFFSET)),expr)

#define XCFI_TOS_ADDR ADD(CFA, LITERAL(XCFI_OFFSET + 1))

XCFI_PC_DC:	MACRO size
#if __CODE_MODEL__ == __SMALL_CODE_MODEL__
	CFI	PCL AND(ADD(LOAD(2, DATA, ADD(CFA, -1)), <size>), 0xFF)
	CFI	PCH AND(RSHIFTL(ADD(LOAD(2,DATA,ADD(CFA,-1)),<size>),8),0xFF)
#else
	CFI	PC ADD(LOAD(3, DATA, ADD(CFA, -2)), <size>)
#endif
	ENDM

XCFI_PC_FRAME:	MACRO
#ifdef XCFI_NO_FRAME_CHECK
#if __CODE_MODEL__ == __SMALL_CODE_MODEL__
	CFI	PCL AND(LOAD(2, DATA, ADD(CFA, -1)), 0xFF)
	CFI	PCH AND(RSHIFTL(LOAD(2, DATA, ADD(CFA, -1)), 8), 0xFF)
#else
	CFI	PC LOAD(3, DATA, ADD(CFA, -2))
#endif
        CFI     CFA SUB(SP, LITERAL(XCFI_OFFSET))
#else
#if __CODE_MODEL__ == __SMALL_CODE_MODEL__
	CFI	PCL Frame(CFA, 0)
	CFI	PCH Frame(CFA, -1)
#else
	CFI	PC Frame(CFA, -2)
#endif
#endif
	ENDM

#define XCFI_X OR(LSHIFT(XH,8),XL)

#define XCFI_Y OR(LSHIFT(YH,8),YL)

#define XCFI_W0 OR(LSHIFT(?b0,8),?b1)

#define XCFI_W1 OR(LSHIFT(?b2,8),?b3)

#define XCFI_E0 OR(LSHIFT(?b1,16), OR(LSHIFT(?b2,8),?b3))

XCFI_SAVE_A:    MACRO
        XCFI_ADJUST_SP -1
#ifdef XCFI_NO_FRAME_CHECK
        CFI     A LOAD(1, DATA, ADD(CFA, LITERAL(XCFI_OFFSET + 1)))
#else
        CFI     A Frame(CFA, XCFI_OFFSET + 1)
#endif
        ENDM

XCFI_RESTORE_A: MACRO
        XCFI_ADJUST_SP 1
        CFI     A SameValue
        ENDM

XCFI_SAVE_X:    MACRO
        XCFI_ADJUST_SP -2
#ifdef XCFI_NO_FRAME_CHECK
        CFI     XL LOAD(1, DATA, ADD(CFA, LITERAL(XCFI_OFFSET + 2)))
        CFI     XH LOAD(1, DATA, ADD(CFA, LITERAL(XCFI_OFFSET + 1)))
#else
        CFI     XL Frame(CFA, XCFI_OFFSET + 2)
        CFI     XH Frame(CFA, XCFI_OFFSET + 1)
#endif
        ENDM

XCFI_RESTORE_X: MACRO
        XCFI_ADJUST_SP 2
        CFI     XL SameValue
        CFI     XH SameValue
        ENDM

XCFI_SAVE_Y:    MACRO
        XCFI_ADJUST_SP -2
#ifdef XCFI_NO_FRAME_CHECK
        CFI     YL LOAD(1, DATA, ADD(CFA, LITERAL(XCFI_OFFSET + 2)))
        CFI     YH LOAD(1, DATA, ADD(CFA, LITERAL(XCFI_OFFSET + 1)))
#else
        CFI     YL Frame(CFA, XCFI_OFFSET + 2)
        CFI     YH Frame(CFA, XCFI_OFFSET + 1)
#endif
        ENDM

XCFI_RESTORE_Y: MACRO
        XCFI_ADJUST_SP 2
        CFI     YL SameValue
        CFI     YH SameValue
        ENDM

XCFI_SAVE_W4:   MACRO
        XCFI_ADJUST_SP -2
#ifdef XCFI_NO_FRAME_CHECK
        CFI     ?b9 LOAD(1, DATA, ADD(CFA, LITERAL(XCFI_OFFSET + 2)))
        CFI     ?b8 LOAD(1, DATA, ADD(CFA, LITERAL(XCFI_OFFSET + 1)))
#else
        CFI     ?b9 Frame(CFA, XCFI_OFFSET + 2)
        CFI     ?b8 Frame(CFA, XCFI_OFFSET + 1)
#endif
        ENDM

XCFI_RESTORE_W4:        MACRO
        XCFI_ADJUST_SP 2
        CFI     ?b9 SameValue
        CFI     ?b8 SameValue
        ENDM

XCFI_SAVE_W6:   MACRO
        XCFI_ADJUST_SP -2
#ifdef XCFI_NO_FRAME_CHECK
        CFI     ?b13 LOAD(1, DATA, ADD(CFA, LITERAL(XCFI_OFFSET + 2)))
        CFI     ?b12 LOAD(1, DATA, ADD(CFA, LITERAL(XCFI_OFFSET + 1)))
#else
        CFI     ?b13 Frame(CFA, XCFI_OFFSET + 2)
        CFI     ?b12 Frame(CFA, XCFI_OFFSET + 1)
#endif
        ENDM

XCFI_RESTORE_W6:        MACRO
        XCFI_ADJUST_SP 2
        CFI     ?b13 SameValue
        CFI     ?b12 SameValue
        ENDM

XCFI_SAVE_L2:   MACRO
        XCFI_ADJUST_SP -4
#ifdef XCFI_NO_FRAME_CHECK
        CFI     ?b11 LOAD(1, DATA, ADD(CFA, LITERAL(XCFI_OFFSET + 4)))
        CFI     ?b10 LOAD(1, DATA, ADD(CFA, LITERAL(XCFI_OFFSET + 3)))
        CFI     ?b9 LOAD(1, DATA, ADD(CFA, LITERAL(XCFI_OFFSET + 2)))
        CFI     ?b8 LOAD(1, DATA, ADD(CFA, LITERAL(XCFI_OFFSET + 1)))
#else
        CFI     ?b11 Frame(CFA, XCFI_OFFSET + 4)
        CFI     ?b10 Frame(CFA, XCFI_OFFSET + 3)
        CFI     ?b9 Frame(CFA, XCFI_OFFSET + 2)
        CFI     ?b8 Frame(CFA, XCFI_OFFSET + 1)
#endif
        ENDM

XCFI_RESTORE_L2:        MACRO
        XCFI_ADJUST_SP 4
        CFI     ?b11 SameValue
        CFI     ?b10 SameValue
        CFI     ?b9 SameValue
        CFI     ?b8 SameValue
        ENDM

XCFI_SAVE_L3:   MACRO
        XCFI_ADJUST_SP -4
#ifdef XCFI_NO_FRAME_CHECK
        CFI     ?b15 LOAD(1, DATA, ADD(CFA, LITERAL(XCFI_OFFSET + 4)))
        CFI     ?b14 LOAD(1, DATA, ADD(CFA, LITERAL(XCFI_OFFSET + 3)))
        CFI     ?b13 LOAD(1, DATA, ADD(CFA, LITERAL(XCFI_OFFSET + 2)))
        CFI     ?b12 LOAD(1, DATA, ADD(CFA, LITERAL(XCFI_OFFSET + 1)))
#else
        CFI     ?b15 Frame(CFA, XCFI_OFFSET + 4)
        CFI     ?b14 Frame(CFA, XCFI_OFFSET + 3)
        CFI     ?b13 Frame(CFA, XCFI_OFFSET + 2)
        CFI     ?b12 Frame(CFA, XCFI_OFFSET + 1)
#endif
        ENDM

XCFI_RESTORE_L3:        MACRO
        XCFI_ADJUST_SP 4
        CFI     ?b15 SameValue
        CFI     ?b14 SameValue
        CFI     ?b13 SameValue
        CFI     ?b12 SameValue
        ENDM

XCFI_PRESERVE_L0:       MACRO
        CFI     ?b0 SameValue
        CFI     ?b1 SameValue
        CFI     ?b2 SameValue
        CFI     ?b3 SameValue
        ENDM

XCFI_PRESERVE_L1:       MACRO
        CFI     ?b4 SameValue
        CFI     ?b5 SameValue
        CFI     ?b6 SameValue
        CFI     ?b7 SameValue
        ENDM

XCFI_PRESERVE_L2:       MACRO
        CFI     ?b8 SameValue
        CFI     ?b9 SameValue
        CFI     ?b10 SameValue
        CFI     ?b11 SameValue
        ENDM

XCFI_PRESERVE_L3:       MACRO
        CFI     ?b12 SameValue
        CFI     ?b13 SameValue
        CFI     ?b14 SameValue
        CFI     ?b15 SameValue
        ENDM

        CFI     Common cfiCommonInternal_StandardC Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A Undefined
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL Undefined
        CFI     YH Undefined
        CFI     CC Undefined
        CFI     ?b0 Undefined
        CFI     ?b1 Undefined
        CFI     ?b2 Undefined
        CFI     ?b3 Undefined
        CFI     ?b4 Undefined
        CFI     ?b5 Undefined
        CFI     ?b6 Undefined
        CFI     ?b7 Undefined
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_StandardC

        CFI     Common cfiCommonInternal_Interrupt Using cfiNamesInternal
        CFI     CodeAlign 1
        CFI     DataAlign 1
        CFI     ReturnAddress PC CODE
        CFI     CFA SP+9
        CFI     PC Frame(CFA, -2)
        CFI     PCL Undefined
        CFI     PCH Undefined
        CFI     PCE Undefined
        CFI     A Frame(CFA, -4)
        CFI     XL Frame(CFA, -5)
        CFI     XH Frame(CFA, -6)
        CFI     YL Frame(CFA, -7)
        CFI     YH Frame(CFA, -8)
        CFI     CC Frame(CFA, -3)
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_Interrupt

        CFI     Common cfiCommonInternal_none Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_none

        CFI     Common cfiCommonInternal_a Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A Undefined
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_a

        CFI     Common cfiCommonInternal_a_x Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A Undefined
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_a_x

        CFI     Common cfiCommonInternal_a_x_y Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A Undefined
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL Undefined
        CFI     YH Undefined
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_a_x_y

        CFI     Common cfiCommonInternal_a_x_y_b0 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A Undefined
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL Undefined
        CFI     YH Undefined
        CFI     CC Undefined
        CFI     ?b0 Undefined
        CFI     ?b1 SameValue
        CFI     ?b2 SameValue
        CFI     ?b3 SameValue
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_a_x_y_b0

        CFI     Common cfiCommonInternal_a_x_y_b2 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A Undefined
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL Undefined
        CFI     YH Undefined
        CFI     CC Undefined
        CFI     ?b0 SameValue
        CFI     ?b1 SameValue
        CFI     ?b2 Undefined
        CFI     ?b3 SameValue
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_a_x_y_b2

        CFI     Common cfiCommonInternal_a_x_y_l0 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A Undefined
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL Undefined
        CFI     YH Undefined
        CFI     CC Undefined
        CFI     ?b0 Undefined
        CFI     ?b1 Undefined
        CFI     ?b2 Undefined
        CFI     ?b3 Undefined
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_a_x_y_l0

        CFI     Common cfiCommonInternal_a_x_y_l1 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A Undefined
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL Undefined
        CFI     YH Undefined
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        CFI     ?b4 Undefined
        CFI     ?b5 Undefined
        CFI     ?b6 Undefined
        CFI     ?b7 Undefined
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_a_x_y_l1

        CFI     Common cfiCommonInternal_a_x_y_l0_l1 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A Undefined
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL Undefined
        CFI     YH Undefined
        CFI     CC Undefined
        CFI     ?b0 Undefined
        CFI     ?b1 Undefined
        CFI     ?b2 Undefined
        CFI     ?b3 Undefined
        CFI     ?b4 Undefined
        CFI     ?b5 Undefined
        CFI     ?b6 Undefined
        CFI     ?b7 Undefined
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_a_x_y_l0_l1

        CFI     Common cfiCommonInternal_a_x_l0 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A Undefined
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        CFI     ?b0 Undefined
        CFI     ?b1 Undefined
        CFI     ?b2 Undefined
        CFI     ?b3 Undefined
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_a_x_l0

        CFI     Common cfiCommonInternal_a_x_l0_l1 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A Undefined
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        CFI     ?b0 Undefined
        CFI     ?b1 Undefined
        CFI     ?b2 Undefined
        CFI     ?b3 Undefined
        CFI     ?b4 Undefined
        CFI     ?b5 Undefined
        CFI     ?b6 Undefined
        CFI     ?b7 Undefined
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_a_x_l0_l1

        CFI     Common cfiCommonInternal_a_x_l1 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A Undefined
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        CFI     ?b4 Undefined
        CFI     ?b5 Undefined
        CFI     ?b6 Undefined
        CFI     ?b7 Undefined
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_a_x_l1

        CFI     Common cfiCommonInternal_a_x_l2 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A Undefined
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        CFI     ?b8 Undefined
        CFI     ?b9 Undefined
        CFI     ?b10 Undefined
        CFI     ?b11 Undefined
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_a_x_l2

        CFI     Common cfiCommonInternal_a_x_l3 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A Undefined
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        CFI     ?b12 Undefined
        CFI     ?b13 Undefined
        CFI     ?b14 Undefined
        CFI     ?b15 Undefined
        CFI     EndCommon cfiCommonInternal_a_x_l3

        CFI     Common cfiCommonInternal_a_b0 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A Undefined
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        CFI     ?b0 Undefined
        CFI     ?b1 SameValue
        CFI     ?b2 SameValue
        CFI     ?b3 SameValue
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_a_b0

        CFI     Common cfiCommonInternal_a_l0 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A Undefined
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        CFI     ?b0 Undefined
        CFI     ?b1 Undefined
        CFI     ?b2 Undefined
        CFI     ?b3 Undefined
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_a_l0

        CFI     Common cfiCommonInternal_x Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x

        CFI     Common cfiCommonInternal_x_y Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL Undefined
        CFI     YH Undefined
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x_y

        CFI     Common cfiCommonInternal_x_w0 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        CFI     ?b0 Undefined
        CFI     ?b1 Undefined
        CFI     ?b2 SameValue
        CFI     ?b3 SameValue
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x_w0

        CFI     Common cfiCommonInternal_x_w1 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        CFI     ?b0 SameValue
        CFI     ?b1 SameValue
        CFI     ?b2 Undefined
        CFI     ?b3 Undefined
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x_w1

        CFI     Common cfiCommonInternal_x_w2 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        CFI     ?b4 Undefined
        CFI     ?b5 Undefined
        CFI     ?b6 SameValue
        CFI     ?b7 SameValue
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x_w2

        CFI     Common cfiCommonInternal_x_w3 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        CFI     ?b4 SameValue
        CFI     ?b5 SameValue
        CFI     ?b6 Undefined
        CFI     ?b7 Undefined
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x_w3

        CFI     Common cfiCommonInternal_x_w4 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        CFI     ?b8 Undefined
        CFI     ?b9 Undefined
        CFI     ?b10 SameValue
        CFI     ?b11 SameValue
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x_w4

        CFI     Common cfiCommonInternal_x_w5 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        CFI     ?b8 SameValue
        CFI     ?b9 SameValue
        CFI     ?b10 Undefined
        CFI     ?b11 Undefined
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x_w5

        CFI     Common cfiCommonInternal_x_w6 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        CFI     ?b12 Undefined
        CFI     ?b13 Undefined
        CFI     ?b14 SameValue
        CFI     ?b15 SameValue
        CFI     EndCommon cfiCommonInternal_x_w6

        CFI     Common cfiCommonInternal_x_w7 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        CFI     ?b12 SameValue
        CFI     ?b13 SameValue
        CFI     ?b14 Undefined
        CFI     ?b15 Undefined
        CFI     EndCommon cfiCommonInternal_x_w7

        CFI     Common cfiCommonInternal_x_e0 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        CFI     ?b0 SameValue
        CFI     ?b1 Undefined
        CFI     ?b2 Undefined
        CFI     ?b3 Undefined
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x_e0

        CFI     Common cfiCommonInternal_x_e1 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        CFI     ?b4 SameValue
        CFI     ?b5 Undefined
        CFI     ?b6 Undefined
        CFI     ?b7 Undefined
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x_e1

        CFI     Common cfiCommonInternal_x_e2 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        CFI     ?b8 SameValue
        CFI     ?b9 Undefined
        CFI     ?b10 Undefined
        CFI     ?b11 Undefined
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x_e2

        CFI     Common cfiCommonInternal_x_e3 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        CFI     ?b12 SameValue
        CFI     ?b13 Undefined
        CFI     ?b14 Undefined
        CFI     ?b15 Undefined
        CFI     EndCommon cfiCommonInternal_x_e3

        CFI     Common cfiCommonInternal_x_l0 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        CFI     ?b0 Undefined
        CFI     ?b1 Undefined
        CFI     ?b2 Undefined
        CFI     ?b3 Undefined
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x_l0

        CFI     Common cfiCommonInternal_x_l1 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        CFI     ?b4 Undefined
        CFI     ?b5 Undefined
        CFI     ?b6 Undefined
        CFI     ?b7 Undefined
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x_l1

        CFI     Common cfiCommonInternal_x_l2 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        CFI     ?b8 Undefined
        CFI     ?b9 Undefined
        CFI     ?b10 Undefined
        CFI     ?b11 Undefined
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x_l2

        CFI     Common cfiCommonInternal_x_l3 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        CFI     ?b12 Undefined
        CFI     ?b13 Undefined
        CFI     ?b14 Undefined
        CFI     ?b15 Undefined
        CFI     EndCommon cfiCommonInternal_x_l3

        CFI     Common cfiCommonInternal_x_y_w0 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL Undefined
        CFI     YH Undefined
        CFI     CC Undefined
        CFI     ?b0 Undefined
        CFI     ?b1 Undefined
        CFI     ?b2 SameValue
        CFI     ?b3 SameValue
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x_y_w0

        CFI     Common cfiCommonInternal_x_y_w1 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL Undefined
        CFI     YH Undefined
        CFI     CC Undefined
        CFI     ?b0 SameValue
        CFI     ?b1 SameValue
        CFI     ?b2 Undefined
        CFI     ?b3 Undefined
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x_y_w1

        CFI     Common cfiCommonInternal_x_y_w2 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL Undefined
        CFI     YH Undefined
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        CFI     ?b4 Undefined
        CFI     ?b5 Undefined
        CFI     ?b6 SameValue
        CFI     ?b7 SameValue
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x_y_w2

        CFI     Common cfiCommonInternal_x_y_l0 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL Undefined
        CFI     YH Undefined
        CFI     CC Undefined
        CFI     ?b0 Undefined
        CFI     ?b1 Undefined
        CFI     ?b2 Undefined
        CFI     ?b3 Undefined
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x_y_l0

        CFI     Common cfiCommonInternal_x_y_l1 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL Undefined
        CFI     YH Undefined
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        CFI     ?b4 Undefined
        CFI     ?b5 Undefined
        CFI     ?b6 Undefined
        CFI     ?b7 Undefined
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x_y_l1

        CFI     Common cfiCommonInternal_x_y_l2 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL Undefined
        CFI     YH Undefined
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        CFI     ?b8 Undefined
        CFI     ?b9 Undefined
        CFI     ?b10 Undefined
        CFI     ?b11 Undefined
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_x_y_l2

        CFI     Common cfiCommonInternal_x_y_l3 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL Undefined
        CFI     XH Undefined
        CFI     YL Undefined
        CFI     YH Undefined
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        CFI     ?b12 Undefined
        CFI     ?b13 Undefined
        CFI     ?b14 Undefined
        CFI     ?b15 Undefined
        CFI     EndCommon cfiCommonInternal_x_y_l3

        CFI     Common cfiCommonInternal_w0 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        CFI     ?b0 Undefined
        CFI     ?b1 Undefined
        CFI     ?b2 SameValue
        CFI     ?b3 SameValue
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_w0

        CFI     Common cfiCommonInternal_w1 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        CFI     ?b0 SameValue
        CFI     ?b1 SameValue
        CFI     ?b2 Undefined
        CFI     ?b3 Undefined
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_w1

        CFI     Common cfiCommonInternal_w2 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        CFI     ?b4 Undefined
        CFI     ?b5 Undefined
        CFI     ?b6 SameValue
        CFI     ?b7 SameValue
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_w2

        CFI     Common cfiCommonInternal_w3 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        CFI     ?b4 SameValue
        CFI     ?b5 SameValue
        CFI     ?b6 Undefined
        CFI     ?b7 Undefined
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_w3

        CFI     Common cfiCommonInternal_w4 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        CFI     ?b8 Undefined
        CFI     ?b9 Undefined
        CFI     ?b10 SameValue
        CFI     ?b11 SameValue
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_w4

        CFI     Common cfiCommonInternal_w5 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        CFI     ?b8 SameValue
        CFI     ?b9 SameValue
        CFI     ?b10 Undefined
        CFI     ?b11 Undefined
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_w5

        CFI     Common cfiCommonInternal_w6 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        CFI     ?b12 Undefined
        CFI     ?b13 Undefined
        CFI     ?b14 SameValue
        CFI     ?b15 SameValue
        CFI     EndCommon cfiCommonInternal_w6

        CFI     Common cfiCommonInternal_w7 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        CFI     ?b12 SameValue
        CFI     ?b13 SameValue
        CFI     ?b14 Undefined
        CFI     ?b15 Undefined
        CFI     EndCommon cfiCommonInternal_w7

        CFI     Common cfiCommonInternal_e0 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        CFI     ?b0 SameValue
        CFI     ?b1 Undefined
        CFI     ?b2 Undefined
        CFI     ?b3 Undefined
        XCFI_PRESERVE_L1
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_e0

        CFI     Common cfiCommonInternal_e1 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        CFI     ?b4 SameValue
        CFI     ?b5 Undefined
        CFI     ?b6 Undefined
        CFI     ?b7 Undefined
        XCFI_PRESERVE_L2
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_e1

        CFI     Common cfiCommonInternal_e2 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
        CFI     ?b8 SameValue
        CFI     ?b9 Undefined
        CFI     ?b10 Undefined
        CFI     ?b11 Undefined
        XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_e2

        CFI     Common cfiCommonInternal_e3 Using cfiNamesInternal
        XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        XCFI_PRESERVE_L0
        XCFI_PRESERVE_L1
	XCFI_PRESERVE_L2
        CFI     ?b12 SameValue
        CFI     ?b13 Undefined
        CFI     ?b14 Undefined
        CFI     ?b15 Undefined
        CFI     EndCommon cfiCommonInternal_e3

        CFI     Common cfiCommonInternal_l0 Using cfiNamesInternal
	XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
        CFI     ?b0 Undefined
        CFI     ?b1 Undefined
        CFI     ?b2 Undefined
        CFI     ?b3 Undefined
	XCFI_PRESERVE_L1
	XCFI_PRESERVE_L2
	XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_l0

        CFI     Common cfiCommonInternal_l1 Using cfiNamesInternal
	XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
	XCFI_PRESERVE_L0
        CFI     ?b4 Undefined
        CFI     ?b5 Undefined
        CFI     ?b6 Undefined
        CFI     ?b7 Undefined
	XCFI_PRESERVE_L2
	XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_l1

        CFI     Common cfiCommonInternal_l2 Using cfiNamesInternal
	XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
	XCFI_PRESERVE_L0
	XCFI_PRESERVE_L1
        CFI     ?b8 Undefined
        CFI     ?b9 Undefined
        CFI     ?b10 Undefined
        CFI     ?b11 Undefined
	XCFI_PRESERVE_L3
        CFI     EndCommon cfiCommonInternal_l2

        CFI     Common cfiCommonInternal_l3 Using cfiNamesInternal
	XCFI_COMMON_HEADER
        CFI     A SameValue
        CFI     XL SameValue
        CFI     XH SameValue
        CFI     YL SameValue
        CFI     YH SameValue
        CFI     CC Undefined
	XCFI_PRESERVE_L0
	XCFI_PRESERVE_L1
	XCFI_PRESERVE_L2
        CFI     ?b12 Undefined
        CFI     ?b13 Undefined
        CFI     ?b14 Undefined
        CFI     ?b15 Undefined
        CFI     EndCommon cfiCommonInternal_l3

#endif /* CFI_INTERNAL_INC */
