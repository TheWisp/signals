// event.cpp
/*
  Transplanted from neogfx C++ GUI Library
  Copyright (c) 2015-2018 Leigh Johnston.  All Rights Reserved.
  
  This program is free software: you can redistribute it and / or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../include/neolib/neolib.hpp"
#include "../include/neolib/async_thread.hpp"
#include "../include/neolib/async_task.hpp"
#include "../include/neolib/timer.hpp"
#include "../include/neolib/event.hpp"

namespace neolib
{ 
	class async_event_queue::local_thread : public async_thread
	{
	public:
		local_thread(async_event_queue& aOwner) :
			async_thread{ "neolib::async_event_queue::local_thread" },
			iOwner{ aOwner }
		{
		}
	private:
		async_event_queue& iOwner;
	};

	async_event_queue::async_event_queue() :
		async_event_queue{ std::make_shared<local_thread>(*this) }
	{
		static_cast<async_thread&>(*iTask).start();
	}

	async_event_queue::async_event_queue(async_task& aTask) : 
		async_event_queue{ std::shared_ptr<async_task>{std::shared_ptr<async_task>{}, &aTask} }
	{
		std::lock_guard lg{ instance_mutex() };
		instance_ptrs().aliased = this;
	}

	async_event_queue::async_event_queue(std::shared_ptr<async_task> aTask) :
		iTask { aTask },
		iTimer {
			*aTask,
			[this](neolib::callback_timer& aTimer)
			{
				publish_events();
				if (!iEvents.empty() && !aTimer.waiting())
					aTimer.again();
				if (iCache.first && std::chrono::steady_clock::now() > iCache.second)
					iCache.first.reset();
			}, 10, false
		},
		iHaveThreadedCallbacks{ false },
		iTerminated{ false }
	{
	}

	async_event_queue::~async_event_queue()
	{
		exec();
		while (iHaveThreadedCallbacks)
			std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
		std::lock_guard lg{ instance_mutex() };
		if (instance_ptrs().aliased == this)
			instance_ptrs().aliased = nullptr;
	}

	std::shared_ptr<async_event_queue> async_event_queue::instance()
	{
		std::lock_guard lg{ instance_mutex() };
		if (instance_ptrs().aliased != nullptr)
			return std::shared_ptr<async_event_queue>{ std::shared_ptr<async_event_queue>{}, instance_ptrs().aliased };
		if (!instance_ptrs().counted.expired())
			return instance_ptrs().counted.lock();
		auto instanceRef = std::make_shared<async_event_queue>();
		instance_ptrs().counted = instanceRef;
		return instanceRef;
	}

	std::recursive_mutex& async_event_queue::instance_mutex()
	{
		static std::recursive_mutex sMutex;
		return sMutex;
	}

	async_event_queue::instance_pointers& async_event_queue::instance_ptrs()
	{
		static instance_pointers sInstancePtrs;
		return sInstancePtrs;
	}

	bool async_event_queue::exec()
	{
		bool didSome = false;
		if (iHaveThreadedCallbacks)
		{
			callback_list work;
			{
				destroyable_mutex_lock_guard<event_mutex> guard{ iThreadedCallbacksMutex };
				work = std::move(iThreadedCallbacks[std::this_thread::get_id()]);
				iThreadedCallbacks.erase(iThreadedCallbacks.find(std::this_thread::get_id()));
				iHaveThreadedCallbacks = !iThreadedCallbacks.empty();
			}
			for (auto& cb : work)
			{
				if (iTerminated)
					return didSome;
				cb();
				didSome = true;
			}
		}
		return didSome;
	}

	void async_event_queue::terminate()
	{
		destroyable_mutex_lock_guard<event_mutex> guard{ iThreadedCallbacksMutex };
		iTerminated = true;
		iEvents.clear();
		if (iTimer.waiting())
			iTimer.cancel();
		iThreadedCallbacks.clear();
		iHaveThreadedCallbacks = false;
	}

	void async_event_queue::persist(std::shared_ptr<async_event_queue> aPtr, uint32_t aDuration_ms)
	{
		iCache.first = aPtr;
		iCache.second = std::chrono::steady_clock::now() + std::chrono::milliseconds(aDuration_ms);
	}

	void async_event_queue::enqueue_to_thread(std::thread::id aThreadId, callback aCallback)
	{
		if (iTerminated)
			return;
		destroyable_mutex_lock_guard<event_mutex> guard{ iThreadedCallbacksMutex };
		iThreadedCallbacks[aThreadId].push_back(aCallback);
		iHaveThreadedCallbacks = true;
	}

	void async_event_queue::add(const void* aEvent, callback aCallback, neolib::lifetime::destroyed_flag aDestroyedFlag)
	{
		if (iTerminated)
			return;
		destroyable_mutex_lock_guard<event_mutex> guard{ iEventsMutex };
		iEvents.emplace(aEvent, std::make_pair(aCallback, aDestroyedFlag));
		if (!iTimer.waiting())
			iTimer.again();
	}

	void async_event_queue::remove(const void* aEvent)
	{
		if (iTerminated)
			return;
		event_list toPublish;
		{
			destroyable_mutex_lock_guard<event_mutex> guard{ iEventsMutex };
			auto events = iEvents.equal_range(aEvent);
			if (events.first == events.second)
				throw event_not_found();
			toPublish.insert(events.first, events.second);
			iEvents.erase(events.first, events.second);
		}
		for (auto& e : toPublish)
			if (!e.second.second)
				e.second.first();
	}

	bool async_event_queue::has(const void* aEvent) const
	{
		destroyable_mutex_lock_guard<event_mutex> guard{ iEventsMutex };
		return iEvents.find(aEvent) != iEvents.end();
	}

	void async_event_queue::publish_events()
	{
		if (iTerminated)
			return;
		event_list toPublish;
		{
			destroyable_mutex_lock_guard<event_mutex> guard{ iEventsMutex };
			toPublish.swap(iEvents);
		}
		for (auto& e : toPublish)
			if (!e.second.second)
				e.second.first();
	}
}