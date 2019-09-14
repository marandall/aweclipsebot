#include "stdafx.h"
#include "property_editors.h"
#include "property_object.h"
#include "property_objects.h"

namespace Property
{
	namespace Editors
	{
		bool Editor::run(Property::Objects& Objects) 
		{ 
			bool changed = false;
			ObjectData od;

			for (Property::Objects::iterator ptr = Objects.begin(); ptr != Objects.end(); ptr++)
			{
				od = *ptr;
				bool chg = run(*ptr); 
				if (chg)
				{
					Objects.update_trackers(od);
					Objects.update_trackers(*ptr);
				}

				changed = chg ? true : changed;
			}

			// rebuild trackers as required
			if (changed) Objects.rebuild_trackers();
			return changed; 
		}

		bool Duplicate::run(Property::Objects& Objects)
		{
			// copy all of the objects to copy
			Property::Objects existing;
			for (Property::Objects::iterator ptr = Objects.begin(); ptr != Objects.end(); )
			{
				if (ptr->getSelected())
				{
					existing.insert_object(*ptr);
					// possibility of removing the existing object
					if (getDeleteOrigional() == true)
					{
						Objects.update_trackers(*ptr);
						ptr = Objects.erase(ptr);
					}
					else
					{
						ptr++;
					}
				}
				else
				{
					ptr++;
				}
			}

			// translate coordinates
			Location la(transform_a), lb(transform_b);

			// duplicate it x times
			for (int a = 0; a < repeat_a; a++)
			{
				for (int b = 0; b < repeat_b; b++)
				{
					// skip when a and b are zero
					if (a == 0 && b == 0)
						continue;

					// iterate each of the pre-defined obejcts
					for (Property::Objects::iterator ptr = existing.begin(); 
						ptr != existing.end(); 
						ptr++)
					{
						ObjectData obj = *ptr;

						// the new coords and shift our copy
						int x = (a * la.getX()) + (b * lb.getX());
						int y = (a * la.getY()) + (b * lb.getY());
						int z = (a * la.getZ()) + (b * lb.getZ());
						obj.ShiftXYZ(x, y, z);
						obj.timestamp = 0;

						// add to data
						Objects.update_trackers(obj);
						Objects.insert_object(obj);
					}
				}
			}

			// re-index
			Objects.rebuild_trackers();

			// all good
			return true;
		}

		bool DeleteSelected::run(Property::Objects& Objects)
		{
			Objects.deleteSelected(true);
			return true;
		}

		bool Crop::run(Property::Objects& Objects)
		{
			Objects.deleteSelected(false);
			return true;
		}

		bool StringReplace::run(ObjectData& Data)
		{
			// why exactly are we searching?
			if (getSearchAction() == false && getSearchModel() == false && getSearchDescription() == false)
				return false;

			// checking
			bool changed = false;

			// string replacements
			if (getSearchModel() == true) {
				Data.model.Replace(getFind(), getReplacement());
				changed = true;
			}

			// description
			if (getSearchDescription() == true) {
				Data.description.Replace(getFind(), getReplacement());
				changed = true;
			}

			// action
			if (getSearchAction() == true) {
				Data.action.Replace(getFind(), getReplacement());
				changed = true;
			}

			// has it changed?
			return changed;
		}

		bool ReplaceField::run(ObjectData& Data)
		{
			bool edit = false;

			if (do_model == true) {
				Data.model = new_model;
				edit = true;
			}

			if (do_description == true) {
				Data.description = new_description;
				edit = true;
			}

			if (do_action == true) {
				Data.action = new_action;
				edit = true;
			}

			return edit;
		}

		bool Translate::onUpdated()
		{
			Location lo(origin);
			Location ld(destination);

			// converstion
			offset_result = lo.CalculateOffset(ld);
			return true;
		}

		bool Translate::run(ObjectData& Data)
		{
			if (Data.getSelected() == true)
				Data.ShiftXYZ(offset_result.getX(), offset_result.getY(), offset_result.getZ());
			return Data.getSelected();
		}

		bool QueueSelected::run(ObjectData& Data)
		{
			if (Data.getSelected()) {
				Data.changeBuildState(ObjectData::BS_QUEUED);
				return true;
			} else {
				return false;
			}
		}

		bool QueueEverything::run(ObjectData& Data)
		{
			Data.changeBuildState(ObjectData::BS_QUEUED);
			return true;
		}

		bool StripRwx::run(ObjectData& Data)
		{
			if (Data.model.Right(4) == ".rwx")
			{
				Data.model = Data.model.Mid(0, Data.model.GetLength() - 4);
				return true;
			}
			else
			{
				return false;
			}
		}

		bool AppendRwx::run(ObjectData& Data)
		{
			if (Data.model.Right(4) != ".rwx")
			{
				Data.model = Data.model + ".rwx";
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}