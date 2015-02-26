//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================

#ifndef _HWCONEXANT_H_
#define _HWCONEXANT_H_

/**
	@addtogroup GroupHW
	@{
*/

#ifdef __cplusplus

#include "HypCSerialPort.hpp"

/**
	@brief Load patch to modem (C++ API).

	This function allows to load patch to the Conexant modem connected
	to the given serial port.

	@param patch Pointer to the array with patch data.
	@param patch_len Length of the patch data.
	@param sp Pointer to serial port class to use during patch loading.

	@return Zero on failure, non-zero on success.

	@note Normally this function is used internally by HSDK. It is not
	      recommended to use it in applications code.
*/
extern int HW_LoadPatchCPP( const char *patch, unsigned int patch_len, HypCSerialPort *sp=NULL );

/**
	@brief Load patch to V90 modem by ID (C++ API).

	@param patchID Built-in patch ID.
	@param sp Pointer to serial port class to use during patch loading.

	@return Zero on failure, non-zero on success.
*/
int HW_LoadPatchV90CPP( int patchID, HypCSerialPort *sp=NULL );

/**
	@brief Load patch to V22 modem by ID (C++ API).

	@param patchID Built-in patch ID.
	@param sp Pointer to serial port class to use during patch loading.

	@return Zero on failure, non-zero on success.
*/
int HW_LoadPatchV22CPP( int patchID, HypCSerialPort *sp=NULL );

/**
	@brief Set fixed baud rate to modem (C++ API).

	This function allows to set fixed baud rate to the Conexant modem
	connected to the given serial port.

	@param sp Pointer to serial port class to use during patch loading.

	@return Zero on failure, non-zero on success.

	@note This function must not to be called before loading of patch
	      because of the bug in Conexant modem prevents loading of the
	      patch on fixed baud rate.

	@note Normally this function is used internally by HSDK. It is not
	      recommended to use it in applications code.
*/
extern int HW_SetFixedBaudRateCPP( HypCSerialPort *sp=NULL );

extern "C" {
#endif //__cplusplus

/**
	@brief Type of the patch.
*/
typedef enum patchType
{
	Patch_None = 0,         ///< No patch at all.
	Patch_Normal = 1,       ///< Normal patch that only fix bugs.
	Patch_S6 = 3            ///< Patch that changes S6 units to 0.1s.
} PatchType;

/**
	@brief V90 patch IDs.
*/
enum patchV90ID
{
	Patch_V90_None = 0,     ///< Do not use.
	Patch_V90_2C_3D_aus,	///< Patch_EIS_2C_3D_aus
	Patch_V90_2C_3D_hk,	///< Patch_EIS_2C_3D_hk
	Patch_V90_2C_3D_NZL,	///< Patch_EIS_2C_3D_NZL
	Patch_V90_2C_3D_US	///< Patch_EIS_2C_3D_US
};

/**
	@brief V22 patch IDs.
*/
enum patchV22ID
{
	Patch_V22_None = 0,     ///< Do not use.
	Patch_V22_C20206_Aust,	///< PatchC20206_Aust
	Patch_V22_C20206_HK,	///< PatchC20206_HK.c
	Patch_V22_C20206_NZ,	///< PatchC20206_NZ.c
	Patch_V22_C20206_US	///< PatchC20206_US.c
};

extern const char Patch_EIS_2C_3D_aus[];
extern const unsigned int Patch_EIS_2C_3D_ausSize;
extern const unsigned int Patch_EIS_2C_3D_ausType;

extern const char Patch_EIS_2C_3D_hk[];
extern const unsigned int Patch_EIS_2C_3D_hkSize;
extern const unsigned int Patch_EIS_2C_3D_hkType;

extern const char Patch_EIS_2C_3D_NZL[];
extern const unsigned int Patch_EIS_2C_3D_NZLSize;
extern const unsigned int Patch_EIS_2C_3D_NZLType;

extern const char Patch_EIS_2C_3D_US[];
extern const unsigned int Patch_EIS_2C_3D_USSize;
extern const unsigned int Patch_EIS_2C_3D_USType;

extern const char PatchC20206_Aust[];
extern const unsigned int PatchC20206_AustSize;
extern const unsigned int PatchC20206_AustType;

extern const char PatchC20206_HK[];
extern const unsigned int PatchC20206_HKSize;
extern const unsigned int PatchC20206_HKType;

extern const char PatchC20206_NZ[];
extern const unsigned int PatchC20206_NZSize;
extern const unsigned int PatchC20206_NZType;

extern const char PatchC20206_US[];
extern const unsigned int PatchC20206_USSize;
extern const unsigned int PatchC20206_USType;

/**
	@brief Reset Conexant modem.

	Applies reset signal to the Conexant modem.
*/
void HW_ResetConexantModem( void );

/**
	@brief Load patch to modem.

	This function allows to load patch to the Conexant modem.

	@param patch Pointer to the array with patch data.
	@param patch_len Length of the patch data.

	@return Zero on failure, non-zero on success.
*/
int HW_LoadPatch( const char *patch, unsigned int patch_len );

/**
	@brief Load patch to V90 modem by ID.

	@param patchID Built-in patch ID.

	@return Zero on failure, non-zero on success.
*/
int HW_LoadPatchV90( int patchID );

/**
	@brief Get patch type by patch ID.

	@param patchID Built-in patch ID.

	@return Patch type of Patch_None if unknown.
*/
PatchType HW_GetPatchTypeV90( int patchID );

/**
	@brief Load patch to V22 modem by ID.

	@param patchID Built-in patch ID.

	@return Zero on failure, non-zero on success.
*/
int HW_LoadPatchV22( int patchID );

/**
	@brief Get patch type by patch ID.

	@param patchID Built-in patch ID.

	@return Patch type of Patch_None if unknown.
*/
PatchType HW_GetPatchTypeV22( int patchID );

/**
	@brief Set fixed baud rate to modem.

	This function allows to set fixed baud rate to the Conexant modem.

	@return Zero on failure, non-zero on success.

	@note This function must not to be called before loading of patch
	      because of the bug in Conexant modem prevents loading of the
	      patch on fixed baud rate.
*/
int HW_SetFixedBaudRate( void );

#ifdef __cplusplus
}
#endif //__cplusplus

/**
	@}
*/

#endif //_HWCONEXANT_H_
