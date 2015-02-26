#include "hwconexant.h"

/*-*
	@brief Load patch to V90 modem by ID.

	@param patchID Built-in patch ID.

	@return Zero on failure, non-zero on success.
*/
int HW_LoadPatchV90( int patchID )
{
	const char *patch;
	int patchSize;

	switch( patchID )
	{
	case Patch_V90_None:
		return 1;

#ifdef MAKE_PATCH_AU
	case Patch_V90_2C_3D_aus:
		patch = Patch_EIS_2C_3D_aus;
		patchSize = Patch_EIS_2C_3D_ausSize;
		break;
#endif

#ifdef MAKE_PATCH_HK
	case Patch_V90_2C_3D_hk:
		patch = Patch_EIS_2C_3D_hk;
		patchSize = Patch_EIS_2C_3D_hkSize;
		break;
#endif

#ifdef MAKE_PATCH_NZ
	case Patch_V90_2C_3D_NZL:
		patch = Patch_EIS_2C_3D_NZL;
		patchSize = Patch_EIS_2C_3D_NZLSize;
		break;
#endif

#ifdef MAKE_PATCH_US
	case Patch_V90_2C_3D_US:
		patch = Patch_EIS_2C_3D_US;
		patchSize = Patch_EIS_2C_3D_USSize;
		break;
#endif

	default:
		return 0;
	}

	return HW_LoadPatch( patch, patchSize );
}

/*-*
	@brief Load patch to V90 modem by ID.

	@param patchID Built-in patch ID.
	@param sp Pointer to serial port class to use during patch loading.

	@return Zero on failure, non-zero on success.
*/
int HW_LoadPatchV90CPP( int patchID, HypCSerialPort *sp )
{
	const char *patch;
	int patchSize;

	switch( patchID )
	{
	case Patch_V90_None:
		return 1;

#ifdef MAKE_PATCH_AU
	case Patch_V90_2C_3D_aus:
		patch = Patch_EIS_2C_3D_aus;
		patchSize = Patch_EIS_2C_3D_ausSize;
		break;
#endif

#ifdef MAKE_PATCH_HK
	case Patch_V90_2C_3D_hk:
		patch = Patch_EIS_2C_3D_hk;
		patchSize = Patch_EIS_2C_3D_hkSize;
		break;
#endif

#ifdef MAKE_PATCH_NZ
	case Patch_V90_2C_3D_NZL:
		patch = Patch_EIS_2C_3D_NZL;
		patchSize = Patch_EIS_2C_3D_NZLSize;
		break;
#endif

#ifdef MAKE_PATCH_US
	case Patch_V90_2C_3D_US:
		patch = Patch_EIS_2C_3D_US;
		patchSize = Patch_EIS_2C_3D_USSize;
		break;
#endif

	default:
		return 0;
	}

	return HW_LoadPatchCPP( patch, patchSize, sp );
}

/*-*
	@brief Get patch type by patch ID.

	@param patchID Built-in patch ID.

	@return Patch type of Patch_None if unknown.
*/
PatchType HW_GetPatchTypeV90( int patchID )
{
	switch( patchID )
	{
	case Patch_V90_None:
		return Patch_Normal;

#ifdef MAKE_PATCH_AU
	case Patch_V90_2C_3D_aus:
		return (PatchType)Patch_EIS_2C_3D_ausType;
#endif

#ifdef MAKE_PATCH_HK
	case Patch_V90_2C_3D_hk:
		return (PatchType)Patch_EIS_2C_3D_hkType;
#endif

#ifdef MAKE_PATCH_NZ
	case Patch_V90_2C_3D_NZL:
		return (PatchType)Patch_EIS_2C_3D_NZLType;
#endif

#ifdef MAKE_PATCH_US
	case Patch_V90_2C_3D_US:
		return (PatchType)Patch_EIS_2C_3D_USType;
#endif

	default:
		return Patch_None;
	}
}

/*-*
	@brief Load patch to V22 modem by ID.

	@param patchID Built-in patch ID.

	@return Zero on failure, non-zero on success.
*/
int HW_LoadPatchV22( int patchID )
{
	const char *patch;
	int patchSize;

	switch( patchID )
	{
	case Patch_V22_None:
		return 1;

#ifdef MAKE_PATCH_AU
	case Patch_V22_C20206_Aust:
		patch = PatchC20206_Aust;
		patchSize = PatchC20206_AustSize;
		break;
#endif

#ifdef MAKE_PATCH_HK
	case Patch_V22_C20206_HK:
		patch = PatchC20206_HK;
		patchSize = PatchC20206_HKSize;
		break;
#endif

#ifdef MAKE_PATCH_NZ
	case Patch_V22_C20206_NZ:
		patch = PatchC20206_NZ;
		patchSize = PatchC20206_NZSize;
		break;
#endif

#ifdef MAKE_PATCH_US
	case Patch_V22_C20206_US:
		patch = PatchC20206_US;
		patchSize = PatchC20206_USSize;
		break;
#endif

	default:
		return 0;
	}

	return HW_LoadPatch( patch, patchSize );
}

/*-*
	@brief Load patch to V22 modem by ID.

	@param patchID Built-in patch ID.
	@param sp Pointer to serial port class to use during patch loading.

	@return Zero on failure, non-zero on success.
*/
int HW_LoadPatchV22CPP( int patchID, HypCSerialPort *sp )
{
	const char *patch;
	int patchSize;

	switch( patchID )
	{
	case Patch_V22_None:
		return 1;

#ifdef MAKE_PATCH_AU
	case Patch_V22_C20206_Aust:
		patch = PatchC20206_Aust;
		patchSize = PatchC20206_AustSize;
		break;
#endif

#ifdef MAKE_PATCH_HK
	case Patch_V22_C20206_HK:
		patch = PatchC20206_HK;
		patchSize = PatchC20206_HKSize;
		break;
#endif

#ifdef MAKE_PATCH_NZ
	case Patch_V22_C20206_NZ:
		patch = PatchC20206_NZ;
		patchSize = PatchC20206_NZSize;
		break;
#endif

#ifdef MAKE_PATCH_US
	case Patch_V22_C20206_US:
		patch = PatchC20206_US;
		patchSize = PatchC20206_USSize;
		break;
#endif

	default:
		return 0;
	}

	return HW_LoadPatchCPP( patch, patchSize, sp );
}

/*-*
	@brief Get patch type by patch ID.

	@param patchID Built-in patch ID.

	@return Patch type of Patch_None if unknown.
*/
PatchType HW_GetPatchTypeV22( int patchID )
{
	switch( patchID )
	{
	case Patch_V22_None:
		return Patch_Normal;

#ifdef MAKE_PATCH_AU
	case Patch_V22_C20206_Aust:
		return (PatchType)PatchC20206_AustType;
#endif

#ifdef MAKE_PATCH_HK
	case Patch_V22_C20206_HK:
		return (PatchType)PatchC20206_HKType;
#endif

#ifdef MAKE_PATCH_NZ
	case Patch_V22_C20206_NZ:
		return (PatchType)PatchC20206_NZType;
#endif

#ifdef MAKE_PATCH_US
	case Patch_V22_C20206_US:
		return (PatchType)PatchC20206_USType;
#endif

	default:
		return Patch_None;
	}
}
