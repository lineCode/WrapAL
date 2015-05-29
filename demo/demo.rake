Project.target("demo") do |target|
  current_dir = File.dirname(__FILE__).relative_path_from(Dir.pwd)
  relative_from_root = File.dirname(__FILE__).relative_path_from(PROJECT_ROOT)
  current_build_dir = "#{build_dir}/#{relative_from_root}"
  # headers depend
  headers = Dir.glob("#{PROJECT_ROOT}/include/*.h").map { |f| f }.compact
  # get object file 
  objs = Dir.glob("#{current_dir}/*.cpp").map { |f|
    outfile = objfile(f.pathmap("#{current_build_dir}/%n"))
    ext_include_path = ["#{PROJECT_ROOT}/include/"]
    # set file task for build
    file outfile => headers << f do
      target.cxx.run(outfile, f, [], ext_include_path)
    end
    outfile
  }.compact
  # resource file
=begin
  resource_infile = "#{current_dir}/resources.rc"
  resource_oufile = objfile("resources.rc".pathmap("#{current_build_dir}/%n"))
  file resource_oufile => resource_infile do |t|
    target.rc.run(resource_oufile, resource_infile)
  end
  objs << resource_oufile
=end
  # build the exe
  full_outname = "#{build_dir}/#{target.outname}" 
  desc "build demo"
  task :demo => [:wrapal, full_outname]
  # libraries
  static_libraries = [
    "#{Project.targets['ogg'].build_dir}/#{Project.targets['ogg'].outname}",
    "#{Project.targets['vorbis'].build_dir}/#{Project.targets['vorbis'].outname}",
    "#{Project.targets['wrapal'].build_dir}/#{Project.targets['wrapal'].outname}",
  ].compact
  # system libraty
  system_libraries = %w(ole32)
  # library path
  library_path = [
    Project.targets['ogg'].build_dir,
    Project.targets['vorbis'].build_dir,
    Project.targets['wrapal'].build_dir,
  ].uniq
  # do the file task
  file full_outname => objs do |t|
    target.linker.run(full_outname, objs + static_libraries, system_libraries, [], %w(-static))
  end
end
