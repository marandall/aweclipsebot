#include "stdafx.h"
#include "property_projects.h"

namespace Property
{
	// external reference for projects
	Projects g_Projects;

	Project* Projects::CreateProject(const char* ProjectId)
	{
		// project already exists
		if (Find(ProjectId))
			return NULL;

		// create new project, pop it in and return it
		Project pr;
		pr.id = ProjectId;
		m_list.push_back(pr);
		return Find(ProjectId);
	}

	bool Projects::Erase(const char* ProjectId)
	{
		for (iterator objPtr = begin(); objPtr != end(); objPtr++)
			if (objPtr->getId().CompareNoCase(ProjectId) == 0 && objPtr->isProtected() == false)
			{
				m_list.erase(objPtr);
				return true;
			}
		return false;
	}

	Project* Projects::Find(const char* ProjectId)
	{
		for (iterator objPtr = begin(); objPtr != end(); objPtr++)
			if (objPtr->getId().CompareNoCase(ProjectId) == 0)
				return &(*objPtr);
		return NULL;
	}

	Project* Projects::checkProjectPtr(Project* P)
	{
		for (iterator objPtr = begin(); objPtr != end(); objPtr++)
			if ((&*objPtr) == P)
				return P;
		return NULL;
	}

}