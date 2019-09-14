#pragma once
#include <vector>

namespace LayoutManager
{
	const int SEPERATORS  = 3;

	class LayoutManager 
	{
	public: // the children
		std::vector<LayoutManager*> children;
		int		standard_width, standard_height, align, border;
		CWnd*	pWnd;

	public: // constructor
		LayoutManager()
		{
			pWnd			= 0;
			standard_width	= 0;
			standard_height	= 0;
			align			= 0;
			border			= 0;
		}

		virtual ~LayoutManager()
		{
			for (size_t i = 0; i < children.size(); i++)
				delete children[i];
			children.clear();
		}

	public: // width and height setting
		int			getWidth		(int width)  const	{ return standard_width;		}
		int			getHeight		(int height) const	{ return standard_height;		}
		int			getAlign		(void) const		{ return align;					}
		int			getBorder		(void) const		{ return border;				}
		size_t		getChildCount	(void) const		{ return children.size();		}  

	public: // set size
		LayoutManager& assignWidth(int width)
		{
			standard_width = width;
			return *this;
		}

		LayoutManager& assignHeight(int height)
		{
			standard_height = height;
			return *this;
		}

		LayoutManager& assignSize(int width, int height)
		{
			standard_width	= width;
			standard_height	= height;
			return *this;
		}

		LayoutManager& assignBorder(int border)
		{
			this->border = border;
			return *this;
		}

	public: // add elements
		LayoutManager& add(LayoutManager& LM, int Align = 0)
		{
			children.push_back(&LM);
			LM.align = Align;
			return LM;
		}

		LayoutManager& add(CWnd& CW, int Align = 0)
		{
			LayoutManager* pLM = new LayoutManager();
			pLM->pWnd = &CW;
			pLM->align = Align;

			// calculate the rectangle from this
			RECT rt;
			CW.GetWindowRect(&rt);
			pLM->standard_width		= rt.right - rt.left;
			pLM->standard_height	= rt.bottom - rt.top;

			return add(*pLM, Align);
		}

	public: // run the layout element
		virtual void layout(int left, int top, int width, int height)
		{
			pWnd->MoveWindow(left, top, width, height);
		}

		void modify_layout(int& left, int& top, int& width, int& height)
		{
			// modify layout via border
			left+=		getBorder();
			width-=		getBorder() * 2;
			top+=		getBorder();
			height-=	getBorder() * 2;
		}
	};

	class GridLayout : 
		public LayoutManager
	{
	public: // layout tracking
		int rows, columns;

	public: // constructor
		GridLayout(int rows, int columns)
		{
			this->rows = rows;
			this->columns = columns;
		}

	public: // layout positions
		void layout(int left, int top, int width, int height)
		{
			// modify layout via border
			modify_layout(left, top, width, height);

			int sub_seperators	= (columns - 1) * SEPERATORS;
			int child_width		= static_cast<int>((width - sub_seperators) / (double)columns);
			
			int per_element = 0;
			int track_x = left;
			int track_y = top;
			int child_height = 50;

			// for each child element
			for (size_t i = 0; i < children.size(); i++)
			{
				// if we have gone past the columns
				size_t r = i % columns;
				if ((i % columns) == 0 && i != 0)
				{
					track_x = left;
					track_y+= (child_height + SEPERATORS);
					int j = 0;
				}

				// items of an element need to be aligned
				children[i]->layout(track_x, track_y, child_width, child_height);
				track_x+= (child_width + SEPERATORS);
			}
		}
	};

	class BorderLayout : 
		public LayoutManager
	{
	public: // layout
		enum
		{
			NOTHING = 0,
			PAGE_TOP,
			PAGE_BOTTOM,
			LEFT,
			RIGHT,
			CENTER
		};

	public: // layout manager
		void layout(int left, int top, int width, int height)
		{
			// modify layout via border
			modify_layout(left, top, width, height);

			int track_x			= left;
			int track_y			= top;
			int center_top		= top;
			int center_left		= 0;
			int center_height	= 0;

			// north elements
			for (size_t i = 0; i < children.size(); i++)
			{
				// if not using top alignment, jump out
				if (children[i]->getAlign() != PAGE_TOP)
					continue;

				// add item
				int child_height = children[i]->getHeight(height);
				children[i]->layout(track_x, track_y, width, child_height);
				track_y+= (child_height + SEPERATORS);
			}

			// top
			center_top = track_y;

			// southern elements
			int south_height = 0;
			for (size_t i = 0; i < children.size(); i++)
			{
				// if not using bottom alignment, jump out
				if (children[i]->getAlign() != PAGE_BOTTOM)
					continue;

				// add element
				south_height+= (children[i]->getHeight(height) + SEPERATORS);
			}

			// add southern elements
			track_y = height - south_height + SEPERATORS;
			int track_east_top = track_y;
			for (size_t i = 0; i < children.size(); i++)
			{
				// if not using bottom alignment, jump out
				if (children[i]->getAlign() != PAGE_BOTTOM)
					continue;

				// add item
				int child_height = children[i]->getHeight(height);
				children[i]->layout(track_x, track_y, width, child_height);
				track_y += (child_height + SEPERATORS); 
			}

			// reassign variables
			height			= height - center_top - south_height;
			center_height	= height;
			top				= center_top;
			track_y			= top;
			
			// add eastern elements
			track_x = left;
			int left_width = 0;
			for (size_t i = 0; i < children.size(); i++)
			{
				// if not using left alignment, jump out
				if (children[i]->getAlign() != LEFT)
					continue;

				// add item
				int child_width = children[i]->getWidth(width);
				children[i]->layout(track_x, track_y, child_width, height);
				track_x+= (child_width + SEPERATORS);
			}

			// top left position
			left_width  = track_x - left;
			center_top  = top;
			center_left = track_x;

			// western elements
			int west_width = 0;
			for (size_t i = 0; i < children.size(); i++)
			{
				// if not using bottom alignment, jump out
				if (children[i]->getAlign() != RIGHT)
					continue;

				// add element
				west_width+= (children[i]->getWidth(width) + SEPERATORS);
			}

			// the center width is the full width minus the east and west data
			int center_width = width - west_width - left_width;

			// add western elements
			track_x = width - west_width + SEPERATORS;
			for (size_t i = 0; i < children.size(); i++)
			{
				// if not using left alignment, jump out
				if (children[i]->getAlign() != RIGHT)
					continue;

				// add item
				int child_width = children[i]->getWidth(width);
				children[i]->layout(track_x, track_y, child_width, height);
				track_x+= (child_width + SEPERATORS);
			}

			// add a center point
			for (size_t i = 0; i < children.size(); i++)
			{
				// if not using left alignment, jump out
				if (children[i]->getAlign() != CENTER)
					continue;

				// add item
				children[i]->layout(center_left, center_top, center_width, height);
			}
		}
	};

	class TableLayout : 
		public LayoutManager
	{
	public: // tracking rows and columns
		struct ColumnInfo
		{
			int width;
		};

		struct Cell
		{
			LayoutManager* child;

			Cell()
			{
				child = 0;
			}
		};

		struct RowInfo
		{
			std::vector<Cell> columns;
		};

	public: // column tracking
		std::vector<ColumnInfo> columns;
		std::vector<RowInfo>	rows;

	public: // constructor
		TableLayout()
		{
		}

	public: // add a column
		void addColumn(int width)
		{
			ColumnInfo ci = { width } ;
			columns.push_back(ci);
		}

		RowInfo& getRow(int row)
		{
			if (row < (int)rows.size())
			{
				return rows[row];
			}
			else
			{
				// add any additional items
				for (size_t i = rows.size(); i < (size_t)row + 1; i++)
					rows.push_back(RowInfo());

				// assign a full list
				rows[row].columns = std::vector<Cell>(columns.size());	
				return rows[row];
			}
		}

	public: // ad element
		LayoutManager& add(LayoutManager& LM, int Row, int Column)
		{
			LayoutManager* pLayout = &LayoutManager::add(LM, 0);
			getRow(Row).columns[Column].child = pLayout;
			return *pLayout;
		}

		LayoutManager& add(CWnd& CW, int Row, int Column)
		{
			LayoutManager* pLayout = &LayoutManager::add(CW, 0);
			getRow(Row).columns[Column].child = pLayout;
			return *pLayout;
		}

		TableLayout& addToRow(int Row, CWnd& C1, CWnd& C2)
		{
			add(C1, Row, 0);
			add(C2, Row, 1);
			return *this;
		}

	public: // layout manager
		void layout(int left, int top, int width, int height)
		{
			// modify layout via border
			modify_layout(left, top, width, height);

			int row_height = 20;
			int track_x = left;
			int track_y = top;

			// go through each cell
			for (size_t i = 0; i < rows.size(); i++)
			{
				// each cell
				for (size_t c = 0; c < columns.size() - 1; c++)
				{
					// skip if there is no assigned control
					if (rows[i].columns[c].child == NULL)
						continue;

					// check the length
					rows[i].columns[c].child->layout(track_x, track_y, columns[c].width, row_height);

					// add an extra rows
					track_x+= columns[c].width + SEPERATORS;
				}

				// wrap the last cell
				rows[i].columns[columns.size() - 1].child->layout(track_x, track_y, width - track_x, row_height);

				// track the y
				track_x = left;
				track_y+= row_height + SEPERATORS;
			}
		}
	};
}