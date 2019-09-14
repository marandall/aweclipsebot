#pragma once

/* conversion macros */
#define Bb(x)	((x) == TRUE)
#define bB(x)	((x) ? TRUE : FALSE)
#define BtB(x)	((x) == TRUE ? TRUE : FALSE)



#define CITIZEN_DUAL_PROPERTY(name, internal, type)				\
private:														\
	type internal;												\
public:															\
	inline type get##name() const { return internal;		}	\
public:															\
	inline void set##name( type intp_arg )	{ internal = intp_arg;	}

#define CITIZEN_PROT_PROPERTY(name, internal, type)				\
private:														\
	type internal;												\
public:															\
	inline type get##name() const { return internal;		}	\
protected:														\
	inline void set##name( type intp_arg )	{ internal = intp_arg;	}

#define DUAL_PROPERTY(name, internal, type)						\
private:														\
	type internal;												\
public:															\
	inline type get##name() const { return internal;		}	\
	inline void set##name( type intp_arg )	{ internal = intp_arg;	}

#define PROTECTED_DUAL_PROPERTY(name, internal, type)			\
protected:														\
	type internal;												\
public:															\
	inline type get##name() const { return internal;		}	\
protected:														\
	inline void set##name( type intp_arg )	{ internal = intp_arg;	}
#define PROTECTED_DUAL_PROPERTY_DEFINED								// repeat protection

#define READ_PROPERTY(name, internal, type)						\
private:														\
	type internal;										\
public:															\
	inline type get##name() const { return internal; }	

#define INT_BASE_CHECK()										\
	if (getInitialised() == FALSE) return RC_NOT_INITIALIZED;	\
	if (SelectInstance() == FALSE) return RC_NO_INSTANCE;

#define DEFINE_BASE_BOT()										\
	AW::Bot* pBot = bots.getCurInstance();						\
	if (pBot == NULL)											\
		return;


#define FOR_EACH_OF_DEFINED_LL(var_name, pos_name, loop_source)	POSITION pos_name = loop_source.getHeadPosition(); var_name = NULL; while ( pos_name ) { var_name = loop_source.getNext( pos_name );  
#define END_FOR_EACH_LL		}
#define FOR_EACH_OF_DEFINED_LINKERS_LL(var_name, pos_name, back_pos, loop_source)	POSITION pos_name = loop_source.getHeadPosition(); POSITION back_pos = NULL; var_name = NULL; while ( pos_name ) { back_pos = pos_name; var_name = loop_source.getNext( pos_name );  
#define ERASE_LINKED_LIST_LL(x) while (x.size()) delete x.RemoveHead();
