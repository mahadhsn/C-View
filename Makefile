c-view: ./src/C-View.c ./src/blur.c ./src/edgeDetect.c ./src/grayscale.c ./src/reflection.c ./src/rotation.c 
	gcc -o c-view ./src/C-View.c ./src/blur.c ./src/edgeDetect.c ./src/grayscale.c ./src/reflection.c ./src/rotation.c

c-view-test: ./src/C-View.c ./src/blur.c ./src/edgeDetect.c ./src/grayscale.c ./src/reflection.c ./src/rotation.c 
	gcc -o c-view-test --coverage ./src/C-View.c ./src/blur.c ./src/edgeDetect.c ./src/grayscale.c ./src/reflection.c ./src/rotation.c 