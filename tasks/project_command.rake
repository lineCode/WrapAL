#==============================================================================
# 鈻?Project 妯″潡
#------------------------------------------------------------------------------
# 銆€链琑ake椤圭洰榛樿妯″潡, 绠椾綔锻藉悕绌洪棿
#==============================================================================
module Project
  #============================================================================
  # 鈻? 锻戒护 镓€链塁ommand鐖剁被
  #============================================================================
  class Command
    #--------------------------------------------------------------------------
    # 鈼?瀹氢箟瀹炰緥鍙橀噺
    #--------------------------------------------------------------------------
    include               Rake::DSL
    attr_accessor         :build, :command
    #--------------------------------------------------------------------------
    # 鈼?鍒濆鍖?
    #--------------------------------------------------------------------------
    def initialize(build)
      @build = build
    end
    #--------------------------------------------------------------------------
    NotFoundCommands = {}
    private
    # 璋冭瘯
    #def sh(cmd)
     # p cmd
     # system(cmd)
    #end
    #--------------------------------------------------------------------------
    # 鈼?镓ц
    #--------------------------------------------------------------------------
    def _run(options, params={})
      return sh command + ' ' + ( options % params ) if NotFoundCommands.key? @command
      begin
        sh build.filename(command) + ' ' + ( options % params )
      rescue RuntimeError
        NotFoundCommands[@command] = true
        _run options, params
      end
    end
  end
  #============================================================================
  # 鈻? 缂栬疟鍣?
  #============================================================================
  class Compiler < Command
    #--------------------------------------------------------------------------
    # 鈼?瀹氢箟瀹炰緥鍙橀噺
    #--------------------------------------------------------------------------
    attr_accessor         :flags, :include_paths, :defines, :source_exts
    attr_accessor         :compile_options, :option_define, :out_ext
    attr_accessor         :option_include_path
    #--------------------------------------------------------------------------
    # 鈼?鍒濆鍖?瑕佹眰鎻愪緵婧愭枃浠舵墿灞曞悕
    #--------------------------------------------------------------------------
    def initialize(build, source_exts=[])
      super(build)
      @command = ENV['CC'] || 'cc'
      @flags = [ENV['CFLAGS'] || []]
      @source_exts = source_exts
      @include_paths = ["#{PROJECT_ROOT}/include"]
      @defines = %w()
      @option_include_path = '-I%s'
      @option_define = '-D%s'
      @compile_options = '%{flags} -o %{outfile} -c %{infile}'
    end
    #--------------------------------------------------------------------------
    # 鈼?杈揿嚭镓€链?flag
    #--------------------------------------------------------------------------
    def all_flags(_defineds=[], _include_paths=[], _flags=[])
      define_flags = [defines, _defineds].flatten.map{ |d| option_define % d }
      include_path_flags = [include_paths, _include_paths].flatten.map do |f|
        option_include_path % build.filename(f)
      end
      [flags, define_flags, include_path_flags, _flags].flatten.join(' ')
    end
    #--------------------------------------------------------------------------
    # 鈼?杩愯
    #--------------------------------------------------------------------------
    def run(outfile, infile, _defineds=[], _include_paths=[], _flags=[])
      FileUtils.mkdir_p File.dirname(outfile)
      _pp "CC", infile.relative_path, outfile.relative_path
      _run compile_options, { 
        :flags => all_flags(_defineds, _include_paths, _flags),
        :infile => build.filename(infile), 
        :outfile => build.filename(outfile) 
      }
    end
  end
  #============================================================================
  # 鈻? 璧勬簮缂栬疟鍣?
  #============================================================================
  class ResourceCompiler < Compiler
    #--------------------------------------------------------------------------
    # 鈼?鍒濆鍖?瑕佹眰鎻愪緵婧愭枃浠舵墿灞曞悕
    #--------------------------------------------------------------------------
    def initialize(build, source_exts=[])
      super(build, source_exts)
      @command = ENV['RC'] || 'rc'
      @flags = [ENV['RCFLAGS'] || []]
      @source_exts = source_exts
      @include_paths = ["#{PROJECT_ROOT}/include"]
      @defines = %w()
      @option_include_path = '-I%s'
      @option_define = '-D%s'
      @compile_options = '%{flags} -o %{outfile} %{infile}'
    end
    #--------------------------------------------------------------------------
    # 鈼?杩愯
    #--------------------------------------------------------------------------
    def run(outfile, infile, _defineds=[], _include_paths=[], _flags=[])
      FileUtils.mkdir_p File.dirname(outfile)
      _pp "RC", infile.relative_path, outfile.relative_path
      _run compile_options, { 
        :flags => all_flags(_defineds, _include_paths, _flags),
        :infile => build.filename(infile), 
        :outfile => build.filename(outfile) 
      }
    end
  end
  #============================================================================
  # 鈻? 褰掓。鍣?鐢熸垚闱欐€佸簱
  #============================================================================
  class Archiver < Command
    #--------------------------------------------------------------------------
    # 鈼?瀹氢箟瀹炰緥鍙橀噺
    #--------------------------------------------------------------------------
    attr_accessor :archive_options
    #--------------------------------------------------------------------------
    # 鈼?鍒濆鍖?瑕佹眰鎻愪緵婧愭枃浠舵墿灞曞悕
    #--------------------------------------------------------------------------
    def initialize(build)
      super(build)
      @command = ENV['AR'] || 'ar'
      @archive_options = 'rs %{outfile} %{objs}'
    end
    #--------------------------------------------------------------------------
    # 鈼?杩愯
    #--------------------------------------------------------------------------
    def run(outfile, objfiles)
      FileUtils.mkdir_p File.dirname(outfile)
      _pp "AR", outfile.relative_path
      _run archive_options, { 
        :outfile => build.filename(outfile), 
        :objs => build.filename(objfiles).join(' ') 
      }
    end
  end
  #============================================================================
  # 鈻? 阈炬帴鍣?
  #============================================================================
  class Linker < Command
    #--------------------------------------------------------------------------
    # 鈼?瀹氢箟瀹炰緥鍙橀噺
    #--------------------------------------------------------------------------
    attr_accessor         :flags, :library_paths, :flags_before_libraries
    attr_accessor         :libraries, :flags_after_libraries, :link_options
    attr_accessor         :option_library, :option_library_path
    #--------------------------------------------------------------------------
    # 鈼?鍒濆鍖?
    #--------------------------------------------------------------------------
    def initialize(build, source_exts=[])
      super(build)
      @command = ENV['LD'] || 'ld'
      @flags = (ENV['LDFLAGS'] || [])
      @flags_before_libraries, @flags_after_libraries = [], []
      @libraries = []
      @library_paths = []
      @option_library = '-l%s'
      @option_library_path = '-L%s'
      @link_options = "%{flags} -o %{outfile} %{objs} %{flags_before_libraries} %{libs} %{flags_after_libraries}"
    end
    #--------------------------------------------------------------------------
    # 鈼?鍏ㄩ儴flag
    #--------------------------------------------------------------------------
    def all_flags(_library_paths=[], _flags=[])
      library_path_flags = [library_paths, _library_paths].flatten.map do |f|
        option_library_path % build.filename(f)
      end
      [flags, library_path_flags, _flags].flatten.join(' ')
    end
=begin
    #--------------------------------------------------------------------------
    # 鈼?搴揻lag
    #--------------------------------------------------------------------------
    def library_flags(_libraries)
      [libraries, _libraries].flatten.map{ |d| option_library % d }.join(' ')
    end
=end
    #--------------------------------------------------------------------------
    # 鈼?杩愯
    #--------------------------------------------------------------------------
    def run(outfile, objfiles, _libraries=[], _library_paths=[], _flags=[], _flags_before_libraries=[], _flags_after_libraries=[])
      FileUtils.mkdir_p File.dirname(outfile)
      library_flags = [libraries, _libraries].flatten.map { |d| option_library % d }

      _pp "LD", outfile.relative_path
      _run link_options, { 
        :flags => all_flags(_library_paths, _flags),
         :outfile => build.filename(outfile) , 
         :objs => build.filename(objfiles).join(' '),
         :flags_before_libraries => [flags_before_libraries, _flags_before_libraries].flatten.join(' '),
         :flags_after_libraries => [flags_after_libraries, _flags_after_libraries].flatten.join(' '),
         :libs => library_flags.join(' ') 
       }
     end
  end
end
