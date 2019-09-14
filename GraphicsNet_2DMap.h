#pragma once
#include "GraphicsNet_Base.h"
#include "awsdk_location.h"
#include "gdiplus.h"

class Avatar;

namespace GraphicsNet
{

	class FlatMap :
		public AW::Location,
		public BaseObject
	{
	public: // much needed constructor
		FlatMap()
		{
			bbox_visibility = BBOX_VIS_DISABLED;
		}

	public:
		float	m_PPU;					// pixel per unit
		int		m_visible_units;		// the most visible cells
		
	public: // the position (in aw coordinates) of the mouse
		void	setclick_point(int x, int y);
		int		mouse_x;
		int		mouse_z;
		int		select_start_x;
		int		select_start_z;

	public: // bounding box visibility
		static const int BBOX_VIS_DISABLED	= 0;		// bounding box is disabled
		static const int BBOX_VIS_TRACKING	= 1;		// bounding box is tracking (changing)
		static const int BBOX_VIS_FIXED		= 2;		// bounding box is in place (right click up)

	public: // bounding box options
		int		bbox_visibility;
		void	changeBoundingBoxVisibility(int Mode);
		int		getBoundingBoxVisibility() { return bbox_visibility; }

	public: // basic label
		static const int ALIGN_TOPLEFT		= 0;
		static const int ALIGN_TOPRIGHT		= 1;
		static const int ALIGN_BOTTOMLEFT	= 2;
		static const int ALIGN_BOTTOMRIGHT	= 3;

	public: // click position information
		bool			bbox_activated;
		AW::Location	pos_MouseDown;
		AW::Location	pos_MouseUp;
		AW::Location	pos_Mouse;

	public: // click changes
		int		getMoveUnitX() const { return m_visible_units / 10; }
		int		getMoveUnitZ() const { return m_visible_units / 10; }

	public: // the center location
		void	changeCenter(int x, int z)	{ m_x = x; m_z = z; }
		void	moveNorth	(int size)		{ m_z+= size; }
		void	moveSouth	(int size)		{ m_z-= size; }
		void	moveEast	(int size)		{ m_x-= size; }
		void	moveWest	(int size)		{ m_z+= size; }

	public: // calculations
		void	zoom_out(float Percent);
		void	zoom_in(float Percent);

	public: // mouse capture handlers
		void	OnLButtonDown	(int X, int Y);
		void	OnMButtonDown	(int X, int Y);
		void	OnRButtonDown	(int X, int Y);
		void	OnLButtonUp		(int X, int Y);
		void	OnMButtonUp		(int X, int Y);
		void	OnRButtonUp		(int X, int Y);
		void	OnMouseMove		(int X, int Y, UINT Flags);
		virtual void OnMButtonDoubleClick(int X, int Y);

	public: // overrides
		virtual void onMouseLDown(AW::Location& Pos, UINT Flags) { }
		virtual void onMouseMDown(AW::Location& Pos, UINT Flags) { }
		virtual void onMouseRDown(AW::Location& Pos, UINT Flags) { }
		virtual void onMouseLUp	 (AW::Location& Pos, UINT Flags) { }
		virtual void onMouseMUp	 (AW::Location& Pos, UINT Flags) { }
		virtual void onMouseRUp	 (AW::Location& Pos, UINT Flags) { }
		virtual void onMouseMove (AW::Location& Pos, UINT Flags) { }

	public:
		void	setScale(int Width, int Height, int Cells);
		AW::Location getMouseLocation()		  { return AW::Location(mouse_x, 0, mouse_z, 0); }
		AW::Location GetSelectStartLocation() { return AW::Location(select_start_x, 0, select_start_z, 0); }

	protected:
		Gdiplus::PointF	TranslatePoint(int X, int Z);
		Gdiplus::PointF	TranslatePoint(const AW::Location& Loc);
		void	DrawGrid(int CellSpace);
			
	public:
		void	drawSelectionBox(Gdiplus::Graphics& G);
		void	drawPosition(Gdiplus::Graphics& G);

		virtual bool OnDraw();
		void	DrawAvatar(Avatar& Av);
	};
}