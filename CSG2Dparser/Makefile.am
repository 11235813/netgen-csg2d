libngcsg2d.so: ngcsg2d.cpp
	g++ -shared -fPIC -Wall ngcsg2d.cpp -g -I $(HOME)/work/netgen-csg2d/netgen-mesher/netgen/libsrc/include/ -I $(HOME)/work/netgen-csg2d/netgen-mesher/netgen/ -I /usr/include/tcl8.5/ -I /usr/include/opencascade/ -L/usr/lib/opencas/ -DUSE_STL_STREAM -DHAVE_IOSTREAM -DHAVE_LIMITS_H -lTKIGES -o libngcsg2d.so

clean: 
	rm libngcsg2d.so > /dev/null 2>&1
