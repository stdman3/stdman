cmake_minimum_required(VERSION 3.13)
project(stdman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(Boost REQUIRED)
find_package(glog CONFIG REQUIRED)
find_package(Threads)

link_libraries(glog::glog ${CMAKE_THREAD_LIBS_INIT})

include_directories(src)
add_executable(stdman src/main.cpp)

set(CPPREFERENCE_ARCHIVE_LINK https://github.com/stdman3/cppreference-archive-html/raw/master/html_book_20190607.tar.xz)
string(REGEX REPLACE "^(.*\/)" "" ZIPFILE ${CPPREFERENCE_ARCHIVE_LINK})
message(STATUS "Generate mange page with zip file:${ZIPFILE}")

set(MAN_FOLDER man3)
add_custom_target(generate
	COMMAND /bin/sh -c "if [ ! -f ${ZIPFILE} ]\; then wget ${CPPREFERENCE_ARCHIVE_LINK}\; fi"
	COMMAND echo "Uncompress zip file"
	COMMAND tar -xJf ${ZIPFILE} > /dev/null 2>&1
	COMMAND echo "Create folder ${MAN_FOLDER}"
	COMMAND /bin/sh -c "if [ -d ${MAN_FOLDER} ]\; then rm -rf ${MAN_FOLDER}\; fi"
	COMMAND mkdir ${MAN_FOLDER} tmp > /dev/null 2>&1 || (exit 0)
	COMMAND echo "Run stdman to generate"
	COMMAND find reference -print0 | xargs -0 -n 1 -P 4 -I % ${CMAKE_CURRENT_BINARY_DIR}/stdman -d ${MAN_FOLDER} % > /dev/null 2>&1 || (exit 0)
	COMMAND echo "Clean unneed files"
	COMMAND find ${MAN_FOLDER} -name '*.3' | grep -v 'std::' | paste -sd " " - | xargs rm || (exit 0)
	COMMAND echo "Create soft link for alias like std::string"
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/do_link ${MAN_FOLDER} || (exit 0)
)

# TODO read destination folder from env
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${MAN_FOLDER} DESTINATION /usr/local/man)
