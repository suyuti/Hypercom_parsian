//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      hdtestext.c
//
// File Description:
//      Spanish message table
//
//=============================================================================

#ifdef MAKE_SPANISH

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"

#include "struct.h"
#include "appdata.h"
#include "msg.h"
#include "rsptext.h"
#include "hdttextinit.h"
#include "syserr.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================


//=============================================================================
// Public data definitions
//=============================================================================

// Hdt and pinpad Spanish messages table for Dynamic Multilingual Support


//====================================================
//!===  GENERAL SPANISH MESSAGE TABLE               ==
//====================================================
const char *const SpanishMessages[] = {
	"CM",						// CustomMsg
	"CM2",						// CustomMsg2
	"CM3",						// CustomMsg3
	"CM4",						// CustomMsg4
	"CM5",						// CustomMsg5
	"CM6",						// CustomMsg6
	"CM7",						// CustomMsg7
	"CM8",						// CustomMsg8
	"PASE LA TARJETA",			// Swipecard
	"INTRODUZCA N. TARJE.",		// EnterCard
	"**ERROR DE TARJETA**",		// CardErr
	"TARJETA INVALIDA",			// UnsuppCard
	"TARJETA INVALIDA",			// InvalidCard
	"TARJETA CADUCADA",			// ExpiredCard
    "TARJETA PREVALIDADA",		// PrevalidCard
	"IMPOSIBLE ENT.MANUAL",		// NoManEntry
    "FECHA INICIO MMYY",		// StartDate
    "TEMA NUMERO",				// IssueNum
	"CADUCIDAD MMYY",			// ExpireDate
	"???",						// TakeImprint
	"ULTIMOS 4 NUMEROS DE",		// EntPan4
	"LA TARJ. DEL CLIENTE",		// CustomerCard
	"INGRESE NRO. CAJERO",		// CashierNum
	"INGRESO INVALIDO",			// InvalidEntry
    "IMPORTE INVALIDO",			// InvalidAmount
	"IMPORTE",					// AmountEnt
	"CONFIRME IMPORTE",			// AmountAgain
	"IMPORTE BASE",				// AmountBase
	"BAR",						// AmountBar
	"IMPUESTO",					// AmountTax
	"PROPINA",					// AmountTip
	"VENTA",					// AmountSale
	"EFECTIVO",					// AmountCash
	"IMPORTE EFECTIVO",			// AmountTender
	"TOTAL",					// AmountTotal
	"CORRECTO ? SI O NO",		// CorrectYN
	"ENTRE PLAN DE CRED.",		// CreditPlan
	"SELECCIONE PRODUCTO",		// EntDescrip
	"NUMERO DE BOLETA",			// InvoiceNum
	"ESPERANDO APROBACION",		// WaitApprov
	"ESPERANDO LINEA",			// LineIdle
	"PROBANDO LINEA",			// CheckingLine
	"LINEA OCUPADA ESPERE",		// LineBusy
	"ESPERANDO TONO",			// WaitDialTone
	"TOMANDO LINEA",			// DialNow
	"ESPERANDO RESPUESTA",		// WaitForAnswer
	"ESPERANDO LINEA",			// WaitForLine
	"CONFIGURANDO MODEM",		// TrainModem
	"PROCESANDO",				// ProcNow
	"TRANSMITIENDO",			// Transmitting
	"MARCANDO",					// NowDialing
	"INICIANDO MODEM",			// InitModem
    "CONEXION DE RED",			// NetworkLogon
    "LOCALIZANDO",				// Paging
    "CONEC. PUERTA ENLACE",		// GateConnecting
    "ENLAZANDO...",				// Linking
    "CONECTANDO...",			// Connecting
	"ESTADO PAG. MEMORIA:",		// MemPgStatus
	"CONFIGURE EL TERMINAL",	// ConfigTerm
	"LLAMAR SERVICO TECN.",		// CallHelp
	"INICIALICE POR FAVOR",		// PleaseInit
	"",							// BlankMsg
	"ERROR DE IMPRESORA",		// PrintError
	"MEMORIA DE TRANS.",		// TransMemErr
	"ESPERE AUTO-PRUEBA",		// SelfTest
	"NO HAY SUF. MEMORIA",		// NotEnoughMem
	"PARA LA APLICACION",		// ToRunApplic
	"INICIALICE",				// InitTran
	"FUNCION DE COMERCIO",		// MerchantFunc
	"CONCILIACION",				// SettleTran
	"SALDO",					// BalanceTran
	"DEVOLUCION",				// RefundTran
	"REIMPRIMIR BOLETA",		// ReprintTran
	"ULTIMO",					// ReprintLast
	"CARGO",					// ReprintInvoice
	"** FUERA DE LINEA **",		// OfflineTran
	"INTRO. NRO. BOLETA",		// EnterInvNum
	"INTRO. NRO. SOCIAL",		// EnterSOCNum
	"P.O. / NUMERO I.D.",		// EnterPONum
	"FUNCION DE COMERCIO",		// MercFunction
	"CLAVE DE ACCESO",			// MaintPswd
	"CLAVE INCORRECTA",			// PswdNotMatch
	"CORRECTO ?",				// CorrectYesNo
	"PROG. DE COMERCIO",		// MerchProg
	"CLAVE DE ACCESO",			// EnterPswd
	"ID DEL TERMINAL",			// EnterTSer
	"NRO. TFNO PRINCIPAL",		// EnterInitTel
	"NRO. TFNO NMS",			// EnterNMSTel
	"MODO DE MARCADO",			// EnterDialMode
    "FIRMA",					// Signature
	"Por favor firme",			// SignaturePlease
	"Y presione ENTER",			// PressEnter
	"Firmar otra vez?",			// RetrySigCap
	"CODIGO ACCESO PABX",		// EnterPABX
	"PULSE 'CANCELAR'",			// ExitPressCncl
	"DUPLICAR TRANS.",			// DuplicateTrans
	"INTRODUCIR?",				// AddYesNo
	"NADA PARA IMPRIMIR",		// NoInvoice
	"ESPERE POR FAVOR",			// PleaseWait
	"TRANSACCION COMPLETA",		// TransComplete
	"TRANSACTION ACEPTADA",		// TransAccepted
	"CARGA DE PROGRAMA",		// ProgamLoad
	"CLAVE DE FUNCTION 99",		// Fun99Password
	"FUNCION DE PROGRAMA",		// ProgFunction
	"SELEC. OPC. LOCALES",		// LocalOptSel
	"SELEC. CONFIG. LOCAL",		// LocalConf
	"CANT. MAX. EXCEDIDA",		// ExceedMax
	"MENSAJES DE MARCADO ",		// EnhDial
	"0-LL 1-DIAL 2,3-LAN",		// LDLPmt
	"DIRECCION CU",				// CUAddrP
	"ACTUAL",// Currently
	"INTRODUCIR/ELIMINAR",		// SignOnOff
	"ENTRE NRO. DE CAJA",		// TillNum
	"INTRODUCIR",				// SignOnYN
	"CLAVE ACCESO ANULAR",		// VoidPassM
	"CLAVE ACCESO CREDITO",		// RefPassM
	"CLAVE ACCESO AJUSTE",		// AdjPassM
	"CLAVE ACCESO INFORME",		// RepPassM
	"PERMITE USO TELEFONO",		// KbdDialM
	"PERMITE LARGA DIST.",		// TollDialM
	"MEMORIA DE MARCADO  ",		// MemDialM
	"MARCA PARA PREFERIDA",		// RefDialM
	"BLOQUEAR TERMINAL ?",		// KeyLock
	"ENTRE NRO. DE CUENTA",		// EnterAcct
	"CLAVE ACCESO ANTERIOR",	// OldPassword
	"NUEVA CLAVE ACCESO",		// NewPassword
	"REPITA NUEVA CLAVE",		// NewPassAgn
	"SIN IMPRESORA",			// NoPrinter
	"REIMPRESION COMPLETA",		// ReprintComplete
	"CARGO NO ENCONTRADO",		// InvNumberMissing
	"IMPRIMIENDO",				// PrintingWait
	"SERVIDORES REGISTRA.",		// ServerOn
	"CAJEROS REGISTRADOS",		// CashierOn
	"PRUEBA DE TECLADO",		// KeyBrdTest
	"P/ SALIR P/ TERMINAR",		// Cancel2End
	"TECLA =  ",				// DispKeyP
	"SE REQUIRE CIERRE",		// SettlementRequired
	"LECTURA DE TARJETA",		// CardRdT
	"IMPRESORA EN LINEA",		// PrinterEP
	"IMPRESORA DESACTIVADA",	// PrinterDP
	"ACTIVAR  ?",				// EnablePP
	"DESACTIVAR ?",				// DisablePP
	"REVISANDO LOTE",			// ScanningBatch
	"SIN TRANSACCIONES",		// NoBatchTotals
	"NO HAY MAS TOTALES",		// NoMoreTotals
	"PRUEBA DE IMPRESION",		// TestPrnC
	"BLOQUEADA",				// Closed
	"EMTRE FECHA MMDDAA",		// DateMDY
	"ENTRE HORA HHMM",			// TimeHM
	"LOTE ACTUAL DEL HOST",		// HostBatch
	"NUM. DE ACQUIRER",			// NoAcq
	"COMIENZO DEL LOTE:",		// StartOfBatch
	"FIN DEL LOTE:",			// EndOfBatch
	"SELECIONE NRO. TURNO",		// SelectShiftNum
	"0 PARA TODOS TURNOS",		// ZeroForAllShift
	"CAJA",						// ReportMenuTill
	"CAJERO",					// ReportMenuCashier
	"SERVIDOR",					// ReportMenuServer
	"REVISAR",					// ReportMenuAudit
	"RESUMEN",					// ReportMenuSummary
	"MESA AB",					// ReportMenuOpenTabs
	"1=DETALLE 3=SIN AJUS",		// CashServMenuLine1
	"2=RESUMEN",				// CashServMenuLine2
	"ANULACION NO PERMIT.",		// VoidNotAllowed
	"ENTRE NRO. DE HOST",		// HostNum
	"TIPO ?",					// Type
	"AJUSTE NO PERMITIDO",		// AdjustNotAllowed
	"REINTENTE CIERRE",			// RetryBatchTrans
	"TOTAL",					// AmountTotals
	"NUEVO IMPORTE?",			// AmountNew
	"ENTRE TEXTO RECIBO",		// RecieptTXT4
	"ENTRE COD. AUTORIZA.",		// EnterApp
	"*MODO ENTRENAMIENTO*",		// TrainingMode
	"IR A MODO ENTRENA.?",		// GoToTrainingMode
	"SALIR MODO ENTRENA.?",		// LeaveTrainingMode
	"NO SE PERMITE CREDIT",		// RefundNotAllowed
	"TOTAL VENTAS",				// SalesTotal
	"ERROR TOTAL VENTAS",		// SalesTotalsWrong
	"TOTAL CREDIT",				// RefundTotal
	"ERROR TOTAL CREDITOS",		// RefundTotalsWrong
	"TOTALES",					// BatchTotals
	"DESCARGO",					// BatchRelease
	"TRANS. DE PRUEBA",			// TestTransactions
	"SIN TRANSACCIONES",		// NoTransactions
	"INCREMENTAR",				// IncrementShift
	"RESTABLECER",				// ResetShift
	"SIN COMUNICACION",			// NoComms
    "SIN LINEA",				// NoPhoneline
	"INTRODUZCA PROPINA",		// EnterTip
	"INTRODUZCA TOTAL",			// EnterTotal
	"1-ABR. TAB 3-TAB RPT",		// TabProcMsg1
	"2-CERRAR TAB",				// TabProcMsg2
	"ENTRE NRO. SERVIDOR",		// ServerNum
	"TODO TAB ABIERTO",			// AllOpenTabs
	"NO HAY TAB ABIERTO",		// NotOpenTab
	"CONFIGURAR",				// N_Configure
	"INICIAR",					// N_Initialize
	"PRINCIPAL",				// N_Main
	"TRANSACCION",				// N_NewTrans
	"LOTE",						// N_Batch
	"INFORME",					// N_Reports
	"TERMINAL",					// N_Terminal
	"MAS",						// N_More
	"MENU",						// N_Menu
	"FECHA/HORA",				// N_Datime
	"IMPRESORA",				// N_Printer
	"COMUNICACIONES",			// N_Comms
	"MARCADO",					// N_Dial
	"CLAVES",					// N_Passwords
	"VELOCIDAD MARCADO",		// N_Speedial
	"TARJETA",					// N_CardRdr
	"CONTRASTE",				// N_Contrast
	"VENTA",					// N_Sale
	"VENTA+EFECTIVO",			// N_SalCash
	"CORREO",					// N_MailOrder
	"F LINEA",					// N_Force
	"AUTORIZACION",				// N_Auth
	"VERIFICICACION",			// N_CardVer
	"REVISAR",					// N_Review
	"BUSCAR",					// N_Find
	"AJUSTAR",					// N_Adjust
	"ANULAR",					// N_Void
	"EFECTIVO",					// N_Cash
	"TELECARGAR",				// N_ProgLoad
	"TURNO",					// N_Shift
	"VER",						// N_View
	"ENTRENAMIENTO",			// N_TrainMode
	"CONFIG",					// N_Setup
	"FUNCION",					// N_Function
	"PRUEBAS",					// N_Tests
	"MESA",						// N_Tab
	"REIMPRIMIR",				// N_Reprint
    "EXIT",						// N_Exit
	"ERROR",					// N_Error
	"BORRAR",					// N_Clear
	"B.CAJERO",					// N_ClearChsTbl
	"B.MESERO",					// N_ClearSerTbl
	"ENTRAR",					// N_Enter
	"CANCELAR",					// N_Cancel
	"REINICIAR",				// N_Reset
	"RAPIDO",					// N_Quick
	"INTRODUCIR/ELIMINAR",		// N_AddRmv
	"DETALLE",					// N_Detail
	"RESUMEN",					// N_Summary
	"NO-AJUS",					// N_Unadjusted
	"SERVIDOR",					// N_Host
	"DISPLAY",					// N_Display
	"TECLADO",					// N_Keyboard
	"PINPAD",					// N_Pinpad
	"TELEFONO INICIO",			// N_InitPhone
	"TEL NMS",					// N_NMSPhone
	"TIPO DE MARCADO",			// N_DialType
	"CENTRALITA",				// N_PABX
	"NUMERO DE TERMINAL",		// N_TermId
	"TODAS",					// N_All
	"ACTIVAR/DESACTIVAR",		// N_OnOff
	"RECIBO",					// N_Receipt
	"TEXTO",					// N_Text
	"INTRODUCIR PAPEL",			// N_PaperFeed
	"SALIR",					// N_Quit
	"CAMBIAR",					// N_Change
	"",							// N_NullStr
	"COMUNICACIONES",			// N_Communications
	"CAMBIO CLAVE",				// N_PswdChange
	"AUMENTAR",					// N_Higher
	"DISMINUIR",				// N_Lower
	"OPCIONES LOCALES",			// N_LocalOpts
	"RESPUESTA",				// N_Response
	"ABRIR",					// N_Open
	"CERRAR",					// N_Close
	"IMPRIMIR",					// N_Print
	"VER MAS",					// N_Scroll
	"ENTRE FUNCION",			// N_EnterFunction
	"IMPRIMIR SERVIDOR",		// N_PrintHost
	"INFORME RESPALDO LIN",     // N_DBRpt
	"INFORME TxRx",     	// N_TxRxRpt
	"VER TABLA",				// N_ViewTable
	"PRUEBA DISPLAY",     // N_DisplayTest
	"FUNCION NO PERMITIDA",		// N_FuncNotSupported
	"VERSION",					// N_Version
	"ULTIMA RESPUESTA",			// N_LastRsp
	"ULTIMA",					// N_Last
	"SIN DATOS",				// N_NoData
	"INFO LOTE",				// N_BatchInfo
	"ID SERVIDOR",     // N_HostId
	"NOMBRE SERVIDOR",     // N_HostName
	"# DEL LOTE : ",			// N_BatchNumber
	"0 PARA TODO HOST",			// N_ZeroForAllHost
	"INFORME",     // N_Report
	"DEBITO",					// N_Debit
	"CREDITO",					// N_Credit
	"ENTRE CODIGO AUT",			// N_EnterAuth
	"NUM DE GENERACION",		// N_EnterGenNum
	"PRUEBA DIARIA",     // JournalTesting
	"ENTRE No DE ENTRADAS",		// NumEntries
	"SIN PAPEL",				// PaperOut
    "IMPRESORA RECALENTA.",     // PrinterOverheat - NEED TO BE TRANSLATED !!
	"ESPERANDO NRO.CUENTA",		// WaitingForAcct
	"ESPERANDO APROBACION",		// WaitingForApproval
	"ESPERANDO POR PIN",		// WaitingForPin
	"PASE TARJ. EN PINPAD",		// SwipeCardPin
	"LECTURA POR PIN?",			// ChangeToPinRead
	"LECTURA POR TERM?",		// ChangeToTermRead
	"CUENTA INVALIDA",			// InvalidAccount
	"BAL. DISPLAY",     // AvailBal
	"HOY",				// Today
	"TARJETA NO COINCIDE",     // CardNoMatch
	"PRUEBA DE PINPAD",			// PinPadTest
	"DETECTANDO PINPAD",		// PinPadAutoDetect
	"SELECCIONE CUENTA",     // SelectAccount
	"CUENTA",					// N_Account
	"0",						// N_0
	"1",						// N_1
	"2",						// N_2
	"3",						// N_3
	"4",						// N_4
	"INTRODUZCA PIN",     // EnterPin
	"SIN PINPAD EXTERNO",		// ExtPinNot
	"ERROR DE CIFRADO",     // EncryptError
	"CONEXION",     // N_Logon
	"MANTENIMIENTO",			// MaintFunction
	"ENTRE FECHA ORIGINAL",		// N_OrigDate
	"IDENTIFICACION",			// N_Identification
	"SOFTWARE",					// N_Software
	"DLL",						// N_DLL
	"BOOTER",					// N_EPROM
	"REFER",					// N_Referral
	"TODOS SERVIDORES",     // N_AllHosts
	"ICEPAC",					// N_IcePac
	"PRES. ENTRAR P/TODOS",		// PressEnterForAll
	"ENTER PARA ULT.CARGO",		// EnterForLastInv
	"TIEMPO EXCEDIDO",			// N_Timeout
	"USARIO CANCELADO",			// N_UserCancelled
	"REVERSOS",					// N_Reversals
	"PINPAD NO ENCONTRADO",		// PinpadNotFound
	"SI",						// N_Yes
	"NO",						// N_No
	"OK",						// N_OK
	"SALIR",     // N_QUIT
	"PROXIMO",     // N_NEXT
	"PASE/INSERTE TARJETA",     // N_SwipeOrInsertCard
	"X",						// N_X
	"CONTINUAR",     // N_Continue
	"TECLA",					// N_KEY
	"CORRECTA",     // N_Passed
	"PERDIDO",					// N_Failed
	"PRUEBA",					// N_Test
	"TONO",						// N_Tone
	"PULSO",					// N_Pulse
	"ERROR LECT. PINPAD",		// PinReadErr
	"ERROR ESCRIT. PINPAD",		// PinWriteErr
	"DESACTIVAR-FIRMA?",		// SignOffYN
	"ENERO",     // Month_1
	"FEBRERO",     // Month_2
	"MARZO",     // Month_3
	"ABRIL",     // Month_4
	"MAYO",     // Month_5
	"JUNIO",     // Month_6
	"JULIO",     // Month_7
	"AGOSTO",     // Month_8
	"SEPTIEMBRE",     // Month_9
	"OCTUBRE",     // Month_10
	"NOVIEMBRE",     // Month_11
	"DICIEMBRE",     // Month_12
	"REENTRE PIN",				// Msg_Reenter_Pin
	"PIN?",						// Msg_Pin
	"EXTERNO: ",				// Msg_External
	"INTERNO: ",				// Msg_Internal
	"CKSUM: ",					// Msg_Chksum
	"CORRECTO ?",				// Msg_Correct
	"SVR:",						// Msg_SVR
	"CSH:",						// Msg_CSH
	"INV:",						// Msg_INV
	"REVOCADA",     // Msg_reversal
	"anulada",					// Msg_voided
	"AVISO",     // Msg_advice
	"NRO. SECUENCIA",     // Msg_SequenceNo
	"ARTICULO",     // Msg_Items
	"VENTAS",					// Msg_Sales
	"ERROR **********",			// Msg_Error
	"SERVIDORES DISPONIB.",     // Msg_AvailHosts
	"EFECTIVO",     // Msg_Cash
	"Ofertar: ",				// Msg_Tender
	"IMPORTE OPCIONAL",     // AmountOpt
	"NUMERO CVV2",     // N_CVV2Indicator
	"0=SALTAR",					// N_CVV2IndEntry1
	"1=PRESENTE",				// N_CVV2IndEntry2
	"2=ILEGIBLE",				// N_CVV2IndEntry3
	"9=AUSENT",					// N_CVV2IndEntry4
	"CVV2 DE TARJ.",			// N_CVV2Entry
	"TARJ PRESENTE?",			// Msg_CardPresent
	"ENCAB 4",					// Msg_HeaderLine
	"TITULO",					// Msg_FooterLine
	"PASE SU TARJETA",     // Msg_SwipeYourCard
	"SELEC. OPCION PROPINA",	// Msg_PreTip
	"OTROS",					// Msg_Other
	"ERROR PROGRAMA:",			// Msg_ProgramError
	"PROPINA",					// Msg_Tips
	"PROPINA %",				// Msg_TipPct
	"% PROPINA SUGERIDA",		// Msg_Sug_Tip
	"OPCIONES LOCALES    ",     // Msg_LocalOpt
	"ERRORES TX/RX",			// Msg_TxRxErrors
	"ERR TRANSMISION:",			// Msg_TranErrors
	"ERR RECEPCION:",			// Msg_RecvErrors
	"CARGA ARCHIVO",			// N_FileDownLoad
	"CIERRE",					// N_Settle
	"FUERA DE LINEA",     // N_Offline
	"TRANSMISION",     // N_Trans
	"CONFIGURAR FECH/HORA",     // N_DateTimeSetup
	"CONTRASTE",     // N_Contrast
	"ALIMENTAR PAPEL",     // N_PaperFd
	"ESTADO DE CIERRE",			// N_SettleStat
	"Seleccione Opcion",		// Msg_ChooseSummary
	"AUTO",					// Msg_Auto
	"GRUPO",					// Msg_Group
	"MANUAL",				// Msg_Manual
	"NUNCA",					// Msg_Never
	"ARTICULOS DE VENTA",     // Msg_SaleItems
	"ARTICULO DEVUELTO",     // Msg_RefundItems
	"INTENTO # ",				// Msg_Attempt
	"RESP",					// Msg_Resp
	"TODO",						// Msg_Evrythg
	"TODO",						// Msg_Everything
	"TABLA HOST VACIA",     // Msg_SrvTabEmpty
	"TABLA CAJERO VACIA",		// Msg_CshTabEmpty
	"ESTADO",					// Msg_Status
	"ERROR ABRIR FICHERO ",     // Msg_fopenerror
	"Recibiendo archivo: ",		// Msg_DownloadingFile
	"SELECCION IDIOMA",     // Msg_SelectLanguage
	"IDIOMA",     // Msg_Language
	"INGLES",     // Lng_English
	"ALEMAN",     // Lng_German
	"SUECO",     // Lng_Swedish
	"ESPAN~OL",     // Lng_Spanish
	"PORTUGUES",				// Lng_Portugues
	"ACEPTA IDIOMA ACTUAL",     // Msg_CurrentLang
	"IDIOMA",     // Msg_Lang
	"INVALIDA TABLA CLAV.",     // Msg_KeyTableInvalid
	"INFORME BASE DATOS  ",     // Prt_DatabaseReport
	"---------------------\n",	// Prt_Dashes
	"CONFIG. TERMINAL EPROM\n",	// Prt_TermEpromCfg
	"CONFIGURACION TERMINAL\n",	// Prt_TermConfig
	"ACQUIRIENTE ID: ",     // Prt_AcquirerID
	"EMISOR ID: ",     // Prt_IssuerID
	"RANGO TARJETAS ID: ",		// Prt_CardRangeID
	"DESCRIPTOR ID:",			// Prt_DescriptorID
	"DATOS RECIBIDOS",     // Prt_ReceivedData
	"DATOS ENVIADOS",     // Prt_SendData
	"RAW DATA (HEX BYTES):\n",	// Prt_RawDataHexBytes
	"ESPERANDO ETHERNET",     // WaitingForLanCable
	"MODO LINEA CANCELADO",     // LineModeCanceled
	"REARRANCAR TERMINAL",     // RestartTerminal
	"SOLO 0-9 A-F VALIDOS",     // N_Only09AFvalid
	"FUNCION PARA BOTONES",		// N_IdleButton
	"SELECC. BOTON",			// SelectButton
	"SEL. TIPO DE EVENTO",     // EventType
	"BOTON",					// N_Button
	"TRANSAC. PERMITIDA",		// ConfirmTrans
	"FUNCION  PERMITIDA",		// ConfirmFunc
	"INIC. TABLA BINES",     // N_InitButTab
	"INIC. DESHABILITADO",     // Init_Dis
	"INIC. HABILITADO",			// Init_Ena
	"BTN IDL",					// N_IdlBtn
	"INIC. BTN",				// N_InitIdlBtn
	"MODIF. BTN",				// N_ChgIdlBtn
	"CAMBIO EVENTO BOTON 1",	// Chang_Btn1
	"CAMBIO EVENTO BOTON 2",	// Chang_Btn2
	"CAMBIO EVENTO BOTON 3",	// Chang_Btn3
	"CAMBIO EVENTO BOTON 4",	// Chang_Btn4
	"BOTON 1",					// B_1
	"BOTON 2",					// B_2
	"BOTON 3",					// B_3
	"BOTON 4",					// B_4
	"RETIRE LA COPIA",          // RemoveMerchantCopy
    "NO HAY TABLA P/IMPRIMIR",	// PclTabError
	"RENDIM",					// Prt_PerformBtn 
	"CONFIG",					// Prt_ConfBtn    
	"REPORTE DE CONFIG.",		// Config_RPT    
	"TERMINAL",					// Terminal_Tab 
	"EMISOR",					// Issuer_Tab 
	"ADQUIRIENTE",				// Acq_Tab 
	"TARJETA",					// Card_Tab 
	"TODO",						// All_Tab
	"LIBS VERSION",				// N_LibsVersion
	"SDK",						// N_SDKVersion
	"VERSION SISTEMA OPE.",     // N_OSVersion
	"REVISION SISTEM.OPE.",     // N_OSRevision
    "HFC",                      // N_HFCVersion
	"INFORME HISTORICO   ",     // History_RPT
	"ERROR LECTURA FICHE.",     // Msg_freaderror
	"ENTRA NUM. MAX. EXC.",     // N_EntryNumExceedsMax
	"TABLA ID:           ",     // N_TableId
	"HFS",						// N_HFS
	"ESPACIO INSUFIC. HFS",     // N_NoEnougthSpaceInHFS
	"SUPERVISOR PASSWORD",      // SVPassword
	"CENTRO AUTENTI. LLAM",     // CallAuthCentre
	"CODIGO AUTENTICACION",     // AuthCode
    "EFECTIVO NO PERMITI.",     // N_CashNotAllowed
	"BANDA NO PERMITIDA  ",     // SwipeNotAllowed
	"TRANSACCION NO PERM.",     // TranNotAllowed
    "INICIA INTENTOS MAX.",     // InitMaxTries
	"SUBIR",     // N_Up
	"BAJAR",     // N_Down
	"SELECCIONAR",     // N_Select
	"REIMPRIMIR",     // N_ReprintShort
	"CABEZA",     // Msg_HeaderLine
	"VERTICAL",     // N_Veritcal
	"HORIZONTAL",     // N_Horizontal
	"MODEM",     // N_Modem
	"COM1",                     // N_RS232RS0
	"COM2",                     // N_RS232RS1
	"MODEM DIRECCION CU",     // N_RS232   
	"MODEM CONFIGURACION",     // N_RS232Config
	"MODEM INTERFACE",     // N_EnableComm 
	"MODEM PROTOCOLO",     // N_RS232Prot	
	"MODEM VELOCIDAD(bps)",     // N_RS232Baud  
	"1200",                     // N_Baud1200   
	"2400",                     // N_Baud2400   
	"4800",                     // N_Baud4800   
	"9600",                     // N_Baud9600   
	"19200",                    // N_Baud19200  
	"38400",                    // N_Baud38400  
	"57600",                    // N_Baud57600  
	"115200",                   // N_Baud115200 
	"MODEM BITS DE PARADA",     // N_RS232Stop  
	"1  bit",                   // N_RS232Stop1 
	"2 bits",                   // N_RS232Stop2 
	"MODEM BITS DE DATOS",     // N_RS232Data  
	"7 bits",                   // N_RS232Data7 
	"8 bits",                   // N_RS232Data8 
	"MODEM PARIDAD",     // N_RS232Parity
	"NINGUNA",     // N_RS232None  
	"IMPAR",     // N_RS232Odd   
	"PAR",     // N_RS232Even  
	"MAPA DE CARACTERES",     // N_CharMap	
    "MENSAJES DE LINEA",     // N_DialMsgs
	"MODEM FLUJO DE CONTR",     // N_RS232FlowC
    "RX",                       // N_RX
    "RX / TX",                  // N_RXTX
    "MODEM MODO",     // N_CommsModemMode
    "MODEM RESPALDO LINEA",     // N_CommsDBModemMode
    "MODEM CAMBIADO",     // N_CommsChanged
    "REARRANCAR TERMINAL",     // N_RestartTerminal
    "PULSE CANCEL SALIR",     // MsgPressCancelToExit
    "CAMBIAR APLICACION",     // N_ChangeApp
    "SELECCION APLICACION",     // N_SelectApplication
    "TRAZAR REGISTRO COM.",     // N_TraceLog
    "COMENZAR DE NUEVO",     // N_StartNew
    "ENVIAR VIA COM1",     // N_Send1
    "ENVIAR VIA COM2",     // N_Send
    "PARAR",     // N_Stop
    "REGISTRO VACIO",     // N_TraceIsEmpty,
    "****REGISTRANDO*****",     // Tracing
    "***TRAZA COMPLETA***",     // TraceOverflow
	"GSM",                      // N_GSM
    "ETHERNET",             // N_Ethernet
    "LAN",                  // N_LAN
    "BLUETOOTH",            // N_Bluetooth
    "RDSI",     // N_ISDN
    "RESPALDO LINEA TELEF",     // N_DialBackUp    
	"INI. NRO TELEFONO RL", 	// EnterDBInitTel
	"NMS NRO TELEFONO RL ",     // EnterDBNMSTel
	"INI. SERVICIO PREFE.",     // InitPrefService
	"NMS SERVICIO PREFER.",  	// NMSPrefService
	"SENAL BAJA",     // LowSignal
	"INICIAR PREFERIDO",     // N_DBInitPref
	"NMS PREFERIDO",     // N_DBNMSPref
	"INICIAR RESPALDO LIN",     // N_DBInitPhone
	"NMS RESPALDO LINEA",     // N_DBNMSPhone
	"GSM CON RESPALDO LIN",     // N_GSMwDial
	"RESPALDO LIN CON GSM",     // N_DialwGSM
	"GSM SOLO",     // N_GSMonly
	"RESPALDO LINEA SOLO",     // N_Dialonly
	"INICIANDO MODEM GSM",     // StartGSMModem
	"PROCEDIMIENTO CANCEL",     //ProcWasCanel
	"FALTA SIM",     // NoSIM	
	"SIM PROHIBIDA",     // ForbSIMStatus
	"INSERTE SU SIM",     // Msg_InsertYourCard
    "ULTIMA DIRECCION IP",     // N_LastIPaddr
    "BATERIA BAJA",     // BatteryLow
    "ERROR DESBLOQUEANDO",     // UnlockingError - NEED TO BE TRANSLATED

#ifdef	MAKE_EMV
	"EMV INFO ENABLED",         // EmvInfoEna
	"EMV INFO DISABLED",        // EmvInfoDis
	"DISABLE EMV INFO ?",      // DisEmvInfo
	"ENABLE EMV INFO ?",      // EnaEmvInfo
	"PROCESSING CARD",			// ProcCard
	"SMART CARD",				// SmartCard
	"PLEASE REMOVE CARD",		// PlsRemoveCared
	"CARD DECLINED",			// CardDeclined
	"PIN FAILED",				// PINFailed
	"DESLICE/INSERTE TARJ.",	// SwipeInsert
	"CARD FAIL",				// CardFail
	"NO MORE APPS",				// NoMoreApps
	"CARD BLOCKED",				// CardBlocked
	"PIN BLOCKED",				// PinBlocked
	"APPLICATION BLOCKED",		// AppBlocked
	"EMV",						// N_EMV
	"APP SELECTION",			// AppSelection
	"AUTHENTICATION",			// Authentication
	"EMV DEBUG",				// EMVdebug
	"CHIP NOT SUPPORTED",		// ChipNotSupported
	"ON PINPAD",				// OnPinpad
	"WAITING FOR PIN",			// WaitingForPin
    "BYPASS",                   // N_Bypass
    "PINPAD IS ENABLED",		// PinpadEP
    "PINPAD IS DISABLED",		// PinpadDP
	"ACCEPTED",                 // Accepted

    "EASY ENTRY",           // "EASY ENTRY"
    "LAST TRY LEFT",        // "LAST TRY LEFT"
    "TRIES LEFT: ",         // "TRIES LEFT: "
    "CARD FAIL",            // "CARD FAIL"
    "PIN TRIES EXCEEDED",   // "PIN TRIES EXCEEDED"
    "PIN BLOCKED",          // "PIN BLOCKED"
    "LAST PIN TRY LEFT",    // "LAST PIN TRY LEFT"
    "PRESS OK TO CONTINUE", // "PRESS OK TO CONTINUE"
    "SELECTING",            // "SELECTING"
    "BLOCKED",              // "BLOCKED"
    "CHIP CARD",            // "CHIP CARD"
    "PLEASE INSERT CARD",   // "PLEASE INSERT CARD"
    "CONTINUE?",            // "CONTINUE?"
       
    "APP BLOCKED",          // "APP BLOCKED"
    "LIST EMPTY",           // "LIST EMPTY"
    "APPLICATION SELECT",   // "APPLICATION SELECT"
    "SELECT",               // "SELECT"
    "CARD DECLINE",         // "CARD DECLINE"
    "OVERRIDE?",            // "OVERRIDE?"
#endif	// MAKE_EMV

#ifdef	MAKE_SMARTINIT                
    "SMART INIT ENABLED",         // SmartInitEna 
	"SMART INIT DISABLED",        // SmartInitDis 
	"DISABLE SMART INIT?",      // DisSmartInit 
	"ENABLE SMART INIT ?",      // EnaSmartInit
#endif // MAKE_SMARTINIT

#ifdef MAKE_FARSI
    "PERSIAN", // Lng_Farsi						
#endif
    "N_IpCon",
    "N_NMSIp",
    "N_NMSPort",
    "N_InitIp",
    "N_InitPort",
    "N_LocalIp",
    "N_LocalSubnetMask",
    "N_GatewayIp",
    "N_LastSettle",
    "N_DBFormattedReport",
    "N_AcqTableReport",
    "N_TermTableReport",
    "N_IssTableReport",
    "N_CardTableReport",
    "N_PreAuth_Long",
    "N_CancelPreAuth",
    "N_ClosedPreAuth",
    "N_CancelClosedPreAuth",
    "N_SaleRefund",
    "N_InstalRefund",
    "N_PreAuth",

        "TERMINAL NOT ALLOWED",    // N_TerminalNotAllowed
        "TRANSACTION UNSUPPORT",   // TranUnsupported
        "INSTALMENT",              // Installment
        "YYYY/MM/DD",              // gg_aa_yy
        "ENTER TRANS DATE",        // TarihGir
        "INS.REFUND",              // N_InstalRefund
        "PREAUTH",                 // N_PreAuth
        "PREAUTH",                // N_PreAuth_Long
        "CLOSED PREAUTH",          // N_ClosedPreAuth
        "CANCEL PREAUTH",          // N_CancelPreAuth
        "CANCEL CLOSED P.",        // N_CancelClosedPreAuth
        "PRESS ANY KEY",           // N_ClickKey
        "FOR SECOND RECEIPT",      // N_SecondReceipt
        "USE MAGNETIC STRIPE",     // UseMagneticStripeMsg
        "  NO MANUEL ENTRY  ",     // Msg_NoManEntry,         // @ydeniz --  BC --   
        "OVER INSTALLMENT",        // Msg_OverInsNum
        "LOWER INSTALLMENT",       // Msg_LowerInsNUm
        "INS. NUM.",               // InstNum  "TAKSÝT ADETÝ:",			// 462	// 
		"FORMATTED REP",                // N_DBFormattedReport @erd 06.03.2007 !!!
		"ACQ. TABLE RPT.",          // N_AcqTableReport  @erd 06.03.2007
		"TERM. TABLE RPT.",          // N_TermTableReport @erd 06.03.2007
		"ISS. TABLE RPT.",            // N_IssTableReport  @erd 06.03.2007
		"CARD TABLE RPT.",              // N_CardTableReport @erd 06.03.2007
		"SELECT = '.'",				// N_Selectkey
		"WRONG CARD",				// Msg_WrongCard
        "PREAUTH NOT ALLOWED", // Msg_PreAuthNotAllowed
        "CLOSED PREAUTH NOT ALLOWED",
        "INSTALLMENT NOT ALLOWED",
        "MAIL ORDER NOT ALLOWED",
        "SALE REFUND",                  // N_SaleRefund
        "LAST SETTLE",                  // N_LastSettle,
        "BAL INQ." ,                           // N_BalanceInq
};


//====================================================
//!===  PINPAD MESSAGE TABLE                       ===
//====================================================
const char *const EsPinPadMessages[] = {
	"                ",			//   ClrScrn
	"    APROBADA    ",			//   Approved
	"PRUEBA DE PINPAD",			//   PinTestM
	"    DECLINADA   ",			//   Declined
	" PIN INCORRECTO ",			//   IncrtPin
	"ERROR DE TARJETA",			//   PinCardErr
	"TARJETA INVALIDA",			//   UnspCard
	"TARJETA INVALIDA",			//   InvldCard
	"INTENTE OTRA VEZ",			//   TryAgain
	"READY           ",         //	 Msg_PinReady
	"PROCESSING CARD ",         //	 Msg_ProcCard
	"ENTER CARD      ",         //   Msg_EnterCard
	"TABLE IS EMPTY  "			//	 Msg_InternalTablesAreEmpty
};


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Add HDT and pinpad Spanish messages tables for DML support.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void HdtTextEs_Init( void )
{
	short pgnum;

	// Verify the number of messages match
	TableItemCount = MSG_SPECIAL;
	if ( TableItemCount != ( sizeof( SpanishMessages ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "SpanishMessages" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Verify the number of messages match
	TableItemCount = LAST_PINM_ID;
	if ( TableItemCount != ( sizeof( EsPinPadMessages ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "EsPinPadMessages" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put text table SpanishMessages into database
	TextIndexAddTable( hdttext_table_id, LNG_ES, SpanishMessages, pgnum );

	// Put text table EsPinPadMessages into database
	TextIndexAddTable( pintext_table_id, LNG_ES, EsPinPadMessages, pgnum );
}

//=============================================================================
// Private function definitions
//=============================================================================


#endif // MAKE_SPANISH
