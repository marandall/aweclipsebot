#include "stdafx.h"
#include "zones.h"
#include "tsb.h"

namespace Zones
{
	errno_t	Zone::save(String Path)
	{
		Serializer sz;
		serializedSave(sz);

		// this tracking id
		sz["header"]["root_id"] = ToString(tracking_id);
		return sz.Save(Path) ? 1 : 0;
	}

	errno_t	Zone::load(String Path)
	{
		Serializer sz;
		sz.Load(Path);
		
		// erase the current list
		Erase();
		
		// load the routines
		serializedLoad(sz, atoi(sz["header"]["root_id"])) ? 1 : 0;
		id = ".";
		
		// update everything for this zone
		update_after_changes();
		return 0;
	}

	bool Zone::serializedLoad(Serializer& SZ, int ZoneID)
	{
		// setup query
		String z_id = "zone" + ToString(ZoneID);

		// load from the rights data
		Zones::AutoTemplates::ZoneData::Load(SZ, z_id);

		// load the areas
		int area_count		= SZ.Read(z_id, "area_count", 0);
		for (int i = 0; i < area_count; i++)
		{
			String a_id = "area" + ToString(i);
			String type = SZ.Read(z_id, a_id + "type", "");
			
			// handle rectangle
			if (type == "rect")
			{
				AW::Coordinates crd_top, crd_bottom;	
				crd_top		.FromString(SZ.Read(z_id, a_id + "hi"));
				crd_bottom	.FromString(SZ.Read(z_id, a_id + "lo"));
				
				/* create a new rectangle and add it to the grid*/
				Regions::Rectangle rect;
				rect.name	= SZ.Read(z_id, a_id + "name");
				rect.top	= crd_top;
				rect.bottom = crd_bottom;
				regions.push_back(rect);
			}

			// handle sphere
			else if (type == "sphere")
			{
				// convert coordinates
				AW::Coordinates coords;
				coords.FromString(SZ.Read(z_id, a_id + "center"));

				// build structure
				Regions::Sphere sphere;
				sphere.name		= SZ.Read(z_id, a_id + "name");
				sphere.centre	= coords;
				sphere.radius	= SZ.Read(z_id, a_id + "radius", 0);
				regions.push_back(sphere);
			}

			// handle cylinder
			else if (type == "cylinder")
			{
				// convert coordinates
				AW::Coordinates coords;
				coords.FromString(SZ.Read(z_id, a_id + "base"));

				Regions::Cylinder cyl; 
				cyl.centre	= coords;
				cyl.name	= SZ.Read(z_id, a_id + "name");
				cyl.height	= SZ.Read(z_id, a_id + "height", 0);
				cyl.radius	= SZ.Read(z_id, a_id + "radius", 0);
				regions.push_back(cyl);
			}

			// handle polygon
			else if (type == "polygon")
			{
				Regions::Polygon polygon;

				// read the base elevation
				AW::Coordinates coords;
				coords.FromString(SZ.Read(z_id, a_id + "base"));
				
				// read the height of the polygon
				polygon.height	= SZ.Read(z_id, a_id + "height", 0);
				polygon.base	= SZ.Read(z_id, a_id + "base", 0);
				polygon.name	= SZ.Read(z_id, a_id + "name");

				// vertex count
				int vert_count = SZ.Read(z_id, a_id + "vert_count", 0);
				for (int j = 0; j < vert_count; j++)
				{
					String p_id = a_id + "vert" + ToString(j);

					// vertex positions
					String s2 = SZ.Read(z_id, p_id);
					coords.FromString(s2);
					polygon.AddPoint(coords.getX(), coords.getZ());
				}

				// push to tail
				regions.push_back(polygon);
			}
		}

		// handle children
		int children_count = SZ.Read(z_id, "children_count", 0);
		for (int i = 0; i < children_count; i++)
		{
			String c_id = "child" + ToString(i);
			Zone child;
			child.parent_zone = this;
			child.serializedLoad(SZ, SZ.Read(z_id, c_id, -1));
			children.push_back(child);
		}

		return true;
	}

	String ToAltitude(int Elev)
	{
		String x;
		x.Format("%d", Elev);
		return x;
	}

	bool Zone::serializedSave(Serializer& SZ)
	{
		// setup query
		String z_id = "zone" + ToString(tracking_id);

		// load from the rights data
		Zones::AutoTemplates::ZoneData::Save(SZ, z_id);

		// write the regions
		SZ[z_id]["area_count"			] = ToString(regions.GetCount());
		int area_id = 0;

		// write rectangles
		for (size_t i = 0; i < regions.rectangles.size(); i++)
		{
			Zones::Regions::Rectangle& r = regions.rectangles[i];
			String a_id = "area" + ToString(area_id++);
			SZ[z_id][a_id + "type"		] = "rect";
			SZ[z_id][a_id + "name"		] = r.name;
			SZ[z_id][a_id + "hi"		] = r.top.getCoordinates();
			SZ[z_id][a_id + "lo"		] = r.bottom.getCoordinates();
		}

		// write sphere
		for (size_t i = 0; i < regions.spheres.size(); i++)
		{
			Zones::Regions::Sphere& r = regions.spheres[i];
			String a_id = "area" + ToString(area_id++);
			SZ[z_id][a_id + "type"		] = "sphere";
			SZ[z_id][a_id + "name"		] = r.name;
			SZ[z_id][a_id + "center"	] = r.centre.getCoordinates();
			SZ[z_id][a_id + "radius"	] = ToString(r.radius);
		}

		// write cylinder
		for (size_t i = 0; i < regions.cylinders.size(); i++)
		{
			Zones::Regions::Cylinder& r = regions.cylinders[i];
			String a_id = "area" + ToString(area_id++);
			SZ[z_id][a_id + "type"		] = "cylinder";
			SZ[z_id][a_id + "name"		] = r.name;
			SZ[z_id][a_id + "base"		] = r.centre.getCoordinates();
			SZ[z_id][a_id + "radius"	] = ToString(r.radius);
			SZ[z_id][a_id + "height"	] = ToAltitude(r.height);
		}

		// write polygon
		for (size_t i = 0; i < regions.polygons.size(); i++)
		{
			Zones::Regions::Polygon& r = regions.polygons[i];
			String a_id = "area" + ToString(area_id++);
			SZ[z_id][a_id + "type"		] = "polygon";
			SZ[z_id][a_id + "name"		] = r.name;
			SZ[z_id][a_id + "base"		] = ToString(r.base);
			SZ[z_id][a_id + "height"	] = ToString(r.height);
			SZ[z_id][a_id + "vert_count"] = ToString(r.verts.size());

			// write each vertex
			for (size_t j = 0; j < r.verts.size(); j++)
			{
				String p_id = a_id + "vert" + ToString(j);
				SZ[z_id][p_id			] = r.verts[j].getCoordinates();
			}
		}

		// write children
		int child_index = 0;
		SZ[z_id]["children_count"		] = ToString(children.size());
		for (ZoneList::iterator itr = children.begin(); 
			itr != children.end();
			itr++)
		{
			String c_id = "child" + ToString(child_index++);
			SZ[z_id][c_id				] = ToString(itr->tracking_id);
			itr->serializedSave(SZ);
		}
			
		return true;
	}
}