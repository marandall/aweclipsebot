#pragma once
#include "GraphicsNet_2DMap.h"
#include "property_registry.h"

namespace AW
{
	class Query_5x5;
	class Query_Cache;
}

namespace Property
{
	class Objects;
}

namespace Querying
{
	class Query_5x5;
}

namespace Gdiplus
{
	class Graphics;
}

namespace GraphicsDisplay
{
	class QueryMap : 
		public GraphicsNet::FlatMap
	{
	public: // the color coding types
		static const int COLORSTYLE_SELECTED		= 0;
		static const int COLORSTYLE_BUILD_STATUS	= 1;
		int	color_style;

	public: // layout styles
		static const int VIEWTYPE_DOTS			= 0;
		static const int VIEWTYPE_NUMBERS		= 1;
		static const int VIEWTYPE_CELLS			= 2;

	public:
		int		view_type;

	public:
		 QueryMap();
		 ~QueryMap();

	public: // requirements
		Querying::Query_5x5*	m_pQuery;
		Property::Objects*		m_pCache;
		__time64_t				m_last_updated;

	public: // caching bitmap
		Gdiplus::Bitmap*		m_pCellBitmap;
		Gdiplus::Bitmap*		m_pSelectionRect;
		Gdiplus::Bitmap*		m_pGrid;

		Property::Registry		registry;
		void					Reset();

	public: // the draw styles
		void	drawDots(Gdiplus::Graphics& G);
		void	drawCells(Gdiplus::Graphics& G);
		void	drawNumbers(Gdiplus::Graphics& G);
		void	drawOverlay(Gdiplus::Graphics& G);

	protected:
		void	drawGrid();
		void	drawCells();

	public: // drawing function
		virtual bool OnDraw();
	};
}