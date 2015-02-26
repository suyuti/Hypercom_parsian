
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             prtfields.c
//      Print fields definition
//
//=============================================================================

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "fixdata.h"

#include "struct.h"
#include "comdata.h"
#include "appdata.h"

#include "prtfields.h"
#include "prtdef2.h"


// Field Definition Table
// The order of the following array should match FLDTAB_ENUM
// Furthermore, the order should not be altered, or downloaded
// reports will not print properly.
// This table will support up to 160 fields
const struct FLDDEF FLDTABDEF[] = {
    {                           // not_used
     0,                         // ADDRESS OR OFFSET OF THE FIELD
     0,                         // LENGTH OF THE FIELD
     A_NONE,                    // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_CNAME
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct issuer_rec, ISNAME ),
     S_ISNAME,                  // LENGTH OF THE FIELD
     A_ISS,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_SRVID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRCASHIER ),
     S_TRCASHIER,               // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_EXPDT
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TREXPD ),
     S_TREXPD,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_BCDYMD,                  // EDIT TYPE FOR THE FIELD
     F_DATE,                    // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

	 {                           // FLD_SYSDT
     PRT_RTCbuff,               // ADDRESS OR OFFSET OF THE FIELD
     6,                         // LENGTH OF THE FIELD
     A_RTC,                     // ADDRESSING TYPE OF THE FIELD
     E_ASCYMD,                  // EDIT TYPE FOR THE FIELD
     F_DATE,                    // FIELD TYPE
     T_ALWAYS                   // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_SYSTM
     &PRT_RTCbuff[6],           // ADDRESS OR OFFSET OF THE FIELD
     6,                         // LENGTH OF THE FIELD
     A_RTC,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_TIME,                    // FIELD TYPE
     T_ALWAYS                   // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_RRN
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRRRN ),
     S_TRRRN,                   // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_AUTH
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRAUTH ),
     S_TRAUTH,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_RSPC
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRRSPC ),
     S_TRRSPC,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_AMT1
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRBASE ),
     S_TRBASE,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_TIP
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRTIP ),
     S_TRTIP,                   // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_DCD1
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRDESC[0] ),
     1,                         // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_DTX1
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRDESC[0] ),
     S_DSTXT,                   // LENGTH OF THE FIELD
     A_DSC,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_DCD2
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRDESC[1] ),
     1,                         // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_DTX2
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRDESC[1] ),
     S_DSTXT,                   // LENGTH OF THE FIELD
     A_DSC,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_DCD3
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRDESC[2] ),
     1,                         // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_DTX3
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRDESC[2] ),
     S_DSTXT,                   // LENGTH OF THE FIELD
     A_DSC,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_DCD4
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRDESC[3] ),
     1,                         // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_DTX4
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRDESC[3] ),
     S_DSTXT,                   // LENGTH OF THE FIELD
     A_DSC,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_NAME
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRTRK1 ),
     S_TRTRK1,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_NAME,                    // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_VISATID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQVISATID ),
     S_AQVISATID,               // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_AQID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRAQID ),
     1,                         // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_AQNAME
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQNAME ),
     S_AQNAME,                  // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_SERNO
     (UBYTE *)TERM.TERMSer,     // ADDRESS OR OFFSET OF THE FIELD
     sizeof( TERM.TERMSer ),    // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     }
    ,

    {                           // FLD_TILLNO
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRTILLNO ),
     S_TRTILLNO,                // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_SALEVCNT
     TERMTOTALS.TOTBASIC.TOTDBVCNT, // ADDRESS OR OFFSET OF THE FIELD
     S_TOTDBVCNT,                // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_SALEVAMT
     TERMTOTALS.TOTBASIC.TOTDBVAMT, // ADDRESS OR OFFSET OF THE FIELD
     S_TOTDBVAMT,               // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_AMT2,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },


    {                           // FLD_HOSTTEXT1
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRHOSTTEXT[0] ),
     S_TRHOSTTEXT - 20,         // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_HOSTTEXT2
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRHOSTTEXT[20] ),
     S_TRHOSTTEXT - 20,         // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_FEE
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRSURC ),
     S_TRBASE,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_TOTF
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRTOTF ),
     S_TRBASE,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_CUSTOM1
     ( UBYTE * ) NULL,          // ADDRESS OR OFFSET OF THE FIELD
     0,                         // FIELD LENGTH/USE AS INDEX OF CUSTOM RTN
     A_CUS,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_CUSTOM2
     ( UBYTE * ) NULL,          // ADDRESS OR OFFSET OF THE FIELD
     1,                         // FIELD LENGTH/USE AS INDEX OF CUSTOM RTN
     A_CUS,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_CUSTOM3
     ( UBYTE * ) NULL,          // ADDRESS OR OFFSET OF THE FIELD
     2,                         // FIELD LENGTH/USE AS INDEX OF CUSTOM RTN
     A_CUS,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_CUSTOM4
     ( UBYTE * ) NULL,          // ADDRESS OR OFFSET OF THE FIELD
     3,                         // FIELD LENGTH/USE AS INDEX OF CUSTOM RTN
     A_CUS,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_AQNUM
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQNAME ),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_PNAME
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQPNAME ),
     S_AQPNAME,                 // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_DMODE
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQDMODE ),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },


    {                           // FLD_PTTEL
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.trans.primary.phoneNum),
     S_AQPTTEL,                 // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_FF                       // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_PTTRY
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.trans.primary.trys),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_PTCON
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.trans.primary.timeout),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_STTEL
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.trans.secondary.phoneNum),
     S_AQSTTEL,                 // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_FF                       // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_STTRY
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.trans.secondary.trys),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_STCON
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.trans.secondary.timeout),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_PSTEL
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.settle.primary.phoneNum),
     S_AQPSTEL,                 // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_FF                       // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_PSTRY
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.settle.primary.trys),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_PSCON
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.settle.primary.timeout),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_SSTEL
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.settle.secondary.phoneNum),
     S_AQSSTEL,                 // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_FF                       // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_SSTRY
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.settle.secondary.trys),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_SSCON
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.settle.secondary.timeout),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_MODEM
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.aqModem),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_SMODEM
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQSMODEM ),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },


    {                           // FLD_V2MID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2MERCH ),
     S_V2MERCH,                 // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                     // TEST TYPE FOR THE FIELD
    },

    {                           // FLD_V2NUM
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2NUM ),
     1,                         // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
    },

    {                           // FLD_V2AQID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2AQID ),
     1,                         // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
    },

    {                           // FLD_V2NAME
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2MERCHNAME ),
     S_V2MERCHNAME,             // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           // FLD_V2CITYST
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2CITYST ),
     S_V2CITYST,                // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           // FLD_V2STATE
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2STATE ),
     S_V2STATE,                 // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           // FLD_V2BNUM
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2BNUM ),
     S_V2BNUM,                  // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           // FLD_V2CAT
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2CATEG ),
     S_V2CATEG,                 // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           // FLD_V2LOC
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2LOCAT ),
     S_V2LOCAT,                 // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           // FLD_V21STLOC
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V21STLOCAT ),
     1,                         // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           // FLD_V2SER
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2TSER ),
     S_V2TSER,                  // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           // FLD_V2CITY
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2CITY ),
     S_V2CITY,                  // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           // FLD_V2CTRY
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2COUNTRY ),
     S_V2COUNTRY,               // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           // FLD_V2SEC
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2SECURITY ),
     S_V2SECURITY,              // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           // FLD_V2CUR
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2CURRENCY ),
     S_V2CURRENCY,              // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           // FLD_V2ZONE
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2TIMEZONE ),
     S_V2TIMEZONE,              // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           // FLD_V2APPID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2APPID ),
     S_V2APPID,                 // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           // FLD_V2APPVER
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2APPVER ),
     S_V2APPVER,                // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           // FLD_V2STORE
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2STORE ),
     S_V2STORE,                 // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           // FLD_V2TERM
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2TERM ),
     S_V2TERM,                  // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },



    {                           // FLD_TDBAL
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRTIP ),
     S_TRTIP,                   // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_FF                       // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_TAX
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRTAX ),
     S_TRBASE,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_BASEAMT
     TERMTOTALS.TOTBASEAMT,     // ADDRESS OR OFFSET OF THE FIELD
     S_TOTBASEAMT,              // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_AMT2,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_TAXAMT
     TERMTOTALS.TOTTAXAMT,      // ADDRESS OR OFFSET OF THE FIELD
     S_TOTTAXAMT,               // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_AMT2,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_HSTPRDATA1
     ( UBYTE * ) HSTPRDATA,     // ADDRESS OR OFFSET OF THE FIELD
     20,                        // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHRTRNC,                 // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_HSTPRDATA2
     ( UBYTE * ) & HSTPRDATA[20],   // ADDRESS OR OFFSET OF THE FIELD
     20,                        // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHRTRNC,                 // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_AMT3
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRBAR ),
     S_TRBASE,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_BARAMT
     TERMTOTALS.TOTBARAMT,      // ADDRESS OR OFFSET OF THE FIELD
     S_TOTBARAMT,               // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_AMT2,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_CASH
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRCASHBK ),
     S_TRBASE,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_MSURC
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRMSURC ),
     S_TRBASE,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_AVSRSP
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRAVSRSP ),
     S_TRAVSRSP,                // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_CASHCNT
     TERMTOTALS.TOTCBCNT,       // ADDRESS OR OFFSET OF THE FIELD
     S_TOTCBCNT,                // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_CASHAMT
     TERMTOTALS.TOTCBAMT,       // ADDRESS OR OFFSET OF THE FIELD
     S_TOTCBAMT,                // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_AMT2,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_CUSTOM5
     ( UBYTE * ) NULL,          // ADDRESS OR OFFSET OF THE FIELD
     4,                         // FIELD LENGTH/USE AS INDEX OF CUSTOM RTN
     A_CUS,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_CUSTOM6
     ( UBYTE * ) NULL,          // ADDRESS OR OFFSET OF THE FIELD
     5,                         // FIELD LENGTH/USE AS INDEX OF CUSTOM RTN
     A_CUS,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_CUSTOM7
     ( UBYTE * ) NULL,          // ADDRESS OR OFFSET OF THE FIELD
     6,                         // FIELD LENGTH/USE AS INDEX OF CUSTOM RTN
     A_CUS,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_CUSTOM8
     ( UBYTE * ) NULL,          // ADDRESS OR OFFSET OF THE FIELD
     7,                         // FIELD LENGTH/USE AS INDEX OF CUSTOM RTN
     A_CUS,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_RCPTHDR4
     USRDTAB,                   // ADDRESS OR OFFSET OF THE FIELD
     USRDSZE,                   // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_SHIFT
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRSHIFT ),
     S_TRSHIFT,                 // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

	 {                           // FLD_ACHSVC
     TCONF.TCACHSVC,            // ADDRESS OR OFFSET OF THE FIELD
     S_TCACHSVC,                // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     }
};

// Second Field Definition Table This is past the Reserved section
// The order of the following array should match ENFLDDEF
// Furthermore, the order should not be altered, or downloaded
// reports will not print properly.
// This table will support fields from 231 to 255
const struct FLDDEF FLDTABDEF2[] = {

    {                           // FLD_AMTDUE
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TROPTAMT ),
     S_TROPTAMT,                // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_RCPTFTR1
     FTTAB.FTFOOTER1,           // ADDRESS OR OFFSET OF THE FIELD
     S_FOOTER,                  // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_RCPTFTR2
     FTTAB.FTFOOTER2,           // ADDRESS OR OFFSET OF THE FIELD
     S_FOOTER,                  // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_RCPTFTR3
     FTTAB.FTFOOTER3,           // ADDRESS OR OFFSET OF THE FIELD
     S_FOOTER,                  // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_RCPTFTR4
     FTTAB.FTFOOTER4,           // ADDRESS OR OFFSET OF THE FIELD
     S_FOOTER,                  // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_TIPPCT1
     ( UBYTE * ) & TCONF.TCTIPPCT_1,    // ADDRESS OR OFFSET OF THE FIELD
     1,                         // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_TIPPCT2
     ( UBYTE * ) & TCONF.TCTIPPCT_2,    // ADDRESS OR OFFSET OF THE FIELD
     1,                         // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_TIPPCT3
     ( UBYTE * ) & TCONF.TCTIPPCT_3,    // ADDRESS OR OFFSET OF THE FIELD
     1,                         // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

// kaldýrýlacak {
    {                           // FLD_SUGTIP1
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct issuer_rec, ISFLOOR ),
     S_SUGTIP,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_SUGTIP2
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct issuer_rec, ISFLOOR ),
     S_SUGTIP,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_SUGTIP3
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct issuer_rec, ISFLOOR ),
     S_SUGTIP,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },
// }
    {                           // FLD_CSTMSG
     ( UBYTE * ) CSTMSG,        // ADDRESS OR OFFSET OF THE FIELD
     sizeof( CSTMSG ),          // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHRTRNC,                 // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     }
    ,

    {                           // FLD_CSTMSG2
     ( UBYTE * ) CSTMSG2,       // ADDRESS OR OFFSET OF THE FIELD
     sizeof( CSTMSG2 ),         // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHRTRNC,                 // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     }
    ,

    {                           // FLD_CSTMSG3
     ( UBYTE * ) CSTMSG3,       // ADDRESS OR OFFSET OF THE FIELD
     sizeof( CSTMSG3 ),         // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHRTRNC,                 // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     }
    ,

    {                           // FLD_CSTMSG4
     ( UBYTE * ) CSTMSG4,       // ADDRESS OR OFFSET OF THE FIELD
     sizeof( CSTMSG4 ),         // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHRTRNC,                 // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     }
    ,

    {                           // FLD_CSTMSG5
     ( UBYTE * ) CSTMSG5,       // ADDRESS OR OFFSET OF THE FIELD
     sizeof( CSTMSG5 ),         // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHRTRNC,                 // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     }
    ,

    {                           // FLD_CSTMSG6
     ( UBYTE * ) CSTMSG6,       // ADDRESS OR OFFSET OF THE FIELD
     sizeof( CSTMSG6 ),         // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHRTRNC,                 // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     }
    ,

};

// Field Definition Table
// The order of the following array should match ENFLDEXT
// Furthermore, the order should not be altered, or downloaded
// reports will not print properly.
// This table will support up to 160 fields
const struct FLDDEF FLDTABEXT[] = {

    {                           // not_used
     0,                         // ADDRESS OR OFFSET OF THE FIELD
     0,                         // LENGTH OF THE FIELD
     A_NONE,                    // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_TOTAPPROVAL                              
     (UBYTE *) & PFTAB.TOTAPPROVAL, // ADDRESS OR OFFSET OF THE FIELD           
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     }
     ,

    {                           // FLD_TOTDENIALS                              
     (UBYTE *) & PFTAB.TOTDENIALS,  // ADDRESS OR OFFSET OF THE FIELD           
     2,                     // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           // FLD_TOTMANUAL                              
     (UBYTE *) & PFTAB.TOTMANUAL,   // ADDRESS OR OFFSET OF THE FIELD           
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           // FLD_TOTSWIPE                              
     (UBYTE *) & PFTAB.TOTSWIPE,    // ADDRESS OR OFFSET OF THE FIELD           
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           // FLD_TOTCOUNT                              
     (UBYTE *) & PFTAB.TOTCOUNT,    // ADDRESS OR OFFSET OF THE FIELD           
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           // FLD_TOTREDIALS                              
     (UBYTE *) & PFTAB.TOTREDIALS,  // ADDRESS OR OFFSET OF THE FIELD           
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           // FLD_TOTTIMEOUT                              
     (UBYTE *) & PFTAB.TOTTIMEOUT,  // ADDRESS OR OFFSET OF THE FIELD           
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           // FLD_TOTRETRANS                              
     (UBYTE *) & PFTAB.TOTRETRANS,  // ADDRESS OR OFFSET OF THE FIELD           
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           // FLD_APPROV_PERC                              
     (UBYTE *) & PFTAB.APPROV_PERC, // ADDRESS OR OFFSET OF THE FIELD           
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           // FLD_DENIAL_PERC                          
     (UBYTE *) & PFTAB.DENIALS_PERC,    // ADDRESS OR OFFSET OF THE FIELD       
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           // FLD_MANUAL_PERC                              
     (UBYTE *) & PFTAB.MANUAL_PERC, // ADDRESS OR OFFSET OF THE FIELD           
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           // FLD_SWIPE_PERC                           
     (UBYTE *) & PFTAB.SWIPE_PERC,  // ADDRESS OR OFFSET OF THE FIELD       
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           // FLD_FLOOR
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct issuer_rec, ISFLOOR ),
     S_ISFLOOR,                 // LENGTH OF THE FIELD
     A_ISS,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_ISREAUTH
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct issuer_rec, ISREAUTH ),
     1,                         // LENGTH OF THE FIELD
     A_ISS,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_ISID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct issuer_rec, ISID ),
     1,                         // LENGTH OF THE FIELD
     A_ISS,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_NII
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQNII ),
     S_AQNII,                   // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },


    {                           // FLD_CURRBATCH
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQBATCUR ),
     S_AQBATCUR,                // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_NEXTBATCH
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQBATNXT ),
     S_AQBATNXT,                // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //  FLD_MAX_SETTLE,
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQSMAXTRIES ),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_ACQUID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQID ),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },


    {                           // FLD_CRNUM
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct card_rec, CRNUM ),
     1,                         // LENGTH OF THE FIELD
     A_CRD,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_CRPANL
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct card_rec, CRPANL ),
     S_CRPANL,                  // LENGTH OF THE FIELD
     A_CRD,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },


    {                           // FLD_CRPANH
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct card_rec, CRPANH ),
     S_CRPANH,                  // LENGTH OF THE FIELD
     A_CRD,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_CRISID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct card_rec, CRISID ),
     1,                         // LENGTH OF THE FIELD
     A_CRD,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_CRAQID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct card_rec, CRAQID ),
     1,                         // LENGTH OF THE FIELD
     A_CRD,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_CRPLEN
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct card_rec, CRPLEN ),
     1,                         // LENGTH OF THE FIELD
     A_CRD,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLG_CRD
     0,                         // ADDRESS OR OFFSET OF THE FIELD
     1,                         // LENGTH OF THE FIELD
     A_CRD,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     T_FLAGX                    // TEST TYPE FOR THE FIELD
     },

};
// Second Field Definition Table This is past the Reserved section
// The order of the following array should match ENFLDEXT
// Furthermore, the order should not be altered, or downloaded
// reports will not print properly.
// This table will support fields from 231 to 255
const struct FLDDEF FLDTABEXT2[] = {

    {                           // FLD_PERIOD                           
     (UBYTE *) & PFTAB.PERIODDATE,  // ADDRESS OR OFFSET OF THE FIELD       
     S_PERIOD,                  // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_ASCYMD,                  // EDIT TYPE FOR THE FIELD                  
     F_DATE,                    // FIELD TYPE                               
     T_ALWAYS                   // TEST TYPE FOR THE FIELD                  
     },


};

const struct FLDDEF FLDTABSERVUS[] =
{
	// FLD_TCNAM1
	{                          
		TCONF.TCNAM1,              // ADDRESS OR OFFSET OF THE FIELD
		S_TCNAM1,                  // LENGTH OF THE FIELD
		A_ACT,                     // ADDRESSING TYPE OF THE FIELD
		E_CHR,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_NULL                     // TEST TYPE FOR THE FIELD
    },
	// FLD_TCNAM2
    {                        
		TCONF.TCNAM2,              // ADDRESS OR OFFSET OF THE FIELD
		S_TCNAM2,                  // LENGTH OF THE FIELD
		A_ACT,                     // ADDRESSING TYPE OF THE FIELD
		E_CHR,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	//FLD_TCNAM3
    {                        
		TCONF.TCNAM3,              // ADDRESS OR OFFSET OF THE FIELD
		S_TCNAM3,                  // LENGTH OF THE FIELD
		A_ACT,                     // ADDRESSING TYPE OF THE FIELD
		E_CHR,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_AQACCID
	{                       
		( UBYTE * ) offsetof( struct acquirer_rec, AQACCID ),
		S_AQACCID,                 // LENGTH OF THE FIELD
		A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
		E_CHR,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_SPC                      // TEST TYPE FOR THE FIELD
    },
	// FLD_AQTID
	{                           // FLD_TID
		( UBYTE * ) offsetof( struct acquirer_rec, AQTID ),
		S_AQTID,                   // LENGTH OF THE FIELD
		A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
		E_CHR,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_SPC                      // TEST TYPE FOR THE FIELD
    },
	// FLD_TRYEAR
	{
		( UBYTE * ) offsetof( struct trandata_rec, TRYEAR ),
		3,                         // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_BCDYMD,                  // EDIT TYPE FOR THE FIELD
		F_DATE,                    // FIELD TYPE
		T_NULL                     // TEST TYPE FOR THE FIELD
    },
	// FLD_TRTIME
    {                          
		( UBYTE * ) offsetof( struct trandata_rec, TRTIME ),
		S_TRTIME,                  // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_BCD,                     // EDIT TYPE FOR THE FIELD
		F_TIME,                    // FIELD TYPE
		T_NULL                     // TEST TYPE FOR THE FIELD
    },
	// FLD_TRSTAN
	{                           
		( UBYTE * ) offsetof( struct trandata_rec, TRSTAN ),
		S_TRSTAN,                  // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_BCD,                     // EDIT TYPE FOR THE FIELD
		F_NUM,                     // FIELD TYPE
		T_NULL                     // TEST TYPE FOR THE FIELD     
	},
	// FLD_TRBATNO
	{                           
		( UBYTE * ) offsetof( struct trandata_rec, TRBATNO ),
		S_TRBATNO,                 // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_CHR,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_TRINV
    {                           
		( UBYTE * ) offsetof( struct trandata_rec, TRINV ),
		S_TRINV,                   // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_CHR,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_TRSRC
	{
		(UBYTE *) offsetof( struct trandata_rec, TRSRC),
		S_TRSRC,
		A_TR,
		E_CHR,
		F_CHR,
		T_NOS
	},
	// FLD_TRCRDTYPE
	{						
		(UBYTE *) offsetof( struct trandata_rec, TRCARDTYPE),
		S_TRCARDTYPE,
		A_TR,
		E_CHR,
		F_CHR,
		T_NOS
	},
	// FLD_TRPAN
	{                     
		( UBYTE * ) offsetof( struct trandata_rec, TRPAN ),
		S_TRPAN,                   // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_PAN,                     // EDIT TYPE FOR THE FIELD
		F_NUM,                     // FIELD TYPE
		T_FF                       // TEST TYPE FOR THE FIELD
    },
	// FLD_TRNAME
	{
		(UBYTE *) offsetof( struct trandata_rec, TRNAME),
		S_TRNAME,
		A_TR,
		E_CHR,
		F_CHR,
		T_NOS
	},
	// FLD_TRNTEXT
	{                           
		( UBYTE * ) ( SecondEvent.TRNTEXT ),
		sizeof( SecondEvent.TRNTEXT ), // LENGTH OF THE FIELD
		A_TRN,                     // ADDRESSING TYPE OF THE FIELD
		E_CHR,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_SPC                      // TEST TYPE FOR THE FIELD
        },

        // FLD_TRNTEXTC
	{                           
		( UBYTE * ) ( CurrentEvent.TRNTEXT ),
		sizeof( CurrentEvent.TRNTEXT ), // LENGTH OF THE FIELD
		A_TRN,                     // ADDRESSING TYPE OF THE FIELD
		E_CHR,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_SPC                      // TEST TYPE FOR THE FIELD
        },

	// FLD_TRTOTAM
	{                         
		( UBYTE * ) offsetof( struct trandata_rec, TRTOTAM ),
		S_TRTOTAM,                 // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_AMT2,                    // EDIT TYPE FOR THE FIELD
		F_AMT,                     // FIELD TYPE
		T_NULL                     // TEST TYPE FOR THE FIELD
        },
	// FLD_TRRRN
	{                           
		( UBYTE * ) offsetof( struct trandata_rec, TRRRN ),
		S_TRRRN,                   // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_CHR,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_ISNAME
	{                          
		( UBYTE * ) offsetof( struct issuer_rec, ISNAME ),
		S_ISNAME,                  // LENGTH OF THE FIELD
		A_ISS,                     // ADDRESSING TYPE OF THE FIELD
		E_CHR,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_SPC                      // TEST TYPE FOR THE FIELD
    },
	//SETTLEMENT FIELDS
	// FLD_TOTALCNT
	{                           
		( UBYTE * ) SETTLETOTALS.TOTALCNT,
		S_CNT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_BCD,                     // EDIT TYPE FOR THE FIELD
		F_NUM,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_TOTALAMT
	{                           
		( UBYTE * ) SETTLETOTALS.TOTALAMT,
		S_AMT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_AMT2,                    // EDIT TYPE FOR THE FIELD
		F_AMT,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_SALECNT
	{                           
		( UBYTE * ) SETTLETOTALS.SALECNT,
		S_CNT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_BCD,                     // EDIT TYPE FOR THE FIELD
		F_NUM,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_SALEAMT
	{                           
		( UBYTE * ) SETTLETOTALS.SALEAMT,
		S_AMT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_AMT2,                    // EDIT TYPE FOR THE FIELD
		F_AMT,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_REFUNDCNT
	{                           
		( UBYTE * ) SETTLETOTALS.REFUNDCNT,
		S_CNT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_BCD,                     // EDIT TYPE FOR THE FIELD
		F_NUM,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_REFUNDAMT
	{                           
		( UBYTE * ) SETTLETOTALS.REFUNDAMT,
		S_AMT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_AMT2,                    // EDIT TYPE FOR THE FIELD
		F_AMT,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_VOIDCNT
	{                           
		( UBYTE * ) SETTLETOTALS.VOIDCNT,
		S_CNT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_BCD,                     // EDIT TYPE FOR THE FIELD
		F_NUM,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_VOIDAMT
	{                           
		( UBYTE * ) SETTLETOTALS.VOIDAMT,
		S_AMT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_AMT2,                    // EDIT TYPE FOR THE FIELD
		F_AMT,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_PAYCNT
	{                           
		( UBYTE * ) SETTLETOTALS.PAYCNT,
		S_CNT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_BCD,                     // EDIT TYPE FOR THE FIELD
		F_NUM,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_PAYAMT
	{                           
		( UBYTE * ) SETTLETOTALS.PAYAMT,
		S_AMT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_AMT2,                    // EDIT TYPE FOR THE FIELD
		F_AMT,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_DMCRCNT
	{                           
		( UBYTE * ) SETTLETOTALS.DMCRCNT,
		S_CNT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_BCD,                     // EDIT TYPE FOR THE FIELD
		F_NUM,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_DMCRAMT
	{                           
		( UBYTE * ) SETTLETOTALS.DMCRAMT,
		S_AMT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_AMT2,                    // EDIT TYPE FOR THE FIELD
		F_AMT,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_INTVISACRCNT
	{                           
		( UBYTE * ) SETTLETOTALS.INTVISACRCNT,
		S_CNT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_BCD,                     // EDIT TYPE FOR THE FIELD
		F_NUM,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_INTVISACRAMT
	{                           
		( UBYTE * ) SETTLETOTALS.INTVISACRAMT,
		S_AMT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_AMT2,                    // EDIT TYPE FOR THE FIELD
		F_AMT,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_INTEUROCRCNT
	{                           
		( UBYTE * ) SETTLETOTALS.INTEUROCRCNT,
		S_CNT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_BCD,                     // EDIT TYPE FOR THE FIELD
		F_NUM,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_INTEUROCRAMT
	{                           
		( UBYTE * ) SETTLETOTALS.INTEUROCRAMT,
		S_AMT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_AMT2,                    // EDIT TYPE FOR THE FIELD
		F_AMT,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_DMDBCNT
	{                           
		( UBYTE * ) SETTLETOTALS.DMDBCNT,
		S_CNT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_BCD,                     // EDIT TYPE FOR THE FIELD
		F_NUM,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_DMDBAMT
	{                           
		( UBYTE * ) SETTLETOTALS.DMDBAMT,
		S_AMT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_AMT2,                    // EDIT TYPE FOR THE FIELD
		F_AMT,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_INTDBCNT
	{                           
		( UBYTE * ) SETTLETOTALS.INTDBCNT,
		S_CNT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_BCD,                     // EDIT TYPE FOR THE FIELD
		F_NUM,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_INTDBAMT
	{                           
		( UBYTE * ) SETTLETOTALS.INTDBAMT,
		S_AMT,					   // LENGTH OF THE FIELD
		A_ACT,					   // ADDRESSING TYPE OF THE FIELD
		E_AMT2,                    // EDIT TYPE FOR THE FIELD
		F_AMT,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },


	// FOOTERS
	// FLD_FTFOOTER1
	{                         
		FTTAB.FTFOOTER1,           // ADDRESS OR OFFSET OF THE FIELD
		S_FOOTER,                  // LENGTH OF THE FIELD
		A_ACT,                     // ADDRESSING TYPE OF THE FIELD
		E_CHR,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_FTFOOTER2
	{                         
		FTTAB.FTFOOTER2,           // ADDRESS OR OFFSET OF THE FIELD
		S_FOOTER,                  // LENGTH OF THE FIELD
		A_ACT,                     // ADDRESSING TYPE OF THE FIELD
		E_CHR,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_FTFOOTER3
	{                         
		FTTAB.FTFOOTER3,           // ADDRESS OR OFFSET OF THE FIELD
		S_FOOTER,                  // LENGTH OF THE FIELD
		A_ACT,                     // ADDRESSING TYPE OF THE FIELD
		E_CHR,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_FTFOOTER4
	{                         
		FTTAB.FTFOOTER4,           // ADDRESS OR OFFSET OF THE FIELD
		S_FOOTER,                  // LENGTH OF THE FIELD
		A_ACT,                     // ADDRESSING TYPE OF THE FIELD
		E_CHR,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLG_TCONF
	{                           
		( UBYTE * ) & TCONF,       // ADDRESS OR OFFSET OF THE FIELD
		1,                         // LENGTH OF THE FIELD
		A_ACT,                     // ADDRESSING TYPE OF THE FIELD
		E_NONE,                    // EDIT TYPE FOR THE FIELD
		F_NONE,                    // FIELD TYPE
		T_FLAG                     // TEST TYPE FOR THE FIELD
    },
    // FLG_TRSTATUS
    {                           
		( UBYTE * ) offsetof( struct trandata_rec, TRSTATUS ),                
		1,                         // LENGTH OF THE FIELD
		A_ACT,                      // ADDRESSING TYPE OF THE FIELD
		E_NONE,                    // EDIT TYPE FOR THE FIELD
		F_NONE,                    // FIELD TYPE
		T_FLAG                     // TEST TYPE FOR THE FIELD
    },

#ifdef MAKE_EMV
	// FLD_AID
    {                           
		( UBYTE * ) offsetof( struct trandata_rec, TRAID ),
		S_TRAID,                   // fld len used for null testing
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_VARHEX,                  // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_NULL                     // TEST TYPE FOR THE FIELD
    },
	// FLD_TRAC
    {                           
		( UBYTE * ) offsetof( struct trandata_rec, TRAC ),
		S_TRAC,                    // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_BCD,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_NULL                     // TEST TYPE FOR THE FIELD
    },

     {    //  FLD_APPLABEL
        ( UBYTE * ) AppLabel,	// ADDRESS OR OFFSET OF THE FIELD
	 16,			// LENGTH OF THE FIELD
	 A_ACT,			// ADDRESSING TYPE OF THE FIELD
	 E_CHR,		// EDIT TYPE FOR THE FIELD
	 F_CHR,			// FIELD TYPE
	 T_NOS			// TEST TYPE FOR THE FIELD
    }
         ,
#endif
    // FLD_HSTPRDATA
      {                           
          ( UBYTE * ) HSTPRDATA,
          S_HSTPRDATA,               // LENGTH OF THE FIELD
          A_ACT,                     // ADDRESSING TYPE OF THE FIELD
          E_CHRTRNC,                 // EDIT TYPE FOR THE FIELD
          F_CHR,                     // FIELD TYPE
          T_NULL                     // TEST TYPE FOR THE FIELD
      },
      
      // FLD_TCRCNT
      {                               
          TERMTOTALS.TOTBASIC.TOTCRCNT, // ADDRESS OR OFFSET OF THE FIELD
           S_TOTCRCNT,                // LENGTH OF THE FIELD
           A_ACT,                     // ADDRESSING TYPE OF THE FIELD
           E_BCD,                     // EDIT TYPE FOR THE FIELD
           F_NUM,                     // FIELD TYPE
           T_NULL                     // TEST TYPE FOR THE FIELD
      },
      // FLD_TCRAMT
     {                               
           TERMTOTALS.TOTBASIC.TOTCRAMT, // ADDRESS OR OFFSET OF THE FIELD
           S_TOTCRAMT,                // LENGTH OF THE FIELD
           A_ACT,                     // ADDRESSING TYPE OF THE FIELD
           E_AMT2,                    // EDIT TYPE FOR THE FIELD
           F_AMT,                     // FIELD TYPE
           T_NULL                     // TEST TYPE FOR THE FIELD
     },

    // FLD_REFCNT
    {                           
           TERMTOTALS.TOTBASIC.TOTRFCNT,  // ADDRESS OR OFFSET OF THE FIELD
           S_TOTCRCNT,                // LENGTH OF THE FIELD
           A_ACT,                     // ADDRESSING TYPE OF THE FIELD
           E_BCD,                     // EDIT TYPE FOR THE FIELD
           F_NUM,                     // FIELD TYPE
           T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           // FLD_REFAMT
           TERMTOTALS.TOTBASIC.TOTRFAMT,  // ADDRESS OR OFFSET OF THE FIELD
           S_TOTCRAMT,                // LENGTH OF THE FIELD
           A_ACT,                     // ADDRESSING TYPE OF THE FIELD
           E_AMT2,                    // EDIT TYPE FOR THE FIELD
           F_AMT,                     // FIELD TYPE
           T_NULL                     // TEST TYPE FOR THE FIELD
     },
    
	// FLD_HOSTTEXT0
	{                           
		( UBYTE * ) offsetof( struct trandata_rec, TRHOSTTEXT[0] ),
		S_TRHOSTTEXT - 20,         // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_CHR,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_HOSTTEXT20
    {                           
		( UBYTE * ) offsetof( struct trandata_rec, TRHOSTTEXT[20] ),
		S_TRHOSTTEXT - 20,         // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_CHR,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },
	// FLD_RSPTXT
    {                           
		( UBYTE * ) offsetof( struct trandata_rec, TRRSPTXT ),
		S_TRRSPTXT,                // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_CHR,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_NOS                      // TEST TYPE FOR THE FIELD
    },

	// FLD_TRFLAG
	{                           
		( UBYTE * ) offsetof( struct trandata_rec, TRFLAG ),
		1,                // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_CHR,                     // EDIT TYPE FOR THE FIELD
		F_CHR,                     // FIELD TYPE
		T_NULL                     // TEST TYPE FOR THE FIELD
      },

	// FLD_INSTNUM
	{							// 88 FLD_INSTNUM
	 ( UBYTE * ) offsetof( struct trandata_rec, TRINSTNUM ),
	 1,                          // LENGTH OF THE FIELD
     A_TR,						// ADDRESSING TYPE OF THE FIELD
	 E_BCD,						// EDIT TYPE FOR THE FIELD
	 F_CHR,						// FIELD TYPE
	 T_NULL						// TEST TYPE FOR THE FIELD
	},

	// FLD_EXTREAMOUNT
	{							// 80 FLD_EKSTREAMOUNT1
	 &ekstreamount[0],			// ADDRESS OR OFFSET OF THE FIELD
	 6,							// LENGTH OF THE FIELD
	 A_ACT,						// ADDRESSING TYPE OF THE FIELD
	 E_AMT2,					// EDIT TYPE FOR THE FIELD
	 F_AMT,						// FIELD TYPE
	 T_NULL						// TEST TYPE FOR THE FIELD
	},

	// FLD_BILLINGDATE
	{							// 82 FLD_HESAPKESIMDATE
	 billingdate,			   	// ADDRESS OR OFFSET OF THE FIELD
	 sizeof(billingdate),		// LENGTH OF THE FIELD
	 A_ACT,						// ADDRESSING TYPE OF THE FIELD
	 E_ASCYMD,					// EDIT TYPE FOR THE FIELD
	 F_DATE,					// FIELD TYPE
	 T_ALWAYS					// TEST TYPE FOR THE FIELD
	},

#ifdef MAKE_KIB 
      // FLD_AVAIL_LOYALTY_AMT
	{                         
		( UBYTE * ) offsetof( struct trandata_rec, AVAIL_LOYALTY_AMT),
		S_TRTOTAM,                 // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_AMT2,                    // EDIT TYPE FOR THE FIELD
		F_AMT,                     // FIELD TYPE
		T_NULL                     // TEST TYPE FOR THE FIELD
        },

        // FLG_TRLOYALTYSTATUS
        {                           
		( UBYTE * ) offsetof( struct trandata_rec, TRLOYALTYSTATUS ),                
		1,                         // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_CHR,                    // EDIT TYPE FOR THE FIELD
		F_CHR,                    // FIELD TYPE
		T_FLAG                     // TEST TYPE FOR THE FIELD
        },

        // FLD_TRUSEDLOYALTYAMOUNT
        {                         
		( UBYTE * ) offsetof( struct trandata_rec, TRUSEDLOYALTYAMOUNT ),
		S_TRTOTAM,                 // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_AMT2,                    // EDIT TYPE FOR THE FIELD
		F_AMT,                     // FIELD TYPE
		T_NULL                     // TEST TYPE FOR THE FIELD
        },

        // FLD_TRTOTAMAFTPOINT
        {                         
		( UBYTE * ) offsetof( struct trandata_rec, TRTOTAMAFTPOINT ),
		S_TRTOTAM,                 // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_AMT2,                    // EDIT TYPE FOR THE FIELD
		F_AMT,                     // FIELD TYPE
		T_NULL                     // TEST TYPE FOR THE FIELD
        },

        // FLD_TRTOTAMAFTDISCOUNT
         {                         
		( UBYTE * ) offsetof( struct trandata_rec, TRTOTAMAFTDISCOUNT ),
		S_TRTOTAM,                 // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_AMT2,                    // EDIT TYPE FOR THE FIELD
		F_AMT,                     // FIELD TYPE
		T_NULL                     // TEST TYPE FOR THE FIELD
        },

        // FLD_TRDISCOUNTTOTAM
        {                         
		( UBYTE * ) offsetof( struct trandata_rec, TRDISCOUNTTOTAM ),
		S_TRTOTAM,                 // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_AMT2,                    // EDIT TYPE FOR THE FIELD
		F_AMT,                     // FIELD TYPE
		T_NULL                     // TEST TYPE FOR THE FIELD
        },

        // FLD_TRDISCOUNTRATIO
	{                           
		( UBYTE * ) offsetof( struct trandata_rec, TRDISCOUNTRATIO ),
		2,                          // LENGTH OF THE FIELD
		A_TR,                      // ADDRESSING TYPE OF THE FIELD
		E_BCD,                     // EDIT TYPE FOR THE FIELD
		F_NUM,                     // FIELD TYPE
		T_NULL                     // TEST TYPE FOR THE FIELD     
	},

#endif

};


const STRFLDTAB fieldTable[] = 
{
 { FX_DEFAULT, FLDTABDEF, FLDTABDEF2},
 { FX_EXTENDED, FLDTABEXT, FLDTABEXT2},
 { FX_SERVUS, FLDTABSERVUS, NULL}
};

#define NUM_FLDTAB (sizeof(fieldTable)/sizeof(STRFLDTAB))

//-----------------------------------------------------------------------------
//  FindTableType       Find type of Table 
//
//  Parameters:         none
//      
//  Global Inputs:
//
//  Returns:            nothing
//     
//  Notes:
//
//-----------------------------------------------------------------------------
Bool FindFldTableType( TABTYPE tableType )
{
    UBYTE i;
	Bool tableSet;

   for (i=0; i < NUM_FLDTAB ; i++)
   {
     if (tableType == fieldTable[i].Type)
     {
          
          switch (tableType)
          {
             // To select Typedef enum "Default" field table as current table 
             case FX_DEFAULT:
          
                currElement.currentTab.Type = fieldTable[i].Type;
                currElement.currentTab.ptable1 = fieldTable[i].ptable1;
                currElement.currentTab.ptable2 = fieldTable[i].ptable2;

                tableSet = 1 ;
                                     
                break;
             // To select typedef enum "Extended" field table as current table 
             case FX_EXTENDED:

                currElement.currentTab.Type = fieldTable[i].Type;
                currElement.currentTab.ptable1 = fieldTable[i].ptable1;
                currElement.currentTab.ptable2 = fieldTable[i].ptable2;

                tableSet = 1 ;

                break;

			 case FX_SERVUS:

				currElement.currentTab.Type = fieldTable[i].Type;
                currElement.currentTab.ptable1 = fieldTable[i].ptable1;
                currElement.currentTab.ptable2 = fieldTable[i].ptable2;

                tableSet = 1 ;

				 break;
             default: 
            
             // Error messages -"No field table to print". 
             ShowErrMsg( PclTabError );

             // Error, the table is not registered 
             tableSet = 0 ;

          }
      }
       
   }

  return tableSet;
}

//-----------------------------------------------------------------------------
//  FindField           Find the Field Definition in FLDTAB or FLDTAB2
//                      The fields must be below 160 or over 230 
//
//  Parameters:         None
//
//  Global Inputs:      FieldNum - the field to find.
//
//  Returns:            None
//
//  Notes:
//
//-----------------------------------------------------------------------------

void FindField( void )
{
    if ( vFieldNum < MARKER1 )
    { 
        // copy Field Definition base on index FieldNum
        FieldDef = currElement.currentTab.ptable1[vFieldNum];
    }
    else
    {
        FieldDef = currElement.currentTab.ptable2[vFieldNum-231];
    }
}
