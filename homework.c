#include "homework.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int num_threads;
int resize_factor;

image* aux_in;
image* aux_out;

//GaussianKernel
int GaussianKernel[3][3] = { {1, 2, 1}, {2, 4, 2}, {1, 2, 1} };

void readInput(const char * fileName, image *img) {

	FILE* file = fopen(fileName, "rb");

	img->type = (char*)malloc(2 * sizeof(char));

	fscanf(file, "%s", img->type);
	fscanf(file, "%d", &img->width);
	fscanf(file, "%d", &img->height);
	fscanf(file, "%d\n", &img->maxval);

	int i;

	if (strcmp(img->type, "P6") == 0) {

		img->matrix_color = (pixel**)malloc(img->height * sizeof(pixel*));
		// Aloc memorie pentru o matrice pixel
		for (i = 0; i < img->height; i++) {
			img->matrix_color[i] = (pixel*)malloc(img->width * sizeof(pixel));
		}
		// Citesc datele pentru matricea pixel
		for (i = 0; i < img->height; i++) {
			fread(img->matrix_color[i], sizeof(pixel), img->width, file);
		}
	}

	if (strcmp(img->type, "P5") == 0) {

		img->matrix_gray = (unsigned char**)malloc(img->height * sizeof(unsigned char*));
		// Aloc memorie pentru o matrice gray
		for (i = 0; i < img->height; i++) {
			img->matrix_gray[i] = (unsigned char*)malloc(img->width * sizeof(unsigned char));
		}
		// Citesc datele pentru matricea gray
		for (i = 0; i < img->height; i++) {
			fread(img->matrix_gray[i], sizeof(unsigned char), img->width, file);
		}
	}

	fclose(file);

}

void writeData(const char * fileName, image *img) {

	FILE* file = fopen(fileName, "wb");

	fprintf(file, "%s\n", img->type);
	fprintf(file, "%d %d\n", img->width, img->height);
	fprintf(file, "%d\n", img->maxval);

	int i;

	if (strcmp(img->type, "P6") == 0) {
		
		for (i = 0; i < img->height; i++) {
			fwrite(img->matrix_color[i], sizeof(pixel), img->width, file);
		}
	}

	if (strcmp(img->type, "P5") == 0) {
		
		for (i = 0; i < img->height; i++) {
			fwrite(img->matrix_gray[i], sizeof(unsigned char), img->width, file);
		}
	}

	fclose(file);


}

void* threadFunction(void *var) {

	int thread_id = *(int*)var;
	int i, j, k, l;

	int start = thread_id * aux_out->height / num_threads * resize_factor;
	int end = (thread_id + 1) * aux_out->height / num_threads * resize_factor;
	
	// Ca sa fie divizibile cu resize_factor
	while (start % resize_factor != 0){
		start++;
	}
 
	while (end % resize_factor != 0){
		end++;
	}

	// Resize pentru GW
	if (strcmp(aux_in->type, "P5") == 0) {

		if (resize_factor % 2 == 0) {
			int sum;
			// Merg in blocuri de dimensiunea (resize_factor x resize_factor)
			for (i = start; i < end; i += resize_factor) {
				for (j = 0; j < aux_out->width * resize_factor; j += resize_factor) {

					sum = 0;

					for (k = i; k < resize_factor + i; k++) {
						for (l = j; l < resize_factor + j; l++) {
							sum += (unsigned int)aux_in->matrix_gray[k][l];
						}
					}

					sum /= resize_factor * resize_factor;
					aux_out->matrix_gray[i / resize_factor][j / resize_factor] = (unsigned char)sum;
				}
			}
		}

		if (resize_factor == 3) {

			int sum;
			// Merg in blocuri de dimensiunea (resize_factor x resize_factor)
			for (i = start; i < end; i += resize_factor) {
				for (j = 0; j < aux_out->width * resize_factor; j += resize_factor) {

					sum = 0;

					for (k = i; k < resize_factor + i; k++) {
						for (l = j; l < resize_factor + j; l++) {
							sum +=(unsigned int)(aux_in->matrix_gray[k][l] * GaussianKernel[k % 3][l % 3]);
						}

					}
					sum /= 16;
					aux_out->matrix_gray[i / resize_factor][j / resize_factor] = (unsigned char)sum;
				}
			}
		}
	}

	// Rezise Color
	if (strcmp(aux_in->type, "P6") == 0) {

		if (resize_factor % 2 == 0) {

			int sum_red, sum_green, sum_blue;
			// Merg in blocuri de dimensiunea (resize_factor x resize_factor)
			for (i = start; i < end; i += resize_factor) {
				for (j = 0; j < aux_out->width * resize_factor; j += resize_factor) {

					sum_red = sum_green = sum_blue = 0;

					for (k = i; k < resize_factor + i; k++) {
						for (l = j; l < resize_factor + j; l++) {
							sum_red += (unsigned int)aux_in->matrix_color[k][l].red;
							sum_green += (unsigned int)aux_in->matrix_color[k][l].green;
							sum_blue += (unsigned int)aux_in->matrix_color[k][l].blue;
						}
					}

					sum_red /= resize_factor * resize_factor;
					sum_green /= resize_factor * resize_factor;
					sum_blue /= resize_factor * resize_factor;
					aux_out->matrix_color[i / resize_factor][j / resize_factor].red = (unsigned char)sum_red;
					aux_out->matrix_color[i / resize_factor][j / resize_factor].green = (unsigned char)sum_green;
					aux_out->matrix_color[i / resize_factor][j / resize_factor].blue = (unsigned char)sum_blue;

				}
			}
		}	

		if (resize_factor == 3) {

			int sum_red, sum_green, sum_blue;
			// Merg in blocuri de dimensiunea (resize_factor x resize_factor)
			for (i = start; i < end; i += resize_factor) {
				for (j = 0; j < aux_out->width * resize_factor; j += resize_factor) {

					sum_red = sum_green = sum_blue = 0;

					for (k = i; k < resize_factor + i; k++) {
						for (l = j; l < resize_factor + j; l++) {
							sum_red += (unsigned int)(aux_in->matrix_color[k][l].red * GaussianKernel[k % 3][l % 3]);
							sum_green += (unsigned int)(aux_in->matrix_color[k][l].green * GaussianKernel[k % 3][l % 3]);
							sum_blue += (unsigned int)(aux_in->matrix_color[k][l].blue * GaussianKernel[k % 3][l % 3]);
						}
					}

					sum_red /= 16;
					sum_green /= 16;
					sum_blue /= 16;
					aux_out->matrix_color[i / resize_factor][j / resize_factor].red = (unsigned char)sum_red;
					aux_out->matrix_color[i / resize_factor][j / resize_factor].green = (unsigned char)sum_green;
					aux_out->matrix_color[i / resize_factor][j / resize_factor].blue = (unsigned char)sum_blue;

				}
			}
		}
	}
}


void resize(image *in, image * out) { 

	int i;

	// Initializare parametrii pentru imaginea de out
	out->type = (char*)malloc(2 * sizeof(char));
	strcpy(out->type, in->type);
	out->width = in->width / resize_factor;
	out->height = in->height / resize_factor;
	out->maxval = in->maxval;

	// Alocare pentru GW
	if (strcmp(in->type, "P5") == 0) {

		out->matrix_gray = (unsigned char**)malloc(in->height * sizeof(unsigned char*));
		// Alocare memorie
		for (i = 0; i < in->height; i++) {
			out->matrix_gray[i] = (unsigned char*)malloc(in->width * sizeof(unsigned char));
		}
	}

	// Alocare pentru Color
	if (strcmp(in->type, "P6") == 0) {
		out->matrix_color = (pixel**)malloc(in->height * sizeof(pixel*));
		// Alocare memorie
		for (i = 0; i < in->height; i++) {
			out->matrix_color[i] = (pixel*)malloc(in->width * sizeof(pixel));
		}	
	}
	
	aux_in = in;
	aux_out = out;
	
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