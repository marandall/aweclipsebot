#pragma once

#pragma warning(disable:4146)
#import "c:\program files\common files\system\ado\msado15.dll" rename_namespace("ADODB_LIB") named_guids rename("EOF", "bEOF") rename("BOF", "bBOF")
#pragma warning(default:4146)

namespace ADODB
{
	class Recordset;
	class Connection
	{
		friend class Recordset;

	protected:
		ADODB_LIB::_ConnectionPtr	m_pIConn;	// connection pointer
		CStringA		m_LastError;			// last error string
		BOOL			m_bOK;					// is there an error

	public:
		Connection		();
		~Connection		();
		BOOL Open		(CString strOpen);
		BOOL IsOpen		();
		BOOL Execute	(CString strSQL);
		BOOL Query		(Recordset& RS, CString strSQL);
		BOOL Close		() { m_pIConn->Close(); return TRUE; }
	};
}