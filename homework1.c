#include "homework1.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int num_threads;
int resolution;

image* aux;

void initialize(image *im) {

	im->type = (char*)malloc(2 * sizeof(char));
	strcpy(im->type, "P5");
	im->width = resolution;
	im->height = resolution;
	im->maxval = 255;

	int i;

	im->matrix = (unsigned char**)malloc(resolution * sizeof(unsigned char*));
	for (i = 0; i < resolution; i++) {
		im->matrix[i] = (unsigned char*)malloc(resolution * sizeof(unsigned char));
	}
	
}

void* threadFunction(void *var) {

	int thread_id = *(int*)var;

	int start = thread_id * resolution / num_threads;
	int end = (thread_id + 1) * resolution / num_threads;

	int i, j;
	double distance, x, y;
	double res_aux = ((double)100 / resolution);

	for (i = start; i < end; i++) {
		for (j = 0; j < resolution; j++) {
			// Calculez distanta
			x = j * res_aux + res_aux * 0.5;
			y = (resolution - 1 - i) * res_aux + res_aux * 0.5;
			distance = (abs((-1) * x + 2 * y) / sqrt(5.0f));
			// Verifica grosimea
			if (distance <= 3.0f) {
				aux->matrix[i][j] = 0;
			} else {
				aux->matrix[i][j] = 255;
			}
		}
	}
}

void render(image *im) {

	int i;
	aux = im;
	
	pthread_t tid[num_threads];
	int thread_id[num_threads];

	for(i = 0;i < num_threads; i++)
		thread_id[i] = i;

	for(i = 0; i < num_threads; i++) {
		pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
	}

	for(i = 0; i < num_threads; i++) {
		pthread_join(tid[i], NULL);
	}

}

void writeData(const char * fileName, image *img) {

	FILE* file = fopen(fileName, "wb");

	fprintf(file, "%s\n", img->type);
	fprintf(file, "%d %d\n", img->width, img->height);
	fprintf(file, "%d\n", img->maxval);

	int i;

	for (i = 0; i < img->height; i++) {
		fwrite(img->matrix[i], sizeof(unsigned char), img->width, file);
	}

	fclose(file);

}

