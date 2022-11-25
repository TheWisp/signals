cmake_minimum_required(VERSION 3.9)
project (TestApp)

add_executable(${PROJECT_NAME})
add_executable(FTengSignals::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	OUTPUT_NAME FTengSignals-${PROJECT_NAME}
)

target_include_directories(${PROJECT_NAME}
	PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/include/
	$<TARGET_PROPERTY:FTengSignals::Signals,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE
	FTengSignals::Signals
)

set(HEADERS
	include/Bench.h
)
set(SOURCES
	src/main.cpp
)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/include/
	PREFIX ""
	FILES ${HEADERS}
)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src/
	PREFIX ""
	FILES ${SOURCES}
)
target_sources(${PROJECT_NAME}
	PRIVATE
	${SOURCES}
	)

if(NOT FTENG_SIGNALS_BUNDLED)
	install(TARGETS ${PROJECT_NAME}
		EXPORT ${FTengSignals_TARGETS}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	)
endif()

