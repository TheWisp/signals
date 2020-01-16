cmake_minimum_required(VERSION 3.9)
project (Signals)

add_executable(Signals 
	main.cpp
	signals.hpp
	Bench.h
)
set_target_properties(Signals PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)