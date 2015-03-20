# 创建
Project::Toolchain.new(:clang) do |conf|
  # clang 基本兼容gcc
  toolchain :gcc
  # C++ 编译器
  conf.cxx.command = ENV['CXX'] || 'clang++'
  # 链接器
  conf.linker.command = ENV['LD'] || 'clang'
end