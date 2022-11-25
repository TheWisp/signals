#pragma once

//
// Created by Anton Södergren on 2018-01-03.
//

#include <functional>
#include <vector>
#include <algorithm>


namespace observe
{
	// Observer with templates and function callbacks

	template<typename... Args>
	class subject;


	// Takes a function argument with Args arguments. Not meant to be inherited from.
	template<typename... Args>
	class observer
	{
	public:
		observer() = default;
		explicit observer(std::function<void(Args ...)> function);
		~observer();

		observer(const observer& other);
		observer& operator=(const observer& other);

		observer(observer&& other) noexcept;
		observer& operator=(observer&& other) noexcept;

		void set_function(std::function<void(Args ...)> function);

	private:
		void on_notify(Args ... args);
		void clear();

		std::function<void(Args ...)> function_{};
		std::vector<subject<Args...>*> subjects_{};

		friend class subject<Args...>;
	};


	// To be used together with Observer objects of the same template type.
	template<typename... Args>
	class subject
	{
	public:
		subject() = default;
		~subject();

		subject(const subject& other);
		subject& operator=(const subject& other);

		subject(subject&& other) noexcept;
		subject& operator=(subject&& other) noexcept;

		void add_observer(observer<Args...>& observer);
		void remove_observer(observer<Args...>& observer);
		void clear();
		void operator()(Args ... args);

	private:
		std::vector<observer<Args...>*> observers_{};
	};


	template<typename... Args>
	observer<Args...>::observer(std::function<void(Args ...)> function)
		: function_{move(function)} { }


	// Remove observer from all subjects when it gets destroyed
	template<typename ... Args>
	observer<Args...>::~observer()
	{
		clear();
	}


	template<typename ... Args>
	observer<Args...>::observer(const observer& other)
		: function_{other.function_} {}


	template<typename ... Args>
	observer<Args...>& observer<Args...>::operator=(const observer& other)
	{
		clear();
		function_ = other.function_;

		return *this;
	}


	template<typename ... Args>
	observer<Args...>::observer(observer&& other) noexcept
		: function_{move(other.function_)}
	{
		for(auto subject : other.subjects_)
		{
			subject->add_observer(*this);
		}
		other.clear();
	}


	template<typename ... Args>
	observer<Args...>& observer<Args...>::operator=(observer&& other) noexcept
	{
		function_ = move(other.function_);

		clear();
		for(auto subject : other.subjects_)
		{
			subject->add_observer(*this);
		}
		other.clear();

		return *this;
	}


	template<typename ... Args>
	void observer<Args...>::set_function(std::function<void(Args ...)> function)
	{
		function_ = move(function);
	}


	template<typename ... Args>
	void observer<Args...>::on_notify(Args ... args)
	{
		function_(args...);
	}


	template<typename ... Args>
	void observer<Args...>::clear()
	{
		// Maybe this should just loop backwards instead?
		while(!subjects_.empty())
		{
			subjects_[0]->remove_observer(*this);
		}
	}


	template<typename ... Args>
	subject<Args...>::~subject()
	{
		// This could probably also be looped backwards
		for(auto observer : observers_)
		{
			observer->subjects_.erase(
				std::remove(observer->subjects_.begin(), observer->subjects_.end(), this),
				observer->subjects_.end());
		}
	}


	template<typename ... Args>
	subject<Args...>::subject(const subject& other)
	{
		for(auto observer : other.observers_)
		{
			add_observer(*observer);
		}
	}


	template<typename ... Args>
	subject<Args...>& subject<Args...>::operator=(const subject& other)
	{
		clear();
		for(auto observer : other.observers_)
		{
			add_observer(*observer);
		}

		return *this;
	}


	template<typename ... Args>
	subject<Args...>::subject(subject&& other) noexcept
	{
		for(auto observer : other.observers_)
		{
			add_observer(*observer);
		}
		other.clear();
	}


	template<typename ... Args>
	subject<Args...>& subject<Args...>::operator=(subject&& other) noexcept
	{
		clear();
		for(auto observer : other.observers_)
		{
			add_observer(*observer);
		}
		other.clear();

		return *this;
	}


	template<typename ... Args>
	void subject<Args...>::add_observer(observer<Args...>& observer)
	{
		if (auto iter = std::find(std::begin(observers_), std::end(observers_), &observer); iter != std::end(observers_)) return;

		observers_.push_back(&observer);
		observer.subjects_.push_back(this);
	}


	template<typename ... Args>
	void subject<Args...>::remove_observer(observer<Args...>& observer)
	{
		observers_.erase(std::remove(observers_.begin(), observers_.end(), &observer), observers_.end());
		observer.subjects_.erase(
			std::remove(observer.subjects_.begin(), observer.subjects_.end(), this),
			observer.subjects_.end());
	}


	template<typename ... Args>
	void subject<Args...>::clear()
	{
		for(auto observer : observers_)
		{
			observer->subjects_.erase(
				std::remove(observer->subjects_.begin(), observer->subjects_.end(), this),
				observer->subjects_.end());
		}
		observers_.clear();
	}


	template<typename ... Args>
	void subject<Args...>::operator()(Args ... args)
	{
		for(auto observer : observers_)
		{
			observer->on_notify(args...);
		}
	}
}
