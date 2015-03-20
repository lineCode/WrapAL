  current_dir = File.dirname(__FILE__).relative_path_from(Dir.pwd)

  # libogg
load "#{current_dir}/libogg/libogg.rake"

  # libvorbis
load "#{current_dir}/libvorbis/libvorbis.rake"
