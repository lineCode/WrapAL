##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=vorbis
ConfigurationName      :=Release
WorkspacePath          :=D:/github/WrapAL/Project_CodeLite
ProjectPath            :=D:/github/WrapAL/Project_CodeLite/vorbis
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
ObjectsFileList        :="vorbis.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/TDM-GCC-64/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../3rdparty/libogg/include/ $(IncludeSwitch)../../3rdparty/libvorbis/include/ $(IncludeSwitch)../../3rdparty/libvorbis/lib/ $(IncludeSwitch). 
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
CXXFLAGS :=  -O2 -Wall $(Preprocessors)
CFLAGS   :=  -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/TDM-GCC-64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_analysis.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_barkmel.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_bitrate.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_block.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_codebook.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_envelope.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_floor0.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_floor1.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_info.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lookup.c$(ObjectSuffix) \
	$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lpc.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lsp.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_mapping0.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_mdct.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_psy.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_registry.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_res0.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_sharedbook.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_smallft.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_synthesis.c$(ObjectSuffix) \
	$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_tone.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_vorbisenc.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_vorbisfile.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_window.c$(ObjectSuffix) 



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
	@echo rebuilt > "D:\github\WrapAL\Project_CodeLite/.build-release/vorbis"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Release"


./Release:
	@$(MakeDirCommand) "./Release"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_analysis.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/analysis.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_analysis.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/analysis.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_analysis.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_analysis.c$(DependSuffix): ../../3rdparty/libvorbis/lib/analysis.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_analysis.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_analysis.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/analysis.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_analysis.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/analysis.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_analysis.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/analysis.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_barkmel.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/barkmel.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_barkmel.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/barkmel.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_barkmel.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_barkmel.c$(DependSuffix): ../../3rdparty/libvorbis/lib/barkmel.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_barkmel.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_barkmel.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/barkmel.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_barkmel.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/barkmel.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_barkmel.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/barkmel.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_bitrate.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/bitrate.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_bitrate.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/bitrate.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_bitrate.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_bitrate.c$(DependSuffix): ../../3rdparty/libvorbis/lib/bitrate.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_bitrate.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_bitrate.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/bitrate.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_bitrate.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/bitrate.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_bitrate.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/bitrate.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_block.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/block.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_block.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/block.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_block.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_block.c$(DependSuffix): ../../3rdparty/libvorbis/lib/block.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_block.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_block.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/block.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_block.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/block.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_block.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/block.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_codebook.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/codebook.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_codebook.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/codebook.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_codebook.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_codebook.c$(DependSuffix): ../../3rdparty/libvorbis/lib/codebook.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_codebook.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_codebook.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/codebook.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_codebook.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/codebook.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_codebook.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/codebook.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_envelope.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/envelope.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_envelope.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/envelope.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_envelope.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_envelope.c$(DependSuffix): ../../3rdparty/libvorbis/lib/envelope.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_envelope.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_envelope.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/envelope.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_envelope.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/envelope.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_envelope.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/envelope.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_floor0.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/floor0.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_floor0.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/floor0.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_floor0.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_floor0.c$(DependSuffix): ../../3rdparty/libvorbis/lib/floor0.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_floor0.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_floor0.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/floor0.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_floor0.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/floor0.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_floor0.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/floor0.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_floor1.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/floor1.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_floor1.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/floor1.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_floor1.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_floor1.c$(DependSuffix): ../../3rdparty/libvorbis/lib/floor1.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_floor1.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_floor1.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/floor1.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_floor1.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/floor1.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_floor1.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/floor1.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_info.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/info.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_info.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/info.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_info.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_info.c$(DependSuffix): ../../3rdparty/libvorbis/lib/info.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_info.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_info.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/info.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_info.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/info.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_info.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/info.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lookup.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/lookup.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lookup.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/lookup.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lookup.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lookup.c$(DependSuffix): ../../3rdparty/libvorbis/lib/lookup.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lookup.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lookup.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/lookup.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lookup.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/lookup.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lookup.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/lookup.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lpc.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/lpc.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lpc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/lpc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lpc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lpc.c$(DependSuffix): ../../3rdparty/libvorbis/lib/lpc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lpc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lpc.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/lpc.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lpc.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/lpc.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lpc.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/lpc.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lsp.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/lsp.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lsp.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/lsp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lsp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lsp.c$(DependSuffix): ../../3rdparty/libvorbis/lib/lsp.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lsp.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lsp.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/lsp.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lsp.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/lsp.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_lsp.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/lsp.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_mapping0.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/mapping0.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_mapping0.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/mapping0.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_mapping0.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_mapping0.c$(DependSuffix): ../../3rdparty/libvorbis/lib/mapping0.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_mapping0.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_mapping0.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/mapping0.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_mapping0.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/mapping0.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_mapping0.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/mapping0.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_mdct.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/mdct.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_mdct.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/mdct.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_mdct.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_mdct.c$(DependSuffix): ../../3rdparty/libvorbis/lib/mdct.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_mdct.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_mdct.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/mdct.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_mdct.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/mdct.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_mdct.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/mdct.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_psy.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/psy.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_psy.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/psy.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_psy.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_psy.c$(DependSuffix): ../../3rdparty/libvorbis/lib/psy.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_psy.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_psy.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/psy.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_psy.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/psy.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_psy.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/psy.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_registry.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/registry.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_registry.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/registry.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_registry.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_registry.c$(DependSuffix): ../../3rdparty/libvorbis/lib/registry.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_registry.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_registry.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/registry.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_registry.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/registry.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_registry.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/registry.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_res0.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/res0.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_res0.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/res0.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_res0.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_res0.c$(DependSuffix): ../../3rdparty/libvorbis/lib/res0.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_res0.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_res0.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/res0.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_res0.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/res0.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_res0.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/res0.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_sharedbook.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/sharedbook.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_sharedbook.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/sharedbook.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_sharedbook.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_sharedbook.c$(DependSuffix): ../../3rdparty/libvorbis/lib/sharedbook.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_sharedbook.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_sharedbook.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/sharedbook.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_sharedbook.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/sharedbook.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_sharedbook.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/sharedbook.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_smallft.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/smallft.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_smallft.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/smallft.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_smallft.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_smallft.c$(DependSuffix): ../../3rdparty/libvorbis/lib/smallft.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_smallft.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_smallft.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/smallft.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_smallft.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/smallft.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_smallft.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/smallft.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_synthesis.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/synthesis.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_synthesis.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/synthesis.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_synthesis.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_synthesis.c$(DependSuffix): ../../3rdparty/libvorbis/lib/synthesis.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_synthesis.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_synthesis.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/synthesis.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_synthesis.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/synthesis.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_synthesis.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/synthesis.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_tone.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/tone.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_tone.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/tone.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_tone.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_tone.c$(DependSuffix): ../../3rdparty/libvorbis/lib/tone.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_tone.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_tone.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/tone.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_tone.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/tone.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_tone.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/tone.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_vorbisenc.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/vorbisenc.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_vorbisenc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/vorbisenc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_vorbisenc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_vorbisenc.c$(DependSuffix): ../../3rdparty/libvorbis/lib/vorbisenc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_vorbisenc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_vorbisenc.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/vorbisenc.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_vorbisenc.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/vorbisenc.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_vorbisenc.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/vorbisenc.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_vorbisfile.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/vorbisfile.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_vorbisfile.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/vorbisfile.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_vorbisfile.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_vorbisfile.c$(DependSuffix): ../../3rdparty/libvorbis/lib/vorbisfile.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_vorbisfile.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_vorbisfile.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/vorbisfile.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_vorbisfile.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/vorbisfile.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_vorbisfile.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/vorbisfile.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_window.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/window.c $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_window.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/github/WrapAL/3rdparty/libvorbis/lib/window.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_window.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_window.c$(DependSuffix): ../../3rdparty/libvorbis/lib/window.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_window.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_window.c$(DependSuffix) -MM ../../3rdparty/libvorbis/lib/window.c

$(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_window.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/window.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_3rdparty_libvorbis_lib_window.c$(PreprocessSuffix) ../../3rdparty/libvorbis/lib/window.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


