#pragma once
#include "aw.h"
#include "umbra/umbra_remoteclass.h"

namespace AW 
{
	class Bot;

	// reserved ids for various parts of the HUD
	const int HUD_RESERVED_ID_MAP			= 1;
	const int HUD_RESERVED_ID_MAP_2			= 2;

	// classes for the hud element
	class HUD_Element : public Umbra::RemoteClass
	{
	public:
		int		type;
		int		id;
		int		session;
		int		origin;
		int		x, y, z;
		int		flags;
		CStringA text;
		int		color;
		float	opacity;
		int		size_x, size_y, size_z;
		int		click_x, click_y, click_z;
		int		texture_offsetx;
		int		texture_offsety;
		string	last_hash;
	
	public: // property
		bool doUmbraProperty(Umbra::Variant& V, Umbra::PropertyRequest& R);
	
	protected: // hash checking
		string getLastHash();

	public:
		HUD_Element()
		{
			type = AW_HUD_TYPE_IMAGE;
			id = 0;
			session = 0;
			origin = 0;
			x = y = z = 0;
			flags = 0;
			color = 0xffffff;
			opacity	= 1.0f;
			size_x = size_y = size_z = click_x = click_y = click_z = 0;
			texture_offsetx = texture_offsety = 0;
		}

	public: // setting options
		void	setPosition	(int X, int Y, int Z) { x = X; y = Y; z = Z;						}
		void	setSize		(int X, int Y, int Z) { size_x = X; size_y = Y; size_z = Z;			}
		void	setOffset	(int X, int Y)		  { texture_offsetx = X; texture_offsety = Y; }

	public: // check against flags for the heads up display
		bool	IsClickable()		  const { return (flags & AW_HUD_ELEMENT_FLAG_CLICKS)		!= 0; }
		bool	IsStretched()		  const { return (flags & AW_HUD_ELEMENT_FLAG_STRETCH)		!= 0; }
		bool	IsAdditive()  		  const { return (flags & AW_HUD_ELEMENT_FLAG_ADDITIVE)		!= 0; }
		bool	IsPercentage()	  	  const { return (flags & AW_HUD_ELEMENT_FLAG_SIZE_PERCENT) != 0; }
		bool	IsTransitional()	  const { return (flags & AW_HUD_ELEMENT_FLAG_TRANSITION)	!= 0; }
		bool	IsTempoary()		  const { return (flags & AW_HUD_ELEMENT_FLAG_TEMPORARY)	!= 0; }
		bool	IsUsingUniversePath() const { return (flags & AW_HUD_ELEMENT_FLAG_UNIVERSE_PATH)!= 0; }
		bool	IsClamped()			  const { return (flags & AW_HUD_ELEMENT_FLAG_CLAMP)		!= 0; }
		bool	IsHighlighted()		  const { return (flags & AW_HUD_ELEMENT_FLAG_HIGHLIGHT)	!= 0; }
		bool	HasStyle(int Style)	  const { return (flags & Style)							!= 0; } 

	public: // checks against the type
		bool	IsText()			  const { return type == AW_HUD_TYPE_TEXT;	}
		bool	IsImage()			  const { return type == AW_HUD_TYPE_IMAGE;	}
		bool	IsModel()			  const { return type == AW_HUD_TYPE_MODEL;	}

	public: // checks against the type
		void	MakeText()			  { type = AW_HUD_TYPE_TEXT;	}
		void	MakeImage()			  { type = AW_HUD_TYPE_IMAGE;	}
		void	MakeModel()			  { type = AW_HUD_TYPE_MODEL;	}

	public: // check against the origin
		bool	IsAlignTopLeft()		const	{ return origin == AW_HUD_ORIGIN_TOP_LEFT;		}
		bool	IsAlignTop()			const	{ return origin == AW_HUD_ORIGIN_TOP;			}
		bool	IsAlignTopRight()		const	{ return origin == AW_HUD_ORIGIN_TOP_RIGHT;		}
		bool	IsAlignLeft()			const	{ return origin == AW_HUD_ORIGIN_LEFT;			}
		bool	IsAlignCenter()			const	{ return origin == AW_HUD_ORIGIN_CENTER;		}
		bool	IsAlignRight()			const	{ return origin == AW_HUD_ORIGIN_RIGHT;			}
		bool	IsAlignBottomLeft()		const	{ return origin == AW_HUD_ORIGIN_BOTTOM_LEFT;	}
		bool	IsAlignBottom()			const	{ return origin == AW_HUD_ORIGIN_BOTTOM;		}
		bool	IsAlignBottomRight()	const	{ return origin == AW_HUD_ORIGIN_BOTTOM_RIGHT;	}

	public: // set alignments
		void	AlignTopLeft()			{ origin = AW_HUD_ORIGIN_TOP_LEFT;		}
		void	AlignTop()				{ origin = AW_HUD_ORIGIN_TOP;			}
		void	AlignTopRight()			{ origin = AW_HUD_ORIGIN_TOP_RIGHT;		}
		void	AlignLeft()				{ origin = AW_HUD_ORIGIN_LEFT;			}
		void	AlignCenter()			{ origin = AW_HUD_ORIGIN_CENTER;		}
		void	AlignRight()			{ origin = AW_HUD_ORIGIN_RIGHT;			}
		void	AlignBottomLeft()		{ origin = AW_HUD_ORIGIN_BOTTOM_LEFT;	}
		void	AlignBottom()			{ origin = AW_HUD_ORIGIN_BOTTOM;		}
		void	AlignBottomRight()		{ origin = AW_HUD_ORIGIN_BOTTOM_RIGHT;	}

		void	SetRect_FromBR(int Width, int Height, int PadX, int PadY)
										{ 
										  origin = AW_HUD_ORIGIN_BOTTOM_RIGHT;
										  size_x = Width; size_y = Height;
										  x = -(size_x + PadX);
										  y = -(size_y + PadY);
										};
		void	SetRect_FromTL(int Width, int Height, int PadX, int PadY)
										{ 
										  origin = AW_HUD_ORIGIN_TOP_LEFT;
										  size_x = Width; size_y = Height;
										  x = (PadX);
										  y = (PadY);
										};

	public: // options for adding styles
		void	setStyle(int Style, bool On){ if (On) flags|= Style; else flags&= ~Style;	}

	public: // operations regarding SDK
		void	CopyToSDK(Bot& SDK);
	};
}
