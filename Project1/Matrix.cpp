#include "Matrix.h"

Matrix::Matrix(int wid, int hei)
	:
	start(new float[wid*hei])
{
	width = wid;
	height = hei;
}

Matrix::Matrix(const Matrix &rhs)
	:
	Matrix(rhs.width, rhs.height)
{
	const int nelmnt = width * height;
	for (int i = 0; i < nelmnt; i++) {
		start[i] = rhs.start[i];
	}
}

Matrix::~Matrix()
{
}

int Matrix::getwidth()
{
	return width;
}

int Matrix::getheight()
{
	return height;
}

float Matrix::get(int x, int y)
{
	return start[y*width + x];
}

void Matrix::put(int x, int y, float value)
{
	start[y*width + x] = value;
}
