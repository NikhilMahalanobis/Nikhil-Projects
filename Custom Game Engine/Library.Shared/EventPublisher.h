#pragma once

#include <memory>
#include "RTTI.h"
#include "Vector.h"
#include "EventSubscriber.h"

namespace FieaGameEngine
{
	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI)

	public:
		/// <summary>
		/// TypeCast Constructor for EventPublisher
		/// </summary>
		/// </param name="subscribers"> Takes a reference to a Vector<EventSubscriber*> </param>
		EventPublisher(Vector<EventSubscriber*>& subscribers);

		/// <summary>
		/// Compiler supplied defaulted Default Constructor for EventPublisher
		/// </summary>
		EventPublisher() = default;

		/// <summary>
		/// Compiler supplied defaulted Copy Constructor for EventPublisher
		/// </summary>
		/// <param name="rhs"> Takes a const reference to an EventPublisher </param>
		EventPublisher(const EventPublisher& rhs) = default;

		/// <summary>
		/// Compiler supplied defaulted Copy Assignment Operator for EventPublisher
		/// </summary>
		/// <param name="rhs"> Takes a const reference to an EventPublisher </param>
		/// <returns> Returns a reference to an EventPublisher </returns>
		EventPublisher& operator=(const EventPublisher& rhs) = default;

		/// <summary>
		/// Compiler supplied defaulted Move Constructor for EventPublisher 
		/// </summary>
		/// <param name="rhs"> Takes a rvalue reference to an EventPublisher </param>
		/// <returns></returns>
		EventPublisher(EventPublisher&& rhs) noexcept = default;

		/// <summary>
		/// Compiler supplied defaulted Move Assignment Operator for EventPublisher
		/// </summary>
		/// <param name="rhs"> Takes a rvalue reference to an EventPublisher </param>
		/// <returns> Returns a lvalue reference to an EventPublisher </returns>
		EventPublisher& operator=(EventPublisher&& rhs) noexcept = default;

		/// <summary>
		/// Pure Virtual Destructor for EventPublisher 
		/// </summary>
		virtual ~EventPublisher() = 0 {};

		/// <summary>
		/// Virtual Deliver method for EventPublisher 
		/// </summary>
		virtual void Deliver();
	private:
		Vector<EventSubscriber*>* _subscribers;
	};
}