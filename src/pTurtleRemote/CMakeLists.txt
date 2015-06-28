set(EXEC pTurtleRemote)

# next few lines are simply to insert icons into
# the console application on windows
IF(WIN32)
    SET(RESOURCES icon1.ico pTurtleRemote.rc)
ENDIF(WIN32)

FIND_PACKAGE(MOOS 10.0)

#what files are needed?
SET(SRCS MOOSTurtleRemote.cpp  pTurtleRemoteMain.cpp  ${RESOURCES})

#where to look for help
INCLUDE_DIRECTORIES( {MOOS_INCLUDE_DIRS} ${MOOS_DEPEND_INCLUDE_DIRS} )

# Add executable called pLogger
ADD_EXECUTABLE( ${EXEC} ${SRCS})

# indicate how to link
TARGET_LINK_LIBRARIES(${EXEC}   ${MOOS_LIBRARIES} ${MOOS_DEPEND_LIBRARIES}  )

#install directive
INSTALL(TARGETS ${EXEC} RUNTIME DESTINATION bin )

