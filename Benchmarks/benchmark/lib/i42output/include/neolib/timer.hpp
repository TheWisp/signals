// timer.hpp
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
#include <boost/bind.hpp>
#include "noncopyable.hpp"
#include "async_task.hpp"
#include "lifetime.hpp"

namespace neolib
{
	class timer : private noncopyable, public lifetime
	{
		// types
	public:
		struct already_waiting : std::logic_error { already_waiting() : std::logic_error("neolib::timer::already_waiting") {} };
		struct already_enabled : std::logic_error { already_enabled() : std::logic_error("neolib::timer::already_enabled") {} };
		struct already_disabled : std::logic_error { already_disabled() : std::logic_error("neolib::timer::already_disabled") {} };
		struct timer_destroyed : std::logic_error { timer_destroyed() : std::logic_error("neolib::timer::timer_destroyed") {} };
	private:
		class handler_proxy
		{
		public:
			handler_proxy(timer& aParent) : iParent(aParent), iOrphaned(false)
			{
			}
		public:
			void operator()(const boost::system::error_code& aError)
			{
				if (!iOrphaned)
					iParent.handler(aError);
			}
			void orphan(bool aCreateNewHandlerProxy = true)
			{
				iOrphaned = true;
				if (aCreateNewHandlerProxy)
					iParent.iHandlerProxy = std::make_shared<handler_proxy>(iParent);
				else
					iParent.iHandlerProxy.reset();
			}
		private:
			timer& iParent;
			bool iOrphaned;
		};
		// construction
	public:
		timer(async_task& aIoTask, uint32_t aDuration_ms, bool aInitialWait = true);
		timer(const timer& aOther);
		timer& operator=(const timer& aOther);
		virtual ~timer();
		// operations
	public:
		async_task& owner_task() const;
		void enable(bool aWait = true);
		void disable();
		bool enabled() const;
		bool disabled() const;
		void again();
		void again_if();
		void cancel();
		void reset();
		bool waiting() const;
		uint32_t duration() const;
		void set_duration(uint32_t aDuration_ms, bool aEffectiveImmediately = false);
		uint32_t duration_ms() const;
		// implementation
	private:
		void handler(const boost::system::error_code& aError);
		virtual void ready() = 0;
		// attributes
	private:
		async_task& iIoTask;
		std::shared_ptr<handler_proxy> iHandlerProxy;
		boost::asio::deadline_timer iTimerObject;
		uint32_t iDuration_ms;
		bool iEnabled;
		bool iWaiting;
		bool iInReady;
	};

	class callback_timer : public timer
	{
	public:
		callback_timer(async_task& aIoTask, std::function<void(callback_timer&)> aCallback, uint32_t aDuration_ms, bool aInitialWait = true);
		~callback_timer();
	private:
		virtual void ready();
	private:
		std::function<void(callback_timer&)> iCallback;
	};
}