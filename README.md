## WrapAL
Wrapped Audio Library, to make easier to play audio

####Current Version : 0.2.1
[Version History](./VersionHistory.md)  

##Document
- [so easy~](./doc/doc.md)
- 暂时没有中文文档, 不过实在是[太简单了~](./doc/doc.md)

##Demo
1. [hello world](./doc/1.helloworld.md)
2. [basic configure](./doc/2.configure-baisc.md)

##How to build
1. for visual studio:  
  - open the vcproject and build it
2. for mingw-w64(WrapAL need some dxsdk/winsdk headers)  
  - use rakefile to build, you may download ruby/rake at first
  - modify the build_config.rb  
  - rake it 
  
##How to configure WrapAL
  - you should modify [wrapalconf.h](./include/wrapalconf.h) to configure WrapAL
  - macro: 
  - constant:
  - runtime message:
  - locker:

##About the libmpg123
Because of the LGPL, you should build the libmpg123.dll by youself,  
this repo provided a pre-build libmpg123.dll for x86 in 
'Bin/libmpg123_x86.dll'

由于LGPL的限制, 你必须自己创建libmpg123.dll, 当然, 本项目提供了一个  
现成的用于x86的libmpg123.dll, 喂鱼'Bin/libmpg123_x86.dll', 黑喂鱼~

## License
Under the MIT License. Please refer to [License.txt](./License.txt).