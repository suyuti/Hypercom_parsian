/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef __magdev_h
#define __magdev_h

#ifdef __cplusplus
extern "C" {
#endif

/**
	@addtogroup GroupIOCS
	@{
*/

/**
	@brief Config mode equates (IOCSMODE) for MSR devices.
*/
enum MAGCARD_IOCSMODE {
	MAGCARD_NORMAL = 10,	/**< Data is decoded according to defaults or
				     parameters set after last open. */
	MAGCARD_RAW = 11,	/**< Data is not decoded, is given to user
				     exactly as read by the hardware (not supported). */
	MAGCARD_PARAMS = 12	/**< Override the default values with given
				     table (not supported). */
};

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif // __magdev_h
