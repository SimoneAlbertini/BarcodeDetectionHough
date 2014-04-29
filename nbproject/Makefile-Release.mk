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
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/src/ArtelabDataset.o \
	${OBJECTDIR}/src/HoughTransform.o \
	${OBJECTDIR}/src/ImageProcessor.o \
	${OBJECTDIR}/src/MLP.o \
	${OBJECTDIR}/src/TimeCounter.o \
	${OBJECTDIR}/src/WueinsterDataset.o \
	${OBJECTDIR}/src/accuracy.o \
	${OBJECTDIR}/utils-3rdparty/DirectoryInfo.o \
	${OBJECTDIR}/utils-3rdparty/FileInfo.o \
	${OBJECTDIR}/utils-3rdparty/utils.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/barcodedetectionhough

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/barcodedetectionhough: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/barcodedetectionhough ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/src/ArtelabDataset.o: src/ArtelabDataset.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ArtelabDataset.o src/ArtelabDataset.cpp

${OBJECTDIR}/src/HoughTransform.o: src/HoughTransform.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/HoughTransform.o src/HoughTransform.cpp

${OBJECTDIR}/src/ImageProcessor.o: src/ImageProcessor.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ImageProcessor.o src/ImageProcessor.cpp

${OBJECTDIR}/src/MLP.o: src/MLP.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/MLP.o src/MLP.cpp

${OBJECTDIR}/src/TimeCounter.o: src/TimeCounter.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TimeCounter.o src/TimeCounter.cpp

${OBJECTDIR}/src/WueinsterDataset.o: src/WueinsterDataset.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/WueinsterDataset.o src/WueinsterDataset.cpp

${OBJECTDIR}/src/accuracy.o: src/accuracy.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/accuracy.o src/accuracy.cpp

${OBJECTDIR}/utils-3rdparty/DirectoryInfo.o: utils-3rdparty/DirectoryInfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/utils-3rdparty
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/utils-3rdparty/DirectoryInfo.o utils-3rdparty/DirectoryInfo.cpp

${OBJECTDIR}/utils-3rdparty/FileInfo.o: utils-3rdparty/FileInfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/utils-3rdparty
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/utils-3rdparty/FileInfo.o utils-3rdparty/FileInfo.cpp

${OBJECTDIR}/utils-3rdparty/utils.o: utils-3rdparty/utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/utils-3rdparty
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/utils-3rdparty/utils.o utils-3rdparty/utils.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/barcodedetectionhough

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
