#pragma once

// import the forward references to other classes
namespace Gdiplus { class Graphics; }

// handler forward references 
class GraphicsLabelHandler;
class GraphicsLabel;

namespace GraphicsNet
{
	class BaseObject
	{
	public:
		int						m_Width, m_Height;		// width and height of render
		Gdiplus::Graphics*		m_Gdi;
		GraphicsLabel*			m_pWindow;
		GraphicsLabelHandler*	m_pMsgTarget;

		BaseObject() { m_pMsgTarget = NULL; m_pWindow = NULL; m_Gdi = NULL; }

		virtual bool OnDraw() { return true; }
		virtual bool OnKey(int KeyCode, bool CtrlKey, bool ShiftKey)
					{ return false; }

		// post back messages
		void post_handler_msg(const char* Msg, void* Data);

		// mouse handlers
		virtual void OnLButtonDown	(int X, int Y) { }
		virtual void OnMButtonDown	(int X, int Y) { }
		virtual void OnRButtonDown	(int X, int Y) { }
		virtual void OnLButtonUp	(int X, int Y) { }
		virtual void OnMButtonUp	(int X, int Y) { }
		virtual void OnRButtonUp	(int X, int Y) { }
		virtual void OnMouseMove	(int X, int Y, UINT Flags) { }
		virtual void OnMButtonDoubleClick(int X, int Y) { }
	};
}