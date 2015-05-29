# encoding: utf-8
# version: 0.1.2
PROJECT_ROOT = File.dirname(File.expand_path(__FILE__))

# load build systems
load "#{PROJECT_ROOT}/tasks/ruby_ext.rake"
load "#{PROJECT_ROOT}/tasks/project_build.rake"
# load configuration file
MRUBY_CONFIG = (ENV['PROJECT_CONFIG'] && ENV['PROJECT_CONFIG'] != '') ? 
    ENV['PROJECT_CONFIG'] : "#{PROJECT_ROOT}/build_config.rb"
load MRUBY_CONFIG

# 3rdparty
load "#{PROJECT_ROOT}/3rdparty/3rdparty.rake"
# wrapal
load "#{PROJECT_ROOT}/src/wrapal.rake"
# demo
load "#{PROJECT_ROOT}/demo/demo.rake"


# 榛樿rake
task :default => :all

# WrapAL Library
desc "build wrapal based lbrary"
file :wrapal => [:libogg, :libvorbis, :wrapalonly] do |t|
  puts 
  puts "build wrapal done! please check it out!"
  puts 
end

# All 
desc "build all"
file :all => [:wrapal, :demo] do |t|

end
