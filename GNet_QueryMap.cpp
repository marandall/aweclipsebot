#include "stdafx.h"
#include "GNet_QueryMap.h"
#include "propertyproject.h"
#include "gdiplus.h"
#include "property_query_queue.h"

using namespace Gdiplus;
using namespace Property;

namespace GraphicsDisplay
{
	QueryMap::QueryMap()
	{
		m_pCellBitmap	= NULL;
		m_last_updated	= 0;

		color_style		= QueryMap::COLORSTYLE_SELECTED; 
		view_type		= QueryMap::VIEWTYPE_DOTS;
	}

	QueryMap::~QueryMap()
	{
		delete m_pCellBitmap;
	}

	void QueryMap::Reset()
	{
		delete m_pCellBitmap;
		m_pCellBitmap  = 0;
		m_last_updated = 0;
	}

	
	// rotation
	void rotateMatrix(int x, int z, int& fx, int& fz, float theta)
	{
		fx = (int)(cos(theta) * x - sin(theta) * z);
		fz = (int)(sin(theta) * x + cos(theta) * z);
	}

	void QueryMap::drawCells(Graphics& g)
	{
		// empty cell
		SolidBrush emptyBrush(Color::White);

		// the width to draw
		int block_width = static_cast<int>(1000 * m_PPU);
		const int CELL_BLOCK_RESOLUTION	= 5;

		// cell grid positions
		const int grid_size	  = 80; // size in cells 
		const int base_x = getCellX() * 1000;
		const int base_z = getCellZ() * 1000;		
		
		// the cell grid position
		int high_x	= base_x + grid_size * 500;
		int high_z	= base_z + grid_size * 500;
		int low_x	= base_x - grid_size * 500;
		int low_z	= base_z - grid_size * 500;

		// object cache
		for(Regions::iterator citr = m_pCache->cells.begin(); 
			citr != m_pCache->cells.end();
			citr++)
		{
			// updated
			if (citr->second.last_updated < m_last_updated)
				continue;

			// first reset needed
			int x = citr->second.x + 1;
			int z = citr->second.z + 1;
			PointF f = TranslatePoint(x * 1000, z * 1000);

			// density
			const int MAX_DENSITY = 12;
			if (citr->second.count == 0)
				g.FillRectangle(&emptyBrush, f.X, f.Y, (REAL)block_width + 1, (REAL)block_width + 1);
			else
			{
				int clrval = static_cast<int>(240 - (240.0F / MAX_DENSITY * (citr->second.count > MAX_DENSITY ? MAX_DENSITY : citr->second.count)));

				Color clr;
				clr.SetFromCOLORREF(RGB(clrval, clrval, 255));
				SolidBrush br(clr);

				g.FillRectangle(&br, f.X, f.Y, (REAL)block_width + 1, (REAL)block_width + 1);
			}
		}
	}

	void QueryMap::drawOverlay(Graphics& g)
	{
		// cell grid positions
		const int grid_size	  = 80; // size in cells 
		const int base_x = getCellX() * 1000;
		const int base_z = getCellZ() * 1000;		
		
		// the cell grid position
		int high_x	= base_x + grid_size * 500;
		int high_z	= base_z + grid_size * 500;
		int low_x	= base_x - grid_size * 500;
		int low_z	= base_z - grid_size * 500;

		// the width to draw
		int block_width = static_cast<int>(1000 * m_PPU);
		const int CELL_BLOCK_RESOLUTION	= 5;

		Pen grid_pen(Color::LightYellow, 1);

		if (block_width > CELL_BLOCK_RESOLUTION)
		{
			// east west
			for (int x = low_x; x <= high_x; x+= 1000)
			{
				PointF line_top		= this->TranslatePoint(x, high_z);
				PointF line_bottom	= this->TranslatePoint(x, low_z);
				g.DrawLine(&grid_pen, line_top.X, line_top.Y, line_bottom.X, line_bottom.Y);
			}

			// north south
			for (int z = low_z; z <= high_z; z+= 1000)
			{
				PointF line_top		= this->TranslatePoint(low_x,  z);
				PointF line_bottom	= this->TranslatePoint(high_x, z);
				g.DrawLine(&grid_pen, line_top.X, line_top.Y, line_bottom.X, line_bottom.Y); 
			}
		}

		// draw center point of query
		PointF	query_centre_nw = TranslatePoint(500, 500);
		REAL	gz_width		= 1000 * m_PPU;
		Pen		gz_pen(Color::Blue, 1.1F);
		g.DrawRectangle(&gz_pen, query_centre_nw.X, query_centre_nw.Y, gz_width, gz_width);
		

		// draw sectors
		int sector_size   = 8000;
		int sector_half   = 4000;
		const int MAX_SECTORS_DRAWN = m_visible_units / 8000 + 2;
		const int MAX_SECTORS_START	= -(MAX_SECTORS_DRAWN / 2);
		const int MAX_SECTORS_END	= (MAX_SECTORS_DRAWN / 2);

		float sect_width  = sector_size * m_PPU;

		for (int i = MAX_SECTORS_START; i <= MAX_SECTORS_END; i++)
		{
			for (int j = MAX_SECTORS_START; j <= MAX_SECTORS_END; j++)
			{
				int sect_offsetx = aw_sector_from_cell(base_x / 1000) * 8000 + 8000 * i;
				int sect_offsetz = aw_sector_from_cell(base_z / 1000) * 8000 + 8000 * j;

				PointF sector_mid = TranslatePoint(sect_offsetx, sect_offsetz);
				RectF  sector_f(sector_mid.X - sect_width / 2,
								sector_mid.Y - sect_width / 2,
								sect_width, sect_width);

				Pen pSector(Color::Gray, 1);
				g.DrawRectangle(&pSector, sector_f);
			}
		}

		// outline drawing for large query
		if (bot.getQueryMode() == bot.QUERY_LARGE_AREA)
		{
			Pen query_pen(Color::DarkCyan, 2);
			Querying::MultiQueue& mQueue = bot.getQueryLarge(); 
			PointF nw = TranslatePoint(mQueue.bounds_nw);
			PointF se = TranslatePoint(mQueue.bounds_se);
			g.DrawRectangle(&query_pen, nw.X, nw.Y, se.X - nw.X, se.Y - nw.Y);
		}
	}

	void QueryMap::drawNumbers(Graphics& G)
	{

	}

	void QueryMap::drawDots(Graphics& g)
	{
		// cell grid positions
		const int grid_size	  = 80; // size in cells 
		const int base_x = getCellX() * 1000;
		const int base_z = getCellZ() * 1000;		
		
		// the cell grid position
		int high_x	= base_x + grid_size * 500;
		int high_z	= base_z + grid_size * 500;
		int low_x	= base_x - grid_size * 500;
		int low_z	= base_z - grid_size * 500;
		
		// the grid view
		Pen		grid_pen(Color::Bisque, 1);		
		bool	cells_changed = false;

		//
		//
		//

		// iterate each cell, wipe out as needed


		// color styles
		Color buildStateColors[ObjectData::BS_MAX];
		buildStateColors[ObjectData::BS_NONE]		= Color::Black;
		buildStateColors[ObjectData::BS_QUEUED]		= Color::Blue;
		buildStateColors[ObjectData::BS_PENDING]	= Color::Yellow;
		buildStateColors[ObjectData::BS_SUCCEEDED]	= Color::Green;
		buildStateColors[ObjectData::BS_FAILED]		= Color::Red;

		// color style pens
		Pen* buildStatePens[ObjectData::BS_MAX];
		for (int i = 0; i < ObjectData::BS_MAX; i++)
			buildStatePens[i] = new Pen(buildStateColors[i], 1.0);

		// a list of cells that have been reset
		std::map<int, bool> reset_cells;


		// the width to draw
		int block_width = static_cast<int>(1000 * m_PPU);
		const int CELL_BLOCK_RESOLUTION	= 5;

		// only draw updated cells
		size_t count = m_pCache->cells.size();
	
		using namespace Property;

		// setup pen
		Pen penNormal(Color::Blue, 1);
		Pen penSelected(Color::Red, 1);

		vector<Color> clr;
		clr.push_back(Color::Wheat);

		SolidBrush clearBrush(clr[rand() % clr.size()]);

		// object cache
		for(Regions::iterator citr = m_pCache->cells.begin(); 
			citr != m_pCache->cells.end();
			citr++)
		{
			// updated
			if (citr->second.last_updated < m_last_updated)
				continue;

			// first reset needed
			int x = citr->second.x + 1;
			int z = citr->second.z + 1;
			PointF f = TranslatePoint(x * 1000, z * 1000);

			g.FillRectangle(&clearBrush, f.X, f.Y, (REAL)block_width + 1, (REAL)block_width + 1);
		}

		// go through each object
		for (Property::Objects::iterator ptr = m_pCache->begin(); 
			ptr != m_pCache->end(); 
			ptr++)
		{
			//
			//
			//

			// is in an updated sector
			time_t lu = m_pCache->cells.getLastUpdated(ptr->getCellX(), ptr->getCellZ());
			if (lu < m_last_updated)
				continue;

			//
			//
			//

			PointF pt = TranslatePoint(ptr->getX(), ptr->getZ());
			Pen* pen = ptr->getSelected() ? &penSelected : &penNormal;

			// get the model
			Registry::Model* pModel = registry[ptr->getModel()];
			if (pModel && (abs(pModel->x1) > 4000 || abs(pModel->z1) > 4000))
			{
				// rotation
				float theta = ptr->GetRadianYAW();
				if (fabs(theta) > 0.001)
				{
					int j = 0;
				}

				// translate
				Registry::Model m2;
				rotateMatrix(pModel->x1, pModel->z1, m2.x1, m2.z1, theta);
				rotateMatrix(pModel->x2, pModel->z1, m2.x2, m2.z1, theta);
				rotateMatrix(pModel->x1, pModel->z2, m2.x1, m2.z2, theta);
				rotateMatrix(pModel->x2, pModel->z2, m2.x2, m2.z2, theta);

				// the mapping data
				//
				// bx,ty    tx,ty
				// 
				//
				// bx,bz    tx,bz
				int xleft	= ptr->getX() + m2.x1;
				int xright	= ptr->getX() + m2.x2;
				int ztop	= ptr->getZ() + m2.z1;
				int zbottom = ptr->getZ() + m2.z2;

				// copy data
				//
				PointF points[4];
				points[0] = TranslatePoint(xleft,	ztop);
				points[1] = TranslatePoint(xright,	ztop);
				points[2] = TranslatePoint(xright,  zbottom);
				points[3] = TranslatePoint(xleft,   zbottom);

				// draw the rectangle
				g.DrawPolygon(pen, points, 4);
			}
			else
			{
				// the draw state
				Pen* pSelectedPen = 0;
				if (color_style == QueryMap::COLORSTYLE_SELECTED)
				{
					pSelectedPen = ptr->getSelected() ? &penSelected : &penNormal;
				}
				else if (color_style == QueryMap::COLORSTYLE_BUILD_STATUS)
				{
					pSelectedPen = buildStatePens[ptr->getBuildState()];
				}

				// selection
				if (ptr->getSelected())
				{
					g.DrawRectangle(pSelectedPen, pt.X - 2, pt.Y - 2, 5.0, 5.0);
				}
				else
				{
					g.DrawRectangle(pSelectedPen, pt.X, pt.Y, 1.0, 1.0);
				}
			}
		}

		drawOverlay(g);

		// cleanup the additional pens
		for (int i = 0; i < ObjectData::BS_MAX; i++)
			delete buildStatePens[i];
	}

	bool QueryMap::OnDraw()
	{
		// create a cached bitmap
		bool just_updating = true;
		if (m_pCellBitmap == NULL)
		{
			just_updating	= false;
			m_last_updated	= 0;
			m_pCellBitmap	= new Bitmap(m_Width, m_Height);
			Graphics g(m_pCellBitmap);

			// fill it
			RectF		full_box(0.0F, 0.0F, (REAL)m_Width, (REAL)m_Height);
			SolidBrush	box_white(Color::White);
			g.FillRectangle(&box_white, full_box);
		}

		// pass to whatever
		Graphics g(m_pCellBitmap);

		// the draw mode
		if (view_type == QueryMap::VIEWTYPE_DOTS)
			drawDots(g);
		else if (view_type == QueryMap::VIEWTYPE_CELLS)
			drawCells(g);

		// overlay either way
		drawOverlay(g);
		m_Gdi->DrawImage(m_pCellBitmap, PointF(0, 0));
		drawSelectionBox(*m_Gdi);
		m_last_updated = time64(0);

		return true;
	}
}