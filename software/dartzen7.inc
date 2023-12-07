;****************************************************************
;       Euro-Dart (C) Ringer Attila (C)1995-2023								*
;****************************************************************

;  ** DART GEP ZENE KEZELESE

  DB 'HANGOK'

DUPHANG         DB      0, 31H, 99H, 31H, 31H, 31H, 31H, 0, 31H, 0,  0, 0
                DB     31H,  0,  0,  0,  0, 31H,  0, 0,  0, 0, 31H

TRIPHANG        DB      0, 47H, 99H, 47H, 47H, 47H, 47H, 0, 47H, 0,  0, 0
                DB     47H,  0,  0,  0,  0, 47H,  0, 0,  0, 0, 47H

;******************************************************
; HANG ELOALLITAS
; MINDEN ESEMENY SAJAT RUTIN MEGHIVASAVAL AD HANGOT
; HANGEFFEKT
; >HANGAD
HH5A            LJMP    HH5            ; SEGED UGRO CIM
HANGAD          PUSH    AREG
                PUSH    JNUM
                PUSH    BREG
                PUSH    B
                JB      ELVESZNYIL,HH5A ; HA ELVESSZUK A NYILAT, NINCS HANG
                MOV     A,BREG          ; MEGNEZZUK, MILYEN JATEK
                CJNE    A,#GAME_CRICKET,HH0A  ; HA NEM CRICKET, UGRAS
                JB      HASZNOS,HH0B    ; CRICKET, NEM HOZ PONTOT => NULLDOB
                JB      ROSSZDOB,HH0C   ; HA ROSSZ DOBAS, UGRIK
                MOV     A,#H_NULLDOB       ; NEM HASZNOS,
                LCALL   H_EFFEKT
                LJMP    HH5
HH0B            XBYTE   OLD_CR,#0
                MOV     DREG,A
HH0A            JNB     ROSSZDOB,HH0
HH0C            MOV     A,#H_FALSE
                LCALL   H_EFFEKT
                LJMP    HH5
HH0             MOV     A,DREG
                JNZ     HH1
                MOV     A,#H_NULLDOB
                LCALL   H_EFFEKT
                SJMP    HH5
HH1             CJNE    A,#1,HH2
HH1A            MOV     A,#H_JODOB
                LCALL   H_EFFEKT
                SJMP    HH5
HH2             CJNE    A,#2,HH3
                MOV     A,BREG
                CJNE    A,#GAME_CRICKET,HH2B  ; CRICKET-NEL VAN HANG
                SJMP    HH2A
HH2B            CJNE    A,#GAME_LOSCORE,HH2C  ; LO SCORE-NAL NEM KELL HANG
                SJMP    HH1A
HH2C            MOV     A,CTPONT
             ;  LCALL   KIIRAS8
                CBYTE   DUPHANG,BREG       ; MEGEZI, KELL-E DUPLA HANG
                CLR     C                  ; CARRY TORLESE KIVONAS ELOTT
                SUBB    A,CTPONT           ; KIVONAS
                JNC      HH1A              ; NEM KELL DUPLA HANG, UGRAS
HH2A            MOV     A,#H_DUPDOB
                LCALL   H_EFFEKT
                MOV     B,#B_DOUBLE
                LCALL   B_PLAYER
                SJMP    HH5
HH3             CJNE    A,#3,HH4
                MOV     A,BREG
                CJNE    A,#GAME_CRICKET,HH3B  ; CRICKET-NEL VAN HANG
                SJMP    HH3A
HH3B            CJNE    A,#GAME_LOSCORE,HH3C  ; LO SCORE-NAL NEM KELL HANG
                SJMP    HH1A
HH3C            CBYTE   TRIPHANG,BREG         ; MEGEZI, KELL-E TRIPLA HANG
                CLR     C                     ; CARRY TORLESE KIVONAS ELOTT
                SUBB    A,CTPONT              ; KIVONAS
                JNC     HH1A                  ; NEM KELL DUPLA HANG, UGRAS
HH3A            MOV     A,#H_TRIPDOB
                LCALL   H_EFFEKT
                MOV     B,#B_TRIPLE
                LCALL   B_PLAYER
                SJMP    HH5
HH4             CJNE    A,#4,HH6
                MOV     A,#H_BULL25
                LCALL   H_EFFEKT
                MOV     B,#B_BULL
                LCALL   B_PLAYER
                SJMP    HH5
HH6             MOV     A,#H_BULL50
                LCALL   H_EFFEKT
                MOV     B,#B_BULL
                LCALL   B_PLAYER
HH5         ;   CLR     P10             ; HANG BIT NULLABA
                POP     B
                POP     BREG
                POP     JNUM
                POP     AREG
                RET

; EZ A RUTIN MEGHIVJA A JATEKHOZ TARTOZO ELLENORZO RUTINT
; BEJOVO ADAT : A-BAN AZ EFFEKT SORSZAMA
; >H_EFFEKT
H_EFFEKT        PUSH   PSW
                PUSH   AREG
                PUSH   BREG
                PUSH   DREG
                PUSH   JNUM
                PUSH   B
                PUSH   SEG
                PUSH   A
                POP    A
                MOV    DPTR,#EFFECT
                DEC    A
                RL     A
                RL     A
                JMP    @A+DPTR           ; UGRAS ERRE A CIMRE
EFF_VEG     ;   PDLOW                    ; HANG IC ENGEDELYEZES
                CLR    P16
                CLR    P17
                POP    SEG
                POP    B
                POP    JNUM
                POP    DREG
                POP    BREG
                POP    AREG
                POP    PSW
                RET                      ; IDE VARJUK VISSZA AZ UGRAST

EFFECT        LJMP   E_GYOZ0            ; 1.  E_GYOZ0
              NOP
              LJMP   E_PENZBE           ; 2.  E_PENZBE
              NOP
              LJMP   E_VALTO            ; 3.  E_VALTO
              NOP
              LJMP   E_JODOB            ; 4.  E_JODOB
              NOP
              LJMP   E_LOWEST           ; 5.  E_LOWEST
              NOP
              LJMP   E_HIGHEST          ; 6.  E_HIGHEST
              NOP
              LJMP   E_FIVEJO           ; 7.  E_FIVEJO
              NOP
              LJMP   E_FIVENEM          ; 8.  E_FIVENEM
              NOP
              LJMP   E_BULL50           ; 9.  E_BULL50
              NOP
              LJMP   E_BULL25           ; 10. E_BULL25
              NOP
              LJMP   E_FALSE            ; 11. E_FALSE
              NOP
              LJMP   E_PENZKEY          ; 12. E_PENZKEY
              NOP
              LJMP   E_VILL             ; 13. E_VILL
              NOP
              LJMP   E_KISZHANG         ; 14. E_KISZHANG
              NOP
              LJMP   E_TRIPDOB          ; 15. E_TRIPDOB
              NOP
              LJMP   E_NULLDOB          ; 16. E_NULLDOB
              NOP
              LJMP   E_DUPDOB           ; 17. E_DUPDOB
              NOP
              LJMP   E_BEKAP            ; 18. E_BEKAP
              NOP
              LJMP   E_NYVESZ           ; 19. E_NYVESZ
              NOP
              LJMP   E_RESET            ; 20. E_RESET
              NOP
              LJMP   E_SORSOL           ; 21. E_SORSOL
              NOP
              LJMP   E_PARC             ; 22. E_PARC
              NOP
              LJMP   E_SZIREN           ; 23. E_SZIREN
              NOP
              LJMP   E_NYVEGE           ; 24. E_NYVEGE
              NOP


H_GYOZ0            EQU          1       ;  H_GYOZ0
H_PENZBE           EQU          2       ;  H_PENZBE
H_VALTO            EQU          3       ;  H_VALTO
H_JODOB            EQU          4       ;  H_JODOB
H_LOWEST           EQU          5       ;  H_LOWEST
H_HIGHEST          EQU          6       ;  H_HIGHEST
H_FIVEJO           EQU          7       ;  H_FIVEJO
H_FIVENEM          EQU          8       ;  H_FIVENEM
H_BULL50           EQU          9       ;  H_BULL50
H_BULL25           EQU          10      ; H_BULL25
H_FALSE            EQU          11      ; H_FALSE
H_PENZKEY          EQU          12      ; H_PENZKEY
H_VILL             EQU          13      ; H_VILL
H_KISZHANG         EQU          14      ; H_KISZHANG
H_TRIPDOB          EQU          15      ; H_TRIPDOB
H_NULLDOB          EQU          16      ; H_NULLDOB
H_DUPDOB           EQU          17      ; H_DUPDOB
H_BEKAP            EQU          18      ; H_BEKAP
H_NYVESZ           EQU          19      ; H_NYVESZ
H_RESET            EQU          20      ; H_RESET
H_SORSOL           EQU          21      ; H_SORSOL
H_PARC             EQU          22      ; H_PARC
H_SZIREN           EQU          23      ; H_SZIREN
H_NYVEGE           EQU          24      ; H_NYVEGE

; BILLENTYUNYOMAS  HANGJA
;E_BILL     PUSH    A
;           MOV     AREG,#180      ; 1 KHZ HANG
;           MOV     B,#20          ; HOSSZ 0.1 SEC
;           LCALL   BEEP
;           POP     A
;           RET

; ZENE NELKULI GYOZELEM HIRDETES
; >E_GYOZ0  - 01.
E_GYOZ0    MOV     DREG,#40
           LJMP    E_SORSOL
           LJMP    EFF_VEG

; EZ JO LESZ PENZBEDOBASHOZ
; >H-PENZBE  - 02.
E_PENZBE   MOV     AREG,#140
           MOV     B,#7    ; ITT 20 VOLT
           LCALL   BEEP3
           LJMP    EFF_VEG

; EZ LESZ EGY UJ JATEKOS DOBASAIHOZ
; >E_VALTO - 03.
E_VALTO    MOV     AREG,#128      ; FREKI
           MOV     B,#15          ; HOSSZ
           LCALL   BEEP
           MOV     AREG,#102      ; FREKI
           MOV     B,#15          ; HOSSZ
           LCALL   BEEP
           MOV     AREG,#86       ; FREKI
           MOV     B,#15          ; HOSSZ
           LCALL   BEEP
           MOV     AREG,#64       ; FREKI
           MOV     B,#45          ; HOSSZ
           LCALL   BEEP
           LJMP    EFF_VEG

; EZ LESZ AZ ERVENYES DOBASHOZ
; >E_JODOB - 04.
E_JODOB    MOV     AREG,#40
           MOV     B,#25
           LCALL   BEEP
           LJMP    EFF_VEG

; A LEGMELYEBB HANG
; >E_LOWEST  - 05.
E_LOWEST   MOV     AREG,#254
           MOV     B,#50
           LCALL   BEEP2
           LJMP    EFF_VEG

; EGY ELEG MAGAS HANG
; >E_HIGHEST  - 06.
E_HIGHEST  MOV     AREG,#20
           MOV     B,#80
           LCALL   BEEP
           LJMP    EFF_VEG

; FIVES OTTEL OSZTHATO A VEGE
; >E_FIVEJO  - 07.
E_FIVEJO        MOV     AREG,#64
                MOV     B,#20
                LCALL   BEEP
                LCALL   TIM30MS
                MOV     AREG,#64
                MOV     B,#20
                LCALL   BEEP
                LCALL   TIM30MS
                MOV     AREG,#64
                MOV     B,#50
                LCALL   BEEP
                LJMP    EFF_VEG

;  FIVES NEM OSZTHATO OTTEL
; >E_FIVENEM - 08.
E_FIVENEM       JB      ELVESZNYIL,FV1  ; HA ELVESZIK A NYILAT, NEM KELL
                MOV     AREG,#214
                MOV     B,#30
                LCALL   BEEP
                MOV     AREG,#240
                MOV     B,#30
                LCALL   BEEP
                MOV     AREG,#254
                MOV     B,#60
                LCALL   BEEP
FV1             LJMP    EFF_VEG

SJUM1        LJMP    E_TRIPDOB
; >E_BULL50 - 09.
E_BULL50   JB    CHAMP,SJUM1
           MOV   JNUM,#0
TOR1       MOV   DPTR,#TORRF
           MOV   A,JNUM
           MOVC  A,@A+DPTR
           MOV   AREG,A
           INC   JNUM
           MOV   A,JNUM
           MOVC  A,@A+DPTR
           MOV   B,A
           INC   JNUM
           LCALL BEEP
           LCALL TIM30MS
           MOV   A,JNUM
           CJNE  A,#20,TOR1
           LJMP    EFF_VEG

SJUM2        LJMP    E_DUPDOB
; >H-BULL25 - 10.
E_BULL25   JB    CHAMP,SJUM2
           MOV   JNUM,#0
TOR3       MOV   DPTR,#TORRF
           MOV   A,JNUM
           MOVC  A,@A+DPTR
           MOV   AREG,A
           INC   JNUM
           MOV   A,JNUM
           MOVC  A,@A+DPTR
           MOV   B,A
           INC   JNUM
           LCALL BEEP
           LCALL TIM30MS
           MOV   A,JNUM
           CJNE  A,#10,TOR3
           LJMP  EFF_VEG

; EZ JO LESZ FALSE DOBAS ESETERE - A BEEP-BE BE EGY INC AREG-T !!!
; >E_FALSE - 11.
E_FALSE    JB      CHAMP,FSE1
           MOV     B,#90           ; 120 VOLT
           MOV     AREG,#40
           SJMP    FSE2
FSE1       MOV     B,#50
           MOV     AREG,#40
FSE2       LCALL   BEEP2
           LJMP    EFF_VEG

; EZ JO LESZ KULCSOS PENZBEDOBASHOZ
; >E_PENZKEY  - 12.
E_PENZKEY  MOV     AREG,#200
           MOV     B,#20
           LCALL   BEEP3
           LJMP    EFF_VEG

; LAMPAVILLOGASHOZ HASZNALT HANG
; >E_VILL - 13.
E_VILL     MOV     AREG,#200
           MOV     B,#10
           LCALL   BEEP
           LJMP    EFF_VEG

; EGY KISZALLO NYIL ESETEN FIGYELEM FELKELTESHEZ
; >KISZHANG - 14.
E_KISZHANG LCALL    TIM100MS
           LCALL    COIN
           LCALL    TIM100MS
           MOV     AREG,#24       ;
           MOV     B,#14          ; HOSSZ 0.1 SEC
           LCALL   BEEP
           LCALL   TIM30MS
           MOV     AREG,#24       ;
           MOV     B,#14          ; HOSSZ 0.1 SEC
           LCALL   BEEP
           LJMP    EFF_VEG

; TRIPLA DOBAS
; >E_TRIPDOB - 15.
E_TRIPDOB       JB      CHAMP,MD1A
                MOV     JNUM,#4            ; 5 VOLT
                SJMP    MD1
MD1A            MOV     JNUM,#2
MD1             MOV     AREG,#48
                MOV     B,#6
                LCALL   BEEP
                MOV     AREG,#38
                MOV     B,#6
                LCALL   BEEP
                MOV     AREG,#64
                MOV     B,#6
                LCALL   BEEP
                DJNZ    JNUM,MD1
                LJMP    EFF_VEG

; ROSSZ DOBAS
; >E_NULLDOB - 16.
E_NULLDOB  MOV     AREG,#240
           MOV     B,#50
           LCALL   BEEP
           LJMP    EFF_VEG

; DUPLA DOV BAS
; >E_DUPDOB - 17.
E_DUPDOB        JB      CHAMP,MD3A
                MOV     JNUM,#4            ; 5 VOLT
                SJMP    MD3
MD3A            MOV     JNUM,#2
MD3             MOV     AREG,#40
                MOV     B,#10
                LCALL   BEEP
                MOV     AREG,#80
                MOV     B,#10
                LCALL   BEEP
                DJNZ    JNUM,MD3
                LJMP    EFF_VEG

; EZ LESZ BEKAPCSOLASHOZ
; >E_BEKAP - 18.
E_BEKAP    MOV   JNUM,#0
BD2        MOV   DPTR,#BUD
           MOV   A,JNUM
           MOVC  A,@A+DPTR
           MOV   AREG,A
           INC   JNUM
           MOV   A,JNUM
           MOVC  A,@A+DPTR
           MOV   B,A
           INC   JNUM
           LCALL BEEP
           LCALL TIM30MS
           LCALL COIN
           LCALL TIM30MS
           MOV   A,JNUM
           CJNE  A,#12,BD2
           LJMP    EFF_VEG

; GOMBBAL TORTENO NYILELVETELHEZ
; >E_NYVESZ - 19.
E_NYVESZ   MOV     B,#30
           MOV     AREG,#144
           LCALL   BEEP
           MOV     B,#30
           MOV     AREG,#170
           LCALL   BEEP
           MOV     B,#60
           MOV     AREG,#128
           LCALL   BEEP
           MOV     B,#60
           MOV     AREG,#170
           LCALL   BEEP
           LJMP    EFF_VEG

; BILLENTYUS RESET HANGJA
; >E_RESET - 20.
E_RESET    MOV     AREG,#48       ; 1 KHZ HANG
           MOV     B,#20          ; HOSSZ 0.1 SEC
           LCALL   BEEP3
           LJMP    EFF_VEG

; SORSOLAS HANGJA
; BEJOVO ADAT: DREG - (1-100) ENNYISZER 15 MSEC-IG TART A HANG
; >SORSOL - 21.
E_SORSOL   MOV      SEG,#100
HGY1       MOV      AREG,SEG
           MOV      B,#2
           LCALL    BEEP
           DEC      SEG
           DEC      SEG
           DJNZ     DREG,HGY1
           LJMP     EFF_VEG

PTIME      EQU     20

; 301PARC NULLAZAS  GYASZINDULO
; >PARC - 22.
E_PARC     MOV   JNUM,#0
PRC1       MOV   DPTR,#PARC
           MOV   A,JNUM
           MOVC  A,@A+DPTR
           MOV   AREG,A
           INC   JNUM
           MOV   A,JNUM
           MOVC  A,@A+DPTR
           MOV   B,A
           INC   JNUM
           LCALL BEEP
           LCALL TIM30MS
           MOV   A,JNUM
           CJNE  A,#20,PRC1
           LJMP    EFF_VEG

; SZIRENA
; >SZIRENA - 23.
E_SZIREN   MOV      JNUM,#8
HSZI3      MOV      SEG,#90
           MOV      DREG,#20
HSZI1      MOV      AREG,SEG
           MOV      B,#1
           LCALL    BEEP
           DEC      SEG
           DEC      SEG
           DEC      SEG
           DEC      SEG
           DJNZ     DREG,HSZI1
           MOV      SEG,#10
           MOV      DREG,#20
HSZI2      MOV      AREG,SEG
           MOV      B,#1
           LCALL    BEEP
           INC      SEG
           INC      SEG
           INC      SEG
           INC      SEG
           DJNZ     DREG,HSZI2
           DJNZ     JNUM,HSZI3
           LJMP    EFF_VEG

; >E_NYVEGE
E_NYVEGE   MOV     AREG,#64       ; FREKI
           MOV     B,#15          ; HOSSZ
           LCALL   BEEP
   ;        MOV     AREG,#86       ; FREKI
   ;        MOV     B,#15          ; HOSSZ
   ;        LCALL   BEEP
   ;        MOV     AREG,#102      ; FREKI
   ;        MOV     B,#15          ; HOSSZ
   ;        LCALL   BEEP
           MOV     AREG,#128      ; FREKI
           MOV     B,#45          ; HOSSZ
           LCALL   BEEP
           LJMP    EFF_VEG


      DB   'ZENE'

NG1        EQU         254
NGIS1      EQU         240
NA1        EQU         228
NB1        EQU         214
NH1        EQU         202
NC2        EQU         190
NCIS2      EQU         180
ND2        EQU         170
NDIS2      EQU         160
NE2        EQU         152
NF2        EQU         144
NFIS2      EQU         136
NG2        EQU         128
NGIS2      EQU         120
NA2        EQU         114
NB2        EQU         108
NH2        EQU         102
NC3        EQU          96
ND3        EQU          86
NES3       EQU          80
NE3        EQU          76
NF3        EQU          72
NG3        EQU          64


;UTEM       MACRO   PATTERN,LENGTH
;           MOV   DPTR,#PATTERN
;           MOV   SNUM,#LENGTH
;           LCALL PLAY
;     ;      LCALL GYOZ_FENY
;           ENDM

TEMP_TEN_32      EQU          6     ; TENGERESZ
TEMP_OLD_32      EQU          4     ; OLDBOYS
TEMP_GRA_32      EQU          5     ; GRANDFA
TEMP_TOR_32      EQU         6  ; TOROK
TEMP_LAN_32      EQU         5  ; AULD LANG

TP_TEN_32       EQU          TEMP_TEN_32
TP_TEN_16       EQU          TEMP_TEN_32+TEMP_TEN_32
TP_TEN_8        EQU          TP_TEN_16+TP_TEN_16
TP_TEN_10       EQU          TP_TEN_8    ; +TP_TEN_16
TP_TEN_4        EQU          TP_TEN_8+TP_TEN_8
TP_TEN_R8       EQU          TEMP_TEN_32+TEMP_TEN_32+TEMP_TEN_32
                                           ; ( NYOLCAD TRIOLA)
TP_TEN_2        EQU          TP_TEN_4+TP_TEN_4
TP_TEN_6        EQU          TP_TEN_4+TP_TEN_8

TP_OLD_32       EQU          TEMP_OLD_32
TP_OLD_16       EQU          TEMP_OLD_32+TEMP_OLD_32
TP_OLD_8        EQU          TP_OLD_16+TP_OLD_16
TP_OLD_10       EQU          TP_OLD_8    ; +TP_OLD_16
TP_OLD_4        EQU          TP_OLD_8+TP_OLD_8
TP_OLD_R8       EQU          TEMP_OLD_32+TEMP_OLD_32+TEMP_OLD_32
                                         ; ( NYOLCAD TRIOLA)
TP_OLD_2        EQU          TP_OLD_4+TP_OLD_4
TP_OLD_6        EQU          TP_OLD_4+TP_OLD_8

TP_GRA_32       EQU          TEMP_GRA_32
TP_GRA_16       EQU          TEMP_GRA_32+TEMP_GRA_32
TP_GRA_8        EQU          TP_GRA_16+TP_GRA_16
TP_GRA_10       EQU          TP_GRA_8    ; +TP_GRA_16
TP_GRA_4        EQU          TP_GRA_8+TP_GRA_8
TP_GRA_R8       EQU          TEMP_GRA_32+TEMP_GRA_32+TEMP_GRA_32
                                        ; ( NYOLCAD TRIOLA)
TP_GRA_2        EQU          TP_GRA_4+TP_GRA_4
TP_GRA_6        EQU          TP_GRA_4+TP_GRA_8

TP_TOR_32       EQU          TEMP_TOR_32
TP_TOR_16       EQU          TEMP_TOR_32+TEMP_TOR_32
TP_TOR_8        EQU          TP_TOR_16+TP_TOR_16
TP_TOR_10       EQU          TP_TOR_8    ; +TP_TOR_16
TP_TOR_4        EQU          TP_TOR_8+TP_TOR_8
TP_TOR_R8       EQU          TEMP_TOR_32+TEMP_TOR_32+TEMP_TOR_32
                                                 ; ( NYOLCAD TRIOLA)
TP_TOR_2        EQU          TP_TOR_4+TP_TOR_4
TP_TOR_6        EQU          TP_TOR_4+TP_TOR_8

TP_LAN_32       EQU          TEMP_LAN_32
TP_LAN_16       EQU          TEMP_LAN_32+TEMP_LAN_32
TP_LAN_8        EQU          TP_LAN_16+TP_LAN_16
TP_LAN_10       EQU          TP_LAN_8    ; +TP_LAN_16
TP_LAN_4        EQU          TP_LAN_8+TP_LAN_8
TP_LAN_R8       EQU          TEMP_LAN_32+TEMP_LAN_32+TEMP_LAN_32
                                                   ; ( NYOLCAD TRIOLA)
TP_LAN_2        EQU          TP_LAN_4+TP_LAN_4
TP_LAN_6        EQU          TP_LAN_4+TP_LAN_8


NSZ        EQU          2  ; SZUNETHEZ MAGAS HANG

;HANGOL     DB           NG2,TP_LAN_2
;
;HANGOLO    UTEM         HANGOL,2
;           RET


; AULD LANG SYNE
SYNE1      DB          NC2,TP_LAN_4

SYNE2      DB          NF2,TP_LAN_4+TP_LAN_8
           DB          NF2,TP_LAN_8
SYNE2B     DB          NF2,TP_LAN_4
SYNE2C     DB          NA2,TP_LAN_4

SYNE3      DB          NG2,TP_LAN_4+TP_LAN_8
           DB          NF2,TP_LAN_8
SYNE3B     DB          NG2,TP_LAN_4
SYNE3C      DB          NA2,TP_LAN_4

SYNE4      DB          NF2,TP_LAN_4+TP_LAN_8
           DB          NF2,TP_LAN_8
SYNE4B     DB          NA2,TP_LAN_4
SYNE4C     DB          NC3,TP_LAN_4

SYNE5      DB          ND3,TP_LAN_2+TP_LAN_4
SYNE5C     DB          ND3,TP_LAN_4

SYNE6      DB          NC3,TP_LAN_4+TP_LAN_8
           DB          NA2,TP_LAN_8
SYNE6B     DB          NA2,TP_LAN_4
SYNE6C     DB          NF2,TP_LAN_4

SYNE7      DB          NG2,TP_LAN_4+TP_LAN_8
           DB          NF2,TP_LAN_8
SYNE7B     DB          NG2,TP_LAN_4
SYNE7C     DB          NA2,TP_LAN_4

SYNE8      DB          NF2,TP_LAN_4+TP_LAN_8
           DB          ND2,TP_LAN_8
SYNE8B     DB          ND2,TP_LAN_4
SYNE8C     DB          NC2,TP_LAN_4

SYNE9      DB          NF2,TP_LAN_2+TP_LAN_4
SYNE9C     DB          ND3,TP_LAN_4

SYNE10     DB          NC3,TP_LAN_4+TP_LAN_8
           DB          NA2,TP_LAN_8
SYNE10B     DB          NA2,TP_LAN_4
SYNE10C     DB          NF2,TP_LAN_4

SYNE11     DB          NG2,TP_LAN_4+TP_LAN_8
           DB          NF2,TP_LAN_8
SYNE11B    DB          NG2,TP_LAN_4
SYNE11C    DB          ND3,TP_LAN_4

SYNE12     DB          NC3,TP_LAN_4+TP_LAN_8
           DB          NA2,TP_LAN_8
SYNE12B    DB          NA2,TP_LAN_4
SYNE12C    DB          NC3,TP_LAN_4

SYNE13     DB          ND3,TP_LAN_2+TP_LAN_4
SYNE13C    DB          NF3,TP_LAN_4

SYNE14     DB          NG2,TP_LAN_4+TP_LAN_8
           DB          NF2,TP_LAN_8
SYNE14B     DB          NG2,TP_LAN_4
SYNE14C     DB          NA2,TP_LAN_8
           DB          NG2,TP_LAN_8

SYNE15     DB          NF2,TP_LAN_4+TP_LAN_8
           DB          ND2,TP_LAN_8
SYNE15B     DB          ND2,TP_LAN_4
SYNE15C     DB          NC2,TP_LAN_4

SYNE16     DB          NF2,TP_LAN_2+TP_LAN_4

LANGS      DW        SYNE1,2
           DW        SYNE2,6    ; B
           DW        SYNE2C,2
           DW        SYNE3,6    ; B
           DW        SYNE3C,2
           DW        SYNE4,6    ; B
           DW        SYNE4C,2
           DW        SYNE5,2    ; B
           DW        SYNE5C,2
           DW        SYNE6,6    ; B
           DW        SYNE6C,2
           DW        SYNE7,6    ; B
           DW        SYNE7C,2
           DW        SYNE8,6    ; B
           DW        SYNE8C,2
           DW        SYNE16,2   ; B

           DW        SYNE1,2
           DW        SYNE2,6    ; B
           DW        SYNE2C,2
           DW        SYNE3,6    ; B
           DW        SYNE3C,2
           DW        SYNE4,6    ; B
           DW        SYNE4C,2
           DW        SYNE5,2    ; B
           DW        SYNE5C,2

           DW        SYNE6,6    ; B
           DW        SYNE6C,2
           DW        SYNE7,6    ; B
           DW        SYNE7C,2
           DW        SYNE8,6    ; B
           DW        SYNE8C,2
           DW        SYNE9,2    ; B
           DW        SYNE9C,2
           DW        SYNE10,6   ; B
           DW        SYNE10C,2
           DW        SYNE11,6   ; B
           DW        SYNE11C,2
           DW        SYNE12,6   ; B
           DW        SYNE12C,2
           DW        SYNE13,2   ; B
           DW        SYNE13C,2
           DW        SYNE6,6    ; B
           DW        SYNE6C,2
           DW        SYNE14,6   ; B
           DW        SYNE14C,4
           DW        SYNE15,6   ; B
           DW        SYNE15C,2
           DW        SYNE9,2    ; B
           DW        SYNE9C,2
           DW        SYNE10,6   ; B
           DW        SYNE10C,2
           DW        SYNE11,6   ; B
           DW        SYNE11C,2
           DW        SYNE12,6   ; B
           DW        SYNE12C,2
           DW        SYNE13,2   ; B
           DW        SYNE13C,2
           DW        SYNE6,6    ; B
           DW        SYNE6C,2
           DW        SYNE14,6   ; B
           DW        SYNE14C,4
           DW        SYNE15,6   ; B
           DW        SYNE15C,2
           DW        SYNE16,2   ; B
           RET


; TOROK INDULO LEJATSZO
; >P_TOROK
P_TOROK         PUSH   DREG
                MOV    DPTR,#TOROK
                MOV    DREG,#47         ; ENNYI UTEMBOL ALL A DAL
                SJMP   C_PLAY


; GRANDFATHER!S CLOCK LEJATSZO
; >P_GRANDFA
P_GRANDFA       PUSH   DREG
                MOV    DPTR,#GRANDFA
                MOV    DREG,#92         ; ENNYI UTEMBOL ALL A DAL
                SJMP   C_PLAY


; OLDBOYS LEJATSZO
; >P_OLDBOYS
P_OLDBOYS       PUSH   DREG
                MOV    DPTR,#OLDBOYS
                MOV    DREG,#64         ; ENNYI UTEMBOL ALL A DAL
                SJMP   C_PLAY

; AULD LANG SYNE LEJATSZO
; >P_LANGS
P_LANGS         PUSH   DREG
                MOV    DPTR,#LANGS
                MOV    DREG,#64         ; ENNYI UTEMBOL ALL A DAL
                SJMP   C_PLAY

; WHAT SHALL WE DO LEJATSZO
; >P_TENG
P_TENG          PUSH   DREG
                MOV    DPTR,#TENG
                MOV    DREG,#32         ; ENNYI UTEMBOL ALL A DAL
                SJMP   C_PLAY

; >C_PLAY
C_PLAY      ;   PUSH   BREG
                PUSH   PSW
                MOV    PSW,#rbank2
PLP1            MOV    A,#0
                MOVC   A,@A+DPTR
                MOV    R0,A             ; DPH LESZ
                INC    DPTR
                MOV    A,#0
                MOVC   A,@A+DPTR
                MOV    R1,A             ; DPL LESZ
                INC    DPTR
                MOV    A,#0
                MOVC   A,@A+DPTR        ; EZ UGYIS NULLA LESZ
                MOV    R2,A
                INC    DPTR
                MOV    A,#0
                MOVC   A,@A+DPTR
                INC    DPTR
                MOV    SNUM,A            ; EZ MEGY AZ SNUM -BA
                PUSH   DPL
                PUSH   DPH
                MOV    DPH,R0           ; UTEMCIMEK MEGADASA
                MOV    DPL,R1
                LCALL  PLAY             ; LEJATSZO RUTIN
                POP    DPH
                POP    DPL
                JB     TILT,PLP2
                DJNZ   DREG,PLP1
PLP2            POP    PSW
             ;  POP    BREG
                POP    DREG
                RET

; TOROK INDULO

TORK1      DB          NSZ,TP_TOR_8
           DB          NH1,TP_TOR_16
           DB          NA1,TP_TOR_16
           DB          NGIS1,TP_TOR_16
           DB          NA1,TP_TOR_16

TORK1B     DB          NC2,TP_TOR_8
           DB          NSZ,TP_TOR_8

TORK1C     DB          ND2,TP_TOR_16
           DB          NC2,TP_TOR_16
           DB          NH1,TP_TOR_16
           DB          NC2,TP_TOR_16

TORK2      DB          NE2,TP_TOR_8
           DB          NSZ,TP_TOR_8

TORK2B     DB          NF2,TP_TOR_16
           DB          NE2,TP_TOR_16
           DB          NDIS2,TP_TOR_16
           DB          NE2,TP_TOR_16

TORK2C     DB          NH2,TP_TOR_16
           DB          NA2,TP_TOR_16
           DB          NGIS2,TP_TOR_16
           DB          NA2,TP_TOR_16

TORK2D     DB          NH2,TP_TOR_16
           DB          NA2,TP_TOR_16
           DB          NGIS2,TP_TOR_16
           DB          NA2,TP_TOR_16

TORK3      DB          NC3,TP_TOR_4

TORK3B     DB          NA2,TP_TOR_8
           DB          NC3,TP_TOR_8

TORK3C     DB          NH2,TP_TOR_8
           DB          NA2,TP_TOR_8

TORK3D     DB          NG2,TP_TOR_8
           DB          NA2,TP_TOR_8

TORK4      DB          NH2,TP_TOR_8
           DB          NA2,TP_TOR_8
TORK4B     DB          NG2,TP_TOR_8
           DB          NA2,TP_TOR_8

TORK4C     DB          NH2,TP_TOR_8
           DB          NA2,TP_TOR_8
TORK4D     DB          NG2,TP_TOR_8
           DB          NFIS2,TP_TOR_8

TORK5       DB          NE2,TP_TOR_4

TORK6       DB          NE2,TP_TOR_8
            DB          NF2,TP_TOR_8

TORK7       DB          NG2,TP_TOR_8
            DB          NG2,TP_TOR_8
TORK7B      DB          NA2,TP_TOR_16
            DB          NG2,TP_TOR_16
            DB          NF2,TP_TOR_16
            DB          NE2,TP_TOR_16

TORK7C      DB          ND2,TP_TOR_4
TORK7D      DB          NE2,TP_TOR_8
            DB          NF2,TP_TOR_8

TORK8       DB          NG2,TP_TOR_8
            DB          NG2,TP_TOR_8
TORK8B      DB          NA2,TP_TOR_16
            DB          NG2,TP_TOR_16
            DB          NF2,TP_TOR_16
            DB          NE2,TP_TOR_16

TORK8C      DB          ND2,TP_TOR_4
TORK8D      DB          NC2,TP_TOR_8
            DB          ND2,TP_TOR_8

TORK9       DB          NE2,TP_TOR_8
            DB          NE2,TP_TOR_8
TORK9A      DB          NF2,TP_TOR_16
            DB          NE2,TP_TOR_16
            DB          ND2,TP_TOR_16
            DB          NC2,TP_TOR_16

TORK9B      DB          NH1,TP_TOR_4
TORK9C      DB          NC2,TP_TOR_8
            DB          ND2,TP_TOR_8

TORK10      DB          NE2,TP_TOR_8
            DB          NE2,TP_TOR_8
TORK10A     DB          NF2,TP_TOR_16
            DB          NE2,TP_TOR_16
            DB          ND2,TP_TOR_16
            DB          NC2,TP_TOR_16

TORK10B     DB          NH1,TP_TOR_4
TORK10C     DB          NH1,TP_TOR_16
            DB          NA1,TP_TOR_16
            DB          NGIS1,TP_TOR_16
            DB          NA1,TP_TOR_16

TORK11      DB          NC2,TP_TOR_8
            DB          NSZ,TP_TOR_8
TORK11A     DB          ND2,TP_TOR_16
            DB          NC2,TP_TOR_16
            DB          NH1,TP_TOR_16
            DB          NC2,TP_TOR_16

TORK11B     DB          NE2,TP_TOR_8
            DB          NSZ,TP_TOR_8
TORK11C     DB          NF2,TP_TOR_16
            DB          NE2,TP_TOR_16
            DB          NDIS2,TP_TOR_16
            DB          NE2,TP_TOR_16

TORK12      DB          NH2,TP_TOR_16
            DB          NA2,TP_TOR_16
            DB          NGIS2,TP_TOR_16
            DB          NA2,TP_TOR_16
TORK12B     DB          NH2,TP_TOR_16
            DB          NA2,TP_TOR_16
            DB          NGIS2,TP_TOR_16
            DB          NA2,TP_TOR_16

TORK12C     DB          NC3,TP_TOR_4
TORK12D     DB          NA2,TP_TOR_8
            DB          NH2,TP_TOR_8

TORK13      DB          NC3,TP_TOR_8
            DB          NH2,TP_TOR_8
TORK13B     DB          NA2,TP_TOR_8
            DB          NGIS2,TP_TOR_8

TORK13C     DB          NA2,TP_TOR_8
            DB          NE2,TP_TOR_8
TORK13D     DB          NF2,TP_TOR_8
            DB          ND2,TP_TOR_8

TORK14      DB          NC2,TP_TOR_4
TORK14B     DB          NH1,TP_TOR_8+TP_TOR_16
            DB          NA1,TP_TOR_32
            DB          NH1,TP_TOR_32
TORK14C     DB          NA1,TP_TOR_4


TOROK      DW        TORK1,10
           DW        TORK1B,12
           DW        TORK2,12
           DW        TORK2C,16
           DW        TORK3,6
           DW        TORK3C,8
           DW        TORK4,8
           DW        TORK4C,10  ;5.DIKET IS

           DW        TORK1,10
           DW        TORK1B,12
           DW        TORK2,12
           DW        TORK2C,16
           DW        TORK3,6
           DW        TORK3C,8
           DW        TORK4,8
           DW        TORK4C,8  ;
           DW        TORK5,6  ;5.,6DIKET IS

           DW        TORK7,12
           DW        TORK7C,6
           DW        TORK8,12
           DW        TORK8C,6
           DW        TORK9,12
           DW        TORK9B,6
           DW        TORK10,12
           DW        TORK10B,10
           DW        TORK11,12
           DW        TORK11B,12
           DW        TORK12,16
           DW        TORK12C,6
           DW        TORK13,8
           DW        TORK13C,8
           DW        TORK14,10
           DW        TORK6,16
           DW        TORK7C,6
           DW        TORK8,12
           DW        TORK8C,6
           DW        TORK9,12
           DW        TORK9B,6
           DW        TORK10,12
           DW        TORK10B,10
           DW        TORK11,12
           DW        TORK11B,12
           DW        TORK12,16
           DW        TORK12C,6
           DW        TORK13,8
           DW        TORK13C,8
           DW        TORK14,10
           RET

; GRANDFATHER'S CLOCK
GFA1
           DB           NSZ,TP_GRA_4
GFA1B      DB           NSZ,TP_GRA_8
           DB           NC2,TP_GRA_8

GFA2       DB           NF2,TP_GRA_4
GFA2B      DB           NE2,TP_GRA_8
           DB           NF2,TP_GRA_8
GFA2C      DB           NG2,TP_GRA_4
GFA2D      DB           NF2,TP_GRA_8
           DB           NG2,TP_GRA_8

GFA3       DB           NA2,TP_GRA_4
GFA3B      DB           NB2,TP_GRA_8
           DB           NA2,TP_GRA_8
GFA3C      DB           ND2,TP_GRA_4
GFA3D      DB           NG2,TP_GRA_8
           DB           NG2,TP_GRA_8

GFA4       DB           NF2,TP_GRA_4
GFA4B      DB           NF2,TP_GRA_8
           DB           NF2,TP_GRA_8
GFA4C      DB           NE2,TP_GRA_4
GFA4D      DB           ND2,TP_GRA_8
           DB           NE2,TP_GRA_8

GFA5       DB           NF2,TP_GRA_4
GFA5B      DB           NSZ,TP_GRA_4

GFA6       DB           NSZ,TP_GRA_4
GFA6B      DB           NC2,TP_GRA_8
           DB           NC2,TP_GRA_8

GFA7       DB           NSZ,TP_GRA_4
GFA7B      DB           NA2,TP_GRA_8
           DB           NB2,TP_GRA_8

GFA8       DB           NC3,TP_GRA_4
GFA8B      DB           NA2,TP_GRA_8
           DB           NG2,TP_GRA_8
GFA8C      DB           NF2,TP_GRA_4
GFA8D      DB           NE2,TP_GRA_8
           DB           NF2,TP_GRA_8

GFA9       DB           NG2,TP_GRA_8
           DB           NF2,TP_GRA_8
GFA9B      DB           NE2,TP_GRA_8
           DB           ND2,TP_GRA_8
GFA9C      DB           NC2,TP_GRA_4
GFA9D      DB           NA2,TP_GRA_8
           DB           NB2,TP_GRA_8

GFA10      DB           NC3,TP_GRA_4
GFA10B     DB           NA2,TP_GRA_8
           DB           NG2,TP_GRA_8
GFA10C     DB           NF2,TP_GRA_8
           DB           NF2,TP_GRA_8
GFA10D     DB           NE2,TP_GRA_8
           DB           NF2,TP_GRA_8

GFA11      DB           NG2,TP_GRA_4
GFA11B     DB           NSZ,TP_GRA_4

GFA12      DB           NF2,TP_GRA_4
GFA12B     DB           NSZ,TP_GRA_4
GFA12C     DB           NG2,TP_GRA_4
GFA12D     DB           NSZ,TP_GRA_4

GFA13      DB           NA2,TP_GRA_8
           DB           NA2,TP_GRA_8
GFA13B     DB           NB2,TP_GRA_8
           DB           NA2,TP_GRA_8
GFA13C     DB           ND2,TP_GRA_4
GFA13D     DB           NG2,TP_GRA_8
           DB           NG2,TP_GRA_8

GFA14      DB           NF2,TP_GRA_2
GFA14B     DB           NE2,TP_GRA_2

GFA15      DB           NF2,TP_GRA_4
GFA15B     DB           NC2,TP_GRA_8
           DB           NC2,TP_GRA_8
GFA15C     DB           ND2,TP_GRA_4
GFA15D     DB           NC2,TP_GRA_8
           DB           NC2,TP_GRA_8

GFA16      DB           NA1,TP_GRA_8
           DB           NC2,TP_GRA_8
GFA16B     DB           NC2,TP_GRA_4
GFA16C     DB           NSZ,TP_GRA_4
GFA16D     DB           NSZ,TP_GRA_8
           DB           NC2,TP_GRA_8

GFA17      DB           NA1,TP_GRA_8
           DB           NC2,TP_GRA_8
GFA17B     DB           NC2,TP_GRA_4


GRANDFA    DW         GFA1,2
           DW         GFA1B,4
           DW         GFA2,2
           DW         GFA2B,4
           DW         GFA2C,2
           DW         GFA2D,4
           DW         GFA3,2
           DW         GFA3B,4
           DW         GFA3C,2
           DW         GFA3D,4
           DW         GFA4,2
           DW         GFA4B,4
           DW         GFA4C,2
           DW         GFA4D,4
           DW         GFA5,2
           DW         GFA5B,2
           DW         GFA6,2
           DW         GFA6B,4
           DW         GFA2,2
           DW         GFA2B,4
           DW         GFA2C,2
           DW         GFA2D,4
           DW         GFA3,2
           DW         GFA3B,4
           DW         GFA3C,2
           DW         GFA3D,4
           DW         GFA4,2
           DW         GFA4B,4
           DW         GFA4C,2
           DW         GFA4D,4
           DW         GFA5,2
           DW         GFA5B,2
           DW         GFA7,2
           DW         GFA7B,4
           DW         GFA8,2
           DW         GFA8B,4
           DW         GFA8C,2
           DW         GFA8D,4
           DW         GFA9,4
           DW         GFA9B,4
           DW         GFA9C,2
           DW         GFA9D,4
           DW         GFA10,2
           DW         GFA10B,4
           DW         GFA10C,4
           DW         GFA10D,4
           DW         GFA11,2
           DW         GFA11B,2
           DW         GFA6,2
           DW         GFA6B,4
           DW         GFA12,2
           DW         GFA12B,2
           DW         GFA12C,2
           DW         GFA12D,2
           DW         GFA13,4
           DW         GFA13B,4
           DW         GFA13C,2
           DW         GFA13D,4
           DW         GFA14,2
           DW         GFA14B,2
           DW         GFA5,2
           DW         GFA5B,2
           DW         GFA6,2
           DW         GFA6B,4
           DW         GFA15,2
           DW         GFA15B,4
           DW         GFA15C,2
           DW         GFA15D,4
           DW         GFA16,4
           DW         GFA16B,2
           DW         GFA16C,2
           DW         GFA16D,4
           DW         GFA15,2
           DW         GFA15B,4
           DW         GFA15C,2
           DW         GFA15D,4
           DW         GFA17,4
           DW         GFA17B,2
           DW         GFA6,2
           DW         GFA6B,4
           DW         GFA12,2
           DW         GFA12B,2
           DW         GFA12C,2
           DW         GFA12D,2
           DW         GFA13,4
           DW         GFA13B,4
           DW         GFA13C,2
           DW         GFA13D,4
           DW         GFA14,2
           DW         GFA14B,2
           DW         GFA5,2
           DW         GFA5B,2
           RET

; OLD BOYS
OBO1       DB           NSZ,TP_OLD_8
           DB           NG2,TP_OLD_8
OBO1B      DB           NA2,TP_OLD_8
           DB           NG2,TP_OLD_8

OBO2       DB           NC3,TP_OLD_4
OBO2B      DB           NC3,TP_OLD_4
OBO2C      DB           NA2,TP_OLD_8
           DB           NG2,TP_OLD_8
OBO2D      DB           NE2,TP_OLD_4

OBO3       DB           NG2,TP_OLD_4
OBO3B      DB           NG2,TP_OLD_8
           DB           NA2,TP_OLD_8
OBO3C      DB           NG2,TP_OLD_8
           DB           NE2,TP_OLD_8
OBO3D      DB           ND2,TP_OLD_8
           DB           NC2,TP_OLD_8

OBO4       DB           NC2,TP_OLD_4
OBO4B      DB           NC2,TP_OLD_8
           DB           ND2,TP_OLD_8
OBO4C      DB           NE2,TP_OLD_8
           DB           NG2,TP_OLD_8
OBO4D      DB           NA2,TP_OLD_8
           DB           NG2,TP_OLD_8

OBO5       DB           NC3,TP_OLD_4
OBO5B      DB           NSZ,TP_OLD_4

OBO6       DB           NG2,TP_OLD_4
OBO6B      DB           NG2,TP_OLD_8
           DB           NA2,TP_OLD_8
OBO6C      DB           NH2,TP_OLD_8
           DB           ND3,TP_OLD_8
OBO6D      DB           NH2,TP_OLD_8
           DB           NA2,TP_OLD_8

OBO7       DB           NG2,TP_OLD_4
OBO7B      DB           NSZ,TP_OLD_4

OBO8       DB           NC3,TP_OLD_4
OBO8B      DB           NC3,TP_OLD_4
OBO8C      DB           NC3,TP_OLD_4
OBO8D      DB           NC3,TP_OLD_4

OBO9       DB           NC3,TP_OLD_4
OBO9B      DB           NC3,TP_OLD_4
OBO9C      DB           NA2,TP_OLD_8
           DB           NG2,TP_OLD_8
OBO9D      DB           NE2,TP_OLD_8
           DB           NC2,TP_OLD_8

OBO10      DB           NF2,TP_OLD_4
OBO10B     DB           NF2,TP_OLD_4
OBO10C     DB           NF2,TP_OLD_4
OBO10D     DB           NF2,TP_OLD_4

OBO11      DB           NA2,TP_OLD_4
OBO11B     DB           NC3,TP_OLD_4
OBO11C     DB           ND3,TP_OLD_8
           DB           NC3,TP_OLD_8
OBO11D     DB           ND3,TP_OLD_8
           DB           NC3,TP_OLD_8

OBO12      DB           NE3,TP_OLD_8
           DB           NES3,TP_OLD_8
OBO12B     DB           NE3,TP_OLD_8
           DB           NES3,TP_OLD_8
OBO12C     DB           NE3,TP_OLD_8
OBO12D     DB           NG3,TP_OLD_4+TP_OLD_8

OBO13      DB           NE3,TP_OLD_4
OBO13B     DB           NE3,TP_OLD_8
           DB           NG3,TP_OLD_8
OBO13C     DB           NE3,TP_OLD_8
           DB           ND3,TP_OLD_8
OBO13D     DB           NC3,TP_OLD_8
           DB           NA2,TP_OLD_8

OBO14      DB           NC3,TP_OLD_4
OBO14B     DB           NC3,TP_OLD_4
OBO14C     DB           NA2,TP_OLD_8
           DB           NG2,TP_OLD_8
OBO14D     DB           NE2,TP_OLD_8
           DB           ND2,TP_OLD_8

OBO15      DB           NC2,TP_OLD_4
OBO15B     DB           NSZ,TP_OLD_4

OLDBOYS     DW       OBO1,4
            DW       OBO1B,4
            DW       OBO2,2
            DW       OBO2B,2
            DW       OBO2C,4
            DW       OBO2D,2
            DW       OBO3,2
            DW       OBO3B,4
            DW       OBO3C,4
            DW       OBO3D,4
            DW       OBO4,2
            DW       OBO4B,4
            DW       OBO4C,4
            DW       OBO4D,4
            DW       OBO5,2
            DW       OBO5B,2
            DW       OBO1,4
            DW       OBO1B,4
            DW       OBO2,2
            DW       OBO2B,2
            DW       OBO2C,4
            DW       OBO2D,2
            DW       OBO3,2
            DW       OBO3B,4
            DW       OBO3C,4
            DW       OBO3D,4
            DW       OBO6,2
            DW       OBO6B,4
            DW       OBO6C,4
            DW       OBO6D,4
            DW       OBO7,2
            DW       OBO7B,2
            DW       OBO1,4
            DW       OBO1B,4
            DW        OBO8,2
            DW        OBO8B,2
            DW        OBO8C,2
            DW        OBO8D,2
            DW        OBO9,2
            DW        OBO9B,2
            DW        OBO9C,4
            DW        OBO9D,4
            DW        OBO10,2
            DW        OBO10B,2
            DW        OBO10C,2
            DW        OBO10D,2
            DW        OBO11,2
            DW        OBO11B,2
            DW        OBO11C,4
            DW        OBO11D,4
            DW        OBO12,4
            DW        OBO12B,4
            DW        OBO12C,2
            DW        OBO12D,2
            DW        OBO13,2
            DW        OBO13B,4
            DW        OBO13C,4
            DW        OBO13D,4
            DW        OBO14,2
            DW        OBO14B,2
            DW        OBO14C,4
            DW        OBO14D,4
            DW        OBO15,2
            DW        OBO15B,2
            RET

;WHAT SHALL WE DO
      DB 'PAT1'
PAT1       DB           NA2,TP_TEN_8
           DB           NA2,TP_TEN_16
           DB           NA2,TP_TEN_16


PAT2       DB           NG2,TP_TEN_8
           DB           NG2,TP_TEN_16
           DB           NG2,TP_TEN_16

PAT3       DB           NA2,TP_TEN_8
           DB           ND2,TP_TEN_8
PAT3B      DB           NF2,TP_TEN_8
           DB           NA2,TP_TEN_8

PAT4       DB           NG2,TP_TEN_8
           DB           NC2,TP_TEN_8
PAT4B      DB           NE2,TP_TEN_8
           DB           NG2,TP_TEN_8

PAT5       DB           NA2,TP_TEN_8
           DB           NH2,TP_TEN_8
PAT5B      DB           NC3,TP_TEN_8
           DB           ND3,TP_TEN_8

PAT6       DB           NC3,TP_TEN_8
           DB           NA2,TP_TEN_8
PAT6B      DB           NG2,TP_TEN_8
           DB           NE2,TP_TEN_8

PAT7       DB           NG2,TP_TEN_4
PAT7B      DB           NG2,TP_TEN_10
           DB           NG2,TP_TEN_16

PAT8       DB           NA2,TP_TEN_4
PAT8B      DB           NA2,TP_TEN_10
           DB           NA2,TP_TEN_16

PAT9       DB           ND2,TP_TEN_4
PAT9B      DB           ND2,TP_TEN_4


; WHAT SHALL WE DO WITH THE DRUNKEN SAILOR ?
; >TENG
      DB   'TENG'
TENG       DW      PAT1,6
           DW      PAT1,6
           DW      PAT3,4
           DW      PAT3B,4
           DW      PAT2,6
           DW      PAT2,6
           DW      PAT4,4
           DW      PAT4B,4
           DW      PAT1,6
           DW      PAT1,6
           DW      PAT5,4
           DW      PAT5B,4
           DW      PAT6,4
           DW      PAT6B,4
           DW      PAT9,2
           DW      PAT9B,2
           DW      PAT8,2
           DW      PAT8B,4
           DW      PAT3,4
           DW      PAT3B,4
           DW      PAT7,2
           DW      PAT7B,4
           DW      PAT4,4
           DW      PAT4B,4
           DW      PAT8,2
           DW      PAT8B,4
           DW      PAT5,4
           DW      PAT5B,4
           DW      PAT6,4
           DW      PAT6B,4
           DW      PAT9,2
           DW      PAT9B,2
           RET

; ALTALANOS LEJATSZO MODUL
; BEJOVO ADATOK: R1 - DPH
;                R2 - DPL
;                SNUM - HANGOK*2
; >PLAY
PLAY       MOV   R1,DPH
           MOV   R2,DPL
           LCALL  GYOZ_FENY
           PUSH  CREG
           MOV   JNUM,#0        ; BEOLVASASI ELTOLAS MUTATO
PLA1       MOV   DPH,R1
           MOV   DPL,R2         ; DPTR-BE A HANGJEGY CIM
           MOV   A,JNUM
           MOVC  A,@A+DPTR
           MOV   AREG,A         ; AREG-BE A HANGMAGASSAG
           MOV   A,JNUM
           ADD   A,#1
           MOV   JNUM,A         ; JNUM NOVELESE
           MOV   DPH,R1
           MOV   DPL,R2
           MOV   A,JNUM
           MOVC  A,@A+DPTR
           MOV   B,A           ; B-BE A HANG HOSSZA
PLA2       MOV   A,AREG
           CJNE  A,#2,UTE1    ; NEM SZUNET, UGRAS
           MOV   CREG,#5
UTE3       MOV   A,B
UTE2       LCALL TIM1MS
           LCALL COIN           ; SZUNETBEN IS NEZZUK A P�NZT
           DJNZ  A,UTE2
           DJNZ  CREG,UTE3
           SJMP  UTE4
UTE1       LCALL BEEP
UTE4    ;  LCALL COIN
           LCALL SKIPKEY        ; MEGNEZZUK, NEM KULDTEK-E EL A ZENET ?
           JZ    UTE6
           SETB  TILT
           POP   CREG           ; KILEPES
           RET                  ; VISSZATERES EGYBOL A GYOZTES RUTINBA
UTE6       MOV   A,JNUM
           ADD   A,#1
           MOV   JNUM,A
           CJNE  A,SNUM,PLA3
           LCALL WAIT_COIN_10
           POP   CREG
           RET
PLA3       LCALL  GYOZ_FENY
           LCALL  WAIT_COIN_35
           LJMP  PLA1

PAT        EQU     40H             ; BIT KIMASZKOLAS

; >BEEP
; R�OLVAS AZ �RMEVIZSG�L� PORTRA
; bej�v� adatok: AREG - FREKVENCIA
;                BREG - HOSSZ

BEEP            CLR     EA              ; 1u  - IT LETILTAS ;
                PUSH    B
                PUSH    DPH
                PUSH    DPL
                PUSH    BREG            ; 2u
                PUSH    PSW             ; 2u
                PUSH    AREG            ; 2u
                PUSH    A               ; 2u
                MOV     PSW,#rbank3     ; 2u
                MOV     A,AREG          ; 1u
                RR      A               ; 1u   FREKI OSZTVA 2-VEL

                MOV     AREG,A          ; 1u
                MOV     R1,AREG         ; 1u  R1-BE A FREKI
                MOV     R2,B            ; 1u  DURATION
                MOV     R3,#0           ; 2u
                MOV     BREG,#PAT       ; 2u
MAIN            MOV     A,BREG          ; 1u  A-BA A KAPCSOLOVEZ.
                XRL     P1,A            ; 1u  KIMENO BIT INVERTALAS
           ;    NOP                     ; 1u
           ;    NOP
           ;    NOP
           ;    NOP
           ;    NOP
           ;    NOP
           ;    NOP
           ;    NOP
                NOP
		MOV	DPTR,#COINPORT  ; #2,
		MOVX	A,@DPTR         ; #2,
                CPL     A               ; #1, INVERTALAS
                ANL     A,#COIN_MASK    ; #2, EGYEB JELEK TORLESE
                JZ      MMA1            ; #2, NEM OLVASOTT, UGRAS
                PUSH    AREG
                PUSH    A
                MOV     A,R1
                PUSH    A
                MOV     A,R2
                PUSH    A
                MOV     A,R3
                PUSH    A
                LCALL   COIN
                POP     A
                MOV     R3,A
                POP     A
                MOV     R2,A
                POP     A
                MOV     R1,A
                POP     A
                POP     AREG
MMA1            DJNZ    R1,NOROLL1      ; 2u HA MEG NEM TELT LE A FREKI,UGRAS
              ; INC     AREG
                MOV     R1,AREG         ; 1u  FREKI UJRATOLTES
                MOV     BREG,#PAT       ; 2u  MASZKOLAS BETOLTES
                NOP                     ; 1u
DURLO           DJNZ    R3,MAIN         ; 2u
              ; INC     AREG
             ;  PUSH    AREG
             ;  POP     AREG
                DJNZ    R2,MAIN         ; 2u
                POP     A               ; 2u
                POP     AREG
                POP     PSW
                POP     BREG
                POP     DPL
                POP     DPH
                POP     B
             ;  CLR     P16
                SETB    EA
                RET

NOROLL1         MOV     BREG,#0         ; MASZKOLAS TORLES
                SJMP    DURLO

; >BEEP2
BEEP2          CLR     EA              ; IT LETILTAS
               PUSH    BREG
               PUSH    PSW
               PUSH    AREG
               PUSH    A
                MOV     PSW,#rbank3
                MOV     A,AREG
                RR      A               ; FREKI OSZTVA 2-VEL
                MOV     AREG,A
                MOV     R1,AREG         ; R1-BE A FREKI
                MOV     R2,B            ; DURATION
                MOV     R3,#0
                MOV     BREG,#PAT
MAIN2           MOV     A,BREG          ; A-BA A KAPCSOLOVEZ.
                XRL     P1,A            ; KIMENO BIT INVERTALAS
                NOP                     ; 1u
                NOP
                NOP
                NOP
                NOP
       ;        NOP
                NOP
                NOP
                NOP
                NOP
          ;     NOP
                DJNZ    R1,NOROLL2      ; HA MEG NEM TELT LE A FREKI, UGRAS
              ; INC     AREG
                MOV     R1,AREG         ; FREKI UJRATOLTES
                MOV     BREG,#PAT       ; MASZKOLAS BETOLTES
                NOP
DURLO2          DJNZ    R3,MAIN2
                INC     AREG
                PUSH    PSW
                PUSH    A
                PUSH    AREG
                LCALL   COIN
                POP     AREG
                POP     A
                POP     PSW
                DJNZ    R2,MAIN2
                POP     A
                POP     AREG
                POP     PSW
                POP     BREG
                SETB    EA
                RET

NOROLL2         MOV     BREG,#0         ; MASZKOLAS TORLES
                SJMP    DURLO2

; >BASS
;BASS            CLR     EA              ; IT LETILTAS
;                PUSH    PSW
;           ;    PUSH    DREG
;                MOV     PSW,#rbank3
;            ;    MOV     A,AREG
;             ;   RRC     A               ; FREKI OSZTVA 2-VEL
;             ;  MOV     AREG,A
;                MOV     R1,AREG         ; R1-BE A FREKI
;                MOV     R2,B            ; DURATION
;                MOV     R3,#0
;                MOV     BREG,#PAT
;BMAIN            MOV     A,BREG          ; A-BA A KAPCSOLOVEZ.
;                XRL     P1,A            ; KIMENO BIT INVERTALAS
;               NOP
;               NOP
;               NOP
;               NOP
;               NOP
;               NOP
;               NOP
;               NOP
;               NOP
;               NOP
;                DJNZ    R1,BNOROLL1      ; HA MEG NEM TELT LE A FREKI, UGRAS
;              ;  INC     AREG
;                MOV     R1,AREG         ; FREKI UJRATOLTES
;                MOV     BREG,#PAT       ; MASZKOLAS BETOLTES
;                NOP
;BDURLO          DJNZ    R3,BMAIN
;              ;  INC     AREG
;                DJNZ    R2,BMAIN
;                POP     PSW
;                SETB    EA
;                RET

;BNOROLL1        MOV     BREG,#0         ; MASZKOLAS TORLES
;                SJMP    BDURLO


;NOISE           MOV     DPTR,#ELEJE
;                MOV     DREG,#50
;NOI2            MOVC    A,@A+DPTR
;                MOV     B,#8            ; NYOLC BITET KULDUNK KI
;NOI1            RLC     A
;                MOV     P10,C
;                LCALL   TIM100US
;                DJNZ    B,NOI1
;                INC     DPTR
;                DJNZ    DREG,NOI2
;                RET

;>BEEP3
BEEP3           CLR     EA              ; 1u  - IT LETILTAS ;
                PUSH    BREG            ; 2u
                PUSH    PSW             ; 2u
                PUSH    AREG            ; 2u
                PUSH    A               ; 2u
                MOV     PSW,#rbank3     ; 2u
                MOV     A,AREG          ; 1u
                RR      A               ; 1u   FREKI OSZTVA 2-VEL

                MOV     AREG,A          ; 1u
                MOV     R1,AREG         ; 1u  R1-BE A FREKI
                MOV     R2,B            ; 1u  DURATION
                MOV     R3,#0           ; 2u
                MOV     BREG,#PAT       ; 2u
MAIN3           MOV     A,BREG          ; 1u  A-BA A KAPCSOLOVEZ.
                XRL     P1,A            ; 1u  KIMENO BIT INVERTALAS
                NOP                     ; 1u
                NOP
        ;       NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
          ;     NOP
                DJNZ    R1,NOROLL3      ; 2u HA MEG NEM TELT LE A FREKI,UGRAS
              ; INC     AREG
                MOV     R1,AREG         ; 1u  FREKI UJRATOLTES
                MOV     BREG,#PAT       ; 2u  MASZKOLAS BETOLTES
                NOP                     ; 1u
DURLO3          DJNZ    R3,MAIN3         ; 2u
              ; INC     AREG
                DJNZ    R2,MAIN3         ; 2u
                POP     A               ; 2u
                POP     AREG
                POP     PSW
                POP     BREG
             ;  CLR     P16
                SETB    EA
                RET

NOROLL3         MOV     BREG,#0         ; MASZKOLAS TORLES
                SJMP    DURLO3


JELZES       DB   'ITT A VEGE'
