#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/BWTA/Source/BWTA.o \
	${OBJECTDIR}/BWTA/Source/BWTA_Result.o \
	${OBJECTDIR}/BWTA/Source/BalanceMetrics.o \
	${OBJECTDIR}/BWTA/Source/BaseLocationGenerator.o \
	${OBJECTDIR}/BWTA/Source/BaseLocationImpl.o \
	${OBJECTDIR}/BWTA/Source/ChokepointImpl.o \
	${OBJECTDIR}/BWTA/Source/ClosestObjectMap.o \
	${OBJECTDIR}/BWTA/Source/GraphColoring.o \
	${OBJECTDIR}/BWTA/Source/LoadData.o \
	${OBJECTDIR}/BWTA/Source/MapData.o \
	${OBJECTDIR}/BWTA/Source/Painter.o \
	${OBJECTDIR}/BWTA/Source/PolygonGenerator.o \
	${OBJECTDIR}/BWTA/Source/PolygonImpl.o \
	${OBJECTDIR}/BWTA/Source/RegionGenerator.o \
	${OBJECTDIR}/BWTA/Source/RegionImpl.o \
	${OBJECTDIR}/BWTA/Source/TerrainAnalysis.o \
	${OBJECTDIR}/BWTA/Source/Utils.o \
	${OBJECTDIR}/BWTA/Source/distanceTransform.o \
	${OBJECTDIR}/BWTA/Source/main.o \
	${OBJECTDIR}/BWTA/Source/pathfinding.o \
	${OBJECTDIR}/BWTA/Source/stdafx.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libBWTA2.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libBWTA2.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libBWTA2.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/BWTA/Source/BWTA.o: BWTA/Source/BWTA.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/BWTA.o BWTA/Source/BWTA.cpp

${OBJECTDIR}/BWTA/Source/BWTA_Result.o: BWTA/Source/BWTA_Result.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/BWTA_Result.o BWTA/Source/BWTA_Result.cpp

${OBJECTDIR}/BWTA/Source/BalanceMetrics.o: BWTA/Source/BalanceMetrics.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/BalanceMetrics.o BWTA/Source/BalanceMetrics.cpp

${OBJECTDIR}/BWTA/Source/BaseLocationGenerator.o: BWTA/Source/BaseLocationGenerator.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/BaseLocationGenerator.o BWTA/Source/BaseLocationGenerator.cpp

${OBJECTDIR}/BWTA/Source/BaseLocationImpl.o: BWTA/Source/BaseLocationImpl.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/BaseLocationImpl.o BWTA/Source/BaseLocationImpl.cpp

${OBJECTDIR}/BWTA/Source/ChokepointImpl.o: BWTA/Source/ChokepointImpl.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/ChokepointImpl.o BWTA/Source/ChokepointImpl.cpp

${OBJECTDIR}/BWTA/Source/ClosestObjectMap.o: BWTA/Source/ClosestObjectMap.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/ClosestObjectMap.o BWTA/Source/ClosestObjectMap.cpp

${OBJECTDIR}/BWTA/Source/GraphColoring.o: BWTA/Source/GraphColoring.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/GraphColoring.o BWTA/Source/GraphColoring.cpp

${OBJECTDIR}/BWTA/Source/LoadData.o: BWTA/Source/LoadData.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/LoadData.o BWTA/Source/LoadData.cpp

${OBJECTDIR}/BWTA/Source/MapData.o: BWTA/Source/MapData.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/MapData.o BWTA/Source/MapData.cpp

${OBJECTDIR}/BWTA/Source/Painter.o: BWTA/Source/Painter.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/Painter.o BWTA/Source/Painter.cpp

${OBJECTDIR}/BWTA/Source/PolygonGenerator.o: BWTA/Source/PolygonGenerator.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/PolygonGenerator.o BWTA/Source/PolygonGenerator.cpp

${OBJECTDIR}/BWTA/Source/PolygonImpl.o: BWTA/Source/PolygonImpl.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/PolygonImpl.o BWTA/Source/PolygonImpl.cpp

${OBJECTDIR}/BWTA/Source/RegionGenerator.o: BWTA/Source/RegionGenerator.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/RegionGenerator.o BWTA/Source/RegionGenerator.cpp

${OBJECTDIR}/BWTA/Source/RegionImpl.o: BWTA/Source/RegionImpl.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/RegionImpl.o BWTA/Source/RegionImpl.cpp

${OBJECTDIR}/BWTA/Source/TerrainAnalysis.o: BWTA/Source/TerrainAnalysis.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/TerrainAnalysis.o BWTA/Source/TerrainAnalysis.cpp

${OBJECTDIR}/BWTA/Source/Utils.o: BWTA/Source/Utils.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/Utils.o BWTA/Source/Utils.cpp

${OBJECTDIR}/BWTA/Source/distanceTransform.o: BWTA/Source/distanceTransform.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/distanceTransform.o BWTA/Source/distanceTransform.cpp

${OBJECTDIR}/BWTA/Source/main.o: BWTA/Source/main.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/main.o BWTA/Source/main.cpp

${OBJECTDIR}/BWTA/Source/pathfinding.o: BWTA/Source/pathfinding.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/pathfinding.o BWTA/Source/pathfinding.cpp

${OBJECTDIR}/BWTA/Source/stdafx.o: BWTA/Source/stdafx.cpp
	${MKDIR} -p ${OBJECTDIR}/BWTA/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BWTA/Source/stdafx.o BWTA/Source/stdafx.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
