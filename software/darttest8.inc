;****************************************************************
;       Euro-Dart (C) Ringer Attila (C)1995-2023								*
;****************************************************************
; TEST-MODE
; 
; TESZT UZEMMOD RUTINJAI

; KIJELZI AZ AKT. TESZT ALLAPOT NEVET
TCIMIR        MOV      A,BREG
              PLAYER   1,1
              LCALL    TSTCIM
              MOV     A,#M_TES        ; TES FELIRAT
              PLAYER  0,1             ; BAL FELSO KIJELZO
              LCALL   TSTCIM          ; MEGJELENITES
              RET

; TESZT UZEMMOD KERETPROGRAM
; >TESTMODE
TEST_MODE       MOV    DPTR,#MEG_ORZ
                MOVX   A,@DPTR          ; MEGNEZI, KELL-E FLAG
                CJNE   A,#2,TST0        ; NEM KELL, UGRAS

                MOV     DPTR,#ST_FLAG
                MOV     A,#ST_TEST      ; MEGJEGYZI, HOGY TESZT-BEN VAN
                MOVX    @DPTR,A

TST0            CLR     DOBJON
                SETB    EA
                CLR     P_SETUP
                CLR     P_JATEK
                CLR     P_DEMO
                SETB    P_TEST
                CLR     CHAMP           ; NINCS VERSENYALLAPOT
                LCALL   LAMSOTET        ; NYOMOGOMBOK LAMPAINAK OLTASA
                LCALL   LAMPALEC_OFF
                LCALL   BUFINI          ; KIJELZO TORLES
                LCALL   DARTKIJ
                MOV     A,#H_VALTO      ; TESZ MOD BEJELENTKEZO ZENE
                LCALL   H_EFFEKT
                LCALL   WAIT_NOKEY      ; MEGVARJA MIG ELENGEDIK A GOMBOT
                MOV       BREG,#1       ; BREG SZAMOLJA A JATEKFAZIST
                BELAM     IZVALTO       ; VALTOGOMB LAMPAJA  EGJEN
                KEYCLR                  ; KEZDETI BILL. NYOMAS TORLES
TMT2            LCALL     TCIMIR
                LCALL     JUMPFAZIS     ; AZ EGYES FAZISPROGRAMOK
                MOV       A,#H_JODOB    ; HANGADAS
                LCALL     H_EFFEKT
                LCALL     WAIT_NOKEY    ; MEGVARJA, MIG ELENGEDIK A GOMBOT
                MOV       A,BREG
                CJNE      A,#11,TMT1    ; HA ELERTE A 10-ET, UJRA 1
                MOV       BREG,#1
                SJMP      TMT2          ; VISSZA AZ ELEJERE
TMT1            ADD       A,#1
              ;  DA       A
                MOV       BREG,A        ; BREG NOVELESE
                SJMP      TMT2          ; VISSZA AZ ELEJERE
                RET                     ; CSAK KIKAPCSOLASSAL VAN VEGE !!!

; >JUMPFAZIS
JUMPFAZIS       MOV    DPTR,#FAZRUT
                MOV    A,BREG
                DEC    A
                RL     A
                RL     A
                JMP    @A+DPTR           ; UGRAS ERRE A CIMRE
TESTVEGE        RET                      ; IDE VARJUK VISSZA AZ UGRAST

FAZRUT          LJMP      T_SOUND
                NOP
                LJMP      T_SPEAK
                NOP
                LJMP      T_INFRA
                NOP
                LJMP      T_SENSOR
                NOP
                LJMP      T_TARGET
                NOP
                LJMP      T_LIGHT
                NOP
                LJMP      T_BUTTON
                NOP
                LJMP      T_COIN
                NOP
                LJMP      T_DISPLAY
                NOP
                LJMP      T_PROGRAM
                NOP
                LJMP      T_EXIT


MEHA            DB     0
                DB     H_TRIPDOB
                DB     H_HIGHEST
                DB     H_FIVEJO
                DB     H_FIVENEM
                DB     H_VALTO
                DB     H_BULL50


; HANGSZORO TESZTELES
; AZ A-REGNEK MEGFELELO HANGOT INDITJA EL
; >MELYHANG
MELYHANG         PUSH   JNUM
                 PUSH   BREG
                 PUSH   AREG
                 MOV    DPTR,#MEHA      ; A-BAN A HANG SORSZAMA
                 MOVC   A,@A+DPTR
                 LCALL  H_EFFEKT
                 POP     AREG
                 POP     BREG
                 POP     JNUM
                 RET

T_SOUND          DELAY   10
TSPR1            JB      FLAGTIM,TSPR2    ; 1 SEC-IG NEM KEZDI EL A HANGOKAT
                 LCALL   V_BILL
                 SJMP    TSPR1
TSPR2            MOV    JNUM,#1          ;
TSOU3            MOV    A,JNUM
                 LCALL  MELYHANG
                 LCALL  TTSOU
                 MOV    A,JNUM
                 CJNE   A,#6,TSOU4
                 MOV    JNUM,#1         ; UJRA ALAPERTEK
                 PUSH  B
                 MOV   B,#B_THANK       ; HANG
                 LCALL B_PLAYER
                 POP   B
                 LCALL  TIM500MS
                 LCALL  TIM500MS
                 SJMP   TSOU3
TSOU4            MOV    A,JNUM
                 ADD    A,#1
                 MOV    JNUM,A
                 SJMP   TSOU3

SOUND_DEL        EQU    10
TTSOU            DELAY  SOUND_DEL
TSOU2            JB     FLAGTIM,TSOU1   ; HA LETELT AZ IDO, VISSZA
                 LCALL  VALKEY          ; MEGNYOMTAK A VALTOGOMBOT ?
                 JZ     TSOU2           ; NEM, VISSZA
                 POP    DPH
                 POP    DPL             ; A HIVASI CIMET TOROLJUK
                 LJMP   TESTVEGE        ; KILEPHET
TSOU1            RET                    ; VISSZATERES A HIVO RUTINBA

; BESZEDHANG ELLENORZES
T_SPEAK      ;  MOV     A,#9        ;   SPE FELIRAT
             ;  PLAYER  1,5
             ;  LCALL   SETUPJAT
                XBYTEKI  SPEAK_EN,#0,#1   ; BESZED ENGEDELYEZES
                BELAM   IZVALTO
                PUSH   B
T_SPE           MOV   B,#1               ; HANG SORSZAMA
T_SP1           PUSH  B
                LCALL B_PLAYER
                SETB  EOM             ; MAJD AZ IC HOZZA LE L-BE
                POP   B
                LCALL TIM100MS
T_SPW           LCALL VALKEY
                JNZ   T_SPV
                JB    EOM,T_SPW       ; MEGVARJUK A LEFUTO ELT
                LCALL TIM100MS
                INC   B               ; SZOVEG SORSZAM NOVELES
; T_SPW2        JNB   P15,T_SPW2      ; MEGVARJUK A FELFUTO ELT
;               LCALL TIM10MS         ;
                MOV   A,B             ; ATTOLTES A-BA
                CJNE  A,#30,T_SPW3    ; HA ELERTE, KILEPHET
                MOV   B,#32           ; AT KELL UGORNI KET UZENETET
                SJMP  T_SP1
T_SPW3          CJNE  A,#51,T_SP1     ; HA ELERTE, KILEPHET
                MOV     A,#H_JODOB    ; EGYSZER KORBEERT
                LCALL   H_EFFEKT
                LJMP  T_SPE           ; VISSZA A SZOVEGTESZT ELEJERE
T_SPV           POP   B
                LJMP  TESTVEGE

; INFRA ERZEKELO TESZTELES
T_INFRA       FENYBE
              INFEN                  ; INFRA ERZEKELO ENGEDELYEZES
              XBYTEKI INF_MODE,#0,#1  ; INFRA UZEMMOD ALLITAS
T_INF1        DELAY    VLTIME
              SETB    SVILLOG              ; A VILLOGAS EGESSEL KEZD
              KILAM   IZVALTO
TINFRA1       JNB     FLAGTIM,TINF22       ; HA MEG NEM TELT LE, UGRIK
              CPL     SVILLOG
	      JB     SVILLOG,TINF2B         ; OLTASRA UGRAS
	      BELAM   IZVALTO
              DELAY   VLTIME
	      SJMP    TINF22
TINF2B	      KILAM   IZVALTO           ; LEPTETOGOMB LAMPA KIKAPCS
	      DELAY   VLTIME
TINF22      ;  LCALL   COIN
              KEYCLR
              LCALL   VALKEY            ; NYOMOGOMB OLVASAS
              JZ      TINF1             ; HA KEY = 0, NEZI AZ INFRAT
              SJMP    TINF2            ; HA MEGVAN, UGRAS
              KEYCLR         ; ESEMENY TORLESE
TINF1         LCALL    INFRABE          ; INFRAJEL BEOLVASAS
              JNB      INFJEL,TINFRA1   ; HA NINCS JEL, VISSZA AZ ELEJERE
              BELAM    IZBULLEYE
TINF3         LCALL    INFRABE          ; INFRAJEL BEOLVASAS
              JB      INFJEL,TINF2A     ; HA ELMULT AZ INFRAJEL, KILEP
              KILAM   IZBULLEYE
TINF2A        KILAM    IZVALTO          ; INFRA EREZ, VALTOGOMB KIKAPCS
           ;  LCALL    COIN
              LCALL   VALKEY            ; NYOMOGOMB OLVASAS
              JZ      TINF5             ; HA KEY = 0, NEZI AZ INFRAT
              SJMP    TINF2
TINF5         JB      INFJEL,TINF3    ; AMIG C=1, VARJA A LEFUTO ELET
              SJMP    T_INF1          ; KILEPES HELYETT VISSZA
TINF2         KEYCLR                   ; GOMB ESEMENY TORLESE, KILEPES
              KILAM    IZVALTO
              KILAM   IZBULLEYE
              FENYKI
              INFDIS                 ; INFRA ERZEKELO LETILTAS
              LJMP   TESTVEGE        ; KILEPHET


; UTES ERZEKELO TESZTELES
T_SENSOR         FENYBE
                 BELAM   IZVALTO        ; VALTOGOMB LAMPAJA  EGJEN
UT3A             LCALL  UTESBE
                 JNC    UT3              ; UGRIK, HA C<>1
                 BELAM  IZNOSCORE1
                 BELAM  IZNOSCORE2
                 MOV     A,#H_FALSE
                 LCALL   H_EFFEKT
                 KILAM   IZNOSCORE1
                 KILAM   IZNOSCORE2
UT3              LCALL   VALKEY
                 JZ      UT3A        ; HA NEM A VALTOGOMB, VISSZA AZ ELEJERE
UT2              KILAM   IZNOSCORE1
                 KILAM   IZNOSCORE2
                 FENYKI
                 LJMP    TESTVEGE

; VAR, AMIG ELENGEDIK A NYOMOGOMBOKAT
; >WAIT_NOKEY
WAIT_NOKEY      PUSH    A
WNK1            BUTTONMSB               ; HA VAN VALAMI NYOMVA, VISSZA
                JNZ      WNK1
                BUTTONLSB
                JNZ      WNK1
                MOV      KEY,#0        ; KILEPHET, ELENGEDTEK
                LCALL    TIM30MS       ; PRELLMENTESITES
                POP      A
                RET

; CELTABLA TESZTELES
; >T_TARGET
T_TARGET        PUSH    BREG
                MOV     BREG,#GAME_SHANGHAI
                BELAM   IZVALTO        ; VALTOGOMB LAMPAJA  EGJEN
                CLR     ELVESZNYIL
                CLR     ROSSZDOB             ; EZEK A BUTEK LEGYENEK TOROLVE
TTA1            MOV     CTPONT,#0
                LCALL   DOBERT1
                MOV     A,CTPONT
                JZ      TTA2
                MOV     NUM,A
                MOV     JNUM,#9
                MOV     A,DREG            ; DREG ALAPJAN BETUIRAS
                CJNE    A,#1,TTA3         ; HA NEM SZIMPLA, UGRAS
                MOV     SNUM,#0AAH
                SJMP    TTA4
TTA3            CJNE    A,#2,TTA5         ; HA NEM SZIMPLA, UGRAS
                MOV     SNUM,#0CH
                SJMP    TTA4
TTA5            CJNE    A,#3,TTA6         ; HA NEM SZIMPLA, UGRAS
                MOV     SNUM,#0BH
                SJMP    TTA4
TTA6            MOV     SNUM,#0AH
TTA4            LCALL   PONTSZAM
                LCALL   DARTKIJ
                LCALL   HANGAD
TTA2            KEYCLR
                LCALL   VALKEY
                JZ      TTA1
                LCALL   BUFINI
                LCALL   DARTKIJ
                POP    BREG
                LJMP   TESTVEGE        ; KILEPHET

; NYOMOGOMB TESZTELES
T_BUTTON
TBUT1            BUTTONLSB              ; ALSO BILL. NYOLCAS OLVASAS
                 WRITE1
                 BUTTONMSB
                 CJNE     A,#40H,TBT3   ; HA A VALTOGOMB, KILEP
                 LCALL    VALKEY
                 JZ       TBT3          ; MEG EGYSZER RAOLVASUNK
                 BELAM    IZVALTO
                 LJMP     TESTVEGE
TBT3             ORL      A,#40H        ; A VALTOGOMB MINDIG EGJEN
                 WRITE2
            ;    BELAM  IZVALTO
                 LJMP  TBUT1


LAM_SOR         DB  0,        IZGOMB16,  IZGOMB1
                DB  IZGOMB2,  IZGOMB3,   IZGOMB4, IZGOMB5
                DB  IZGOMB6,  IZGOMB7,   IZGOMB8, IZNOSCORE2
                DB  IZSZABAD, IZBULLEYE, IZTILOS, IZNOSCORE1

; LAMPA TESZTELES
T_LIGHT
T_LIG2           CLR    LEPHET
                 PUSH  B
                 MOV   B,#14            ; 14 LAMPAT KELL TESZTELNI
TLI1A            MOV   DPTR,#LAM_SOR    ; A SORREND TERULET KEZDUCIME
                 MOV   A,B
                 MOVC  A,@A+DPTR        ; BEOLVASSUK A KOVETKEZO LAMPAT
                 PUSH  A                ; LAMPA SORSZAM MENTESE
                 LCALL   IZZOK_BE       ; MEGADOTT SORSZAMU LAMPA KIGYUL
                 LCALL  TLI_DEL         ; IDOZITES, VALTOGOMBRA KILEP
                 POP    A               ; LAMPA SORSZAM ELOJOJJON
                 JB     LEPHET,T_LIGV1  ; KILEP, HA KI KELL
                 LCALL  IZZOK_KI        ; LAMPA ELALSZIK
                 DJNZ   B,TLI1A         ; HA MEG VAN LAMPA, VISSZA
                 POP    B

                 LCALL  TLI_DEL          ; MINDEN LAMPA KIGYUL
                 JB     LEPHET,T_LIGV
                 MOV    A,#0FFH
                 WRITE1
                 MOV    A,#0FFH
                 WRITE2
                 LCALL  TLI_DEL         ; MINDEN LAMPA ELALSZIK
                 JB     LEPHET,T_LIGV
                 LCALL  TLI_DEL
                 JB     LEPHET,T_LIGV
                 CLR    A
                 WRITE1
                 CLR    A
                 WRITE2
                 BELAM  IZVALTO         ; A VALTOGOMB MINDIG MARADJON EGVE
                 LCALL  TLI_DEL
                 JB     LEPHET,T_LIGV
                 LJMP   T_LIG2

T_LIGV1          POP    B
T_LIGV           LJMP   TESTVEGE


;>TLI_DEL
TLI_DEL          DELAY  5               ; 5 TIZED SEC KESLELTETES
TLI2             JB     FLAGTIM,TLI1
                 LCALL  VALKEY
                 JZ     TLI2
                 CLR    A
                 MOV    SL1DATA,A
                 WRITE1
                 CLR   A
                 MOV    SL2DATA,A
                 WRITE2
                 BELAM  IZVALTO
                 SETB   LEPHET          ; KILEPHET BIT BEALLITASA
TLI1             RET

; ERMEVIZSGALO TESZTELES
TCOI23B         LJMP    TCOI23          ; SEGEDUGRO CIM
TCOI22A     ;   LJMP    TCOI22
                LCALL  VALKEY           ; ERME NINCS, HA VALTOGOMB, KILEPES
                JNZ    TCOI23B           ; HA VALTOGOMB, KILEPHET
                SJMP    TCOI21           ; EGYEBKENT NEM KELL IRNI
T_COIN
                PUSH    A
                PUSH    PSW
            ;   CLR     VANGYOZ         ; BELSO SEGEDVALTOZO

                MOV     A,#7            ; ALSO VONAL MEGJELENITES
                PLAYER  2,3             ;
                LCALL   SETUP_CH        ;
                MOV     A,#7            ; ALSO VONAL MEGJELENITES
                PLAYER  2,4             ;
                LCALL   SETUP_CH        ;

TCOI21          READ0                   ; ERMEVIZSG.OLVASAS
                CPL     A               ; INVERTALAS
                ANL     A,#COIN_MASK    ; EGYEB JELEK TORLESE
                JZ      TCOI22A          ; HA NINCS JEL, UGRAS
                LCALL   TIM30MS         ; VOLT JEL, PRELLMENTESITES
                READ0                   ; ERMEVIZSGALO OLVASAS
                CPL     A               ; INVERTALAS
                ANL     A,#COIN_MASK    ; EGYEB JELEK TORLESE
                JZ      TCOI22A          ; HA MOST NINCS JEL, HAZARD VOLT

                RRC     A               ; JOBBRA KILEPTETJUK A CARRY-N AT
                JNC     COI_D1          ;
                JB      MECH_SEL,COI_D1A  ; HA MECH., UGRAS
                MOV     B,#5            ; CH5 JELE
                SJMP    COI_C
COI_D1A         MOV     B,#1            ; MECHANIKUS ESETEN CH1 LESZ BELOLE
                SJMP    COI_C           ; CH1 JELE
COI_D1          RRC     A               ;
                JNC     COI_D2          ;
                JB      MECH_SEL,COI_D4A  ; HA MECH., UGRAS
                MOV     B,#1            ; CH1 JELE
                SJMP    COI_C
COI_D4A         MOV     B,#2            ; MECHANIKUSNAL CH2 LESZ
                SJMP    COI_C
COI_D2          RRC     A               ;
                JNC     COI_D3          ;
                MOV     B,#3            ; CH3 JELE
                SJMP    COI_C
COI_D3          RRC     A               ;
                JNC     COI_D4          ;
                MOV     B,#4            ; CH4 JELE
                SJMP    COI_C
COI_D4          RRC     A               ;
                JNC     COI_D5          ;
                MOV     B,#2            ; CH5 JELE
                SJMP    COI_C
COI_D5          RRC     A               ;
                JNC     T_COI6A         ;
                MOV     B,#6            ; CH6 JELE

COI_C           MOV     A,B             ; CSATORNASZAM KIIRATAS
                PLAYER  2,1             ; BAL FELSO KIJELZO, ALSO TABLA
                LCALL   SETUP_CH        ; MEGJELENITES
                MOV     A,B             ; CSATORNASZAM KIIRATAS
                PLAYER  2,2             ; BAL FELSO KIJELZO, ALSO TABLA
                LCALL   SETUP_CH        ; MEGJELENITES
                MOV     A,#8            ; FELSO VONAL MEGJELENITES
                PLAYER  2,3             ;
                LCALL   SETUP_CH        ;
                MOV     A,#8            ; FELSO VONAL MEGJELENTES
                PLAYER  2,4             ;
                LCALL   SETUP_CH        ;

                MOV     A,#H_PENZBE
                LCALL   H_EFFEKT        ; PENZBEDOBAS HANGJA
                LCALL   DARTKIJ
                LCALL   TIM100MS        ; KESLELTETES
           ;    LCALL   TIM100MS
                LCALL   TIM100MS

T_COI6A         DELAY   MVTIME          ; TIME OUT INICIALIZALAS
T_COI6          JB      FLAGTIM,T_COI8  ; HA LETELIK AZ IDO, ERMEVIZSG. HIBA!
                LCALL   TIM30MS         ; MEGVARJUK A JEL VEGET
                READ0                   ; ERMEVIZSGALO OLVASAS
                CPL    A                ; INVERTALAS
                ANL    A,#COIN_MASK     ; EGYEB JELEK TORLESE
                JNZ    T_COI6           ; HA MEG TART A JEL, TOVABB
TCOI22          LCALL  VALKEY           ; HA VALTPGOMB, KILEPES
                JNZ    TCOI23           ; HA NEM A VALTOGOMB, VISSZA
TCOI22B         MOV     A,#7            ; ALSO VONAL MEGJELENITES
                PLAYER  2,3             ;
                LCALL   SETUP_CH        ;
                MOV     A,#7            ; ALSO VONAL MEGJELENITES
                PLAYER  2,4             ;
                LCALL   SETUP_CH        ; ERME JEL VEGE
                LJMP    TCOI21          ; VISSZA A CIKLUS ELEJERE

TCOI23          POP    PSW              ; VALTOGOMB MEGNYOMVA, KILEPES
                POP    A
                LJMP   TESTVEGE        ; KILEPHET

T_COI8          BELAM   IZBULLEYE
                LCALL   ERRMSG
                MOV     A,#H_PENZBE
                LCALL   H_EFFEKT        ; HIBAJELZO PITTY
                MOV     B,#B_ERROR
                LCALL   B_PLAYER
                LCALL   TIM500MS
                LCALL   TIM100MS
                LCALL   TIM100MS
                MOV     B,#B_BADCOIN
                LCALL   B_PLAYER
T_COI8A         READ0                   ; ERMEVIZSGALO OLVASAS
                CPL    A               ; INVERTALAS
                ANL    A,#COIN_MASK    ; EGYEB JELEK TORLESE
                JZ     T_COI8B         ; HA MAR INCS JEL, KILEPHET
                LCALL   TIM500MS
                KILAM    IZBULLEYE
                MOV     A,#H_PENZBE
                LCALL   H_EFFEKT        ; HIBAJELZO PITTY
                READ0                   ; ERMEVIZSGALO OLVASAS
                CPL    A               ; INVERTALAS
                ANL    A,#COIN_MASK    ; EGYEB JELEK TORLESE
                JZ     T_COI8B         ; HA MAR INCS JEL, KILEPHET
                LCALL   TIM500MS
                BELAM    IZBULLEYE
                MOV     A,#H_PENZBE
                LCALL   H_EFFEKT        ; HIBAJELZO PITTY
                SJMP    T_COI8A
T_COI8B         KILAM   IZBULLEYE
                LCALL   TCIMIR
                LJMP    TCOI21           ; STACK VISSZAALLITAS, KILEPES

; szamjegy kiirato segedprogram
; >SZAMJEGY
SZAMJEGY        PUSH  DREG
                PUSH  B
                PUSH  SNUM
                PUSH  NUM
                PUSH  JNUM
                PUSH  AKTKOR
                XBYTE SZJEGY,#0
                ANL   A,#0FH
                MOV   DREG,A
                MOV   SNUM,A            ; SZAZASOK ELOKESZITESE
                MOV   NUM,A             ; TIZESEKBE IS
                SWAP  A                 ; FELSO NEGYESBE IS
                ORL   NUM,A             ; VAGY-OLAS A NUM-MAL
                MOV   AKTKOR,A          ; AKTKOR IS LEGYEN ENNYI
                MOV   JNUM,#9
SZJ1            LCALL PONTSZAM          ; MINDEN JATEKOSRA MEGHIVJUK
                DJNZ  JNUM,SZJ1
                LCALL   KORKIJ
          ;     LCALL   DARTKIJ
                MOV   A,DREG
                ADD   A,#1
                MOV   DREG,A
                CJNE  A,#10,SZJ2        ; HA NEM ERTE EL A TIZET, UGRAS
                MOV   DREG,#0              ; ELERTE, UJRA LEGYEN NULLA
SZJ2            XBYTEKI   SZJEGY,#0,DREG
                POP     AKTKOR
                POP     JNUM
                POP     NUM
                POP     SNUM
                POP     B
                POP     DREG
                RET

;********************
; DISPLAY TESZT RUTINJAI
D_KEY       DELAY   5              ; 5 TIZED A FAZISOK KOZOTT
D_KP1       JB   FLAGTIM,D_K1      ; HA LETELT, KILEPHET
            KEYCLR
            LCALL   CURKEY          ; RAOLVAS A BILLENTYUZETRE
         ;  LCALL   KEYBE
            JZ      D_KP1             ; HA NINCS, VISSZA
         ;  LJMP    D_K1
         ;  MOV     A,KEY
            CJNE    A,#IZVALTO,D_KP2  ; HA VALTOGOMB, KILEPHET A TESZT-BOL
            SETB    LEPHET            ; KILEPHET BIT BEALLITASA
            SJMP    D_K1              ; KILEPES
D_KP2       CJNE    A,#IZGOMB16,D_KP1
            LCALL   BUF_DUMMY         ; MINDENT KIGYUJT
D_KP3       KEYCLR
            LCALL   KEYPRESSED        ; BILLENYTURE OLVAS
            JNZ     D_KP3             ; AMIG EL NEM ENGEDJUK, OLVAS
            LCALL  BUFINI             ; VISSZATERESKOR KEPERNYO TORLES
            LCALL  DARTKIJ
D_K1        RET

; OSZLOPVILLOGTATO TESZT LEPES
; >T_COLDEMO
T_COLDEMO     PUSH     NUM
              PUSH     DREG
              CLR      LEPHET           ; ALAPHELYZETBEN NE LEPJEN KI
              MOV      DREG,#1          ; FUGGOLEGES LEDSOR SZALAD BEFELE
TCLD1         MOV      NUM,#7           ; KIGYUJTAS
              LCALL    LEDCOL
              LCALL    SZAMJEGY         ; SZAMJEGY KIIRATAS, NOVELES
              LCALL    DARTKIJ
              LCALL    D_KEY
              JB       LEPHET,TCOL1V
              MOV      NUM,#0          ; ELOLTAS
              LCALL    LEDCOL
              INC      DREG
              MOV      A,DREG
              CJNE     A,#8,TCLD1
              LCALL    DARTKIJ         ; BELSO OSZLOP EGETT UTOLJARA
TCOL1V        POP      DREG
              POP      NUM
              RET

; >T_ROWDEMO
T_ROWDEMO
              MOV      DREG,#1
T_ROD1        MOV      A,#1          ; A SOR KIGYULLAD
              LCALL    LEDROW
              LCALL    SZAMJEGY
              LCALL    DARTKIJ
              LCALL    D_KEY
              JB       LEPHET,TROW1_V
              CLR      A           ; A SOR ELALSZIK
              LCALL    LEDROW
              MOV      A,DREG
              ADD      A,#1
              MOV      DREG,A
              CJNE     A,#13,T_ROD1
              LCALL    DARTKIJ
TROW1_V       RET

; >BUF_DUMMY
BUF_DUMMY
          CLR   EA
          PUSH  NUM
          PUSH  SEG
          PUSH  JNUM
          PUSH  B
          PUSH  DPL
          PUSH  DPH
          MOV   R0,#DF_BUF
          MOV   A,#0FFH
          MOV   B,#36
TN_B1     MOV   @R0,A
          INC   R0
          DJNZ  B,TN_B1
          MOV   R0,#DA_BUF
          MOV   A,#FFH
          MOV   B,#36
TN_B2     MOV   @R0,A
          INC   R0
          DJNZ  B,TN_B2
          MOV     JNUM,#8
TBI3      MOV	NUM,#1
	  MOV	SEG,#1
  	  LCALL	LAMPA
	  MOV	SEG,#0
          LCALL	LAMPA
          MOV   SEG,#2
          LCALL LAMPA
          DJNZ    JNUM,TBI3
          LCALL   DARTKIJ
	  POP	DPH
	  POP	DPL
	  POP	B
          POP     JNUM
	  POP	SEG
          POP	NUM
          SETB  EA
          RET

; KIJELZO TESZTELES
;>T_DISPLAY
T_DISPLAY
                BELAM   IZMAS           ; MASTOUT LAMPA GYUJTASA
                CLR     LEPHET          ; ALAPHELYZETBEN NE LEPJEN KI
                MOV     DREG,#0
                XBYTEKI   SZJEGY,#0,DREG
T_DI1           LCALL	BUFINI          ; KIJELZO TORLES
                LCALL   DARTKIJ         ; KEPERNYO TORLES
                JB      LEPHET,T_DIV    ; KILEPJEN ?
                LCALL   T_COLDEMO       ; OSZLOPONKENT
                JB      LEPHET,T_DIV    ; KILEPJEN ?
                LCALL   T_ROWDEMO       ; SORONKENT
                JB      LEPHET,T_DIV    ; KILEPJEN ?
                ADNYIL  1
                LCALL   SZAMJEGY
                LCALL   D_KEY
                JB      LEPHET,T_DIV    ; KILEPJEN ?
                ADNYIL  2
                LCALL   SZAMJEGY
                LCALL   D_KEY
                JB      LEPHET,T_DIV    ; KILEPJEN ?
                ADNYIL  3
                LCALL   SZAMJEGY
                LCALL   D_KEY
                JB      LEPHET,T_DIV    ; KILEPJEN ?
                ADNYIL  0
                LCALL   D_KEY
                JB      LEPHET,T_DIV    ; KILEPJEN ?
;               SJMP    T_DI1           ; VISSZA A CIKLUS ELEJERE

                MOV     JNUM,#1         ; 1. JATEKOSTOL KEZDVE
T_DI2           MOV     NUM,#3          ; LED-TOMBOK KIGYJTASA
                LCALL   TOMBLED
                LCALL   SZAMJEGY
                MOV     SEG,#2          ; CRICKET LAMPA CIME
                MOV     NUM,#0          ; KIGYULLADJON
                LCALL   LAMPA
                FLAMBE  JNUM            ; EBBEN BENNE LESZ A DARTKIJ
                LCALL   D_KEY
                INC     JNUM
                MOV     A,JNUM
                CJNE    A,#9,T_DI2
                LJMP    T_DI1           ; KORBE-KORBE KIJELZO TESZT

T_DIV           LCALL   BUFINI    ; KIJELZO TORLES
                LCALL   DARTKIJ
                KILAM   IZMAS
                LJMP    TESTVEGE         ; VEGE, KILEPHET


; RAM ES ROM TESZTELES
MEM_MAX      EQU      7FFFH

T_PROGRAM        MOV     A,#M_TES           ; TES FELIRAT
                 PLAYER  0,1             ; BAL FELSO KIJELZO
                 LCALL   TSTCIM          ; MEGJELENITES
                 LCALL   DARTKIJ
                 DELAY   10
TPR6             JB      FLAGTIM,TPR5
                 LCALL   V_BILL
                 SJMP    TPR6
TPR5             MOV     A,#H_FIVEJO
                 LCALL   H_EFFEKT
TPR4             MOV    DPTR,#MEM_MAX    ; MAXIMALIS MEMORIACIM BETOLTESE
                 MOV    DREG,#0         ; BLOKK SZAMLALO
TPR3             MOV    NUM,DREG
                 MOV    SNUM,#0AAH
                 MOV    JNUM,#9
                 LCALL  PONTSZAM
            ;    SETB   KIJVALT
                 LCALL  DARTKIJ
                 MOV    AREG,#2
TPR2             MOV    B,#0FFH          ; CIKLUSSZAMLALO
TPR1             MOVX   A,@DPTR
                 PUSH   A               ; IDOLEGES TAROLAS
                 MOV    A,#55H           ; PROBA PATTERN
                 MOVX   @DPTR,A
                 CLR    A                ; ACC. TORLESE
                 MOVX   A,@DPTR          ; ELLENORZES
                 CJNE   A,#55H,M_HIBA    ; MEMORIA HIBA !!!
                 MOV    A,#0AAH           ; PROBA PATTERN
                 MOVX   @DPTR,A
                 CLR    A                ; ACC. TORLESE
                 MOVX   A,@DPTR          ; ELLENORZES
                 CJNE   A,#0AAH,M_HIBA    ; MEMORIA HIBA !!!
                 POP    A                ; VISSZATOLTES
                 MOVX   @DPTR,A          ;
                 DJNZ   B,TPR1
                 DJNZ   AREG,TPR2       ; KULSO CIKLUS
                 LCALL  V_BILL
                 MOV    A,DREG
                 ADD    A,#1
                 DA     A
                 MOV    DREG,A          ; VISSZA A DREG-BE
                 CJNE   A,#64H,TPR3      ; AMIG NEM ERI EL, VISSZA
                 MOV     A,#H_VALTO          ; OK HANG
                 LCALL   H_EFFEKT
                 LJMP   TPR4

; ATLEPES JATEK MODBA
; >T_EXIT
T_EXIT           MOV     A,#M_TES           ; TES FELIRAT
                 PLAYER  0,1             ; BAL FELSO KIJELZO
                 LCALL   TSTCIM          ; MEGJELENITES
                 LCALL   DARTKIJ
                 BELAM   IZMAS
              BELAM   IZVALTO
              MOV     A,#H_KISZHANG        ; JELZI, HOGY KI LEHET LEPNI
              LCALL   H_EFFEKT
TEX2          KEYCLR
              LCALL   KEYBE
              MOV     A,KEY
              JZ      TEX2              ; VARUNK EGY BILLENTYUT
              CJNE    A,#IZVALTO,TEX3
              SJMP    TEX1              ; VALTOGOMB, KILEPHETUNK
TEX3          CJNE    A,#IZMAS,TEX2        ; HA SZELSO GOMB, JATEK !!
              MOV     A,#H_TRIPDOB         ; NULLAZASHOZ HANG
              LCALL   H_EFFEKT
              LCALL   LAMSOTET          ; LAMPAK OLTASA
              SETB    P_JATEK           ; UGRAS AZ ELEJERE
 ;            LCALL  SETUPLOAD          ; ADATOK BETOLTESE
              KEYCLR
       ;       POP   DPL                 ; ATUGORJUK A HIVOT
       ;       POP   DPH
       ;       POP   BREG                ; ATUGRAS JATEK MODBA
              CLR   P_TEST              ; TESZT UZEMMED VEGE
       ;       RET
               LJMP   MELEGINDIT        ; UJRAINDITJA A GEPET
TEX1          KEYCLR
              KILAM   IZMAS
              LJMP    TESTVEGE

M_HIBA        MOV     A,#H_FALSE
              LCALL   H_EFFEKT
              LJMP   TESTVEGE
