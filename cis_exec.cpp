#include "stdafx.h"
#include "cis_exec.h"
#include "cis_configuration.h"
#include "cis_interfaces.h"
#include "scripting_host.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace CIS 
{
	namespace Exec 
	{
		int	IExec::run()
		{
			// needs a right
			if (getRightsRequired().length() != 0)
			{
				if (getRight(getRightsRequired().c_str()) == false) {
					return Reason::Failed;
				}
			}

			// context check
			bool context_ok = false;

			// context
			if (getContextConsole() && running_context == CONTEXT_CONSOLE)
				context_ok = true;
			else if (getContextAvatar() && running_context == CONTEXT_AVATAR)
				context_ok = true;
			else
			{
				write("This command cannot be called from this context");
				return Reason::Failed;
			}

			// is the world connection dead
			if (getWorldRequired() == true && bot.getWorldConnection() != ConState::Connected)
			{
				write("This command requires a world connection");
				return Reason::Failed;
			}

			// the universe required
			if (getUniverseRequired() == true && bot.getUniverseConnection() != ConState::Connected)
			{
				write("This command requires a universe connection");
				return Reason::Failed;
			}

			// requires eject
			if (getEjectRequired() == true && bot.HasEject() != true)
			{
				write("This command requires eject rights");
				return Reason::Failed;
			}

			// requires CT
			if (getCaretakerRequired() == true && bot.HasCT() != true)
			{
				write("This command requires caretaker rights");
				return Reason::Failed;
			}

			// process
			return process();
		}

		void IExec::runResumption()
		{
			if (this_avatar_autonum)
			{
				Avatar* pAv = avatars.getByAuto(this_avatar_autonum);
				if (pAv) {
					String s;
					s.Format("Resuming session by %s", pAv->getName());
					WriteMsg(s, RGB_RED, OT_MAIN);
					this_avatar = pAv;
				}
				else 
					return;
			}

			// continue
			resume();
		}

		bool IExec::getFlag(const char* Flag) 
		{
			return input_flags.key(Flag);
		}
		
		CStringA IExec::getFlagText()
		{
			return input_flags.final;
		}

		// avatar contexts
		bool IExec::getRight(const char* Right)
		{
			// is not avatar
			if (this_avatar == 0) return true;

			// check avatar
			if (this_avatar && this_avatar->GetRight(Right))
				return true;

			// check false
			return false;
		}

		CStringA IExec::getName()
		{
			return getAvatar() ? getAvatar()->getName() : "@[client]";
		}

		//
		// Arguments
		//
		void IExec::setupArgs()
		{
			if (arguments.size() == 0)
				arguments.split( getFlagText(), ",", getArgLimit() );
		}

		CStringA IExec::getArg(size_t ArgId)
		{
			setupArgs();
			return arguments[ArgId];
		}

		int	IExec::getArgCount()
		{ 
			setupArgs();
			return (int)arguments.size(); 
		}

		//
		// Parameters
		//
		CStringA IExec::getParam(const char* Param, const char* Default)
		{
			return cp.getText(Param, Default);
		}

		int	IExec::getParam(const char* Param, int Default)
		{
			return atoi(cp.getText(Param, "0"));
		}

		bool IExec::getParamExists(const char* Param)
		{
			return cp.getExists(Param);
		}

		// output information
		void IExec::write(const char* Text)
		{
			write_output(Text);
		}

		void IExec::write(const char* Text, Umbra::VariantMap VM)
		{
			write_output(VM.parse_string(Text).c_str());
		}

		bool IExec::getUseConsole()
		{
			// will use console messages if at all possible
			if (CIS::Config::g_Interface.getAlwaysUseConsoles())
				return true;

			// this avatar exists 
			if (this_avatar) 
				return !((this_avatar->InRect(bot, 20000) && bot.getInvisible() == FALSE) || bot.GetGlobal() == false);
			else		 
				return false;
		}

		void IExec::write_output(CStringA Text)
		{
			if (this_avatar != 0)
			{
				// always use consoles
				if (getUseConsole() == true)
				{
					// send a console message
					this_avatar->ConsoleWhisper(Text, 
									CIS::Config::g_Interface.getOutputColour().getColour(), 
									Make_CMF(CIS::Config::g_Interface.getOutputBold(), CIS::Config::g_Interface.getOutputItalic(), false));
				}
				else
				{
					// was whispered to
					if (chat_type == AW_CHAT_WHISPER)
					{
						this_avatar->WhisperEx(Text, false);
					}
					else
					{
						bot.say(Text);
					}
				}
			}
			else
			{

				String logText;
				logText.Format(">\t%s", Text);
				WriteMsg(logText, 0x884444, OT_MAIN | MF_ITALIC);
			}
		}

		// rig for technology
		void IExec::rigForAvatar(Avatar& User, CStringA Text)
		{
			this_avatar					= &User;
			this_avatar_autonum	= User.getAutoNumber();
			raw_text						= Text;
			v_avatar						= User.getUmbraRemoteClass();
			running_context			= CONTEXT_AVATAR;

			// split the text
			cp.ToTokens(raw_text);
			input_flags.search(raw_text);
		}

		void IExec::rigForClient(CStringA Text)
		{
			raw_text = Text;
			running_context = CONTEXT_CONSOLE;

			// split the text
			cp.ToTokens(raw_text);
			input_flags.search(raw_text);
		}

		bool IExec::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
		{
			/*
				CStringA	getParam(const char* Param, const char* Default = "");
				int			getParam(const char* Param, int Default);
				bool		getParamExists(const char* Param);
			*/

			// 
			// Access a parameter
			//

			UMBRA_FUNCTION_BEGIN(getParam) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Key, Umbra::VT_STRING);
				UMBRA_FUNCTION_OA(1, Def, Umbra::VT_STRING, "");

				// code information break
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( getParam(vaKey.toString().c_str(), vaDef.toString().c_str()) );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// 
			// Access a parameter existance
			//

			UMBRA_FUNCTION_BEGIN(getParamExists) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Key, Umbra::VT_STRING);

				// code information break
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( getParamExists(vaKey.toString().c_str()) );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// 
			// write back to the user
			//

			UMBRA_FUNCTION_BEGIN(write) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Text, Umbra::VT_STRING);

				// code information break
				UMBRA_FUNCTION_CS() {
					write( vaText.toString().c_str() );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// 
			// write back to the user
			//

			UMBRA_FUNCTION_BEGIN(getRaw) {
				// code information break
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( (const char*)raw_text );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// 
			// Get a flag if it exists
			//

			UMBRA_FUNCTION_BEGIN(getFlag) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Key, Umbra::VT_STRING);

				// code information break
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( getFlag(vaKey.toString().c_str()) );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// 
			// Access a parameter existance
			//

			UMBRA_FUNCTION_BEGIN(getFlagText) {
				// code information break
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( (const char*)getFlagText() );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			// 
			// Get a right exists
			//

			UMBRA_FUNCTION_BEGIN(getRight) {
				// required arguments
				UMBRA_FUNCTION_DA(0, Key, Umbra::VT_STRING);

				// code information break
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( getRight(vaKey.toString().c_str()) );
				} UMBRA_FUNCTION_CE();
			} UMBRA_FUNCTION_END();

			//
			// Cannot Find
			//

			return Umbra::FunctionRequest::NOT_HANDLED;
		}

		bool IExec::doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R)
		{
			//
			// Basic Properties
			//

			UMBRA_PROPERTY_R(is_client, isClient());
			UMBRA_PROPERTY_R(is_avatar, isAvatar());

			// access

			UMBRA_PROPERTY_R(avatar, isAvatar() ? *this_avatar : Umbra::Variant());

			//
			// Cannot Find
			//

			return Umbra::PropertyRequest::NOT_HANDLED;
		}

		//
		// Scripted Processor
		//

		int ScriptedProcessor::process()
		{
			return ScriptingHost::getInst().doFunction(function_name, 0, 0, this).toInt32();
		}
	}
}