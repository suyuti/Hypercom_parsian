//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      hdtpttext.c
//
// File Description:
//      Portuguese message table
//
//=============================================================================

#ifdef MAKE_PORTUGUESE

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

//====================================================
//!===  PORTUGUESE MESSAGE TABLE                    ==
//====================================================
const char *const PortugueseMessages[] = {
	"CM",						// CustomMsg
	"CM2",						// CustomMsg2
	"CM3",						// CustomMsg3
	"CM4",						// CustomMsg4
	"CM5",						// CustomMsg5
	"CM6",						// CustomMsg6
	"CM7",						// CustomMsg7
	"CM8",						// CustomMsg8   
	"PASSE O CARTAO",			// Swipecard
	"DIGITE NUM CARTAO",		// EnterCard
	"**ERRO DE CARTAO**",		// CardErr
	"CARTAO INVALIDO",			// UnsuppCard
	"CARTAO INVALIDO",			// InvalidCard
	"CARTAO VENCIDO",			// ExpiredCard
    "PREVALID CARD",            // PrevalidCard
	"MANUAL NAO PERMITIDA",		// NoManEntry
	"DATA VENCIMENTO MMAA",		// ExpireDate
    "START DATE MMYY",         	// StartDate
    "ISSUE NUMBER",			    // IssueNum
	"PEGUE IMPRES DO CART",		// TakeImprint
	"ENTRE ULTIM.4 NUM DE",		// EntPan4
	"CARTAO CLIENTE",			// CustomerCard
	"ENTRE NUMERO CAIXA",		// CashierNum
	"ENTRADA INVALIDA",			// InvalidEntry
    "INVALID AMOUNT",           // InvalidAmount
	"VALOR",					// AmountEnt
	"CONFIRME VALOR0",			// AmountAgain
	"VALOR BASE",				// AmountBase
	"VALOR BAR",				// AmountBar
	"VALOR IMPOSTO",			// AmountTax
	"VALOR GORJETA",			// AmountTip
	"VALOR DA VENDA",			// AmountSale
	"VALOR CAIXA",				// AmountCash
	"VALOR",					// AmountTender
	"TOTAL",					// AmountTotal
	"CORRETO ?",				// CorrectYN
	"ENTRE PLANO DE CRED.",		// CreditPlan
	"SELECIONE PRODUTO",		// EntDescrip
	"NUMERO DE FATURA",			// InvoiceNum
	"ESPERANDO APROVACAO",		// WaitApprov
	"AGUARDANDO LINHA",			// LineIdle
	"CHECANDO LINHA",			// CheckingLine
	"LINHA OCUPADA ESPERE",		// LineBusy
	"ESPERE TOM DISCAGEM",		// WaitDialTone
	"DISCANDO",					// DialNow
	"ESPERANDO RESPOSTA",		// WaitForAnswer
	"ESPERANDO LINHA",			// WaitForLine
	"TREINANDO MODEM",			// TrainModem
	"PROCESSANDO",				// ProcNow
	"TRANSMITINDO",				// Transmitting
	"DISCANDO",					// NowDialing
	"INITIALIZING MODEM",		// InitModem
    "NETWORK LOGON",            // NetworkLogon
    "PAGING",                   // Paging
    "GATE CONNECTING",          // GateConnecting
    "LINKING...",               // Linking
    "CONNECTING...",            // Connecting
	"STATUS PAG. MEMORIA:",		// MemPgStatus
	"CONFIGURE TERMINAL",		// ConfigTerm
	"OU LIGUE HELP - DESK",		// CallHelp
	"INICIALIZE POR FAVOR",		// PleaseInit
	" ",						// BlankMsg
	"ERRO DE IMPRESSORA",		// PrintError
	"MEMORIA DE TRANS.",		// TransMemErr
	"AGUARDE AUTO-TESTE",		// SelfTest
	"MEMORIA INSUFICIENTE",		// NotEnoughMem
	"PARA RODAR APLICACAO",		// ToRunApplic
	"INICIALIZAR",				// InitTran
	"FUNCAO DE COMERCIO",		// MerchantFunc
	"FECHA",					// SettleTran
	"BALANCO",					// BalanceTran
	"ESTORNO",					// RefundTran
	"REIMPRIME TRANSACAO",		// ReprintTran
	"ULTIMO",					// ReprintLast
	"FATURA",					// ReprintInvoice
	"** MODO OFF-LINE **",		// OfflineTran
	"DIGITE NUMERO DOC",		// EnterInvNum
	"ENTRE NUMERO SOC",			// EnterSOCNum
	"P.O. / NUMERO I.D.",		// EnterPONum
	"FUNCAO DE COMERCIO",		// MercFunction
	"SENHA DE ACESSO",			// MaintPswd
	"SENHA INCORRETA",			// PswdNotMatch
	"CORRETO ?",				// CorrectYesNo
	"FUNCAO DO LOGISTA",		// MerchProg
	"DIGITE SENHA",				// EnterPswd
	"NUMERO ID. TERMINAL",		// EnterTSer
	"NUMERO TEL. INICIAL.",		// EnterInitTel
	"NUMERO TEL. NMS",			// EnterNMSTel
	"MODO DISCADO",				// EnterDialMode
    "SIGNATURE",             	// Signature
	"POR FAVOR ASSINE",			// SignaturePlease
	"E PRESSIONE ENTER",		// PressEnter
	"ASSINE NOVAMENTE?",		// RetrySigCap
	"CODIGO ACESSO PABX",		// EnterPABX
	"SAIR, TECLE 'CANCEL'",		// ExitPressCncl
	"DUPLICAR TRANS.",			// DuplicateTrans
	"ADICIONAR?",				// AddYesNo
	"SEM DOC P/ IMPRIMIR",		// NoInvoice
	"ESPERE POR FAVOR",			// PleaseWait
	"TRANSACAO COMPLETA",		// TransComplete
	"TRANSACAO ACEITA",			// TransAccepted
	"CARGA DE PROGRAMA",		// ProgamLoad
	"SENHA DE FUNCAO 99",		// Fun99Password
	"FUNCAO DE PROGRAMA",		// ProgFunction
	"SELEC. OPC. LOCAL",		// LocalOptSel
	"SELEC. CONFIG. LOCAL",		// LocalConf
	"NUMERO MAX. EXCEDIDO",		// ExceedMax
	"MENSAGENS DISCAGEM",		// EnhDial
	"0-LL 1-DIAL 2,3-LAN",		// LDLPmt
	"ENDERECO CU",				// CUAddrP
	"ATUAL",					// Currently
	"ADICIONAR/ELIMINAR",		// SignOnOff
	"ENTRE NUMERO",				// TillNum
	"ADICIONAR?",				// SignOnYN
	"SENHA USADA INVALIDA",		// VoidPassM
	"SENHA DEVOLUCAO",			// RefPassM
	"SENHA DE AJUSTE",			// AdjPassM
	"SENHA DE REPORTE",			// RepPassM
	"PERMITE DISCAGEM TEL",		// KbdDialM
	"PERMITE LARGA DIST.",		// TollDialM
	"MEMORIA DISC RAPIDA",		// MemDialM
	"DISCAGEM REFERENCIA",		// RefDialM
	"BLOQUEAR TERMINAL ?",		// KeyLock
	"ENTRE NUM. DA CONTA",		// EnterAcct
	"DIGITE SENHA ANTIGA",		// OldPassword
	"DIGITE NOVA SENHA",		// NewPassword
	"NOVA SENHA NOVAMENTE",		// NewPassAgn
	"SEM IMPRESSORA",			// NoPrinter
	"REIMPRESSAO COMPLETA",		// ReprintComplete
	"NUM NAO ENCONTRADO",		// InvNumberMissing
	"IMPRIMINDO",				// PrintingWait
	"CAMAREIRO REGISTRADO",		// ServerOn
	"CAIXAS REGISTRADOS",		// CashierOn
	"TESTE DE TECLADO",			// KeyBrdTest
	"CANCEL 2X PARA SAIR",		// Cancel2End
	"TECLA =  ",				// DispKeyP
	"FECHAMENTO REQUERIDO",		// SettlementRequired
	"LEITURA DE CARTAO",		// CardRdT
	"IMPRESSORA ATIVADA",		// PrinterEP
	"IMPRESSORA DESATIV.",		// PrinterDP
	"ATIVAR  ?",				// EnablePP
	"DESATIVAR ?",				// DisablePP
	"PROCURANDO LOTE",			// ScanningBatch
	"NAO HA TOTAIS LOTE",		// NoBatchTotals
	"NAO HA MAIS TOTAIS",		// NoMoreTotals
	"TESTE DE IMPRESSAO",		// TestPrnC
	"BLOQUEADO",				// Closed
	"DIGITE DATA MMDDAA",		// DateMDY
	"DIGITE HORA HHMM",			// TimeHM
	"LOTE ATUAL DO HOST",		// HostBatch
	"NUM. DO ACQUIRER",			// NoAcq
	"INICIO DO LOTE:",			// StartOfBatch
	"FIM DO LOTE:",				// EndOfBatch
	"SELECIONE DESLOCAM.",		// SelectShiftNum
	"0 P/ DESLOCAR TODOS",		// ZeroForAllShift
	"GAVETA",					// ReportMenuTill
	"CAIXA",					// ReportMenuCashier
	"CAMAREIRO",				// ReportMenuServer
	"REVISAO",					// ReportMenuAudit
	"SUMARIO",					// ReportMenuSummary
	"0 TABS",					// ReportMenuOpenTabs
	"1=DETALHE 3=SEM AJUS",		// CashServMenuLine1
	"2=SUMARIO",				// CashServMenuLine2
	"ESTORNO NAO PERMIT.",		// VoidNotAllowed
	"DIGITE NUM. DO HOST",		// HostNum
	"TIPO ?",					// Type
	"AJUSTE NAO PERMITIDO",		// AdjustNotAllowed
	"RETRANSMITA O LOTE",		// RetryBatchTrans
	"TOTAL",					// AmountTotals
	"NOVO VALOR?",				// AmountNew
	"CONFIG TEXTO RECIBO",		// RecieptTXT4
	"DIGITE COD. AUTORIZ",		// EnterApp
	"**MODO TREINAMENTO**",		// TrainingMode
	"IR P/ MODO TREINO?",		// GoToTrainingMode
	"SAIR DO MODO TREINO?",		// LeaveTrainingMode
	"DEVOLUCAO NAO PERMIT",		// RefundNotAllowed
	"TOTAL VENDAS",				// SalesTotal
	"TOTAL VENDAS ERRADO",		// SalesTotalsWrong
	"TOTAL DEVOL",				// RefundTotal
	"TOTAL DEVOL ERRADO",		// RefundTotalsWrong
	"TOTAIS",					// BatchTotals
	"LIBERACAO",				// BatchRelease
	"TESTE DE TRANSACOES",		// TestTransactions
	"NAO HA TRANSACOES",		// NoTransactions
	"INCREM",					// IncrementShift
	"RESETAR",					// ResetShift
	"SEM COMUNICACAO",			// NoComms
    "NO PHONE LINE",            // NoPhoneline
	"DIGITE GORGETA",			// EnterTip
	"DIGITE TOTAL",				// EnterTotal
	"1-ABR. TAB 3-TAB RPT",		// TabProcMsg1
	"2-FECHAR TAB",				// TabProcMsg2
	"DIGITE NUM CAMAREIRO",		// ServerNum
	"FECHE TODAS TABS ABERTAS",	// CloseOpenTabs
	"NAO HA TAB ABERTO",		// NotOpenTab
	"CONFIGURE",				// N_Configure
	"INIC",						// N_Initialize
	"PRINCIPAL",				// N_Main
	"TRANS",					// N_NewTrans
    "REFUND",                   // N_Refund
	"LOTE",						// N_Batch
	"RELAT",					// N_Reports
	"TERM",						// N_Terminal
	"MAIS",						// N_More
	"MENU",						// N_Menu
	"DAT/HOR",					// N_Datime
	"IMPRESS",					// N_Printer
	"COMUNIC",					// N_Comms
	"DISC",						// N_Dial
	"SENHAS",					// N_Passwords
	"V.DISC",					// N_Speedial
	"CARTAO",					// N_CardRdr
	"CONTR",					// N_Contrast
	"VENDA",					// N_Sale
	"SALE+CASH",				// N_SalCash
	"V.POST",					// N_MailOrder
	"OFFLINE",					// N_Force
	"AUTORIZ",					// N_Auth
	"V.CART",					// N_CardVer
	"REVISAR",					// N_Review
	"BUSCAR",					// N_Find
	"AJUSTAR",					// N_Adjust
	"ANULAR",					// N_Void
	"CASH",						// N_Cash
	"CARGA",					// N_ProgLoad
	"DESLOC",					// N_Shift
	"VER",						// N_View
	"TREINA",					// N_TrainMode
	"CONFIG",					// N_Setup
	"FUNCAO",					// N_Function
	"TESTES",					// N_Tests
	"TAB",						// N_Tab
	"REIMPR",					// N_Reprint
    "EXIT",                     // N_Exit
	"ERRO",						// N_Error
	"LIMPAR",					// N_Clear
	"LIMPAR TAB DO CAIXA",		// N_ClearChsTbl
	"LIMPAR TAB CAMAREIRO",		// N_ClearSerTbl
	"ENTRAR",					// N_Enter
	"CANCELAR",					// N_Cancel
	"RESETAR",					// N_Reset
	"QUICK",					// N_Quick
	"ADI/ELI",					// N_AddRmv
	"DETALHE",					// N_Detail
	"RESUMO",					// N_Summary
	"SEM AJU",					// N_Unadjusted
	"HOST",						// N_Host
	"DISPLAY",					// N_Display
	"TECLADO",					// N_Keyboard
	"PINPAD",					// N_Pinpad
	"T.INIC",					// N_InitPhone
	"T.CARGA",					// N_NMSPhone
	"T.DISC",					// N_DialType
	"PABX",						// N_PABX
	"ID TERM",					// N_TermId
	"TODAS",					// N_All
	"LIG/DES",					// N_OnOff
	"TEXTO",					// N_Receipt
	"RECIBO",					// N_Text
	"PUXA PAPEL",				// N_PaperFeed
	"SAIR",						// N_Quit
	"MUDAR",					// N_Change
	" ",						// N_NullStr
	"COMUNICACOES",				// N_Communications
	"MUDAR SENHA",				// N_PswdChange
	"AUMENTAR",					// N_Higher
	"DIMINUIR",					// N_Lower
	"OPCOES LOCAIS",			// N_LocalOpts
	"RESPOSTA",					// N_Response
	"ABRIR",					// N_Open
	"FECHAR",					// N_Close
	"IMPRIMIR",					// N_Print
	"MAIS",						// N_Scroll
	"DIGITE FUNCAO",			// N_EnterFunction
	"IMP HOST",					// N_PrintHost
	"RPT BD",					// N_DBRpt
	"RPTTXRX",					// N_TxRxRpt
	"VER TABELA",				// N_ViewTable
	"TESTE DE DISPLAY",			// N_DisplayTest
	"FUNCAO NAO PERMITIDA",		// N_FuncNotSupported
	"VERSAO",					// N_Version
	"ULTIMA RESPOSTA",			// N_LastRsp
	"ULTIMA",					// N_Last
	"SEM DADOS",				// N_NoData
	"INFO LOTE",				// N_BatchInfo
	"ID HOST: ",				// N_HostId
	"NOME DO HOST: ",			// N_HostName
	"NUM DO LOTE: ",			// N_BatchNumber
	"0 PARA HOST",				// N_ZeroForAllHost
	"RELAT",					// N_Report
	"DEBITO",					// N_Debit
	"CREDITO",					// N_Credit
	"ENTRE CODIGO AUT",			// N_EnterAuth
	"NUM DE GENERACAO",			// N_EnterGenNum
	"TESTE DE JOURNAL",			// JournalTesting
	"ENTRE No DE ENTRADAS",		// NumEntries
	"SEM PAPEL",				// PaperOut
    "PRINTER OVERHEAT",         // PrinterOverheat - NEED TO BE TRANSLATED !!
	"ESPERANDO NUM CONTA",		// WaitingForAcct
	"ESPERANDO APROVACAO",		// WaitingForApproval
	"ESPERANDO POR PIN",		// WaitingForPin
	"PASSE CARTAO PINPAD",		// SwipeCardPin
	"LEITURA POR PIN?",			// ChangeToPinRead
	"LEITURA POR TERM?",		// ChangeToTermRead
	"CONTA INVALIDA",			// InvalidAccount
	"SALDO DISPONIVEL",			// AvailBal
	"       HOJE",				// Today
	"CARTAO NAO EXISTE",		// CardNoMatch
	"TESTE DE PINPAD",			// PinPadTest
	"DETECTANDO PINPAD",		// PinPadAutoDetect
	"SELEC. CONTA",				// SelectAccount
	"CONTA",					// N_Account
	"0",						// N_0
	"1",						// N_1
	"2",						// N_2
	"3",						// N_3
	"4",						// N_4
	"DIGITE PIN",				// EnterPin
	"SEM PINPAD",				// ExtPinNot
	"ERRO DE CRIPTOGRAFIA",		// EncryptError
	"CONEXAO",					// N_Logon
	"MANUTENCAO",				// MaintFunction
	"ENTRE RRN ORIGINAL",		// N_OrigDate
	"IDENTIFICACAO",			// N_Identification
	"SOFTWARE",					// N_Software
	"DLL",						// N_DLL
	"BOOTER",					// N_EPROM
	"REFER",					// N_Referral
	"TODOS",					// N_AllHosts
	"ICEPAC",					// N_IcePac
	"PRES. ENTRA P/TODOS",		// PressEnterForAll
	"ENTRA PARA ULT. DOC.",		// EnterForLastInv
	"TEMPO EXCEDIDO",			// N_Timeout
	"USUARIO CANCELADO",		// N_UserCancelled
	"DESFAZ",					// N_Reversals
	"PINPAD NAO ENCONTRADO",	// PinpadNotFound
	"SIM",						// N_YES
	"NAO",						// N_NO
	"OK",						// N_OK
	"SAIR",						// N_QUIT
	"PROXIMO",					// N_NEXT
	"SWIPE OR INSERT CARD",     // N_SwipeOrInsertCard
	"X",						// N_X
	"CONTINUE",					// N_Continue
	"TECLA",					// N_KEY
	"PASSOU",					// N_Passed
	"FALHOU",					// N_Failed
	"TESTE",					// N_Test
	"TOM",						// N_Tone
	"PULSO",					// N_Pulse
	"ERRO DE LEITURA - PINPAD",	// PinReadErr
	"ERRO DE ESCRITA - PINPAD",	// PinWriteErr
	"ELIMINAR?",				// SignOffYN
	"JAN",						// Month_1
	"FEV",						// Month_2
	"MAR",						// Month_3
	"ABR",						// Month_4
	"MAI",						// Month_5
	"JUN",						// Month_6
	"JUL",						// Month_7
	"AGO",						// Month_8
	"SET",						// Month_9
	"OUT",						// Month_10
	"NOV",						// Month_11
	"DEZ",						// Month_12
	"RE-ENTRE SENHA",			// Msg_Reenter_Pin
	"SENHA?",					// Msg_Pin
	"EXTERNO:",					// Msg_External
	"INTERNO:",					// Msg_Internal
	"CKSUM:",					// Msg_Chksum
	"CORRETO?",					// Msg_Correct
	"SVR:",						// Msg_SVR
	"CSH:",						// Msg_CSH
	"INV:",						// Msg_INV
	"DESFAZIMENTO",				// Msg_reversal
	"CANCELADO",				// Msg_voided
	"advice",					// Msg_advice
	"NUM SEQUENCIA",			// Msg_SequenceNo
	"ITENS",					// Msg_Items
	"VENDAS",					// Msg_Sales
	"ERRO **********",			// Msg_Error
	"HOSTS DISPONIVEIS",		// Msg_AvailHosts
	"DINHERO: ",				// Msg_Cash
	"TENDER : ",				// Msg_Tender
	"VALOR OPCIONAL ",			// AmountOpt
	"CVV2 INDICADOR",			// N_CVV2Indicator
	"0=SALTE",					// N_CVV2IndEntry1
	"1=PRESENTE",				// N_CVV2IndEntry2
	"2=ILEGIVEL",				// N_CVV2IndEntry3
	"9=AUSENTE",				// N_CVV2IndEntry4
	"CVV2 DO CARTAO",			// N_CVV2Entry
	"APRESENTA CARTAO ?",		// Msg_CardPresent
	"CABEC 4",					// Msg_HeaderLine
	"RODAPE ",					// Msg_FooterLine
	"PASSE SEU CARTAO",			// Msg_SwipeYourCard
	"ESCOLHA OPCAO GORJETA",	// Msg_PreTip
	"OUTROS",					// Msg_Other
	"ERRO PROGRAMA:",			// Msg_ProgramError
	"GORJETA",					// Msg_Tips
	"GORJ %",					// Msg_TipPct
	"% GORJETA SUGERIDA",		// Msg_Sug_Tip
	"OPCAO LOCAL",				// Msg_LocalOpt
	"ERRO TX/RX",				// Msg_TxRxErrors
	"ERRO TRANSMISSAO:",		// Msg_TranErrors
	"ERRO RECEPCAO:",			// Msg_RecvErrors
	"DOWNLOAD DE ARQUIVO",		// N_FileDownLoad
	"FECHAMENTO",				// N_Settle
	"OFFLINE",					// N_Offline
	"TRANS",					// N_Trans
	"CONFIG DATA/HORA",			// N_DateTimeSetup
	"CONTRASTE",				// N_Contrast
	"ALIM.PAPEL",				// N_PaperFd
	"FECHAMENTO STATUS",		// N_SettleStat
	"ESCOLHA OPCAO SUMARIO",	// Msg_ChooseSummary
	"AUTO   ",					// Msg_Auto
	"GRUPO   ",					// Msg_Group
	"MANUAL   ",				// Msg_Manual
	"NUNCA",					// Msg_Never
	"ITEMS VENDA",				// Msg_SaleItems
	"ITEMS ESTORNADOS",			// Msg_RefundItems
	"NUM.TENTATIVA ",			// Msg_Attempt
	"RESP ",					// Msg_Resp
	"TUDO",						// Msg_Evrythg
	"TUDO",						// Msg_Everything
	"TABELA SERVICO VAZIA",		// Msg_SrvTabEmpty
	"TABELA CAIXA VAZIA",		// Msg_CshTabEmpty
	"STATUS",					// Msg_Status
	"fopen error: ",			// Msg_fopenerror
	"Down loading file: ",		// Msg_DownloadingFile
	"ESCOLHA A LINGUA",			// Msg_SelectLanguage
	"LINGUA",					// Msg_Language
	"ENGLISH",					// Lng_English
	"GERMAN",					// Lng_German
	"SWEDISH",					// Lng_Swedish
	"ESPANOL",					// Lng_Spanish
	"PORTUGUES",				// Lng_Portugues
	"ACEITAR LINGUA ATUAL",		// Msg_CurrentLang
	"LINGUA",					// Msg_Lang
	"KEY TABLE INVALID",		// Msg_KeyTableInvalid
	"RELATORIO DO BANCO DE DADOS\n",	// Prt_DatabaseReport
	"---------------------------\n",	// Prt_Dashes
	"TERMINAL - CONFIG DA EPROM\n",	// Prt_TermEpromCfg
	"CONFIGURACAO DO TERMINAL\n",	// Prt_TermConfig
	"ACQUIRER ID:",				// Prt_AcquirerID
	"ISSUER ID:",				// Prt_IssuerID
	"CARD RANGE ID:",			// Prt_CardRangeID
	"DESCRIPTOR ID:",			// Prt_DescriptorID
	"DADOS RECEBIDOS:\n",		// Prt_ReceivedData
	"DADOS ENVIADOS:\n",		// Prt_SendData
	"RAW DATA (HEX BYTES):\n",	// Prt_RawDataHexBytes
	"Waiting for LAN cable",	// WaitingForLanCable
	"Line mode canceled",		// LineModeCanceled
	"Restart terminal",			// RestartTerminal
	"ONLY 0-9 A-F VALID",		// N_Only09AFvalid
	"FUNCAO PARA BOTOES",		// N_IdleButton
	"SELECIONE BOTAO",			// SelectButton 
	"SEL. TIPO DE EVENTO",		// EventType
	"BOTAO ",					// N_Button
	"TRANSACAO PERMITIDA ",		// ConfirmTrans
	"FUNCAO PERMITIDA",			// ConfirmFunc
	"INIC. TABELA BOTOES",		// N_InitButTab 
	"INIC. DESABILITADO",		// Init_Dis
	"INIC. HABILITADO",			// Init_Ena
	"BTS IDL",					// N_IdlBtn
	"INIC. BTS",				// N_InitIdlBtn
	"MODIF. BTS",				// N_ChgIdlBtn
	"EVENTO P/ BOTAO 1",		// Chang_Btn1
	"EVENTO P/ BOTAO 2",		// Chang_Btn2
	"EVENTO P/ BOTAO 3",		// Chang_Btn3
	"EVENTO P/ BOTAO 4",		// Chang_Btn4
	"BOTAO 1",					// B_1
	"BOTAO 2",					// B_2
	"BOTAO 3",					// B_3
	"BOTAO 4",					// B_4  
	"REMOVE MERCHANT COPY",     // RemoveMerchantCopy
	"NO HAY TABLA P/IMPRIMIR",	// PclTabError
	"RENDIM",					// Prt_PerformBtn 
	"CONFIGU",					// Prt_ConfBtn    
	"REPORTE DE CONFIG.  ",		// Config_RPT    
	"TERMINAL",					// Terminal_Tab 
	"EMISOR",					// Issuer_Tab 
	"ADQUIRIENTE",				// Acq_Tab 
	"CARTAO",					// Card_Tab
	"TODO",						// All_Tab
	"LIBS VERSION",				// N_LibsVersion
	"SDK",						// N_SDKVersion
	"O/S VER",					// N_OSVersion
	"O/S REV",					// N_OSRevision
    "HFC",                      // N_HFCVersion
	"HISTORY REPORT",			// History_RPT
	"fread error: ",			// Msg_freaderror
	"Entry # Exceeds Max",		// N_EntryNumExceedsMax
	"Table ID: ",				// N_TableId
	"HFS",						// N_HFS
	"No enougth space in HFS",	// N_NoEnougthSpaceInHFS
	"SUPERVISOR PASSWORD",      // SVPassword
	"CALL AUTH CENTRE",         // CallAuthCentre
	"AUTH CODE ?",              // AuthCode
    "CASH NOT ALLOWED",         // N_CashNotAllowed
	"SWIPE NOT ALLOWED",        // SwipeNotAllowed
	"TRANS. NOT ALLOWED",       // TranNotAllowed
    "INIT MAX TRIES",			// InitMaxTries
	"Up",						// N_Up
	"Down",						// N_Down
	"Sel",						// N_Select
	"Repr",						// N_ReprintShort
	"HEAD",						// Msg_HeaderLine
	"Vertic",					// N_Veritcal
	"Horiz"	,					// N_Horizontal
	"LINE",                     // N_Modem
	"COM1",                     // N_RS232RS0
	"COM2",                     // N_RS232RS1
	"COMMS: CU ADDRESS",        // N_RS232   
	"COMMS CONFIG",             // N_RS232Config
	"COMMS: INTERFACE",         // N_EnableComm 
	"COMMS: PROTOCOL",          // N_RS232Prot	
	"COMMS: BIT RATE, bps",     // N_RS232Baud  
	"1200",                     // N_Baud1200   
	"2400",                     // N_Baud2400   
	"4800",                     // N_Baud4800   
	"9600",                     // N_Baud9600   
	"19200",                    // N_Baud19200  
	"38400",                    // N_Baud38400  
	"57600",                    // N_Baud57600  
	"115200",                   // N_Baud115200 
	"COMMS: STOP BITs",         // N_RS232Stop  
	"1  bit",                   // N_RS232Stop1 
	"2 bits",                   // N_RS232Stop2 
	"COMMS: DATA BITs",         // N_RS232Data  
	"7 bits",                   // N_RS232Data7 
	"8 bits",                   // N_RS232Data8 
	"COMMS: PARITY",            // N_RS232Parity
	"None",                     // N_RS232None  
	"Odd",                      // N_RS232Odd   
	"Even",                     // N_RS232Even  
	"CHAR MAP",                 // N_CharMap	
    "DIALING MESSAGES",         // N_DialMsgs
	"COMMS: FLOW CONTROL",      // N_RS232FlowC
    "RX",                       // N_RX
    "RX / TX",                  // N_RXTX
    "COMMS: MODEM MODE",        // N_CommsModemMode
    "COMMS: DB MODEM MODE",     // N_CommsDBModemMode
    "COMMS CHANGED",            // N_CommsChanged
    "RESTART TERMINAL",         // N_RestartTerminal
    "PRESS CANCEL TO EXIT",     // MsgPressCancelToExit
    "CHANGE APP",               // N_ChangeApp
    "SELECT APP",               // N_SelectApplication
    "TRACE LOG",                // N_TraceLog
    "START NEW",                // N_StartNew
    "SEND VIA COM1",            // N_Send1
    "SEND VIA COM2",            // N_Send
    "STOP",                     // N_Stop
    "TRACE IS EMPTY",           // N_TraceIsEmpty,
    "*** TRACING ***",          // Tracing
    "** TRACE OVERFLOW **",     // TraceOverflow
	"GSM",                      // N_GSM
    "ETHERNET",                 // N_Ethernet
    "LAN",                      // N_LAN
    "BLUETOOTH",                // N_Bluetooth
    "ISDN",                     // N_ISDN
    "DIAL BACKUP",              // N_DialBackUp    
	"DB INIT TELEPHONE NO.",		// EnterDBInitTel
	"DB NMS TELEPHONE NO.",		// EnterDBNMSTel
	"INIT. PREF. SERVICE",		// InitPrefService
	"NMS PREFERRED SERVICE",      	// NMSPrefService
	"LOW SIGNAL",			// LowSignal
	"INIT PREF",     			// N_DBInitPref
	"NMS PREF",				// N_DBNMSPref
	"DB INIT",     				// N_DBInitPhone
	"DB NMS",				// N_DBNMSPhone
	"GSM with DB",    			// N_GSMwDial
	"DB with GSM",			// N_DialwGSM
	"GSM only",     			// N_GSMonly
	"DB only",			// N_Dialonly
	"",					// StartGSMModem
	"",					//ProcWasCanel
	"",					// NoSIM	
	"",					// ForbSIMStatus
	"",					// Msg_InsertYourCard
    "LAST IP ADDRESS",          // N_LastIPaddr
    "BATTERY LOW",              // BatteryLow
    "UNLOCKING ERROR",          // UnlockingError - NEED TO BE TRANSLATED


#ifdef	MAKE_EMV
	"EMV INFO ENABLED",         // EmvInfoEna
	"EMV INFO DISABLED",        // EmvInfoDis
	" DISABLE EMV INFO ?",      // DisEmvInfo
	"  ENABLE EMV INFO ?",      // EnaEmvInfo
	"PROCESSING CARD",			// ProcCard
	"SMART CARD",				// SmartCard
	"PLEASE REMOVE CARD",		// PlsRemoveCared
	"CARD DECLINED",			// CardDeclined
	"PIN FAILED",				// PINFailed
	"SWIPE/INSERT CARD",		// SwipeInsert
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
	" DISABLE SMART INIT?",      // DisSmartInit 
	" ENABLE SMART INIT ?",      // EnaSmartInit
#endif // MAKE_SMARTINIT
#ifdef MAKE_FARSI
    "PERSIAN", // Lng_Farsi						
#endif
    "SET COM PARAM",        // N_SetComParam,
    "HOST INFO TYPE",    // N_HostInfoType,
    "SET PARAM.",        // N_SetParam,
    "COM TYPE",          // N_ComType,
    "HOST INFO",         // N_HostInfo,
    "TRANSACTION HOST",  // N_TransactionHost,
    "SETTLEMENT HOST",   // N_SettlementHost,
    "PRIMARY",           // N_Primary,
    "SECONDARY",         // N_Secondary,
    "PHONE NUMBER",      // N_PhoneNumber,
    "CONNECT TIME",      // N_ConnectTime,
    "DIAL ATTEMPT",      // N_DialAttemp,
    "MODEM MODE",        // N_ModemMode,
    "IPADDRESS",         // N_IpAddress,
    "PPP ENABLE",        // N_PPPEnable,
    "PPP Mode",          // N_PPPMode
    "ENTER PHONE NUMBER",// EnterPhoneNumber
    "ENTER CONNECT TIME",// EnterConnectTime
    "ENTER DIAL ATTEMP ", // EnterDialAttemp
    "ENTER PPP MODE",      // EnterPPPMode
    "ENTER MODEM MODE",    // EnterModemMode, 
    "ENTER IPADDRESS",     // EnterIPAddress, 
    "PPP ENABLED?",         // PPPEnabled,
    "USE * IN ORDER TO .",          // UsePoint, 
    "CURRENT IP:",         // CurrentIP,
    "NII",                       //     N_NII,
    "PORT NUMBER", //N_PortNumber,
    "HEX LEN",      //N_HexLenEnable,
    "POS TPDU",    // N_PosTPDUEnable,
    "ENTER NII",    // EnterNII
    "PPP is ENABLED", // PPP_Ena
    "PPP is DISABLED",// PPP_Dis
    "ENTER IP PORT",              // EnterIPPort
    "HEX LEN IN HEADER is ENABLED", //HexLen_Ena,
    "HEX LEN IN HEADER is DISABLED",//HexLen_Dis,
    "POS TPDU IN HEADER is ENABLED",//PosTPDU_Ena,
    "POS TPDU IN HEADER is DISABLE",//PosTPDU_Dis,
    "IP",   //N_IP,
    "GPRS", //N_GPRS,
    "HLAN", //N_HLAN,
    "DIAL BACKUP", //N_DIALBACKUP,
    "DIAL UP",  //N_ONLYDIALUP,
    "IP is ENABLED", //IP_Ena,
    "IP is DISABLED",//IP_Dis,
    "GPRS is ENABLED", //GPRS_Ena,
    "GPRS is DISABLED",//GPRS_Dis,
    "HLAN is ENABLED", //HLAN_Ena,
    "HLAN is DISABLED", //HLAN_Dis,
    "DIAL BACKUP is ENABLED", //DBackup_Ena,
    "DIAL BACKUP is DISABLED", //DBackup_Dis,
    "ONLY DIAL UP is ENABLED", //Dial_Ena,
    "ONLY DIAL UP is DISABLED", //Dial_Dis,
    "COM PARAMETER", //N_ComParameter

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
#ifdef MAKE_KIB 
      "N_LoyaltyInquiry",
      "N_LoyaltyList",
      "N_SaleWithLoyalty", 
      "LoyaltyAmountEnt",
      "Msg_NotPermitLoyIns",
      "N_InstallmentWithLoyalty",
      "N_Discount", 
      "N_DiscountSale", 
      "N_DiscountInst", 
      "N_RateEntry",
      "Msg_OverDiscount",
      "N_Point",
      "N_Disc",            
#endif

};

//====================================================
//!===  PORTUGUESE PINPAD MESSAGE TABLE             ==
//====================================================
const char *const PtPinPadMessages[] = {
	"               ",			//   ClrScrn
	"    APROVADA   ",			//   Approved
	"TESTE DE PINPAD",			//   PinTestM
	"    DECLINADA  ",			//   Declined
	" PIN INCORRETO ",			//   IncrtPin
	"ERRO DE CARTAO ",			//   PinCardErr
	"CARTAO INVALIDO",			//   UnspCard
	"CARTAO INVALIDO",			//   InvldCard
	"TENTE OUTRA VEZ",			//   TryAgain
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
//!      Add HDT and pinpad Portuguese messages tables for DML support.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void HdtTextPt_Init( void )
{
	short pgnum;

	// Verify the number of messages match
	TableItemCount = MSG_SPECIAL;
	if ( TableItemCount !=
		 ( sizeof( PortugueseMessages ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "SpanishMessages" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Verify the number of messages match
	TableItemCount = LAST_PINM_ID;
	if ( TableItemCount != ( sizeof( PtPinPadMessages ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "EsPinPadMessages" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put text table PortugueseMessages into database
	TextIndexAddTable( hdttext_table_id, LNG_PT, PortugueseMessages, pgnum );

	// Put text table PtPinPadMessages into database
	TextIndexAddTable( pintext_table_id, LNG_PT, PtPinPadMessages, pgnum );
}


//=============================================================================
// Private function definitions
//=============================================================================



#endif // MAKE_PORTUGUESE
