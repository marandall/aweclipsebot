#pragma once
#include <vector>

/* functions for outputting */
void WriteMsg(const char* MessageText, long Colour, int Style);
void WriteDebugMsg(const char* MessageText);

/* functions for handling output */
size_t SplitToMultipart(std::vector<CStringA>& lines, const char* Input, size_t MAX_SIZE, const char* breakers);

// variables for the output message
#define MF_BOLD				1
#define MF_ITALIC			2
#define MF_DUAL				3
#define OT_MAIN				4
#define OT_TECH				8
#define OT_BOTH				(OT_MAIN | OT_TECH)
#define OT_DEBUG			16
#define OM_TIME				32
#define MF_FBOLD			64
#define OT_USERLIST			128
#define OT_SECURITY			256
#define OT_ERROR			(512 | OT_TECH | OM_TIME)
#define OM_COMMAND			(OT_MAIN | MF_ITALIC)
#define OMT_SIGNAL			(OT_MAIN | OM_TIME)

/* colour defenitions */
#define RGB_GREEN			0x00ff00
#define RGB_RED				0x0000ff
#define RGB_BLUE			0xff0000
#define RGB_SUCCESS			RGB(20, 20, 200)
#define RGB_FAIL			0x0000FF
#define RGB_NOTIFY			0x800080
#define RGB_AVATAR			0x338833
#define RGB_IOMSG			0x227722
#define RGB_EJECT			0x404000
#define RGB_COMMAND_OUT		0x000000
#define RGB_COMMAND_IN		0x404080
#define RGB_ENABLED			0x00ff00
#define RGB_CONNECTING		RGB_BLUE
#define RGB_UNAVAILABLE		0x555555