#ifndef HOMEWORK_H
#define HOMEWORK_H

typedef struct {
	unsigned char red, green, blue;
}pixel;

typedef struct {
	char* type;
	int width, height, maxval;
	pixel** matrix_color;
	unsigned char** matrix_gray;
}image;

void readInput(const char * fileName, image *img);

void writeData(const char * fileName, image *img);

void resize(image *in, image * out);

#endif /* HOMEWORK_H */