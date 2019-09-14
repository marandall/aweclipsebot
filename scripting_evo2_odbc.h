#pragma once
#include "afxdb.h"
#include "umbra/umbra_remoteclass.h"

namespace ScriptingLib
{
	class OdbcRecordsetImpl;

	class OdbcDatabaseImpl : public Umbra::RemoteClass
	{
		friend class OdbcRecordsetImpl;

	protected: // database object
		CDatabase	_database;
		String		_last_error;
		int			_last_errno;

	protected: // recordset following
		std::vector<OdbcRecordsetImpl*> _recordsets;

	public: // connection routines
		bool				open(String ConnectionString, DWORD Options);
		bool				execute(String Sql);
		OdbcRecordsetImpl*	query();
	};

	class OdbcField
	{
	public: // parameters
		bool	dirty;
		DWORD	type;
		String	value;
	};

	class OdbcRow
	{
	public: // row information
		typedef std::map<String, OdbcField>	FieldMap;
		FieldMap fields;
	};

	class OdbcRecordsetImpl : public Umbra::RemoteClass
	{
		friend class OdbcDatabaseImpl;

	protected: // database object
		CRecordset				_rs;
		OdbcDatabaseImpl*		_database;
		std::vector<OdbcRow>	_rows;

	public: // constructor
		OdbcRecordsetImpl(OdbcDatabaseImpl* Database);

	public: // field read
		String		getField(String FieldID);
		int			getIntField(String FieldID);
		double		getFloatField(String FieldID);
	};
}