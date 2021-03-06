#ifndef HOMEWORK_H1
#define HOMEWORK_H1

typedef struct {
	char* type;
	int width, height, maxval;
	unsigned char** matrix;
}image;

void initialize(image *im);
void render(image *im);
void writeData(const char * fileName, image *img);

#endif /* HOMEWORK_H1 */