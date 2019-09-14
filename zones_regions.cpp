#include "stdafx.h"
#include "awsdk_location.h"
#include "zone_regions.h"
#include "zones.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

namespace Zones
{
	void RegionList::push_back(Zones::Regions::Cylinder& Shape)
	{
		cylinders.push_back(Shape);
		regions.push_back(&cylinders.back());
	}

	void RegionList::push_back(Zones::Regions::Polygon& Shape)
	{
		polygons.push_back(Shape);
		regions.push_back(&polygons.back());
	}

	void RegionList::push_back(Zones::Regions::Rectangle& Shape)
	{
		rectangles.push_back(Shape);
		regions.push_back(&rectangles.back());
	}

	void RegionList::push_back(Zones::Regions::Sphere& Shape)
	{
		spheres.push_back(Shape);
		regions.push_back(&spheres.back());
	}

	void RegionList::clear(void)
	{
		/* erase the lists */
		cylinders	.clear();
		polygons	.clear();
		rectangles	.clear();
		spheres		.clear();

		/* erase the tracking vector */
		regions		.clear();
	}

	namespace Regions
	{
		Region::Region()
		{
			static int reg_id = 0;
			reg_id++;
			id = reg_id;
		}

		bool PointCollides(CPoint pt, CPoint hi, CPoint lo) ;

		// Rectangle Contains
		// 
		//
		bool Rectangle::Contains(const AW::Location& pt) const 
		{
			bool bx = pt.PointInRect(top, bottom);
			return bx;
		}

		CStringA Rectangle::getDescription()
		{ 
			CStringA about; 
			about.Format("hi=%s lo=%s", top.getCoordinates(), bottom.getCellCoordinates()); 
			return about; 
		}
		
		// Sphere Contains
		// 
		//

		bool Sphere::Contains(const AW::Location& pt) const 
		{
			return static_cast<float>(centre.distanceTo(pt)) < this->radius;
		}

		CStringA Sphere::getDescription()
		{ 
			CStringA about; 
			about.Format("centre=%s radius=%.1f", centre.getCoordinates(), (float)radius / 100.0); 
			return about; 
		}

		// Polygon Contains
		//
		//
		bool Polygon::Contains(const AW::Location& pt) const 
		{	
			/* simple bounary checker */
			if ((pt.getX() < low.x || pt.getX() > high.x || pt.getZ() < low.y || pt.getZ() > high.y)) 
				return false;

			/* variables for looping */
			AW::Location old			= verts[0];
			AW::Location next;
			int	hit_count	= 0;

			/* use first to check height */
			if (pt.getY() < base || pt.getY() > (base + height))
				return false;
			
			/* variables employed */
			CPoint	al(pt.getX(), pt.getZ());

			/* through each remaining vertex */
			for (size_t c = 1; c < verts.size(); c++) 
			{
				next = verts[c];		// it always feels good to type that
				/* use functional checker */
				if (PointCollides(al, CPoint(old.getX(), old.getZ()), CPoint(next.getX(), next.getZ())))
					hit_count++;
				/* make our first one our old one */
				old = next;
			}

			/* then do it from out start to our finish */
			AW::Location px = verts[0];
			CPoint old_point(old.getX(), old.getZ());
			CPoint new_point(px.getX(), px.getZ());
			if (PointCollides(al, old_point, new_point))
				hit_count++;

			/* print it out */
			return (hit_count % 2 == 1 ? true : false);
		}
		
		bool PointCollides(CPoint pt, CPoint hi, CPoint lo) 
		{
			bool	hiv = hi.y > pt.y;
			bool	lov = lo.y > pt.y;
			/* quick check - if they never cross over its never going to intercept */
			if (hiv == lov) 	
				return false;
			int		base_to_upper = (hi.y - pt.y);
			
			/* deltas for our code */
			int		delta_y		= (hi.y - lo.y);
			int		delta_x		= (hi.x - lo.x);
			int		offsety	= (pt.y - lo.y);

			/* calculate the approx ranger */
			double ratio		= (1.0 / delta_y) * offsety;	
			int	   collides		= (int)((delta_x * ratio) + lo.x);
			
			/* check collision */
			return (collides < pt.x);
		}

		void Polygon::AddPoint(int x, int z) 
		{
			if (verts.size() == 0) 
			{ 
				high.x = x;	
				high.y = z; 
				low.x = x;	
				low.y = z; 
			}
			else
			{
				/* is the point at a maxima */
				if (x > high.x)	high.x = x;
				if (z > high.y)	high.y = z;
				if (x < low.x)	low.x = x;
				if (z < low.y)	low.y = z;
			}

			/* append our new point object to the list */
			AW::Location lc (x, 0, z, 0);
			verts.push_back(lc);
		}

		void Polygon::AddPoint(const AW::Location& rhs) 
		{
			AddPoint(rhs.getX(), rhs.getZ());
		}

		CStringA Polygon::getDescription()
		{ 
			CStringA about; 
			about.Format("verts=%d", (int)verts.size()); 
			return about; 
		}

		// Cylinder Contains
		//
		//

		bool Cylinder::Contains(const AW::Location& pt) const
		{
			bool rad = (centre.distanceToGD(pt) <= radius);
			bool ht  = (pt.getY() >= centre.getY()) && (pt.getY() < (centre.getY() + height));
			return rad && ht;
		}

		CStringA Cylinder::getDescription()
		{ 
			CStringA about; 
			about.Format("centre=%s radius=%.1f", centre.getCoordinates(), (float)radius / 100.0); 
			return about; 
		}
	}
}