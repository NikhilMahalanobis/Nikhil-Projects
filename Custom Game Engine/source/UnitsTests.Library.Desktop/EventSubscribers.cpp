#include "pch.h"
#include "EventSubscribers.h"

namespace UnitsTestsLibraryDesktop
{
	FooSubscriber::FooSubscriber(int data) :
		_data(data)
	{
	}
	void FooSubscriber::Notify(const FieaGameEngine::EventPublisher& publisher)
	{
		const FieaGameEngine::Event<Foo>* foo = publisher.As<FieaGameEngine::Event<Foo>>();

		if (foo != nullptr)
		{
			_data = foo->Message().Data();
			_wasNotified = true;
		}
	}
	void FooSubscriber::SetData(int value)
	{
		_data = value;
	}
	int FooSubscriber::GetData() const
	{
		return _data;
	}
}