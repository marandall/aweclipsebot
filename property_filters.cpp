#include "stdafx.h"
#include "property_filters.h"
#include "property_object.h"

String temp;

namespace Property
{
	namespace Filters
	{
		// onUpdated
		//   splits the citizen list into numbers
		// 
		bool Citizens::onUpdated()
		{
			// reset citizen list
			check_list.clear();

			// split by comma
			SplitString tokens(citizens, ",", -1);
			for (size_t i = 0; i < tokens.size(); i++)
			{
				temp = tokens[i]; temp.Trim();
				check_list.push_back(atoi(temp));
			}

			return true;
		}

		bool Inequality::onUpdated()
		{
			bound_x = Location(getX());
			bound_y = Location(getY());
			bound_z = Location(getZ());
			return true;
		}

		bool MoverBounds::onUpdated()
		{
			return true;
		}

		bool ParticleBounds::onUpdated()
		{
			return true;
		}

		bool SelectionRect::onUpdated()
		{
			bound_hi	= Location(getTop()).getThisLocation();
			bound_low	= Location(getBottom()).getThisLocation();
			return true;
		}

		bool StringFinder::onUpdated()
		{
			return true;
		}

		bool TypeFinder::onUpdated()
		{
			// build the type checks
			type_checks[AW_OBJECT_TYPE_V3			] = getV3();
			type_checks[AW_OBJECT_TYPE_UNKNOWN		] = false;
			type_checks[AW_OBJECT_TYPE_ZONE			] = getZones();
			type_checks[AW_OBJECT_TYPE_PARTICLES	] = getParticles();
			type_checks[AW_OBJECT_TYPE_MOVER		] = getMovers();
			type_checks[AW_OBJECT_TYPE_CAMERA		] = getCameras();
			return true;
		}

		bool UserType::onUpdated()
		{
			return true;
		}

		bool ZoneBounds::onUpdated()
		{
			return true;
		}

		bool Zones::onUpdated()
		{
			return true;
		}

		// filter operations
		bool Citizens::run(ObjectData& Object)
		{
			// owner must be in the check list
			for (size_t i = 0; i < check_list.size(); i++)
				if (Object.getOwner() == check_list[i])
					return true;

			// this is not in the list
			return false;
		}

		bool Inequality::run(ObjectData& Object)
		{
			bool so_far = true;

			// check when expecting something greater than the bound
			if (getGreaterX() == B_TRUE && Object.getX() < bound_x.getX()) so_far = false;
			if (getGreaterY() == B_TRUE && Object.getY() < bound_y.getY()) so_far = false;
			if (getGreaterZ() == B_TRUE && Object.getZ() < bound_z.getZ()) so_far = false;
			
			// check when expecting something less than the bound
			if (getGreaterX() == B_FALSE && Object.getX() > bound_x.getX()) so_far = false;
			if (getGreaterY() == B_FALSE && Object.getY() > bound_y.getY()) so_far = false;
			if (getGreaterZ() == B_FALSE && Object.getZ() > bound_z.getZ()) so_far = false;
			
			// everything we have checked so far
			return so_far;
		}

		bool MoverBounds::run(ObjectData& Object)
		{
			return true;
		}

		float vector_max(::aw_type_vector& V)
		{
			return max(V.x, max(V.y, V.z));
		}

		bool ParticleBounds::run(ObjectData& Object)
		{
			// is it a particle
			if (Object.isParticle() == false)
				return false;

			// convert to particle
			::aw_object_data_particles* p = Object.ToParticlesData(); 
			if (p == 0)
				return false;

			// check particle size
			if (vector_max(p->size.min) > max_plane_size || vector_max(p->size.max) > max_plane_size)
				return true;

			// not found
			return false;
		}

		bool SelectionRect::run(ObjectData& Object)
		{
			return Object.PointInRect(bound_hi, bound_low);
		}

		bool StringFinder::run(ObjectData& Object)
		{
			// the key actions
			String model  = Object.getModel();
			String action = Object.getAction();
			String descr	 = Object.getDescription();
			String test   = string;

			// case insensitive causes all of the checks to be dropped to lower case
			if (getCaseSensitive() == false)
			{
				model. MakeLower();
				action.MakeLower();
				descr. MakeLower();
				test.  MakeLower();
			}

			// carry out checks on strings
			if (getInModel() && model.Find(test) != -1)
				return true;
			else if (getInAction() && action.Find(test) != -1)
				return true;
			else if (getInDescription() && descr.Find(test) != -1)
				return true;

			// was not found in any of the strings
			return false;
		}

		bool TypeFinder::run(ObjectData& Object)
		{
			return type_checks[Object.getType()];
		}

		bool UserType::run(ObjectData& Object)
		{
			return true;
		}

		bool ZoneBounds::run(ObjectData& Object)
		{
			// is this a zone
			if (Object.isZone() == false)
				return false;

			// check the size
			if (max_plane_size == 0)
			{
				return false;
			}
			else
			{
				return Object.ToZoneData()->size.x > getMaxPlaneSize() || Object.ToZoneData()->size.z > getMaxPlaneSize();
			}
		}

		bool Zones::run(ObjectData& Object)
		{
			return true;
		}

		bool Sector::run(ObjectData& Object)
		{
			return (Object.getSectorX() == sector_x && Object.getSectorZ() == sector_z);
		}
	}
}