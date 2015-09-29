##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libogg
ConfigurationName      :=Debug
WorkspacePath          := "D:\Documents\GitHub\WrapAL\Project_CodeLite"
ProjectPath            := "D:\Documents\GitHub\WrapAL\Project_CodeLite\libogg"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=DUST
Date                   :=2015-09-29
CodeLitePath           :="C:\Program Files\CodeLite"
LinkerName             :=C:/SB/TDM-GCC-64/bin/g++.exe
SharedObjectLinkerName :=C:/SB/TDM-GCC-64/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=../lib/lib$(ProjectName).a
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libogg.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/SB/TDM-GCC-64/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)./../../3rdparty/libogg/include/ 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/SB/TDM-GCC-64/bin/ar.exe rcu
CXX      := C:/SB/TDM-GCC-64/bin/g++.exe
CC       := C:/SB/TDM-GCC-64/bin/gcc.exe
CXXFLAGS :=  -g $(Preprocessors)
CFLAGS   :=  -g $(Preprocessors)
ASFLAGS  := 
AS       := C:/SB/TDM-GCC-64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/src_bitwise.c$(ObjectSuffix) $(IntermediateDirectory)/src_framing.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList) $(ArLibs)
	@$(MakeDirCommand) "D:\Documents\GitHub\WrapAL\Project_CodeLite/.build-debug"
	@echo rebuilt > "D:\Documents\GitHub\WrapAL\Project_CodeLite/.build-debug/libogg"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


./Debug:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_bitwise.c$(ObjectSuffix): ../../3rdparty/libogg/src/bitwise.c $(IntermediateDirectory)/src_bitwise.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libogg/src/bitwise.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bitwise.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bitwise.c$(DependSuffix): ../../3rdparty/libogg/src/bitwise.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bitwise.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bitwise.c$(DependSuffix) -MM "../../3rdparty/libogg/src/bitwise.c"

$(IntermediateDirectory)/src_bitwise.c$(PreprocessSuffix): ../../3rdparty/libogg/src/bitwise.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bitwise.c$(PreprocessSuffix) "../../3rdparty/libogg/src/bitwise.c"

$(IntermediateDirectory)/src_framing.c$(ObjectSuffix): ../../3rdparty/libogg/src/framing.c $(IntermediateDirectory)/src_framing.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libogg/src/framing.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_framing.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_framing.c$(DependSuffix): ../../3rdparty/libogg/src/framing.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_framing.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_framing.c$(DependSuffix) -MM "../../3rdparty/libogg/src/framing.c"

$(IntermediateDirectory)/src_framing.c$(PreprocessSuffix): ../../3rdparty/libogg/src/framing.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_framing.c$(PreprocessSuffix) "../../3rdparty/libogg/src/framing.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


