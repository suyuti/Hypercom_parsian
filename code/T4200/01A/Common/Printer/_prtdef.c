//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             prtdef.c
//      Print definition
//
//=============================================================================
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkmem.h"
#include "fixdata.h"

#include "struct.h"
#include "comdata.h"
#include "appdata.h"
#include "pcltext.h"

#include "prtdef.h"
#include "prtdef2.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================

#define AQOPT1 ((UBYTE) offsetof(struct acquirer_rec, AQOPT1))
#define AQOPT2 ((UBYTE) offsetof(struct acquirer_rec, AQOPT2))
#define AQOPT3 ((UBYTE) offsetof(struct acquirer_rec, AQOPT3))

#define CROPT1 ((UBYTE) offsetof(struct card_rec, CROPT1))

#define TCOPT1 ((UBYTE) offsetof(struct termconf_rec, TCOPT1))
#define TCOPT2 ((UBYTE) offsetof(struct termconf_rec, TCOPT2))
#define TCOPT3 ((UBYTE) offsetof(struct termconf_rec, TCOPT3))
#define TCOPT4 ((UBYTE) offsetof(struct termconf_rec, TCOPT4))
#define TCOPT5 ((UBYTE) offsetof(struct termconf_rec, TCOPT5))
#define TCOPT6 ((UBYTE) offsetof(struct termconf_rec, TCOPT6))
#define TCOPT7 ((UBYTE) offsetof(struct termconf_rec, TCOPT7))
#define TCLOCOP ((UBYTE) offsetof(struct termconf_rec, TCLOCOP))

#define ISOPT1 ((UBYTE) offsetof(struct issuer_rec, ISOPT1))
#define ISOPT2 ((UBYTE) offsetof(struct issuer_rec, ISOPT2))
#define ISOPT3 ((UBYTE) offsetof(struct issuer_rec, ISOPT3))
#define ISOPT4 ((UBYTE) offsetof(struct issuer_rec, ISOPT4))
#define ISOPT5 ((UBYTE) offsetof(struct issuer_rec, ISOPT5))
#define ISOPT6 ((UBYTE) offsetof(struct issuer_rec, ISOPT6))
#define ISPINTRAN ((UBYTE) offsetof(struct issuer_rec, ISPINTRAN))

#define BATSECUR ((UBYTE) offsetof(struct batch_rec, BATSECUR))


//=============================================================================
// Private function declarations
//=============================================================================


//=============================================================================
// Public data definitions
//=============================================================================
//
// The following set of initializations must be compile
// time constants.  Most of the initializers use an offset
// into a structure.  This offset used to be created using
// pointer subtaction:
//
//      (UBYTE *) ((UBYTE *)&AQTAB[0].AQTID - (UBYTE *)&AQTAB),
//
// Unfortunately, this is not ISO compliant.  However, even ISO
// recognizes the need for offsets into structures at compile
// time and provides the offsetof() macro for this purpose.
// The UNIWARE compiler does not support the offsetof() macro.
//
// Harbison and Steele, "C A Reference Manual" states:
//  If offsetof() is not defined (in a non-ISO implementation),
//  it is often possible to define it as follows:
//
//  #define offsetof(type,memb) ((size_t)&((type *)0)->memb)
//
// We will use a variation on this for the initializers.
// Start with the value 0.
// Cast it to be a pointer to struct
// Use that to reference the desired element
// Take the address of the desired element (relative to 0)
// Cast that as a pointer to UBYTE
//                                                  0
//                           (struct acquirer_rec *)0
//                          ((struct acquirer_rec *)0)->AQTID
//                        &(((struct acquirer_rec *)0)->AQTID)
//              (UBYTE *) &(((struct acquirer_rec *)0)->AQTID)
//
// This will get through all three of the compilers.
//

//! Field Definition Table
//! The order of the following array should match FLDTAB_ENUM
//! Furthermore, the order should not be altered, or downloaded
//! reports will not print properly.
//! This table will support up to 160 fields
const struct FLDDEF FLDTABDEF[] = {
    {                           //! not_used
     0,                         // ADDRESS OR OFFSET OF THE FIELD
     0,                         // LENGTH OF THE FIELD
     A_NONE,                    // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    // Begin (TEST) Acquirer table
  /*  {                           //! FLD_MID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQACCID ),
     S_AQACCID,                 // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
     },
     */
    // End (TEST) Acquirer table 




    {                           //! FLD_RCPT1
     TCONF.TCNAM1,              // ADDRESS OR OFFSET OF THE FIELD
     S_TCNAM1,                  // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     }
    ,

    {                           //! FLD_RCPT2
     TCONF.TCNAM2,              // ADDRESS OR OFFSET OF THE FIELD
     S_TCNAM2,                  // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     }
    ,

    {                           //! FLD_RCPT3
     TCONF.TCNAM3,              // ADDRESS OR OFFSET OF THE FIELD
     S_TCNAM3,                  // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     }
    ,

    {                           //! FLD_TID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQTID ),
     S_AQTID,                   // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_MID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQACCID ),
     S_AQACCID,                 // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_CNAME
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct issuer_rec, ISNAME ),
     S_ISNAME,                  // LENGTH OF THE FIELD
     A_ISS,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_SRVID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRCASHIER ),
     S_TRCASHIER,               // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_PAN
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRPAN ),
     S_TRPAN,                   // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_PAN,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_FF                       // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_TRNNAME
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     ( SecondEvent.TRNTEXT ),
     sizeof( SecondEvent.TRNTEXT ), // LENGTH OF THE FIELD
     A_TRN,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
     }
    ,

    {                           //! FLD_EXPDT
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TREXPD ),
     S_TREXPD,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_BCDYMD,                  // EDIT TYPE FOR THE FIELD
     F_DATE,                    // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_BATNO
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct batch_rec, BATCUR ),
     S_BATCUR,                  // LENGTH OF THE FIELD
     A_BAT,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_INVNO
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRINV ),
     S_TRINV,                   // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_SYSDT
     PRT_RTCbuff,               // ADDRESS OR OFFSET OF THE FIELD
     6,                         // LENGTH OF THE FIELD
     A_RTC,                     // ADDRESSING TYPE OF THE FIELD
     E_ASCYMD,                  // EDIT TYPE FOR THE FIELD
     F_DATE,                    // FIELD TYPE
     T_ALWAYS                   // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_SYSTM
     &PRT_RTCbuff[6],           // ADDRESS OR OFFSET OF THE FIELD
     6,                         // LENGTH OF THE FIELD
     A_RTC,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_TIME,                    // FIELD TYPE
     T_ALWAYS                   // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_TRNDT
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRYEAR ),
     3,                         // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_BCDYMD,                  // EDIT TYPE FOR THE FIELD
     F_DATE,                    // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_TRNTM
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRTIME ),
     S_TRTIME,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_TIME,                    // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_RRN
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRRRN ),
     S_TRRRN,                   // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_AUTH
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRAUTH ),
     S_TRAUTH,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_RSPC
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRRSPC ),
     S_TRRSPC,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_TOTAL
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRTOTAM ),
     S_TRTOTAM,                 // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_AMT1
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRBASE ),
     S_TRBASE,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_TIP
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRTIP ),
     S_TRTIP,                   // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_DCD1
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRDESC[0] ),
     1,                         // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_DTX1
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRDESC[0] ),
     S_DSTXT,                   // LENGTH OF THE FIELD
     A_DSC,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_DCD2
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRDESC[1] ),
     1,                         // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_DTX2
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRDESC[1] ),
     S_DSTXT,                   // LENGTH OF THE FIELD
     A_DSC,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_DCD3
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRDESC[2] ),
     1,                         // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_DTX3
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRDESC[2] ),
     S_DSTXT,                   // LENGTH OF THE FIELD
     A_DSC,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_DCD4
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRDESC[3] ),
     1,                         // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_DTX4
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRDESC[3] ),
     S_DSTXT,                   // LENGTH OF THE FIELD
     A_DSC,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_NAME
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRTRK1 ),
     S_TRTRK1,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_NAME,                    // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_VISATID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQVISATID ),
     S_AQVISATID,               // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_RSPTXT
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRRSPTXT ),
     S_TRRSPTXT,                // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_AQID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRAQID ),
     1,                         // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_AQNAME
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQNAME ),
     S_AQNAME,                  // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_SERNO
     (UBYTE *)TERM.TERMSer,     // ADDRESS OR OFFSET OF THE FIELD
     sizeof( TERM.TERMSer ),    // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     }
    ,

    {                           //! FLD_TILLNO
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRTILLNO ),
     S_TRTILLNO,                // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_SALECNT
     TERMTOTALS.TOTBASIC.TOTDBCNT,  // ADDRESS OR OFFSET OF THE FIELD
     S_TOTDBCNT,                // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_SALEAMT
     TERMTOTALS.TOTBASIC.TOTDBAMT,  // ADDRESS OR OFFSET OF THE FIELD
     S_TOTDBAMT,                // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_AMT2,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_SALEVCNT
     TERMTOTALS.TOTBASIC.TOTDBVCNT, // ADDRESS OR OFFSET OF THE FIELD
     S_TOTDBVCNT,               // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_SALEVAMT
     TERMTOTALS.TOTBASIC.TOTDBVAMT, // ADDRESS OR OFFSET OF THE FIELD
     S_TOTDBVAMT,               // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_AMT2,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_REFCNT
     TERMTOTALS.TOTBASIC.TOTCRCNT,  // ADDRESS OR OFFSET OF THE FIELD
     S_TOTCRCNT,                // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_REFAMT
     TERMTOTALS.TOTBASIC.TOTCRAMT,  // ADDRESS OR OFFSET OF THE FIELD
     S_TOTCRAMT,                // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_AMT2,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_HOSTTEXT1
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRHOSTTEXT[0] ),
     S_TRHOSTTEXT - 20,         // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_HOSTTEXT2
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRHOSTTEXT[20] ),
     S_TRHOSTTEXT - 20,         // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_FEE
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRSURC ),
     S_TRBASE,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_TOTF
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRTOTF ),
     S_TRBASE,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_CUSTOM1
     ( UBYTE * ) NULL,          // ADDRESS OR OFFSET OF THE FIELD
     0,                         // FIELD LENGTH/USE AS INDEX OF CUSTOM RTN
     A_CUS,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_CUSTOM2
     ( UBYTE * ) NULL,          // ADDRESS OR OFFSET OF THE FIELD
     1,                         // FIELD LENGTH/USE AS INDEX OF CUSTOM RTN
     A_CUS,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_CUSTOM3
     ( UBYTE * ) NULL,          // ADDRESS OR OFFSET OF THE FIELD
     2,                         // FIELD LENGTH/USE AS INDEX OF CUSTOM RTN
     A_CUS,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_CUSTOM4
     ( UBYTE * ) NULL,          // ADDRESS OR OFFSET OF THE FIELD
     3,                         // FIELD LENGTH/USE AS INDEX OF CUSTOM RTN
     A_CUS,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_AQNUM
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQNAME ),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_PNAME
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQPNAME ),
     S_AQPNAME,                 // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_DMODE
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQDMODE ),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },


    {                           //! FLD_PTTEL
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.trans.primary.phoneNum),
     S_AQPTTEL,                 // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_FF                       // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_PTTRY
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.trans.primary.trys),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_PTCON
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.trans.primary.timeout),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_STTEL
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.trans.secondary.phoneNum),
     S_AQSTTEL,                 // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_FF                       // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_STTRY
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.trans.secondary.trys),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_STCON
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.trans.secondary.timeout),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_PSTEL
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.settle.primary.phoneNum),
     S_AQPSTEL,                 // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_FF                       // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_PSTRY
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.settle.primary.trys),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_PSCON
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.settle.primary.timeout),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_SSTEL
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.settle.secondary.phoneNum),
     S_AQSSTEL,                 // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_FF                       // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_SSTRY
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.settle.secondary.trys),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_SSCON
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.settle.secondary.timeout),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_MODEM
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     & (((struct acquirer_rec *) 0)->conn.dial.aqModem),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_SMODEM
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQSMODEM ),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },


    {                           //! FLD_V2MID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2MERCH ),
     S_V2MERCH,                 // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                     // TEST TYPE FOR THE FIELD
    },

    {                           //! FLD_V2NUM
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2NUM ),
     1,                         // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
    },

    {                           //! FLD_V2AQID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2AQID ),
     1,                         // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
    },

    {                           //! FLD_V2NAME
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2MERCHNAME ),
     S_V2MERCHNAME,             // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           //! FLD_V2CITYST
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2CITYST ),
     S_V2CITYST,                // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           //! FLD_V2STATE
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2STATE ),
     S_V2STATE,                 // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           //! FLD_V2BNUM
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2BNUM ),
     S_V2BNUM,                  // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           //! FLD_V2CAT
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2CATEG ),
     S_V2CATEG,                 // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           //! FLD_V2LOC
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2LOCAT ),
     S_V2LOCAT,                 // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           //! FLD_V21STLOC
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V21STLOCAT ),
     1,                         // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           //! FLD_V2SER
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2TSER ),
     S_V2TSER,                  // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           //! FLD_V2CITY
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2CITY ),
     S_V2CITY,                  // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           //! FLD_V2CTRY
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2COUNTRY ),
     S_V2COUNTRY,               // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           //! FLD_V2SEC
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2SECURITY ),
     S_V2SECURITY,              // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           //! FLD_V2CUR
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2CURRENCY ),
     S_V2CURRENCY,              // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           //! FLD_V2ZONE
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2TIMEZONE ),
     S_V2TIMEZONE,              // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           //! FLD_V2APPID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2APPID ),
     S_V2APPID,                 // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           //! FLD_V2APPVER
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2APPVER ),
     S_V2APPVER,                // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           //! FLD_V2STORE
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2STORE ),
     S_V2STORE,                 // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },

    {                           //! FLD_V2TERM
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
     offsetof( struct v2_rec, V2TERM ),
     S_V2TERM,                  // LENGTH OF THE FIELD
     A_V2,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_SPC                      // TEST TYPE FOR THE FIELD
    },



    {                           //! FLD_TDBAL
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRTIP ),
     S_TRTIP,                   // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_FF                       // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_TAX
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRTAX ),
     S_TRBASE,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_BASEAMT
     TERMTOTALS.TOTBASEAMT,     // ADDRESS OR OFFSET OF THE FIELD
     //TERMTOTALS.TOTSALAMT,     // ADDRESS OR OFFSET OF THE FIELD
     //S_TOTAMT,
     S_TOTBASEAMT,              // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_AMT2,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_TAXAMT
     TERMTOTALS.TOTTAXAMT,      // ADDRESS OR OFFSET OF THE FIELD
     S_TOTTAXAMT,               // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_AMT2,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_TIPAMT
     TERMTOTALS.TOTTIPAMT,      // ADDRESS OR OFFSET OF THE FIELD
     S_TOTTIPAMT,               // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_AMT2,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_HSTPRDATA
     ( UBYTE * ) HSTPRDATA,     // ADDRESS OR OFFSET OF THE FIELD
     S_HSTPRDATA,               // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHRTRNC,                 // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_HSTPRDATA1
     ( UBYTE * ) HSTPRDATA,     // ADDRESS OR OFFSET OF THE FIELD
     20,                        // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHRTRNC,                 // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_HSTPRDATA2
     ( UBYTE * ) & HSTPRDATA[20],   // ADDRESS OR OFFSET OF THE FIELD
     20,                        // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHRTRNC,                 // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_AMT3
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRBAR ),
     S_TRBASE,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_BARAMT
     TERMTOTALS.TOTBARAMT,      // ADDRESS OR OFFSET OF THE FIELD
     S_TOTBARAMT,               // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_AMT2,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_CASH
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRCASHBK ),
     S_TRBASE,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_MSURC
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRMSURC ),
     S_TRBASE,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_AVSRSP
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRAVSRSP ),
     S_TRAVSRSP,                // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_CASHCNT
     TERMTOTALS.TOTCBCNT,       // ADDRESS OR OFFSET OF THE FIELD
     S_TOTCBCNT,                // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_CASHAMT
     TERMTOTALS.TOTCBAMT,       // ADDRESS OR OFFSET OF THE FIELD
     S_TOTCBAMT,                // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_AMT2,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_CUSTOM5
     ( UBYTE * ) NULL,          // ADDRESS OR OFFSET OF THE FIELD
     4,                         // FIELD LENGTH/USE AS INDEX OF CUSTOM RTN
     A_CUS,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_CUSTOM6
     ( UBYTE * ) NULL,          // ADDRESS OR OFFSET OF THE FIELD
     5,                         // FIELD LENGTH/USE AS INDEX OF CUSTOM RTN
     A_CUS,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_CUSTOM7
     ( UBYTE * ) NULL,          // ADDRESS OR OFFSET OF THE FIELD
     6,                         // FIELD LENGTH/USE AS INDEX OF CUSTOM RTN
     A_CUS,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_CUSTOM8
     ( UBYTE * ) NULL,          // ADDRESS OR OFFSET OF THE FIELD
     7,                         // FIELD LENGTH/USE AS INDEX OF CUSTOM RTN
     A_CUS,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_RCPTHDR4
     USRDTAB,                   // ADDRESS OR OFFSET OF THE FIELD
     USRDSZE,                   // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_SHIFT
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRSHIFT ),
     S_TRSHIFT,                 // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_TIPPCT
     TERMTOTALS.TOTTIPPCT,      // ADDRESS OR OFFSET OF THE FIELD
     S_TOTTIPPCT,               // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_TIPPCT,                  // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_BUSDATE
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRBUSDT ),
     S_TRBUSDT,                 // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_BCDYMD,                  // EDIT TYPE FOR THE FIELD
     F_DATE,                    // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_ACHSVC
     TCONF.TCACHSVC,            // ADDRESS OR OFFSET OF THE FIELD
     S_TCACHSVC,                // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_STAN
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRSTAN ),
     S_TRSTAN,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLG_TCONF
     ( UBYTE * ) & TCONF,       // ADDRESS OR OFFSET OF THE FIELD
     1,                         // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     T_FLAG                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLG_ACQ
     0,                         // ADDRESS OR OFFSET OF THE FIELD
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     T_FLAGX                    // TEST TYPE FOR THE FIELD
     },

    {                           //! FLG_ISS
     0,                         // ADDRESS OR OFFSET OF THE FIELD
     1,                         // LENGTH OF THE FIELD
     A_ISS,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     T_FLAGX                    // TEST TYPE FOR THE FIELD
     },

    {                           //! FLG_TRN
     0,                         // ADDRESS OR OFFSET OF THE FIELD
     1,                         // LENGTH OF THE FIELD
     A_TRN,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     T_FLAG                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLG_BAT
     0,                         // ADDRESS OR OFFSET OF THE FIELD
     1,                         // LENGTH OF THE FIELD
     A_BAT,                     // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     T_FLAG                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLG_TRSTATUS
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
           offsetof( struct trandata_rec, TRSTATUS ),
     1,                         // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     T_FLAG                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_TRBATNO
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
           offsetof( struct trandata_rec, TRBATNO ),
     S_TRBATNO,                 // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

#ifdef MAKE_EMV
    {                           //! FLD_AID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRAID ),
     S_TRAID,                   // fld len used for null testing
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_VARHEX,                  // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     }
    ,

    {                           //! FLD_TC
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRAC ),
     S_TRAC,                    // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     }
    ,
#endif // MAKE_EMV

};

//! Second Field Definition Table This is past the Reserved section
//! The order of the following array should match ENFLDDEF
//! Furthermore, the order should not be altered, or downloaded
//! reports will not print properly.
//! This table will support fields from 231 to 255
const struct FLDDEF FLDTABDEF2[] = {

    {                           //! FLD_AMTDUE
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TROPTAMT ),
     S_TROPTAMT,                // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_RCPTFTR1
     FTTAB.FTFOOTER1,           // ADDRESS OR OFFSET OF THE FIELD
     S_FOOTER,                  // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_RCPTFTR2
     FTTAB.FTFOOTER2,           // ADDRESS OR OFFSET OF THE FIELD
     S_FOOTER,                  // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_RCPTFTR3
     FTTAB.FTFOOTER3,           // ADDRESS OR OFFSET OF THE FIELD
     S_FOOTER,                  // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_RCPTFTR4
     FTTAB.FTFOOTER4,           // ADDRESS OR OFFSET OF THE FIELD
     S_FOOTER,                  // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHR,                     // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_TIPPCT1
     ( UBYTE * ) & TCONF.TCTIPPCT_1,    // ADDRESS OR OFFSET OF THE FIELD
     1,                         // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_TIPPCT2
     ( UBYTE * ) & TCONF.TCTIPPCT_2,    // ADDRESS OR OFFSET OF THE FIELD
     1,                         // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_TIPPCT3
     ( UBYTE * ) & TCONF.TCTIPPCT_3,    // ADDRESS OR OFFSET OF THE FIELD
     1,                         // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },
/*
    {                           //! FLD_SUGTIP1
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRSUGTIP1 ),
     S_SUGTIP,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_SUGTIP2
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRSUGTIP2 ),
     S_SUGTIP,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_SUGTIP3
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct trandata_rec, TRSUGTIP3 ),
     S_SUGTIP,                  // LENGTH OF THE FIELD
     A_TR,                      // ADDRESSING TYPE OF THE FIELD
     E_AMT1,                    // EDIT TYPE FOR THE FIELD
     F_AMT,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },
*/
    {                           //! FLD_CSTMSG
     ( UBYTE * ) CSTMSG,        // ADDRESS OR OFFSET OF THE FIELD
     sizeof( CSTMSG ),          // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHRTRNC,                 // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     }
    ,

    {                           //! FLD_CSTMSG2
     ( UBYTE * ) CSTMSG2,       // ADDRESS OR OFFSET OF THE FIELD
     sizeof( CSTMSG2 ),         // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHRTRNC,                 // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     }
    ,

    {                           //! FLD_CSTMSG3
     ( UBYTE * ) CSTMSG3,       // ADDRESS OR OFFSET OF THE FIELD
     sizeof( CSTMSG3 ),         // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHRTRNC,                 // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     }
    ,

    {                           //! FLD_CSTMSG4
     ( UBYTE * ) CSTMSG4,       // ADDRESS OR OFFSET OF THE FIELD
     sizeof( CSTMSG4 ),         // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHRTRNC,                 // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     }
    ,

    {                           //! FLD_CSTMSG5
     ( UBYTE * ) CSTMSG5,       // ADDRESS OR OFFSET OF THE FIELD
     sizeof( CSTMSG5 ),         // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHRTRNC,                 // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     }
    ,

    {                           //! FLD_CSTMSG6
     ( UBYTE * ) CSTMSG6,       // ADDRESS OR OFFSET OF THE FIELD
     sizeof( CSTMSG6 ),         // LENGTH OF THE FIELD
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD
     E_CHRTRNC,                 // EDIT TYPE FOR THE FIELD
     F_CHR,                     // FIELD TYPE
     T_NOS                      // TEST TYPE FOR THE FIELD
     }
    ,

};

//! Field Definition Table
//! The order of the following array should match ENFLDEXT
//! Furthermore, the order should not be altered, or downloaded
//! reports will not print properly.
//! This table will support up to 160 fields
const struct FLDDEF FLDTABEXT[] = {

    {                           //! not_used
     0,                         // ADDRESS OR OFFSET OF THE FIELD
     0,                         // LENGTH OF THE FIELD
     A_NONE,                    // ADDRESSING TYPE OF THE FIELD
     E_NONE,                    // EDIT TYPE FOR THE FIELD
     F_NONE,                    // FIELD TYPE
     0                          // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_TOTAPPROVAL                              
     (UBYTE *) & PFTAB.TOTAPPROVAL, // ADDRESS OR OFFSET OF THE FIELD           
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     }
     ,

    {                           //! FLD_TOTDENIALS                              
     (UBYTE *) & PFTAB.TOTDENIALS,  // ADDRESS OR OFFSET OF THE FIELD           
     2,                     // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           //! FLD_TOTMANUAL                              
     (UBYTE *) & PFTAB.TOTMANUAL,   // ADDRESS OR OFFSET OF THE FIELD           
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           //! FLD_TOTSWIPE                              
     (UBYTE *) & PFTAB.TOTSWIPE,    // ADDRESS OR OFFSET OF THE FIELD           
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           //! FLD_TOTCOUNT                              
     (UBYTE *) & PFTAB.TOTCOUNT,    // ADDRESS OR OFFSET OF THE FIELD           
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           //! FLD_TOTREDIALS                              
     (UBYTE *) & PFTAB.TOTREDIALS,  // ADDRESS OR OFFSET OF THE FIELD           
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           //! FLD_TOTTIMEOUT                              
     (UBYTE *) & PFTAB.TOTTIMEOUT,  // ADDRESS OR OFFSET OF THE FIELD           
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           //! FLD_TOTRETRANS                              
     (UBYTE *) & PFTAB.TOTRETRANS,  // ADDRESS OR OFFSET OF THE FIELD           
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           //! FLD_APPROV_PERC                              
     (UBYTE *) & PFTAB.APPROV_PERC, // ADDRESS OR OFFSET OF THE FIELD           
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           //! FLD_DENIAL_PERC                          
     (UBYTE *) & PFTAB.DENIALS_PERC,    // ADDRESS OR OFFSET OF THE FIELD       
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           //! FLD_MANUAL_PERC                              
     (UBYTE *) & PFTAB.MANUAL_PERC, // ADDRESS OR OFFSET OF THE FIELD           
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           //! FLD_SWIPE_PERC                           
     (UBYTE *) & PFTAB.SWIPE_PERC,  // ADDRESS OR OFFSET OF THE FIELD       
     2,                         // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_HEX,                     // EDIT TYPE FOR THE FIELD                  
     F_NUM,                     // FIELD TYPE                               
     T_NULL                     // TEST TYPE FOR THE FIELD                  
     },

    {                           //! FLD_FLOOR
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct issuer_rec, ISFLOOR ),
     S_ISFLOOR,                 // LENGTH OF THE FIELD
     A_ISS,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_ISREAUTH
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct issuer_rec, ISREAUTH ),
     1,                         // LENGTH OF THE FIELD
     A_ISS,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_ISID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct issuer_rec, ISID ),
     1,                         // LENGTH OF THE FIELD
     A_ISS,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_NII
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQNII ),
     S_AQNII,                   // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },


    {                           //! FLD_CURRBATCH
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQBATCUR ),
     S_AQBATCUR,                // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_NEXTBATCH
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQBATNXT ),
     S_AQBATNXT,                // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //!  FLD_MAX_SETTLE,
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQSMAXTRIES ),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_ACQUID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct acquirer_rec, AQID ),
     1,                         // LENGTH OF THE FIELD
     A_ACQ,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },


    {                           //! FLD_CRNUM
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct card_rec, CRNUM ),
     1,                         // LENGTH OF THE FIELD
     A_CRD,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_CRPANL
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct card_rec, CRPANL ),
     S_CRPANL,                  // LENGTH OF THE FIELD
     A_CRD,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },


    {                           //! FLD_CRPANH
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct card_rec, CRPANH ),
     S_CRPANH,                  // LENGTH OF THE FIELD
     A_CRD,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_CRISID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct card_rec, CRISID ),
     1,                         // LENGTH OF THE FIELD
     A_CRD,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_CRAQID
     ( UBYTE * )                // ADDRESS OR OFFSET OF THE FIELD
        offsetof( struct card_rec, CRAQID ),
     1,                         // LENGTH OF THE FIELD
     A_CRD,                     // ADDRESSING TYPE OF THE FIELD
     E_BCD,                     // EDIT TYPE FOR THE FIELD
     F_NUM,                     // FIELD TYPE
     T_NULL                     // TEST TYPE FOR THE FIELD
     },

    {                           //! FLD_CRPLEN
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

//! Second Field Definition Table This is past the Reserved section
//! The order of the following array should match ENFLDEXT
//! Furthermore, the order should not be altered, or downloaded
//! reports will not print properly.
//! This table will support fields from 231 to 255
const struct FLDDEF FLDTABEXT2[] = {

    {                           //! FLD_PERIOD                           
     (UBYTE *) & PFTAB.PERIODDATE,  // ADDRESS OR OFFSET OF THE FIELD       
     S_PERIOD,                  // LENGTH OF THE FIELD                      
     A_ACT,                     // ADDRESSING TYPE OF THE FIELD             
     E_ASCYMD,                  // EDIT TYPE FOR THE FIELD                  
     F_DATE,                    // FIELD TYPE                               
     T_ALWAYS                   // TEST TYPE FOR THE FIELD                  
     },


};


const STRFLDTAB fieldTable[] = 
{
 { FX_DEFAULT, FLDTABDEF, FLDTABDEF2},
 { FX_EXTENDED, FLDTABEXT, FLDTABEXT2},

};

#define NUM_FLDTAB (sizeof(fieldTable)/sizeof(STRFLDTAB))


//
//--------------------------------------------------------------------
//       RECEIPT
//--------------------------------------------------------------------
//! Receipt printing
const UBYTE ALIGN(4) RP_RECEIPT_DATA[] = {
//*****************************************************************************
// ICE PAC header image
//*****************************************************************************
    GRPH, 0x50, 0x00,

//*****************************************************************************
// Merchant text header
//*****************************************************************************
    REPORT, RP_HEAD_MRCH,

//*****************************************************************************
// Merchant #
//*****************************************************************************
    IF, CND_ROLL,
        TXT, Merchant,
    ENDIF,
    RGHT,
    
    FLD_F, FLD_MID, FC_TRIM,
    LINE,
    LINE,

//*****************************************************************************
// Card (issuer) name
//*****************************************************************************
    FLD, FLD_CNAME,

//*****************************************************************************
// Cashier (server) number
//*****************************************************************************
    RGHT,
    IF, FLG_TCONF, TCOPT1, TC1_CASHIER, AND, CND_ROLL,
        IF, FLG_TCONF, TCOPT4, TC4_SRV_PRM,
            TXT, Srv,
        ELSE,
            TXT, Csh,
        ENDIF,
    ENDIF,
    FLD_F, FLD_SRVID, FC_TRIM,

//*****************************************************************************
// Till number
//*****************************************************************************
    IF, FLG_TCONF, TCOPT1, TC1_TILL,
        IF, CND_ROLL,
            TXT, Till,
        ELSE,
            TXT, Csh, ' ',
        ENDIF,
        FLD_F, FLD_TILLNO, FC_TRIM,
    ENDIF,
    LINE,

//*****************************************************************************
// Card PAN (Primary Account Number)
//*****************************************************************************
    IF, TRN_DBCD, OR, TRN_BALI,
        MLTCHR, 12, '*', FLD_S, FLD_PAN, ( UBYTE )-4,   // Always truncate PAN for these
    ELSE,
        IF, FLG_TCONF, TCOPT6, TC6_TRUNCPAN,    // PAN truncation enabled?
            IF, FLG_TCONF, TCOPT6, TC6_NOSECOND, OR, OPT5,
                // This is either 2-part paper or the customer copy
                MLTCHR, 12, '*', FLD_S, FLD_PAN, ( UBYTE )-4,   // Truncated PAN
            ELSE,
                FLD, FLD_PAN,               // Merchant copy of receipt
            ENDIF,
        ELSE,
            FLD, FLD_PAN,               // PAN truncation disabled
        ENDIF,
    ENDIF,
    IF, CND_MAN, ' ', '*', ENDIF,
    LINE,

//*****************************************************************************
// Application ID (for ICC card)
//*****************************************************************************
#ifdef MAKE_EMV
    IF,FLD_AID,
    'A','I','D',':',' ',FLD,FLD_AID,LINE,
    ENDIF,
//    IF,FLD_TC,
//    'T','C',':',' ',FLD,FLD_TC,LINE,
//    ENDIF,
#endif // MAKE_EMV


//*****************************************************************************
// Empty line
//*****************************************************************************
    IF, NOT, CND_ROLL, 
        LINE, 
    ENDIF,

//*****************************************************************************
// Transaction Name
//*****************************************************************************
    IF, CND_ROLL, 
        DOUBLE, 
    ENDIF,

    IF, TRN_VOID,
        TXT, Void_s,
    ELSE,
        FLD_F, FLD_TRNNAME, FC_TRIM,
    ENDIF,

//*****************************************************************************
// Card expiration date
//*****************************************************************************
    NORMAL,
    IF, FLD_EXPDT,
        IF, FLG_TCONF, TCOPT6, TC6_NOSECOND, OR, OPT5,
            // This is either 2-part paper or the customer copy.
            IF, FLG_TCONF, TCOPT6, TC6_TRUNCPAN,    // PAN truncation enabled?
                // Yes, don't show expiration date on customer receipt
            ELSE,
                // No, so show expiration date.
                RGHT, 
                IF, CND_ROLL, 
                    'E', 'X', 'P', '.', ':', ' ', ' ', 
                ENDIF, 
                FLD_F, FLD_EXPDT, FD_MDYS,
            ENDIF,
        ELSE,
            // Show expiration date on the merchant receipt.
            RGHT, 
            IF, CND_ROLL, 
                'E', 'X', 'P', '.', ':', ' ', ' ', 
            ENDIF, 
            FLD_F, FLD_EXPDT, FD_MDYS,
        ENDIF,
    ENDIF,
    LINE,


//*****************************************************************************
// Batch Number
//*****************************************************************************
    IF, CND_ROLL,
        TXT, Batch,
    ENDIF, 
    FLD, FLD_BATNO,

    RGHT, 
    IF, CND_ROLL,
        TXT, Invoice,
    ENDIF, 
    FLD, FLD_INVNO,




//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
// *** PRE-PRINT STOP ***
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
    PPSTOP,


//*****************************************************************************
// Transaction Date/Time
//*****************************************************************************
    IF, CND_ROLL,
        TXT, Date,
    ENDIF, 
    FLD_F, FLD_TRNDT, FD_TEXT,
    
    IF, FLG_TCONF, TCOPT1, TC1_PR_TIME,
        IF, CND_ROLL, RGHT,
            TXT, Time,
        ENDIF,
        ' ', FLD_FL, FLD_TRNTM, FT_MIL, 5,
    ENDIF,
    LINE,

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//  IF NOT REJECTED
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
    IF, NOT, CND_ROLL, OR, NOT, CND_REJ,


//*****************************************************************************
// RRN (Retrival Reference Number)
//*****************************************************************************
        IF, FLD_RRN,
            IF, CND_ROLL, TXT, Rrn,ENDIF,
            FLD_F, FLD_RRN, FC_TRIM,
        ENDIF,
        
//*****************************************************************************
// Authorisation code
//*****************************************************************************
        IF, FLD_AUTH,
            RGHT, 
            IF, CND_ROLL, TXT, AuthN, ENDIF,
            FLD, FLD_AUTH,
        ENDIF,
        LINE,

//*****************************************************************************
// AVS (Address Verification System) response
//*****************************************************************************
        IF, FLD_AVSRSP,
            TXT, Avs,
            FLD_F, FLD_AVSRSP, FC_TRIM,LINE,
        ENDIF,

//*****************************************************************************
// Host print text
//*****************************************************************************
        IF, FLD_HSTPRDATA,
            IF, CND_ROLL, 
                LINE, 
            ENDIF,
            FLD, FLD_HSTPRDATA,
            IF, CND_ROLL, 
                LINE, 
            ENDIF,
            LINE,
        ENDIF,

//*****************************************************************************
// Transaction amounts
//*****************************************************************************

//--------------------------------
// Balance enquiry transaction:
        IF, TRN_BALI,
            LINE, LINE,
            IF, CND_ROLL, 
                DOUBLE, 
            ENDIF,
            TXT, AvailB,':',
            LINE,RGHT, 
            FLD, FLD_TOTAL, 
            LINE,
            IF, FLD_TDBAL,
                IF, CND_ROLL, 
                    DOUBLE, 
                ENDIF,
                TXT, PclToday,
                RGHT, FLD, FLD_TDBAL,
            ENDIF,
            LINE,
        ELSE,

//--------------------------------
// Base amount for Refund transaction only:
            IF, TRN_REFD,
                IF, FLD_TAX, OR, FLD_TIP,
                    TXT, Base,
                    RGHT, FLD_S, FLD_AMT1, 4, LINE,
                ENDIF,
//--------------------------------
// Base amount for other transactions:
            ELSE,
                IF, CND_TIP, OR, FLD_TAX, OR, FLD_AMT3,
                    TXT, Base,
                    RGHT, FLD_S, FLD_AMT1, 4, LINE,
                ENDIF,
            ENDIF,
//--------------------------------
// Tax amount (not BalInq)
            IF, FLD_TAX,
                TXT, Tax,
                RGHT, FLD_S, FLD_TAX, 4, LINE,
            ENDIF,
//--------------------------------
// Bar amount (not BalInq)
            IF, CND_ROLL,
                IF, FLD_AMT3,
                    TXT, Bar,
                    RGHT, FLD_S, FLD_AMT3, 4, LINE,
                ENDIF,
            ENDIF,

//--------------------------------
// Restaurant processing amounts:
            IF, CND_TIP,
                IF, CND_ROLL,
// Cash Back amount
                    IF, FLD_CASH,
                        TXT, CashBack,
                        RGHT, FLD, FLD_CASH, LINE,
                    ENDIF,
// FEE amount
                    IF, FLD_FEE,
                        TXT, Fee,
                        RGHT, FLD, FLD_FEE, LINE,
                    ENDIF,
// Merchant Surcharge amount
                    IF, FLD_MSURC,
                        TXT, Surcharge,
                        RGHT, FLD_S, FLD_MSURC, 4, LINE,
                    ENDIF,
                ENDIF,

// Tip amount
                IF, NOT, TRN_REFD,
                    TXT, Tip,
                ELSE,
                    IF, FLD_TIP,
                        TXT, Tip,
                    ENDIF,
                ENDIF,

                IF, FLD_TIP,
                    RGHT, FLD_S, FLD_TIP, 4,
                ENDIF,
                LINE,
// _____
// TOTAL amount
                IF, NOT, TRN_REFD, 
                    RGHT, MLTCHR, 9, '_', 
                    LINE, 
                ENDIF,

                IF, CND_ROLL, 
                    DOUBLE,
                ENDIF,
                TXT, Total, 
                
                IF, FLD_TIP, OR, TRN_REFD, 
                    RGHT, FLD_S, FLD_TOTAL, 4, 
                ENDIF,
// Suggested Tips guide                
                /*IF, NOT, FLD_TIP, AND, NOT, TRN_REFD, AND, NOT, TRN_VOID,
                    IF, FLD_SUGTIP1, OR, FLD_SUGTIP2, OR, FLD_SUGTIP3,
                        LINE, LINE, CNTR,
                        TXT, TipGuide,
                        LINE,
                    ENDIF,
                    CNTR,
                    IF, FLD_SUGTIP1,
                        FLD_F, FLD_TIPPCT1, FN_TRIM, '%', '=',
                        FLD_F, FLD_SUGTIP1, FA_TRIM, ' ', ' ',
                    ENDIF,
                    IF, FLD_SUGTIP2,
                        FLD_F, FLD_TIPPCT2, FN_TRIM, '%', '=',
                        FLD_F, FLD_SUGTIP2, FA_TRIM, ' ', ' ',
                    ENDIF,
                    IF, FLD_SUGTIP3,
                        FLD_F, FLD_TIPPCT3, FN_TRIM, '%', '=',
                        FLD_F, FLD_SUGTIP3, FA_TRIM, ' ', ' ',
                    ENDIF,
                ENDIF,
            
            */
//--------------------------------
// NON-Restaurant processing amounts:
            ELSE,
                IF, FLD_CASH, OR, FLD_FEE, OR, FLD_MSURC,
// Sale amount
                    IF, FLD_AMT1,
                        TXT, SaleAmt,
                        RGHT, FLD, FLD_AMT1, LINE,
                    ENDIF,
// Cash amount
                    IF, FLD_CASH,
                        TXT, CashBack,
                        RGHT, FLD, FLD_CASH, LINE,
                    ENDIF,
// FEE
                    IF, FLD_FEE,
                        TXT, Fee,
                        RGHT, FLD, FLD_FEE, LINE,
                    ENDIF,
// Merchant Surcharge amount
                    IF, FLD_MSURC,
                        TXT, Surcharge,
                        RGHT, FLD_S, FLD_MSURC, 4, LINE,
                    ENDIF,
                    RGHT, MLTCHR, 13, '_', LINE,
                ENDIF,
                
// TOTAL amount
                IF, CND_ROLL, 
                    DOUBLE, 
                ENDIF,
                IF, FLG_TCONF, TCOPT2, TC2_PRTTOT,
                    TXT, Total,
                ENDIF,
                RGHT, FLD_F, FLD_TOTF, FA_TRIM,
            ENDIF,
        ENDIF,
        LINE,

//*****************************************************************************
// Amount due:
//*****************************************************************************
        IF, FLD_AMTDUE,
            LINE, DOUBLE,
            TXT, AmountDue,
            RGHT, FLD_F, FLD_AMTDUE, FA_TRIM, LINE,
        ENDIF,


//*****************************************************************************
// Descriptor 1
//*****************************************************************************
        IF, FLD_DCD1,
            FLD, FLD_DTX1, ' ', '0', FLD, FLD_DCD1,
            LINE,
        ENDIF,
        

//*****************************************************************************
// Descriptor 2
//*****************************************************************************
        IF, CND_ROLL,
            IF, FLD_DCD2,
                FLD, FLD_DTX2, ' ', '0', FLD, FLD_DCD2, LINE,
            ENDIF,
        ELSEIF, NOT, CND_LNGE, 20,
            IF, FLD_DCD2,
                FLD, FLD_DTX2, ' ', '0', FLD, FLD_DCD2,
            ENDIF,
            LINE,
        ENDIF,

//*****************************************************************************
// Descriptor 3 or "No refunds allowed" or "Offline entered"
//*****************************************************************************
        IF, NOT, CND_ROLL, AND, FLG_ISS, ISOPT3, IS3_NOREFPRT,
            TXT, NoRefundsAllow,
        ELSEIF, FLD_DCD3,
            FLD, FLD_DTX3, ' ', '0', FLD, FLD_DCD3,
        ELSEIF, TRN_OFFL,
            TXT, OfflineEntered,
        ENDIF,
#ifndef MAKE_M2
        LINE,
#endif

//*****************************************************************************
// Descriptor 4
//*****************************************************************************
        IF, CND_ROLL,
            IF, FLD_DCD4,
                FLD, FLD_DTX4, ' ', '0', FLD, FLD_DCD4, LINE,
            ENDIF,
        ELSEIF, NOT, CND_LNGE, 23,
            IF, FLD_DCD4,
                FLD, FLD_DTX4, ' ', '0', FLD, FLD_DCD4,
            ENDIF,
            LINE,
        ENDIF,

//*****************************************************************************
// "No refund allowed" text
//*****************************************************************************
        IF, CND_ROLL, AND, FLG_ISS, ISOPT3, IS3_NOREFPRT,
            TXT, NoRefundsAllow,
            LINE,
        ENDIF,

        // SKIP REST OF RECEIPT FOR OPEN TAB TRANSACTIONS
        IF, NOT, TRN_OPENT,         

//*****************************************************************************
// "Duplicate" or "Signature not required" or cardholder's name
//*****************************************************************************
            IF, CND_REPR,
                CNTR,
                TXT, Duplicate,
            ELSEIF, CND_SREQ,
                TXT, SignatureNotReq,
            ELSE,
                FLD, FLD_NAME,
            ENDIF,

//  ******************* LINE 24 & 25 *******************

			IF, FLG_TRSTATUS, 1, TS2_AVSVOID,
                LINE, LINE, DOUBLE,
                TXT, Void,
                NORMAL, ' ', '-', ' ',
                TXT, AvsCanceled,
            ELSEIF, FLG_TCONF, TCOPT4, TC4_TRAINING,
                LINE, LINE, CNTR,
                TXT, PclTrainingMode,
            ELSEIF, CND_REJ,
                LINE, LINE, MLTCHR, 50, 'X',
            ELSEIF, TRN_VOID,OR,TRN_NOSIG,  // SUPPRESS SIGNATURE & DISCLAIMER FOR VOIDS
				IF, CND_ROLL,
					IF, CND_SREQ,
						LINE,
					ELSE,
						LINE, LINE, LINE, 'X', MLTCHR, 50, '_', LINE,
					ENDIF,
					CNTR,   
					IF, TRN_DBCD,
						/*FONT, 0,*/ TXT, TransferFunds, LINE, CNTR,
						/*FONT, 0,*/ TXT, ForTAmount, 
					ELSE,
						EXTFLD, FX_DEFAULT,
						/*FONT, 0,*/ TXT, IAgree, LINE, CNTR,
						/*FONT, 0,*/ TXT, According, LINE, CNTR,
						/*FONT, 0,*/ TXT, MerchantAgreement,
					ENDIF,
				ENDIF,
            ENDIF,
            LINE,

        ENDIF, // not open tab

#ifdef MAKE_SIGCAPTURE
		// Print the Signature Line
		LINE,
		SIG,
		LINE,
#endif // MAKE_SIGCAPTURE

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//  ROLL PRINTER & REJECTED
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

    ELSE,
        LINE,
        TXT, TransactFailed,
        LINE,
        IF, FLD_HOSTTEXT1,
            FLD, FLD_HOSTTEXT1, LINE,
            FLD, FLD_HOSTTEXT2,
        ELSE,
            FLD, FLD_RSPTXT,
        ENDIF,
        LINE,
    ENDIF,

//*****************************************************************************
// CUSTOM RECEIPT FOOTER TEXT LINES
//*****************************************************************************
    IF, FLD_RCPTFTR1, 
        CNTR, FLD_F, FLD_RCPTFTR1, FC_TRIM, 
        LINE, 
    ENDIF,

    IF, FLD_RCPTFTR2, 
        CNTR, FLD_F, FLD_RCPTFTR2, FC_TRIM, 
        LINE, 
    ENDIF,

    IF, FLD_RCPTFTR3, 
        CNTR, FLD_F, FLD_RCPTFTR3, FC_TRIM, 
        LINE, 
    ENDIF,

    IF, FLD_RCPTFTR4, 
        CNTR, FLD_F, FLD_RCPTFTR4, FC_TRIM, 
        LINE, 
    ENDIF,

//*****************************************************************************
// ICE PAC footer image
//*****************************************************************************
    GRPH, 0x60, 0x00,

//*****************************************************************************
// Paper cut over
//*****************************************************************************
    FORM
};


//
//--------------------------------------------------------------------
//       PRINTER TEST
//--------------------------------------------------------------------
//
const UBYTE ALIGN(4) RP_TEST_DATA[] = {

    IF, OPT1,
        FIRMWARE, LINE, 
        GRPH, 0X50, 0x00,
        IF, CND_ROLL, REPORT, RP_HEAD_SERIAL, ENDIF,
    ENDIF,
    
    IF, OPT2,
        FLD, FLD_HOSTTEXT1,
        FLD, FLD_HOSTTEXT2,
        LINE,
    ELSE,
        MLTCHR, 50, '*', CNTR, ' ', 'N', 'O', 'R', 'M', 'A', 'L', ' ', 'F', 'O', 'N', 'T', ' ', LINE,
        LINE,
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 
        RGHT, 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', LINE,
        CNTR, 'U', 'V', 'W', 'X', 'Y', 'Z', LINE,
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 
        RGHT, 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', LINE,
        CNTR, 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', LINE,
        LINE,
        CNTR, '!', '"', '#', '$', '%', '$', 0x27, '(', ')', '*', '+', 0x2C, '-',
        '.', '/', ':', '/', '/', '<', '=', '>', '?', '@', LINE,
        CNTR, '[', '\\', ']', '^', '_', '`', '{', '|', '}', 0x7E, 0x7F, LINE,
        LINE,
        'N', 'O', 'R', 'M', 'A', 'L', DOUBLE, RGHT, 'D', 'O', 'U', 'B', 'L', 'E', LINE,
        LINE,
		CNTR, DOUBLE, REVERSE_ON, 'D', 'O', 'U', 'B', 'L', 'E', ' ', 'I', 'N', 'V', 'E', 'R', 'S', 'E', LINE,
		CNTR, UNDERLINE_ON, 'U', 'N', 'D', 'E', 'R', 'L', 'I', 'N', 'E', UNDERLINE_OFF,
		REVERSE_ON, UNDERLINE_ON, 'I', 'N', 'V', 'E', 'R' ,'S', 'E', ' ', 'U', 'N', 'D', 'E', 'R', 'L', 'I', 'N', 'E', LINE, LINE,
		MLTCHR, 50, '*', CNTR, ' ', 'B', 'A', 'R', 'C', 'O', 'D', 'E', ' ', 'E', 'X', 'A', 'M', 'P', 'L', 'E', ' ', LINE,
		BARCODE_ON, BARCODE_START, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', BARCODE_END, LINE,
    MLTCHR, 50, '*', CNTR, ' ', 'T', 'H', 'E', ' ', 'E', 'N', 'D', ' ', LINE,
    ENDIF,

    IF, OPT3,
        GRPH, 0x60, 0x00,
        FORM,
    ENDIF
};
//
//--------------------------------------------------------------------
//       AUDIT REPORT
//      OPT1 & FLD_CUSTOM2  Totals
//      OPT2            Cash Back
//      OPT3            All Acquirers
//--------------------------------------------------------------------
const UBYTE ALIGN(4) RP_AUDIT_DATA[] = {
    
    IF, CND_FIRST,              ///**** HEADING LINES *************
        REPORT, RP_HEAD_SERIAL,

        TXT, AuditReport,
        RGHT, FLD_F, FLD_SYSDT, FD_TEXT, 
        LINE,

        FLD, FLD_BATNO, RGHT, FLD_F, FLD_MID, FC_TRIM, 
        LINE,
    
        IF, OPT3,
            TXT, AllAcquirers,
        ELSE,
            FLD, FLD_AQNAME,
        ENDIF, 
        LINE,
        LINE,

        TXT, InvTrans,
        IF, FLG_TCONF, TCOPT1, TC1_CASHIER,
            IF, CND_ROLL,               // It can all fit on one line.
                POSLFT, 21 + 1,
            ELSE,
                POSLFT, 14 + 1,
            ENDIF,
            IF, FLG_TCONF, TCOPT4, TC4_SRV_PRM,
                TXT, Server,
            ELSE,
                TXT, Cashr,
            ENDIF,
        ENDIF,
        RGHT, TXT, Auth,
        LINE,
    
        TXT, CardNumber,
        RGHT,
        TXT, CardType,
        LINE,

        MLTSPC, 5,
        IF, FLG_TCONF, TCOPT1, TC1_REST,
            ' ',
            TXT, Tip,
        ENDIF,
        IF, OPT2,
            POSLFT, 13,
            TXT, Cashbk,
        ENDIF,
        RGHT,
        TXT, Total,
        LINE,
    ENDIF,// of CND_FIRST
    LINE,
//

    IF, NOT, CND_ROLL, AND, CND_LNGE, 24,
        FORM,                       ///*** PAGE OVERFLOW ****
    ENDIF,
//
    IF, OPT1,                   // **** TOTALS **********************
        IF, FLD_CUSTOM2,
            LINE,
            IF, FLG_TCONF, TCOPT1, TC1_REST,
                POSRGHT, 14,
                TXT, TipTotal,
                LINE,
                POSRGHT, 19, FLD, FLD_CUSTOM1, 
                LINE,
            ENDIF,

            IF, OPT2,
                POSRGHT, 14,
                TXT, CashTotal,
                LINE,
                POSRGHT, 19, FLD, FLD_CUSTOM5, 
                LINE,
            ENDIF,
    
            POSRGHT, 14,
            TXT, TransTotal,
            LINE,
            POSRGHT, 19, FLD, FLD_CUSTOM2,
        ENDIF, // FLD_CUSTOM2

        IF, CND_ROLL,               // This might be an MT301, we do not want to cut
            LINE, LINE, LINE, LINE, LINE, LINE, LINE,
        ELSE,
            FORM,
        ENDIF,
        
    ELSE,  // OPT1                     // ************* DETAIL LINES *************
        FLD, FLD_INVNO, ' ',
        IF, TRN_VOID,
            TXT, Void,
            IF, CND_ROLL,               // It can all fit on one line.
                ' ', FLD_F, FLD_TRNNAME, FC_TRIM,
            ENDIF,
        ELSE,
            FLD_F, FLD_TRNNAME, FC_TRIM,
        ENDIF,
    
        RGHT, FLD, FLD_AUTH,
        
        IF, FLD_SRVID,
            IF, CND_ROLL,               // It can all fit on one line.
                POSLFT, 21,
            ELSE,
                POSLFT, 14,
            ENDIF,
            ' ', FLD, FLD_SRVID,
        ENDIF,

        IF, FLD_SHIFT,
            POSLFT, 24,
            TXT, Shift,
            ' ', FLD_F, FLD_SHIFT, FN_LDZ,
        ENDIF,
        LINE,

        FLD, FLD_PAN,
        RGHT, 
        IF, CND_MAN, 
            '*', 
        ENDIF,
        FLD_F, FLD_CNAME, FC_TRIM, 
        LINE,

        IF, FLD_TIP,
            POSLFT, 1, FLD_S, FLD_TIP, 4,
        ENDIF,
        IF, FLD_CASH,
            POSLFT, 11, FLD_S, FLD_CASH, 6,
        ENDIF,
        RGHT, FLD, FLD_TOTAL, 
        LINE,

    ENDIF, // OPT1
};
//
//--------------------------------------------------------------------
//       OPEN TABS REPORT
//--------------------------------------------------------------------
const UBYTE ALIGN(4) RP_OPEN_TABS_DATA[] = {
    IF, CND_FIRST,              ///**** HEADING LINES *************
    REPORT, RP_HEAD_SERIAL,
    TXT, OpenTabs,
    RGHT, FLD_F, FLD_SYSDT, FD_TEXT, LINE,
    FLD, FLD_BATNO, RGHT, FLD_F, FLD_MID, FC_TRIM, LINE,
    LINE,
    TXT, PclInvTrans,
    IF, FLG_TCONF, TCOPT1, TC1_CASHIER,
    IF, FLG_TCONF, TCOPT4, TC4_SRV_PRM,
    TXT, Server,
    ELSE,
    TXT, Cashr,
    ENDIF,
    ENDIF,
    RGHT,
    TXT, Auth,
    LINE,
    TXT, CardNumber,
    RGHT,
    TXT, CardType,
    LINE,
    MLTSPC, 5,
    IF, FLG_TCONF, TCOPT1, TC1_REST,
    TXT, Tip,
    ENDIF,
    RGHT,
    TXT, Total,
    LINE,
    ENDIF,
    LINE,
    IF, NOT, CND_ROLL, AND, CND_LNGE, 24,
    FORM,                       ///*** PAGE OVERFLOW ****
    ENDIF,
    FLD, FLD_INVNO, ' ',
    IF, TRN_VOID,
    TXT, Void,
    ELSE,
    FLD_F, FLD_TRNNAME, FC_TRIM,
    ENDIF,
    RGHT, FLD, FLD_AUTH,
    IF, FLD_SRVID,
    POSLFT, 16, ' ', FLD, FLD_SRVID,
    ENDIF,
    IF, FLD_SHIFT,
    POSLFT, 19,
    TXT, Shift,
    ' ',
    FLD_F, FLD_SHIFT, FN_LDZ,
    ENDIF,
    LINE,
    FLD, FLD_PAN,
    RGHT, IF, CND_MAN, '*', ENDIF,
    FLD_F, FLD_CNAME, FC_TRIM, LINE,
    IF, FLD_TIP,
    POSLFT, 1, FLD_S, FLD_TIP, 4,
    ENDIF,
    RGHT, FLD, FLD_TOTAL, LINE
};
//
//--------------------------------------------------------------------
//       ISSUER REPORT
//--------------------------------------------------------------------
//
const UBYTE ALIGN(4) RP_ISSUER_DATA[] = {
    IF, CND_FIRST,
    REPORT, RP_HEAD_SERIAL,
    TXT, TransTotal,
    LINE,
    TXT, ByIssuer,
    RGHT, FLD_F, FLD_SYSDT, FD_TEXT,
    LINE, LINE,
    IF, OPT4,                   // CASHIER SUMMARY REPORT
    CNTR,
    IF, FLG_TCONF, TCOPT4, TC4_SRV_PRM,
    TXT, PclServer,
    ELSE,
    TXT, Cashier,
    ENDIF,
    FLD, FLD_SRVID, LINE,
    IF, OPT5,
    CNTR,
    TXT, PclTill,
    FLD, FLD_TILLNO,
    ENDIF,
    LINE,
    ENDIF,
    ENDIF,
    IF, NOT, CND_ROLL, AND, CND_LNGE, 22, FORM, ENDIF,
    IF, OPT1,
    LINE, LINE,
    TXT, GranTotals,
    ELSE,
    FLD, FLD_CNAME,
    ENDIF, LINE,
    REPORT, RP_TOTALS,          // SALES & REFUNDS TOTALS
    TXT, Total,
    ' ', ' ', FLD_F, FLD_CUSTOM1, FN_LDZ,
    RGHT, FLD_F, FLD_CUSTOM2, FA_TRIM, LINE,
    IF, OPT1, 
      IF, NOT, CND_ROLL,
        FORM,
      ELSE,
        LINE, LINE, LINE, LINE,
      ENDIF,
    ELSE, 
      LINE, 
    ENDIF
};
//
//--------------------------------------------------------------------
//       SETTLEMENT RECEIPT
//--------------------------------------------------------------------
//
const UBYTE ALIGN(4) RP_HEAD_SETTLE_DATA[] = {
    REPORT, RP_HEAD_MRCH, REPORT, RP_HEAD_MRCH2,    // MERCHANT INFO
    FLD_F, FLD_TRNDT, FD_TEXT,  // DATE
    IF, FLG_TCONF, TCOPT1, TC1_PR_TIME, // TIME
    ' ', FLD_FL, FLD_TRNTM, FT_MIL, 5,
    ENDIF, LINE,
    LINE, LINE
};

const UBYTE ALIGN(4) RP_SETTLE_DATA[] = {
    REPORT, RP_HEAD_SETTLE,     // MERCHANT, ACQUIRER, DATE INFO
    IF, FLD_HOSTTEXT1,
        FLD, FLD_HOSTTEXT1, LINE,
        FLD, FLD_HOSTTEXT2,
//    ELSE,
//        FLD, FLD_RSPTXT,
    ENDIF,
    LINE,
    DOUBLE,FLD,FLD_AQNAME,LINE, // Host Name
    REPORT, RP_TOTALS,          // HOST RESPONSE
    FORM
};
//
//--------------------------------------------------------------------
//       CASHIER REPORT
//      OPT1    Totals
//      OPT2    Cash Back
//      OPT3    Unadjusted
//      OPT4    Cashier Summary
//      OPT5    Till
//--------------------------------------------------------------------
//
const UBYTE ALIGN(4) RP_CASHIER_DATA[] = {
    IF, CND_FIRST,              ///**** HEADING LINES *************
    REPORT, RP_HEAD_SERIAL,
    IF, FLG_TCONF, TCOPT4, TC4_SRV_PRM,
    TXT, Server,
    ELSE,
    TXT, Cashr,
    ENDIF,
    IF, OPT3,
    TXT, Unadj,
    ELSE,
    TXT, Audit,
    ENDIF,
    RGHT, FLD_F, FLD_SYSDT, FD_TEXT,
    LINE,                       ///*************************LINE 1
    RGHT, FLD, FLD_MID, LINE,
    TXT, Terminal,
    RGHT, FLD, FLD_SERNO, LINE,
    TXT, AllAcquirers,
    LINE, LINE,
    CNTR,
    IF, FLG_TCONF, TCOPT4, TC4_SRV_PRM,
    TXT, Server,
    ELSE,
    TXT, Cashr,
    ENDIF,
    FLD, FLD_SRVID, LINE, LINE,
    TXT, InvTrans,
    IF, FLG_TCONF, TCOPT1, TC1_CASHIER,
    IF, FLG_TCONF, TCOPT4, TC4_SRV_PRM,
    TXT, Server,
    ELSE,
    TXT, Cashr,
    ENDIF,
    ENDIF,
    RGHT,
    TXT, Auth,
    LINE,
    TXT, CardNumber,
    RGHT,
    TXT, CardType,
    LINE,
    MLTSPC, 5,
    IF, FLG_TCONF, TCOPT1, TC1_REST,
    TXT, Tip,
    ENDIF,
    IF, OPT2,
    POSLFT, 13,
    TXT, Cashbk,
    ENDIF,
    RGHT,
    TXT, Total,
    LINE,
    LINE,
    ENDIF,
    IF, NOT, CND_ROLL, AND, CND_LNGE, 24, FORM, ENDIF,  ///*** PAGE OVERFLOW ****
//
    IF, OPT1,                   // **** TOTALS **********************
    IF, FLD_CUSTOM4,
    LINE,
    IF, FLG_TCONF, TCOPT1, TC1_REST,
    POSRGHT, 14,
    TXT, Tip,
    ' ',
    TXT, Total,
    ':', LINE,
    POSRGHT, 19, FLD, FLD_CUSTOM3, LINE,
    ENDIF,
    IF, OPT2,
    POSRGHT, 14,
    TXT, Cash,
    ' ',
    TXT, Total,
    ':', LINE,
    POSRGHT, 19, FLD, FLD_CUSTOM5, LINE,
    ENDIF,
    POSRGHT, 14,
    TXT, TransTotal, ':', LINE,
    POSRGHT, 19, FLD, FLD_CUSTOM4,
    ENDIF,
    IF, CND_LNGE, 2,            ////// IF THERE'S DATA PRINT IT
      IF, NOT, CND_ROLL,
        FORM,
      ELSE,
        LINE, LINE, LINE, LINE,
      ENDIF,
    ENDIF,
    ELSE,                       // ************* DETAIL LINES *************
    FLD, FLD_INVNO, ' ',
    IF, TRN_VOID,
    TXT, Void,
    ELSE,
    FLD_F, FLD_TRNNAME, FC_TRIM,
    ENDIF,
    RGHT, FLD, FLD_AUTH,
    IF, FLD_SRVID,
    POSLFT, 16, ' ', FLD, FLD_SRVID,
    ENDIF,
    IF, FLD_SHIFT,
    POSLFT, 19,
    TXT, Shift,
    ' ', FLD_F, FLD_SHIFT, FN_LDZ,
    ENDIF,
    LINE,
    FLD, FLD_PAN,
    RGHT, IF, CND_MAN, '*', ENDIF,
    FLD_F, FLD_CNAME, FC_TRIM, LINE,
    IF, FLD_TIP,
    POSLFT, 1, FLD_S, FLD_TIP, 4,
    ENDIF,
    IF, FLD_CASH,
    IF, FLD_TIP, LINE, ENDIF,
    POSLFT, 11, FLD_S, FLD_CASH, 6,
    ENDIF,
    RGHT, FLD, FLD_TOTAL, LINE,
    ENDIF
};
//
//--------------------------------------------------------------------
//       CASHIER LOGON REPORT
//--------------------------------------------------------------------
//
const UBYTE ALIGN(4) RP_LOGON_DATA[] = {
    IF, CND_FIRST,              ///**** HEADING LINES *************
    REPORT, RP_HEAD_SERIAL,
    IF, FLG_TCONF, TCOPT4, TC4_SRV_PRM,
    TXT, Server,
    ELSE,
    TXT, Cashr,
    ENDIF,
    ' ', ' ',
    TXT, Log,
    '-',
    TXT, On,
    ' ', ' ',
    TXT, Report,
    IF, NOT, CND_ROLL, LINE, ENDIF,
    RGHT, FLD_F, FLD_SYSDT, FD_TEXT, LINE,
    MLTCHR, 50, '-', LINE,
    LINE, LINE,
    ENDIF,
//
    IF, NOT, CND_ROLL, AND, CND_LNGE, 24, FORM, ENDIF,  ///*** PAGE OVERFLOW ****
//
    IF, FLG_TCONF, TCOPT4, TC4_SRV_PRM, // ** DETAIL LINES **
    TXT, Srv, ':',
    ELSE,
    TXT, Csh, ':',
    ENDIF,
    FLD, FLD_SRVID, ' ',
    IF, FLG_TCONF, TCOPT1, TC1_TILL,
    TXT, Till,
    ':', FLD, FLD_TILLNO, LINE,
    ELSE,
    ' ', ' ', ' ',
    IF, OPT1, LINE, ENDIF,
    ENDIF
};
//
//--------------------------------------------------------------------
//       DATABASE DUMP
//--------------------------------------------------------------------
//
const UBYTE ALIGN(4) RP_DATABASE_DATA[] = {
    IF, CND_FIRST,
    TXT, DatabaseReport,
    LINE,
    MLTCHR, 15, '-', LINE,
    LINE,
    ENDIF,
    IF, OPT1,
    TXT, TermEpromConfig,
    LINE,
    ENDIF,
    IF, OPT2,
    TXT, TermConfig,
    ENDIF,
    IF, OPT3,
    TXT, AcqId,
    ENDIF,
    IF, OPT4,
    TXT, IssuerID,
    ENDIF,
    IF, OPT5,
    TXT, CardRangeId,
    ENDIF,
    IF, OPT6,
    TXT, Id_Descriptor,
    ENDIF,
    IF, OPT7,
    TXT, Visa2Id,
    ENDIF,
    IF, OPT8,
    TXT, AddPromptId,
    ENDIF,
    IF, OPT3, OR, OPT4, OR, OPT5, OR, OPT6, OR, OPT7, OR, OPT8,
    FLD_L, FLD_CUSTOM1, 2, LINE,
    FLD_S, FLD_CUSTOM1, 3, LINE,
    ENDIF,
    IF, OPT1, OR, OPT2, OR, OPT3, OR, OPT4, OR, OPT5, OR, OPT7, OR, OPT8,
    TXT, RawData,
    LINE, LINE,
    ENDIF,
    FLD, FLD_CUSTOM2, LINE
};
//
//--------------------------------------------------------------------
//       DATASCOPE
//--------------------------------------------------------------------
//
const UBYTE ALIGN(4) RP_DATASCOPE_DATA[] = {
    IF, OPT1,
    TXT, ReceivedData,
    LINE, LINE,
    ENDIF,
    IF, OPT2,
    TXT, SendData,
    LINE, LINE,
    ENDIF,
    FLD, FLD_CUSTOM2, LINE
};

//
//--------------------------------------------------------------------
//       HEADERS
//--------------------------------------------------------------------
//
const UBYTE ALIGN(4) RP_HEAD_COMMON_DATA[] = {
// ********************** LINE 1 **
    CNTR,
    IF, FLG_TCONF, TCOPT4, TC4_TRAINING,
    TXT, PclTrainingMode,
    ELSE,
    FLD_F, FLD_RCPT3, FC_TRIM,
    ENDIF,
    LINE,
// ********************** LINE 2 **
    CNTR, FLD_F, FLD_RCPT1, FC_TRIM,
    LINE,
// ********************** LINE 3 **
    CNTR, FLD_F, FLD_RCPT2, FC_TRIM,
    LINE,
// ********************** LINE 4 **
    IF, CND_ROLL,
    IF, FLD_RCPTHDR4,
    CNTR, FLD_F, FLD_RCPTHDR4, FC_CENTER, LINE,
    ENDIF,
#ifndef MAKE_M2
    LINE,
#endif
    TXT, TerminalID,
    ENDIF
};

const UBYTE ALIGN(4) RP_HEAD_SERIAL_DATA[] = {
    REPORT, RP_HEAD_COMMON,
    IF, CND_ROLL, RGHT, ENDIF,
    FLD_F, FLD_SERNO, FC_TRIM, LINE,
    IF, CND_ROLL, LINE, ENDIF
};
//
const UBYTE ALIGN(4) RP_HEAD_MRCH_DATA[] = {
    REPORT, RP_HEAD_COMMON,
    RGHT, FLD_F, FLD_TID, FC_TRIM, LINE,
#ifndef MAKE_M2
    IF, CND_ROLL, LINE, ENDIF
#endif
};
//
const UBYTE ALIGN(4) RP_HEAD_MRCH2_DATA[] = {
    IF, CND_ROLL,
    TXT, MerchantN,
    RGHT,
    ELSE,
    RGHT,
    ENDIF,
    FLD_F, FLD_MID, FC_TRIM, LINE,  // MERCHANT NUMBER
    FLD, FLD_AQNAME, LINE,
    LINE
};
//
//--------------------------------------------------------------------
//       SALES & REFUNDS TOTALS
//--------------------------------------------------------------------
//
const UBYTE ALIGN(4) RP_TOTALS_DATA[] = {
    TXT, Sales,
    FLD_F, FLD_SALECNT, FN_LDZ,
    IF, FLD_BASEAMT,
    //IF, FLD_SALEAMT,
        LINE, ' ', ' ',
        TXT, Base,
        FLD, FLD_BASEAMT,
        //FLD, FLD_SALEAMT,
    ENDIF,
    IF, FLD_TAXAMT,
        LINE, ' ', ' ',
        TXT, Tax,
        FLD, FLD_TAXAMT,
    ENDIF,
    IF, FLD_BARAMT,
        LINE, ' ', ' ',
        TXT, Bar,
        FLD, FLD_BARAMT,
    ENDIF,
    IF, FLD_TIPAMT,
        LINE, ' ', ' ',
        TXT, Tip,
        ' ',
        FLD, FLD_TIPAMT,
        ' ', ' ', FLD_F, FLD_TIPPCT, FN_TRIM, '%',
    ENDIF,
    IF, FLD_CASHAMT,
        LINE, ' ', ' ',
        TXT, Cash,
        FLD, FLD_CASHAMT,
    ENDIF,
    IF, FLD_BASEAMT, OR, FLD_TAXAMT, OR, FLD_BARAMT, OR, FLD_TIPAMT, OR, FLD_CASHAMT,
        LINE, ' ', ' ',
        TXT, Total,
    ENDIF,
    RGHT, FLD_F, FLD_SALEAMT, FA_TRIM, LINE,
    TXT, Refunds,
    FLD_F, FLD_REFCNT, FN_LDZ,
    RGHT, FLD_F, FLD_REFAMT, FA_TNEG, LINE,
    IF, FLD_CASHCNT,
        TXT, CashBk,
        FLD_F, FLD_CASHCNT, FN_LDZ,
        RGHT, FLD_F, FLD_CASHAMT, FA_TRIM, LINE,
    ENDIF,
};


//
//--------------------------------------------------------------------
//       SETTLMENT SUMMARY REPORT
//--------------------------------------------------------------------
//
const UBYTE ALIGN(4) RP_SETTLE_SUM_DATA[] = {

    IF, OPT3,                   // Report is done, just do a form feed and exit
    FORM,
    ELSE,
    IF, OPT1,                   // Only Display the Header the first time
    CNTR,
    TXT, SettlStatus,
    LINE,
    CNTR, '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_',
    '_', '_', '_', LINE,
    LINE, LINE,
    ENDIF,
    IF, FLD_CSTMSG,
    FLD, FLD_CSTMSG, LINE,
    ENDIF,
    IF, FLD_CSTMSG2,
    FLD, FLD_CSTMSG2, LINE, LINE,
    ENDIF,
    IF, FLD_CSTMSG3,
    FLD, FLD_CSTMSG3, LINE,
    ENDIF,
    IF, FLD_CSTMSG4,
    FLD, FLD_CSTMSG4, LINE, LINE,
    ENDIF,
    IF, FLD_CSTMSG5,
    FLD, FLD_CSTMSG5, LINE,
    ENDIF,
    IF, FLD_CSTMSG6,
    FLD, FLD_CSTMSG6, LINE, LINE,
    ENDIF,
    IF, OPT2,
    LINE,
    CNTR, '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_',
    '_', '_', '_', LINE,
    LINE, LINE,
    ENDIF,
    ENDIF,
};

//
//--------------------------------------------------------------------
//       PERFORMANCE REPORT
//--------------------------------------------------------------------
//
const UBYTE ALIGN(4) RP_PERFORMANCE_DATA[] = {
    REPORT, RP_HEAD_SERIAL,     // TERMINAL INFO

    IF, CND_ROLL,
    TXT, Date,
    ENDIF,
    RGHT, FLD_F, FLD_SYSDT, FD_TEXT,LINE,   // SYSTEM DATE
    IF, CND_ROLL, 
    TXT, Time,
    ENDIF,
    RGHT,' ', FLD_FL, FLD_SYSTM, FT_MIL, 5, LINE, LINE,  // SYSTEM TIME
    IF, CND_ROLL, DOUBLE,ENDIF,
    CNTR, TXT, lblPerfRpt,
    LINE , LINE, TXT, Period,

    EXTFLD,FX_EXTENDED,
        FLD_F, FLD_PERIOD, FD_MDYS,
        TXT, To, 
    EXTFLD,FX_DEFAULT,
        FLD_F, FLD_SYSDT, FD_MDYS, LINE, LINE,
        TXT, lblTotAprorv,
    EXTFLD,FX_EXTENDED,
        FLD_F, FLD_TOTAPPROVAL, FN_TRIM, 
        RGHT, FLD_F, FLD_APPROV_PERC, FN_TRIM, '%',LINE,  // Approval Percent 
        TXT, lblTotDenials,
        FLD_F, FLD_TOTDENIALS, FN_TRIM,  
        RGHT, FLD_F, FLD_DENIAL_PERC, FN_TRIM, '%',LINE,  // Denials Percent
        TXT, lblTotManual,
        FLD_F, FLD_TOTMANUAL, FN_TRIM, 
        RGHT, FLD_F, FLD_MANUAL_PERC, FN_TRIM, '%',LINE,  // MANUAL Percent
        TXT, lblTotSwipe,
        FLD_F, FLD_TOTSWIPE, FN_TRIM, 
        RGHT, FLD_F, FLD_SWIPE_PERC, FN_TRIM, '%',LINE,LINE,  // SWIPE Percent

        CNTR,TXT, Comm, LINE, LINE,
        TXT, TransCount ,LINE,
        TXT, lblTransCount, 
        FLD_F, FLD_TOTCOUNT, FN_TRIM, LINE, LINE,
        TXT, lblTotRedials,
        FLD_F, FLD_TOTREDIALS, FN_TRIM, LINE, LINE,
        TXT, lblTottimeout,
        FLD_F, FLD_TOTTIMEOUT, FN_TRIM, LINE, LINE,
        TXT, lblTotRetrans,
        FLD_F, FLD_TOTRETRANS, FN_TRIM, LINE, LINE,
    EXTFLD,FX_DEFAULT,
    LINE,LINE, FORM
};



//--------------------------------------------------------------------
//       ISSUER CONFIGURATION REPORT
//--------------------------------------------------------------------

const UBYTE ALIGN(4) RP_ISSCONFIG_DATA[] = {
    IF,OPT1,
        IF, CND_FIRST,                                 ////*** HEADING LINES *************
            REPORT, RP_HEAD_SERIAL,                    // TERMINAL INFO
            IF, CND_ROLL,
            TXT, Date,
            ENDIF,
            RGHT, FLD_F, FLD_SYSDT, FD_TEXT,LINE,      // SYSTEM DATE
            IF, CND_ROLL, 
                TXT, Time,
            ENDIF,
            RGHT,' ', FLD_FL, FLD_SYSTM, FT_MIL, 5, LINE, LINE,  // SYSTEM TIME
            IF, CND_ROLL, DOUBLE,ENDIF,
                CNTR, TXT, lblConfigRpt,LINE,          // CONFIGURATION REPORT
            EXTTXT,TX_EXTENDED,
                IF, CND_ROLL, DOUBLE,ENDIF,
                CNTR, TXT, lblIssuerTab,LINE, LINE,    // ISSUER TABLE
                CNTR, TXT, lblIssAvail, LINE, LINE,    // ISSUER AVAILABLE
                TXT,lblName, RGHT, TXT, lblID, LINE, LINE, // NAME      ID 

        ENDIF,                                         // (IF CND_FIRST) 

///********* ISSUER AVAILABLE ********************
            FLD, FLD_CNAME,RGHT,
            EXTFLD, FX_EXTENDED,
                FLD, FLD_ISID, LINE, LINE,
            EXTFLD, FX_DEFAULT,

    ENDIF,                                             //  (( IF OPT1 ))
///********* CONFIGURATION TABLE BY ISSUER ********
    IF, OPT2,
            EXTTXT, TX_EXTENDED,
            LINE,LINE,
            MLTCHR, 50, '*',CNTR,' ', TXT,lblConfigByIss,' ', LINE, LINE, // CONFIGURATION BY ISSUER

            TXT,lblName, RGHT, TXT, lblID, LINE,       // NAME      ID 
            FLD, FLD_CNAME,RGHT,
            EXTFLD, FX_EXTENDED,
                FLD, FLD_ISID, LINE, LINE,
            EXTFLD, FX_DEFAULT,

            EXTTXT, TX_EXTENDED,
                TXT, CaptureTrans, RGHT,               // Capture Transaction
            EXTTXT, TX_DEFAULT,
                IF, FLG_ISS, ISOPT2, IS2_CAPTURE,
                TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, PrintReceipt, RGHT,               // Print Receipt
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT2, IS2_RECEIPT,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, CaptureReceipt,RGHT,              // Capture Receipt
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT6, IS6_ERC,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, FloorLimit, RGHT,                 // Floor Limit
            EXTFLD, FX_EXTENDED,
            FLD_F, FLD_FLOOR, FN_TRIM, LINE,  

            TXT, ReauthPercent, RGHT,                  // Reauthorization
                FLD_F, FLD_ISREAUTH, FN_TRIM, LINE,  
            EXTFLD, FX_DEFAULT,

            EXTTXT, TX_EXTENDED,                       // ACH Check Trans.
                TXT, AchCheckTrans, RGHT,
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT5, IS5_ACHCHECK,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, Use6Digit, RGHT,                  // Use 6 Digit
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT3, IS3_AUTH_FL6 ,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, PinEntryReq, RGHT,                // Pin Entry Required
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT1, IS1_PIN,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, AllowCashBack, RGHT,
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT3, IS3_CASHAMT,          // Allow Cash Back
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, AllowCashOnly, RGHT,              // Allow Cash Only
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT3, IS3_CASH_TRN,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE, LINE,

            EXTTXT, TX_EXTENDED,
                CNTR, TXT, lblProcOption, LINE, LINE,  // PROCCESSING OPTIONS

                TXT, InvoiceN, RGHT,                   // Invoice Number
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT2, IS2_ROC,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, ManualPan, RGHT,                  // Manual Pan Entry
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT1, IS1_MANPAN,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT,TX_EXTENDED,
                TXT, Mod10Check, RGHT,                 // Mod 10 Check on Pan
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT2, IS2_MOD10,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT,TX_EXTENDED,
                TXT, IgnoreTrack, RGHT,                // Ignore Track #1
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT4, IS4_TRK1ERR,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT,TX_EXTENDED,
                TXT, ExpiryDate, RGHT,                 // Expire date required
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT1, IS1_EXPD,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT,TX_EXTENDED,
                TXT, CheckExpiry, RGHT,                // Check Expire Date
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT2, IS2_CH_EXPD,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT,TX_EXTENDED,
                TXT, OfflineAllow, RGHT,               // Offline Allowed
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT1, IS1_OFFL,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT,TX_EXTENDED,                        // Adjust Allowed
                TXT, AdjustAllow, RGHT,
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT1, IS1_ADJ,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT,TX_EXTENDED,
                TXT, VoiceReferral, RGHT,              // Voice Referral
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT1, IS1_REFER,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT,TX_EXTENDED,
                TXT, PromptForTax, RGHT,               // Prompt For Tax
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT4, IS4_TAX,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                CNTR, TXT, lblAvsOptions, LINE, LINE,  // AVS OPTIONS

            TXT, AvsEnab, RGHT,                       // Avs enabled on Sale
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT6, IS6_ALLOWAVS,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, PromptAddress, RGHT,             // Prompt Address
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT6, IS6_MAILADDR_REQ,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT,TX_EXTENDED,
                TXT, PromptZipCode, RGHT,             // Prompt Zip Code
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT6, IS6_AVSZIPCD_REQ,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE, LINE,

            EXTTXT, TX_EXTENDED,
                CNTR, TXT,lblBlockTrans, LINE, LINE,  // Blocked Transaction
            EXTTXT, TX_DEFAULT,

            TXT, Void_s, RGHT,                        // Void
            IF, FLG_ISS, ISOPT3, IS3_NOVOID,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            TXT, Refunds, RGHT,                       // Refunds    
            IF, FLG_ISS, ISOPT2, IS2_NOREF,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, CardVerif, RGHT,                 // Card Verify
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT2, IS2_NOCARDVER,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE, LINE,


            EXTTXT, TX_EXTENDED,
                CNTR, TXT, lblPinRequired, LINE, LINE, // PIN REQUIRED
            EXTTXT, TX_DEFAULT,

                TXT, Debit, RGHT,                       // Debit
            IF, FLG_ISS, ISPINTRAN, ISPIN_SALE,
            TXT, Off, ELSE, TXT, On, ENDIF, LINE,

                TXT, Refunds, RGHT,                     // Refund
            IF, FLG_ISS, ISPINTRAN, ISPIN_RETURN,
            TXT, Off, ELSE, TXT, On, ENDIF, LINE,

                TXT, Void_s, RGHT,                      // Void
            IF, FLG_ISS, ISPINTRAN, ISPIN_VOID,
            TXT, Off, ELSE, TXT, On, ENDIF, LINE,

                TXT, Adjust, RGHT,                      // Adjust   
            IF, FLG_ISS, ISPINTRAN, ISPIN_ADJUST,
            TXT, Off, ELSE, TXT, On, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, BalInq, RGHT,                     // Balinq
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISPINTRAN, ISPIN_BALINQ,
            TXT, Off, ELSE, TXT, On, ENDIF, LINE,

                TXT, CashBack, RGHT,                   // Cash Back
            IF, FLG_ISS, ISPINTRAN, ISPIN_CASHBK,
            EXTFLD, FX_DEFAULT,
            TXT, Off, ELSE, TXT, On, ENDIF, LINE,

   ENDIF,                                             // ( IF OPT2 )


};


//--------------------------------------------------------------------
//       ACQUIRER CONFIGURATION REPORT
//--------------------------------------------------------------------
/*
const UBYTE ALIGN(4) RP_ACQUIRER_DATA[] = {
        //REPORT, RP_HEAD_SERIAL,                        // TERMINAL INFO
        TXT, Date,
    IF,OPT1,
      IF, CND_FIRST,                                   ////*** HEADING LINES *************
        REPORT, RP_HEAD_SERIAL,                        // TERMINAL INFO
        IF, CND_ROLL,
        TXT, Date,
        ENDIF,
        RGHT, FLD_F, FLD_SYSDT, FD_TEXT,LINE,          // SYSTEM DATE
        IF, CND_ROLL, 
            TXT, Time,
        ENDIF,
        RGHT,' ', FLD_FL, FLD_SYSTM, FT_MIL, 5, LINE, LINE,  // SYSTEM TIME
        IF, CND_ROLL, DOUBLE,ENDIF,
        CNTR, TXT, lblConfigRpt, LINE,                 // CONFIGURATION REPORT
            IF, CND_ROLL, DOUBLE,ENDIF,
            CNTR, TXT, AcqTable,LINE, LINE,            // ACQUIRER TABLE
        EXTTXT, TX_EXTENDED,
            CNTR, TXT, lblConfbyAcq, LINE, LINE,           // ISSUER AVAILABLE
            TXT,lblName, RGHT, TXT, lblID, LINE, LINE, // NAME      ID 

     ENDIF,                                            // (IF CND_FIRST) 

///********* ACQUIRER AVAILABLE ********************
//            FLD, FLD_AQNAME,RGHT,
//            EXTFLD, FX_EXTENDED,
//                FLD, FLD_ACQUID, LINE, LINE,
//            EXTFLD, FX_DEFAULT,

//    ENDIF,                                             //  (( IF OPT1 ))

///********* CONFIGURATION TABLE BY ACQUIRER ********

};
*/

//--------------------------------------------------------------------
//       ACQUIRER CONFIGURATION REPORT
//--------------------------------------------------------------------

const UBYTE ALIGN(4) RP_ACQUIRER_DATA[] = {
    IF,OPT1,
      IF, CND_FIRST,                                   ////*** HEADING LINES *************
        REPORT, RP_HEAD_SERIAL,                        // TERMINAL INFO
        IF, CND_ROLL,
        TXT, Date,
        ENDIF,
        RGHT, FLD_F, FLD_SYSDT, FD_TEXT,LINE,          // SYSTEM DATE
        IF, CND_ROLL, 
            TXT, Time,
        ENDIF,
        RGHT,' ', FLD_FL, FLD_SYSTM, FT_MIL, 5, LINE, LINE,  // SYSTEM TIME
        IF, CND_ROLL, DOUBLE,ENDIF,
        CNTR, TXT, lblConfigRpt, LINE,                 // CONFIGURATION REPORT
            IF, CND_ROLL, DOUBLE,ENDIF,
            CNTR, TXT, AcqTable,LINE, LINE,            // ACQUIRER TABLE
        EXTTXT, TX_EXTENDED,
            CNTR, TXT, lblConfbyAcq, LINE, LINE,           // ISSUER AVAILABLE
            TXT,lblName, RGHT, TXT, lblID, LINE, LINE, // NAME      ID 

     ENDIF,                                            // (IF CND_FIRST) 

///********* ACQUIRER AVAILABLE ********************
            FLD, FLD_AQNAME,RGHT,
            EXTFLD, FX_EXTENDED,
                FLD, FLD_ACQUID, LINE, LINE,
            EXTFLD, FX_DEFAULT,

    ENDIF,                                             //  (( IF OPT1 ))

///********* CONFIGURATION TABLE BY ACQUIRER ********

    IF, OPT2, 
            EXTTXT, TX_EXTENDED,
            LINE,LINE,
            MLTCHR, 50, '*',CNTR,' ', TXT, lblConfbyAcq,' ', LINE, LINE, // CONFIGURATION BY ISSUER

            TXT,lblName, RGHT, TXT, lblID, LINE,       // NAME      ID 
            FLD, FLD_AQNAME,RGHT,
            EXTFLD, FX_EXTENDED,
                FLD, FLD_ACQUID, LINE, LINE,
            EXTFLD, FX_DEFAULT,
   
            TXT, NII, RGHT,                            // NII
            EXTFLD, FX_EXTENDED,
                FLD, FLD_NII, LINE,
            EXTFLD, FX_DEFAULT,

            TXT, CardAcceptTerm, RGHT,                 // Card acceptor terminal
            FLD, FLD_TID, LINE,

            TXT, CardAcceptMerc, RGHT,                // Card acceptor Merchant
            FLD_F, FLD_MID, FC_TRIM, LINE,

            TXT, VisaITermId, RGHT,                   // Visa 1 terminal ID
            FLD_F, FLD_VISATID, FC_TRIM, LINE,

            EXTTXT, TX_EXTENDED,                      
                TXT, NotASettlement, RGHT,            // Not a settlement
            EXTTXT, TX_DEFAULT,
            IF, FLG_ACQ, AQOPT3, AQ3_HSEXCL,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,                      
                TXT, IgnoreHostTime, RGHT,            // Ignore Host Time
            EXTTXT, TX_DEFAULT,
            IF, FLG_ACQ, AQOPT1, AQ1_NOTIME,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,                      
                TXT, ISDNprotocol, RGHT,              // ISDN protocol
            EXTTXT, TX_DEFAULT,
            FLD, FLD_DMODE, LINE,

            EXTTXT, TX_EXTENDED,                      
                TXT, CurrentBatch, RGHT,              // Current Batch
            EXTFLD, FX_EXTENDED,
            FLD, FLD_CURRBATCH, LINE,


            TXT, NextBatch, RGHT,
            FLD, FLD_NEXTBATCH, LINE,

            TXT, MaxSettleAttemp, RGHT,               // Max Settle attemp
            FLD, FLD_MAX_SETTLE, LINE, LINE,

            //> Sign On Host Information
            CNTR, TXT, lblSignOnHostInform, LINE, LINE,
            TXT, Primary, LINE,                       // PRIMARY
        
            TXT, PhoneNum, RGHT,                      // Phone Number
            EXTFLD, FX_DEFAULT,
                FLD_F, FLD_PTTEL, FC_CLRFF, LINE,
        
            TXT, ConnectTime, RGHT,                   // Connect time
            FLD, FLD_PTCON, LINE,
        
            TXT, DialAttemps, RGHT,                   // Dial Attemps
            FLD, FLD_PTTRY, LINE,
        
            TXT, ModemMode, RGHT,                     // Modem Mode
            FLD, FLD_MODEM, LINE, LINE,


            //< Sign On Host Information




            CNTR, TXT, lblTransHostInform,LINE, LINE,

            TXT, Primary, LINE,                       // PRIMARY

            TXT, PhoneNum, RGHT,                      // Phone Number
            EXTFLD, FX_DEFAULT,
                FLD_F, FLD_PTTEL, FC_CLRFF, LINE,

            TXT, ConnectTime, RGHT,                   // Connect time
            FLD, FLD_PTCON, LINE,

            TXT, DialAttemps, RGHT,                   // Dial Attemps
            FLD, FLD_PTTRY, LINE,

            TXT, ModemMode, RGHT,                     // Modem Mode
            FLD, FLD_MODEM, LINE, LINE,

            TXT, Secondary, LINE,                     // SECONDARY

            TXT, PhoneNum, RGHT,                      // Phone Number
            FLD_F, FLD_STTEL, FC_CLRFF, LINE,

            TXT, ConnectTime, RGHT,                   // Connect Time       
            FLD, FLD_STCON, LINE,

            TXT, DialAttemps, RGHT,                   // Dial Attemps
            FLD, FLD_STTRY, LINE, LINE,

          
            CNTR, TXT, lblSettleHostInf, LINE, LINE,  // SETTLEMENT HOST INFORMATION

            TXT, Primary, LINE,                       // PRIMARY

            TXT, PhoneNum, RGHT,                      // Phone Number
            FLD_F, FLD_PSTEL, FC_CLRFF,  LINE,

            TXT, ConnectTime, RGHT,                   // Connect time
            FLD, FLD_PSCON, LINE,

            TXT, DialAttemps, RGHT,                   // Dial Attemps
            FLD, FLD_PSTRY, LINE,

            TXT, ModemMode, RGHT,                     // Modem Mode
            FLD, FLD_SMODEM, LINE,

            TXT, Secondary, LINE,                     // SECONDARY

            TXT, PhoneNum, RGHT,                      // Phone Number
            FLD_F, FLD_SSTEL, FC_CLRFF, LINE,

            TXT, ConnectTime, RGHT,                   // Connect Time       
            FLD, FLD_SSCON, LINE,

            TXT, DialAttemps, RGHT,                   // Dial Attemps
            FLD, FLD_SSTRY, LINE,
            EXTFLD, FX_DEFAULT,                       // ALWAYS SET DEFAULT AT THE END

   ENDIF,                                             // ( IF OPT2 )
    
};


//--------------------------------------------------------------------
//       CARD CONFIGURATION REPORT
//--------------------------------------------------------------------

const UBYTE ALIGN(4) RP_CARD_DATA[] = {
    IF,OPT1,
      IF, CND_FIRST,                                   ////*** HEADING LINES *************
        REPORT, RP_HEAD_SERIAL,                        // TERMINAL INFO
        IF, CND_ROLL,
        TXT, Date,
        ENDIF,
        RGHT, FLD_F, FLD_SYSDT, FD_TEXT,LINE,          // SYSTEM DATE
        IF, CND_ROLL, 
            TXT, Time,
        ENDIF,
        RGHT,' ', FLD_FL, FLD_SYSTM, FT_MIL, 5, LINE, LINE,  // SYSTEM TIME
        IF, CND_ROLL, DOUBLE,ENDIF,
        CNTR, TXT, lblConfigRpt, LINE,                 // CONFIGURATION REPORT
        EXTTXT,TX_EXTENDED,
            IF, CND_ROLL, DOUBLE,ENDIF,
            CNTR, TXT, lblCardRangTab,LINE, LINE,      // CARD RANGE TABLE
            CNTR, TXT, lblCardRangAvai, LINE, LINE,        // ISSUER AVAILABLE
            TXT, lblCrnum, RGHT, TXT, SupportIn, LINE,


     ENDIF,                                            // (IF CND_FIRST) 

///********* CARD AVAILABLE ********************
           EXTFLD, FX_EXTENDED,
            FLD, FLD_CRNUM, 

           EXTTXT, TX_DEFAULT,
            RGHT, TXT, IssuerID,  
           EXTFLD, FX_EXTENDED, 
            FLD, FLD_CRISID, LINE,
            RGHT, TXT, AcqId, FLD, FLD_CRAQID, LINE, LINE,


    ENDIF,                                             //  (( IF OPT1 ))

///********* CONFIGURATION TABLE BY CARD ********
   IF, OPT2,
        EXTTXT, TX_EXTENDED,
         LINE,LINE,
            MLTCHR, 50, '*',CNTR,' ', TXT,  lblConfByCard ,' ', LINE, LINE, // CONFIGURATION BY ISSUER

                                               
            TXT, CardRang, RGHT, TXT, lblID,
        EXTFLD, FX_EXTENDED,
            FLD, FLD_CRNUM, LINE, LINE,
                                                         // Pan Range Low
            TXT, PanRangeLow, RGHT,
            FLD, FLD_CRPANL, LINE,

            TXT, PanRangeHigh, RGHT,                     // Pan Range Hight
            FLD, FLD_CRPANH, LINE,

            TXT,PanLength, RGHT,                         // Pan Lenght
            FLD, FLD_CRPLEN, LINE,
    
  ENDIF,                                                // (IF, OPT2)0

};


const struct report_table_struct ReportTable[] = {

    {RP_RECEIPT, ( UBYTE * ) RP_RECEIPT_DATA,
     ( UBYTE * ) ( RP_RECEIPT_DATA + sizeof( RP_RECEIPT_DATA ) )}
    ,

    {RP_TEST, ( UBYTE * ) RP_TEST_DATA,
     ( UBYTE * ) ( RP_TEST_DATA + sizeof( RP_TEST_DATA ) )}
    ,
    {RP_AUDIT, ( UBYTE * ) RP_AUDIT_DATA,
     ( UBYTE * ) ( RP_AUDIT_DATA + sizeof( RP_AUDIT_DATA ) )}
    ,

    {RP_ISSUER, ( UBYTE * ) RP_ISSUER_DATA,
     ( UBYTE * ) ( RP_ISSUER_DATA + sizeof( RP_ISSUER_DATA ) )}
    ,
    {RP_SETTLE, ( UBYTE * ) RP_SETTLE_DATA,
     ( UBYTE * ) ( RP_SETTLE_DATA + sizeof( RP_SETTLE_DATA ) )}
    ,

    {RP_CASHIER, ( UBYTE * ) RP_CASHIER_DATA,
     ( UBYTE * ) ( RP_CASHIER_DATA + sizeof( RP_CASHIER_DATA ) )}
    ,

    {RP_LOGON, ( UBYTE * ) RP_LOGON_DATA,
     ( UBYTE * ) ( RP_LOGON_DATA + sizeof( RP_LOGON_DATA ) )}
    ,
    {RP_DATABASE, ( UBYTE * ) RP_DATABASE_DATA,
     ( UBYTE * ) ( RP_DATABASE_DATA + sizeof( RP_DATABASE_DATA ) )}
    ,

    {RP_DATASCOPE, ( UBYTE * ) RP_DATASCOPE_DATA,
     ( UBYTE * ) ( RP_DATASCOPE_DATA + sizeof( RP_DATASCOPE_DATA ) )}
    ,

    {RP_OPEN_TABS, ( UBYTE * ) RP_OPEN_TABS_DATA,
     ( UBYTE * ) ( RP_OPEN_TABS_DATA + sizeof( RP_OPEN_TABS_DATA ) )}
    ,

    {RP_HEAD_MRCH, ( UBYTE * ) RP_HEAD_MRCH_DATA,
     ( UBYTE * ) ( RP_HEAD_MRCH_DATA + sizeof( RP_HEAD_MRCH_DATA ) )}
    ,

    {RP_TOTALS, ( UBYTE * ) RP_TOTALS_DATA,
     ( UBYTE * ) ( RP_TOTALS_DATA + sizeof( RP_TOTALS_DATA ) )}
    ,
    {RP_HEAD_MRCH2, ( UBYTE * ) RP_HEAD_MRCH2_DATA,
     ( UBYTE * ) ( RP_HEAD_MRCH2_DATA + sizeof( RP_HEAD_MRCH2_DATA ) )}
    ,

    {RP_HEAD_SETTLE, ( UBYTE * ) RP_HEAD_SETTLE_DATA,
     ( UBYTE * ) ( RP_HEAD_SETTLE_DATA + sizeof( RP_HEAD_SETTLE_DATA ) )}
    ,

    {RP_HEAD_COMMON, ( UBYTE * ) RP_HEAD_COMMON_DATA,
     ( UBYTE * ) ( RP_HEAD_COMMON_DATA + sizeof( RP_HEAD_COMMON_DATA ) )}
    ,

    {RP_HEAD_SERIAL, ( UBYTE * ) RP_HEAD_SERIAL_DATA,
     ( UBYTE * ) ( RP_HEAD_SERIAL_DATA + sizeof( RP_HEAD_SERIAL_DATA ) )}
    ,

    {RP_SETTLE_SUM, ( UBYTE * ) RP_SETTLE_SUM_DATA,
     ( UBYTE * ) ( RP_SETTLE_SUM_DATA + sizeof( RP_SETTLE_SUM_DATA ) )}
    ,

    {RP_PERFORMANCE, (UBYTE *) RP_PERFORMANCE_DATA,
     (UBYTE *) (RP_PERFORMANCE_DATA + sizeof(RP_PERFORMANCE_DATA))}
    ,

    {RP_ISSUER_CONFIG, (UBYTE *) RP_ISSCONFIG_DATA,
     (UBYTE *) (RP_ISSCONFIG_DATA + sizeof(RP_ISSCONFIG_DATA))}
    ,

    {RP_ACQUIRER_CONFIG, (UBYTE *) RP_ACQUIRER_DATA,
     (UBYTE *) (RP_ACQUIRER_DATA + sizeof(RP_ACQUIRER_DATA))}
    ,


    {RP_CARD_CONFIG, (UBYTE *) RP_CARD_DATA,
     (UBYTE *) (RP_CARD_DATA + sizeof(RP_CARD_DATA))}
    ,

    {RP_MARKER, 0, 0}
};



//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//! \brief
//!     Print Def Initialization
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void PrtDefInit( void )
{
    // register report table address from this page
    SDK_RegisterAddress( "REPORT_TAB", SDK_ThisPage(  ),
                         ( UBYTE * ) & ReportTable[0] );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Finds the Field Definition in FLDTAB or FLDTAB2
//!
//! Finds the Field Definition in FLDTAB or FLDTAB2. The fields must be below
//! 160 or over 230.
//!
//! \return
//!     None
//!
//! \note
//!     FieldNum ss global input: the field to find
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

//-----------------------------------------------------------------------------
//! \brief
//!     Finds type of Table
//!
//! \param
//!		[in]tableType			Type of table
//!
//! \return
//!     True if table has been found else False
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

                tableSet = 1;
                                     
                break;
             // To select typedef enum "Extended" field table as current table 
             case FX_EXTENDED:

                currElement.currentTab.Type = fieldTable[i].Type;
                currElement.currentTab.ptable1 = fieldTable[i].ptable1;
                currElement.currentTab.ptable2 = fieldTable[i].ptable2;

                tableSet = 1;

                break;
             default: 
            
             // Error messages -"No field table to print". 
             ShowErrMsg( PclTabError );

             // Error, the table is not registered 
             tableSet = 0;

          }
      }
       
   }

  return tableSet;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Finds type of Table
//!
//! \param
//!		[in]tabTxtType			Type of text table
//!
//! \return
//!     True if table has been found else False
//-----------------------------------------------------------------------------
Bool FindTxtTableType( ENTXTTYPE tabTxtType )
{
	Bool bRetVal = True;

     switch (tabTxtType)
     {

        case TX_DEFAULT:

                TxtTableId = pcltext_tableDef_id;

                break;

        case TX_EXTENDED:

                TxtTableId = pcltext_tableExt_id;

                break;

            default:            // Unknown, notify user
				bRetVal = False;
                ShowErrMsg( InvalidEntry );
                break;

      }
	return bRetVal;
}

//=============================================================================
// Private function definitions
//=============================================================================

