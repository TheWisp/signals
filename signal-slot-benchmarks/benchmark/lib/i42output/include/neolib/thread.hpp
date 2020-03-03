// thread.hpp v3.0
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
#include <memory>
#include <thread>
#include <atomic>
#include "noncopyable.hpp"
#include "lockable.hpp"
#include "waitable.hpp"
#include "waitable_event.hpp"
#include "i_thread.hpp"

namespace neolib
{
	class thread : public i_thread, public lockable, public waitable, private noncopyable
	{
		// types
	public:
		typedef std::thread::id id_type;
		typedef std::thread thread_object_type;
		// exceptions
	public:
		struct thread_not_started : public std::logic_error { thread_not_started() : std::logic_error("neolib::thread::thread_not_started") {} };
		struct thread_already_started : public std::logic_error { thread_already_started() : std::logic_error("neolib::thread::thread_already_started") {} };
		struct cannot_wait_on_self : public std::logic_error { cannot_wait_on_self() : std::logic_error("neolib::thread::cannot_wait_on_self") {} };
		struct no_thread_object : public std::logic_error { no_thread_object() : std::logic_error("neolib::thread::no_thread_object") {} };
		struct not_in_thread : public std::logic_error { not_in_thread() : std::logic_error("neolib::thread::not_in_thread") {} };
	private:
		typedef std::unique_ptr<thread_object_type> thread_object_pointer;
		enum state_e { ReadyToStart, Starting, Started, Finished, Aborted, Cancelled, Error };
	protected:
		struct cancellation {};
		// construction
	public:
		thread(const std::string& aName = "", bool aAttachToCurrentThread = false);
		virtual ~thread();
		// operations
	public:
		const std::string& name() const override;
		bool using_existing_thread() const;
		void start();
		void cancel();
		void abort(bool aWait = true);
		void wait() const;
		wait_result wait(const waitable_event_list& aEventList) const;
		bool msg_wait(const message_queue& aMessageQueue) const;
		wait_result msg_wait(const message_queue& aMessageQueue, const waitable_event_list& aEventList) const;
		void block();
		void unblock();
		bool started() const;
		bool running() const;
		bool finished() const override;
		bool aborted() const;
		bool cancelled() const;
		bool error() const;
		id_type id() const;
		bool in() const;
		bool blocked() const;
		bool has_thread_object() const;
		thread_object_type& thread_object() const;
		static void sleep(uint32_t aDelayInMilleseconds);
		static void yield();
		static uint64_t elapsed_ms();
		static uint64_t program_elapsed_ms();
		// implementation
	private:
		// from waitable
		virtual bool waitable_ready() const;
		// own
		void entry_point();
		// attributes
	private:
		const std::string iName;
		bool iUsingExistingThread;
		std::atomic<state_e> iState;
		thread_object_pointer iThreadObject;
		id_type iId;
		std::atomic<std::size_t> iBlockedCount;
	};
}
