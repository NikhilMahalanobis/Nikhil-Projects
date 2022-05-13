#pragma once

namespace FieaGameEngine
{
	class EventPublisher;

	class EventSubscriber
	{
	public: 
		/// <summary>
		/// Compiler supplied defaulted Constructor for EventSubscriber
		/// </summary>
		EventSubscriber() = default;

		/// <summary>
		/// Compiler supplied defaulted Copy Constructor for EventSubscriber
		/// </summary>
		/// <param name="rhs"> Takes a const reference to an EventSubscriber </param>
		EventSubscriber(const EventSubscriber& rhs) = default;

		/// <summary>
		/// Compiler supplied defaulted Copy Assignment Operator for EventSubscriber 
		/// </summary>
		/// <param name="rhs"> Takes a const reference to an EventSubscriber </param>
		/// <returns> Returns a reference to an EventSubscriber </returns>
		EventSubscriber& operator=(const EventSubscriber& rhs) = default;

		/// <summary>
		/// Compiler supplied defaulted Move Constructor for EventSubscriber
		/// </summary>
		/// <param name="rhs"> Takes a rvalue reference to a EventSubscriber </param>
		/// <returns></returns>
		EventSubscriber(EventSubscriber&& rhs) noexcept = default;

		/// <summary>
		/// Complier supplied defaulted Move Assignment Operator for EventSubscriber 
		/// </summary>
		/// <param name="rhs"> Takes a rvalue reference to an EventSubscriber </param>
		/// <returns> Returns a reference to an EventSubscriber </returns>
		EventSubscriber& operator=(EventSubscriber&& rhs) noexcept = default;

		/// <summary>
		/// Virtual defaulted Destructor for EventSubscriber
		/// </summary>
		virtual ~EventSubscriber() = default;
		
		/// <summary>
		/// Pure Virtual Notify method for EventSubscriber 
		/// </summary>
		/// <param name="publisher"> Takes a const reference to an EventPublisher </param>
		virtual void Notify(const EventPublisher& publisher) = 0;
	};
}