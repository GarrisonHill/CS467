cmake_minimum_required(VERSION 3.9)
project(PEX1_Client)
#project(PEX1_Server)

set(CMAKE_C_STANDARD 11)


# Turn on helpful compiler warnings
#set(CMAKE_C_FLAGS "-Wall -Werror") # all warnings are on and treated as errors

# Set the source directory, the place where your main.c and
# other files are located
set(SDIR PEX1Client/)
#set(SDIR PEX1Server/)

#set(SDIR Labs/Lab35/)

# List all files that are required to compile your project
set(SOURCE_FILES
#        ${SDIR}CountedMap.c
#        ${SDIR}WordCloud.c
        #${SDIR}ternaryTreeUnitTest.c
        ${SDIR}PEX1Client.c)
#        ${SDIR}PEX1Server.c)

#        ${SDIR}pa3functs.c
#        ${SDIR}pa3main.c)
#        ${SDIR}pex2.c)
#        ${SDIR}finalProj.c
#        ${SDIR}finalProjFunctions.c)

# Tell cmake the name of your executable and what source files to
# build from
add_executable(PEX1_Client ${SOURCE_FILES})
#add_executable(PEX1_Server ${SOURCE_FILES})
