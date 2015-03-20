# 创建
Project::Toolchain.new(:gcc) do |conf|
  # C 编译器
  [conf.cc].each do |cc|
    cc.command = ENV['CC'] || 'gcc'
    cc.flags = [ENV['CFLAGS'] || %w(-std=gnu99 -O3 -Wall -DNDEBUG -Werror-implicit-function-declaration -Wdeclaration-after-statement)]
    cc.defines = %w()
    cc.option_include_path = '-I%s'
    cc.option_define = '-D%s'
    cc.compile_options = '%{flags} -MMD -o %{outfile} -c %{infile}'
  end
  # C++ 编译器
  [conf.cxx].each do |cxx|
    cxx.command = ENV['CXX'] || 'g++'
    cxx.flags = [ENV['CXXFLAGS'] || ENV['CFLAGS'] || %w(-std=c++1y -O3 -Wall -DNDEBUG -Werror-implicit-function-declaration)]
    cxx.defines = %w()
    cxx.option_include_path = '-I%s'
    cxx.option_define = '-D%s'
    cxx.compile_options = '%{flags} -MMD -o %{outfile} -c %{infile}'
  end
  # 链接器
  conf.linker do |linker|
    linker.command = ENV['LD'] || 'g++'
    linker.flags = [ENV['LDFLAGS'] || %w()]
    linker.libraries = %w()
    linker.library_paths = []
    linker.option_library = '-l%s'
    linker.option_library_path = '-L%s'
    #linker.flags_before_libraries = %w(-Wl,-start-group)
    #linker.flags_after_libraries = %w(-Wl,-end-group)
    linker.link_options = '%{flags} -o %{outfile} -Wl,-start-group %{objs} -Wl,-end-group %{flags_before_libraries} %{libs} %{flags_after_libraries}'
  end
  # 资源编译器
  conf.rc do |rc|
    rc.command = ENV['RC'] || 'windres'
    rc.flags = [ENV['RCFLAGS']  || %w()]
    rc.defines = %w()
    rc.option_include_path = '-I%s'
    rc.option_define = '-D%s'
    rc.compile_options = '%{flags} -o %{outfile} %{infile}'
  end
  # 归档器
  conf.rc do |ar|
    
  end
end