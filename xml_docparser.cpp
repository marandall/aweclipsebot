#include "stdafx.h"
#include "xml_docparser.h"

#define	  FIND_FIELD_START(tname, source)	Field* pField;	FOR_EACH_OF_DEFINED(pField, pos_loop, source) if (pField->getName() == tname) {
#define	  FIND_FIELD_END					break; } END_FOR_EACH

#ifdef DEBUG
#define new DEBUG_NEW
#endif

namespace XML 
{
	String StringToEncoded(const char* base) {
		String buffer = base;
		buffer.Replace("&", "&amp; ");
		buffer.Replace("\"", "&qt; ");
		buffer.Replace("\\", "\\\\");
		buffer.Replace("\t", "\\t");
		buffer.Replace("\n", "\\n");
		buffer.Replace("\r", "\\r");
		return buffer;
	}

	String EncodedToString(const char* base) {
		String buffer = base;

		// replace the inline instreams
		buffer.Replace("\\t", "\t");
		buffer.Replace("\\r", "\r");
		buffer.Replace("\\n", "\n");
		buffer.Replace("\\\\",  "\\");
		buffer.Replace("&qt; ", "\"");
		buffer.Replace("&amp; ", "&");
		return buffer;
	}

	/* header manipulations */
	void Node::InsertHeader(const char* field, const char* data) {
		Field*	pField = new Field(field, data);
		headers.push_back(pField);
	}

	void Node::InsertHeaderInt(const char* field, int data) {
		Field*	pField = new Field(field, data);
		headers.push_back(pField);
	}

	String Node::getHeader(const char* field, const char* default_str) const {
		FIND_FIELD_START(field, headers)
			return pField->data;   
		FIND_FIELD_END
		return default_str;
	}

	int	Node::getHeaderInt(const char* field, int default_int) const {
		FIND_FIELD_START(field, headers)	return atoi(pField->data);   FIND_FIELD_END
		return default_int;
	}

	bool Node::HeaderExists(const char* field) const {
		FIND_FIELD_START(field, headers)	return true;   FIND_FIELD_END
		return false;
	}

	/* get a field information */
	void Node::InsertField(const char* field, const char* data) {
		Field*	pField	= new Field(field, data);
		String	temp	= field;
		
		if (temp.Left(1) == "@") {
			pField->name = pField->name.Mid(1);
			headers.push_back(pField);
		}
		else
			fields.push_back(pField);
	}

	void Node::InsertFieldInt(const char* field, int data) {
		Field*	pField = new Field(field, data);
		String	temp	= field;
		
		if (temp.Left(1) == "@") {
			pField->name = pField->name.Mid(1);
			headers.push_back(pField);
		}
		else fields.push_back(pField);
	}

	String Node::getField(const char* field, const char* default_str) const {
		String temp = field;
		if (temp.Left(1) == "@")
			return getHeader(temp.Mid(1), default_str);
		else {
			for (size_t i = 0; i < fields.size(); i++) {
				if (fields[i]->name == field)
					return fields[i]->data;
			}
			return default_str;
		}
	}

	int	Node::getFieldInt(const char* field, int default_int) const {
		String temp = field;
		if (temp.Left(1) == "@")
			return getHeaderInt(temp.Mid(1), default_int);
		else {
			FIND_FIELD_START(field, fields)	
				return atoi(pField->data);   
			FIND_FIELD_END
			return default_int;
		}
	}

	float Node::getFieldFloat(const char* field, float default_float) const {
		String floatStr; floatStr.Format("%f", default_float);
		String temp = field;
		if (temp.Left(1) == "@")
			return (float)atof(getHeader(temp.Mid(1), floatStr));
		else {
			FIND_FIELD_START(field, fields)	
				return (float)atof(pField->data);   
			FIND_FIELD_END
			return default_float;
		}
	}

	void Node::InsertFieldFloat(const char* field, float data) {
		String floatData; floatData.Format("%f", data);
		InsertField(field, floatData);
	}

	bool Node::FieldExists(const char* field) const {
		FIND_FIELD_START(field, fields)	return true;   FIND_FIELD_END
		return false;
	}

	bool Node::Exists(const char* field) const {
		if (*field == '@') {
			String fname = field;
			fname = fname.Mid(1);
			return HeaderExists(fname);
		} else {
			return FieldExists(field);
		}
	}

	void Node::setField(const char* field, const char* data) {
		FIND_FIELD_START(field, fields)	pField->data = data; return; FIND_FIELD_END
	}

	void Node::setFieldInt(const char* field, int data) {
		FIND_FIELD_START(field, fields)	pField->data.Format("%d", data); return; FIND_FIELD_END
	}

	/* get a record information */
	Node* Node::InsertNode(const char* name) {
		Node* pNode = new Node();
		pNode->type = name;
		records.push_back(pNode);
		return pNode;
	}

	Node* Node::getNode(const char* field) {
		Node* pNode;
		FOR_EACH_OF_DEFINED(pNode, pos, records)
			if (pNode->getType() == field)
				return pNode;
		END_FOR_EACH
		return NULL;
	}

	bool Node::NodeExists(const char* key) const {
		for (size_t i = 0; i < records.size(); i++) {
			if (records[i]->getType() == key)
				return true;
		}
		return false;
	}

	Node*	
	Node::InsertNode(Node* pNode)
	{
		records.push_back(pNode);
		return pNode;
	}

	NodeList Node::getNodeList(const char* NodesType)
	{
		NodeList ndList;
		for (size_t i = 0; i < records.size(); i++)
			if (records[i]->type == NodesType)
				ndList.m_list.push_back(records[i]);
		return ndList;
	}

	bool	
	Node::SaveToFile(const char* file)
	{
		FILE* fptr = NULL;
		errno_t err = fopen_s(&fptr, file, "w");
		if (err != 0)
			return false;
		
		fprintf(fptr, "<?xml version=\"1.0\" ?>\n");

		Save(fptr, 0);
		fclose(fptr);

		return true;
	}


	File::File(void)
	{
		InitialiseParser();
	}

	File::~File(void)
	{
	}

	BOOL	
	File::LoadFile(const char* file)
	{
		FILE*	fptr = NULL;
		if (fopen_s(&fptr, file, "r") != 0)
			return FALSE;
		
		char	buffer[1024];
		String buf;

		if (fptr == NULL)
			return FALSE;

		while (!(fgets(buffer, 1024, fptr) == NULL)) {
			buf = buffer;
			buf.Trim();
			ParseFile(buf);
		}

		/* close the file */
		fclose(fptr);

		return TRUE;
	}

	void File::WriteError(const char* error)
	{
		error_count++;
		WriteMsg(String("<XML> ") + error, RGB(150, 20, 40), OT_DEBUG | MF_ITALIC);
	}


	void
	File::ParseFile(const char* aString)
	{
		bool	opened	= false;
		int		len		= (int)strlen(aString);
		String buffer, error, field_name, field_data;
		
		current_line++;

		/* String of the line */
		String line = aString; line = line.Trim();
		char*	pch		= (char*)(const char*)line;
		const char*	string = line;
		bool	end_quoted = false;
		bool	end_backeted = false;
		bool	char_escaped = false;

		/* skip iff line ok */
		if (line.Left(4) == "<!--" || line.Left(2) == "<?") 
			return;

		/* check the special closing tag situation here */
		String temp;  
		temp.Format("</%s>", current_record->getType());

		/* check if we have closing tag */
		if (temp == pch) {												// is closing tag
			current_record = current_record->level_up;					// pop up level
			return;														// We have enough here
		}

		/* check if the first byte is < */
		if (*pch != '<')
			return;														// error in code or blank

		
		/* loop through everything to see if we have the relevant data  note that its important to realise */
		/* that this is being done on our current set to see if we have got the closing tag for it         */
		int pos_1 = line.Find(">");										// do we close immediately?

		if (pos_1 != -1 && pos_1 != line.GetLength() - 1 ) {			// not the end byte
	        
			/* declare the tag that it is */
			String code = line.Mid(1, pos_1 - 1);						// extract real tag
			
			/* this is a check */
			if (code.GetLength() == 0) {								// is it blank?
				error.Format("Zero length attribute name on line %d position %d", current_line, (pch - string));
				WriteError(error);
				return;
			}

			/* do we have this key at the end as well? */
			String searcher; searcher.Format("</%s>", code);
		
			/* we check if our data is the correct key */
			if (line.Right(searcher.GetLength()) == searcher) {
				String tag_id = line.Mid(pos_1 + 1, 
					line.GetLength() - pos_1 - searcher.GetLength() - 1);	// extract string
				current_record->InsertField( code, EncodedToString(tag_id));						// Add attribute
				return;
			}
		}

		/* we create the new record */
		old_record					= current_record;				// backup of level
		current_record				= new Node();				// new level in memory
		current_record->level_up	= old_record;					// set the old level
		old_record->InsertNode(current_record);				// add sub item
			
		/* need to move forward one byte to put us at the first part of the name*/
		pch++;
		end_backeted = false;

		/* check if there is a # at the start of the key to define it new */
		String end_bytes = line; end_bytes = end_bytes.Trim();
		if (end_bytes.Right(2) == "/>")
			current_record->record_opened = false;					// Node is private
		else
			current_record->record_opened = true;					// Node is inclusive

		/* loop through characters until the next data */
		while (NameCharValid(*pch) == true) 
		{
			buffer.AppendChar(*pch);
			pch++;
		}

		/* check if it is > and subseuqently close */
		if (*pch == '>')
			end_backeted = true;									// used later

		/* we only check for an error if we cannot find the end bracket - next redundant? */
		if (!end_backeted) {
			/* find out why we screwed up - if we do not have a whitespace next we have issues*/
			if (WhiteSpace(*pch) == false) {						// is next character whitespace?
				/* format error, / found before end of code */
				error.Format("None whitespace found '%c' after TYPE on line %d position %d", *pch, current_line, (pch - string));
				WriteError(error);
				return;
			}
		}

		/* check the buffer */
		if (buffer.GetLength() == 0) {								// is type name zero length
			error.Format("Zero length type on line %d position %d", current_line, (pch - string));
			WriteError(error);
			return;
		}

		/* set its name to be this variable */
		current_record->type	= buffer;							// record type is defined

		/* check if end bracketed or not */
		if (end_backeted)
			return;

		/* loop ahead until we find the next character */
		while (WhiteSpace(*pch))
			pch++;

		/* our main infinate loop to find each key and the data associated with it each loop */
		for(;;) {													// launch infinate loop
			buffer			= "";									// reset the buffer

			/* example of position                             
			 * <zone name="pie" type="meh">
			 *       ^                    ^                        
			 *                                     */

			/* check what we have at this position, if we have a > then we have various possibilities  */
			if (*pch == '>') {
				/* looks like we have found the end character and can jump out */
				return;
			}

			/* check if we had a valid keycode or not */
			if (*pch == '/') {											// pre closing slash - ie: empty
				if (*(pch + 1) == '>') {
					current_record = current_record->level_up;			// we push up the current level
					return;
				}

				/* format error, / found before end of code */
				error.Format("Open / found not followed by > on line %d position %d", current_line, (pch - string));
				WriteError(error);
				return;
			}

			/* we try to extract the entire key name by looping through until the end */
			while (*pch != '=') {
				/* check if the character we have is valid for the data */
				if (NameCharValid(*pch) == false) {
					error.Format("Invalid name on line %d position %d", current_line, (pch - string));
					WriteError(error);
					return;
				}
		
				/* append to our current buffer */
				buffer.AppendChar(*pch);
				pch++;
			}

			/* loop on one to skip the next character */
			pch++;											// skips over the = statement
			field_name = buffer;

			/* we have the first character we need to check is " */
			if (*pch != '"') {
				error.Format("No opening DQUOTE after '=' on line %d position %d", current_line, (pch - string));
				WriteError(error);
				return;
			}

			/* skip forward one character */
			pch++;
			buffer = "";
			end_quoted = false;

			// -----------------------------------------------------
			// <area type="hello" circle="something">
			//                 ^
			// -----------------------------------------------------
			while (DataCharValid(*pch) == true || *pch == '"')
			{
				/* check if we are the double quote situation */
				if (*pch == '"') {								// are double quote
					field_data = buffer;						// set data buffer
					pch++;										// increment again (whowa)
					end_quoted = true;							// so we can check
					break;										// jump out of data loop
				} else {
					buffer.AppendChar(*pch);					// add valid character
					pch++;										// jump to next character
				}
			}

			if (!end_quoted) {									// only check if end is quoted
				/* check if we returned an invalid character */
				if (DataCharValid(*pch) == false) {					// check for validity
					error.Format("Invalid header data on line %d position %d", current_line, (pch - string));
					WriteError(error);
					return;
				}
			}

			current_record->InsertHeader(EncodedToString(field_name), EncodedToString(field_data));

			/* loop ahead until we find the next character */
			while (WhiteSpace(*pch))
				pch++;
		}
	}

	void	
	Node::DestroyContent()
	{
		/* delete everything */
		headers.	DeleteAll();
		fields.		DeleteAll();
		records.	DeleteAll();
	}

	Node::~Node()
	{
		DestroyContent();
	}

	bool Node::Save(FILE* fptr, int level)
	{
		/* variables used here */
		int i = 0;
		String tabs, line, stabs;
		Field*	pField	= NULL;
		Node*	pNode	= NULL;

		/* create the depth */
		for (int i = 0; i < level; i++)
			stabs.AppendChar('\t');
		tabs.Format("%s\t", stabs);

		/* go through each of the headers and print to a line */
		line.Format("%s<%s ", stabs, type);

		FOR_EACH_OF_DEFINED(pField, pos_headers, headers)
			line.AppendFormat("%s=\"%s\" ", pField->getName(), StringToEncoded(pField->data));
		END_FOR_EACH

		/* do we have any more attributes? */
		if (fields.size() == 0 && records.size() == 0)
			line.AppendFormat("/>\n");
		else
			line.AppendFormat(">\n");
		
		/* print out each of the attributes associated with it */
		FOR_EACH_OF_DEFINED(pField, pos_attributes, fields)
			line.AppendFormat("%s<%s>%s</%s>\n", tabs, pField->getName(), StringToEncoded(pField->data), pField->getName());
		END_FOR_EACH

		/* print to file before printing this */
		fprintf(fptr, "%s", (const char*)line);
		line = "";

		/* print off other records here */
		FOR_EACH_OF_DEFINED(pNode, pos_records, records)
			pNode->Save(fptr, level + 1);
		END_FOR_EACH

		/* now we need to close it? */
		if (!(fields.size() == 0 && records.size() == 0))
			fprintf(fptr, "%s</%s>\n", (const char*)stabs, (const char*)type);

		/* don't know why, haha */
		return true;
	}
}
