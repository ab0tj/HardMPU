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
SOURCEFILES_QUOTED_IF_SPACED=hardmpu.c midi.c mpu401.c pic.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/hardmpu.o ${OBJECTDIR}/midi.o ${OBJECTDIR}/mpu401.o ${OBJECTDIR}/pic.o
POSSIBLE_DEPFILES=${OBJECTDIR}/hardmpu.o.d ${OBJECTDIR}/midi.o.d ${OBJECTDIR}/mpu401.o.d ${OBJECTDIR}/pic.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/hardmpu.o ${OBJECTDIR}/midi.o ${OBJECTDIR}/mpu401.o ${OBJECTDIR}/pic.o

# Source Files
SOURCEFILES=hardmpu.c midi.c mpu401.c pic.c

# Pack Options 
PACK_COMPILER_OPTIONS=-I "${DFP_DIR}/include"
PACK_COMMON_OPTIONS=-B "${DFP_DIR}/gcc/dev/atmega1284"



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

MP_PROCESSOR_OPTION=ATmega1284
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
${OBJECTDIR}/hardmpu.o: hardmpu.c  .generated_files/flags/Release/3b961ee985f3966b7613164d2aef160a80e9084c .generated_files/flags/Release/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/hardmpu.o.d 
	@${RM} ${OBJECTDIR}/hardmpu.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega1284   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mrelax -funsigned-char -funsigned-bitfields -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/hardmpu.o.d" -MT "${OBJECTDIR}/hardmpu.o.d" -MT ${OBJECTDIR}/hardmpu.o  -o ${OBJECTDIR}/hardmpu.o hardmpu.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -frename-registers -g2
	
${OBJECTDIR}/midi.o: midi.c  .generated_files/flags/Release/2d1579e11596889fd92ad8563d87b1445f95c3fd .generated_files/flags/Release/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/midi.o.d 
	@${RM} ${OBJECTDIR}/midi.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega1284   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mrelax -funsigned-char -funsigned-bitfields -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/midi.o.d" -MT "${OBJECTDIR}/midi.o.d" -MT ${OBJECTDIR}/midi.o  -o ${OBJECTDIR}/midi.o midi.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -frename-registers -g2
	
${OBJECTDIR}/mpu401.o: mpu401.c  .generated_files/flags/Release/b97842504aadee7226085a923a89be4143d23896 .generated_files/flags/Release/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/mpu401.o.d 
	@${RM} ${OBJECTDIR}/mpu401.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega1284   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mrelax -funsigned-char -funsigned-bitfields -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/mpu401.o.d" -MT "${OBJECTDIR}/mpu401.o.d" -MT ${OBJECTDIR}/mpu401.o  -o ${OBJECTDIR}/mpu401.o mpu401.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -frename-registers -g2
	
${OBJECTDIR}/pic.o: pic.c  .generated_files/flags/Release/a610e5fd88de93d44125c8c5f166fd15dd7c44b0 .generated_files/flags/Release/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pic.o.d 
	@${RM} ${OBJECTDIR}/pic.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega1284   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mrelax -funsigned-char -funsigned-bitfields -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/pic.o.d" -MT "${OBJECTDIR}/pic.o.d" -MT ${OBJECTDIR}/pic.o  -o ${OBJECTDIR}/pic.o pic.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -frename-registers -g2
	
else
${OBJECTDIR}/hardmpu.o: hardmpu.c  .generated_files/flags/Release/7e84972f8f662285f77ca4eacb251f0c126c2f6d .generated_files/flags/Release/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/hardmpu.o.d 
	@${RM} ${OBJECTDIR}/hardmpu.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega1284    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mrelax -funsigned-char -funsigned-bitfields -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/hardmpu.o.d" -MT "${OBJECTDIR}/hardmpu.o.d" -MT ${OBJECTDIR}/hardmpu.o  -o ${OBJECTDIR}/hardmpu.o hardmpu.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -frename-registers -g2
	
${OBJECTDIR}/midi.o: midi.c  .generated_files/flags/Release/c005065fa56126624c1bc4874cabf6ad13ba9627 .generated_files/flags/Release/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/midi.o.d 
	@${RM} ${OBJECTDIR}/midi.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega1284    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mrelax -funsigned-char -funsigned-bitfields -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/midi.o.d" -MT "${OBJECTDIR}/midi.o.d" -MT ${OBJECTDIR}/midi.o  -o ${OBJECTDIR}/midi.o midi.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -frename-registers -g2
	
${OBJECTDIR}/mpu401.o: mpu401.c  .generated_files/flags/Release/53b21053f6150d6761d84732aaf70167a9464b9 .generated_files/flags/Release/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/mpu401.o.d 
	@${RM} ${OBJECTDIR}/mpu401.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega1284    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mrelax -funsigned-char -funsigned-bitfields -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/mpu401.o.d" -MT "${OBJECTDIR}/mpu401.o.d" -MT ${OBJECTDIR}/mpu401.o  -o ${OBJECTDIR}/mpu401.o mpu401.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -frename-registers -g2
	
${OBJECTDIR}/pic.o: pic.c  .generated_files/flags/Release/76c8b760584eca21976ce098c958297bfd4af6 .generated_files/flags/Release/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pic.o.d 
	@${RM} ${OBJECTDIR}/pic.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega1284    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mrelax -funsigned-char -funsigned-bitfields -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/pic.o.d" -MT "${OBJECTDIR}/pic.o.d" -MT ${OBJECTDIR}/pic.o  -o ${OBJECTDIR}/pic.o pic.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -frename-registers -g2
	
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
	${MP_CC} $(MP_EXTRA_LD_PRE) -mmcu=atmega1284    -gdwarf-2 -D__$(MP_PROCESSOR_OPTION)__  -Wl,-Map="${DISTDIR}/HardMPU.${IMAGE_TYPE}.map"    -o ${DISTDIR}/HardMPU.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_Release=$(CND_CONF)  -mrelax $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1 -Wl,--gc-sections -Wl,--start-group  -Wl,-lm -Wl,-lm -Wl,--end-group 
	
	${MP_CC_DIR}/avr-objcopy -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex "${DISTDIR}/HardMPU.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/HardMPU.${IMAGE_TYPE}.eep" || exit 0
	${MP_CC_DIR}/avr-objdump -h -S "${DISTDIR}/HardMPU.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" > "${DISTDIR}/HardMPU.${IMAGE_TYPE}.lss"
	${MP_CC_DIR}/avr-objcopy -O srec -R .eeprom -R .fuse -R .lock -R .signature "${DISTDIR}/HardMPU.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/HardMPU.${IMAGE_TYPE}.srec"
	
	
else
${DISTDIR}/HardMPU.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mmcu=atmega1284   -D__$(MP_PROCESSOR_OPTION)__  -Wl,-Map="${DISTDIR}/HardMPU.${IMAGE_TYPE}.map"    -o ${DISTDIR}/HardMPU.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_Release=$(CND_CONF)  -mrelax $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION) -Wl,--gc-sections -Wl,--start-group  -Wl,-lm -Wl,-lm -Wl,--end-group 
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
