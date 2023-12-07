;****************************************************************
;       Euro-Dart (C) Ringer Attila (C)1995-2023								*
;****************************************************************

; DART GEP SETUP RUTINJAI


; A JATEKOK EGYMAS UTANI SORRENDJET MUTATJA
;JATKAPCS10  LONG  0, 0, 0, 0, 0, 0, 0, 0
; - MUTATO        0, 1,  2,  3,  4,  5,  6, 7,  8,  9, 10, 11, 12, 13, 14
;JATSOR      DB    0, 2,  7,  4,  5,  6, 10, 8,  3,  0, 17,  0, 14, 0,   1
; -MUTATO        15, 16, 17, 18, 19, 20, 21, 22, 23
;            DB    0,  0, 22,  0,  0,  0,  0, 12,  0

;JATKAPCS10  LONG  0, 0, 0, 0, 0, 0, 0, 0
; - MUTATO        0, 1,  2,  3,  4,  5,  6, 7,  8,  9, 10, 11, 12, 13, 14
JATSOR      DB    0, 2, 18,  4,  5,  6, 16, 8, 19, 11, 20, 15, 14,  9, 10
; -MUTATO        15, 16, 17, 18, 19, 20, 21, 22, 23
            DB    1, 23, 22,  7,  3, 13, 12, 21, 17


; HARDWARE SETUP RUTIN
; >SETUP
SETUP       SETB    EA
            MOV    DPTR,#MEG_ORZ
            MOVX   A,@DPTR          ; MEGNEZI, KELL-E FLAG
            CJNE   A,#2,STP0A       ; NEM KELL, UGRAS

            MOV     DPTR,#ST_FLAG
            MOV     A,#ST_SETUP     ; MEGJEGYZI, HOGY SETUP-BAN VAN
            MOVX    @DPTR,A

STP0A       CLR     P_TEST
            CLR     P_JATEK
            PUSH    MINDKI
            CLR     DOBJON
            LCALL   BUFINI          ; KIJELZO TORLES
            MOV     A,#M_SET        ; SET FELIRAT
            PLAYER  0,1             ; BAL FELSO KIJELZO
            LCALL   TSTCIM          ; MEGJELENITES
            LCALL   DARTKIJ
            MOV     A,#H_VALTO         ; HIBAJELZO HANG
            LCALL   H_EFFEKT
STP1A    ;  LCALL   CURSETKEY
         ;  JNZ     STP1A           ; MEGVARJA, MIG ELENGEDIK
            LCALL   WAIT_NOKEY      ; MEGVARJA MIG ELENGEDIK A GOMBOT
            PUSH    BREG
            MOV     BREG,#1     ; KEZDO LEPES
STP1    ;   LCALL   SETLEDBE    ; SETUP LED KIKAPCS.
            SETB    TILT
            BELAM   IZVALTO
            BELAM   IZMAS
            BELAM   IZDOUT
            MOV     JNUM,#9
            MOV     NUM,BREG
            MOV     SNUM,#0AH   ; SZAZASOK HELYEN NE LEGYEN SEMMI
            LCALL   PONTSZAM    ; KIIRATAS A KIJELZORE
            PUSH    JNUM
            PUSH    SEG
            MOV     A,BREG
            PLAYER  2,4
            LCALL   SETUPJAT
            POP     SEG
            POP     JNUM
            KEYCLR
            LCALL   READSETKEY
            MOV     A,KEY
            CJNE    A,#CHANGE,STP2
            LCALL   HWRUTIN        ; MEGHIVJA A BREG-HEZ TARTOZO RUTINT
            JB      P_JATEK,STP4   ; HA JATEK FAZIS KELL, KILEP
            JB      P_TEST,STP4    ; HA TESZT FAZIS KELL, KILEP
            SJMP    STP1           ; EGYEBKENT VISSZA AZ ELEJERE
STP2        CJNE    A,#UP,STP3
            LCALL   NEXTSET
            SJMP    STP1
STP3        LCALL   PREVSET
            SJMP    STP1
STP4        POP     BREG
            POP     MINDKI
            CLR     P_SETUP             ; SETUP UZEMMOD VEGE
            RET

;*************************
; SETUP NYOMOGOMBOK BEOLVASASA
; >SETKAPBE
; SETKAPBE	MOV     A,#0
;                RET

; >CURSETKEY
;CURSETKEY	MOV   A,#0
;                RET

; KAPCSOLOSOR OLVASO RUTIN
; A BEOLVASOTT KAPCSOLO SZAMA LESZ ESEMENYKENT ELTAROLVA.
; A RUTIN CSAK AKKOR TER VISSZA, HA A NYOMOTT GOMBOT ELENGEDJUK
; BEJOVO ADAT: R5-BEN A MASZK
; >SETKEYBE

; M_DOWN          EQU  0FFH
; M_UP            EQU  0FFH
; M_CHANGE        EQU  0FFH
; M_CHGAME        EQU  0FFH

SETKEYBE        PUSH    CREG
		MOV	A,KEY		; NEM IS OLVAS, HA NINCS KEZELVE
                JNZ	SKY6		;               AZ ELOZO ESEMENY
		BUTTONLSB		; LSB KAPCSOLOK OLVASASA
		JZ	SKY4		; HA NINCS NYOMVA, UGRIK AZ MSB-RE
		LCALL	BITNUM		; KISZAMITJUK, HANYADIK NYOMOGOMB
		INC	A
		MOV	KEY,A		; ELTAROLJUK AZ ESEMENYT
SKY6		LJMP	SKYVEGE		; UGRAS A VEGERE

SKY4		BUTTONMSB    		; MSB CSOPORT OLVASASA
		JZ	SKYVEGE
		LCALL	BITNUM
		ADD	A,#9		; HOZZAADUNK 8+1-ET
		MOV	KEY,A		; TAROLJUK
SKYVEGE     ;   SETB	EA
                LCALL   TIM30MS
                POP     CREG
		RET

CHANGE          EQU     2
UP              EQU     4
DOWN            EQU     1
CHGAME          EQU     5

; SETUP ALATT KEZELI A BILLENTYUZETET - EGYIDEJULEG A BELSOT ES A KULSOT
; MEGHIVASKOR ADDIG A RUTINBAN MARAD, AMIG MEG NEM NYOMUNK EGY BILLENTYUT
; >READSETKEY
READSETKEY    MOV       A,KEY
              JZ        RDST2
              RET                       ; HA KEY<>NULLA, KILEP
RDST2      ;   LCALL     SETKAPBE        ; SETUP KAPCSOLOK BEOLVASASA
           ;   MOV       A,KEY
           ;   JNZ       RDST1           ; HA VOLT VALAMI, UGRAS
              LCALL     SETKEYBE
              MOV       A,KEY
              JZ        RDST2           ; HA NEM VOLT, VISSZA AZ ELEJERE
              CJNE      A,#LEPTETO,RDST3
              MOV       KEY,#CHANGE
              LCALL     H_KEY
              RET
RDST3         CJNE      A,#IZGOMB16,RDST4
              MOV       KEY,#UP
              LCALL     H_KEY
              RET
RDST4         CJNE      A,#IZGOMB1,RDST5
              MOV       KEY,#DOWN
              LCALL     H_KEY
              RET
RDST5         JB        TILT,RDST9      ; HA TILT=1, UGRAS
              CJNE      A,#IZGOMB2,RDST9
              MOV       KEY,#CHGAME     ; JATEKOK KOZOTT VALTO GOMB
              LCALL     H_KEY
              RET
RDST9         KEYCLR          ; EGYEB BILLENTYU, TOROLJUK
              SJMP      RDST2           ; VISSZA A CIKLUS ELEJERE
RDST1         CJNE      A,#DOWN,RDST6
              MOV       KEY,#DOWN
              LCALL     H_KEY
              RET
RDST6         CJNE      A,#CHANGE,RDST7
              MOV       KEY,#CHANGE
              LCALL     H_KEY
              RET
RDST7         CJNE      A,#UP,RDST8
              MOV       KEY,#UP
              LCALL     H_KEY
              RET
RDST8         KEYCLR         ; EGYEB BILLENTYU NEM ERDEKES
              SJMP      RDST2         ; VISSZA AZ ELEJERE


;>RD_SETKEY

M_CHANGE        EQU     40H
M_UP            EQU     80H
M_DOWN          EQU     01H
M_CHGAME        EQU     02H


RD_CHANGE	BUTTONMSB    		; MSB CSOPORT OLVASASA
                ANL     A,#M_CHANGE     ; CSAK A VALTORA VAGYUNK KIVANCSIAK
		JZ	RD_CKVEG
		MOV	KEY,#CHANGE	; TAROLJUK
                LCALL   H_KEY
RD_CK1          BUTTONMSB
                JNZ      RD_CK1         ; MEGVARJUK, MIG ELENGEDI
RD_CKVEG       	LCALL    TIM1MS
                RET

RD_UP		BUTTONMSB    		; MSB CSOPORT OLVASASA
                ANL     A,#M_UP         ; CSAK AZ UP KELL
		JZ	RD_UKVEG
		MOV	KEY,#UP	        ; TAROLJUK
                LCALL   H_KEY
RD_UK1          BUTTONMSB
                JNZ      RD_UK1         ; MEGVARJUK, MIG ELENGEDI
RD_UKVEG       	LCALL    TIM1MS
                RET


RD_DOWN		BUTTONLSB    		; MSB CSOPORT OLVASASA
                ANL     A,#M_DOWN       ; CSAK AZ UP KELL
		JZ	RD_DKVEG
		MOV	KEY,#DOWN	        ; TAROLJUK
                LCALL   H_KEY
RD_DK1          BUTTONLSB
                JNZ      RD_DK1         ; MEGVARJUK, MIG ELENGEDI
RD_DKVEG        LCALL    TIM1MS
                RET


RD_CHGAME	BUTTONLSB    		; MSB CSOPORT OLVASASA
                ANL     A,#M_CHGAME     ; CSAK AZ UP KELL
		JZ	RD_GKVEG
		MOV	KEY,#CHGAME	        ; TAROLJUK
                LCALL   H_KEY
RD_GK1          BUTTONLSB
                JNZ      RD_GK1         ; MEGVARJUK, MIG ELENGEDI
RD_GKVEG        LCALL    TIM1MS
                RET

EN_CH           EQU      01H
EN_UP           EQU      02H
EN_DN           EQU      04H
EN_CG           EQU      08H

RD_SETKEYBE     MOV      A,KEY
                JZ       RD_SK_VEG      ; HA NINCS A KEY KEZELVE, KILEP
                MOV      A,R3           ; A-BA AZ ENGEDELYEZES
                RRC      A              ; ALSO BIT A CARRY-BE
                MOV      R3,A
                JNC      RDSK1          ; NEM KELL A CHANGE
                LCALL    RD_CHANGE
                MOV      A,KEY          ; MEGNEZZUK, VAN-E
                JNZ      RD_SK_VEG
RDSK1           MOV      A,R3           ; A-BA AZ ENGEDELYEZES
                RRC      A              ; ALSO BIT A CARRY-BE
                MOV      R3,A
                JNC      RDSK2          ; NEM KELL AZ UP GOMB
                LCALL    RD_UP
                MOV      A,KEY          ; MEGNEZZUK, VAN-E
                JNZ      RD_SK_VEG
RDSK2           MOV      A,R3           ; A-BA AZ ENGEDELYEZES
                RRC      A              ; ALSO BIT A CARRY-BE
                MOV      R3,A
                JNC      RDSK3          ; NEM KELL A DOWN GOMB
                LCALL    RD_DOWN
                MOV      A,KEY          ; MEGNEZZUK, VAN-E
                JNZ      RD_SK_VEG
RDSK3           MOV      A,R3           ; A-BA AZ ENGEDELYEZES
                RRC      A              ; ALSO BIT A CARRY-BE
                MOV      R3,A
                JNC      RD_SK_VEG       ; NEM KELL A CHGAME
                LCALL    RD_CHGAME
                MOV      A,KEY          ; MEGNEZZUK, VAN-E
RD_SK_VEG       RET



; A KOVETKEZO SETUP LEPESRE VALT
; BEJOVO  ADAT: BREG - JELENLEGI SETUP LEPES
NEXTSET    MOV      A,BREG             ; ATTESSZUK A-BA
           CJNE     A,#HWSTEPS,KVT1    ; HA NEM A MAX, UGRIK
           MOV      BREG,#1            ; KORBEERT, ELOLROL
           RET
KVT1       CLR      C                  ; CARRY TORLES
           ADD      A,#1               ; NOVELES
           DA       A                  ; BCD KORREKCIO
           MOV      BREG,A
           RET

; AZ ELOZO SETUP LEPESRE VALT
; BEJOVO  ADAT: BREG - JELENLEGI SETUP LEPES
PREVSET    MOV      A,BREG             ; ATTESSZUK A-BA
           CJNE     A,#1,KVT2          ; HA NEM A MAX, UGRIK
           MOV      BREG,#HWSTEPS      ; KORBEERT, ELOLROL
           RET
KVT2       CJNE     A,#30H,KTV3A
           MOV      BREG,#29H
           RET
KTV3A      CJNE     A,#20H,KVT3
           MOV      BREG,#19H
           RET
KVT3       CJNE     A,#10H,KVT4
           MOV      BREG,#9H
           RET
KVT4       DEC      BREG                 ; CSOKKENTES
           RET

HWSTEPS     EQU     32H                  ; ENNYI LEPESBOL ALL A SETUP

HWSETUP     LJMP    HW_FACTSET           ; 01-GYARI BEALLITAS BETOLTESE
            NOP
            LJMP    HW_SERNUM            ; 02-GYARI SZAM KIJELZESE
            NOP
            LJMP    HW_UZEMSZAM          ; 03-UZEMELTETOI SZAM BEIRASA
            NOP
            LJMP    HW_PASSWORD          ; 04-BIZ. IDO KIKAPCS UTAN JELSZO
            NOP
            LJMP    HW_INFRASET          ; 05-INFRA ERZEKELO UZEMMOD
            NOP
            LJMP    HW_VALTKES           ; 06-VALTO ELJARAS KESLELTETESE
            NOP
            LJMP    HW_SZUNSZIR          ; 07-JATEKSZUNETBEN DOBASKOR SZIRENA
            NOP
            LJMP    HW_SONGSEL           ; 08-GYOZELMI ZENE VALASZTAS
            NOP
            LJMP    HW_SPEAK             ; 09-BESZEDHANG ENGEDELYEZES
            NOP
            LJMP    HW_CHAMPION         ; 10- VERSENYBEALLITASOK
            NOP
            LJMP    HW_COUNTERS        ;  11- ERMEVIZSGALO ALLASOK
            NOP
            LJMP    HW_COUNTKEY         ; 12- KULCSOS KAPCSOLO KREDIT
            NOP
            LJMP    HW_COUNTGAME        ; 13- JATEK SZAMLALO
            NOP
            LJMP    HW_COUNTFREE        ; 14-SZABADJATEK SZAMLALO
            NOP
            LJMP    HW_SZAMLNULL         ; 15-SZAMLALOK NULLAZASA
            NOP
            LJMP    HW_ERMKREDIT         ; 16-ERMESZAM KREDITENKENT
            NOP
            LJMP    HW_KREDORZ           ; 17-KREDIT MEGORZES KIKAPCSOLAS UTAN
            NOP
            LJMP    HW_SORSJAT          ; 18-SORSOLAS MINDEN JATEK UTAN
            NOP
            LJMP    HW_AJANKREDIT       ; 19-X JATEK UTAN AJANDEK KREDIT
            NOP
            LJMP    HW_UZEMORA          ; 20-UZEMORA SZAMLALO
            NOP
            LJMP    HW_STAT             ; 21-STATISZTIKA MEGJELENITES
            NOP
            LJMP    HW_JATNEPSZ        ; 22-JATEKOK NEPSZERUSEGI LISTAJA
            NOP
            LJMP    HW_JATKORSZ        ; 23-  JATEKOK ENNYI KORBOL ALLNAK
            NOP
            LJMP    HW_JATKRED          ; 24-EGY JATEKOSHOZ SZUKSEGES KREDIT
            NOP
            LJMP    HW_SPLBULL          ; 25 - OSZTOTT BULL EYE ALLITAS
            NOP
            LJMP    HW_KRIKZAR          ; 26-KRIKETTBEN MINDENKI ZARJON
            NOP
            LJMP    HW_TKRIKGYOZ        ; 27 - TEAM KRIKETT GYOZELEMHEZ ZARNI
            NOP
            LJMP    HW_PLUSZKOR         ; 28 - PLUSZ KOROK SZAMA NEHEZITVE
            NOP
            LJMP    HW_FELDOBAS         ; 29 - BULLRA DOBAS ENGEDELYEZES
            NOP
            LJMP    HW_NYILFOGY         ; 30 - NYILELFOGYAS HANGJA
            NOP
            LJMP    HW_TEST             ; 31 - ATLEPES TESZT MODBA
            NOP
            LJMP    HW_BYE              ; 32 - ATLEPES JATEK MODBA
            NOP

; >HWRUTIN
HWRUTIN         KILAM   IZDOUT
                KILAM   IZMAS           ; A RUTINBAN NEM BIZTOS, HOGY EGJEN
                MOV    DPTR,#HWSETUP
                MOV    A,BREG           ; BREG-BEN A SETUP SZAMA, BCD
                LCALL  BCD2BIN          ; KONVERZIO BINARISRA
                DEC    A
                RL     A
                RL     A
                JMP    @A+DPTR           ; UGRAS ERRE A CIMRE
HWEND        ;   MOV    A,#33H
             ;   LCALL  KIIRAS8
                RET                      ; IDE VARJUK VISSZA AZ UGRAST


; MINDEN SETUP RUTIN ELEJEN EZT MEGHVIJUK
; >SETBELEP
SETBELEP      CLR     MINDKI
              BELAM   IZMAS
              BELAM   IZDOUT
            ;  LCALL   SETLEDBE
              RET

; A SETUP RUTINOK KILEPESKOR EZT A RUTINT HIVJAK MEG - KIJELZO TORLES
; >SETKILEP
SETKILEP    LCALL   BUFINI
            LCALL   DARTKIJ
            KILAM   IZGOMB2
            KILAM   IZMAS
            KILAM   IZDOUT
            KILAM   IZDIN
            KEYCLR
            RET

; YES-NO ALLAPOT ALLITASA
; BEJOVO ADAT : CREG
; KIMENO ADAT : CREG
; >YESNOBOX
YESNOBOX      LCALL   SETBELEP
              MOV     NUM,CREG
HWSO4         MOV     A,NUM             ; KIJELZESHEZ ELOKESZITNEI
              LCALL   SETUPMESS
              LCALL   DARTKIJ
HWSO1         KEYCLR
              LCALL   READSETKEY
              MOV     A,KEY
              JZ      HWSO1
              CJNE    A,#CHANGE,HWSO2
              JNB     MINDKI,HWSO6
              MOV     CREG,NUM          ; KIMENO ADATOT ELOKESZITJUK
HWSO6         LCALL   SETKILEP
              RET
HWSO2         CJNE    A,#UP,HWSO3        ; HA NOVELES, FOLYTATODIK
              MOV     A,#1
              MOV     NUM,A
              SETB    MINDKI            ; JELEZZUK, HOGY VALTOZAS TORTENT
              SJMP    HWSO4
HWSO3         CJNE    A,#DOWN,HWSO5      ; HA NEM DOWN, UGRIK
              CLR     A
              MOV     NUM,A
              SETB    MINDKI
              SJMP    HWSO4
HWSO5         KEYCLR
              SJMP    HWSO4

; ONE-ALL ALLAPOT ALLITASA
; BEJOVO ADAT : CREG
; KIMENO ADAT : CREG
; >ONEALLBOX
ONEALLBOX     LCALL   SETBELEP
              MOV     NUM,CREG
OASO4         MOV     A,NUM             ; KIJELZESHEZ ELOKESZITNEI
              ADD     A,#5              ; KIIRATASI OFFSET
              LCALL   SETUPMESS
              LCALL   DARTKIJ
OASO1         KEYCLR
              LCALL   READSETKEY
              MOV     A,KEY
              JZ      OASO1
              CJNE    A,#CHANGE,OASO2
              JNB     MINDKI,OASO6
              MOV     CREG,NUM          ; KIMENO ADATOT ELOKESZITJUK
OASO6         LCALL   SETKILEP
              RET
OASO2         CJNE    A,#UP,OASO3        ; HA NOVELES, FOLYTATODIK
              MOV     A,#1
              MOV     NUM,A
              MOV     CREG,A
              SETB    MINDKI            ; JELEZZUK, HOGY VALTOZAS TORTENT
              SJMP    OASO4
OASO3         CJNE    A,#DOWN,OASO5      ; HA NEM DOWN, UGRIK
              CLR     A
              MOV     NUM,A
              MOV     CREG,A
              SETB    MINDKI
              SJMP    OASO4
OASO5         KEYCLR
              SJMP    OASO4

; SZAMLALO JELLEGU KIJELZES
; BEJOVO ADATOK: CREG - DREG - AREG
; BEJOVO ADAT:   JNUM - A KIJELZOS KEZOD POZICIOJA
; >SZAMLALOK
SZAMLALOK ; KILAM   IZMAS
          ; KILAM   IZDOUT
         ;  LCALL   SETLEDBE
            MOV     A,CREG
            SWAP    A           ; ALSO-FELSO NEGYES CSERE
            MOV     CREG,A      ; ELMENTJUK ESOBBRE
            ANL     A,#0FH      ; FELSO NEGYEST NULLAZZUK
            MOV     SNUM,A      ;
            MOV     A,DREG
            SWAP    A
            MOV     DREG,A      ; ELMENTES KESOBBRE
            MOV     A,CREG
            ANL     A,#0F0H
            MOV     NUM,A
            MOV     A,DREG
            ANL     A,#0FH
            ORL     NUM,A
         ;   MOV     JNUM,#7
            LCALL   PONTSZAM    ; KIIRATAS A KIJELZORE
            INC     JNUM
            MOV     A,DREG
            SWAP    A
            MOV     SNUM,A      ; MASODIK HARMAS ELSO SZAMJEGY
            ANL     SNUM,#0FH   ; FELSO NEGY BIT NULLAZVA
            MOV     NUM,AREG
            LCALL   PONTSZAM    ; KIIRATAS A KIJELZORE
            LCALL   DARTKIJ
            RET

; >WAIT_EXIT
WAIT_EXIT
HWA1        KEYCLR
          ;  LCALL   READSETKEY
            LCALL   RD_CHANGE
            MOV     A,KEY
            JZ      HWA1
            CJNE    A,#CHANGE,HWA1
            LCALL   SETKILEP
            RET


GYARSZAM_INIT   MOV       NUM,#56H
                XBYTEKI   GYAR_SZAM,#2,NUM
                MOV       NUM,#34H
                XBYTEKI   GYAR_SZAM,#1,NUM
                MOV       NUM,#12H
                XBYTEKI   GYAR_SZAM,#0,NUM
                MOV       DPTR,#SPBULL
                MOV       B,#24
GYSZI1          CLR       A
                MOVX      @DPTR,A
                INC       DPTR
                DJNZ      B,GYSZI1
                RET


; 01-GYARI BEALLITAS BETOLTESE
; >HW_FACTSET
HW_FACTSET    MOV   A,BREG
              PLAYER  2,2
              LCALL   SETUPJAT
              CLR     A                 ; ALAPHELYZETBEN NEM NULLAZTATUNK
              MOV     CREG,A
              LCALL   YESNOBOX
              MOV     A,CREG
              JZ      HWLF1             ; HA NEM VOLT VALTOZAS, KILEPHETUNK
              MOV     A,#H_TRIPDOB      ; NULLAZASHOZ HANG
              LCALL   H_EFFEKT
              CLR     LEZARVA
              LCALL   LOADFACT          ; GYARI ADAT BETOLTES
              LCALL   SETUPSAVE
HWLF1         LJMP    HWEND

; 02-GYARI SZAM KIJELZESE
; HW_SERNUM
HW_SERNUM     MOV     A,BREG             ; SETUP NEV KIIRATAS
              PLAYER  2,1
              LCALL   SETUPJAT
              XBYTE   GYAR_SZAM,#0
              MOV     CREG,A            ; ELMENTJUK ESOBBRE
              XBYTE   GYAR_SZAM,#1
              MOV     DREG,A            ; ELMENTES KESOBBRE
              XBYTE   GYAR_SZAM,#2
              MOV     AREG,A
              MOV     JNUM,#7
              LCALL   SZAMLALOK
              MOV     JNUM,#6           ; A 6.JATEKOSHOZ IRATJUK KI
              MOV     NUM,A
              CBYTE   VER,#0
              MOV     SNUM,A            ; SZAZASOK HELYEN NE LEGYEN SEMMI
              CBYTE   VER,#2
              MOV     NUM,A             ; SZAZASOK HELYEN NE LEGYEN SEMMI
              LCALL   PONTSZAM          ; KIIRATAS A KIJELZORE
              PUSH    B                 ; HASZNALT REGISZTER MENTESE
              MOV     JNUM,#2
              MOV     SEG,#2
              MOV     NUM,#10H          ; EZ A KOTOJEL
              MOV     B,#2              ; 2. DIGITRE IRUNK
              LCALL   EGYSZAM           ; KIIRATAS
              LCALL   DARTKIJ
              LCALL   WAIT_EXIT         ; VARAKOZIK A KILEPESRE
              POP     B
              LJMP    HWEND

; 03-UZEMELTETOI SZAM BEIRASA
; >HW_UZEMSZAM
HW_UZEMSZAM   ; LCALL     SETBELEP
           ;   KILAM       IZDOUT
           ;   KILAM       IZMAS         ; A KEZELOGOMBOK LAMPAI KIKAPCS.
              MOV       A,BREG
              PLAYER    2,2
              LCALL     SETUPJAT
              XBYTE   UZEM_SZAM,#0
              MOV     NUM,A
USR44         MOV     JNUM,#8
              MOV     SNUM,#0AH         ; SZAZASOK HELYEN NE LEGYEN SEMMI
              LCALL   PONTSZAM          ; KIIRATAS A KIJELZORE
              LCALL   DARTKIJ
USR1          KEYCLR
              LCALL   RD_CHANGE         ; CSAK A VALTOGOMB ERDEKES
              MOV     A,KEY
              JZ      USR1
              CJNE    A,#CHANGE,USR1    ; AMIG NEM A VALTOT NYOMJAK, VISSZA
              LCALL   SETBELEP
USR4          MOV     JNUM,#8
              MOV     SNUM,#0AH         ; SZAZASOK HELYEN NE LEGYEN SEMMI
              LCALL   PONTSZAM          ; KIIRATAS A KIJELZORE
              LCALL   DARTKIJ
USR7          KEYCLR                    ; INNEN MAR RENDESEN MEGY TOVABB
              LCALL   READSETKEY
              MOV     A,KEY
              JZ      USR7
              CJNE    A,#CHANGE,USR2    ; AMIG NEM A VALTOT NYOMJAK, VISSZA
              JNB     MINDKI,USR6
              LCALL   SETUPSAVE         ; HA VOLT VALTOZAS, MENTJUK A SETUPOT
USR6          LCALL   SETKILEP
              LJMP    HWEND
USR2          CJNE    A,#UP,USR3        ; HA NOVELES, FOLYTATODIK
              MOV     A,NUM
              ADD     A,#1
              DA      A
              MOV     NUM,A
              XBYTEKI UZEM_SZAM,#0,NUM  ; ADATOT VISSZAIRJUK
              SETB    MINDKI            ; JELEZZUK, HOGY VALTOZAS TORTENT
              SJMP    USR4
USR3          CJNE    A,#DOWN,USR5      ; HA NEM DOWN, UGRIK
              CLR     A
              MOV     NUM,A
              XBYTEKI UZEM_SZAM,#0,NUM
              SETB    MINDKI
              SJMP    USR4
USR5          KEYCLR
              SJMP    USR4

; PIN-CODE, KODSZAM KERO RUTIN
; >PINKOD
PINKOD       MOV        A,#4            ;   PIN      FELIRAT
             PLAYER     1,1             ; FELSO KIJEZO KOZEPSO
             LCALL      SETUPJAT
PNK0         MOV        MAXJAT,#0         ; EZ LESZ A SZAMJEGY SZAMLALO
PNK1         LCALL      DOBERT1         ; VAR EGY DOBAST
             JZ         PNK1
             PUSH       CTPONT          ; A DOBOTT SZAMOT ELMENTJUK
             MOV     A,#H_JODOB
             LCALL   H_EFFEKT
PNK1A        LCALL      DOBERT1         ; MEGVARJUK, MIG ELENGEDIK
             JNZ        PNK1A
             CBYTE      PIN_KOD,MAXJAT    ; SZAMJEGY BEOLVASAS
             POP        CTPONT
             CJNE       A,CTPONT,PNK0    ; HA NEM EGYEZIK, ELOLROL
             INC        MAXJAT             ; AZ ELSO SZAM JO, MEHET TOVABB
             MOV        A,MAXJAT
             CJNE       A,#4,PNK1        ; HA MEG NINCS MEG MIND, VISSZA
             MOV        A,#H_GYOZ0
             LCALL      H_EFFEKT
             LCALL      BUFINI
             LCALL      DARTKIJ
             LCALL      TIM500MS
             RET

; 04-BIZ. IDO KIKAPCS UTAN JELSZO
; >HW_PASSWORD
HW_PASSWORD   MOV     A,BREG
              PLAYER  2,2
              LCALL   SETUPJAT
              XBYTE   PASS_WD,#0
              MOV     CREG,A
              LCALL   YESNOBOX
              JNB     MINDKI,HWP1
              XBYTEKI PASS_WD,#0,CREG
              LCALL   SETUPSAVE
HWP1          LJMP    HWEND

; 05-INFRA ERZEKELO UZEMMOD
; >HW_INFRASET
HW_INFRASET   LCALL   SETBELEP
              MOV     A,BREG
              PLAYER  2,2             ; HOVA KELL KIIRNI A SZOVEGET
              LCALL   SETUPJAT
              XBYTE   INF_MODE,#0
              MOV     NUM,A
HIK4          MOV     JNUM,#8
              MOV     SNUM,#0AH   ; SZAZASOK HELYEN NE LEGYEN SEMMI
              LCALL   PONTSZAM    ; KIIRATAS A KIJELZORE
              LCALL   DARTKIJ
HIK1          KEYCLR
              LCALL   READSETKEY
              MOV     A,KEY
              JZ      HIK1
              CJNE    A,#CHANGE,HIK2
              JNB     MINDKI,HIK6
              LCALL   SETUPSAVE         ; HA VOLT VALTOZAS, MENTJUK A SETUPOT
HIK6          LCALL   SETKILEP
              LJMP    HWEND
HIK2          CJNE    A,#UP,HIK3        ; HA NOVELES, FOLYTATODIK
              MOV     A,NUM
              CJNE    A,#2,HIK7
              SJMP    HIK4
HIK7          ADD     A,#1
              DA      A
              MOV     NUM,A
              XBYTEKI INF_MODE,#0,NUM  ; ADATOT VISSZAIRJUK
              SETB    MINDKI            ; JELEZZUK, HOGY VALTOZAS TORTENT
              SJMP    HIK4
HIK3          CJNE    A,#DOWN,HIK5      ; HA NEM DOWN, UGRIK
              CLR     A
              MOV     NUM,A
              XBYTEKI INF_MODE,#0,NUM
              SETB    MINDKI
              SJMP    HIK4
HIK5          KEYCLR
              SJMP    HIK4

; 06-VALTO ELJARAS KESLELTETESE
; >HW_VALTKES
HW_VALTKES    LCALL     SETBELEP
              MOV     A,BREG
              PLAYER  2,2
              LCALL   SETUPJAT
              XBYTE   VALT_KES,#0
              MOV     NUM,A
HVT4          MOV     JNUM,#8
              MOV     SNUM,#0AH   ; SZAZASOK HELYEN NE LEGYEN SEMMI
              LCALL   PONTSZAM    ; KIIRATAS A KIJELZORE
              LCALL   DARTKIJ
HVT1          KEYCLR
              LCALL   READSETKEY
              MOV     A,KEY
              JZ      HVT1
              CJNE    A,#CHANGE,HVT2
              JNB     MINDKI,HVT6
              LCALL   SETUPSAVE         ; HA VOLT VALTOZAS, MENTJUK A SETUPOT
HVT6          LCALL   SETKILEP
              LJMP    HWEND
HVT2          CJNE    A,#UP,HVT3        ; HA NOVELES, FOLYTATODIK
              MOV     A,NUM
              CJNE    A,#8,HVT7
              SJMP    HVT4
HVT7          ADD     A,#1
              DA      A
              MOV     NUM,A
              XBYTEKI VALT_KES,#0,NUM  ; ADATOT VISSZAIRJUK
              SETB    MINDKI            ; JELEZZUK, HOGY VALTOZAS TORTENT
              SJMP    HVT4
HVT3          CJNE    A,#DOWN,HVT5      ; HA NEM DOWN, UGRIK
              MOV     A,#1
              MOV     NUM,A
              XBYTEKI VALT_KES,#0,NUM
              SETB    MINDKI
              SJMP    HVT4
HVT5          KEYCLR
              SJMP    HVT4


; 07-JATEKSZUNETBEN DOBASKOR SZIRENA
; >HW_SZUNSZIR
HW_SZUNSZIR   MOV     A,BREG
              PLAYER  2,2
              LCALL   SETUPJAT
              XBYTE   SZUN_SZIR,#0
              MOV     CREG,A
              LCALL   YESNOBOX
              JNB     MINDKI,HWSR1
              XBYTEKI SZUN_SZIR,#0,CREG
              LCALL   SETUPSAVE
HWSR1         LJMP    HWEND

; 08-HANGOK BEALLITASAI
; >HW_SONGSEL
HW_SONGSEL    LCALL   SETBELEP
              MOV     A,BREG
              PLAYER  2,2
              LCALL   SETUPJAT
              XBYTE   WIN_ZENE,#0
              MOV     NUM,A
ZVT4          MOV     JNUM,#8
              MOV     SNUM,#0AH   ; SZAZASOK HELYEN NE LEGYEN SEMMI
              LCALL   PONTSZAM    ; KIIRATAS A KIJELZORE
              LCALL   DARTKIJ
ZVT1          KEYCLR
              LCALL   READSETKEY
              MOV     A,KEY
              JZ      ZVT1
              CJNE    A,#CHANGE,ZVT2
              JNB     MINDKI,ZVT6
              LCALL   SETUPSAVE         ; HA VOLT VALTOZAS, MENTJUK A SETUPOT
ZVT6          LCALL   SETKILEP
              LJMP    HWEND
ZVT2          CJNE    A,#UP,ZVT3        ; HA NOVELES, FOLYTATODIK
              MOV     A,NUM
              CJNE    A,#6,ZVT7
              SJMP    ZVT4
ZVT7          ADD     A,#1
              DA      A
              MOV     NUM,A
              XBYTEKI WIN_ZENE,#0,NUM  ; ADATOT VISSZAIRJUK
              SETB    MINDKI            ; JELEZZUK, HOGY VALTOZAS TORTENT
              SJMP    ZVT4
ZVT3          CJNE    A,#DOWN,ZVT5      ; HA NEM DOWN, UGRIK
              CLR     A
              MOV     NUM,A
              XBYTEKI WIN_ZENE,#0,NUM
              SETB    MINDKI
              SJMP    ZVT4
ZVT5          KEYCLR
              SJMP    ZVT4

; 09-BESZEDHANG ENGEDELYEZES
; >HW_SPEAK
HW_SPEAK      LCALL     SETBELEP
              MOV     A,BREG
              PLAYER  2,2
              LCALL   SETUPJAT
              XBYTE   SPEAK_EN,#0
              MOV     NUM,A
SPE4          MOV     JNUM,#8
              MOV     SNUM,#0AH   ; SZAZASOK HELYEN NE LEGYEN SEMMI
              LCALL   PONTSZAM    ; KIIRATAS A KIJELZORE
              LCALL   DARTKIJ
SPE1          KEYCLR
              LCALL   READSETKEY
              MOV     A,KEY
              JZ      SPE1
              CJNE    A,#CHANGE,SPE2
              JNB     MINDKI,SPE6
              LCALL   SETUPSAVE         ; HA VOLT VALTOZAS, MENTJUK A SETUPOT
SPE6          LCALL   SETKILEP
              LJMP    HWEND
SPE2          CJNE    A,#UP,SPE3        ; HA NOVELES, FOLYTATODIK
              MOV     A,NUM
              CJNE    A,#3,SPE7
              SJMP    SPE4
SPE7          ADD     A,#1
              DA      A
              MOV     NUM,A
              XBYTEKI SPEAK_EN,#0,NUM  ; ADATOT VISSZAIRJUK
              SETB    MINDKI            ; JELEZZUK, HOGY VALTOZAS TORTENT
              SJMP    SPE4
SPE3          CJNE    A,#DOWN,SPE5      ; HA NEM DOWN, UGRIK
              CLR     A
              MOV     NUM,A
              XBYTEKI SPEAK_EN,#0,NUM
              SETB    MINDKI
              SJMP    SPE4
SPE5          KEYCLR
              SJMP    SPE4

;  10- VERSENY BEALLITASOK
; >HW_CHAMPION
HW_CHAMPION   LCALL     SETBELEP
              MOV     A,BREG
              PLAYER  2,2
              LCALL   SETUPJAT
              MOV     A,#10H          ; CHA - SZO KIIRASA
              PLAYER  0,1
              LCALL   SETUPJAT
              XBYTE   CHAMP_SET,#0
              MOV     NUM,A
CHE4          MOV     JNUM,#8
              MOV     SNUM,#0AH   ; SZAZASOK HELYEN NE LEGYEN SEMMI
              LCALL   PONTSZAM    ; KIIRATAS A KIJELZORE
              LCALL   DARTKIJ
CHE1          KEYCLR
              LCALL   READSETKEY
              MOV     A,KEY
              JZ      CHE1
              CJNE    A,#CHANGE,CHE2
              JNB     MINDKI,CHE6
              LCALL   SETUPSAVE         ; HA VOLT VALTOZAS, MENTJUK A SETUPOT
CHE6          LCALL   SETKILEP
              LJMP    HWEND
CHE2          CJNE    A,#UP,CHE3        ; HA NOVELES, FOLYTATODIK
              MOV     A,NUM
              CJNE    A,#3,CHE7
              SJMP    CHE4
CHE7          ADD     A,#1
              DA      A
              MOV     NUM,A
              XBYTEKI CHAMP_SET,#0,NUM  ; ADATOT VISSZAIRJUK
              SETB    MINDKI            ; JELEZZUK, HOGY VALTOZAS TORTENT
              SJMP    CHE4
CHE3          CJNE    A,#DOWN,CHE5      ; HA NEM DOWN, UGRIK
              CLR     A
              MOV     NUM,A
              XBYTEKI CHAMP_SET,#0,NUM
              SETB    MINDKI
              SJMP    CHE4
CHE5          KEYCLR
              SJMP    CHE4

;  11- ERMEVIZSGALO ALLASOK
; >HW_COUNTERS
HW_COUNTERS   BELAM   IZMAS
              XBYTE   CH_NUM,#0         ; CSATORNASZAM BEOLVASAS
              MOV     MAXJAT,A          ; A MAXJAT-BA
              MOV     A,#33H            ; LFT - LIFETIME SZO KIIRASA
              PLAYER  0,2
              LCALL   SETUPJAT
COU_KE        MOV     B,#1              ; CH1-EL KEZDJUK A KIIRATAST
              MOV     AKTKOR,#0         ; BAZISCIM ELTOLAS VALTOZOJA
COU_K         PLAYER    0,1             ; A C-C FELIRAT ATMEGY BALRA
              MOV       A,B
              LCALL     SETUP_CH        ; CSATORNASZAM KIIRATAS
              XRBYTE   CH1_ALL,AKTKOR
              MOV      CREG,A           ; ELMENTJUK ESOBBRE
              XRBYTE   CH1_ALL+1,AKTKOR
              MOV      DREG,A           ; ELMENTES KESOBBRE
              XRBYTE   CH1_ALL+2,AKTKOR
              MOV      AREG,A
              MOV      JNUM,#3
              LCALL    SZAMLALOK
              MOV      A,B              ;  CSATORNASZAM KIIRATAS
              PLAYER   2,1
              LCALL    SETUP_CH
              MOV      A,#34H           ; CLE - CLEALABLE SZO KIIRASA
              PLAYER   2,2
              LCALL    SETUPJAT
              XRBYTE   CH1_NAPI,AKTKOR
              MOV      CREG,A           ; ELMENTJUK ESOBBRE
              XRBYTE   CH1_NAPI+1,AKTKOR
              MOV      DREG,A           ; ELMENTES KESOBBRE
              XRBYTE   CH1_NAPI+2,AKTKOR
              MOV      AREG,A
              MOV      JNUM,#7
              LCALL    SZAMLALOK
         ;     BELAM     IZMAS           ; A SZAMLALOK RUTIN KIKAPCSOLJA
COU_K1        KEYCLR                     ; BILLENTYUZET OLVASAS
              LCALL   RD_CHANGE
              MOV     A,KEY
              JZ      COU_K1A             ; HA NEM JOTT SEMMI, VARAKOZAS
              CJNE    A,#CHANGE,COU_K2A   ; HA NEM KILEPES, UGRAS
              LCALL   SETKILEP
              LJMP    HWEND

COU_K1A       LCALL   RD_UP
              MOV     A,KEY
              JZ      COU_K1
COU_K2A       MOV     A,B                ; ELERTEK A MAX. CSAT. SZAMOT ?
              JB      MECH_SEL,COU_K2M   ; HA MECHANIKUS, UGRAS
              CJNE    A,MAXJAT,COU_K7A   ; ELEKTRONIKUSNAL MAX. 5 CSATORNA
              MOV     B,#1               ; HA ELERTE, KEZDJUK ELOLROL
              LJMP    COU_KE
COU_K2M       CJNE    A,#2H,COU_K7A      ; MECHANIKUSNAL 2 CSATORNA
              LJMP    COU_KE             ; HA KORBEERT, KEZDJUK ELOLROL
COU_K7A       ADD     A,#1
              DA      A
              MOV     B,A                ; CSATORNASZAM NOVELES
              MOV     A,AKTKOR           ; ELTOLAS NOVELES
              ADD     A,#3
              MOV     AKTKOR,A
              LJMP    COU_K              ; VISSZA A CIKLUSBA
COU_K5        KEYCLR                     ; EGYEB BILL. ESEMENYEKET TOROL
              SJMP    COU_K1             ; VISSZA A CIKLUS ELEJERE


; 12-KULCSOS ERMEVIZSGALO SZAMLALOI
; >HW_COUNKEY
HW_COUNTKEY   MOV     A,#35H            ; FED - KULCSOS FELIRAT
              PLAYER  0,1
              LCALL   SETUPJAT
              MOV     A,#33H            ; LFT - LIFETIME SZO KIIRASA
              PLAYER  0,2
              LCALL   SETUPJAT
              XBYTE   KEY_ALL,#0
              MOV     CREG,A            ; ELMENTJUK ESOBBRE
              XBYTE   KEY_ALL,#1
              MOV     DREG,A            ; ELMENTES KESOBBRE
              XBYTE   KEY_ALL,#2
              MOV     AREG,A
              MOV     JNUM,#3
              LCALL   SZAMLALOK
              MOV     A,#35H            ; FED - FELIRAT
              PLAYER  2,1
              LCALL   SETUPJAT
              MOV     A,#34H            ; CLE - CLEALABLE SZO KIIRASA
              PLAYER  2,2
              LCALL   SETUPJAT
              XBYTE   KEY_NAPI,#0
              MOV     CREG,A            ; ELMENTJUK ESOBBRE
              XBYTE   KEY_NAPI,#1
              MOV     DREG,A            ; ELMENTES KESOBBRE
              XBYTE   KEY_NAPI,#2
              MOV     AREG,A
              MOV     JNUM,#7
              LCALL   SZAMLALOK
              LCALL   WAIT_EXIT         ; VARAKOZIK A KILEPESRE
              LJMP  HWEND

; 13-JATEK SZAMLALO -NAPI ES OSSZES
; >HW_COUNTGAME
HW_COUNTGAME  MOV     A,#13H            ;  GAC - GAME COUNTER
              PLAYER  0,1
              LCALL   SETUPJAT
              MOV     A,#33H            ; LFT - LIFETIME SZO KIIRASA
              PLAYER  0,2
              LCALL   SETUPJAT
              XBYTE   GAME_ALL,#0
              MOV     CREG,A            ; ELMENTJUK ESOBBRE
              XBYTE   GAME_ALL,#1
              MOV     DREG,A            ; ELMENTES KESOBBRE
              XBYTE   GAME_ALL,#2
              MOV     AREG,A
              MOV     JNUM,#3
              LCALL   SZAMLALOK
              MOV     A,#13H            ; GAC - GAME COUNTER
              PLAYER  2,1
              LCALL   SETUPJAT
              MOV     A,#34H            ; CLE - CLEALABLE SZO KIIRASA
              PLAYER  2,2
              LCALL   SETUPJAT
              XBYTE   GAME_NAPI,#0
              MOV     CREG,A            ; ELMENTJUK ESOBBRE
              XBYTE   GAME_NAPI,#1
              MOV     DREG,A            ; ELMENTES KESOBBRE
              XBYTE   GAME_NAPI,#2
              MOV     AREG,A
              MOV     JNUM,#7
              LCALL   SZAMLALOK
              LCALL   WAIT_EXIT         ; VARAKOZIK A KILEPESRE
              LJMP  HWEND

; 14-SZABADJATEK SZAMLALOK
; >HW_COUNTFREE
HW_COUNTFREE  MOV     A,#14H            ;  FRC - FREE COUNTER
              PLAYER  0,1
              LCALL   SETUPJAT
              MOV     A,#38H            ;  CRE - CREDIT SZO KIIRASA
              PLAYER  0,2
              LCALL   SETUPJAT
              XBYTE   CRED_FREE,#0
              MOV     CREG,A            ; ELMENTJUK ESOBBRE
              XBYTE   CRED_FREE,#1
              MOV     DREG,A            ; ELMENTES KESOBBRE
              XBYTE   CRED_FREE,#2
              MOV     AREG,A
              MOV     JNUM,#3
              LCALL   SZAMLALOK
              MOV     A,#14H            ; FRC - FREE COUNTER
              PLAYER  2,1
              LCALL   SETUPJAT
              MOV     A,#37H            ; PLD - PLAYED SZO KIIRASA
              PLAYER  2,2
              LCALL   SETUPJAT
              XBYTE   GAME_FREE,#0
              MOV     CREG,A            ; ELMENTJUK ESOBBRE
              XBYTE   GAME_FREE,#1
              MOV     DREG,A            ; ELMENTES KESOBBRE
              XBYTE   GAME_FREE,#2
              MOV     AREG,A
              MOV     JNUM,#7
              LCALL   SZAMLALOK
              LCALL   WAIT_EXIT         ; VARAKOZIK A KILEPESRE
              LJMP  HWEND

; 15-SZAMLALOK NULLAZASA
; NULLAZZA A NAPI SZAMLALOKAT
; >HW_SZAMLNULL
HWSZM1        LJMP  HWEND
HW_SZAMLNULL  MOV   A,#39H              ; 000- NULLAZASI SZIMBOLUM
              PLAYER  2,2
              LCALL   SETUPJAT
              CLR     A                 ; ALAPHELYZETBEN NEM NULLAZTATUNK
              MOV     CREG,A
              LCALL   YESNOBOX
              MOV     A,CREG
              JZ      HWSZM1             ; HA NEM VOLT VALTOZAS, NEM MENT
              MOV     A,#H_TRIPDOB       ; NULLAZASHOZ HANG
              LCALL   H_EFFEKT
              PUSH    B
              MOV     B,#18              ;NULLAZZUK A NAPI SZAMLALOKAT
              CLR     A
              MOV     DPTR,#CHNAP_BASE
SZMN1         MOVX    @DPTR,A
              INC     DPTR
              DJNZ    B,SZMN1
              MOV     B,#0
              XBYTEKI KEY_NAPI,#0,B
              XBYTEKI KEY_NAPI,#1,B
              XBYTEKI KEY_NAPI,#2,B
              XBYTEKI GAME_NAPI,#0,B
              XBYTEKI GAME_NAPI,#1,B
              XBYTEKI GAME_NAPI,#2,B
              POP     B
              LCALL   SETUPSAVE
              LJMP  HWEND

; 16-ERMESZAM KREDITENKENT
; >HW_ERMKREDIT
HW_ERMKREDIT  LCALL    SETBELEP
              XBYTE    CH_NUM,#0         ; CSATORNASZAM BEOLVASAS
              INC      A
              MOV      MAXJAT,A          ; AT A MAXJAT-BA
              MOV      A,BREG
              PLAYER   2,1               ; A C-C FELIRAT ATMEGY BALRA
              LCALL    SETUPJAT
              BELAM    IZDIN             ; EZ A GOMB VALTJA A JATEKOKAT
              CLR      TILT              ; CHGAME GOMB ENGEDELYEZES
              MOV      B,#1              ; KEZDETBEN A CH1-EL INDUL

ERM_K         MOV     A,B               ; 6-OS JATEKOSHOZ A CSAT. SZAMA
              PLAYER  2,2
              LCALL   SETUP_CH

              XRBYTE   ERM_BASE,B        ; B SZERINTI ELTOLASSAL
              MOV     NUM,A              ; EBBEN LESZ A KREDIT SZAM
              SWAP    A                  ; ALSO NEGYESBE AZ ERMESZAM
              MOV     SNUM,A             ; EBBEN AZ ERMESZAM
              ANL     SNUM,#0FH          ; AZ ALSO NEGYES MARADJON
              MOV     A,NUM
              ANL     A,#0FH             ; AZ ALSO NEGYES MARAD
              MOV     NUM,A              ; NUM-BAN A KREDIT SZAM
ERMK4         PUSH    NUM                ; ELOKESZITES KIJELZESRE
              ORL     NUM,#0A0H          ; A FELSO NEGYES URES LEGYEN
              MOV     JNUM,#8            ; A NYOLCAS JATEKOSHOZ IRJA KI
              LCALL   PONTSZAM           ; KIIRATAS A KIJELZORE
              POP     NUM
ERM1A         LCALL   DARTKIJ
ERMK1         KEYCLR                     ; BILLENTYUZET OLVASAS
              LCALL   READSETKEY
              MOV     A,KEY
              JZ      ERMK1              ; HA NEM JOTT SEMMI, VARAKOZAS
              CJNE    A,#CHANGE,ERMK2A   ; HA NEM KILEPES, UGRAS
              JNB     MINDKI,ERMK6
              LCALL   SETUPSAVE         ; HA VOLT VALTOZAS, MENTJUK A SETUPOT
ERMK6         LCALL   SETKILEP
              LJMP    HWEND

ERMK2A        CJNE    A,#CHGAME,ERMK2    ; LEPES A KOVETKEZO CSATORNARA
              INC     B                  ; CSATORNASZAMOT NOVELJUK
              MOV     A,B                ; ELERTE A 6-OT ?
              JB      MECH_SEL,ERM2M     ; HA MECHANIKUS, UGRAS
              CJNE    A,MAXJAT,ERM2B     ; ELEKTRONIKUSNAL 5 CSATORNA
              MOV     B,#1               ; HA ELERTE, KEZDJUK ELOLROL
              LJMP    ERM_K
ERM2M         CJNE    A,#3,ERM2B
              MOV     B,#1               ; HA ELERTE, KEZDJUK ELOLROL
ERM2B         LJMP    ERM_K

ERMK2         CJNE    A,#UP,ERMK3        ; ERMESZAM MODOSITAS
              MOV     A,NUM
              CJNE    A,#9H,ERMK7
              MOV     NUM,#0            ; KORBEERTUNK
              SJMP    ERMK4V            ; VALTOZAS TORTENIK, MENTES
ERMK7         ADD     A,#1
              DA      A
              MOV     NUM,A             ; NUM-BA A KREDIT SZAM

ERMK4V        PUSH    NUM               ; MODOSITOTT EREDMENY TAROLASA
              PUSH    SNUM
              ANL     NUM,#0FH          ; AZ ALSO NEGYES MARADJON
              MOV     A,SNUM
              SWAP    A
              ANL     A,#0F0H           ; A FELSO NEGYES MARADJON
              ORL     NUM,A             ; NUM-BAN OSSZEFESULJUK
              XBYTEKI ERM_BASE,B,NUM    ; ADATOT VISSZAIRJUK
              SETB    MINDKI            ; JELEZZUK, HOGY VALTOZAS TORTENT
              POP     SNUM
              POP     NUM
              SJMP    ERMK4

ERMK3         CJNE    A,#DOWN,ERMK5      ; KREDIT SZAM MODOSITAS
              MOV     A,SNUM             ;
              CJNE    A,#9H,ERMK7A
              MOV     SNUM,#0            ; HA K�RBE�RT, KEZDJ�K EL�LR�L
              LJMP    ERMK4V             ; MODOSITAS MENTESE
ERMK7A        ADD     A,#1
              DA      A
              MOV     SNUM,A
              SJMP    ERMK4V             ; MODOSITAS MENTESE
ERMK5         KEYCLR                     ; EGYEB BILL. ESEMENYEKET TOROL
              LJMP    ERMK4              ; VISSZA A CIKLUS ELEJERE

; 17 KREDIT MEGORZES KIKAPCSOLAS UTAN
; Allapot megorzes aramszunet utan

; >HW_KREDORZ
HW_KREDORZ
              LCALL     SETBELEP
              MOV     A,BREG
              PLAYER  2,2
              LCALL   SETUPJAT    ; SETUP LEPES NEVENEK KIIRATASA
              XBYTE   MEG_ORZ,#0  ; AKTUALIS ERTEK BETOLTESE
              MOV     NUM,A
KRD4          MOV     JNUM,#8
              MOV     SNUM,#0AH   ; SZAZASOK HELYEN NE LEGYEN SEMMI
              LCALL   PONTSZAM    ; KIIRATAS A KIJELZORE
              LCALL   DARTKIJ
KRD4A         KEYCLR
              LCALL   READSETKEY
              MOV     A,KEY             ; VARUNK EGY GOMBRA
              JZ      KRD4A
              CJNE    A,#CHANGE,KRD2
              JNB     MINDKI,KRD6
              LCALL   SETUPSAVE         ; HA VOLT VALTOZAS, MENTJUK A SETUPOT
KRD6          LCALL   SETKILEP
              LJMP    HWEND
KRD2          CJNE    A,#UP,KRD3        ; HA NOVELES, FOLYTATODIK
              MOV     A,NUM
              CJNE    A,#2,KRD7         ; MAX. 2 LEHET AZ ERTEK
              SJMP    KRD4
KRD7          ADD     A,#1
              DA      A
              MOV     NUM,A
              XBYTEKI MEG_ORZ,#0,NUM  ; ADATOT VISSZAIRJUK
              SETB    MINDKI            ; JELEZZUK, HOGY VALTOZAS TORTENT
              SJMP    KRD4
KRD3          CJNE    A,#DOWN,KRD5      ; HA NEM DOWN, UGRIK
              CLR     A
              MOV     NUM,A
              XBYTEKI MEG_ORZ,#0,NUM
              SETB    MINDKI
              SJMP    KRD4
KRD5          KEYCLR
              SJMP    KRD4

; 18-SORSOLAS MINDEN JATEK UTAN
; >HW_SORSJAT
HW_SORSJAT    MOV     A,BREG
              PLAYER  2,2
              LCALL   SETUPJAT
              XBYTE   SORS_JAT,#0
              MOV     CREG,A
              LCALL   YESNOBOX
              JNB     MINDKI,HWT1
              XBYTEKI SORS_JAT,#0,CREG
              LCALL   SETUPSAVE
HWT1          LJMP    HWEND

; 19- X JATEK UTAN AJANDEK KREDIT
; >HW_AJANKREDIT
HW_AJANKREDIT LCALL     SETBELEP
              MOV     A,BREG
              PLAYER  2,2
              LCALL   SETUPJAT
              XBYTE   AJA_KRED,#0
              MOV     NUM,A
AJK4          MOV     JNUM,#8
              MOV     SNUM,#0AH   ; SZAZASOK HELYEN NE LEGYEN SEMMI
              LCALL   PONTSZAM    ; KIIRATAS A KIJELZORE
              LCALL   DARTKIJ
AJK1          KEYCLR
              LCALL   READSETKEY
              MOV     A,KEY
              JZ      AJK1
              CJNE    A,#CHANGE,AJK2
              JNB     MINDKI,AJK6
              LCALL   SETUPSAVE         ; HA VOLT VALTOZAS, MENTJUK A SETUPOT
AJK6          LCALL   SETKILEP
              LJMP    HWEND
AJK2          CJNE    A,#UP,AJK3        ; HA NOVELES, FOLYTATODIK
              MOV     A,NUM
              CJNE    A,#50H,AJK7
              SJMP    AJK4
AJK7          ADD     A,#1
              DA      A
              MOV     NUM,A
              XBYTEKI AJA_KRED,#0,NUM   ; ADATOT VISSZAIRJUK
              SETB    MINDKI            ; JELEZZUK, HOGY VALTOZAS TORTENT
              SJMP    AJK4
AJK3          CJNE    A,#DOWN,AJK5      ; HA NEM DOWN, UGRIK
              CLR     A
              MOV     NUM,A
              XBYTEKI AJA_KRED,#0,NUM
              SETB    MINDKI
              SJMP    AJK4
AJK5          KEYCLR
              SJMP    AJK4

; 20-UZEMORA SZAMOK KIIRATASA
; >HW_UZEMORA
HW_UZEMORA
              MOV     A,#3AH            ; HOURS
              PLAYER  0,1
              LCALL   SETUPJAT
              XBYTE   UZEM_ORA,#0
              MOV     CREG,A            ; ELMENTJUK ESOBBRE
              XBYTE   UZEM_ORA,#1
              MOV     DREG,A            ; ELMENTES KESOBBRE
              XBYTE   UZEM_ORA,#2
              MOV     AREG,A
              MOV     JNUM,#3
              LCALL   SZAMLALOK
              MOV     A,#3BH            ; PASSIVE TIME
              PLAYER  2,1
              LCALL   SETUPJAT
              XBYTE   PAS_ORA,#0
              MOV     CREG,A            ; ELMENTJUK ESOBBRE
              XBYTE   PAS_ORA,#1
              MOV     DREG,A            ; ELMENTES KESOBBRE
              XBYTE   PAS_ORA,#2
              MOV     AREG,A
              MOV     JNUM,#7
              LCALL   SZAMLALOK
              LCALL   WAIT_EXIT         ; VARAKOZIK A KILEPESRE
              LJMP  HWEND

; 21-STATISZTIKA ENGEDELYEZES
; >HW_STAT
HW_STAT       MOV     A,BREG
              PLAYER  2,2
              LCALL   SETUPJAT
              XBYTE   STAT_EN,#0
              MOV     CREG,A
              LCALL   YESNOBOX
              JNB     MINDKI,HWST1       ; HA NEM VOLT VALTOZAS, NEM MENT
              XBYTEKI STAT_EN,#0,CREG
              LCALL   SETUPSAVE
HWST1         LJMP    HWEND

; 22-JATEKOK NEPSZERUSEGI LISTAJA
; >HW_JATNEPSZ
HW_JATNEPSZ   LCALL    SETBELEP
              KILAM    IZDOUT
              KILAM    IZMAS
              MOV     A,BREG
              PLAYER  2,1
              LCALL   SETUPJAT
             BELAM    IZDIN             ; EZ A GOMB VALTJA A JATEKOKAT
             CLR      TILT              ; ITT KELL A 2.GOMB IS !
             MOV      MAXJAT,#1         ; MAXJAT-BAN A JATEK SORSZAMA
JTN4         MOV      DPTR,#JAT_NEPSZ
             MOV      A,MAXJAT          ; JATEK SZAMANAK BETOLTESE
             DEC      A
             MOV      B,#3              ; 3 BYTE EGY JATEK
             MUL      AB                ; A-BAN AZ EREDMENY
             ADD      A,DPL
             MOV      DPL,A             ; ELTOLASOS CIM KISZAMITVA
             MOVX     A,@DPTR
             MOV      CREG,A            ; ELSO BYTE
             INC      DPTR
             MOVX     A,@DPTR
             MOV      DREG,A
             INC      DPTR
             MOVX     A,@DPTR
             MOV      AREG,A
             MOV      JNUM,#7           ; LEGALSO SORBA IRATJUK KI
             LCALL    SZAMLALOK
             MOV     A,MAXJAT
             PLAYER  2,2                ; HOVA KELL KIIRNI A SZOVEGET
             LCALL   SETJATCIM          ; JATEK NEVENEK KIIRATASA
             LCALL   DARTKIJ
JTN1         KEYCLR
             LCALL   RD_CHANGE          ; FIGYELJUK A VALTOGOMBOT
             MOV     A,KEY
             JZ      JTN1A
             LCALL   SETKILEP           ; HA A VALTOGOMB, KILEPES
             LJMP    HWEND
JTN1A        LCALL   RD_CHGAME
             MOV     A,KEY
             JZ      JTN1              ; HA MOST SINCS GOMB, VISSZA
JTN5         CBYTE   JATSOR,MAXJAT
             MOV     MAXJAT,A
             LJMP    JTN4
JTN8         KEYCLR
             LJMP    JTN4

; 23-  JATEKOK KORSZAMA
; >HW_JATKORSZ
HW_JATKORSZ  LCALL    SETBELEP
             MOV     A,BREG
             PLAYER  2,1
             LCALL   SETUPJAT
             BELAM    IZDIN             ; EZ A GOMB VALTJA A JATEKOKAT
             CLR      TILT              ; ITT KELL A 2.GOMB IS !
             MOV      MAXJAT,#1         ; MAXJAT-BAN A JATEK SORSZAMA
JKR4         XRBYTE    JKORMAX,MAXJAT   ; BEOLVASSUK A JELENLEGI MAX.-OT
             MOV     NUM,A              ; KIJELZESHEZ
             MOV     JNUM,#8            ; UTOLSO MEZOBE IRUNK
             MOV     SNUM,#0AH          ; SZAZASOK HELYEN NE LEGYEN SEMMI
             LCALL   PONTSZAM           ; KIIRATAS A KIJELZORE
             MOV     A,MAXJAT
             PLAYER  2,2                ; HOVA KELL KIIRNI A SZOVEGET
             LCALL   SETJATCIM          ; JATEK NEVENEK KIIRATASA
             LCALL   DARTKIJ
JKR1         KEYCLR
             LCALL   READSETKEY
             MOV     A,KEY
             JZ      JKR1
             CJNE    A,#CHANGE,JKR2    ; HA A LEPTETO, KILEPES
             JNB     MINDKI,JKR6
             LCALL   SETUPSAVE         ; HA VOLT VALTOZAS, MENTJUK A SETUPOT
JKR6         LCALL   SETKILEP
             SETB    TILT              ; UJRA TILTJUK A 2. GOMBOT
             LJMP    HWEND
JKR2         CJNE    A,#UP,JKR3        ; HA NOVELES, FOLYTATODIK
             CBYTE   KORHIGH,MAXJAT    ; BEOLVASSUK A LEHETSEGES MAX.-OT
             CJNE    A,NUM,JKR7        ; NUM-MAL HASONLITJUK
             LJMP    JKR4
JKR7         MOV     A,NUM
             ADD     A,#1
             DA      A
             MOV     NUM,A
    ;         MOV     A,MAXJAT
    ;         CJNE    A,#22,JKR7_A
    ;         MOV     A,NUM            ; HA BULLMASTER, PAROSAVAL EMELKEDIK
    ;         ADD     A,#1
    ;         DA      A
    ;         MOV     NUM,A
JKR7_A       XBYTEKI JKORMAX,MAXJAT,NUM  ; ADATOT VISSZAIRJUK
             SETB    MINDKI            ; JELEZZUK, HOGY VALTOZAS TORTENT
             LJMP    JKR4
JKR3         CJNE    A,#DOWN,JKR5      ; HA NEM DOWN, UGRIK
             CBYTE   KORLOW,MAXJAT     ; BEHOZZUK A LEHETSEGES MINIMUMOT
             MOV     NUM,A
             XBYTEKI JKORMAX,MAXJAT,NUM  ; ADATOT VISSZAIRJUK
             SETB    MINDKI
             LJMP    JKR4
JKR5         CJNE    A,#CHGAME,JKR8    ; HA NEM JATEKVALTO, UGRIK
             CBYTE   JATSOR,MAXJAT
             MOV     MAXJAT,A
             LJMP    JKR4
JKR8         KEYCLR
             LJMP    JKR4


; 24-  KREDIT EGY JATEKOSRA
; >HW_JATKRED
HW_JATKRED   LCALL    SETBELEP
              MOV     A,BREG
              PLAYER  2,1
              LCALL   SETUPJAT
             BELAM    IZDIN             ; EZ A GOMB VALTJA A JATEKOKAT
             CLR      TILT              ; ITT KELL A 2.GOMB IS !
             MOV      MAXJAT,#1         ; MAXJAT-BAN A JATEK SORSZAMA
JKD4         XRBYTE    JKREDIT,MAXJAT   ; BEOLVASSUK A JELENLEGI MAX.-OT
             MOV     NUM,A              ; KIJELZESHEZ
             MOV     JNUM,#8            ; UTOLSO MEZOBE IRUNK
             MOV     SNUM,#0AH          ; SZAZASOK HELYEN NE LEGYEN SEMMI
             LCALL   PONTSZAM           ; KIIRATAS A KIJELZORE
             MOV     A,MAXJAT
             PLAYER  2,2                ; HOVA KELL KIIRNI A SZOVEGET
             LCALL   SETJATCIM          ; JATEK NEVENEK KIIRATASA
             LCALL   DARTKIJ
JKD1         KEYCLR
             LCALL   READSETKEY
             MOV     A,KEY
             JZ      JKD1
             CJNE    A,#CHANGE,JKD2    ; HA A LEPTETO, KILEPES
             JNB     MINDKI,JKD6
             LCALL   SETUPSAVE         ; HA VOLT VALTOZAS, MENTJUK A SETUPOT
JKD6         LCALL   SETKILEP
             SETB    TILT              ; UJRA TILTJUK A 2. GOMBOT
             LJMP    HWEND
JKD2         CJNE    A,#UP,JKD3        ; HA NOVELES, FOLYTATODIK
             MOV     A,NUM
             CJNE    A,#9,JKD7        ; NUM-MAL HASONLITJUK
             SJMP    JKD4
JKD7         MOV     A,NUM
             ADD     A,#1
             DA      A
             MOV     NUM,A
             XBYTEKI JKREDIT,MAXJAT,NUM ; ADATOT VISSZAIRJUK
             SETB    MINDKI             ; JELEZZUK, HOGY VALTOZAS TORTENT
             SJMP    JKD4
JKD3         CJNE    A,#DOWN,JKD5       ; HA NEM DOWN, UGRIK
             MOV     NUM,#1
             XBYTEKI JKREDIT,MAXJAT,NUM ; ADATOT VISSZAIRJUK
             SETB    MINDKI
             SJMP    JKD4
JKD5         CJNE    A,#CHGAME,JKD8     ; HA NEM JATEKVALTO, UGRIK
             CBYTE   JATSOR,MAXJAT
             MOV     MAXJAT,A
             LJMP    JKD4
JKD8         KEYCLR
             LJMP    JKD4

; 25 -  OSZTOTT BULL-EYE BEALLITAS
; >HW_SPLBULL
HW_SPLBULL   LCALL    SETBELEP
             MOV     A,BREG
             PLAYER  2,1
             LCALL   SETUPJAT
             BELAM    IZDIN             ; EZ A GOMB VALTJA A JATEKOKAT
             CLR      TILT              ; ITT KELL A 2.GOMB IS !
             MOV      MAXJAT,#1         ; MAXJAT-BAN A JATEK SORSZAMA
SPB4         XRBYTE    SPBULL,MAXJAT    ; BEOLVASSUK A JELENLEGI ERTEKET
             MOV      NUM,A             ; NUM-BAN LEGYEN AZ AKT. ERTEK
             ADD      A,#2              ; ELTOLAS A NEVTABLAZATBAN
             LCALL    SETUPMESS         ; ALLAPOT KIIRASA
             MOV     A,MAXJAT
             PLAYER  2,2                ; HOVA KELL KIIRNI A SZOVEGET
             LCALL   SETJATCIM          ; JATEK NEVENEK KIIRATASA
             LCALL   DARTKIJ
SPB1         KEYCLR
             LCALL   READSETKEY
             MOV     A,KEY
             JZ      SPB1
             CJNE    A,#CHANGE,SPB2     ; HA A LEPTETO, KILEPES
             JNB     MINDKI,SPB6
             LCALL   SETUPSAVE          ; HA VOLT VALTOZAS, MENTJUK A SETUPOT
SPB6         LCALL   SETKILEP
             SETB    TILT              ; UJRA TILTJUK A 2. GOMBOT
             LJMP    HWEND
SPB2         CJNE    A,#UP,SPB3        ; HA NOVELES, FOLYTATODIK
             MOV     A,NUM             ; ATTOLTES OSSZEHASONLITASHOZ
             CJNE    A,#0,SPB11        ; HA NEM NULLA, UGRAS
             SJMP    SPB7              ; NOVELJUK 1-EL
SPB11        CJNE    A,#2,SPB7         ; HA NEM 2, AKKOR IS UGRAS
             MOV     NUM,#1            ; UJRA LEGYEN 1 A NUM ERTEKE
             XBYTEKI SPBULL,MAXJAT,NUM
             SETB    MINDKI
             SJMP    SPB4
SPB7         MOV     A,NUM
             ADD     A,#1
             DA      A
             MOV     NUM,A
             XBYTEKI SPBULL,MAXJAT,NUM  ; ADATOT VISSZAIRJUK
             SETB    MINDKI             ; JELEZZUK, HOGY VALTOZAS TORTENT
             SJMP    SPB4
SPB3         CJNE    A,#DOWN,SPB5       ; HA NEM DOWN, UGRIK
             CLR     A                  ; OSZTOTT BULL EYE LEGYEN
             MOV     NUM,A
             XBYTEKI SPBULL,MAXJAT,NUM  ; ADATOT VISSZAIRJUK
             SETB    MINDKI
             LJMP    SPB4
SPB5         CJNE    A,#CHGAME,SPB8     ; HA NEM JATEKVALTO, UGRIK
             CBYTE   JATSOR,MAXJAT
             MOV     MAXJAT,A
             LJMP    SPB4
SPB8         KEYCLR
             LJMP    SPB4


; 26-KRIKETTBEN MINDENKI ZARJON   (* CIMKIIRATASHOZ +5 !!*)
; >HW_KRIKZAR
HW_KRIKZAR    MOV     A,BREG
              PLAYER  2,2
              LCALL   SETUPJAT
              XBYTE   CR_MINDZAR,#0
              MOV     CREG,A
              LCALL   ONEALLBOX
              JNB     MINDKI,KWP1       ; HA NEM VOLT VALTOZAS, NEM MENT
              XBYTEKI CR_MINDZAR,#0,CREG
              LCALL   SETUPSAVE
KWP1          LJMP    HWEND

; 27-TEAM KRIKETT GYOZELEMHEZ ZARNI
; >HW_TKRIKGYOZ
HW_TKRIKGYOZ  MOV     A,BREG
              PLAYER  2,2
              LCALL   SETUPJAT
              XBYTE   CR_GYZAR,#0
              MOV     CREG,A
              LCALL   ONEALLBOX
              JNB     MINDKI,GKWP1       ; HA NEM VOLT VALTOZAS, NEM MENT
              XBYTEKI CR_GYZAR,#0,CREG
           ;  MOV     A,CREG
           ;  LCALL   KIIRAS8
              LCALL   SETUPSAVE
GKWP1         LJMP    HWEND

; 28 - PLUSZ KOROK SZAMA NEHEZITVE
; >HW_PLUSZKOR
HW_PLUSZKOR   LCALL   SETBELEP
              MOV     A,BREG
              PLAYER  2,2             ; HOVA KELL KIIRNI A SZOVEGET
              LCALL   SETUPJAT
              XBYTE   OPT_ROUND,#0
              MOV     NUM,A
HOR4          MOV     JNUM,#8
              MOV     SNUM,#0AH         ; SZAZASOK HELYEN NE LEGYEN SEMMI
              LCALL   PONTSZAM          ; KIIRATAS A KIJELZORE
              LCALL   DARTKIJ
HOR1          KEYCLR
              LCALL   READSETKEY
              MOV     A,KEY
              JZ      HOR1
              CJNE    A,#CHANGE,HOR2
              JNB     MINDKI,HOR6
              LCALL   SETUPSAVE         ; HA VOLT VALTOZAS, MENTJUK A SETUPOT
HOR6          LCALL   SETKILEP
              LJMP    HWEND
HOR2          CJNE    A,#UP,HOR3        ; HA NOVELES, FOLYTATODIK
              MOV     A,NUM
              CJNE    A,#6,HOR7
              SJMP    HOR4
HOR7          ADD     A,#1
              DA      A
              MOV     NUM,A
              XBYTEKI OPT_ROUND,#0,NUM  ; ADATOT VISSZAIRJUK
              SETB    MINDKI            ; JELEZZUK, HOGY VALTOZAS TORTENT
              SJMP    HOR4
HOR3          CJNE    A,#DOWN,HOR5      ; HA NEM DOWN, UGRIK
              CLR     A
              MOV     NUM,A
              XBYTEKI OPT_ROUND,#0,NUM
              SETB    MINDKI
              SJMP    HOR4
HOR5          KEYCLR
              SJMP    HOR4

; 29 - BULLRA DOBAS ENGEDELYEZES
; >HW_FELDOBAS
HW_FELDOBAS   MOV     A,BREG
              PLAYER  2,2
              LCALL   SETUPJAT
              XBYTE   FEL_DOB,#0
              MOV     CREG,A
              LCALL   YESNOBOX
              JNB     MINDKI,FLP1
              XBYTEKI FEL_DOB,#0,CREG
              LCALL   SETUPSAVE
FLP1          LJMP    HWEND

; 30 - NYILELFOGYAS HANGJA
; >HW_NYILFOGY
HW_NYILFOGY   MOV     A,BREG
              PLAYER  2,2
              LCALL   SETUPJAT
              XBYTE   NO_DARTS,#0
              MOV     CREG,A
              LCALL   YESNOBOX
              JNB     MINDKI,NOWP1
              XBYTEKI NO_DARTS,#0,CREG
              LCALL   SETUPSAVE
NOWP1         LJMP    HWEND

; 31-ATLEPES TESZT MODBA
; >HW_TEST
HW_TEST       MOV     A,BREG
              PLAYER  2,2
              LCALL   SETUPJAT
              CLR     A                 ; ALAPHELYZETBEN NEM NULLAZTATUNK
              MOV     CREG,A
              LCALL   YESNOBOX
              MOV     A,CREG
              JZ      HWTS1
              MOV     A,#H_DUPDOB        ; NULLAZASHOZ HANG
              LCALL   H_EFFEKT
              LCALL   LAMSOTET           ; LAMPAK OLTASA
              SETB    P_TEST             ; TEST MODE BEALLITAS
HWTS1         LJMP  HWEND

; 32-ATLEPES JATEK MODBA
; >HW_BYE
HW_BYE        MOV     A,BREG
              PLAYER  2,2
              LCALL   SETUPJAT
              CLR     A                  ; ALAPHELYZETBEN NEM NULLAZTATUNK
              MOV     CREG,A
              LCALL   YESNOBOX
              MOV     A,CREG
              JZ      HWB1              ; HA NEM VOLT VALTOZAS, NEM MENT
              MOV     A,#H_TRIPDOB      ; NULLAZASHOZ HANG
              LCALL   H_EFFEKT
              LCALL   LAMSOTET          ; LAMPAK OLTASA
              SETB    P_JATEK           ; JATEK FAZIS BEALLITAS
HWB1          LJMP  HWEND


; SETUP ALLITAS KOZBENI HANGOK
; BEJOVO ADAT: KEY - MEGNYOMOTT BILLENTYU
H_KEY      PUSH    AREG
           PUSH    B
           MOV     A,KEY
           CJNE    A,#CHGAME,HKY1
           MOV     AREG,#80
           MOV     B,#25
           LCALL   BEEP
           SJMP    HKVEG
HKY1       CJNE    A,#CHANGE,HKY2
           MOV     AREG,#160
           MOV     B,#25
           LCALL   BEEP
           SJMP    HKVEG
HKY2       CJNE    A,#UP,HKY3
           MOV     AREG,#30
           MOV     B,#10
           LCALL   BEEP
           SJMP    HKVEG
HKY3       CJNE    A,#DOWN,HKVEG
           MOV     AREG,#30
           MOV     B,#10
           LCALL   BEEP
HKVEG      CLR     P10
           POP     B
           POP     AREG
           PUSH    KEY
           MOV     AREG,#RESWAIT        ; MEGVARJUK, MIG ELENGEDIK A GOMBOT
HKY6       DELAY   VTIME
HKY9       KEYCLR
           LCALL   SETKEYBE
           JB      FLAGTIM,HKY7         ; A LETELT AZ IDO, KEY ERROR
           JNZ     HKY9		        ; MEGVARJA AMIG ELENGEDJUK
           LJMP    HKY8		        ; UGRAS A VEGERE
HKY7       DJNZ    AREG,HKY6
           LCALL   ERR_KEY
HKY8       LCALL   TIM10MS
           POP     KEY
           RET
