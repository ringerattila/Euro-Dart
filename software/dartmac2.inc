;****************************************************************
;       Euro-Dart (C) Ringer Attila (C)1995-2023								*
;****************************************************************

; DART PROGRAM MAKRO DEFINICIOI

;	 6Mhz/12/250 oszto1=256-250 (0.5 ms) EZ FELFELE SZAMOL
;       12Mhz/12/250 oszto1=256-250 (0.5 ms) EZ FELFELE SZAMOL

;	6Mhz/12/250/200 oszto2=200 (0.1 s)  EZ LEFELE SZAMOL

OSZTO1		EQU	  6
OSZTO2		EQU	200


ACK     EQU     P10     ; IIC-BUS NYUGTAZAS
SDATA	EQU	P11	; IIC-BUS ADATJEL
SCL	EQU	P12     ; IIC-BUS ORAJEL
A_CL    EQU     P13     ; KIJELZO, FELSO, ORAJEL
F_CL    EQU     P14     ; KIJELZO, ALSO, ORAJEL
EOM     EQU     P15     ; INPUT
SN1     EQU     P16     ; HANG 1. CSATORNA, ALLITHATO
SN2     EQU     P17     ; HANG 2. CSATORNA, FIX. HANGERO



; INOUT PORTOK
COINPORT        EQU     8000H
INLSB           EQU     8001H
INMSB           EQU     8002H
BUTIN           EQU     8003H

;OUTPUT PORTOK
OUTLSB          EQU     8001H
OUTMSB          EQU     8002H
DISPDATA        EQU     8003H
ISDLSB          EQU     8004H
ISDMSB          EQU     8005H
OENPORT         EQU     8006H

MOD2            EQU    00111100B

MOD_BYTE        EQU     00111111B

K_EN          EQU     20H        ; KIJELZO ENGEDELYEZO BIT
K_TILT        EQU     255-K_EN   ; KIJELZO TILTO BIT

CE              EQU     A2        ; CHIP ENABLE

CE_H            EQU     04H       ; H SZINT
CEL             EQU     255-CE_H  ; L SZINT

PD              EQU     A3        ; POWER DOWN - INVERTALVA MUKODIK !!!

PDL             EQU     08H       ;
PDH             EQU     255-PDL         ;

PR              EQU     A4        ; PLAY/REC
PRH             EQU     10H       ; H SZIN - PLAY
PRL             EQU     255-PRH         ; LSZINT - REC

EEPH            EQU     80H
EEPL            EQU     255-EEPH

; MAKRO DEFINICIOK

DEL5U          .MACRO
                PUSH    A        ; 2u
                POP     A        ; 2u
                PUSH    A
                POP     A
                NOP              ; 1u
                .ENDM

KEYCLR          MACRO           ; TORLI A BILLENTYU ESEMENYT
                MOV  KEY,#0
                ENDM



KIIR            .MACRO
		.ENDM


; **********************************
;>MAKRO RUTINOK

;X_XBYTE       ADD      A,DPL
;;              MOV      DPL,A
;              MOVX     A,@DPTR
;              RET

;X_XBYTEKI     PUSH     PSW
;              ADD      A,DPL
;              MOV      DPL,A
;              MOV      A,R0
;              MOVX     @DPTR,A
;              POP      PSW
;              RET

; MAKRO - KODBYTE BEOLVASAS CIM+ELTOLAS CIMROL
CBYTE         MACRO    CIM, ELTOLAS
              MOV      DPTR,#CIM
              MOV      A,ELTOLAS
              MOVC     A,@A+DPTR
              ENDM

XBYTE          MACRO    CIM,ELTOLAS
               MOV      DPTR,ELTOLAS+CIM   ; #2
               MOVX     A,@DPTR            ; #2
               ENDM

XRBYTE        MACRO    CIM,ELTOLAS
              MOV      DPTR,#CIM        ; A CIMET BETOLTJUK
              MOV      A,ELTOLAS
              ADD      A,DPL
              MOV      DPL,A
              MOVX     A,@DPTR
              ENDM

;XBYTE         MACRO    CIM,ELTOLAS
;              MOV      DPTR,#CIM        ; A CIMET BETOLTJUK
;          ;    .IFNMA    1               ; HA VAN ARG.
;              MOV      A,ELTOLAS
;              LCALL    X_XBYTE
;          ;    .ENDIF
;              MOVX     A,@DPTR          ; BETOLTES
;              ENDM

XZBYTE        MACRO    CIM, ELTOLAS
              PUSH     PSW
              MOV      DPTR,#CIM
              MOV      A,ELTOLAS
              DEC      A
              ADD      A,DPL
              MOV      DPL,A
              MOVX     A,@DPTR
              POP      PSW
              ENDM


XZBYTEKI      MACRO    CIM, ELTOLAS,ADAT
              PUSH     PSW
              MOV      DPTR,#CIM
              MOV      A,ELTOLAS
              DEC      A
              ADD      A,DPL
              MOV      DPL,A
              MOV      A,ADAT
              MOVX     @DPTR,A
              POP      PSW
              ENDM

; X_BYTEKI      POP      PSW
;              POP      DPH
;              POP      DPL
;              RET

XBYTEKI       MACRO    CIM, ELTOLAS,ADAT
              PUSH     PSW
              MOV      A,ELTOLAS
              MOV      DPTR,#CIM
              ADD      A,DPL
              MOV      DPL,A
              MOV      A,ADAT
              MOVX     @DPTR,A
              POP      PSW
              ENDM

PLAYER           MACRO SZEGMENS,JATEKOS
                 MOV   SEG,#SZEGMENS
                 MOV   JNUM,#JATEKOS
                 ENDM

KILAMPALEC      MACRO
                LCALL   LAMPALEC_OFF
                BELAM IZTILOS
                ENDM

BELAM           MACRO LAMPANUM
                MOV   A,#LAMPANUM
            ;   SETB    C               ; C-BE 1 KELL (BEKAPCSOLAS)
                LCALL   IZZOK_BE
                ENDM

; BEJOVO ADAT A-BAN
;BELAMREG        MACRO
;                SETB    C               ; C-BE 1 KELL (BEKAPCSOLAS)
;                LCALL   IZZOK
;                ENDM

KILAM           MACRO LAMPANUM
                MOV   A,#LAMPANUM
             ;   CLR    C               ; C-BE 0 KELL (KIKAPCSOLAS)
                LCALL   IZZOK_KI
                ENDM

; BEJOVO ADAT A-BAN
KILAMREG        MACRO
                CLR    C               ; C-BE 0 KELL (KIKAPCSOLAS)
                LCALL   IZZOK
                ENDM


;**************************
; PORT IRAS-OLVASAS ELOKESZITES
; DPTR-BEN LESZ A PORTCIM, MAR CSAK KI KELL KULDENI AZ A-T


;* IRAST ELOKESZITO RUTINOK

WRITE0		.MACRO
                NOP
		.ENDM

WRITE1		.MACRO                  ; #7,
		MOV	DPTR,#OUTLSB     ; #2
		MOVX	@DPTR,A         ; #2
		.ENDM

WRITE2		.MACRO
		MOV	DPTR,#OUTMSB
		MOVX	@DPTR,A
		.ENDM

WRITE3		.MACRO
		MOV	DPTR,#DISPDATA
		MOVX	@DPTR,A
		.ENDM

WRITE4		.MACRO
		MOV	DPTR,#ISDLSB
		MOVX	@DPTR,A
		.ENDM

WRITE5		.MACRO
		MOV	DPTR,#ISDMSB
		MOVX	@DPTR,A
		.ENDM

WRITE6		.MACRO
		MOV	DPTR,#OENPORT
		MOVX	@DPTR,A
		.ENDM

;* OLVASAST ELOKESZITO RUTINOK

READ0		.MACRO
		MOV	DPTR,#COINPORT
		MOVX	A,@DPTR
		.ENDM

READ1		.MACRO                  ; #7
		MOV	DPTR,#INLSB
		MOVX	A,@DPTR
		.ENDM

READ2		.MACRO
		MOV	DPTR,#INMSB
		MOVX	A,@DPTR
		.ENDM

READ3		.MACRO
		MOV	DPTR,#BUTIN
		MOVX	A,@DPTR
		.ENDM

BUTTONLSB	MACRO
		O6EN		; ENGEDELYEZES KAPCS. OLVASASHOZ
		READ1
		PUSH	A
		O6DIS		; ENGEDELYEZES TITASA
		POP	A
		CPL	A	; INVERTALNI KELL
                ENDM

BUTTONMSB	MACRO
		O6EN		; ENGEDELYEZES KAPCS. OLVASASHOZ
		READ2
		PUSH	A
		O6DIS		; ENGEDELYEZES TITASA
		POP	A
		CPL	A	; INVERTALNI KELL
                ENDM

;*******************************************************
; 7406 BITEKET ENGEDELYEZES ES LETILTAS


O2EN		.MACRO                  ; #10
		MOV	A,SL6DATA	; #1, JELENLEGI ADAT BETOLTESE
		SETB	A2		; #1, A2 BIT BEALLITAS (INVERZ)
		MOV	SL6DATA,A	; #1, KIMENO ADAT ELMENTES
		WRITE6		        ; #7, PORT ELOKESZ. IRASRA
		.ENDM

O2DIS		.MACRO
		MOV	A,SL6DATA	; JELENLEGI ADAT BETOLTESE
		CLR	A2		; A2 BIT TILTAS (INVERZ)
		MOV	SL6DATA,A	; KIMENO ADAT ELMENTES
		WRITE6		        ; PORT ELOKESZ. IRASRA
		.ENDM


O1EN		.MACRO
		MOV	A,SL6DATA	; JELENLEGI ADAT BETOLTESE
		SETB	A3		; A2 BIT BEALLITAS (INVERZ)
		MOV	SL6DATA,A	; KIMENO ADAT ELMENTES
		WRITE6		        ; PORT ELOKESZ. IRASRA
		.ENDM

O1DIS		.MACRO
		MOV	A,SL6DATA	; JELENLEGI ADAT BETOLTESE
		CLR	A3		; A2 BIT TILTAS (INVERZ)
		MOV	SL6DATA,A	; KIMENO ADAT ELMENTES
		WRITE6		        ; PORT ELOKESZ. IRASRA
		.ENDM

O3EN		.MACRO
		MOV	A,SL6DATA	; JELENLEGI ADAT BETOLTESE
		SETB	A4		; A2 BIT BEALLITAS (INVERZ)
		MOV	SL6DATA,A	; KIMENO ADAT ELMENTES
		WRITE6		        ; PORT ELOKESZ. IRASRA
		.ENDM

O3DIS		.MACRO
		MOV	A,SL6DATA	; JELENLEGI ADAT BETOLTESE
		CLR	A4		; A2 BIT TILTAS (INVERZ)
		MOV	SL6DATA,A	; KIMENO ADAT ELMENTES
		WRITE6		        ; PORT ELOKESZ. IRASRA
		.ENDM

O4EN		.MACRO
		MOV	A,SL6DATA	; JELENLEGI ADAT BETOLTESE
		SETB	A5		; A2 BIT BEALLITAS (INVERZ)
		MOV	SL6DATA,A	; KIMENO ADAT ELMENTES
		WRITE6		        ; PORT ELOKESZ. IRASRA
		.ENDM

O4DIS		.MACRO
		MOV	A,SL6DATA	; JELENLEGI ADAT BETOLTESE
		CLR	A5		; A2 BIT TILTAS (INVERZ)
		MOV	SL6DATA,A	; KIMENO ADAT ELMENTES
		WRITE6		        ; PORT ELOKESZ. IRASRA
		.ENDM

O5EN		.MACRO
		MOV	A,SL6DATA	; JELENLEGI ADAT BETOLTESE
		SETB	A6		; A2 BIT BEALLITAS (INVERZ)
		MOV	SL6DATA,A	; KIMENO ADAT ELMENTES
		WRITE6		        ; PORT ELOKESZ. IRASRA
		.ENDM

O5DIS		.MACRO
		MOV	A,SL6DATA	; JELENLEGI ADAT BETOLTESE
		CLR	A6		; A2 BIT TILTAS (INVERZ)
		MOV	SL6DATA,A	; KIMENO ADAT ELMENTES
		WRITE6		        ; PORT ELOKESZ. IRASRA
		.ENDM

O6EN		.MACRO
		MOV	A,SL6DATA	; JELENLEGI ADAT BETOLTESE
		SETB	A7		; A2 BIT BEALLITAS (INVERZ)
		MOV	SL6DATA,A	; KIMENO ADAT ELMENTES
		WRITE6	        	; PORT ELOKESZ. IRASRA
		.ENDM

O6DIS		.MACRO
		MOV	A,SL6DATA	; JELENLEGI ADAT BETOLTESE
		CLR	A7		; A2 BIT TILTAS (INVERZ)
		MOV	SL6DATA,A	; KIMENO ADAT ELMENTES
		WRITE6	        	; PORT ELOKESZ. IRASRA
		.ENDM

INFEN		.MACRO
		MOV	A,VEZBYTE	; JELENLEGI ADAT BETOLTESE
		SETB	A6		; A7 BIT BEALLITAS
		MOV	VEZBYTE,A	; KIMENO ADAT ELMENTES
		WRITE5		        ; PORT IRASA
		.ENDM

INFDIS		.MACRO
		MOV	A,VEZBYTE	; JELENLEGI ADAT BETOLTESE
		CLR	A6		; A7 BIT TILTAS (INVERZ)
		MOV	VEZBYTE,A	; KIMENO ADAT ELMENTES
		WRITE5		        ; PORT IRAS
		.ENDM

FENYBE		.MACRO
		MOV	A,SL6DATA	; JELENLEGI ADAT BETOLTESE
		SETB	A1		; A6 BIT BEALLITAS (INVERZ)
		MOV	SL6DATA,A	; KIMENO ADAT ELMENTES
		WRITE6		        ; PORT ELOKESZ. IRASRA
		.ENDM

FENYKI	        .MACRO
		MOV	A,SL6DATA	; JELENLEGI ADAT BETOLTESE
		CLR	A1		; A6 BIT TILTAS (INVERZ)
		MOV	SL6DATA,A	; KIMENO ADAT ELMENTES
		WRITE6		        ; PORT ELOKESZ. IRASRA
		.ENDM

; A JATEKOS KIJELZOJE FOLOTTI KET IZZO BEKAPCSOLASA
; BEJOVO ADAT: JNUM-BAN A JATEKOS SZAMA
; JNUM-BAN ELO KELL KESZITENI AZ ADATOT

FLAMBE		MACRO
                PUSH	NUM
		PUSH	SEG
		MOV	NUM,#0
		MOV	SEG,#1
		LCALL	LAMPA
         ;       MOV     NUM,#0
		MOV	SEG,#0
		LCALL	LAMPA
                LCALL   DARTKIJ
		POP	SEG
		POP	NUM
		ENDM

FLAMKI		MACRO
                PUSH	NUM
		PUSH	SEG
                MOV	NUM,#1
		MOV	SEG,#1
		LCALL	LAMPA
             ;   MOV     NUM,#1
       		MOV	SEG,#0
		LCALL	LAMPA
                LCALL   DARTKIJ
		POP	SEG
		POP	NUM
		ENDM

DELAY             .MACRO TIZED          ; MAKRO A RUTINHOZ
                  MOV    A,#TIZED       ; A-BA A TIZEDEK SZAMA
                  LCALL  TDELAY         ; A TDELAY RUTIN HIVASA
                  .ENDM

REGDELAY          .MACRO TIZED          ; MAKRO A RUTINHOZ
                  MOV    A,TIZED       ; A-BA A TIZEDEK SZAMA
                  LCALL  TDELAY         ; A TDELAY RUTIN HIVASA
                  .ENDM

ADNYIL          MACRO   MENNYI
                MOV     A,#MENNYI
                LCALL   AD_NYIL
                ENDM

; KIJELZO VEZERLO ORAJELEK
A_CLK_UP        MACRO          ; FELSO KIJELZO FELFUTO ORAJEL
                CLR    A_CL
              ; NOP
              ; NOP
                ENDM

A_CLK_DN        MACRO          ; FELSO KIJELZO LEFUTO ORAJEL
                SETB    A_CL
              ;  NOP
              ; NOP
                ENDM

A_CLK           MACRO         ; EGY KOMPLETT ORAJEL A FELSO KIJELZORE
                A_CLK_UP
                NOP
                A_CLK_DN
                ENDM


F_CLK_UP        MACRO          ; FELSO KIJELZO FELFUTO ORAJEL
                CLR    F_CL
            ;    NOP
            ;    NOP
                ENDM

F_CLK_DN        MACRO          ; FELSO KIJELZO LEFUTO ORAJEL
                SETB    F_CL
            ;    NOP
            ;    NOP
                ENDM

F_CLK           MACRO         ; EGY KOMPLETT ORAJEL A FELSO KIJELZORE
                F_CLK_UP
                NOP
                F_CLK_DN
                ENDM


KIJ_EN          .MACRO
                ORL    VEZBYTE,#K_EN    ; A JELENLEGI ALLAPOTTAL OSSZEVETJUK
                MOV    A,VEZBYTE        ; A-T FOGJUK KIIRATNI
                WRITE5
                .ENDM

KIJ_TILT         .MACRO
                ANL    VEZBYTE,#K_TILT   ; A JELENLEGI ALLAPOTTAL OSSZEVETJUK
                MOV    A,VEZBYTE        ; A-T FOGJUK KIIRATNI
                WRITE5
                .ENDM


;****************************************************-
; hang ic vezerlo rutinok

PDHIGH          .MACRO
                ANL   VEZBYTE,#PDH     ; A JELENLEGI ALLAPOTTAL OSSZEVETJUK
                MOV   A,VEZBYTE        ; A-T FOGJUK KIIRATNI
                WRITE5
                .ENDM

PDLOW           .MACRO
                ORL   VEZBYTE,#PDL     ; A JELENLEGI ALLAPOTTAL OSSZEVETJUK
                MOV   A,VEZBYTE        ; A-T FOGJUK KIIRATNI
                WRITE5
                .ENDM


CEHIGH          .MACRO
                ORL  VEZBYTE,#CE_H     ; A JELENLEGI ALLAPOTTAL OSSZEVETJUK
                MOV  A,VEZBYTE        ; A-T FOGJUK KIIRATNI
                WRITE5
                .ENDM

CELOW           .MACRO
                ANL    VEZBYTE,#CEL     ; A JELENLEGI ALLAPOTTAL OSSZEVETJUK
                MOV    A,VEZBYTE        ; A-T FOGJUK KIIRATNI
                WRITE5
                .ENDM


SETPLAY         .MACRO
                ORL  VEZBYTE,#PRH     ; A JELENLEGI ALLAPOTTAL OSSZEVETJUK
                MOV  A,VEZBYTE        ; A-T FOGJUK KIIRATNI
                WRITE5
                .ENDM

SETREC          .MACRO
                ANL   VEZBYTE,#PRL     ; A JELENLEGI ALLAPOTTAL OSSZEVETJUK
                MOV   A,VEZBYTE        ; A-T FOGJUK KIIRATNI
                WRITE5
                .ENDM

EEPDIS          .MACRO
                ORL  VEZBYTE,#EEPH    ; A JELENLEGI ALLAPOTTAL OSSZEVETJUK
                MOV  A,VEZBYTE        ; A-T FOGJUK KIIRATNI
                WRITE5
                .ENDM

EEPEN           .MACRO
                ANL   VEZBYTE,#EEPL    ; A JELENLEGI ALLAPOTTAL OSSZEVETJUK
                MOV   A,VEZBYTE        ; A-T FOGJUK KIIRATNI
                WRITE5
                .ENDM

BECOU		.MACRO
                PUSH  A
		MOV	A,SL6DATA	; JELENLEGI ADAT BETOLTESE
		SETB	A0		; A2 BIT BEALLITAS (INVERZ)
		MOV	SL6DATA,A	; KIMENO ADAT ELMENTES
		WRITE6		        ; PORT ELOKESZ. IRASRA
                POP     A
		.ENDM

KICOU		.MACRO
                PUSH    A
		MOV	A,SL6DATA	; JELENLEGI ADAT BETOLTESE
		CLR	A0		; A2 BIT TILTAS (INVERZ)
		MOV	SL6DATA,A	; KIMENO ADAT ELMENTES
		WRITE6		        ; PORT ELOKESZ. IRASRA
                POP     A
		.ENDM
