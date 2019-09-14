#pragma once
#include "AWSDK_Support.h"

namespace XML
{
	class Field;
	class Node;

	class NodeList
	{
		friend class Node;
	protected:
		std::vector<Node*> m_list;
	public:
		Node& operator[](size_t Index)  { return *m_list[Index];	}
		size_t size() const				{ return m_list.size(); }
	};

	class Node
	{
	public:
		Node() { record_opened = false; }
		~Node();

		Node*			level_up;
		String		type;
		String		getType() const	{ return type; }

		String	NodeType;
		AW::VectorP<Field, 2>		headers;
		AW::VectorP<Field, 2>		fields;
		AW::VectorP<Node,2>		records;
	
		bool			Save(FILE* fptr, int level);
		bool			record_opened;

		/* header manipulations */
		void	 InsertHeader	(const char* field, const char* data);
		void	 InsertHeaderInt (const char* field, int data);
		void	 InsertFieldFloat(const char* field, float data);
		String	 getHeader		(const char* field, const char* default_str = "") const;
		int		 getHeaderInt	(const char* field, int default_int = 0) const;
		bool	 HeaderExists	(const char* field) const;
		bool	 Exists			(const char* field) const;

		/* get a field information */
		void	 InsertField	(const char* field, const char* data);
		void	 InsertFieldInt	(const char* field, int data);
		String    getField		(const char* field, const char* default_str = "") const;
		int 	 getFieldInt	(const char* field, int default_int = 0) const;
		float	 getFieldFloat	(const char* field, float default_float = 0.0) const;
		bool	 FieldExists	(const char* field) const;
		void	 setField		(const char* field, const char* data);
		void	 setFieldInt	(const char* field, int data);
		void	 setFieldFloat	(const char* field, float data);
		int		 getValue		(const char* field, const char* default_str) const;
		int		 getValueInt	(const char* field, int default_int) const;
		String	 operator[]		(const char* Field) { return getField(Field, ""); }

		/* get a record information */
		Node*	 InsertNode		(const char* name);
		Node*	 getNode		(const char* field);
		Node*	 InsertNode		(Node* pNode);
		NodeList getNodeList	(const char* NodesType);

		bool	SaveToFile(const char* file);
		void	DestroyContent();
		bool	NodeExists(const char* key) const;
	};

	class File
	{
	public:
		File(void);
		~File(void);

		Node*	record, *old_record;
		Node	root_record;
		Node* current_record;
		BOOL	LoadFile(const char* file);

		long	current_line;
		void	InitialiseParser() {
			current_line	= 0;
			current_record	= &root_record;
			error_count		= 0;
		}

		int		error_count;

		void WriteError(const char* error);

		bool WhiteSpace(char c) { return (c == ' ' || c == '\t'); }
		bool NameCharValid(char c) { return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_' || (isdigit(c) != 0)); }
		bool DataCharValid(char c) { return (c != '\0' && c != '>' && c != '<'); }

		void ParseFile(const char* string);
	};

	class Field
	{
	public:
		Field(){}
		Field(const char* _name, const char* _data) { name = _name; data = _data; }
		Field(const char* _name, int _data) {
			data.Format("%d", _data);
			name = _name;
		}

		~Field(){}

		String		name, data;

		/* natural support */
		String	getName()	{ return name; }
		String	getData()	{ return data; }
	};

}