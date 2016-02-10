a.out: mandelbrotParallel_Zacharias_4_2016smalyala.c
	mpicc -lGL -lGLU -lglut mandelbrotParallel_Zacharias_4_2016smalyala.c

clean:
	rm a.out
