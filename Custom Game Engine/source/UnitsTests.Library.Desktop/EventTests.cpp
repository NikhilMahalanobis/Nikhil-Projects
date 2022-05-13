#include "pch.h"
#include "ToStringSpecializations.h"
#include "EventQueue.h"
#include "MonsterFoo.h"
#include "EventSubscribers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitsTestsLibraryDesktop
{
	TEST_CLASS(EventTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(RTTITests)
		{
			Foo foo;
			Event<Foo> event(foo);

			RTTI* rtti = &event;
			Assert::IsFalse(rtti->Is(MonsterFoo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Event<Foo>::TypeIdClass()));
			Assert::IsTrue(rtti->Is(EventPublisher::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));

			MonsterFoo* m = rtti->As<MonsterFoo>();
			Assert::IsNull(m);

			Event<Foo>* e = rtti->As<Event<Foo>>();
			Assert::IsNotNull(e);
			Assert::AreEqual(&event, e);

			EventPublisher* ep = rtti->As<EventPublisher>();
			Assert::IsNotNull(ep);
			Assert::AreEqual(&event, static_cast<Event<Foo>*>(ep));
			Assert::IsTrue(rtti->Is(ep->TypeIdInstance()));
		}

		TEST_METHOD(TestEventDeliver)
		{
			Foo foo;
			FooSubscriber fooSubscriber(10);
			Event<Foo> event(foo);
			Assert::AreEqual(foo, event.Message());

			Assert::IsFalse(fooSubscriber._wasNotified);
			event.Deliver();
			Assert::IsFalse(fooSubscriber._wasNotified);

			Event<Foo>::Subscribe(fooSubscriber);
			event.Deliver();
			Assert::IsTrue(fooSubscriber._wasNotified);
			Assert::AreEqual(event.Message().Data(), fooSubscriber.GetData());

			fooSubscriber._wasNotified = false;
			Event<Foo>::Unsubscribe(fooSubscriber);
			event.Deliver();
			Assert::AreEqual(fooSubscriber._wasNotified, false);

			FooSubscriber monsterSubscriber;
			Event<MonsterFoo>::Subscribe(monsterSubscriber);
			Event<Foo>::Subscribe(fooSubscriber);
			event.Deliver();
			Assert::IsFalse(monsterSubscriber._wasNotified);
			Assert::IsTrue(fooSubscriber._wasNotified);

			Event<MonsterFoo>::UnsubscribeAll();
			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EventQueueEnqueue)
		{
			GameTime gameTime;
			GameState gameState;
			Foo foo{100};
			FooSubscriber fooSubscriber;
			EventQueue eventQueue(gameTime);
	
			std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(foo);
			Assert::AreEqual(event->Message(), foo);

			Assert::AreEqual(size_t(0), eventQueue.Size());
			eventQueue.Enqueue(event);
			Assert::AreEqual(size_t(1), eventQueue.Size());

			Event<Foo>::Subscribe(fooSubscriber);
			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
			Assert::IsFalse(fooSubscriber._wasNotified);
			Assert::AreNotEqual(foo.Data(), fooSubscriber.GetData());
			eventQueue.Update(&gameState);
			Assert::IsTrue(fooSubscriber._wasNotified);
			Assert::AreEqual(foo.Data(), fooSubscriber.GetData());

			fooSubscriber._wasNotified = false;
			eventQueue.Update(&gameState);
			Assert::IsFalse(fooSubscriber._wasNotified);
			Assert::AreEqual(size_t(0), eventQueue.Size());

			std::shared_ptr<Event<Foo>> event2 = std::make_shared<Event<Foo>>(Foo {200});
			std::shared_ptr<Event<Foo>> event3 = std::make_shared<Event<Foo>>(Foo{ 300 });

			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::time_point());
			eventQueue.Enqueue(event, std::chrono::milliseconds(1000));
			eventQueue.Enqueue(event2, std::chrono::milliseconds(999));
			eventQueue.Enqueue(event3, std::chrono::milliseconds(500));
			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(500ms));
			eventQueue.Update(&gameState);
			Assert::IsTrue(fooSubscriber._wasNotified);
			Assert::AreEqual(300, fooSubscriber.GetData());
			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(999ms));
			eventQueue.Update(&gameState);
			Assert::IsTrue(fooSubscriber._wasNotified);
			Assert::AreEqual(200, fooSubscriber.GetData());
			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1000ms));
			eventQueue.Update(&gameState);
			Assert::IsTrue(fooSubscriber._wasNotified);
			Assert::AreEqual(100, fooSubscriber.GetData());

			Assert::IsTrue(eventQueue.IsEmpty());
			eventQueue.Clear();
			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(CopySemantics)
		{
			{
				//Copy Constructor
				Event<Foo> event(Foo {});
				Event<Foo> anotherEvent(event);
				Assert::AreNotSame(event.Message(), anotherEvent.Message());
				Assert::AreEqual(event.Message(), anotherEvent.Message());
			}
			{
				//Copy Assignment
				Event<Foo> event(Foo {});
				Event<Foo> anotherEvent(Foo{});
				anotherEvent = event;
				Assert::AreNotSame(event.Message(), anotherEvent.Message());
				Assert::AreEqual(event.Message(), anotherEvent.Message());
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			{
				//Move Constructor
				Event<Foo> event(Foo {});
				Event<Foo> anotherEvent(std::move(event));
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::IsFalse(&event.Message() == &anotherEvent.Message());
#pragma warning(pop)
			}

			{
				//Move Assignment
				Event<Foo> event(Foo {});
				Event<Foo> anotherEvent(Foo{});
				anotherEvent = std::move(event);
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::IsFalse(&event.Message() == &anotherEvent.Message());
#pragma warning(pop)
			}
		}
	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState EventTests::_startMemState;
}