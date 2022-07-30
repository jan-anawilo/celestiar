compile:
	gcc -o celestiar celestiar.c -lGL -lGLU -lglut -lm

# on Mac:
# gcc -o celestiar celestiar.c -framework OpenGL -framework GLUT -lm