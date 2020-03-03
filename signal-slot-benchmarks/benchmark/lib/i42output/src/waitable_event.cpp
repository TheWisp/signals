// waitable_event.cpp
/*
 *  Copyright (c) 2007 Leigh Johnston.
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     * Neither the name of Leigh Johnston nor the names of any
 *       other contributors to this software may be used to endorse or
 *       promote products derived from this software without specific prior
 *       written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "../include/neolib/neolib.hpp"
#include <chrono>
#include "../include/neolib/waitable_event.hpp"
#include "../include/neolib/thread.hpp"

namespace neolib
{
	waitable_event::waitable_event() : iReady(false), iTotalWaiting(0)
	{
	}

	void waitable_event::signal_one() const
	{
		std::lock_guard<std::mutex> lock(iMutex);
		iReady = true;
		iSignalType = SignalOne;
		iCondVar.notify_one();
	}

	void waitable_event::signal_all() const
	{
		std::lock_guard<std::mutex> lock(iMutex);
		iReady = true;
		iSignalType = SignalAll;
		iCondVar.notify_all();
	}

	void waitable_event::wait() const
	{
		std::unique_lock<std::mutex> lock(iMutex);
		++iTotalWaiting;
		while (!iReady)
			iCondVar.wait(lock);
		--iTotalWaiting;
		if (iSignalType == SignalOne || iTotalWaiting == 0)
			iReady = false;
	}

	bool waitable_event::wait(uint32_t aTimeout_ms) const
	{
		bool result = true;
		std::unique_lock<std::mutex> lock(iMutex);
		++iTotalWaiting;
		if (!iReady)
			result = (iCondVar.wait_for(lock, std::chrono::milliseconds(aTimeout_ms)) == std::cv_status::no_timeout);
		--iTotalWaiting;
		if (result && iSignalType == SignalOne || iTotalWaiting == 0)
			iReady = false;
		return result;
	}

	bool waitable_event::msg_wait(const message_queue& aMessageQueue) const
	{
		for(;;)
		{
			if (wait(0))
				return true;
			else if (aMessageQueue.have_message())
				return false;
			thread::yield();
		}
	}

	bool waitable_event::msg_wait(const message_queue& aMessageQueue, uint32_t aTimeout_ms) const
	{
		auto startTime = std::chrono::steady_clock::now();
		for(;;)
		{
			if (wait(0))
				return true;
			else if (aMessageQueue.have_message())
				return false;
			else if (std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - startTime).count() > aTimeout_ms)
				return false;
			thread::yield();
		}
	}

	void waitable_event::reset() const
	{
		std::lock_guard<std::mutex> lock(iMutex);
		iReady = false;
	}

	wait_result waitable_event_list::wait() const
	{
		for(;;)
		{
			for (list_type::const_iterator i = iEvents.begin(); i != iEvents.end(); ++i)
				if ((**i).wait(0))
					return wait_result_event(**i);
			thread::yield();
		}
	}

	wait_result waitable_event_list::wait(const waitable& aWaitable) const
	{
		for(;;)
		{
			for (list_type::const_iterator i = iEvents.begin(); i != iEvents.end(); ++i)
				if ((**i).wait(0))
					return wait_result_event(**i);
			if (aWaitable.waitable_ready())
				return wait_result_waitable();
			thread::yield();
		}
	}

	wait_result waitable_event_list::msg_wait(const message_queue& aMessageQueue) const
	{
		for(;;)
		{
			for (list_type::const_iterator i = iEvents.begin(); i != iEvents.end(); ++i)
				if ((**i).wait(0))
					return wait_result_event(**i);
			if (aMessageQueue.have_message())
				return wait_result_message();
			thread::yield();
		}
	}

	wait_result waitable_event_list::msg_wait(const message_queue& aMessageQueue, const waitable& aWaitable) const
	{
		for(;;)
		{
			for (list_type::const_iterator i = iEvents.begin(); i != iEvents.end(); ++i)
				if ((**i).wait(0))
					return wait_result_event(**i);
			if (aMessageQueue.have_message())
				return wait_result_message();
			if (aWaitable.waitable_ready())
				return wait_result_waitable();
			thread::yield();
		}
	}
} // namespace neolib
