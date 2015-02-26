#ifndef PRTDEF_DEFINES_H
#define PRTDEF_DEFINES_H

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

#endif
