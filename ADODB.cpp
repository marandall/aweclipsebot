#include "stdafx.h"
#include "ADODB_Connection.h"
#include "ADODB_Recordset.h"
#include "ADODB_SQLBuilder.h"

using namespace ADODB_LIB;

namespace ADODB
{
	inline void TESTHR(HRESULT x) {if FAILED(x) _com_issue_error(x);};

	/**
		Construction
		-----------------------------
	*/
	Connection::Connection() : m_bOK(FALSE) {
		try {
			TESTHR(CoInitialize(NULL));
			TESTHR(m_pIConn.CreateInstance(CLSID_Connection));
		}
		catch(_com_error e) {
			m_LastError = e.ErrorMessage();
			m_bOK = FALSE;
		}
		catch(...) {
			m_LastError = "Unable to open database connection";
			m_bOK = FALSE;
		}
	}

	Connection::~Connection() 
	{
		try { 
			if (IsOpen())
				m_pIConn->Close();
	
			// m_pIConn->Release();
			CoUninitialize();
		} 
		catch (...)
		{
		}
	}

	/**
		IsOpen
		-----------------------------
			- Checks that the connection is open
	*/
	BOOL Connection::IsOpen() {
		if(m_pIConn)
			return m_pIConn->GetState() != adStateClosed;
		return FALSE;
	}

	/**
		Execute
		-----------------------------
			- Run an SQL statement
	*/
	BOOL Connection::Execute(CString strSQL) {
		try {
			m_pIConn->Execute(_bstr_t(strSQL), NULL, adCmdText);  
		}
		catch(_com_error e) {
			String desc = e.Description();
			String err(e.ErrorMessage());
			WriteMsg(CStringA("DB_DEBUG:") + desc, RGB_RED, OT_DEBUG);
			m_bOK = FALSE;
			return FALSE;
		}
		return TRUE;
	}

	/**
		OpenQuery
		-----------------------------
			- Query an SQL statement
	*/
	BOOL Connection::Query(Recordset& RS, CString strSQL) {
		return RS.OpenPrivate(*this, strSQL);
	}

	BOOL Connection::Open(CString strOpen)
	{
		try {
			TESTHR(m_pIConn->Open(_bstr_t(strOpen), _bstr_t(""), _bstr_t(""), -1));
		}
		catch (_com_error e)
		{
			String err = e.Description();
			HRESULT h = e.Error();

			WriteMsg(err, RGB_RED, OT_MAIN);
			m_bOK = FALSE;
			return FALSE;
		}
		catch(...) {
			m_bOK = FALSE;
			return FALSE;
		}

		return TRUE;
	}






	Recordset::Recordset() : m_bOK(TRUE) {
		try { 
			TESTHR(CoInitialize(NULL));
			TESTHR(m_pRS.CreateInstance(CLSID_Recordset));
		}
		catch (...) {
			m_bOK = FALSE;
		}
	}

	Recordset::~Recordset(void)
	{
		try {
			if (IsOpen())
				m_pRS->Close();

			// m_pRS->Release();
			CoUninitialize();
		}
		catch(...) {
			ASSERT(TRUE);
		}
	}

	BOOL Recordset::OpenPrivate(Connection& ConPtr, CString strSQL)
	{
		try {
			HRESULT hr = this->m_pRS->Open(_bstr_t(strSQL), 
									ConPtr.m_pIConn.GetInterfacePtr(), 
									 adOpenDynamic, 
									 adLockOptimistic, 
									 adCmdText);
			TESTHR(hr);
			return TRUE;
		}
		catch(_com_error e) {
			return FALSE;
		}
		catch(...) {
			return FALSE;
		}

	}

	void Recordset::setField(String Name, String Text)
	{
		_variant_t vt = Name;
		m_pRS->Fields->GetItem(vt)->PutValue(_variant_t(Text));
	}

	BOOL Recordset::PerformAction(Actions doAction)
	{
		try {
			switch (doAction) {
			case doMoveNext:
				TESTHR(m_pRS->MoveNext());
				break;
			case doMoveFirst:
				TESTHR(m_pRS->MoveFirst());
				break;
			case doMoveLast:
				TESTHR(m_pRS->MoveLast());
				break;
			case doMovePrevious:
				TESTHR(m_pRS->MovePrevious());
				break;
			case doAddNew:
				TESTHR(m_pRS->AddNew());
				break;
			case doUpdate:
				TESTHR(m_pRS->Update());
				break;
			case doDelete:
				TESTHR(m_pRS->Delete(adAffectCurrent));
				break;
			};
		}
		catch(_com_error e) {
			return FALSE;
		}
		catch(...) {
			return FALSE;
		}  
		return TRUE;
	}

	CStringA Recordset::operator[](const char* KeyName)
	{
		_variant_t vtFld;
		CStringA convText;

		try
		{
			vtFld = m_pRS->Fields->GetItem(KeyName)->Value;
			switch(vtFld.vt) 
			{
			case VT_R4:
				convText.Format("%f", vtFld.fltVal); 
				return convText;

			case VT_R8:
				convText.Format("%f", vtFld.dblVal); 
				return convText;

			case VT_BSTR:
				return CStringA(vtFld.bstrVal);

			case VT_I2:
			case VT_UI1:
				convText.Format("%d", vtFld.iVal); 
				return convText;

			case VT_INT:
				convText.Format("%d", vtFld.intVal);
				return convText;

			case VT_I4:
				convText.Format("%d", vtFld.lVal);
				return convText;

			case VT_UI4:
				convText.Format("%u", vtFld.ulVal);
				return convText;

			default:
				return "";
			}
		}
		catch(...)
		{
			return "";
		}
	}










	void SQLBuilder::Insert(const char* KeyId, const char* Data) {
		CStringA text; text.Format("'%s'", Data);
		KeyPair pair = { KeyId, text };
		m_pairs.push_back(pair);
	}
	
	void SQLBuilder::Insert(const char* KeyId, int Data) {
		CStringA text; text.Format("%d", Data);
		KeyPair pair = { KeyId, text };
		m_pairs.push_back(pair);
	}

	void SQLBuilder::Insert(const char* KeyId, float Data) {
		CStringA text; text.Format("%f", Data);
		KeyPair pair = { KeyId, text };
		m_pairs.push_back(pair);		
	}

	CStringA SQLBuilder::CreateInsert(CStringA Table)
	{
		// if there is no data
		if (m_pairs.size() == 0)
			return "";

		CStringA cmdText;
		CStringA valText;
		for (int i = 0; i < ((int)m_pairs.size() - 1); i++) {
			cmdText.AppendFormat("%s, ", m_pairs[i].name);
			valText.AppendFormat("%s, ", m_pairs[i].value);
		}

		// add final value
		int i = (int)m_pairs.size() - 1;
		cmdText.AppendFormat("%s", m_pairs[i].name);
		valText.AppendFormat("%s", m_pairs[i].value);

		// build final result
		CStringA result;
		result.Format("INSERT INTO %s(%s) VALUES(%s)", (LPCSTR)Table, (LPCSTR)cmdText, (LPCSTR)valText);
		return result;
	}

	CStringA SQLBuilder::CreateUpdate(CStringA Table, CStringA Where)
	{
		// if there is no data
		if (m_pairs.size() == 0)
			return "";

		CStringA cmdText;
		for (int i = 0; i < ((int)m_pairs.size() - 1); i++)
			cmdText.AppendFormat("%s=%s, ", m_pairs[i].name, m_pairs[i].value);

		// add final value
		int i = (int)m_pairs.size() - 1;
		cmdText.AppendFormat("%s=%s", m_pairs[i].name, m_pairs[i].value);

		// build final result
		CStringA result;
		result.Format("UPDATE %s SET %s %s", (LPCSTR)Table, (LPCSTR)cmdText, (LPCSTR)Where);
		return result;
	}


}