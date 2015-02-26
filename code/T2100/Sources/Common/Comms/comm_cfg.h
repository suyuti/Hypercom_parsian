
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             comm_cfg.h
//      setup configuration for comm process
//
//=============================================================================

extern void CommConfig ();
extern void CommPermEthernetConfig(void);
extern void CommPermGSMConfig(void);
extern struct ipcontab *FindIPCById( UBYTE id );
