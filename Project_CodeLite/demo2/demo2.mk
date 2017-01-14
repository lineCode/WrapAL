##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=demo2
ConfigurationName      :=Release
WorkspacePath          :=D:/github/WrapAL/Project_CodeLite
ProjectPath            :=D:/github/WrapAL/Project_CodeLite/demo2
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="demo2.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/TDM-GCC-64/bin/windres.exe
LinkOptions            := -Xlinker "-(" -logg -lvorbis -lOle32 -lwrapal -s
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)./../../include/ $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)../$(IntermediateDirectory) 

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
Objects0=$(IntermediateDirectory)/up_up_demo_demo2.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d "..\.build-release\WrapAL" $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

"..\.build-release\WrapAL":
	@$(MakeDirCommand) "..\.build-release"
	@echo stam > "..\.build-release\WrapAL"




MakeIntermediateDirs:
	@$(MakeDirCommand) "./Release"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Release"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_demo_demo2.cpp$(ObjectSuffix): ../../demo/demo2.cpp $(IntermediateDirectory)/up_up_demo_demo2.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/github/WrapAL/demo/demo2.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_demo_demo2.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_demo_demo2.cpp$(DependSuffix): ../../demo/demo2.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_demo_demo2.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_demo_demo2.cpp$(DependSuffix) -MM ../../demo/demo2.cpp

$(IntermediateDirectory)/up_up_demo_demo2.cpp$(PreprocessSuffix): ../../demo/demo2.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_demo_demo2.cpp$(PreprocessSuffix) ../../demo/demo2.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


