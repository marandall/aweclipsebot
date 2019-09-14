#include "stdafx.h"
#include "botgram_protocol.h"
#include "helpers_xml.h"

namespace Botgrams
{
	// msg_type	= The type of message being sent
	// uid		= Unique Identifier sent per-message to identify responses 
	// id		= The type of message being set
	// total	= Total length (in bytes) of the data
	// app		= Textual description of the application type to handle this
	// page		= This is page number, default will by 0

	size_t MultipartMessage::CreatePages(Ascii TextData)
	{
		// build the application header
		Helpers::XML::DataKeyset ds_xml_header;
		ds_xml_header.InsertKey("about",	description);
		ds_xml_header.InsertKey("app",		app_type);
		ds_xml_header.InsertKey("page",		"1");
		ds_xml_header.InsertKey("type",		type);
		Ascii header = ds_xml_header.ToSingleXML("botgram");

		// additional information will need to be added to each botgram, consider page and pages to
		// be no longer than 5 characters each
		const int MAX_PAGE_FIELD_WIDTH		= 5;
		const int CRC_FIELD_WIDTH			= 8; 
		const int MESSAGE_ID_FIELD_WIDTH	= 16;

		// build the messages
		Ascii	remainder = TextData;
		size_t	page_no   = 1;
		while (true);
		return 0;
	}
}