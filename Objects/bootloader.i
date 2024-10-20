
#line 1 "Bootloader\src\bootloader.c" 









 
 
  
#line 1 ".\Bootloader\inc\bootloader.h" 





 
  
#line 1 "\Users\17263\Desktop\AM32_Bootloader_STC32\Bootloader\inc\main.h" 
 
 
 
  
#line 1 "\Users\17263\Desktop\AM32_Bootloader_STC32\Bootloader\inc\nstdint.h" 
 
 
 
 typedef unsigned char uint8_t;
 typedef unsigned int uint16_t;
 typedef unsigned long uint32_t;
 typedef char 		int8_t;
 typedef int 		int16_t;
 typedef long 		int32_t;
 
 
#line 4 "\Users\17263\Desktop\AM32_Bootloader_STC32\Bootloader\inc\main.h" 
 
  
#line 1 "D:\Compiler\Keil_v5\C251\Inc\intrins.h" 






 
 
 #pragma SAVE
 #pragma PARM251
 
 
 #pragma FUNCTIONS(STATIC)
 
 
 
 extern void          _nop_     (void);
 extern bit           _testbit_ (bit);
 extern unsigned char _cror_    (unsigned char, unsigned char);
 extern unsigned int  _iror_    (unsigned int,  unsigned char);
 extern unsigned long _lror_    (unsigned long, unsigned char);
 extern unsigned char _crol_    (unsigned char, unsigned char);
 extern unsigned int  _irol_    (unsigned int,  unsigned char);
 extern unsigned long _lrol_    (unsigned long, unsigned char);
 extern unsigned char _chkfloat_  (float x)  reentrant;
 extern unsigned char _chkdouble_ (double x) reentrant;
 
 #pragma RESTORE
#line 5 "\Users\17263\Desktop\AM32_Bootloader_STC32\Bootloader\inc\main.h" 
 
  
#line 1 "D:\Compiler\Keil_v5\C251\Inc\STC\STC8051U.H" 
 
 
 
 
 
 sfr         P0          =           0x80;
 sbit    P00         =           P0^0;
 sbit    P01         =           P0^1;
 sbit    P02         =           P0^2;
 sbit    P03         =           P0^3;
 sbit    P04         =           P0^4;
 sbit    P05         =           P0^5;
 sbit    P06         =           P0^6;
 sbit    P07         =           P0^7;
 
 sfr         SP          =           0x81;
 sfr         DPL         =           0x82;
 sfr         DPH         =           0x83;
 sfr         DPXL        =           0x84;
 sfr         SPH         =           0x85;
 
 sfr         PCON        =           0x87;
 sbit    SMOD        =           PCON^7;
 sbit    SMOD0       =           PCON^6;
 sbit    LVDF        =           PCON^5;
 sbit    POF         =           PCON^4;
 sbit    GF1         =           PCON^3;
 sbit    GF0         =           PCON^2;
 sbit    PD          =           PCON^1;
 sbit    IDL         =           PCON^0;
 
 sfr         TCON        =           0x88;
 sbit    TF1         =           TCON^7;
 sbit    TR1         =           TCON^6;
 sbit    TF0         =           TCON^5;
 sbit    TR0         =           TCON^4;
 sbit    IE1         =           TCON^3;
 sbit    IT1         =           TCON^2;
 sbit    IE0         =           TCON^1;
 sbit    IT0         =           TCON^0;
 
 sfr         TMOD        =           0x89;
 sbit    T1_GATE     =           TMOD^7;
 sbit    T1_CT       =           TMOD^6;
 sbit    T1_M1       =           TMOD^5;
 sbit    T1_M0       =           TMOD^4;
 sbit    T0_GATE     =           TMOD^3;
 sbit    T0_CT       =           TMOD^2;
 sbit    T0_M1       =           TMOD^1;
 sbit    T0_M0       =           TMOD^0;
 
 sfr         TL0         =           0x8a;
 sfr         TL1         =           0x8b;
 sfr         TH0         =           0x8c;
 sfr         TH1         =           0x8d;
 
 sfr         AUXR        =           0x8e;
 sbit    T0x12       =           AUXR^7;
 sbit    T1x12       =           AUXR^6;
 sbit    S1M0x6      =           AUXR^5;
 sbit    T2R         =           AUXR^4;
 sbit    T2_CT       =           AUXR^3;
 sbit    T2x12       =           AUXR^2;
 sbit    EXTRAM      =           AUXR^1;
 sbit    S1BRT       =           AUXR^0;
 
 sfr         INTCLKO     =           0x8f;
 sbit    EX4         =           INTCLKO^6;
 sbit    EX3         =           INTCLKO^5;
 sbit    EX2         =           INTCLKO^4;
 sbit    T2CLKO      =           INTCLKO^2;
 sbit    T1CLKO      =           INTCLKO^1;
 sbit    T0CLKO      =           INTCLKO^0;
 
 sfr         P1          =           0x90;
 sbit    P10         =           P1^0;
 sbit    P11         =           P1^1;
 sbit    P12         =           P1^2;
 sbit    P13         =           P1^3;
 sbit    P14         =           P1^4;
 sbit    P15         =           P1^5;
 sbit    P16         =           P1^6;
 sbit    P17         =           P1^7;
 
 sfr         P1M1        =           0x91;
 sfr         P1M0        =           0x92;
 sfr         P0M1        =           0x93;
 sfr         P0M0        =           0x94;
 sfr         P2M1        =           0x95;
 sfr         P2M0        =           0x96;
 
 sfr         AUXR2       =           0x97;
 sbit    RAMTINY     =           AUXR2^7;
 sbit    CPUMODE     =           AUXR2^6;
 sbit    RAMEXE      =           AUXR2^5;
 sbit    CANFD       =           AUXR2^4;
 sbit    CANSEL      =           AUXR2^3;
 sbit    CAN2EN      =           AUXR2^2;
 sbit    CANEN       =           AUXR2^1;
 sbit    LINEN       =           AUXR2^0;
 
 sfr         SCON        =           0x98;
 sbit    SM0         =           SCON^7;
 sbit    SM1         =           SCON^6;
 sbit    SM2         =           SCON^5;
 sbit    REN         =           SCON^4;
 sbit    TB8         =           SCON^3;
 sbit    RB8         =           SCON^2;
 sbit    TI          =           SCON^1;
 sbit    RI          =           SCON^0;
 
 sfr         SBUF        =           0x99;
 
 sfr         S2CON       =           0x9a;
 sbit    S2SM0       =           S2CON^7;
 sbit    S2SM1       =           S2CON^6;
 sbit    S2SM2       =           S2CON^5;
 sbit    S2REN       =           S2CON^4;
 sbit    S2TB8       =           S2CON^3;
 sbit    S2RB8       =           S2CON^2;
 sbit    S2TI        =           S2CON^1;
 sbit    S2RI        =           S2CON^0;
 
 sfr         S2BUF       =           0x9b;
 
 sfr         IRCBAND     =           0x9d;
 sbit    USBCKS      =           IRCBAND^7;
 sbit    USBCKS2     =           IRCBAND^6;
 sbit    HIRCSEL1    =           IRCBAND^1;
 sbit    HIRCSEL0    =           IRCBAND^0;
 
 sfr         LIRTRIM     =           0x9e;
 sfr         IRTRIM      =           0x9f;
 
 sfr         P2          =           0xa0;
 sbit    P20         =           P2^0;
 sbit    P21         =           P2^1;
 sbit    P22         =           P2^2;
 sbit    P23         =           P2^3;
 sbit    P24         =           P2^4;
 sbit    P25         =           P2^5;
 sbit    P26         =           P2^6;
 sbit    P27         =           P2^7;
 
 sfr         BUS_SPEED   =           0xa1;
 
 sfr         P_SW1       =           0xa2;
 sbit    S1_S1       =           P_SW1^7;
 sbit    S1_S0       =           P_SW1^6;
 sbit    CAN_S1      =           P_SW1^5;
 sbit    CAN_S0      =           P_SW1^4;
 sbit    SPI_S1      =           P_SW1^3;
 sbit    SPI_S0      =           P_SW1^2;
 sbit    LIN_S1      =           P_SW1^1;
 sbit    LIN_S0      =           P_SW1^0;
 
 sfr         VRTRIM      =           0xa6;
 
 sfr         IE          =           0xa8;
 sbit    EA          =           IE^7;
 sbit    ELVD        =           IE^6;
 sbit    EADC        =           IE^5;
 sbit    ES          =           IE^4;
 sbit    ET1         =           IE^3;
 sbit    EX1         =           IE^2;
 sbit    ET0         =           IE^1;
 sbit    EX0         =           IE^0;
 
 sfr         SADDR       =           0xa9;
 sfr         WKTCL       =           0xaa;
 sfr         WKTCH       =           0xab;
 sbit    WKTEN       =           WKTCH^7;
 
 sfr         S3CON       =           0xac;
 sbit    S3SM0       =           S3CON^7;
 sbit    S3ST3       =           S3CON^6;
 sbit    S3SM2       =           S3CON^5;
 sbit    S3REN       =           S3CON^4;
 sbit    S3TB8       =           S3CON^3;
 sbit    S3RB8       =           S3CON^2;
 sbit    S3TI        =           S3CON^1;
 sbit    S3RI        =           S3CON^0;
 
 sfr         S3BUF       =           0xad;
 sfr         TA          =           0xae;
 
 sfr         IE2         =           0xaf;
 sbit    EUSB        =           IE2^7;
 sbit    ET4         =           IE2^6;
 sbit    ET3         =           IE2^5;
 sbit    ES4         =           IE2^4;
 sbit    ES3         =           IE2^3;
 sbit    ET2         =           IE2^2;
 sbit    ESPI        =           IE2^1;
 sbit    ES2         =           IE2^0;
 
 sfr         P3          =           0xb0;
 sbit    P30         =           P3^0;
 sbit    P31         =           P3^1;
 sbit    P32         =           P3^2;
 sbit    P33         =           P3^3;
 sbit    P34         =           P3^4;
 sbit    P35         =           P3^5;
 sbit    P36         =           P3^6;
 sbit    P37         =           P3^7;
 
 sbit    RD          =           P3^7;
 sbit    WR          =           P3^6;
 sbit    T1          =           P3^5;
 sbit    T0          =           P3^4;
 sbit    INT1        =           P3^3;
 sbit    INT0        =           P3^2;
 sbit    TXD         =           P3^1;
 sbit    RXD         =           P3^0;
 
 sfr         P3M1        =           0xb1;
 sfr         P3M0        =           0xb2;
 sfr         P4M1        =           0xb3;
 sfr         P4M0        =           0xb4;
 
 sfr         IP2         =           0xb5;
 sbit    PUSB        =           IP2^7;
 sbit    PI2C        =           IP2^6;
 sbit    PCMP        =           IP2^5;
 sbit    PX4         =           IP2^4;
 sbit    PPWMB       =           IP2^3;
 sbit    PPWMA       =           IP2^2;
 sbit    PSPI        =           IP2^1;
 sbit    PS2         =           IP2^0;
 
 sfr         IP2H        =           0xb6;
 sbit    PUSBH       =           IP2H^7;
 sbit    PI2CH       =           IP2H^6;
 sbit    PCMPH       =           IP2H^5;
 sbit    PX4H        =           IP2H^4;
 sbit    PPWMBH      =           IP2H^3;
 sbit    PPWMAH      =           IP2H^2;
 sbit    PSPIH       =           IP2H^1;
 sbit    PS2H        =           IP2H^0;
 
 sfr         IPH         =           0xb7;
 sbit    PPCAH       =           IPH^7;
 sbit    PLVDH       =           IPH^6;
 sbit    PADCH       =           IPH^5;
 sbit    PSH         =           IPH^4;
 sbit    PT1H        =           IPH^3;
 sbit    PX1H        =           IPH^2;
 sbit    PT0H        =           IPH^1;
 sbit    PX0H        =           IPH^0;
 
 sfr         IP          =           0xb8;
 sbit    PPCA        =           IP^7;
 sbit    PLVD        =           IP^6;
 sbit    PADC        =           IP^5;
 sbit    PS          =           IP^4;
 sbit    PT1         =           IP^3;
 sbit    PX1         =           IP^2;
 sbit    PT0         =           IP^1;
 sbit    PX0         =           IP^0;
 
 sfr         SADEN       =           0xb9;
 
 sfr         P_SW2       =           0xba;
 sbit    EAXFR       =           P_SW2^7;
 sbit    I2C_S1      =           P_SW2^5;
 sbit    I2C_S0      =           P_SW2^4;
 sbit    CMPO_S      =           P_SW2^3;
 sbit    S4_S        =           P_SW2^2;
 sbit    S3_S        =           P_SW2^1;
 sbit    S2_S        =           P_SW2^0;
 
 sfr         P_SW3       =           0xbb;
 sbit    I2S_S1      =           P_SW3^7;
 sbit    I2S_S0      =           P_SW3^6;
 sbit    S2SPI_S1    =           P_SW3^5;
 sbit    S2SPI_S0    =           P_SW3^4;
 sbit    S1SPI_S1    =           P_SW3^3;
 sbit    S1SPI_S0    =           P_SW3^2;
 sbit    CAN2_S1     =           P_SW3^1;
 sbit    CAN2_S0     =           P_SW3^0;
 
 sfr         ADC_CONTR   =           0xbc;
 sbit    ADC_POWER   =           ADC_CONTR^7;
 sbit    ADC_START   =           ADC_CONTR^6;
 sbit    ADC_FLAG    =           ADC_CONTR^5;
 sbit    ADC_EPWMT   =           ADC_CONTR^4;
 
 sfr         ADC_RES     =           0xbd;
 sfr         ADC_RESL    =           0xbe;
 
 sfr         P_SW4       =           0xbf;
 sbit    QSPI_S1     =           P_SW4^1;
 sbit    QSPI_S0     =           P_SW4^0;
 
 sfr         P4          =           0xc0;
 sbit    P40         =           P4^0;
 sbit    P41         =           P4^1;
 sbit    P42         =           P4^2;
 sbit    P43         =           P4^3;
 sbit    P44         =           P4^4;
 sbit    P45         =           P4^5;
 sbit    P46         =           P4^6;
 sbit    P47         =           P4^7;
 
 sfr         WDT_CONTR   =           0xc1;
 sbit    WDT_FLAG    =           WDT_CONTR^7;
 sbit    EN_WDT      =           WDT_CONTR^5;
 sbit    CLR_WDT     =           WDT_CONTR^4;
 sbit    IDL_WDT     =           WDT_CONTR^3;
 
 sfr         IAP_DATA    =           0xc2;
 sfr         IAP_ADDRH   =           0xc3;
 sfr         IAP_ADDRL   =           0xc4;
 sfr         IAP_CMD     =           0xc5;
 sfr         IAP_TRIG    =           0xc6;
 
 sfr         IAP_CONTR   =           0xc7;
 sbit    IAPEN       =           IAP_CONTR^7;
 sbit    SWBS        =           IAP_CONTR^6;
 sbit    SWRST       =           IAP_CONTR^5;
 sbit    CMD_FAIL    =           IAP_CONTR^4;
 sbit    SWBS2       =           IAP_CONTR^3;
 
 sfr         P5          =           0xc8;
 sbit    P50         =           P5^0;
 sbit    P51         =           P5^1;
 sbit    P52         =           P5^2;
 sbit    P53         =           P5^3;
 sbit    P54         =           P5^4;
 sbit    P55         =           P5^5;
 sbit    P56         =           P5^6;
 sbit    P57         =           P5^7;
 
 sfr         P5M1        =           0xc9;
 sfr         P5M0        =           0xca;
 sfr         P6M1        =           0xcb;
 sfr         P6M0        =           0xcc;
 
 sfr         SPSTAT      =           0xcd;
 sbit    SPIF        =           SPSTAT^7;
 sbit    WCOL        =           SPSTAT^6;
 
 sfr         SPCTL       =           0xce;
 sbit    SSIG        =           SPCTL^7;
 sbit    SPEN        =           SPCTL^6;
 sbit    DORD        =           SPCTL^5;
 sbit    MSTR        =           SPCTL^4;
 sbit    CPOL        =           SPCTL^3;
 sbit    CPHA        =           SPCTL^2;
 sbit    SPR1        =           SPCTL^1;
 sbit    SPR0        =           SPCTL^0;
 
 sfr         SPDAT       =           0xcf;
 
 sfr         PSW         =           0xd0;
 sbit    CY          =           PSW^7;
 sbit    AC          =           PSW^6;
 sbit    F0          =           PSW^5;
 sbit    RS1         =           PSW^4;
 sbit    RS0         =           PSW^3;
 sbit    OV          =           PSW^2;
 sbit    F1          =           PSW^1;
 sbit    P           =           PSW^0;
 
 sfr         PSW1        =           0xd1;
 sbit    N           =           PSW1^5;
 sbit    Z           =           PSW1^1;
 
 sfr         T4H         =           0xd2;
 sfr         T4L         =           0xd3;
 sfr         T3H         =           0xd4;
 sfr         T3L         =           0xd5;
 sfr         T2H         =           0xd6;
 sfr         T2L         =           0xd7;
 
 sfr         USBCLK      =           0xdc;
 
 sfr         T4T3M       =           0xdd;
 sbit    T4R         =           T4T3M^7;
 sbit    T4_CT       =           T4T3M^6;
 sbit    T4x12       =           T4T3M^5;
 sbit    T4CLKO      =           T4T3M^4;
 sbit    T3R         =           T4T3M^3;
 sbit    T3_CT       =           T4T3M^2;
 sbit    T3x12       =           T4T3M^1;
 sbit    T3CLKO      =           T4T3M^0;
 
 sfr         ADCCFG      =           0xde;
 sbit    RESFMT      =           ADCCFG^5;
 
 sfr         IP3         =           0xdf;
 sbit    PI2S        =           IP3^3;
 sbit    PRTC        =           IP3^2;
 sbit    PS4         =           IP3^1;
 sbit    PS3         =           IP3^0;
 
 sfr         ACC         =           0xe0;
 sfr         P7M1        =           0xe1;
 sfr         P7M0        =           0xe2;
 sfr         DPS         =           0xe3;
 
 sfr         CMPCR1      =           0xe6;
 sbit    CMPEN       =           CMPCR1^7;
 sbit    CMPIF       =           CMPCR1^6;
 sbit    PIE         =           CMPCR1^5;
 sbit    NIE         =           CMPCR1^4;
 sbit    CMPOE       =           CMPCR1^1;
 sbit    CMPRES      =           CMPCR1^0;
 
 sfr         CMPCR2      =           0xe7;
 sbit    INVCMPO     =           CMPCR2^7;
 sbit    DISFLT      =           CMPCR2^6;
 
 sfr         P6          =           0xe8;
 sbit    P60         =           P6^0;
 sbit    P61         =           P6^1;
 sbit    P62         =           P6^2;
 sbit    P63         =           P6^3;
 sbit    P64         =           P6^4;
 sbit    P65         =           P6^5;
 sbit    P66         =           P6^6;
 sbit    P67         =           P6^7;
 
 sfr         WTST        =           0xe9;
 sfr         CKCON       =           0xea;
 sfr         MXAX        =           0xeb;
 sfr         USBDAT      =           0xec;
 sfr         DMAIR       =           0xed;
 
 sfr         IP3H        =           0xee;
 sbit    PI2SH       =           IP3H^3;
 sbit    PRTCH       =           IP3H^2;
 sbit    PS4H        =           IP3H^1;
 sbit    PS3H        =           IP3H^0;
 
 sfr         AUXINTIF    =           0xef;
 sbit    INT4IF      =           AUXINTIF^6;
 sbit    INT3IF      =           AUXINTIF^5;
 sbit    INT2IF      =           AUXINTIF^4;
 sbit    T4IF        =           AUXINTIF^2;
 sbit    T3IF        =           AUXINTIF^1;
 sbit    T2IF        =           AUXINTIF^0;
 
 sfr         B           =           0xf0;
 
 sfr         USBCON      =           0xf4;
 sbit    ENUSB       =           USBCON^7;
 sbit    ENUSBRST    =           USBCON^6;
 sbit    PS2M        =           USBCON^5;
 sbit    PUEN        =           USBCON^4;
 sbit    PDEN        =           USBCON^3;
 sbit    DFREC       =           USBCON^2;
 sbit    DP          =           USBCON^1;
 sbit    DM          =           USBCON^0;
 
 sfr         IAP_TPS     =           0xf5;
 sfr         IAP_ADDRE   =           0xf6;
 
 sfr         P7          =           0xf8;
 sbit    P70         =           P7^0;
 sbit    P71         =           P7^1;
 sbit    P72         =           P7^2;
 sbit    P73         =           P7^3;
 sbit    P74         =           P7^4;
 sbit    P75         =           P7^5;
 sbit    P76         =           P7^6;
 sbit    P77         =           P7^7;
 
 sfr         USBADR      =           0xfc;
 
 sfr         S4CON       =           0xfd;
 sbit    S4SM0       =           S4CON^7;
 sbit    S4ST4       =           S4CON^6;
 sbit    S4SM2       =           S4CON^5;
 sbit    S4REN       =           S4CON^4;
 sbit    S4TB8       =           S4CON^3;
 sbit    S4RB8       =           S4CON^2;
 sbit    S4TI        =           S4CON^1;
 sbit    S4RI        =           S4CON^0;
 
 sfr         S4BUF       =           0xfe;
 
 sfr         RSTCFG      =           0xff;
 sbit    ENLVR       =           RSTCFG^6;
 sbit    P54RST      =           RSTCFG^4;
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 6 "\Users\17263\Desktop\AM32_Bootloader_STC32\Bootloader\inc\main.h" 
 
 
 
#line 7 ".\Bootloader\inc\bootloader.h" 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 void read_flash_bin(uint8_t*  dat , uint32_t add ,int  out_buff_len);
 void save_flash_nolib(uint8_t *dat, int length, uint32_t add);
#line 12 "Bootloader\src\bootloader.c" 
 
  
#line 1 "D:\Compiler\Keil_v5\C251\Inc\string.h" 






 
 
 #pragma SAVE
 #pragma PARM251
 
 
 
 typedef unsigned int size_t;
 
 
 
 
 
 
 
 extern char *strcat (char *s1, const char *s2) reentrant;
 extern char *strncat (char *s1, const char *s2, size_t n) reentrant;
 
 extern char strcmp (const char *s1, const char *s2) reentrant;
 extern char strncmp (const char *s1, const char *s2, size_t n) reentrant;
 
 extern char *strcpy (char *s1, const char *s2) reentrant;
 extern char *strncpy (char *s1, const char *s2, size_t n) reentrant;
 
 extern size_t strlen (const char *) reentrant;
 
 extern char *strchr (const char *s, char c) reentrant;
 extern int strpos (const char *s, char c) reentrant;
 extern char *strrchr (const char *s, char c) reentrant;
 extern int strrpos (const char *s, char c) reentrant;
 
 extern size_t strspn (const char *s, const char *set) reentrant;
 extern size_t strcspn (const char *s, const char *set) reentrant;
 extern char *strpbrk (const char *s, const char *set) reentrant;
 extern char *strrpbrk (const char *s, const char *set) reentrant;
 
 extern char memcmp (const void *s1, const void *s2, size_t n) reentrant;
 extern void *memcpy (void *s1, const void *s2, size_t n) reentrant;
 extern void *memchr (const void *s, char val, size_t n) reentrant;
 extern void *memccpy (void *s1, const void *s2, char val, size_t n) reentrant;
 extern void *memmove (void *s1, const void *s2, size_t n) reentrant;
 extern void *memset  (void *s, char val, size_t n) reentrant;
 
 extern void far   *fmemset  (void far *s,   char val, unsigned int n) reentrant;
 extern void xdata *xmemset  (void xdata *s, char val, unsigned int n) reentrant;
 extern void far   *fmemcpy  (void far *s1,  void far *s2, unsigned int n) reentrant;
 extern void xdata *xmemcpy  (void xdata *s1, void xdata *s2, unsigned int n) reentrant;
 
 extern unsigned long hstrlen (const char huge *s)  reentrant;
 extern          char hstrcmp (const char huge *s1, const char huge *s2)  reentrant;
 extern    char huge *hstrcpy (char huge *s1, const char huge *s2)  reentrant;
 
 #pragma PARM4 
 extern char huge *hstrncpy (char huge *s1, const char huge *s2, unsigned long n) reentrant; 
 
 extern      char  hmemcmp (const void huge *s1, const void huge *s2, unsigned long len) reentrant;
 extern void huge *hmemcpy (void huge *s1, const void huge *s2, unsigned long len) reentrant;
 extern void huge *hmemchr (const void huge *ptr, char val, unsigned long len) reentrant;
 extern char huge *hmemccpy (char huge *dest, const char huge *src, char val, unsigned long len) reentrant;
 extern void huge *hmemmove (void huge *s1, const void huge *s2, unsigned long len) reentrant;
 extern void huge *hmemset (void huge *ptr, char val, unsigned long len) reentrant;
 
 
#line 70 "D:\Compiler\Keil_v5\C251\Inc\string.h" 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 99 "D:\Compiler\Keil_v5\C251\Inc\string.h" 
 
 #pragma RESTORE
#line 13 "Bootloader\src\bootloader.c" 
 
  
#line 1 "D:\Compiler\Keil_v5\C251\Inc\stdio.h" 






 
 
 #pragma SAVE
 #pragma PARM251
 
 
 
 
 
 
#line 17 "D:\Compiler\Keil_v5\C251\Inc\stdio.h" 
  
 
#line 19 "D:\Compiler\Keil_v5\C251\Inc\stdio.h" 
 
 
#line 21 "D:\Compiler\Keil_v5\C251\Inc\stdio.h" 
  
 
 
#line 24 "D:\Compiler\Keil_v5\C251\Inc\stdio.h" 
 
 
 #pragma SAVE
 #pragma FUNCTIONS(STATIC)
 
 extern char _getkey  (void);
 extern char getchar  (void);
 extern char ungetchar(char);
 extern char putchar  (char);
 extern int  printf   (const char *, ...); 
 extern char *gets    (char *, int n);
 extern int  scanf    (const char *, ...);
 extern int  vprintf  (const char *, char *);
 extern int  puts     (const char *);
 #pragma RESTORE
 
 extern int  sprintf  (char *, const char *, ...);
 extern int  vsprintf (char *, const char *, char *);
 extern int  sscanf   (char *, const char *, ...);
 
#line 44 "D:\Compiler\Keil_v5\C251\Inc\stdio.h" 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 57 "D:\Compiler\Keil_v5\C251\Inc\stdio.h" 
 
 #pragma RESTORE
#line 14 "Bootloader\src\bootloader.c" 
 
 
 
 
 
 
 void save_flash_nolib(uint8_t *dat, int length, uint32_t add){
 
 IAPEN = 1;                       	
 
 
 if((add % 0x200) == 0){
 
 IAP_CMD = 3;                        
 
 IAP_ADDRE = (uint8_t)(add >> 16); 
 IAP_ADDRH = (uint8_t)(add >> 8);  
 IAP_ADDRL = (uint8_t)add;         
 
 IAP_TRIG = 0x5A;
 IAP_TRIG = 0xA5;                   
 _nop_();   
 _nop_();
 _nop_();
 _nop_();
 
 while(CMD_FAIL)
 {
 printf("CMD_FAIL\n");
 }                  
 }
 
 IAP_CMD = 2;                         
 do
 {
 IAP_ADDRE = (uint8_t)(add >> 16); 
 IAP_ADDRH = (uint8_t)(add >> 8);  
 IAP_ADDRL = (uint8_t)add;         
 IAP_DATA  = *dat;      
 
 IAP_TRIG = 0x5A;
 IAP_TRIG = 0xA5;                   
 _nop_();   
 _nop_();
 _nop_();
 _nop_();
 
 while(CMD_FAIL)
 {
 printf("CMD_FAIL\n");
 }
 
 add++;                      
 dat++;                     
 }
 while(--length);                     
 
 
 IAP_CONTR = 0; IAP_CMD = 0; IAP_TRIG = 0; IAP_ADDRH = 0xFF; IAP_ADDRL = 0xFF;                       
 }
 
 void read_flash_bin(uint8_t*  dat , uint32_t add , int out_buff_len){
 int i;
 IAPEN = 1;                            
 IAP_CMD = 1;                              
 for (i = 0; i < out_buff_len ; i ++){
 CMD_FAIL = 0;
 IAP_ADDRE = (uint8_t)((add+i) >> 16);  
 IAP_ADDRH = (uint8_t)((add+i) >> 8);   
 IAP_ADDRL = (uint8_t)(add+i);          
 
 IAP_TRIG = 0x5A;
 IAP_TRIG = 0xA5;                   
 _nop_();   
 _nop_();
 _nop_();
 _nop_();
 while(CMD_FAIL)
 {
 printf("CMD_FAIL\n");
 }
 
 dat[i] = IAP_DATA;             
 }
 
 IAP_CONTR = 0; IAP_CMD = 0; IAP_TRIG = 0; IAP_ADDRH = 0xFF; IAP_ADDRL = 0xFF;                      
 }
 
 
