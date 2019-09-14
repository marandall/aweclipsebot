#include "stdafx.h"
#include "umbra_classlib_usl_adodb.h"

// use the adodb library
using namespace ADODB_LIB;

namespace Umbra { namespace ClassLib { namespace Usl {

	inline void TESTHR(HRESULT x) 
	{
		if (FAILED(x) == true)
		{
			_com_issue_error(x);
		}
	}

	//
	// Constructor
	//

	Connection::Connection() 
	{
		try 
		{
			TESTHR(CoInitialize(NULL));
			TESTHR(m_pIConn.CreateInstance(CLSID_Connection));
		}
		catch(_com_error e) 
		{
			last_error = e.ErrorMessage();
			last_errno = -1;
		}
		catch(...) 
		{
			last_error = "Unable to open database connection";
			last_errno = 0;
		}
	}

	Connection::~Connection() 
	{
		try 
		{ 
			if (isOpen() == true)
			{
				m_pIConn->Close();
			}

			CoUninitialize();
		} 
		catch (...)
		{
		}
	}

	//
	// Connection State Checking
	//

	bool Connection::isOpen() 
	{
		if(m_pIConn)
		{
			return m_pIConn->GetState() != adStateClosed;
		}
		else
		{
			return false;
		}
	}

	//
	// Execution mechanisms
	//

	bool Connection::execute(std::string Sql) 
	{
		try 
		{
			m_pIConn->Execute(_bstr_t(Sql.c_str()), NULL, adCmdText);  
			return true;
		}
		catch(_com_error e) 
		{
			last_error = e.ErrorMessage();
			last_errno = -1;
			return false;
		}
	}

	bool Connection::query(Recordset& RS, std::string Sql) 
	{
		return RS.OpenPrivate(*this, Sql);
	}

	//
	// Connection Establishing and Maintainance
	//

	bool Connection::open(std::string CommandString)
	{
		try 
		{
			TESTHR(m_pIConn->Open(_bstr_t(CommandString.c_str()), _bstr_t(""), _bstr_t(""), -1));
			return true;
		}
		catch(_com_error e) 
		{
			last_error = e.ErrorMessage();
			last_errno = -1;
			return false;
		}
		catch(...) 
		{
			return false;
		}
	}

	bool Connection::close(void)
	{
		m_pIConn->Close(); 
		return true; 
	}

	//
	// Connection Control
	//

	bool Connection::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		// open the file database
		//
		
		UMBRA_FUNCTION_BEGIN(open) {
			UMBRA_FUNCTION_DA(0, Path, VT_STRING);
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( open(vaPath.toString()) );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// close the database
		//
		
		UMBRA_FUNCTION_BEGIN(close) {
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( close() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// query the database
		//
		
		UMBRA_FUNCTION_BEGIN(query) {
			UMBRA_FUNCTION_DA(0, Sql, VT_STRING);
			UMBRA_FUNCTION_CS() {
				Recordset* pRs = new Recordset();
				if (query(*pRs, vaSql.toString()) == false)
				{
					this;
					delete pRs;
					UMBRA_FUNCTION_RET( Umbra::Variant() );
				}
				else
				{
					pRs->setUmbraGarbageCollected();
					UMBRA_FUNCTION_RET( *pRs );
				}
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// execute a task on the controller
		//
		
		UMBRA_FUNCTION_BEGIN(execute) {
			UMBRA_FUNCTION_DA(0, Sql, VT_STRING);
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( execute(vaSql.toString().c_str()) );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// nothing found
		return R.NOT_HANDLED;
	}

	//
	// Recordset Lifetime
	//

	Recordset::Recordset()
	{
		try 
		{ 
			TESTHR(CoInitialize(NULL));
			TESTHR(m_pRS.CreateInstance(CLSID_Recordset));
		}
		catch (...) 
		{
		}

		// reset first item
		is_first = true;
	}

	Recordset::~Recordset(void)
	{
		try 
		{
			if (IsOpen())
			{
				m_pRS->Close();
			}

			// m_pRS->Release();
			CoUninitialize();
		}
		catch(_com_error e) 
		{
			std::string last_error = e.ErrorMessage();
		}
		catch(...) 
		{
			ASSERT(TRUE);
		}
	}

	//
	// Exception Handled Navigation
	//

	bool  Recordset::doAction(Actions doAction)
	{
		try 
		{
			switch (doAction) 
			{
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
		catch(_com_error e) 
		{
			return FALSE;
		}
		catch(...) 
		{
			return FALSE;
		}  
		return TRUE;
	}

	bool Recordset::read(void)
	{
		if (is_first)
		{
			if (IsEOF() == TRUE)
			{
				return false;
			}
			else
			{
				is_first = false;
				return true;
			}
		}
		else
		{
			goNext();
			if (IsEOF() == true)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}

	//
	// Query Submission
	//

	bool Recordset::OpenPrivate(Connection& ConPtr, std::string Sql)
	{
		try 
		{
			HRESULT hr = this->m_pRS->Open(_bstr_t(Sql.c_str()), ConPtr.m_pIConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
			TESTHR(hr);
			return true;
		}
		catch(_com_error e) 
		{
			ConPtr.last_error = e.ErrorMessage();
			ConPtr.last_errno = -1;
			return false;
		}
		catch(...) 
		{
			return false;
		}
	}

	//
	// Recordset Field Access
	//

	std::string Recordset::getFieldText(const char* Field)
	{
		// formatting buffer for numbers
		char buffer[32] = { 0 };

		try
		{
			_variant_t field = m_pRS->Fields->GetItem(Field)->Value;
			switch(field.vt)
			{
			case VT_R4:
				sprintf_s(buffer, sizeof(buffer), "%f", field.fltVal); 
				break;

			case VT_R8:
				sprintf_s(buffer, sizeof(buffer), "%f", field.dblVal); 
				break;

			// special handling for the string type
			case VT_BSTR:
			{
				USES_CONVERSION;
				return W2A(field.bstrVal);
			}

			case VT_I2:
			case VT_UI1:
				sprintf_s(buffer, sizeof(buffer), "%d", field.iVal); 
				break;

			case VT_INT:
				sprintf_s(buffer, sizeof(buffer), "%d", field.intVal);
				break;

			case VT_I4:
				sprintf_s(buffer, sizeof(buffer), "%d", field.lVal);
				break;

			case VT_UI4:
				sprintf_s(buffer, sizeof(buffer), "%u", field.ulVal);
				break;

			default:
				return "";
			}

			// return the default value
			return buffer;
		}
		catch(_com_error e) 
		{
			return false;
		}
	}

	int Recordset::getFieldInt(const char* Field)
	{
		return 0;
	}

	double Recordset::getFieldDouble(const char* Field)
	{
		return 0;
	}

	//
	// Umbra Control
	//

	bool Recordset::doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		// open the file database
		//
		
		UMBRA_FUNCTION_BEGIN(field) {
			UMBRA_FUNCTION_DA(0, Key, VT_STRING);
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( getFieldText(vaKey.toString().c_str()).c_str() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// row navigation
		//

		UMBRA_FUNCTION_BEGIN(read) {
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( read() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(next) {
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( goNext() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(previous) {
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( goPrevious() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(first) {
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( goFirst() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		UMBRA_FUNCTION_BEGIN(last) {
			UMBRA_FUNCTION_CS() {
				UMBRA_FUNCTION_RET( goLast() );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// nothing found
		return R.NOT_HANDLED;
	}

} } }