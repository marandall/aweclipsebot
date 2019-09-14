#pragma once

class IRawParamEditor
{
public:
	virtual String param_read(String ParamName) = 0;
	virtual bool param_write(String ParamName, String Value) = 0;
};