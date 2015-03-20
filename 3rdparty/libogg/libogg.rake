Project.target("ogg") do |target|
  current_dir = File.dirname(__FILE__).relative_path_from(Dir.pwd)
  relative_from_root = File.dirname(__FILE__).relative_path_from(PROJECT_ROOT)
  current_build_dir = "#{build_dir}/#{relative_from_root}"
  # headers
  headers = Dir.glob("#{PROJECT_ROOT}/include/*.h").map { |f| f }.compact
  # get object file 
  objs = Dir.glob("#{current_dir}/src/*.c").map { |f|
    outfile = objfile(f.pathmap("#{current_build_dir}/%n"))
    # set file task for build
    file outfile => headers << f do
      target.cc.run(outfile, f)
    end
    outfile
  }.compact
  # build the libwrap.lib
  full_outname = "#{build_dir}/#{target.outname}"
  # friendly interface
  desc "build libogg"
  task :libogg => full_outname
  # do the file task
  file full_outname => objs do |t|
    target.archiver.run(full_outname, objs)
  end
end
