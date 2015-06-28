#--------------------------------------------------------
# The CMakeLists.txt for:                       pTurtlebot
# Author(s):                              Mukul K Singh
#--------------------------------------------------------

# FILE(GLOB SRC *.cpp)

SET(SRC
  Turtlebot.cpp
  Turtlebot_Info.cpp
  create_robot.cpp
  main.cpp
)



ADD_EXECUTABLE(pTurtlebot ${SRC})

TARGET_LINK_LIBRARIES(pTurtlebot
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

