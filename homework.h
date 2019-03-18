#ifndef HOMEWORK_H
#define HOMEWORK_H

#define PGM 5
#define PPM 6

typedef struct {
	unsigned char R;
	unsigned char G;
	unsigned char B;
} ppmCell;

typedef struct {
	unsigned char type;
	int linesNo;
	int columnsNo;
	unsigned char colorsNo;
	unsigned char** pgmMatrix;
	ppmCell** ppmMatrix;
}image;

void readInput(const char * fileName, image *img);

void writeData(const char * fileName, image *img);

void resize(image *in, image * out);

#endif /* HOMEWORK_H */
