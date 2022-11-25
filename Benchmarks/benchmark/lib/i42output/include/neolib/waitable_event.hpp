// waitable_event.hpp
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

#pragma once

#include "neolib.hpp"
#include <stdexcept>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
#include "message_queue.hpp"
#include "waitable.hpp"
#include "variant.hpp"

namespace neolib
{
	class waitable_event
	{
		// constants
	public:
		static const uint32_t ShortTimeout_ms = 20;
		// types
	private:
		enum signal_type
		{
			SignalOne,
			SignalAll
		};
		// construction
	public:
		waitable_event();
		// operations
	public:
		void signal_one() const;
		void signal_all() const;
		void wait() const;
		bool wait(uint32_t aTimeout_ms) const;
		bool msg_wait(const message_queue& aMessageQueue) const;
		bool msg_wait(const message_queue& aMessageQueue, uint32_t aTimeout_ms) const;
		void reset() const;
	private:
		mutable std::mutex iMutex;
		mutable std::condition_variable iCondVar;
		mutable bool iReady;
		mutable std::size_t iTotalWaiting;
		mutable signal_type iSignalType;
	};

	struct wait_result_event { wait_result_event(const waitable_event& aEvent) : iEvent(aEvent) {} const waitable_event& iEvent; };
	struct wait_result_message {};
	struct wait_result_waitable {};
	typedef variant<wait_result_event, wait_result_message, wait_result_waitable> wait_result;

	class waitable_event_list
	{
		// types
	private:
		typedef const waitable_event* event_pointer;
		typedef std::vector<event_pointer> list_type;

		// construction
	public:
		waitable_event_list(const waitable_event& aEvent)
		{
			iEvents.push_back(&aEvent);
		}
		template <typename InputIterator>
		waitable_event_list(InputIterator aFirst, InputIterator aLast)
		{
			while(aFirst != aLast)
				iEvents.push_back(&*aFirst++);
		}

		// operations
	public:
		wait_result wait() const;
		wait_result wait(const waitable& aWaitable) const;
		wait_result msg_wait(const message_queue& aMessageQueue) const;
		wait_result msg_wait(const message_queue& aMessageQueue, const waitable& aWaitable) const;

		// attributes
	private:
		mutable list_type iEvents;
	};
}
