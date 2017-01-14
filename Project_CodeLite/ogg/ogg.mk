##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=ogg
ConfigurationName      :=Release
WorkspacePath          :=D:/github/WrapAL/Project_CodeLite
ProjectPath            :=D:/github/WrapAL/Project_CodeLite/ogg
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=dustpg
Date                   :=14/01/2017
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/TDM-GCC-64/bin/g++.exe
SharedObjectLinkerName :=C:/TDM-GCC-64/bin/g++.exe -shared -fPIC
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
OutputFile             :=../$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="ogg.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/TDM-GCC-64/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../3rdparty/libogg/include/ $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/TDM-GCC-64/bin/ar.exe rcu
CXX      := C:/TDM-GCC-64/bin/g++.exe
CC       := C:/TDM-GCC-64/bin/gcc.exe
CXXFLAGS := -std=c++14 -O2 -Wall $(Preprocessors)
CFLAGS   := -std=c99 -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/TDM-GCC-64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/up_up_3rdparty_libogg_src_bitwise.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libogg_src_framing.c$(ObjectSuffix) 



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
	@$(MakeDirCommand) "D:\github\WrapAL\Project_CodeLite/.build-release"
	@echo rebuilt > "D:\github\WrapAL\Project_CodeLite/.build-release/ogg"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Release"


./Release:
	@$(MakeDirCommand) "./Release"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_3rdparty_libogg_src_bitwise.c$(ObjectSuffix): ../../3rdparty/libogg/src/bitwise.c $(IntermediateDirectory)/up_up_3rdparty_libogg_src_bitwise.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libogg/src/bitwise.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libogg_src_bitwise.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libogg_src_bitwise.c$(DependSuffix): ../../3rdparty/libogg/src/bitwise.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libogg_src_bitwise.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libogg_src_bitwise.c$(DependSuffix) -MM ../../3rdparty/libogg/src/bitwise.c

$(IntermediateDirectory)/up_up_3rdparty_libogg_src_bitwise.c$(PreprocessSuffix): ../../3rdparty/libogg/src/bitwise.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libogg_src_bitwise.c$(PreprocessSuffix) ../../3rdparty/libogg/src/bitwise.c

$(IntermediateDirectory)/up_up_3rdparty_libogg_src_framing.c$(ObjectSuffix): ../../3rdparty/libogg/src/framing.c $(IntermediateDirectory)/up_up_3rdparty_libogg_src_framing.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libogg/src/framing.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libogg_src_framing.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libogg_src_framing.c$(DependSuffix): ../../3rdparty/libogg/src/framing.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libogg_src_framing.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libogg_src_framing.c$(DependSuffix) -MM ../../3rdparty/libogg/src/framing.c

$(IntermediateDirectory)/up_up_3rdparty_libogg_src_framing.c$(PreprocessSuffix): ../../3rdparty/libogg/src/framing.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libogg_src_framing.c$(PreprocessSuffix) ../../3rdparty/libogg/src/framing.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


