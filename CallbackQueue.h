#pragma once
#include <list>

class EclipseBot;

namespace CallbackQueue
{
	class Target
	{
	public:
		virtual ~Target()
		{
		}

		virtual void handle_result(EclipseBot& Client, int Result) = 0;
	};

	class Queue
	{
	protected: // the node
		struct Node
		{
			Target* target;
			Target* system_ptr;
		};

	public: // type defenitions
		typedef std::list<Node>	List;
		typedef List::iterator		iterator;
	
	public: // constructions
		Queue(EclipseBot* pBot) { m_pBot = pBot; }

	public: // variables
		List m_list;
		EclipseBot* m_pBot;

	public: // execution
		void insert(Target* PtrTarget, Target* PtrSystem = 0);
		void handle_result(int Result);
	};
}