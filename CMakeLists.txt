project(N_BODY_PROBLEM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

find_package(OpenGL REQUIRED)
find_package(PNG REQUIRED)
find_package(X11 REQUIRED)

set(EXEC_NAME n-body-problem)

add_executable(${EXEC_NAME} main.cpp Simulator.cpp Particles.cpp olcPixelGameEngine.cpp GravitySource.cpp)

target_include_directories(${EXEC_NAME} PUBLIC ${OPENGL_INCLUDE_DIR} ${PNG_INCLUDE_DIR} ${X11_INCLUDE_DIR})

target_link_libraries(${EXEC_NAME} ${OPENGL_LIBRARIES} ${PNG_LIBRARY} ${X11_LIBRARIES})