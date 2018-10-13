
FIND:=/usr/bin/find
CC:=cl.exe
CCOPT:=-nologo -EHsc -W4 -WX -MDd -Zi -D_UNICODE=1 -DUNICODE=1

.SUFFIXES: .exe .cpp .obj .dll .pch
.cpp.obj:
	$(CC) $(CCOPT) -c -Fo:$@ -Yupch.h $<
.obj.dll:
	$(CC) $(CCOPT) -Fe:$@ $^ -link -dll
.obj.exe:
	$(CC) $(CCOPT) -Fe:$@ $^

dll_TARGET=a.dll
exe_TARGET=MemoryOnHeap.exe
all_TARGET=$(exe_TARGET) $(dll_TARGET)

all: compile

compile: precompile $(all_TARGET)

precompile: pch.pch

clean:
	@${FIND} . -type f -name '*~' -delete
	@${FIND} . -type f -name '*.obj' -delete
	@for i in $(dll_TARGET) $(dll_TARGET:.dll=.exp) $(dll_TARGET:.dll=.ilk) $(dll_TARGET:.dll=.lib) $(dll_TARGET:.dll=.pdb) ; do if [ -f $$i ] ; then rm $$i ; fi ; done;
	@for i in $(exe_TARGET) $(exe_TARGET:.exe=.exp) $(exe_TARGET:.exe=.ilk) $(exe_TARGET:.exe=.lib) $(exe_TARGET:.exe=.pdb) ; do if [ -f $$i ] ; then rm $$i ; fi ; done;
	@if [ -f vc140.pdb ] ; then rm vc140.pdb ; fi
	@if [ -f pch.pch ] ; then rm pch.pch ; fi

a.dll: a.obj pch.obj tubakigishi.obj

a.obj: a.cpp pch.h tubakigishi.h

MemoryOnHeap.exe: MemoryOnHeap.obj app_log.obj pch.obj

MemoryOnHeap.obj: MemoryOnHeap.cpp pch.h tubakigishi.h app_log.h 
app_log.obj: app_log.cpp pch.h tubakigishi.h app_log.h 
tubakigishi.obj: tubakigishi.cpp pch.h tubakigishi.cpp 

pch.obj: pch.pch
	echo pre compile header.

pch.pch: pch.cpp pch.h 
	$(CC) $(CCOPT) -c -Ycpch.h pch.cpp 
