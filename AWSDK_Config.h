#pragma once

/****************************************************************************/
/**   XXX ===== XXX = = = = = = = = = = = = = = = =  = = = = = =  = = = =  **/
/**    XXX === XXX = = = = = = = = = = = = = = = = =  = = = = = = = = = =  **/
/**   = XXX = XXX = = = = = = = = =  = = = = =  = = = = = = = = = =  = =   **/
/**   == XXX XXX ========================================================  **/
/**   === XXXXX =========================================================  **/
/**   ==== XXX ======= OOOO == RRRRR II   OOOO   N == NN ================  **/
/**   === XXXXX ===== OOOOOO  RRRRRR II  OOOOOO  NN   NN ================  **/
/**   == XXX XXX ==  OO == OO RR === II OO == OO NNNN NN ================  **/
/**   = XXX = XXX == OO == OO RR === II OO == OO NN NNNN ================  **/
/**    XXX === XXX == OOOOOO  RR === II  OOOOOO  NN  NNN ================  **/
/**   XXX ===== XXX == OOOO = RR === II   OOOO   NN   NN ================  **/
/**  ====================================================================  **/
/**  ============================== SSSSS  DDDDDD = KK == KK ============  **/
/**  ============================= SSS === DDDDDDD  KK = KK =============  **/
/**  ============================= SSS === DD == DD KK  KK ==============  **/
/**  ==============================  SSSS  DD == DD KKKKK ===============  **/
/**  ================================  SSS DD == DD KK  KK ==============  **/
/**  =============================  SSSSS  DDDDDDD  KK = KK =============  **/
/**  ===== ======================  SSSSS = DDDDDD = KK == KK ============  **/
/****************************************************************************/

//****************************************************************************
// Global Configuration Controls
// The Enable macros include or exclude code that may not be of use in
// an application, and are implimented to allow smaller code.
//
// WORLD_EJECTIONS
//		Adds the world ejection features, this does not include the Eject User
//		functionality which is included as standard
#define		_XORION_ENABLE_WORLD_EJECTIONS

// UNIVERSE_EJECTIONS_MANAGEMENT
//		Adds the functions for administrating universe ejections
#define		_XORION_ENABLE_UNIVERSE_EJECTIONS_MANAGEMENT

// SERVER_MANAGEMENT
//		For special usage when creating admin tools that connect directly to
//		the world server.
#define		_XORION_ENABLE_SERVER_MANAGEMENT

// CITIZEN_MANAGEMENT
//		Enables the citizen features management on the uniserver
#define		_XORION_ENABLE_CITIZEN_MANAGEMENT

// LICENSE MANAGEMENT
//		Enables the world license management on the uniserver allowing world
//		licenses to be manipulated.
#define		_XORION_ENABLE_LICENSE_MANAGEMENT

// OBJECT_ACTIONS
//		Enables object related functions such as clicking etc
#define		XORION_ENABLE_OBJECT_ACTIONS

// EXPLICIT_MODIFIER_ACCESS
//		The access permission given to direct AWAPI functions that can
//		alter the SDKs connection state without going through setting
//		methods (example: aw_destroy, aw_create etc)
#define		XORION_EXPLICIT_MODIFIER_ACCESS		public

// ATTRIBUTE_SETTING_ACCESS
//		sets the access settings for _int_set etc from external classes
//		this will typically be public, although highly structured code will
//		use protected.
#define		XORION_ATTRIBUTE_SETTING_ACCESS		public