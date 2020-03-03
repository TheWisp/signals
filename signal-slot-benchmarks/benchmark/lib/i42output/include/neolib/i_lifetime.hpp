// i_lifetime.hpp
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

namespace neolib
{
	class i_lifetime_flag
	{
	public:
		virtual ~i_lifetime_flag() {}
	public:
		virtual bool is_creating() const = 0;
		virtual bool is_alive() const = 0;
		virtual bool is_destroying() const = 0;
		virtual bool is_destroyed() const = 0;
		virtual operator bool() const = 0;
		virtual void set_alive() = 0;
		virtual void set_destroying() = 0;
		virtual void set_destroyed() = 0;
	public:
		virtual bool debug() const = 0;
		virtual void set_debug(bool aDebug = true) = 0;
	};

	enum class lifetime_state
	{
		Creating,
		Alive,
		Destroying,
		Destroyed
	};

	class i_lifetime
	{
	public:
		struct not_creating : std::logic_error { not_creating() : std::logic_error("neolib::i_lifetime::not_creating") {} };
		struct already_destroyed : std::logic_error { already_destroyed() : std::logic_error("neolib::i_lifetime::already_destroyed") {} };
	public:
	public:
		virtual ~i_lifetime() {}
	public:
		virtual lifetime_state object_state() const = 0;
		virtual bool is_creating() const = 0;
		virtual bool is_alive() const = 0;
		virtual bool is_destroying() const = 0;
		virtual bool is_destroyed() const = 0;
		virtual void set_alive() = 0;
		virtual void set_destroying() = 0;
		virtual void set_destroyed() = 0;
	public:
		virtual void add_flag(i_lifetime_flag* aFlag) const = 0;
		virtual void remove_flag(i_lifetime_flag* aFlag) const = 0;
	};
}
