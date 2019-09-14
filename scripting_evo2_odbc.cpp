#include "stdafx.h"
#include "scripting_evo2_odbc.h"

#pragma once
#include "afxdb.h"
#include "umbra/umbra_remoteclass.h"

namespace ScriptingLib
{
	//
	// Open a connection to a database via any odbc connection string
	//

	/*

	bool OdbcDatabaseImpl::open(String ConnectionString, DWORD Options)
	{
		return _database.OpenEx(ConnectionString, Options) == TRUE;
	}

	//
	// Execute any specific sql string
	//

	bool OdbcDatabaseImpl::execute(String Sql)
	{
		// possible failure on execution
		try
		{
			_database.ExecuteSQL(Sql);
			return true;
		}

		// handling of error from ExecuteSQL
		catch (CDBException& e)
		{
			_last_error = e.m_strError;
			_last_errno = e.m_nRetCode;
			return false;
		}
	}

	//
	// Get a new recordset object
	//

	OdbcRecordsetImpl* OdbcDatabaseImpl::query(String Sql)
	{
		OdbcRecordsetImpl* rs = new OdbcRecordsetImpl(this);
		BOOL ret = rs->_rs.Open((UINT)-1, Sql, CRecordset::none);

		// failure to open recordset
		if (ret == 0)
		{
			delete rs;
			return null;
		}
		else
		{
			return rs;
		}
	} 

	
	//
	// Recordset constructor
	//

	OdbcRecordsetImpl::OdbcRecordsetImpl(Database* Database) 
		: _rs(Database->_database)
	{
	}

	//
	// Row Navigation
	//

	bool OdbcRecordsetImpl::MoveFirst()
	{
		try 
		{
			_rs.MoveFirst();
			return true;
		}
		catch (CDBException& Err)
		{
			return false;
		}
	}

	//
	// Row Navigation
	//

	bool OdbcRecordsetImpl::MoveLast()
	{
		try 
		{
			_rs.MoveLast();
			return true;
		}
		else (CDBException& Err)
		{
			return false;
		}
	}

	//
	// Row Navigation
	//

	bool OdbcRecordsetImpl::MoveNext()
	{
		try 
		{
			_rs.MoveNext();
			return true;
		}
		else (CDBException& Err)
		{
			return false;
		}
	}

	//
	// Row Navigation
	//

	bool OdbcRecordsetImpl::MovePrev()
	{
		try 
		{
			_rs.MovePrev();
			return true;
		}
		else (CDBException& Err)
		{
			return false;
		}
	}

	//
	// Field Reader
	//

	String OdbcRecordsetImpl::getField(String FieldID)
	{
		// get as variant style
		CDBVariant var;
		String	   ret;

		// select the variable
		_rs.GetFieldValue(FieldID, ret);
		return ret;
	}

	//
	// Field Reader
	//

	int	OdbcRecordsetImpl::getIntField(String FieldID)
	{
		return atoi(getField(FieldID));
	}

	//
	// Field Reader
	//

	double OdbcRecordsetImpl::getFloatField(String FieldID)
	{
		return atof(getField(FieldID));
	}*/
}