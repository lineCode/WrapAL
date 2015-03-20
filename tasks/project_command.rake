#==============================================================================
# ■ Project 模块
#------------------------------------------------------------------------------
# 　本Rake项目默认模块, 算作命名空间
#==============================================================================
module Project
  #============================================================================
  # ■  命令 所有Command父类
  #============================================================================
  class Command
    #--------------------------------------------------------------------------
    # ● 定义实例变量
    #--------------------------------------------------------------------------
    include               Rake::DSL
    attr_accessor         :build, :command
    #--------------------------------------------------------------------------
    # ● 初始化
    #--------------------------------------------------------------------------
    def initialize(build)
      @build = build
    end
    #--------------------------------------------------------------------------
    NotFoundCommands = {}
    private
    # 调试
    #def sh(cmd)
     # p cmd
     # system(cmd)
    #end
    #--------------------------------------------------------------------------
    # ● 执行
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
  # ■  编译器
  #============================================================================
  class Compiler < Command
    #--------------------------------------------------------------------------
    # ● 定义实例变量
    #--------------------------------------------------------------------------
    attr_accessor         :flags, :include_paths, :defines, :source_exts
    attr_accessor         :compile_options, :option_define, :out_ext
    attr_accessor         :option_include_path
    #--------------------------------------------------------------------------
    # ● 初始化 要求提供源文件扩展名
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
    # ● 输出所有 flag
    #--------------------------------------------------------------------------
    def all_flags(_defineds=[], _include_paths=[], _flags=[])
      define_flags = [defines, _defineds].flatten.map{ |d| option_define % d }
      include_path_flags = [include_paths, _include_paths].flatten.map do |f|
        option_include_path % build.filename(f)
      end
      [flags, define_flags, include_path_flags, _flags].flatten.join(' ')
    end
    #--------------------------------------------------------------------------
    # ● 运行
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
  # ■  资源编译器
  #============================================================================
  class ResourceCompiler < Compiler
    #--------------------------------------------------------------------------
    # ● 初始化 要求提供源文件扩展名
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
    # ● 运行
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
  # ■  归档器 生成静态库
  #============================================================================
  class Archiver < Command
    #--------------------------------------------------------------------------
    # ● 定义实例变量
    #--------------------------------------------------------------------------
    attr_accessor :archive_options
    #--------------------------------------------------------------------------
    # ● 初始化 要求提供源文件扩展名
    #--------------------------------------------------------------------------
    def initialize(build)
      super(build)
      @command = ENV['AR'] || 'ar'
      @archive_options = 'rs %{outfile} %{objs}'
    end
    #--------------------------------------------------------------------------
    # ● 运行
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
  # ■  链接器
  #============================================================================
  class Linker < Command
    #--------------------------------------------------------------------------
    # ● 定义实例变量
    #--------------------------------------------------------------------------
    attr_accessor         :flags, :library_paths, :flags_before_libraries
    attr_accessor         :libraries, :flags_after_libraries, :link_options
    attr_accessor         :option_library, :option_library_path
    #--------------------------------------------------------------------------
    # ● 初始化
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
    # ● 全部flag
    #--------------------------------------------------------------------------
    def all_flags(_library_paths=[], _flags=[])
      library_path_flags = [library_paths, _library_paths].flatten.map do |f|
        option_library_path % build.filename(f)
      end
      [flags, library_path_flags, _flags].flatten.join(' ')
    end
=begin
    #--------------------------------------------------------------------------
    # ● 库flag
    #--------------------------------------------------------------------------
    def library_flags(_libraries)
      [libraries, _libraries].flatten.map{ |d| option_library % d }.join(' ')
    end
=end
    #--------------------------------------------------------------------------
    # ● 运行
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
