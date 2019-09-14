#pragma once

#define B_EMPTY				(2)
#define B_TRUE				(1)
#define B_FALSE				(0)

typedef int tsb_t;

#define tsb_check(Part1, Part2) ( (Part1 == B_EMPTY) ? (Part2 ? true : false) : (Part1 == B_TRUE ? true : false))

inline CStringA tsb_to_text(tsb_t T)
{
	if (T == B_EMPTY) return "default";
	if (T == B_TRUE)  return "true";
	if (T == B_FALSE) return "false";
	return "unknown";
}