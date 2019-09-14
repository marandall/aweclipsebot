#pragma once
#include <list>

// Botgram Protocol for Eclipse Evolution
// ---------------------------------------------------
//
// <botgram uid="1234567" size="TOTAL_LENGTH" app="APPLICATION_TYPE" page="5" pages="10" about="DESCRIPTION" hash="" />
// <meta ... />
// \n\n
//
// msg_type	= The type of message being sent
// uid		= Unique Identifier sent per-message to identify responses 
// id		= The type of message being set
// total	= Total length (in bytes) of the data
// app		= Textual description of the application type to handle this
// page		= This is page number, default will by 0
// pages	= The total number of pages in this message

// Application Types
// ---------------------------------------------------
// 
// e2/crep		= Eclipse Evolution command response
// e2/relay		= Relay Mechanism
// e2/stop		= Stop Messge <uid>
// ping_req		= Ping Request
// ping_resp	= Ping Response
