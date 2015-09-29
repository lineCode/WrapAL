##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=wrapal
ConfigurationName      :=Debug
WorkspacePath          := "D:\Documents\GitHub\WrapAL\Project_CodeLite"
ProjectPath            := "D:\Documents\GitHub\WrapAL\Project_CodeLite\wrapal"
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
ObjectsFileList        :="wrapal.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/SB/TDM-GCC-64/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)./../../3rdparty/libogg/include/ $(IncludeSwitch)./../../3rdparty/libvorbis/include/ $(IncludeSwitch)./../../3rdparty/libvorbis/lib/ $(IncludeSwitch)./../../include/ 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)vorbis 
ArLibs                 :=  "libvorbis.a" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)../lib/ 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/SB/TDM-GCC-64/bin/ar.exe rcu
CXX      := C:/SB/TDM-GCC-64/bin/g++.exe
CC       := C:/SB/TDM-GCC-64/bin/gcc.exe
CXXFLAGS :=  -g -O0 -std=c++14 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -std=c++14 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/SB/TDM-GCC-64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/src_AudioEngine.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_AudioStreams.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "D:\Documents\GitHub\WrapAL\Project_CodeLite/.build-debug/wrapal"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


./Debug:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_AudioEngine.cpp$(ObjectSuffix): ../../src/AudioEngine.cpp $(IntermediateDirectory)/src_AudioEngine.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/src/AudioEngine.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_AudioEngine.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_AudioEngine.cpp$(DependSuffix): ../../src/AudioEngine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_AudioEngine.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_AudioEngine.cpp$(DependSuffix) -MM "../../src/AudioEngine.cpp"

$(IntermediateDirectory)/src_AudioEngine.cpp$(PreprocessSuffix): ../../src/AudioEngine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_AudioEngine.cpp$(PreprocessSuffix) "../../src/AudioEngine.cpp"

$(IntermediateDirectory)/src_AudioStreams.cpp$(ObjectSuffix): ../../src/AudioStreams.cpp $(IntermediateDirectory)/src_AudioStreams.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/src/AudioStreams.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_AudioStreams.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_AudioStreams.cpp$(DependSuffix): ../../src/AudioStreams.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_AudioStreams.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_AudioStreams.cpp$(DependSuffix) -MM "../../src/AudioStreams.cpp"

$(IntermediateDirectory)/src_AudioStreams.cpp$(PreprocessSuffix): ../../src/AudioStreams.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_AudioStreams.cpp$(PreprocessSuffix) "../../src/AudioStreams.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


