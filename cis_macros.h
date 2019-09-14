#pragma once

#define CIS_DEFINE_CALL_AVATAR		Avatar*	pUser	= _Request.getAvatar();

#define CIS_BEGIN_REQUIRED_ARGUMENTS(FUNCNAME)					\
	CStringA __cis_function_name = FUNCNAME;

#define CIS_REQUIRE_ARGUMENT(KEYTEXT)							\
	{															\
		if (getParamExists(KEYTEXT) == false)					\
		{														\
			CStringA logText;									\
			logText.Format("Missing argument '%s' for %s", KEYTEXT, __cis_function_name);	\
			write(logText);																	\
			return Reason::BadArgument;							\
		}														\
	}

#define CIS_END_REQUIRED_ARGUMENTS()							{}

// function header
#define CIS_BEGIN_FUNCTION(x)													\
			String cisFunctionName = x;											\
			String cisRightsRequired = "";										\
			String cisLog;														\
			std::map<CStringA, CStringA> cisLang;								\
			Avatar* pThisAvatar = getAvatar();									\
			Umbra::VariantMap cisVM = bot.CreateVariantMap(getAvatar());

#define CIS_END_FUNCTION							// nothing
#define CIS_FUNCTION(title, rights)					// nothing
					
// language inclusion
#define CIS_BEGIN_LANGUAGE
#define CIS_LANGUAGE(key, text) cisLang[key] = text;
#define CIS_END_LANGUAGE

// checking
#define CIS_BEGIN_CHECKS
#define CIS_END_CHECKS
#define CIS_TAG(x, y)