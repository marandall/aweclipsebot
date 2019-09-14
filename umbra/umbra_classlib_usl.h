#pragma once
#include "umbra_remoteclass.h"
#include "umbra_classlib.h"
#include "umbra_classlib_usl_array.h"
#include "umbra_classlib_usl_vector.h"
#include "umbra_classlib_usl_file.h"
#include "umbra_classlib_usl_colourlist.h"
#include "umbra_classlib_usl_datetime.h"
#include "umbra_classlib_usl_fileinfo.h"
#include "umbra_classlib_usl_adodb.h"

// UMBRA_NAMESPACE: Win32.Gdi
namespace Umbra { namespace ClassLib { namespace Usl { 

	class D_Usl : public Umbra::RemoteClass
	{
	protected: // the constant defenitions
		ColourListClass		_colourlist;
		
	protected: // usl.io
		FileClass					_file;
		DateTimeClass			_datetime;
		FileInfoClass			_fileinfo;

	protected: // usl.collections
		VectorClass				_vector;
		ArrayClass				_array;

	protected: // usl.database
		AdodbConnectionClass		_adodbconnection;

	public: // interface access
		bool doUmbraProperty(Variant& V, PropertyRequest& R)
		{
			UMBRA_PROPERTY_R(File,				_file);
			UMBRA_PROPERTY_R(FileInfo,		_fileinfo);
			UMBRA_PROPERTY_R(DateTime,		_datetime);
			UMBRA_PROPERTY_R(Vector,			_vector);
			UMBRA_PROPERTY_R(Array,				_array);

			// usl.graphics
			UMBRA_PROPERTY_R(ColourList,		_colourlist);
			UMBRA_PROPERTY_R(ColorList,			_colourlist);

			// usl.database
			UMBRA_PROPERTY_R(AdodbConnection,	_adodbconnection);

			// nothing
			return R.NOT_HANDLED;
		}

		bool doUmbraFunction(Variant& V, FunctionRequest& R);

	public: // static instance
		static D_Usl& getInst() { static D_Usl ns; return ns; }
	};

} } }