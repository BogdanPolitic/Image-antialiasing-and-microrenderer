#ifndef HOMEWORK_H1
#define HOMEWORK_H1

typedef struct {
	int type;
	int linesNo;
	int columnsNo;
	unsigned char colorsNo;
	unsigned char** imgMatrix;
	float eqA, eqB, eqC;
}image;

void initialize(image *im, int a, int b);
void render(image *im);
void writeData(const char * fileName, image *img);

#endif /* HOMEWORK_H1 */
