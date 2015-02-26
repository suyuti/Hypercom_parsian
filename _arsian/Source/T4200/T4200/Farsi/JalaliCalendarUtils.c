#include "basictyp.h"
#include "sdkos.h"
#include "sdk.h"
#include "sdkmem.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "utllib.h"
#include "tabutil.h"

#define LOBYTE(w) ((BYTE)(w))
#define HIBYTE(w) ((BYTE)(((WORD)(w) >> 8) & 0xFF))
//#define TOHEX(n)  ((n >> 4) & 0xF)

#ifndef U08
    #define  U08    unsigned char
#endif

typedef struct  t_DATE_TIME {
    U08    CLKSEC;
    U08    CLKMIN;
    U08    CLKHOUR;
    U08    CLKDATE;
    U08    CLKMONTH;
    //U08    CLKDAY;
    U08    CLKYEARL;
    U08    CLKYEARH;
} DATE_TIME;

#ifndef MAKE_FARSI
    const char WeekDay[ 8][4] = {"   ", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    const char Month12[13][4] = {"   ", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
#else
    void convMJD2JALALI(DATE_TIME *dt);
    void convJALALI2MJD(DATE_TIME *dt);
    const char WeekDay[ 8][4] = {"   ", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    const char Month12[13][4] = {"   ", "Far", "Ord", "Kho", "Tir", "Mor", "Sha", "Meh", "Aba", "Aza", "Dey", "Bah", "Esf"};
                                    /* 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12*/
    const char g_days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const char j_days_in_month[12] = {31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};
/*
     Persian(Jalali) year start Mar.21,0621(03/21/0621) => Far.01,0000(01/01/0000)
             so, new year start Jan.01,2001(01/01/2002) => Day.11,1381(10/11/1381)
     1, farvardin (frvrdyn)     31 days
     2, ordibehesht (ardybhSt)  31 days
     3, khordad (Krdad)         31 days
     4, tir (tyr)               31 days
     5, mordad (mrdad)          31 days
     6, shahrivar (Shryvr)      31 days
     7, mehr (mhr)              30 days
     8, Aban (Aban)             30 days
     9, Azar (AZr)              30 days
    10, day (dy)                30 days
    11, bahman (bhmn)           30 days
    12, esfand (asfnd)       29/30 days
*/
void convMJD2JALALI(DATE_TIME *dt) // MJD->JALALI
{
    int  g_y, g_m, g_d;
    long gy, gm, gd;
    long jy, jm, jd;
    long g_day_no, j_day_no;
    long j_np;
    int  i;

    dt->CLKYEARH = 0x20;
    g_y =((int)((dt->CLKYEARH >> 4)*10 + (dt->CLKYEARH & 0x0F))*100);
    g_y+= (int)((dt->CLKYEARL >> 4)*10 + (dt->CLKYEARL & 0x0F));
    g_m = (int)((dt->CLKMONTH >> 4)*10 + (dt->CLKMONTH & 0x0F));
    g_d = (int)((dt->CLKDATE  >> 4)*10 + (dt->CLKDATE  & 0x0F));
    gy = g_y-1600;
    gm = g_m-1;
    gd = g_d-1;

    g_day_no = 365*gy+(gy+3)/4-(gy+99)/100+(gy+399)/400;
    for (i=0;i<gm;++i) g_day_no += g_days_in_month[i];
    if (gm>1 && ((gy%4==0 && gy%100!=0) || (gy%400==0))) ++g_day_no; /* leap and after Feb */
        
    g_day_no += gd;
    j_day_no = g_day_no-79;

    j_np = j_day_no / 12053;
    j_day_no %= 12053;

    jy = 979 + 33*j_np + 4*(j_day_no/1461);
    j_day_no %= 1461;

    if (j_day_no >= 366) {
        jy += (j_day_no-1)/365;
        j_day_no = (j_day_no-1)%365;
    }

    for (i = 0; i < 11 && j_day_no >= j_days_in_month[i]; ++i) {
        j_day_no -= j_days_in_month[i];
    }
    jm = i+1;
    jd = (int)(j_day_no+1);
    i = (int)(jy/100);
    dt->CLKYEARH = ((i/10)<<4) | (i%10);
    i = (int)(jy%100);
    dt->CLKYEARL = ((i/10)<<4) | (i%10);
    i=0;
    while(jm >= 10) { jm -= 10; i = 0x10; }
    dt->CLKMONTH = (int)(i + jm);
    i=0;
    while(jd >= 10) { jd -= 10; i += 0x10; }
    dt->CLKDATE = (int)(i + jd);
}

void convJALALI2MJD(DATE_TIME *dt) // JALALI->MJD
{
    int  j_y, j_m, j_d;
    long gy, gm, gd;
    long jy, jm, jd;
    long g_day_no, j_day_no;
    int leap;
    int i;

    dt->CLKYEARH = 0x13;
    j_y =((int)((dt->CLKYEARH >> 4)*10 + (dt->CLKYEARH & 0x0F))*100);
    j_y+= (int)((dt->CLKYEARL >> 4)*10 + (dt->CLKYEARL & 0x0F));
    j_m = (int)((dt->CLKMONTH >> 4)*10 + (dt->CLKMONTH & 0x0F));

    jy = j_y-979;
    jm = j_m-1;
    jd = j_d-1;

    j_day_no = 365*jy + (jy/33)*8 + (jy%33+3)/4;
    for (i=0; i < jm; ++i) j_day_no += j_days_in_month[i];

    j_day_no += jd;

    g_day_no = j_day_no+79;

    gy = 1600 + 400*(g_day_no/146097); /* 146097 = 365*400 + 400/4 - 400/100 + 400/400 */
    g_day_no = g_day_no % 146097;

    leap = 1;
    if (g_day_no >= 36525) {/* 36525 = 365*100 + 100/4 */
        g_day_no--;
        gy += 100*(g_day_no/36524); /* 36524 = 365*100 + 100/4 - 100/100 */
        g_day_no = g_day_no % 36524;

        if (g_day_no >= 365) g_day_no++;
        else leap = 0;
    }

    gy += 4*(g_day_no/1461); /* 1461 = 365*4 + 4/4 */
    g_day_no %= 1461;

    if (g_day_no >= 366) {
        leap = 0;
        g_day_no--;
        gy += g_day_no/365;
        g_day_no = g_day_no % 365;
    }

    for (i = 0; g_day_no >= g_days_in_month[i] + (i == 1 && leap); i++) {
        g_day_no -= g_days_in_month[i] + (i == 1 && leap);
    }
    gm = i+1;
    gd = (int)(g_day_no+1);

    i = (int)(gy/100);
    dt->CLKYEARH = ((i/10)<<4) | (i%10);
    i = (int)(gy%100);
    dt->CLKYEARL = ((i/10)<<4) | (i%10);
    i=0;
    while(gm >= 10) { gm -= 10; i = 0x10; }
    dt->CLKMONTH = (int)(i + gm);
    i=0;
    while(gd >= 10) { gd -= 10; i += 0x10; }
    dt->CLKDATE = (int)(i + gd);
}

// _normalDateBuffer : YYMMDDHHmmSS
// _jalaliDateBuffer : YYMMDDHHmmSS

void GetJelaliDateTime(UBYTE* _normalDateBuffer, UBYTE * _jelaliDateBuffer)
{
    static char buf[3];
    static char buf2[3];
    DATE_TIME dt;
    int val;

    memset((UBYTE*)buf, 0, 3);
    // year
    buf[0] = _normalDateBuffer[0];
    buf[1] = _normalDateBuffer[1];
    buf[2] = '\0';

    sprintf(buf2, "%x", atoi(buf));
    dt.CLKYEARH = 0; 
    dt.CLKYEARL = atoi(buf2);

    // month
    buf[0] = _normalDateBuffer[2];
    buf[1] = _normalDateBuffer[3];
    buf[2] = '\0';
    sprintf(buf2, "%x", atoi(buf));
    dt.CLKMONTH = atoi(buf2);

    // day (date)
    buf[0] = _normalDateBuffer[4];
    buf[1] = _normalDateBuffer[5];
    buf[2] = '\0';
    sprintf(buf2, "%x", atoi(buf));
    dt.CLKDATE = atoi(buf2);

    // hour
    buf[0] = _normalDateBuffer[6];
    buf[1] = _normalDateBuffer[7];
    buf[2] = '\0';
    dt.CLKHOUR = atoi(buf);

    // minute
    buf[0] = _normalDateBuffer[8];
    buf[1] = _normalDateBuffer[9];
    buf[2] = '\0';
    dt.CLKMIN = atoi(buf);

    // second
    buf[0] = _normalDateBuffer[10];
    buf[1] = _normalDateBuffer[11];
    buf[2] = '\0';
    dt.CLKSEC = atoi(buf);


    convMJD2JALALI(&dt);

    sprintf(_normalDateBuffer, "%02x%02x%02x%02d%02d%02d",
            dt.CLKYEARL,
            dt.CLKMONTH,
            dt.CLKDATE,
            dt.CLKHOUR,
            dt.CLKMIN,
            dt.CLKSEC);
}


void JelaliToMiladi(UBYTE* _jelaliDateBuffer /*in BCD*/, UBYTE * _normalDateBuffer /*out*/)
{
    static char buf[3];
    static char buf2[3];
    DATE_TIME dt;
    int val;


    memset((UBYTE*)buf, 0, 3);
    // year
    buf[0] = _jelaliDateBuffer[0];
    buf[1] = _jelaliDateBuffer[1];
    buf[2] = '\0';

    sprintf(buf2, "%x", atoi(buf));
    dt.CLKYEARH = 0; 
    dt.CLKYEARL = atoi(buf2);

    // month
    buf[0] = _jelaliDateBuffer[2];
    buf[1] = _jelaliDateBuffer[3];
    buf[2] = '\0';
    sprintf(buf2, "%x", atoi(buf));
    dt.CLKMONTH = atoi(buf2);

    // day (date)
    buf[0] = _jelaliDateBuffer[4];
    buf[1] = _jelaliDateBuffer[5];
    buf[2] = '\0';
    sprintf(buf2, "%x", atoi(buf));
    dt.CLKDATE = atoi(buf2);

    // hour
    buf[0] = _jelaliDateBuffer[6];
    buf[1] = _jelaliDateBuffer[7];
    buf[2] = '\0';
    dt.CLKHOUR = atoi(buf);

    // minute
    buf[0] = _jelaliDateBuffer[8];
    buf[1] = _jelaliDateBuffer[9];
    buf[2] = '\0';
    dt.CLKMIN = atoi(buf);

    // second
    buf[0] = _jelaliDateBuffer[10];
    buf[1] = _jelaliDateBuffer[11];
    buf[2] = '\0';
    dt.CLKSEC = atoi(buf);

    convJALALI2MJD(&dt);

    sprintf(_normalDateBuffer, "%02x%02x%02x%02d%02d%02d",
            dt.CLKYEARL,
            dt.CLKMONTH,
            dt.CLKDATE,
            dt.CLKHOUR,
            dt.CLKMIN,
            dt.CLKSEC);
}

#endif