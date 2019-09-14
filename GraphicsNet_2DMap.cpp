#include "stdafx.h"
#include "GraphicsNet_Base.h"
#include "GraphicsNet_2DMap.h"
#include "graphicslabel.h"

using namespace Gdiplus;

#define	STYLE_BACKGROUND		0x003300


namespace GraphicsNet
{
	void FlatMap::setScale(int Width, int Height, int Units)
	{
		m_visible_units = Units;
		m_PPU = Width / (float)Units;
	}

	Gdiplus::PointF FlatMap::TranslatePoint(int X, int Z)
	{
		Gdiplus::PointF pt;
		pt.X = (m_Width / 2) + (m_x - X) * m_PPU;
		pt.Y = (m_Height / 2) + (m_z - Z) * m_PPU;
		return pt;
	}

	Gdiplus::PointF FlatMap::TranslatePoint(const AW::Location& Loc)
	{
		return TranslatePoint(Loc.getX(), Loc.getZ());
	}

	/*
		Tasks:
		- Draws a small circle (representing type with color)
		- Draws small pointer indicating the user
		- Draw their name below
		
	*/
	using namespace Gdiplus;

	void FlatMap::changeBoundingBoxVisibility(int Mode)
	{
		bbox_visibility = Mode;
		m_pWindow->Invalidate();
	}

	void FlatMap::DrawAvatar(Avatar& Av)
	{
		const Color clr_types[] = { Color::Blue, Color::Green, Color::Red, Color::Green };
		REAL	CIRCLE_SIZE = 10.0f;
		REAL	HALF_CIRCLE	= 5.0f;
		REAL	THIRD_CIRCLE= 8.0f;
		REAL	VERT_OFFSET	= 10;
		REAL	FONT_SIZE	= 10;

		/* create gdi objects */
		PointF		pos = TranslatePoint(Av);
		SolidBrush	sbPoint(clr_types[Av.getType()]);
		Pen			penPoint(clr_types[Av.getType()], 1.0); 

		/* draw circle */
		m_Gdi->DrawArc(&penPoint, (REAL)pos.X - HALF_CIRCLE, (REAL)pos.Y - HALF_CIRCLE, 
				CIRCLE_SIZE, CIRCLE_SIZE, 0.0f, 360.0f);

		/* draw directional line */
		m_Gdi->DrawLine(&penPoint, pos.X, pos.Y, 
				pos.X - sin(Av.GetRadianYAW()) * THIRD_CIRCLE,
				pos.Y - cos(Av.GetRadianYAW()) * THIRD_CIRCLE);

		/* draw name underneith */
		Gdiplus::Font				font(L"Arial", 8);
		Gdiplus::RectF			layoutRect(pos.X - 40, 
										   pos.Y + VERT_OFFSET, 
										   80,
										   FONT_SIZE + 5);
		Gdiplus::StringFormat	stringFormat;
		stringFormat.			SetAlignment(StringAlignmentCenter);
		CStringW				nameW(Av.getName());
		m_Gdi->DrawString(nameW, -1, &font, layoutRect, &stringFormat, &sbPoint);
	}

	/*
		Draws a cell grid on the screen at the cellspace intervals
	*/
	void FlatMap::DrawGrid(int CellSpace)
	{
		
	}

	void FlatMap::zoom_out(float Percent)
	{
		setScale(m_Width, m_Height, (int)(m_visible_units * (1.0F + Percent)));
	}
	
	void FlatMap::zoom_in(float Percent)
	{
		setScale(m_Width, m_Height, (int)(m_visible_units * (1.0F - Percent)));
	}


	bool FlatMap::OnDraw()
	{
		/* draw the background with flood fill */
		SolidBrush	brSolid(Color::White);
		RectF		rtFull(1.0, 1.0, (REAL)m_Width, (REAL)m_Height);
		m_Gdi->		FillRectangle(&brSolid, rtFull);
		
		/* draw the range that we can detect */
		PointF		pos = TranslatePoint(bot);
		Pen			pn(Color::Green, 1);
		pn.SetDashStyle(DashStyleDot);

		const REAL BOUND_BOX_SIZE = 20000 * m_PPU;
		m_Gdi->DrawRectangle(&pn, RectF(pos.X - BOUND_BOX_SIZE, 
										pos.Y - BOUND_BOX_SIZE,
										BOUND_BOX_SIZE * 2,
										BOUND_BOX_SIZE * 2));

		/* draw each avatar */
		for (size_t i = 0; i < avatars.size(); i++)
			DrawAvatar(avatars[i]);
		return true;
	}

	//
	//
	//
	void FlatMap::drawSelectionBox(Graphics& G)
	{
		// must be when deselected
		if (getBoundingBoxVisibility() == FlatMap::BBOX_VIS_DISABLED)
			return;

		// far point
		AW::Location endpt;
		if (getBoundingBoxVisibility() == FlatMap::BBOX_VIS_TRACKING)
			endpt = pos_Mouse;
		else
			endpt = pos_MouseUp;

		// plots a single box around the selection
		PointF pn = TranslatePoint(pos_MouseDown.getX(), pos_MouseDown.getZ());
		PointF ps = TranslatePoint(endpt.getX(), endpt.getZ());
	
		// draw a dashed line and define the bounding box
		Gdiplus::Pen	dashed(Color::Red, 1.5);
		Gdiplus::Pen	fixed(Color::DarkMagenta, 3);
		Gdiplus::Pen*	pPenUsed = &fixed;

		// is it fixed
		if (getBoundingBoxVisibility() == FlatMap::BBOX_VIS_TRACKING) {
			pPenUsed = &dashed;
		}

		// calculate rectangle
		Gdiplus::RectF	rect  (pn.X, pn.Y, ps.X - pn.X, ps.Y - pn.Y);

		// draw it to screen
		dashed.SetDashStyle(DashStyleDashDotDot);
		fixed.SetDashStyle(DashStyleDashDotDot);
		
		// switch style
		G.DrawRectangle(pPenUsed, rect);

		// overlay text
		CStringW dist_x, dist_z;
		dist_x.Format(L"%d m", abs(pos_MouseDown.getX() - endpt.getX()) / 100);
		dist_z.Format(L"%d m", abs(pos_MouseDown.getZ() - endpt.getZ()) / 100);

		// string formats
		StringFormat strFmt;
		strFmt.SetAlignment(StringAlignmentCenter);
		strFmt.SetLineAlignment(StringAlignmentFar);

		// brush
		SolidBrush strBrush(Color::Black);

		// Font
		Gdiplus::Font font(L"Arial", 10, Gdiplus::FontStyleBold, UnitPixel);

		// draw range above the box in the middle
		RectF bbx(pn.X, pn.Y + 20, ps.X - pn.X, ps.Y - pn.Y);
		G.DrawString(dist_x, -1, &font, bbx, &strFmt, &strBrush);
	
		// draw for the Z
		RectF bby(pn.X - 50, (pn.Y + ps.Y) / 2 - 10, 50, 20);
		strFmt.SetAlignment(StringAlignmentFar);
		G.DrawString(dist_z, -1, &font, bby, &strFmt, &strBrush);
	}

	void FlatMap::drawPosition(Gdiplus::Graphics& G)
	{
		// the x scale

	}


	void FlatMap::setclick_point(int x, int y)
	{
		int snapping = 1000;

		mouse_x = -static_cast<int>(((x - (m_Width / 2)) / m_PPU) - m_x);
		mouse_z = -static_cast<int>(((y - (m_Height / 2)) / m_PPU) - m_z);
	
		// snap snap
		// if ((mouse_x % snapping) >= (snapping / 2)) mouse_x+= (mouse_x % snapping);
		// else mouse_x-= (mouse_x % snapping);
		// int mt =  (mouse_z % snapping);
		// mouse_x-= (mouse_x % snapping);
		// mouse_z = mt;
	}

	void FlatMap::OnLButtonDown(int X, int Y)
	{
		setclick_point(X, Y);
		post_handler_msg("map_l_down", this);

		// for the drag data
		select_start_x	= mouse_x;
		select_start_z	= mouse_z;

		// set mouse position
		pos_MouseDown = AW::Location(mouse_x, 10000000, mouse_z, 0);

		// begin bounding box
		changeBoundingBoxVisibility(FlatMap::BBOX_VIS_TRACKING);
	}
	
	void FlatMap::OnMButtonDown(int X, int Y)
	{
		setclick_point(X, Y);
		post_handler_msg("map_m_down", this);
	}

	void FlatMap::OnRButtonDown(int X, int Y)
	{
		setclick_point(X, Y);
		post_handler_msg("map_r_down", this);
	}

	void FlatMap::OnMButtonDoubleClick(int X, int Y)
	{
		setclick_point(X, Y);
		post_handler_msg("map_m_double", this);
	}

	void FlatMap::OnLButtonUp(int X, int Y)
	{
		// begin bounding box
		changeBoundingBoxVisibility(FlatMap::BBOX_VIS_FIXED);

		setclick_point(X, Y);
		post_handler_msg("map_l_up", this);
		post_handler_msg("map_select_complete", this);

		// was it a single click? ie same position
		if (pos_MouseUp.distanceToGD(pos_MouseDown) < 20)
			changeBoundingBoxVisibility(FlatMap::BBOX_VIS_DISABLED);

		// set mouse position
		pos_MouseUp = AW::Location(mouse_x, -10000000, mouse_z, 0);
	}

	void FlatMap::OnMButtonUp(int X, int Y)
	{
		setclick_point(X, Y);
	}
	
	void FlatMap::OnRButtonUp(int X, int Y)
	{
		setclick_point(X, Y);
	}

	void FlatMap::OnMouseMove(int X, int Y, UINT Flags)
	{
		setclick_point(X, Y);

		// mouse has been moved
		AW::Location lp = pos_Mouse;
		pos_Mouse = AW::Location(mouse_x, 0, mouse_z, 0);
		
		// bounding - has position changed?
		if ((Flags & MK_LBUTTON) && (pos_Mouse.getX() != lp.getX() || pos_Mouse.getZ() != lp.getZ()))
		{
			// tracking
			changeBoundingBoxVisibility(FlatMap::BBOX_VIS_TRACKING);
			m_pWindow->Invalidate();
		}

		post_handler_msg("map_mouse", this);
	}
}
