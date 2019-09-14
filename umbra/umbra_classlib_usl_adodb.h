#pragma once

// adodb connection interface
#pragma warning(disable:4146)
#import "c:\program files\common files\system\ado\msado15.dll" rename_namespace("ADODB_LIB") named_guids rename("EOF", "bEOF") rename("BOF", "bBOF")
#pragma warning(default:4146)

// standard templates
#include <string>
#include "umbra_classlib.h"

// relevant classes
namespace Umbra { namespace ClassLib { namespace Usl {

	class RecordSet;

	class Connection : public Umbra::RemoteClass
	{
		friend class Recordset;

	protected:
		ADODB_LIB::_ConnectionPtr	m_pIConn;	// connection pointer
		std::string		last_error;				// last error string
		int				last_errno;				// is there an error

	public: // lifetime operators
		Connection(void);
		~Connection(void);

	public: // basic operations
		bool isOpen(void);
		
	public: // execution and querying routines
		bool execute(std::string Sql);
		bool query(Recordset& RS, std::string Sql);

	public: // connection handling
		bool open(std::string Sql);
		bool close(void);

	public: // umbra information
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
	};

	class Recordset : public Umbra::RemoteClass
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

		friend class Connection;

	public:
		ADODB_LIB::_RecordsetPtr m_pRS;		// database pointer
		BOOL			m_bOK;				// any error
		Connection*		m_pCon;
		bool			is_first;

	public: // lifetime operators
		Recordset(void);
		~Recordset(void);

	protected: // connection routines
		bool OpenPrivate(Connection& ConPtr, std::string Sql);
	
	public: // navigation and actions
		bool doAction(Actions doAction);
		bool goNext(void) { return doAction(doMoveNext);			}
		bool goFirst(void) { return doAction(doMoveFirst);			}
		bool goLast(void) { return doAction(doMoveLast);			}
		bool goPrevious(void) { return doAction(doMovePrevious);	}
		bool addNew(void) { return doAction(doAddNew);				}
		bool update(void) { return doAction(doUpdate);				}
		bool deleteRow(void) { return doAction(doDelete);			}
		bool read(void);

	public: // field access
		std::string getFieldText(const char* Field);
		int getFieldInt(const char* Field);
		double getFieldDouble(const char* Field);

	public: // quick looping tests
		bool IsEOF()		{ return m_pRS->bEOF == VARIANT_TRUE; }
		bool IsOpen()		{ return m_pRS->GetState() != ADODB_LIB::adStateClosed; }
		Recordset& Close()  { if (IsOpen()) m_pRS->Close(); return *this; }

	public: // umbra information
		bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R);
	};

	//
	// Class Interface
	//

	class AdodbConnectionClass: public RemoteClass, public INewInstanceCreator
	{
	public: // the instance creator interface
		RemoteClass* doUmbraNewInstance(Host* HostPtr)
		{
			return new Connection();
		}
	};

} } }