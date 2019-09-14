#pragma once

#include "InputFlags.h"			// input flags
#include "CommandParser.h"
#include "umbra/umbra_remoteclass.h"

namespace CIS
{
	namespace Exec
	{
		class IExec : public Umbra::RemoteClass
		{
		public: // bitmask
			static const int REQUIRES_WORLD			= 1;
			static const int REQUIRES_UNIVERSE		= 2;
			static const int REQUIRES_EJECT			= 4;
			static const int REQUIRES_CARETAKER		= 8;

		public: // contexts
			static const int CONTEXT_AVATAR			= 1;
			static const int CONTEXT_CONSOLE		= 2;
			static const int CONTEXT_REMOTE			= 3;
			static const int CONTEXT_ANY			= (CONTEXT_AVATAR | CONTEXT_CONSOLE | CONTEXT_REMOTE);

		protected: // the root statement
			virtual int  getBitmask() const { return 0; }
			virtual int  getContext() const { return CONTEXT_ANY; } 

		public: // execution
			virtual int  process()	{ return 0; }
			virtual int	 run();		
			virtual void runResumption();
			virtual int  resume()	{ return 0; }		// may be resumed repeatedly

		public: // umbra functions
			bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
			bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);

		public: // constructor
			IExec() { this_avatar = 0; this_avatar_autonum = 0; }
			virtual ~IExec() { }

		public: // the request and response
			CStringA				raw_text;
			Avatar*					this_avatar;
			int							this_avatar_autonum;
			int							chat_type;
			Umbra::Variant	v_avatar;
			int							running_context;

		public: // setup
			void		rigForAvatar(Avatar& User, CStringA Text);
			void		rigForClient(CStringA Text);

		protected: // parsing
			InputFlags	input_flags;
			CStringA	getRaw() { return raw_text; }
			bool		getFlag(const char* getFlag);
			CStringA	getFlagText();

		protected: // command parser
			CommandParser cp;
			bool		is_parsed;
			CStringA	getParam(const char* Param, const char* Default = "");
			int			getParam(const char* Param, int Default);
			bool		getParamExists(const char* Param);

		protected: // argument list
			SplitString	arguments;
			void		setupArgs();
			CStringA	getArg(size_t ArgId);
			int			getArgCount();
			virtual int	getArgLimit() { return -1; }

		protected: // the bot code
			Avatars&	getAvatars();
			Avatar&		getAvatar(size_t Index);

		protected: // operations
			Avatar*		getAvatar() { return this_avatar; }
			CStringA	getName();
			bool		getRight(const char* Right);

		protected: // checking mechanisms
			bool		isAvatar() { return this_avatar != 0; } 
			bool		isClient() { return this_avatar == 0; }

		protected: // responding back
			CStringA	temp;
			void		write(const char* Text);
			void		write(const char* Text, Umbra::VariantMap VM);
			void		write_output(CStringA Text);
			bool		getUseConsole();

		public: // resumption information
			Umbra::VariantMap resumption_args;

		public: // the variable assets
			virtual bool getWorldRequired()	const		{ return (getBitmask() & REQUIRES_WORLD)	 != 0; }
			virtual bool getUniverseRequired()	const	{ return (getBitmask() & REQUIRES_UNIVERSE)	 != 0; }
			virtual bool getEjectRequired() const		{ return (getBitmask() & REQUIRES_EJECT)	 != 0; }
			virtual bool getCaretakerRequired() const	{ return (getBitmask() & REQUIRES_CARETAKER) != 0; }
			
			virtual bool getContextConsole() const		{ return (getContext() & CONTEXT_CONSOLE)	 != 0; }
			virtual bool getContextAvatar() const		{ return (getContext() & CONTEXT_AVATAR)	 != 0; }
			virtual string getRightsRequired() const    { return ""; }
		};

		//
		// Scripted Commands
		//

		class ScriptedProcessor : public IExec
		{
		public: // the bitset
			int		bitmask;
			int		context;
			String	function_name;
			String	required_right;

		public: // execute a script
			int process();

		public: // bitmask copy
			int getBitmask() const { return bitmask; }
			int getContext() const { return context; } 
			string getRightsRequired() const    { return (const char*)required_right; }
		};
	}
}