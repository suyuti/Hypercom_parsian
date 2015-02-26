/*  *****************************************************************        */
/*  *                                                               *        */
/*  *                      Hypercom Inc                             *        */
/*  *                     (c) Copyright 2001                        *        */
/*  *                                                               *        */
/*  *  PRODUCT   :  Hypercom Base Application in C                  *        */
/*  *                                                               *        */
/*  *  COMPONENT :  EMV Level 2 Cardholder Verification             *        */
/*  *                                                               *        */
/*  *  FILENAME  :  emv2_cv.h                                       *        */
/*  *                                                               *        */
/*  *  HISTORY   :                                                  *        */
/*  *                                                               *        */
/*  *    DATE         AUTHOR         COMMENT                        *        */
/*  *  --------     ----------      ----------                      *        */
/*  *  13-11-01     A.Shuttleworth  First draft                     *        */
/*  *  10-01-02     A.Shuttleworth  Removed PINVerifyResponse def.  *        */
/*  *  03-11-03     A.Shuttleworth  2.1 - added cust cond/method    *        */
/*  *  04-11-03     A.Shuttleworth  2.2 - split cash conditions     *        */
/*  *                                                               *        */
/*  *                                                               *        */
/*  *                                                               *        */
/*  *****************************************************************        */

#define AIP_CVMSupported		0x10

/* EMV values for CVM Result */
#define CVMR_UNKNOWN 			  0x00
#define CVMR_FAIL 			     0x01
#define CVMR_SUCCESS	 		  0x02


/* Values for Condition Codes */
#define COND_ALWAYS                 0x00
#define COND_UNATTENDED_CASH        0x01
#define COND_NOT_CASH               0x02
/* 0x03: In the case of offline PIN CVM, this means 'if offline PIN pad present.' */
/*       In the case of online PIN CVM, this means 'If online PIN pad present.'   */
#define COND_TERM_SUPPORTS_CVM      0x03 
#define COND_MANUAL_CASH            0x04
#define COND_CASHBACK               0x05
/* 0x06-0x09: Where Transaction Currency Code = Application Currency Code */
#define COND_UNDER_X                0x06
#define COND_OVER_X                 0x07
#define COND_UNDER_Y                0x08
#define COND_OVER_Y                 0x09
/* 0x04, 0x05 RFU */
/* 0A-7F RFU */
/* 80-FF reserved for individual payment systems */

/* Values for Cardholder Verification Methods */
#define FAIL_CVM_PROCESSING         0
#define PLAINTEXT_OFFLINE           1
#define ENCIPHERED_ONLINE           2
#define PLAINTEXT_OFFLINE_AND_SIG   3
#define ENCIPHERED_OFFLINE          4
#define ENCIPHERED_OFFLINE_AND_SIG  5
#define START_EMV_RANGE             6
#define END_EMV_RANGE               0x1D
#define SIGNATURE                   0x1E
#define NO_CVM_REQUIRED             0x1F
#define NO_MATCH_MARKER             0x3F


typedef struct
{
    UBYTE Method;           // Unique ID number 
    UBYTE Condition;        // Check service ID 
} CVMRule;

/* Used by the CVM processing state machine */
#define STATE_FIRST_TIME_THROUGH        1
#define STATE_REENTRY_WITH_PIN          2
#define STATE_REENTRY_WITH_ONLINE_PIN   3
#define STATE_REENTRY_CUSTOM_CONDITION  4
#define STATE_REENTRY_CUSTOM_METHOD     5

