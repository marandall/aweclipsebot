#pragma once

#define FOR_EACH_OF_DEFINED(var_name, pos_name, loop_source)						\
			var_name = NULL;														\
			for (size_t pos_name = 0; pos_name < loop_source.size(); pos_name++) {		\
				var_name = loop_source[pos_name];

#define END_FOR_EACH				}