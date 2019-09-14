#include "stdafx.h"
#include "Helpers_Time.h"
#include "Helpers_Xml.h"
#include "Helpers_Text.h"
#include "Validators.h"

namespace Helpers
{
	namespace Parsing
	{
		CStringA ExtractBrackets(CStringA _Identify)  {
			int start_br = _Identify.Find("(");
			int end_br   = _Identify.Find(")");

			// check that both have been found
			if (start_br == -1 || end_br == -1)	return "";
			if (start_br > end_br)				return "";

			// extract the middle string
			return _Identify.Mid(start_br + 1, end_br - start_br - 1);
		}
	}

	namespace Time
	{
		// split a structure up into its parts
		Parts SecondsToParts(int _Seconds) {
			Parts	parts;
			parts.tm_seconds	= _Seconds % SECONDS_IN_MINUTE;
			parts.tm_minutes	= (_Seconds / SECONDS_IN_MINUTE) % SECONDS_IN_MINUTE;
			parts.tm_hours		= (_Seconds / SECONDS_IN_HOUR) % HOURS_IN_DAY;
			parts.tm_days		= (_Seconds / (SECONDS_IN_HOUR * HOURS_IN_DAY));
			return parts;
		}

		// displays seconds in format
		CStringA SecondsToTimeStr(time_t _Seconds) {
			Parts parts = SecondsToParts((int)_Seconds);
			CStringA		argStr;
			if (parts.tm_days)	argStr.Format("%dd %0.2d:%0.2d:%0.2d", parts.tm_days, parts.tm_hours, parts.tm_minutes, parts.tm_seconds);
			else				argStr.Format("%0.2d:%0.2d:%0.2d",	   parts.tm_hours, parts.tm_minutes, parts.tm_seconds);
			return argStr;
		}

		// parsing seconds
		int	ToSeconds(const char* _Identity, int _BaseUnit) {
			CStringA	argX = _Identity;
			if (argX == "minute()")
				return SECONDS_IN_MINUTE;
			else if (argX == "hour()")
				return SECONDS_IN_HOUR;
			else if (argX == "day()")
				return SECONDS_IN_HOUR * HOURS_IN_DAY;

			// we extract the minutes
			CStringA argStr = Helpers::Parsing::ExtractBrackets(argX);
			if (!argStr.IsEmpty()) {
				Validators::Double	vd(argStr);
				if (!vd.Valid()) return -1;

				if (argX.Left(7) == "minutes")
					return (int)(vd.Result() * SECONDS_IN_MINUTE);
				else if (argX.Left(5) == "hours")
					return (int)(vd.Result() * SECONDS_IN_HOUR);
				else if (argX.Left(4) == "days")
					return (int)(vd.Result() * SECONDS_IN_HOUR * HOURS_IN_DAY);
				else if (argX.Left(5) == "weeks")
					return (int)(vd.Result() * SECONDS_IN_HOUR * HOURS_IN_DAY * DAYS_IN_WEEK);
				else
					return -1;
			}

			// return the number of seconds
			Validators::Double	vd(argStr);
			if (!vd.Valid()) return -1;
			return (int)(vd.Result() * _BaseUnit);
		}

		// splits a multipart stream into the actual seconds
		int MultipartToSeconds(const char* _Identity) {
			SplitString	  split;
			CStringA  arg;
			size_t c     = split.split(_Identity, "+", -1);
			int total = 0;
			for (size_t i = 0; i < c; i++) {
				arg = split[i]; arg.Trim();
				if (arg.GetLength() == 0)
					return -1;
				else {
					int returned = ToSeconds(arg);
					if (returned == -1) return -1;
					total+= returned;
				}
			}
			return total;
		}
	};


	namespace XML
	{
		// the class defenitions for the sli
		bool DataKeyset::KeyExists(LPCSTR _Index) 
		{ 
			for (DataKeyset::iterator ptr = begin(); ptr != end(); ptr++)
				if (ptr->first == _Index)
					return true;
			
			return false;
		}
		
		void DataKeyset::InsertKey(LPCSTR _Index, LPCSTR _Data) 
		{
		  	list[_Index] = _Data;
		}

		void DataKeyset::InsertKey(LPCSTR _Index, int _Data) 
		{
			CStringA dataBound; dataBound.Format("%d", _Data);
			InsertKey(_Index, dataBound); 
		}

		CStringA DataKeyset::ToSingleXML(LPCSTR _Type) 
		{
			CStringA midText, mainText;
			for (DataKeyset::iterator ptr = begin(); ptr != end(); ptr++)
				midText.AppendFormat("%s=\"%s\" ", ptr->first, HtmlEncode(ptr->second));

			mainText.Format("<%s %s />", _Type, midText);
			return mainText;
		}

		// returns if an xml key is valid
		bool valid_key(char by) {
			return ((by >= 'a' && by <= 'z') || (by >= '1' && by <= '0'));
		}

		// decodes XML data
		CStringA HtmlDecode(CStringA _Input) {
			_Input.Replace("&qt;", "\"");	_Input.Replace("&gt;", ">");
			_Input.Replace("&lt;", "<");	_Input.Replace("&amp;", "&");
			return _Input;
		}

		// decodes XML data
		CStringA HtmlEncode(CStringA _Input) {
			_Input.Replace("&", "&amp;");
			_Input.Replace("\"", "&qt;");	_Input.Replace(">", "&gt;");
			_Input.Replace("<",  "&lt;");
			return _Input;
		}

		bool ParseSingleLine(const char* _Input,  DataKeyset& _Info) {
			// the string that we will use for our parsing
			char*		psz = const_cast<char*>(_Input);
			CStringA	str = psz;
			CStringA	key = "";
			char*		pz  = psz;
			char*		pS  = NULL;

			while (pz != '\0') {
				// skip whitespace
				while (*pz == ' ' || *pz == '\t')
					pz++;
				
				// we must start our line on an open brackets
				if (*pz != '<')	return false;						// false opening bracket

				// move forward until hits whitespace
				pS = (++pz);
				while ((*pz >= 'a' && *pz <= 'z') || *pz == ':' ) 
					pz++;				// loop through each lowercase byte
				if (*pz != ' ')
					return false;						// invalid sequence

				// extract the xml tag name
				_Info.type =  str.Mid((int)(pS - psz), (int)(pz - pS));
				
				while (*pz != '\0') {								// again, loop until the end
					// move on till we find the key
					while (*pz == ' ') pz++;						// jump through whitespace
					if (*pz == '\0') return false;
					if (*pz == '/' && *(pz + 1) == '>')				// check for end of stream 
						return true;								// successful
					if (!valid_key(*pz)) return false;				// invalid starting key
					pS = pz;										// allocate start byte
					while (valid_key(*pz)) pz++;					// loop through the keys until reaches '='
					if (*pz != '=') return false;					// must be an equals '='
					
					// extract the initial part of the key
					key = str.Mid((int)(pS - psz), (int)(pz - pS));

					// extract the actual data section from the keypair
					if (*(++pz) != '"') return false;				// expects opening " thereafter;
					pz++; pS = pz;
					while (*pz != '\0' && *pz != '"') pz++;			// loop through remaining
					if (*pz == '\0') return false;					// should be closing '";
					_Info.InsertKey(key, str.Mid((int)(pS - psz), (int)(pz - pS)));
					pz++;
				}
			}

			// yay, success!
			return true;
		}
	}


}
