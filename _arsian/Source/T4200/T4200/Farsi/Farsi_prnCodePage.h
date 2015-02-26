#ifdef MAKE_FARSI

//=============================================================================
//                           Altius-It
//                      (c) Copyright 2007
//=============================================================================
//
// Module overview:             Farsi_prnCodePage.h
//      Support Farsi Code page to convert ASCII code to local character set
//=============================================================================


// Legend of fpi_options bitmap
#define o_LF  0x0001    // Line Feed requires
#define o_NP  0x0002    // Non printable character
#define o_HG  0x0004    // Homogeneous character: 
#define o_FN  0x0008    // Font selector
#define o_ST  0x0010    // Style selector
#define o_FS  0x0020    // Farsi character - do use ligature style
#define o_BK  0x0040    // Bracket - need to be replaced another bracket
#define o_NB  0x0080    // This char may be the part of whole number
#define o_FF  0x0100    // Form feed
#define o_DJ  0x0200    // Disloint Farsi character - break the ligature

// Legend of fpi_command bitmap when fpi_options = o_ST
#define REV_OFF 0x0001	// Reverse off
#define REV_ON	0x8001	// Reverse on
#define UND_OFF 0x0002	// Underline off
#define UND_ON  0x8002	// Underline on
#define BLD_OFF	0x0004	// Bold off
#define BLD_ON	0x8004	// Bold on
#define ITL_OFF	0x0008	// Italic off
#define ITL_ON	0x8008	// Italic on


#define BAR_ON	0x0010  // Barcode on
#define BAR_ST  0x0020	// Start barcode sequence
#define BAR_END	0x8030	// End barcode sequence



typedef struct
{
    const UBYTE           ASCII;        // ASCII representation
    const CharDefinition* fpi_aln_lft;  // Image of left part,  allone caracter 
    const CharDefinition* fpi_aln_rgt;  // Image of right part, allone caracter
    const CharDefinition* fpi_itl_lft;  // Image of left part,  initial caracter 
    const CharDefinition* fpi_itl_rgt;  // Image of right part, initial caracter
    const CharDefinition* fpi_mdl_lft;  // Image of left part,  middle caracter 
    const CharDefinition* fpi_mdl_rgt;  // Image of right part, middle caracter
    const CharDefinition* fpi_end_lft;  // Image of left part,  end caracter 
    const CharDefinition* fpi_end_rgt;  // Image of right part, end caracter
    const unsigned int    fpi_command;  // Command of Image (additional processing)
    const unsigned int    fpi_options;  // Options of Image
}
__attribute__((packed)) Farsi_prnCodePage_record;


extern Farsi_prnCodePage_record Farsi_prnCodePage_table[256];


#endif // MAKE_FARSI
