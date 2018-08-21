#SDK测试程序

CXX   =  g++-4.4
C     =  gcc-4.4
CXXFLAGS = -Wall -Wno-strict-aliasing -Wno-unused-variable

#包含头文件路径
#SUBDIR   = $(shell ls ./ -R | grep /)
SUBDIR = ./src:
SUBDIRS  = $(subst :,/,$(SUBDIR))
#INCPATHS = $(subst ./,-I./,$(SUBDIRS))

#SUBDIRS = $(SUBDIR)
INCPATHS += -I./include/

VPATH = $(subst : ,:,$(SUBDIR))./
SOURCE = $(foreach dir,$(SUBDIRS),$(wildcard $(dir)*.cpp))

#将cpp的路径名称的.cpp后缀换为.o
OBJS = $(patsubst %.cpp,%.o,$(SOURCE))
#取出.o的文件名，不带路径
#OBJFILE  = $(foreach dir,$(OBJS),$(notdir $(dir)))
OBJFILE  = $(foreach dir,$(OBJS),$(dir))
#.o输出到.OBJ目录下
OBJSPATH = $(addprefix obj/,$(OBJFILE)) 

LIBPATH = ./bin/
LIBS = -Wl,-rpath=./:./HCNetSDKCom:./bin:./libs -lhcnetsdk -lstdc++ ./libs/libjson.a
EXE = ./bin/camap

$(EXE):$(OBJFILE)
	$(CXX) -L$(LIBPATH)  -o $(EXE) $(OBJFILE) $(INCPATHS) $(LIBS)

$(OBJFILE):%.o:%.cpp
	$(CXX)  -c -o $@ $<  $(INCPATHS) -pipe -g -Wall
#	$(CXX)  -c -o $@ $<  $(INCPATHS) -pipe -g -Wall
#	$(CXX)  -c -o ./src $<  $(INCPATHS) -pipe -g -Wall
	
#依赖文件
#DPPS = $(patsubst %.cpp,%.dpp,$(SOURCE))
#include $(DPPS)
#%.dpp: %.cpp	
#	g++ $(INCPATHS) -MM $(CPPFLAGS) $< > $@.$$$$; \
#	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
#	rm -f $@.$$$$ 


install:
	cp -rf ./bin/* /camap/bin/	

.PHONY:clean
clean:
	rm -rf $(OBJFILE)
	rm -rf $(DPPS)
	rm -rf $(EXE)

