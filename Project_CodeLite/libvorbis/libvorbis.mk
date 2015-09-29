##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libvorbis
ConfigurationName      :=Debug
WorkspacePath          := "D:\Documents\GitHub\WrapAL\Project_CodeLite"
ProjectPath            := "D:\Documents\GitHub\WrapAL\Project_CodeLite\libvorbis"
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
ObjectsFileList        :="libvorbis.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/SB/TDM-GCC-64/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)./../../3rdparty/libogg/include/ $(IncludeSwitch)./../../3rdparty/libvorbis/include/ $(IncludeSwitch)./../../3rdparty/libvorbis/lib/ 
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
Objects0=$(IntermediateDirectory)/lib_analysis.c$(ObjectSuffix) $(IntermediateDirectory)/lib_barkmel.c$(ObjectSuffix) $(IntermediateDirectory)/lib_bitrate.c$(ObjectSuffix) $(IntermediateDirectory)/lib_block.c$(ObjectSuffix) $(IntermediateDirectory)/lib_codebook.c$(ObjectSuffix) $(IntermediateDirectory)/lib_envelope.c$(ObjectSuffix) $(IntermediateDirectory)/lib_floor0.c$(ObjectSuffix) $(IntermediateDirectory)/lib_floor1.c$(ObjectSuffix) $(IntermediateDirectory)/lib_info.c$(ObjectSuffix) $(IntermediateDirectory)/lib_lookup.c$(ObjectSuffix) \
	$(IntermediateDirectory)/lib_lpc.c$(ObjectSuffix) $(IntermediateDirectory)/lib_lsp.c$(ObjectSuffix) $(IntermediateDirectory)/lib_mapping0.c$(ObjectSuffix) $(IntermediateDirectory)/lib_mdct.c$(ObjectSuffix) $(IntermediateDirectory)/lib_psy.c$(ObjectSuffix) $(IntermediateDirectory)/lib_registry.c$(ObjectSuffix) $(IntermediateDirectory)/lib_res0.c$(ObjectSuffix) $(IntermediateDirectory)/lib_sharedbook.c$(ObjectSuffix) $(IntermediateDirectory)/lib_smallft.c$(ObjectSuffix) $(IntermediateDirectory)/lib_synthesis.c$(ObjectSuffix) \
	$(IntermediateDirectory)/lib_tone.c$(ObjectSuffix) $(IntermediateDirectory)/lib_vorbisenc.c$(ObjectSuffix) $(IntermediateDirectory)/lib_vorbisfile.c$(ObjectSuffix) $(IntermediateDirectory)/lib_window.c$(ObjectSuffix) 



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
	@echo rebuilt > "D:\Documents\GitHub\WrapAL\Project_CodeLite/.build-debug/libvorbis"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


./Debug:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/lib_analysis.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/analysis.c $(IntermediateDirectory)/lib_analysis.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/analysis.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_analysis.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_analysis.c$(DependSuffix): ../../3rdparty/libvorbis/lib/analysis.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_analysis.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_analysis.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/analysis.c"

$(IntermediateDirectory)/lib_analysis.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/analysis.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_analysis.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/analysis.c"

$(IntermediateDirectory)/lib_barkmel.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/barkmel.c $(IntermediateDirectory)/lib_barkmel.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/barkmel.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_barkmel.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_barkmel.c$(DependSuffix): ../../3rdparty/libvorbis/lib/barkmel.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_barkmel.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_barkmel.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/barkmel.c"

$(IntermediateDirectory)/lib_barkmel.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/barkmel.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_barkmel.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/barkmel.c"

$(IntermediateDirectory)/lib_bitrate.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/bitrate.c $(IntermediateDirectory)/lib_bitrate.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/bitrate.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_bitrate.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_bitrate.c$(DependSuffix): ../../3rdparty/libvorbis/lib/bitrate.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_bitrate.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_bitrate.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/bitrate.c"

$(IntermediateDirectory)/lib_bitrate.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/bitrate.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_bitrate.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/bitrate.c"

$(IntermediateDirectory)/lib_block.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/block.c $(IntermediateDirectory)/lib_block.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/block.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_block.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_block.c$(DependSuffix): ../../3rdparty/libvorbis/lib/block.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_block.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_block.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/block.c"

$(IntermediateDirectory)/lib_block.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/block.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_block.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/block.c"

$(IntermediateDirectory)/lib_codebook.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/codebook.c $(IntermediateDirectory)/lib_codebook.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/codebook.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_codebook.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_codebook.c$(DependSuffix): ../../3rdparty/libvorbis/lib/codebook.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_codebook.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_codebook.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/codebook.c"

$(IntermediateDirectory)/lib_codebook.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/codebook.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_codebook.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/codebook.c"

$(IntermediateDirectory)/lib_envelope.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/envelope.c $(IntermediateDirectory)/lib_envelope.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/envelope.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_envelope.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_envelope.c$(DependSuffix): ../../3rdparty/libvorbis/lib/envelope.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_envelope.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_envelope.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/envelope.c"

$(IntermediateDirectory)/lib_envelope.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/envelope.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_envelope.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/envelope.c"

$(IntermediateDirectory)/lib_floor0.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/floor0.c $(IntermediateDirectory)/lib_floor0.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/floor0.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_floor0.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_floor0.c$(DependSuffix): ../../3rdparty/libvorbis/lib/floor0.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_floor0.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_floor0.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/floor0.c"

$(IntermediateDirectory)/lib_floor0.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/floor0.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_floor0.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/floor0.c"

$(IntermediateDirectory)/lib_floor1.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/floor1.c $(IntermediateDirectory)/lib_floor1.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/floor1.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_floor1.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_floor1.c$(DependSuffix): ../../3rdparty/libvorbis/lib/floor1.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_floor1.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_floor1.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/floor1.c"

$(IntermediateDirectory)/lib_floor1.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/floor1.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_floor1.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/floor1.c"

$(IntermediateDirectory)/lib_info.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/info.c $(IntermediateDirectory)/lib_info.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/info.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_info.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_info.c$(DependSuffix): ../../3rdparty/libvorbis/lib/info.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_info.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_info.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/info.c"

$(IntermediateDirectory)/lib_info.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/info.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_info.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/info.c"

$(IntermediateDirectory)/lib_lookup.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/lookup.c $(IntermediateDirectory)/lib_lookup.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/lookup.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_lookup.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_lookup.c$(DependSuffix): ../../3rdparty/libvorbis/lib/lookup.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_lookup.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_lookup.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/lookup.c"

$(IntermediateDirectory)/lib_lookup.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/lookup.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_lookup.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/lookup.c"

$(IntermediateDirectory)/lib_lpc.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/lpc.c $(IntermediateDirectory)/lib_lpc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/lpc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_lpc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_lpc.c$(DependSuffix): ../../3rdparty/libvorbis/lib/lpc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_lpc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_lpc.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/lpc.c"

$(IntermediateDirectory)/lib_lpc.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/lpc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_lpc.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/lpc.c"

$(IntermediateDirectory)/lib_lsp.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/lsp.c $(IntermediateDirectory)/lib_lsp.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/lsp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_lsp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_lsp.c$(DependSuffix): ../../3rdparty/libvorbis/lib/lsp.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_lsp.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_lsp.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/lsp.c"

$(IntermediateDirectory)/lib_lsp.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/lsp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_lsp.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/lsp.c"

$(IntermediateDirectory)/lib_mapping0.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/mapping0.c $(IntermediateDirectory)/lib_mapping0.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/mapping0.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_mapping0.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_mapping0.c$(DependSuffix): ../../3rdparty/libvorbis/lib/mapping0.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_mapping0.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_mapping0.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/mapping0.c"

$(IntermediateDirectory)/lib_mapping0.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/mapping0.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_mapping0.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/mapping0.c"

$(IntermediateDirectory)/lib_mdct.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/mdct.c $(IntermediateDirectory)/lib_mdct.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/mdct.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_mdct.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_mdct.c$(DependSuffix): ../../3rdparty/libvorbis/lib/mdct.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_mdct.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_mdct.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/mdct.c"

$(IntermediateDirectory)/lib_mdct.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/mdct.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_mdct.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/mdct.c"

$(IntermediateDirectory)/lib_psy.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/psy.c $(IntermediateDirectory)/lib_psy.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/psy.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_psy.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_psy.c$(DependSuffix): ../../3rdparty/libvorbis/lib/psy.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_psy.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_psy.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/psy.c"

$(IntermediateDirectory)/lib_psy.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/psy.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_psy.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/psy.c"

$(IntermediateDirectory)/lib_registry.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/registry.c $(IntermediateDirectory)/lib_registry.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/registry.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_registry.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_registry.c$(DependSuffix): ../../3rdparty/libvorbis/lib/registry.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_registry.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_registry.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/registry.c"

$(IntermediateDirectory)/lib_registry.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/registry.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_registry.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/registry.c"

$(IntermediateDirectory)/lib_res0.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/res0.c $(IntermediateDirectory)/lib_res0.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/res0.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_res0.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_res0.c$(DependSuffix): ../../3rdparty/libvorbis/lib/res0.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_res0.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_res0.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/res0.c"

$(IntermediateDirectory)/lib_res0.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/res0.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_res0.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/res0.c"

$(IntermediateDirectory)/lib_sharedbook.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/sharedbook.c $(IntermediateDirectory)/lib_sharedbook.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/sharedbook.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_sharedbook.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_sharedbook.c$(DependSuffix): ../../3rdparty/libvorbis/lib/sharedbook.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_sharedbook.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_sharedbook.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/sharedbook.c"

$(IntermediateDirectory)/lib_sharedbook.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/sharedbook.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_sharedbook.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/sharedbook.c"

$(IntermediateDirectory)/lib_smallft.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/smallft.c $(IntermediateDirectory)/lib_smallft.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/smallft.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_smallft.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_smallft.c$(DependSuffix): ../../3rdparty/libvorbis/lib/smallft.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_smallft.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_smallft.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/smallft.c"

$(IntermediateDirectory)/lib_smallft.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/smallft.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_smallft.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/smallft.c"

$(IntermediateDirectory)/lib_synthesis.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/synthesis.c $(IntermediateDirectory)/lib_synthesis.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/synthesis.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_synthesis.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_synthesis.c$(DependSuffix): ../../3rdparty/libvorbis/lib/synthesis.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_synthesis.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_synthesis.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/synthesis.c"

$(IntermediateDirectory)/lib_synthesis.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/synthesis.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_synthesis.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/synthesis.c"

$(IntermediateDirectory)/lib_tone.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/tone.c $(IntermediateDirectory)/lib_tone.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/tone.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_tone.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_tone.c$(DependSuffix): ../../3rdparty/libvorbis/lib/tone.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_tone.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_tone.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/tone.c"

$(IntermediateDirectory)/lib_tone.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/tone.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_tone.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/tone.c"

$(IntermediateDirectory)/lib_vorbisenc.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/vorbisenc.c $(IntermediateDirectory)/lib_vorbisenc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/vorbisenc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_vorbisenc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_vorbisenc.c$(DependSuffix): ../../3rdparty/libvorbis/lib/vorbisenc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_vorbisenc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_vorbisenc.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/vorbisenc.c"

$(IntermediateDirectory)/lib_vorbisenc.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/vorbisenc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_vorbisenc.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/vorbisenc.c"

$(IntermediateDirectory)/lib_vorbisfile.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/vorbisfile.c $(IntermediateDirectory)/lib_vorbisfile.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/vorbisfile.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_vorbisfile.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_vorbisfile.c$(DependSuffix): ../../3rdparty/libvorbis/lib/vorbisfile.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_vorbisfile.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_vorbisfile.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/vorbisfile.c"

$(IntermediateDirectory)/lib_vorbisfile.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/vorbisfile.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_vorbisfile.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/vorbisfile.c"

$(IntermediateDirectory)/lib_window.c$(ObjectSuffix): ../../3rdparty/libvorbis/lib/window.c $(IntermediateDirectory)/lib_window.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Documents/GitHub/WrapAL/3rdparty/libvorbis/lib/window.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_window.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_window.c$(DependSuffix): ../../3rdparty/libvorbis/lib/window.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_window.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_window.c$(DependSuffix) -MM "../../3rdparty/libvorbis/lib/window.c"

$(IntermediateDirectory)/lib_window.c$(PreprocessSuffix): ../../3rdparty/libvorbis/lib/window.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_window.c$(PreprocessSuffix) "../../3rdparty/libvorbis/lib/window.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


