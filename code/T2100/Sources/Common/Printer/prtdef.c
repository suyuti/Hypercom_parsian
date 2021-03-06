
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
#include "prtdef_defines.h"
#include "rp_common_data.h"
#include "rp_audit_data.h"
#include "rp_settle_data.h"
#include "rp_summary_byiss_data.h"
#include "rp_sale_data.h"
#include "rp_refund_data.h"
#include "rp_void_data.h"
#include "rp_test_data.h"
#include "rp_other_data.h"
#include "rp_init_data.h"
#ifdef MAKE_KIB
#include "rp_loyaltyinq_data.h"
#include "rp_loyalty_data.h"
#endif
//--------------------------------------------------------------------
//       RECEIPT
//--------------------------------------------------------------------
const UBYTE RP_RECEIPT_DATA[] = {   // Receipt printing
	REPORT, RP_TRANS_SALE, LINE,
/*
	IF, TRN_SALE,
		REPORT, RP_TRANS_SALE, LINE,
	ELSE,
		EXTTXT, TX_SERVUS,
		EXTFLD, FX_SERVUS,

		CNTR,
		REPORT, RP_NAME_TRANS,
		LINE,

		CNTR,
		TXT, NotSupport,
		LINE,

		EXTTXT, TX_DEFAULT,
		EXTFLD, FX_DEFAULT,
	ENDIF,
*/
    FORM
};

const struct report_table_struct ReportTable[] = {

        {
		RP_RECEIPT, 
		( UBYTE * ) RP_RECEIPT_DATA,
		( UBYTE * ) ( RP_RECEIPT_DATA + sizeof( RP_RECEIPT_DATA ) )
	},
	
	// INIALIZE REPORT 
	{
		RP_INIT,
		(UBYTE *) RP_INIT_DATA,
		(UBYTE *) (RP_INIT_DATA + sizeof(RP_INIT_DATA))
	},

	// TRANSACTION REPORTS
	{
		RP_TRANS_SALE,
		(UBYTE *) RP_TRANS_SALE_DATA,
		(UBYTE *) (RP_TRANS_SALE_DATA + sizeof(RP_TRANS_SALE_DATA))
	},

	{
		RP_TRANS_REFUND,
		(UBYTE *) RP_TRANS_REFUND_DATA,
		(UBYTE *) (RP_TRANS_REFUND_DATA + sizeof(RP_TRANS_REFUND_DATA))
	},

	{
		RP_TRANS_VOID,
		(UBYTE *) RP_TRANS_VOID_DATA,
		(UBYTE *) (RP_TRANS_VOID_DATA + sizeof(RP_TRANS_VOID_DATA))
	},
        {
		RP_TRANS_BALINQ,
		(UBYTE *) RP_TRANS_BALINQ_DATA,
		(UBYTE *) (RP_TRANS_BALINQ_DATA + sizeof(RP_TRANS_BALINQ_DATA))
	},



	{
		RP_HEAD_MERCHANT,
		(UBYTE *) RP_HEAD_MERCHANT_DATA,
		(UBYTE *) (RP_HEAD_MERCHANT_DATA + sizeof(RP_HEAD_MERCHANT_DATA))
	},

	// SETTLEMENT REPORTS
	
    {
		RP_HEAD_SETTLE, 
		( UBYTE * ) RP_HEAD_SETTLE_DATA,
		( UBYTE * ) ( RP_HEAD_SETTLE_DATA + sizeof( RP_HEAD_SETTLE_DATA ) )
	},

	{
		RP_SETTLE, 
		( UBYTE * ) RP_SETTLE_DATA,
		( UBYTE * ) ( RP_SETTLE_DATA + sizeof( RP_SETTLE_DATA ) )
	},

	// AUDIT REPORT

	{
		RP_HEAD_AUDIT, 
		( UBYTE * ) RP_HEAD_AUDIT_DATA,
		( UBYTE * ) ( RP_HEAD_AUDIT_DATA + sizeof( RP_HEAD_AUDIT_DATA ) )
	},
	
    {
		RP_AUDIT, 
		( UBYTE * ) RP_AUDIT_DATA,
		( UBYTE * ) ( RP_AUDIT_DATA + sizeof( RP_AUDIT_DATA ) )
	},

	// SUMMARY REPORT BY ISSUER

	{
		RP_HEAD_SUMMARY_BYISS, 
		( UBYTE * ) RP_HEAD_SUMMARY_BYISS_DATA,
		( UBYTE * ) ( RP_HEAD_SUMMARY_BYISS_DATA + sizeof( RP_HEAD_SUMMARY_BYISS_DATA ) )
	},

	{
		RP_SUMMARY_BYISS, 
		( UBYTE * ) RP_SUMMARY_BYISS_DATA,
		( UBYTE * ) ( RP_SUMMARY_BYISS_DATA + sizeof( RP_SUMMARY_BYISS_DATA ) )
	},

	// FIELD REPORTS
	{
		RP_ID_MERCHANT,
		(UBYTE *) RP_ID_MERCHANT_DATA,
		(UBYTE *) (RP_ID_MERCHANT_DATA + sizeof(RP_ID_MERCHANT_DATA))
	},

	{
		RP_ID_TERMINAL,
		(UBYTE *) RP_ID_TERMINAL_DATA,
		(UBYTE *) (RP_ID_TERMINAL_DATA + sizeof(RP_ID_TERMINAL_DATA))
	},

	{
		RP_DATE_TIME_TRANS,
		(UBYTE *) RP_DATE_TIME_TRANS_DATA,
		(UBYTE *) (RP_DATE_TIME_TRANS_DATA + sizeof(RP_DATE_TIME_TRANS_DATA))
	},

	{
		RP_NO_STAN,
		(UBYTE *) RP_NO_STAN_DATA,
		(UBYTE *) (RP_NO_STAN_DATA + sizeof(RP_NO_STAN_DATA))
	},

	{
		RP_NO_BATCH,
		(UBYTE *) RP_NO_BATCH_DATA,
		(UBYTE *) (RP_NO_BATCH_DATA + sizeof(RP_NO_BATCH_DATA))
	},

	{
		RP_NO_INVOICE,
		(UBYTE *) RP_NO_INVOICE_DATA,
		(UBYTE *) (RP_NO_INVOICE_DATA + sizeof(RP_NO_INVOICE_DATA))
	},

	{
		RP_NAME_VERSION,
		(UBYTE *) RP_NAME_VERSION_DATA,
		(UBYTE *) (RP_NAME_VERSION_DATA + sizeof(RP_NAME_VERSION_DATA))
	},

	{
		RP_NAME_SOURCE,
		(UBYTE *) RP_NAME_SOURCE_DATA,
		(UBYTE *) (RP_NAME_SOURCE_DATA + sizeof(RP_NAME_SOURCE_DATA))
	},

	{
		RP_TYPE_CARD,
		(UBYTE *) RP_TYPE_CARD_DATA,
		(UBYTE *) (RP_TYPE_CARD_DATA + sizeof(RP_TYPE_CARD_DATA))
	},

	{
		RP_NO_PAN,
		(UBYTE *) RP_NO_PAN_DATA,
		(UBYTE *) (RP_NO_PAN_DATA + sizeof(RP_NO_PAN_DATA))
	},

	{
		RP_NAME_CARDHOLDER,
		(UBYTE *) RP_NAME_CARDHOLDER_DATA,
		(UBYTE *) ( RP_NAME_CARDHOLDER_DATA + sizeof(RP_NAME_CARDHOLDER_DATA) )
	},

	{
		RP_NAME_TRANS,
		(UBYTE *) RP_NAME_TRANS_DATA,
		(UBYTE *) ( RP_NAME_TRANS_DATA + sizeof( RP_NAME_TRANS_DATA ) )
 	},
        
        {
		RP_CNAME_TRANS,
		(UBYTE *) RP_CNAME_TRANS_DATA,
		(UBYTE *) ( RP_CNAME_TRANS_DATA + sizeof( RP_CNAME_TRANS_DATA ) )
 	},

	{
		RP_HEADER_COMMON,
		(UBYTE *) RP_HEADER_COMMON_DATA,
		(UBYTE *) (RP_HEADER_COMMON_DATA + sizeof(RP_HEADER_COMMON_DATA))
	},

	{
		RP_AMOUNT_TOTAL,
		(UBYTE *) RP_AMOUNT_TOTAL_DATA,
		(UBYTE *) (RP_AMOUNT_TOTAL_DATA + sizeof(RP_AMOUNT_TOTAL_DATA))
	},

	{
		RP_CODE_AUTH,
		(UBYTE *) RP_CODE_AUTH_DATA,
		(UBYTE *) (RP_CODE_AUTH_DATA + sizeof(RP_CODE_AUTH_DATA))
	},

	{
		RP_ID_AID,
		(UBYTE *) RP_ID_AID_DATA,
		(UBYTE *) (RP_ID_AID_DATA + sizeof(RP_ID_AID_DATA))
	},
        
        {
		RP_LABEL_APP,
		(UBYTE *) RP_APP_LABEL_DATA,
		(UBYTE *) (RP_APP_LABEL_DATA + sizeof(RP_APP_LABEL_DATA))
	},        

	{
		RP_NO_RRN,
		(UBYTE *) RP_NO_RRN_DATA,
		(UBYTE *) (RP_NO_RRN_DATA + sizeof(RP_NO_RRN_DATA))
	},
	
	{
		RP_DATE_EXP,
		(UBYTE *) RP_EXP_DATE_DATA,
		(UBYTE *) (RP_EXP_DATE_DATA + sizeof(RP_EXP_DATE_DATA))
	},

	{
		RP_TXT_FOOTERS,
		(UBYTE *) RP_TXT_FOOTERS_DATA,
		(UBYTE *) (RP_TXT_FOOTERS_DATA + sizeof(RP_TXT_FOOTERS_DATA))
	},

	{
		RP_TXT_HOST,
		(UBYTE *) RP_TXT_HOST_DATA,
		(UBYTE *) (RP_TXT_HOST_DATA + sizeof(RP_TXT_HOST_DATA))
	},

	{
		RP_SIGN,
		(UBYTE *) RP_SIGN_DATA,
		(UBYTE *) (RP_SIGN_DATA + sizeof(RP_SIGN_DATA))
	},

	/////////////////////////////////////////////////////////////////

    {
		RP_TEST, 
		( UBYTE * ) RP_TEST_DATA,
		( UBYTE * ) ( RP_TEST_DATA + sizeof( RP_TEST_DATA ) )
	},   

    {
		RP_DATABASE, 
		( UBYTE * ) RP_DATABASE_DATA,
		( UBYTE * ) ( RP_DATABASE_DATA + sizeof( RP_DATABASE_DATA ) )
	},

    {
		RP_DATASCOPE, 
		( UBYTE * ) RP_DATASCOPE_DATA,
		( UBYTE * ) ( RP_DATASCOPE_DATA + sizeof( RP_DATASCOPE_DATA ) )
	},

    {
		RP_HEAD_MRCH, 
		( UBYTE * ) RP_HEAD_MRCH_DATA,
		( UBYTE * ) ( RP_HEAD_MRCH_DATA + sizeof( RP_HEAD_MRCH_DATA ) )
	},

    {
		RP_TOTALS, 
		( UBYTE * ) RP_TOTALS_DATA,
		( UBYTE * ) ( RP_TOTALS_DATA + sizeof( RP_TOTALS_DATA ) )
	},

    {
		RP_HEAD_MRCH2, 
		( UBYTE * ) RP_HEAD_MRCH2_DATA,
		( UBYTE * ) ( RP_HEAD_MRCH2_DATA + sizeof( RP_HEAD_MRCH2_DATA ) )
	},

    {
		RP_HEAD_COMMON, 
		( UBYTE * ) RP_HEAD_COMMON_DATA,
		( UBYTE * ) ( RP_HEAD_COMMON_DATA + sizeof( RP_HEAD_COMMON_DATA ) )
	},

    {
		RP_HEAD_SERIAL, 
		( UBYTE * ) RP_HEAD_SERIAL_DATA,
		( UBYTE * ) ( RP_HEAD_SERIAL_DATA + sizeof( RP_HEAD_SERIAL_DATA ) )
	},

    {
		RP_SETTLE_SUM, 
		( UBYTE * ) RP_SETTLE_SUM_DATA,
		( UBYTE * ) ( RP_SETTLE_SUM_DATA + sizeof( RP_SETTLE_SUM_DATA ) )
	},

    {
		RP_PERFORMANCE, 
		(UBYTE *) RP_PERFORMANCE_DATA,
		(UBYTE *) (RP_PERFORMANCE_DATA + sizeof(RP_PERFORMANCE_DATA))
	},

    {
		RP_ISSUER_CONFIG, 
		(UBYTE *) RP_ISSCONFIG_DATA,
		(UBYTE *) (RP_ISSCONFIG_DATA + sizeof(RP_ISSCONFIG_DATA))
	},

    {
		RP_ACQUIRER_CONFIG, 
		(UBYTE *) RP_ACQUIRER_DATA,
		(UBYTE *) (RP_ACQUIRER_DATA + sizeof(RP_ACQUIRER_DATA))
	},


        {
		RP_CARD_CONFIG, 
		(UBYTE *) RP_CARD_DATA,
		(UBYTE *) (RP_CARD_DATA + sizeof(RP_CARD_DATA))
	},

        {
		RP_PAN, 
		(UBYTE *) RP_PAN_DATA,
		(UBYTE *) (RP_PAN_DATA + sizeof(RP_PAN_DATA))
	},

#ifdef MAKE_KIB
        {
		RP_AVAILABLE_LOYALTY_TOTAL,
		(UBYTE *) RP_AVAILABLE_LOYALTY_TOTAL_DATA,
		(UBYTE *) (RP_AVAILABLE_LOYALTY_TOTAL_DATA + sizeof(RP_AVAILABLE_LOYALTY_TOTAL_DATA))
	},
#endif
        
        {
		RP_DISCOUNT,
		(UBYTE *) RP_DISCOUNT_DATA,
		(UBYTE *) (RP_DISCOUNT_DATA + sizeof(RP_DISCOUNT_DATA))
	},

        {
		RP_POINTPART1,
		(UBYTE *) RP_POINTPART1_DATA,
		(UBYTE *) (RP_POINTPART1_DATA + sizeof(RP_POINTPART1_DATA))
	},
        
         {
		RP_POINTPART2,
		(UBYTE *) RP_POINTPART2_DATA,
		(UBYTE *) (RP_POINTPART2_DATA + sizeof(RP_POINTPART2_DATA))
	},

        {
		RP_LOYALTYAMOUNT,
		(UBYTE *) RP_LOYALTYAMOUNT_DATA,
		(UBYTE *) (RP_LOYALTYAMOUNT_DATA + sizeof(RP_LOYALTYAMOUNT_DATA))
	},                       

        {
		RP_POINTINSTFAIL,
		(UBYTE *) RP_POINTINSTFAIL_DATA,
		(UBYTE *) (RP_POINTINSTFAIL_DATA + sizeof(RP_POINTINSTFAIL_DATA))
	},    

        {
		RP_MARKER, 
		0, 
		0
	},
};



//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================



//-----------------------------------------------------------------------------
//  PrtDefInitFuncName      Print Def Initialization
//
//  Parameters:             None
//
//  Global Inputs:
//
//  Returns:                None
//
//  Notes:
//
//-----------------------------------------------------------------------------

void PrtDefInit( void )
{
    // register report table address from this page
    SDK_RegisterAddress( "REPORT_TAB", SDK_ThisPage(  ),
                         ( UBYTE * ) & ReportTable[0] );
}


//-----------------------------------------------------------------------------
//  FindTxtTableType        FindTxtTableType
//
//  Parameters:
//      None
//  Global Inputs:
//
//  Returns:
//      Nothing
//  Notes:
//
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

		case TX_SERVUS:
			TxtTableId = pcltext_servusDef_id;
			break;

            default:            // Unknown, notify user
				bRetVal = False;
                ShowErrMsg( InvalidEntry );
                break;

      }
	return bRetVal;
}
