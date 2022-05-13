#include "EventPublisher.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventPublisher)

	EventPublisher::EventPublisher(Vector<EventSubscriber*>& subscribers) :
		_subscribers(&subscribers)
	{
	}

	void EventPublisher::Deliver()
	{
		for (auto& subscriber : *_subscribers)
		{
			subscriber->Notify(*this);
		}
	}
}