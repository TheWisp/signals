// timer.cpp
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
#include "../include/neolib/timer.hpp"

namespace neolib
{
	timer::timer(async_task& aIoTask, uint32_t aDuration_ms, bool aInitialWait) :
		iIoTask(aIoTask),
		iHandlerProxy(new handler_proxy(*this)),
		iTimerObject(aIoTask.timer_io_service().native_object()),
		iDuration_ms(aDuration_ms), 
		iEnabled(true),
		iWaiting(false), 
		iInReady(false)
	{
		if (aInitialWait)
			again();
	}

	timer::timer(const timer& aOther) :
		iIoTask(aOther.iIoTask),
		iHandlerProxy(new handler_proxy(*this)),
		iTimerObject(aOther.iIoTask.timer_io_service().native_object()),
		iDuration_ms(aOther.iDuration_ms), 
		iEnabled(aOther.iEnabled),
		iWaiting(false), 
		iInReady(false)
	{
		if (aOther.waiting())
			again();
	}
	
	timer& timer::operator=(const timer& aOther)
	{
		if (waiting())
			cancel();
		iDuration_ms = aOther.iDuration_ms;
		iEnabled = aOther.iEnabled;
		if (aOther.waiting())
			again();
		return *this;
	}
	
	timer::~timer()
	{
		cancel();
	}

	async_task& timer::owner_task() const
	{
		return iIoTask;
	}

	void timer::enable(bool aWait)
	{
		if (iEnabled)
			throw already_enabled();
		iEnabled = true;
		if (aWait)
			again();
	}

	void timer::disable()
	{
		if (!iEnabled)
			throw already_disabled();
		if (waiting())
			cancel();
		iEnabled = false;
	}

	bool timer::enabled() const
	{
		return iEnabled;
	}

	bool timer::disabled() const
	{
		return !iEnabled;
	}

	void timer::again()
	{
		if (disabled())
			enable(false);
		if (waiting())
			throw already_waiting();
		iTimerObject.expires_from_now(boost::posix_time::milliseconds(iDuration_ms));
		iTimerObject.async_wait(boost::bind(&handler_proxy::operator(), iHandlerProxy, boost::asio::placeholders::error));
		iWaiting = true;
	}

	void timer::again_if()
	{
		if (!waiting())
			again();
	}

	void timer::cancel()
	{
		if (!waiting())
			return;
		iHandlerProxy->orphan();
		iTimerObject.cancel();
	}

	void timer::reset()
	{
		cancel();
		again();
	}

	bool timer::waiting() const
	{
		return iWaiting;
	}

	uint32_t timer::duration() const
	{
		return iDuration_ms;
	}

	void timer::set_duration(uint32_t aDuration_ms, bool aEffectiveImmediately)
	{
		iDuration_ms = aDuration_ms;
		if (aEffectiveImmediately && waiting())
		{
			neolib::lifetime::destroyed_flag destroyed{ *this };
			cancel();
			if (destroyed)
				return;
			again();
		}
	}

	uint32_t timer::duration_ms() const
	{
		return iDuration_ms;
	}

	void timer::handler(const boost::system::error_code& aError)
	{
		bool ok = !aError && enabled();
		if (ok && iInReady && !waiting())
		{
			again();
			return;
		}
		iWaiting = false;
		if (ok)
		{
			try
			{
				iInReady = true;
				neolib::lifetime::destroyed_flag destroyed{ *this };
				if (std::uncaught_exceptions() == 0)
					ready();
				else
					again();
				if (destroyed)
					return;
				iInReady = false;
			}
			catch (...)
			{
				iInReady = false;
				throw;
			}
		}
	}

	callback_timer::callback_timer(async_task& aIoTask, std::function<void(callback_timer&)> aCallback, uint32_t aDuration_ms, bool aInitialWait) :
		timer(aIoTask, aDuration_ms, aInitialWait),
		iCallback(aCallback)
	{
	}

	callback_timer::~callback_timer()
	{
		cancel();
	}

	void callback_timer::ready()
	{
		iCallback(*this);
	}
}