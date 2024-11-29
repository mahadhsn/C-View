c-view: C-View.c blur.c edgeDetect.c grayscale.c reflection.c rotation.c 
	gcc -o c-view C-View.c blur.c edgeDetect.c grayscale.c reflection.c rotation.c

c-view-test: C-View.c blur.c edgeDetect.c grayscale.c reflection.c rotation.c 
	gcc -o c-view-test --coverage C-View.c blur.c edgeDetect.c grayscale.c reflection.c rotation.c 