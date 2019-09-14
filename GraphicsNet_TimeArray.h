#pragma once
#include "GraphicsNet_Base.h"

namespace GraphicsNet
{
	class TimeArray :
		public BaseObject
	{
	public:
		bool OnDraw(Gdiplus::Graphics& Gdi, int Width, int Height);
	};
};