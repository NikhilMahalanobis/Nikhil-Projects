#include "Event.h"

namespace FieaGameEngine
{
	template<typename T>
	RTTI_DEFINITIONS(Event<T>)

	template<typename T>
	Event<T>::Event(const T& messageObject) :
		EventPublisher(_subscribers), _messageObject(messageObject)
	{

	}

	template<typename T>
	void Event<T>::Subscribe(EventSubscriber& subscriber)
	{
		auto it = _subscribers.Find(&subscriber);
		if (it == _subscribers.end())
		{
			_subscribers.PushBack(&subscriber);
		}
	}

	template<typename T>
	void Event<T>::Unsubscribe(EventSubscriber& subscriber)
	{
		auto it = _subscribers.Find(&subscriber);
		if (it != _subscribers.end())
		{
			_subscribers.Remove(it);
		}
	}

	template<typename T>
	void Event<T>::UnsubscribeAll()
	{
		_subscribers.Clear();
	}

	template<typename T>
	const T& Event<T>::Message() const 
	{
		return _messageObject;
	}
}