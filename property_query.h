#pragma once
class CommonBot;

namespace Querying
{
	class IQuery
	{
	public: // virtual
		virtual ~IQuery() { }

	public: // the query interfaces
		virtual void handleCellBegin	(void)		= 0;
		virtual void handleCellEnd		(void)		= 0;
		virtual void handleCallbackQuery(int rc)	= 0;

	public: // the object interfaces
		virtual void handleCellObject	(ObjectData& OD) { }
		virtual void handleObjectAdd	(ObjectData& OD) { }
		virtual void handleCallbackCell	(int rc)		 { }

	public: // the client bot
		CommonBot* base_bot;
		CommonBot* getBaseBot() { return base_bot; }

	protected: // cell tracking
		int		cur_cell_x;
		int		cur_cell_z;

	public: // cell tracking
		int		getCurCellX() { return cur_cell_x; }
		int		getCurCellZ() { return cur_cell_z; }
		virtual int getCount() { return 0; }
	};
}