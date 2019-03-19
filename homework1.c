#include "homework1.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

image** im_ref;

int num_threads;
int resolution;

int distance(image *im, int x0, int y0) {
	return floor(((float) abs(im->eqA * ((float) x0 + 0.5) + im->eqB * ((float) y0 + 0.5) + im->eqC) / sqrt(pow(im->eqA, 2) + pow(im->eqB, 2))));
}

void initialize(image *im, char* a, char* b, char* c) {
	int i;

	im->type = 5;
	im->linesNo = im->columnsNo = resolution;
	im->colorsNo = 255;
	im->eqA = atoi(a);
	im->eqB = atoi(b);
	im->eqC = atoi(c);

	im->imgMatrix = (unsigned char**)malloc(im->columnsNo * sizeof(unsigned char*));
	for (i = 0; i < im->columnsNo; i++) {
		im->imgMatrix[i] = (unsigned char*)malloc(im->linesNo * sizeof(unsigned char));
	}
}

void* renderThread(void* var) {
	int thread_id = *(int*)var;
	int start = thread_id * (*im_ref)->linesNo / num_threads;
	int end = thread_id < num_threads - 1 ? (thread_id + 1) * (*im_ref)->linesNo / num_threads : (*im_ref)->linesNo;
	int i, j;

	for (i = start; i < end; i++) {
		for (j = 0; j < (*im_ref)->columnsNo; j++) {
			if (distance(*im_ref, i, j) <= 0.03 * resolution) {
				(*im_ref)->imgMatrix[(*im_ref)->linesNo - j - 1][i] = 0;
			} else {
				(*im_ref)->imgMatrix[(*im_ref)->linesNo - j - 1][i] = 255;
			}
		}
	}

	return var;
}

void render(image *im) {
	im_ref = &im;
	int i;

	pthread_t tid[num_threads];
	int thread_id[num_threads];
	for(i = 0;i < num_threads; i++)
		thread_id[i] = i;

	for(i = 0; i < num_threads; i++) {
		pthread_create(&(tid[i]), NULL, renderThread, &(thread_id[i]));
	}

	for(i = 0; i < num_threads; i++) {
		pthread_join(tid[i], NULL);
	}
}

void writeData(const char * fileName, image *img) {
	FILE* f = fopen(fileName, "w");
	int i;

	fprintf(f, "P%d\n%d %d\n%d\n", img->type, img->columnsNo, img->linesNo, img->colorsNo);
	for (i = 0; i < img->linesNo; i++) {
		fwrite(img->imgMatrix[i], 1, img->columnsNo, f);
	}

	fclose(f);
}

