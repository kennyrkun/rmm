CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_REQUIRED_STANDARD ON)
SET(CMAKE_DEBUG_POSTFIX "-d")

SET(PROJECT_NAME "rmm-client")
PROJECT(${PROJECT_NAME} LANGUAGES CXX)

SET(HEADERS ./AppEngine.hpp ./AppState.hpp ./Client.hpp ./Network.hpp ./RoomOverviewState.hpp ./ServerConnectState.hpp ./VisualWorkstation.hpp)
SET(SOURCES ./AppEngine.cpp ./Client.cpp ./main.cpp ./Network.cpp ./RoomOverviewState.cpp ./ServerConnectState.cpp ./VisualWorkstation.cpp)
ADD_EXECUTABLE(${PROJECT_NAME} ${HEADERS} ${SOURCES})

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC ../SFML/include)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} sfml-graphics sfml-audio sfml-network sfml-window sfml-system -L"../SFML/lib")

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC ../SFUI/include)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} SFUI -L"../SFUI/lib")

TARGET_LINK_LIBRARIES(${PROJECT_NAME} stdc++fs)
