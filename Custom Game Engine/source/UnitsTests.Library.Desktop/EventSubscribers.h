#pragma once

#include "EventQueue.h"
#include "Foo.h"

namespace UnitsTestsLibraryDesktop
{
	class FooSubscriber final : public FieaGameEngine::EventSubscriber
	{
	public:

		explicit FooSubscriber(int data);

		FooSubscriber() = default;

		~FooSubscriber() = default;

		void Notify(const FieaGameEngine::EventPublisher& publisher) override;

		void SetData(int value);

		int GetData() const;

		int _data;

		bool _wasNotified{ false };
	};
}