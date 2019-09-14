#pragma once

// include the sub properties
#include "property_filters_citizens_ag.h"
#include "property_filters_inequality_ag.h"
#include "property_filters_moverbounds_ag.h"
#include "property_filters_particlebounds_ag.h"
#include "property_filters_selectionrect_ag.h"
#include "property_filters_stringfinder_ag.h"
#include "property_filters_typefinder_ag.h"
#include "property_filters_usertype_ag.h"
#include "property_filters_zonebounds_ag.h"
#include "property_filters_zones_ag.h"

// standard requirements

namespace Property
{
	namespace Filters
	{
		class Filter
		{
		public: // update overrides
			Filter() { }
			virtual bool run(ObjectData& Object) = 0;
			virtual bool onUpdated() = 0;

		public: // umbra override
			bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
			{
				// update the internal properties
				UMBRA_FUNCTION_BEGIN(prepare) {
					// code information break
					UMBRA_FUNCTION_CS() {
						UMBRA_FUNCTION_RET( onUpdated() );
					} UMBRA_FUNCTION_CE();
				} UMBRA_FUNCTION_END();

				// not a function
				return Umbra::FunctionRequest::NOT_HANDLED;
			}
		};

		class Citizens : 
			public Filter,
			public AutoTemplates::CitizensData
		{
		protected: // additional information
			std::vector<int> check_list;

		protected: // update overrides
			bool onUpdated();
			bool run(ObjectData& Object);
		};

		class Inequality : 
			public Filter,
			public AutoTemplates::InequalityData
		{
		protected: // additional information
			Location bound_x;
			Location bound_y;
			Location bound_z;

		protected: // update overrides
			bool onUpdated();
			bool run(ObjectData& Object);
		};

		class MoverBounds : 
			public Filter,
			public AutoTemplates::MoverBoundsData
		{
		protected: // update overrides
			bool onUpdated();
			bool run(ObjectData& Object);
		};

		class ParticleBounds : 
			public Filter,
			public AutoTemplates::ParticleBoundsData
		{
		protected: // update overrides
			bool onUpdated();
			bool run(ObjectData& Object);
		};

		class SelectionRect : 
			public Filter,
			public AutoTemplates::SelectionRectData
		{
		public: // additional information
			AW::Location bound_hi;
			AW::Location bound_low;

		protected: // update overrides
			bool onUpdated();

		public: // the execution
			bool run(ObjectData& Object);
		};

		class StringFinder : 
			public Filter,
			public AutoTemplates::StringFinderData
		{
		protected: // update overrides
			bool onUpdated();
			bool run(ObjectData& Object);
		};

		class TypeFinder : 
			public Filter, 
			public AutoTemplates::TypeFinderData
		{
		protected: // additional information
			bool type_checks[AW_MAX_OBJECT_TYPES];

		protected: // update overrides
			bool onUpdated();
			bool run(ObjectData& Object);
		};

		class UserType : 
			public Filter,
			public AutoTemplates::UserTypeData
		{
		protected: // update overrides
			bool onUpdated();
			bool run(ObjectData& Object);
		};

		class ZoneBounds : 
			public Filter,
			public AutoTemplates::ZoneBoundsData
		{
		protected: // update overrides
			bool onUpdated();
			bool run(ObjectData& Object);
		};

		class Zones : 
			public Filter,
			public AutoTemplates::ZonesData
		{
		protected: // update overrides
			bool onUpdated();
			bool run(ObjectData& Object);
		};

		class Sector: 
			public Filter
		{
		public: // sector list
			int	sector_x;
			int sector_z;
		protected:
			bool onUpdated() { return true; }
			bool run(ObjectData& Object);
		};
	}
}