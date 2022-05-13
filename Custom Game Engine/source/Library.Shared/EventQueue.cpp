#include "EventQueue.h"

namespace FieaGameEngine
{
	EventQueue::EventQueue(GameTime& gameTime) :
		_currentGameTime(&gameTime)
	{
	}

	void EventQueue::Update(GameState* gameState)
	{
		gameState;

		auto it = std::partition(_eventQueue.begin(), _eventQueue.end(), [this](EventQueueEntry entry) { return !entry.IsExpired(_currentGameTime->CurrentTime()); });

		for (auto newIt = it; newIt != _eventQueue.end(); ++newIt)
		{
			(*newIt)._publisher->Deliver();
		}

		if (it != _eventQueue.end())
		{
			_eventQueue.Remove(it, _eventQueue.end());
		}
	}

	void EventQueue::Enqueue(std::shared_ptr<EventPublisher> publisher, std::chrono::milliseconds delay)
	{
		EventQueueEntry entry(std::move(publisher), delay, _currentGameTime->CurrentTime());
		_eventQueue.PushBack(entry);
	}

	void EventQueue::Clear()
	{
		_eventQueue.Clear();
	}

	bool EventQueue::IsEmpty() const
	{
		return _eventQueue.IsEmpty();
	}

	size_t EventQueue::Size() const
	{
		return _eventQueue.Size();
	}
}