#include "homework.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>

#define MULT_X 1
#define MULT_Y 1

int num_threads;
int resize_factor;

image** out_ref;
image** in_ref;

int gK[3][3] = {{1, 2, 1},
		{2, 4, 2},
		{1, 2, 1}};

void readInput(const char * fileName, image *img) {
	FILE* f = fopen(fileName, "r");

	int varI;

	fscanf(f, "P%d", &varI);
	img->type = varI;
	fscanf(f, "%d", &varI);
	img->columnsNo = varI;
	fscanf(f, "%d", &varI);
	img->linesNo = varI;
	fscanf(f, "%d", &varI);
	img->colorsNo = varI;

	int i;

	fseek(f, 1, SEEK_CUR);
	if (img->type == PGM) {
		img->pgmMatrix = (unsigned char**)malloc(img->linesNo * sizeof(unsigned char*));
		for (i = 0; i < img->linesNo; i++) {
			img->pgmMatrix[i] = (unsigned char*)malloc(img->columnsNo * sizeof(unsigned char));
		}
		for (i = 0; i < img->linesNo; i++) {
			fread(img->pgmMatrix[i], 1, img->columnsNo, f);
		}
	}
	
	if (img->type == PPM) {
		img->ppmMatrix = (ppmCell**)malloc(img->linesNo * sizeof(ppmCell*));
		for (i = 0; i < img->linesNo; i++) {
			img->ppmMatrix[i] = (ppmCell*)malloc(img->columnsNo * sizeof(ppmCell));
		}
		for (i = 0; i < img->linesNo; i++) {
			fread(img->ppmMatrix[i], 1, 3 * img->columnsNo, f);
		}
	}

	fclose(f);
}

void writeData(const char * fileName, image *img) {
	FILE* f = fopen(fileName, "w");
	int i;

	fprintf(f, "P%d\n%d %d\n%d\n", img->type, img->columnsNo, img->linesNo, img->colorsNo);
	if (img->type == PGM) {
		for (i = 0; i < img->linesNo; i++) {
			fwrite(img->pgmMatrix[i], 1, img->columnsNo, f);
		}
	}

	if (img->type == PPM) {
		for (i = 0; i < img->linesNo; i++) {
			fwrite(img->ppmMatrix[i], 1, 3 * img->columnsNo, f);
		}
	}

	fclose(f);
}

void* resizeThread(void* var) {
	int thread_id = *(int*)var;
	int i, j, k, l;

	int start = thread_id * (*in_ref)->linesNo / num_threads;
	start = start / resize_factor * resize_factor;
	int end = thread_id < num_threads - 1 ? (thread_id + 1) * (*in_ref)->linesNo / num_threads : (*in_ref)->linesNo;

	if ((*in_ref)->type == PGM) {
		if (resize_factor % 2 == 0) {
			for (i = start; i < end; i += resize_factor) {
				for (j = 0; j < (*in_ref)->columnsNo; j += resize_factor) {
					int colorSum = 0;
					for (k = 0; k < resize_factor; k++) {
						for (l = 0; l < resize_factor; l++) {
							colorSum += (*in_ref)->pgmMatrix[i + k][j + l];
						}
					}
					colorSum /= pow(resize_factor, 2);
					(*out_ref)->pgmMatrix[i / resize_factor][j / resize_factor] = colorSum;
				}
			}
		} else {
			for (i = start; i < end; i += resize_factor) {
				for (j = 0; j < (*in_ref)->columnsNo; j += resize_factor) {
					int sum = 0;
					for (k = 0; k < resize_factor; k++) {
						for (l = 0; l < resize_factor; l++) {
							sum += (*in_ref)->pgmMatrix[i + k][j + l] * gK[k][l];
						}
					}
					(*out_ref)->pgmMatrix[i / resize_factor][j / resize_factor] = sum / 16;
				}
			}
		}
	}
	else if ((*in_ref)->type == PPM) {
		if (resize_factor % 2 == 0) {
			for (i = start; i < end; i += resize_factor) {
				for (j = 0; j < (*in_ref)->columnsNo; j += resize_factor) {
					int colorSumB = 0, colorSumG = 0, colorSumR = 0;
					for (k = 0; k < resize_factor; k++) {
						for (l = 0; l < resize_factor; l++) {
							colorSumB += (*in_ref)->ppmMatrix[i + k][j + l].B;
							colorSumG += (*in_ref)->ppmMatrix[i + k][j + l].G;
							colorSumR += (*in_ref)->ppmMatrix[i + k][j + l].R;
						}
					}
					colorSumB /= pow(resize_factor, 2);
					colorSumG /= pow(resize_factor, 2);
					colorSumR /= pow(resize_factor, 2);
					(*out_ref)->ppmMatrix[i / resize_factor][j / resize_factor].B = colorSumB;
					(*out_ref)->ppmMatrix[i / resize_factor][j / resize_factor].G = colorSumG;
					(*out_ref)->ppmMatrix[i / resize_factor][j / resize_factor].R = colorSumR;
				}
			}
		} else {
			for (i = start; i < end; i += resize_factor) {
				for (j = 0; j < (*in_ref)->columnsNo; j += resize_factor) {
					int sum1 = 0, sum2 = 0, sum3 = 0;
					for (k = 0; k < resize_factor; k++) {
						for (l = 0; l < resize_factor; l++) {
							sum1 += (*in_ref)->ppmMatrix[i + k][j + l].B * gK[k][l];
							sum2 += (*in_ref)->ppmMatrix[i + k][j + l].G * gK[k][l];
							sum3 += (*in_ref)->ppmMatrix[i + k][j + l].R * gK[k][l];
						}
					}
					(*out_ref)->ppmMatrix[i / resize_factor][j / resize_factor].B = sum1 / 16;
					(*out_ref)->ppmMatrix[i / resize_factor][j / resize_factor].G = sum2 / 16;
					(*out_ref)->ppmMatrix[i / resize_factor][j / resize_factor].R = sum3 / 16;
				}
			}
		}
	}

	return var;
}

void resize(image *in, image * out) {
	in_ref = &in;
	out_ref = &out;
	int i;

	in->linesNo = in->linesNo / resize_factor * resize_factor;
	in->columnsNo = in->columnsNo / resize_factor * resize_factor;
	out->linesNo = in->linesNo / resize_factor;
	out->columnsNo = in->columnsNo / resize_factor;
	out->colorsNo = in->colorsNo;

	if ((*in_ref)->type == PGM) {
		(*out_ref)->type = PGM;
		(*out_ref)->pgmMatrix = (unsigned char**)malloc((*out_ref)->linesNo * sizeof(unsigned char*));
		for (i = 0; i < (*out_ref)->linesNo; i++) {
			(*out_ref)->pgmMatrix[i] = (unsigned char*)malloc((*out_ref)->columnsNo * sizeof(unsigned char));
		}
	} else if ((*in_ref)->type == PPM) {
		(*out_ref)->type = PPM;
		(*out_ref)->ppmMatrix = (ppmCell**)malloc((*out_ref)->linesNo * sizeof(ppmCell*));
		for (i = 0; i < (*out_ref)->linesNo; i++) {
			(*out_ref)->ppmMatrix[i] = (ppmCell*)malloc((*out_ref)->columnsNo * sizeof(ppmCell));
		}
	}

	pthread_t tid[num_threads];
	int thread_id[num_threads];
	for(i = 0; i < num_threads; i++)
		thread_id[i] = i;

	for(i = 0; i < num_threads; i++) {
		pthread_create(&(tid[i]), NULL, resizeThread, &(thread_id[i]));
	}

	for(i = 0; i < num_threads; i++) {
		pthread_join(tid[i], NULL);
	}
}
