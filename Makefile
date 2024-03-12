CC = g++
# ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
CFLAGS = -g -DGL_GLEXT_PROTOTYPES -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED -DOSX -Wno-deprecated-register -Wno-deprecated-declarations -Wno-shift-op-parentheses
INCFLAGS = -I./glm-0.9.7.1 -I/usr/X11/include -I./include/
LDFLAGS = -framework GLUT -framework OpenGL -L./lib/mac/ \
		-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
		-lm -lstdc++ -lfreeimage
# -lGL -lGLU 
# else
# CFLAGS = -g -DGL_GLEXT_PROTOTYPES 
# INCFLAGS = -I./glm-0.9.7.1 -I./include/ -I/usr/X11R6/include -I/sw/include \
# 		-I/usr/sww/include -I/usr/sww/pkg/Mesa/include
# LDFLAGS = -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib -L./lib/nix/ \
# 		-L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lGLEW -lglut -lGLU -lGL -lX11 -lfreeimage
# endif

RM = /bin/rm -f 
all: raytracer
raytracer: main.o Transform.o RayTracer.o Scene.o Camera.o Image.o Shape.o
	$(CC) $(CFLAGS) -o raytracer main.o Transform.o RayTracer.o Scene.o Camera.o Image.o Shape.o $(INCFLAGS) $(LDFLAGS) 
main.o: main.cpp RayTracer.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
Transform.o: Transform.cpp Transform.h 
	$(CC) $(CFLAGS) $(INCFLAGS) -c Transform.cpp  
RayTracer.o: RayTracer.cpp RayTracer.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c RayTracer.cpp
Scene.o: Scene.cpp RayTracer.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Scene.cpp
Camera.o: Camera.cpp RayTracer.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Camera.cpp
Image.o: Image.cpp RayTracer.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Image.cpp
Shape.o: Shape.cpp RayTracer.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Shape.cpp

clean: 
	$(RM) *.o raytracer *.png *.txt


 
