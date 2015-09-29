##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=demo2
ConfigurationName      :=Debug
WorkspacePath          := "D:\Documents\GitHub\WrapAL\Project_CodeLite"
ProjectPath            := "D:\Documents\GitHub\WrapAL\Project_CodeLite\demo2"
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="demo2.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/SB/TDM-GCC-64/bin/windres.exe
LinkOptions            :=  -Xlinker "-(" -llibogg -llibvorbis -lOle32 -lwrapal
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)./../../include/ $(IncludeSwitch)./../3rdparty/libogg/include/ 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
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
Objects0=$(IntermediateDirectory)/demo_demo2.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/demo_demo2.cpp$(ObjectSuffix): ../../demo/demo2.cpp $(IntermediateDirectory)/demo_demo2.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/demo/demo2.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/demo_demo2.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/demo_demo2.cpp$(DependSuffix): ../../demo/demo2.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/demo_demo2.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/demo_demo2.cpp$(DependSuffix) -MM "../../demo/demo2.cpp"

$(IntermediateDirectory)/demo_demo2.cpp$(PreprocessSuffix): ../../demo/demo2.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/demo_demo2.cpp$(PreprocessSuffix) "../../demo/demo2.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


