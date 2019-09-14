#include "stdafx.h"
#include "Avatar.h"

// memory allocation checking
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

long Avatar::WhisperEx(String MsgText, bool Supress)
{
	if (this->InRect(*plc_bot, 20000) == false || plc_bot->getInvisible() && plc_bot->HasCT())
	{
		return ConsoleWhisper(MsgText, RGB_BLUE, CM_ITALIC | (Supress ? CM_SUPPRESSED : 0));
	}
	else
	{
		std::vector<String> lines;
		size_t c = SplitToMultipart(lines, MsgText, AW_MAX_ATTRIBUTE_LENGTH, bot.getOuputBreakers());

		for (size_t i = 0; i <c; i++) {
			String line = lines[i];
			String log;
			if (!Supress) {
				log.Format("[%s]:\t(to %s) %s", bot.getName(), getName(), line);
				WriteMsg(log, RGB_BLUE, OT_MAIN | MF_ITALIC);
			}
			/* whisper to the target */
			int rc = plc_bot->_whisper(getSession(), line);
			if (rc) return rc;
		}
	}
	return 0;
}

long Avatar::ConsoleMessageEx(String MsgText, COLORREF Colour, int Style)
{
	/* redirect the whisper if no CT */
	if (!plc_bot->HasCT()) 
	{
		return WhisperEx(MsgText, (Style & CM_SUPPRESSED) != 0);
	} 
	else 
	{
		std::vector<String> lines;
		size_t c = SplitToMultipart(lines, MsgText, AW_MAX_ATTRIBUTE_LENGTH, bot.getOuputBreakers());

		for (size_t i = 0; i <c; i++) {
			String line = lines[i];
			String log;
			if ((Style & CM_SUPPRESSED) == 0) {
				log.Format("%s:\t(console to %s) %s", bot.getName(), getName(), line);
				WriteMsg(log, Colour, OT_MAIN);
			}
			/* whisper to the target */
			int rc = plc_bot->ConsoleMessage(getSession(), line, Colour, Style);
			if (rc) return rc;
		}
		return 0;
	}
}

int	Avatar::ConsoleWhisper(String MsgText, COLORREF Colour, int Style)
{
	if (!plc_bot->HasCT()) {
		return WhisperEx(MsgText, (Style & CM_SUPPRESSED) != 0);
	} else {
		std::vector<String> lines;
		int length = AW_MAX_ATTRIBUTE_LENGTH - 5 - bot.getName().GetLength();
		size_t c = SplitToMultipart(lines, MsgText, length, bot.getOuputBreakers());

		for (size_t i = 0; i <c; i++) {
			String line = lines[i];
			String log, out;
			if ((Style & CM_SUPPRESSED) == 0) {
				log.Format("[%s]:\t(c-to %s) %s", bot.getName(), getName(), line);
				WriteMsg(log, Colour, OT_MAIN);
			}
			/* whisper to the target */
			out.Format("[%s]:\t%s", bot.getName(), line);
			int rc = plc_bot->ConsoleMessage(getSession(), out, Colour, Style);
			if (rc) return rc;
		}
		return 0;
	}
}

int	Avatar::ConsoleMessagePrefixed(String Prefix, String MsgText, COLORREF Colour, int Style)
{
	std::vector<String> lines;
	int length = AW_MAX_ATTRIBUTE_LENGTH - 3 - Prefix.GetLength();
	size_t c = SplitToMultipart(lines, MsgText, length, bot.getOuputBreakers());

	for (size_t i = 0; i <c; i++) {
		String line = lines[i];
		String log, out;
		if ((Style & CM_SUPPRESSED) == 0) {
			log.Format("%s:\t(c-to %s) %s", Prefix, getName(), line);
			WriteMsg(log, Colour, OT_MAIN);
		}
		/* whisper to the target */
		out.Format("%s:\t%s", Prefix, line);
		int rc = plc_bot->ConsoleMessage(getSession(), out, Colour, Style);
		if (rc) return rc;
	}
	return 0;
}

int	Avatar::SendSound(String SoundUrl)
{
	plc_bot->Lock();
	plc_bot->_string_set(AW_SOUND_NAME, SoundUrl);
	plc_bot->_noise(getSession());
	plc_bot->Unlock();
	return 0;
}

int	Avatar::SendURL(String Url, String Target, bool Target3D, String Post)
{
	plc_bot->_bool_set			(AW_URL_TARGET_3D,	Target3D ? TRUE : FALSE);
	plc_bot->_string_set		(AW_URL_POST,		Post);
	return plc_bot->_url_send	(m_session,			Url, Target.IsEmpty() ? NULL : Target);
}