#pragma once

#pragma warning(disable:4146)
#import "c:\program files\common files\system\ado\msado15.dll" rename_namespace("ADODB_LIB") named_guids rename("EOF", "bEOF") rename("BOF", "bBOF")
#pragma warning(default:4146)

namespace ADODB
{
	class Connection;
	class Recordset
	{
		enum Actions
		{
			doMoveFirst,
			doMoveLast,
			doMoveNext,
			doMovePrevious,
			doAddNew,
			doUpdate,
			doDelete
		};

	public:
		ADODB_LIB::_RecordsetPtr m_pRS;		// database pointer
		BOOL			m_bOK;				// any error
		Connection*		m_pCon;

	public:
		Recordset		(void);
		~Recordset		(void);
		BOOL OpenPrivate(Connection& ConPtr, CString strSQL);
	
	public: // navigation and actions
		BOOL PerformAction (Actions doAction);
		BOOL MoveNext	() { return PerformAction(doMoveNext);	}
		BOOL MoveFirst	() { return PerformAction(doMoveFirst); }
		BOOL MoveLast	() { return PerformAction(doMoveLast);	}
		BOOL MovePrevious(){ return PerformAction(doMovePrevious); }
		BOOL AddNew		() { return PerformAction(doAddNew);	}
		BOOL Update		() { return PerformAction(doUpdate);	}
		BOOL Delete		() { return PerformAction(doDelete);	}
		
	public: // reading
		CStringA operator[](const char* KeyName);

	public: // set the options
		void setField(String Name, String Text);

	public: // quick looping tests
		inline BOOL IsEOF()  { return m_pRS->bEOF == VARIANT_TRUE; }
		inline BOOL IsOpen() { return m_pRS->GetState() != ADODB_LIB::adStateClosed; }
		Recordset& Close()   { if (IsOpen()) m_pRS->Close(); return *this; }
	};
}