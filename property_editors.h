#pragma once
#include "property_editor_duplicate_ag.h"
#include "property_editor_stringreplace_ag.h"
#include "property_editor_translate_ag.h"
#include "property_editor_replacefield_ag.h"

namespace Property
{
	namespace Editors
	{
		class Editor
		{
		public: // constant
			static const int	PER_OBJECT			= 1;
			static const int	PER_COLLECTION		= 2;

		public: // destructor
			Editor() { }
			virtual ~Editor() { }

		public: // option types
			virtual int		getEditType()					{ return PER_OBJECT; }
			virtual bool	run(ObjectData& Data)			{ return false; }
			virtual bool	run(Property::Objects& Objects);
			virtual bool	runObjects(Property::Objects& Objects) { return run(Objects); }
		};

		class Duplicate : public AutoTemplates::DuplicateData, public Editor
		{
		public: 
			int	getEditType() { return PER_COLLECTION; }
			bool run(Property::Objects& Objects);
		};

		class StringReplace : public AutoTemplates::StringReplaceData, public Editor
		{
			int	getEditType() { return PER_OBJECT; }
			bool run(ObjectData& Data);
		};

		class Translate : public AutoTemplates::TranslateData, public Editor
		{
		public: // the coordinates
			AW::Location offset_result;
		protected:
			int	 getEditType() { return PER_OBJECT; }
			bool onUpdated();
			bool run(ObjectData& Data);
		};

		class QueueSelected : public Editor
		{
		public:
			int	getEditType() { return PER_OBJECT; }
			bool run(ObjectData& Data);
		};

		class QueueEverything : public Editor
		{
		public:
			int	getEditType() { return PER_OBJECT; }
			bool run(ObjectData& Data);
		};

		class StripRwx : public Editor
		{
		public:
			int	getEditType() { return PER_OBJECT; }
			bool run(ObjectData& Data);
		};

		class AppendRwx : public Editor
		{
		public:
			int	getEditType() { return PER_OBJECT; }
			bool run(ObjectData& Data);
		};

		class ReplaceField : public AutoTemplates::ReplaceFieldData, public Editor
		{
		public:
			int	getEditType() { return PER_OBJECT; }
			bool run(ObjectData& Data);
		};

		class DeleteSelected : public Editor
		{
		public: 
			int	getEditType() { return PER_COLLECTION; }
			bool run(Property::Objects& Objects);
		};

		class Crop : public Editor
		{
		public: 
			int	getEditType() { return PER_COLLECTION; }
			bool run(Property::Objects& Objects);
		};
	}
}