#pragma once

namespace Zones
{
	class Zone;

	namespace Regions
	{
		/* base for each type of region specification */
		class Region : public Umbra::RemoteClass
		{
		public:
			// tracking information
			int		 id;		// tracking key  (auto-increment)
			CStringA name;		// tracking name (optional)

			Region();
			virtual bool Contains(const AW::Location& pt) const = 0;
		};

		class Rectangle : public Region 
		{
		public:
			AW::Location top, bottom;

		public:
			Rectangle() { }
			bool Contains(const AW::Location& pt) const;
			CStringA getDescription();
		};
		
		class Polygon : public Region 
		{
		public:
			std::vector<AW::Location>	verts;
			tagPOINT	high, low;
			int			height, base;

		public:
			CStringA getDescription();
			void AddPoint(int x, int z);
			void AddPoint(const AW::Location& rhs);
			bool Contains(const AW::Location& pt) const;
		};

		class Sphere : public Region 
		{
		public:
			AW::Location	centre;
			int				radius;

		public:
			Sphere() { radius = 0; }
			CStringA getDescription();
			bool Contains(const AW::Location& pt) const;
		};

		class Cylinder : public Region 
		{
		public:
			AW::Location		centre;
			int					height, radius;

		public:
			Cylinder() { height = radius = 0; }
			CStringA getDescription();
			bool Contains(const AW::Location& pt) const;
		};
	}
};