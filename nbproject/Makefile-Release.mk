#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-Release.mk)" "nbproject/Makefile-local-Release.mk"
include nbproject/Makefile-local-Release.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=Release
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/HardMPU.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/HardMPU.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=hardmpu.c midi.c mpu401.c pic.c main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/hardmpu.o ${OBJECTDIR}/midi.o ${OBJECTDIR}/mpu401.o ${OBJECTDIR}/pic.o ${OBJECTDIR}/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/hardmpu.o.d ${OBJECTDIR}/midi.o.d ${OBJECTDIR}/mpu401.o.d ${OBJECTDIR}/pic.o.d ${OBJECTDIR}/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/hardmpu.o ${OBJECTDIR}/midi.o ${OBJECTDIR}/mpu401.o ${OBJECTDIR}/pic.o ${OBJECTDIR}/main.o

# Source Files
SOURCEFILES=hardmpu.c midi.c mpu401.c pic.c main.c

# Pack Options 
PACK_COMPILER_OPTIONS=-I "${DFP_DIR}/include"
PACK_COMMON_OPTIONS=-B "${DFP_DIR}/gcc/dev/atmega4809"



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-Release.mk ${DISTDIR}/HardMPU.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATmega4809
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/hardmpu.o: hardmpu.c  .generated_files/flags/Release/1ec880b1cfe333c1c61afadab9ab5ffb09f13cd6 .generated_files/flags/Release/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/hardmpu.o.d 
	@${RM} ${OBJECTDIR}/hardmpu.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mrelax -funsigned-char -funsigned-bitfields -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/hardmpu.o.d" -MT "${OBJECTDIR}/hardmpu.o.d" -MT ${OBJECTDIR}/hardmpu.o  -o ${OBJECTDIR}/hardmpu.o hardmpu.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -frename-registers -g2
	
${OBJECTDIR}/midi.o: midi.c  .generated_files/flags/Release/67206546e8c78d26d73458c641bd5e5a951c84b .generated_files/flags/Release/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/midi.o.d 
	@${RM} ${OBJECTDIR}/midi.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mrelax -funsigned-char -funsigned-bitfields -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/midi.o.d" -MT "${OBJECTDIR}/midi.o.d" -MT ${OBJECTDIR}/midi.o  -o ${OBJECTDIR}/midi.o midi.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -frename-registers -g2
	
${OBJECTDIR}/mpu401.o: mpu401.c  .generated_files/flags/Release/83cc062b3272875cf15cb8f09795ed19621b71bd .generated_files/flags/Release/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/mpu401.o.d 
	@${RM} ${OBJECTDIR}/mpu401.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mrelax -funsigned-char -funsigned-bitfields -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/mpu401.o.d" -MT "${OBJECTDIR}/mpu401.o.d" -MT ${OBJECTDIR}/mpu401.o  -o ${OBJECTDIR}/mpu401.o mpu401.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -frename-registers -g2
	
${OBJECTDIR}/pic.o: pic.c  .generated_files/flags/Release/70f14d21287f6fffe44ce7659fa1735baced2182 .generated_files/flags/Release/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pic.o.d 
	@${RM} ${OBJECTDIR}/pic.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mrelax -funsigned-char -funsigned-bitfields -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/pic.o.d" -MT "${OBJECTDIR}/pic.o.d" -MT ${OBJECTDIR}/pic.o  -o ${OBJECTDIR}/pic.o pic.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -frename-registers -g2
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/Release/4914d1fd97c7df884972a782645d83b5ee2a70f2 .generated_files/flags/Release/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mrelax -funsigned-char -funsigned-bitfields -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/main.o.d" -MT "${OBJECTDIR}/main.o.d" -MT ${OBJECTDIR}/main.o  -o ${OBJECTDIR}/main.o main.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -frename-registers -g2
	
else
${OBJECTDIR}/hardmpu.o: hardmpu.c  .generated_files/flags/Release/72a8b8a81427990ef40e8fc398d2c0d73447a439 .generated_files/flags/Release/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/hardmpu.o.d 
	@${RM} ${OBJECTDIR}/hardmpu.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mrelax -funsigned-char -funsigned-bitfields -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/hardmpu.o.d" -MT "${OBJECTDIR}/hardmpu.o.d" -MT ${OBJECTDIR}/hardmpu.o  -o ${OBJECTDIR}/hardmpu.o hardmpu.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -frename-registers -g2
	
${OBJECTDIR}/midi.o: midi.c  .generated_files/flags/Release/ed7a7f68fc746c48445480eb4969bc99d01e86f4 .generated_files/flags/Release/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/midi.o.d 
	@${RM} ${OBJECTDIR}/midi.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mrelax -funsigned-char -funsigned-bitfields -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/midi.o.d" -MT "${OBJECTDIR}/midi.o.d" -MT ${OBJECTDIR}/midi.o  -o ${OBJECTDIR}/midi.o midi.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -frename-registers -g2
	
${OBJECTDIR}/mpu401.o: mpu401.c  .generated_files/flags/Release/dfd2ae1e9ce1673b7e6589ae00b327aec399100a .generated_files/flags/Release/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/mpu401.o.d 
	@${RM} ${OBJECTDIR}/mpu401.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mrelax -funsigned-char -funsigned-bitfields -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/mpu401.o.d" -MT "${OBJECTDIR}/mpu401.o.d" -MT ${OBJECTDIR}/mpu401.o  -o ${OBJECTDIR}/mpu401.o mpu401.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -frename-registers -g2
	
${OBJECTDIR}/pic.o: pic.c  .generated_files/flags/Release/681ddeac56b6ff04d5464fbf092b632c47817cd4 .generated_files/flags/Release/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pic.o.d 
	@${RM} ${OBJECTDIR}/pic.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mrelax -funsigned-char -funsigned-bitfields -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/pic.o.d" -MT "${OBJECTDIR}/pic.o.d" -MT ${OBJECTDIR}/pic.o  -o ${OBJECTDIR}/pic.o pic.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -frename-registers -g2
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/Release/3c66b07b1e1b3e6282f00b49ab08e1789764dc67 .generated_files/flags/Release/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mrelax -funsigned-char -funsigned-bitfields -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/main.o.d" -MT "${OBJECTDIR}/main.o.d" -MT ${OBJECTDIR}/main.o  -o ${OBJECTDIR}/main.o main.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -frename-registers -g2
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/HardMPU.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mmcu=atmega4809    -gdwarf-2 -D__$(MP_PROCESSOR_OPTION)__  -Wl,-Map="${DISTDIR}/HardMPU.${IMAGE_TYPE}.map"    -o ${DISTDIR}/HardMPU.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_Release=$(CND_CONF)  -mrelax $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1 -Wl,--gc-sections -Wl,--start-group  -Wl,-lm -Wl,-lm -Wl,--end-group 
	
	${MP_CC_DIR}/avr-objcopy -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex "${DISTDIR}/HardMPU.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/HardMPU.${IMAGE_TYPE}.eep" || exit 0
	${MP_CC_DIR}/avr-objdump -h -S "${DISTDIR}/HardMPU.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" > "${DISTDIR}/HardMPU.${IMAGE_TYPE}.lss"
	${MP_CC_DIR}/avr-objcopy -O srec -R .eeprom -R .fuse -R .lock -R .signature "${DISTDIR}/HardMPU.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/HardMPU.${IMAGE_TYPE}.srec"
	
	
else
${DISTDIR}/HardMPU.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mmcu=atmega4809   -D__$(MP_PROCESSOR_OPTION)__  -Wl,-Map="${DISTDIR}/HardMPU.${IMAGE_TYPE}.map"    -o ${DISTDIR}/HardMPU.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_Release=$(CND_CONF)  -mrelax $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION) -Wl,--gc-sections -Wl,--start-group  -Wl,-lm -Wl,-lm -Wl,--end-group 
	${MP_CC_DIR}/avr-objcopy -O ihex "${DISTDIR}/HardMPU.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/HardMPU.${IMAGE_TYPE}.hex"
	${MP_CC_DIR}/avr-objcopy -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex "${DISTDIR}/HardMPU.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/HardMPU.${IMAGE_TYPE}.eep" || exit 0
	${MP_CC_DIR}/avr-objdump -h -S "${DISTDIR}/HardMPU.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" > "${DISTDIR}/HardMPU.${IMAGE_TYPE}.lss"
	${MP_CC_DIR}/avr-objcopy -O srec -R .eeprom -R .fuse -R .lock -R .signature "${DISTDIR}/HardMPU.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/HardMPU.${IMAGE_TYPE}.srec"
	
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
