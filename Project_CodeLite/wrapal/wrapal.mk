##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=WrapAL
ConfigurationName      :=Release
WorkspacePath          :=D:/github/WrapAL/Project_CodeLite
ProjectPath            :=D:/github/WrapAL/Project_CodeLite/WrapAL
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
ObjectsFileList        :="WrapAL.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/TDM-GCC-64/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)./../../include/ $(IncludeSwitch)../../3rdparty/libogg/include/ $(IncludeSwitch). 
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
Objects0=$(IntermediateDirectory)/up_up_src_AudioClip.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_src_AudioEngine.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_src_AudioStreams.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "D:\github\WrapAL\Project_CodeLite/.build-release/WrapAL"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Release"


./Release:
	@$(MakeDirCommand) "./Release"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_src_AudioClip.cpp$(ObjectSuffix): ../../src/AudioClip.cpp $(IntermediateDirectory)/up_up_src_AudioClip.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/github/WrapAL/src/AudioClip.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_src_AudioClip.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_src_AudioClip.cpp$(DependSuffix): ../../src/AudioClip.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_src_AudioClip.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_src_AudioClip.cpp$(DependSuffix) -MM ../../src/AudioClip.cpp

$(IntermediateDirectory)/up_up_src_AudioClip.cpp$(PreprocessSuffix): ../../src/AudioClip.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_src_AudioClip.cpp$(PreprocessSuffix) ../../src/AudioClip.cpp

$(IntermediateDirectory)/up_up_src_AudioEngine.cpp$(ObjectSuffix): ../../src/AudioEngine.cpp $(IntermediateDirectory)/up_up_src_AudioEngine.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/github/WrapAL/src/AudioEngine.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_src_AudioEngine.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_src_AudioEngine.cpp$(DependSuffix): ../../src/AudioEngine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_src_AudioEngine.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_src_AudioEngine.cpp$(DependSuffix) -MM ../../src/AudioEngine.cpp

$(IntermediateDirectory)/up_up_src_AudioEngine.cpp$(PreprocessSuffix): ../../src/AudioEngine.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_src_AudioEngine.cpp$(PreprocessSuffix) ../../src/AudioEngine.cpp

$(IntermediateDirectory)/up_up_src_AudioStreams.cpp$(ObjectSuffix): ../../src/AudioStreams.cpp $(IntermediateDirectory)/up_up_src_AudioStreams.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/github/WrapAL/src/AudioStreams.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_src_AudioStreams.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_src_AudioStreams.cpp$(DependSuffix): ../../src/AudioStreams.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_src_AudioStreams.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_src_AudioStreams.cpp$(DependSuffix) -MM ../../src/AudioStreams.cpp

$(IntermediateDirectory)/up_up_src_AudioStreams.cpp$(PreprocessSuffix): ../../src/AudioStreams.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_src_AudioStreams.cpp$(PreprocessSuffix) ../../src/AudioStreams.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


