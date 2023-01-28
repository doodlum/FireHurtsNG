cmake_minimum_required(VERSION 3.21)

project(
	FireHurtsRE
	VERSION 1.0.3
	LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(XSEPlugin)

target_include_directories(
	"${PROJECT_NAME}"
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/cmake
		${CMAKE_CURRENT_SOURCE_DIR}/src
		${SIMPLEINI_INCLUDE_DIRS}
)

find_package(tomlplusplus REQUIRED CONFIG)
find_package(jsoncpp REQUIRED CONFIG)

target_link_libraries(
	${PROJECT_NAME} 
	PUBLIC 
		CommonLibSSE::CommonLibSSE

	PRIVATE
			tomlplusplus::tomlplusplus
			JsonCpp::JsonCpp 
)