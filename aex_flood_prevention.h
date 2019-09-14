#pragma once

class Avatar;

namespace AEX
{
	class FloodPrevention_Data
	{
		friend class Avatar;

	public: // member variables
		int		mps_count[5];
		int		same_msg_count;
		time_t	muted_time;
		time_t	last_loop;
		String	last_message;
	
	public: // read access
		int		getMPS				(size_t I) const{ return mps_count[I];			}
		int		getSameMessages	(void) const	{ return same_msg_count;	}
		time_t	getMutedTime		(void) const	{ return muted_time;			}
		time_t	getLastLoop		(void) const	{ return last_loop;				}
		String	getLastMessage		(void) const	{ return last_message;			}

	public: // write access
		void	setMPS				(int I, int V)	{ mps_count[I]	 = V;			}
		void	setSameMessages	(int Count)		{ same_msg_count = Count;		}
		void	setMutedTime		(time_t Time)	{ muted_time	 = Time;		}
		void	setLastLoop		(time_t Time)	{ last_loop		 = Time;		}
		void	setLastMessage		(String Text)	{ last_message	 = Text;		}
		int		IncrementMPS		(size_t Index)  { return (++mps_count[Index]);	}

	public:
		FloodPrevention_Data() 
		{ 
			memset(mps_count, 0, sizeof(mps_count));
			same_msg_count		= 0;
			muted_time			= 0;
			last_loop			= 0;
		}
	};

	class FloodPrevetion
	{
	private:
		FloodPrevention_Data	m_flood_prevention_data;
	public:
		FloodPrevention_Data&	getFPD()			{ return m_flood_prevention_data; }
	};
}