# 鍒涘缓
Project::Toolchain.new(:clang) do |conf|
  # clang 鍩烘湰鍏煎gcc
  toolchain :gcc
  # C++ 缂栬疟鍣?
  conf.cxx.command = ENV['CXX'] || 'clang++'
  # 阈炬帴鍣?
  conf.linker.command = ENV['LD'] || 'clang'
end