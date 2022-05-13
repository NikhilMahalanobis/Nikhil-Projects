#pragma once

#include "EventPublisher.h"

namespace FieaGameEngine
{
	template<typename T>
	class Event : public EventPublisher
	{
		RTTI_DECLARATIONS(Event, EventPublisher)

	public:
		/// <summary>
		/// Typecast Constructor for Event
		/// </summary>
		/// <param name="messageObject"> Takes a const reference to a templated type T </param>
		Event(const T& messageObject);

		/// <summary>
		/// Compiler supplied defaulted Copy Constructor for Event
		/// </summary>
		/// <param name="rhs"> Takes a const reference to an Event </param>
		Event(const Event& rhs) = default;

		/// <summary>
		/// Compiler supplied defaulted Copy Assignment Operator for Event
		/// </summary>
		/// <param name="rhs"> Takes a const referene to an Event </param>
		/// <returns> Returns a reference to an Event </returns>
		Event& operator=(const Event& rhs) = default;

		/// <summary>
		/// Compiler supplied defaulted Move Constructor for Event 
		/// </summary>
		/// </param name="rhs"> Takes a rvalue reference to an Event </param>
		Event(Event&& rhs) noexcept = default;

		/// <summary>
		/// Compiler supplied defaulted Move Assignment Operator 
		/// </summary>
		/// </param name="rhs"> Takes a rvalue reference to an Event </param>
		/// <returns> Returns a reference to an Event </returns>
		Event& operator=(Event&& rhs) noexcept = default;

		/// <summary>
		/// Virtual Compiler supplied defaulted Destructor for Event 
		/// </summary>
		virtual ~Event() = default;

		/// <summary>
		/// Subscribe method for Event 
		/// </summary>
		/// </param name="subscriber"> Takes a reference to an EventSubscriber </param>
		static void Subscribe(EventSubscriber& subscriber);

		/// <summary>
		/// Unsubscribe method for Event 
		/// </summary>
		/// </param name="subscriber"> Takes a reference to an EventSubscriber </param>
		static void Unsubscribe(EventSubscriber& subscriber);

		/// <summary>
		/// UnsubscribeAll method for Event 
		/// </summary>
		static void UnsubscribeAll();
		
		/// <summary>
		/// Const Message method for Event 
		/// </summary>
		/// <returns> Returns a const reference to a templated type T </returns>
		const T& Message() const;

	private:
		inline static Vector<EventSubscriber*> _subscribers;

		T _messageObject;
	};
}

#include "Event.inl"