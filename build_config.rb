toolchain_using = :gcc
# OGG
Project::Build.new("ogg") { |conf| 
  toolchain toolchain_using
  conf.outname = 'libogg.a'
 }
# VORBIS
 Project::Build.new("vorbis") { |conf| 
  toolchain toolchain_using
  conf.outname = 'libvorbis.a'
}
# WRAPAL
Project::Build.new("wrapal") { |conf| 
  toolchain toolchain_using
  conf.outname = 'libwrapal.a'
 }
# DEMO
Project::Build.new("demo") { |conf| 
  toolchain toolchain_using
  conf.outname = 'demo.exe'
 }
# EACH
Project.each_target  { |conf|
  # obj extx
  conf.object_extx = '.o'

  # demo did not include ogg-headers
  next if conf == Project.targets["demo"]
  [conf.cxx, conf.cc].each do |cc|
    # ogg
    cc.include_paths << "#{PROJECT_ROOT}/3rdparty/libogg/include/"
  end
 }
