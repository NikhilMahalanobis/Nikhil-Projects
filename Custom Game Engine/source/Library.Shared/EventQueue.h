#pragma once

#include "Event.h"
#include "GameState.h"

namespace FieaGameEngine
{
	class EventQueue final
	{
	public:
		/// <summary>
		/// Typecast constructor for EventQueue
		/// </summary>
		/// <param name="gameTime"> Takes a reference to a GameTime </param>
		EventQueue(GameTime& gameTime);

		/// <summary>
		/// Compiler deleted Copy Constructor for Event
		/// </summary>
		/// <param name="rhs"></param>
		EventQueue(const EventQueue& rhs) = delete;

		/// <summary>
		/// Compiler deleted Copy Assignment Operator 
		/// </summary>
		/// <param name="rhs"> Takes a const reference to an EventQueue </param>
		/// <returns> Returns a reference to an EventQueue </returns>
		EventQueue& operator=(const EventQueue& rhs) = delete;

		/// <summary>
		/// Compiler supplied defaulted Move Constructor for EventQueue
		/// </summary>
		/// <param name="rhs"> Takes a rvalue reference to and EventQueue </param>
		/// <returns></returns>
		EventQueue(EventQueue&& rhs) noexcept = default;

		/// <summary>
		/// Compiler supplied defaulted Move Assignment Operator for EventQueue 
		/// </summary>
		/// <param name="rhs"> Takes a rvalue reference to an EventQueue </param>
		/// <returns> Returns a reference to an EventQueue </returns>
		EventQueue& operator=(EventQueue&& rhs) noexcept = default;

		/// <summary>
		/// Compiler supplied defaulted Destructor for EventQueue 
		/// </summary>
		~EventQueue() = default;

		/// <summary>
		/// Update method for EventQueue
		/// </summary>
		/// <param name="gameState"> Takes a pointer to a GameState </param>
		void Update(GameState* gameState);

		/// <summary>
		/// Enqueue method for EventQueue
		/// </summary>
		/// <param name="publisher"> Takes a copy of a shared_ptr<EventPublisher> </param>
		/// <param name="delay"> Takes a copy of a std::chrono::milliseconds </param>
		void Enqueue(std::shared_ptr<EventPublisher> publisher, std::chrono::milliseconds delay = std::chrono::milliseconds(0));

		/// <summary>
		/// Clear method for EventQueue 
		/// </summary>
		void Clear();

		/// <summary>
		/// IsEmpty method for EventQueue 
		/// </summary>
		/// <returns></returns>
		bool IsEmpty() const;

		/// <summary>
		/// Size method for EventQueue
		/// </summary>
		/// <returns> Returns a copy of a size_t </returns>
		size_t Size() const;

	private:
		
		struct EventQueueEntry {
			std::shared_ptr<EventPublisher> _publisher;
			std::chrono::milliseconds _delay;
			std::chrono::high_resolution_clock::time_point _timeInserted;

			EventQueueEntry(std::shared_ptr<EventPublisher> publisher, const std::chrono::milliseconds& delay, std::chrono::high_resolution_clock::time_point timeInserted) :
				_publisher(std::move(publisher)), _delay(delay), _timeInserted(timeInserted)
			{}


			std::chrono::high_resolution_clock::time_point TimeEnqueued() const
			{
				return _timeInserted;
			}

			std::chrono::milliseconds Delay() const
			{
				return _delay;
			}

			bool IsExpired(std::chrono::high_resolution_clock::time_point currentTime) const
			{
				return (_timeInserted + _delay) <= currentTime;
			}
		};

		Vector<EventQueueEntry> _eventQueue;

		GameTime* _currentGameTime;
	};
}