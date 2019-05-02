#include "Matrix.h"
#include <cstdio>
#include <cassert>

Matrix Matrix::bringrow(int row)
{
	Matrix ret = Matrix(1, getcoulmns());
	for (int i = 0; i < getcoulmns(); i++) {
		ret.start[i] = start[row*getcoulmns() + i];
	}
	return ret;
}

Matrix Matrix::bringcoulmn(int coulmn)
{
	Matrix ret = Matrix(getrows(),1);
	for (int i = 0; i < getrows(); i++) {
		ret.start[i] = start[i*getrows() + coulmn];
	}
	return ret;
}

int Matrix::lenght()
{
	return getrows()*getcoulmns();
}

Matrix::Matrix(int rows, int coulmns)
{
	assert(rows != 0 && coulmns != 0);
	row = rows;
	coulmn = coulmns;
	start = new float[row*coulmn];
}

Matrix::Matrix(const Matrix &rhs)
{
	row = rhs.getrows();
	coulmn = rhs.getcoulmns();
	delete[] start;
	start = new float[row*coulmn];
	for (int i = 0; i < row*coulmn; i++) {
		start[i] = rhs.start[i];
	}
}

Matrix::~Matrix()
{
	delete[] start;
	start = nullptr;
}

Matrix Matrix::operator*(Matrix & rhs)
{
	if (getcoulmns() != rhs.getrows()) {
		return Matrix(0, 0);
	}
	int x = getrows();
	int y = rhs.getcoulmns();
	Matrix rtn = Matrix(x, y);
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			Matrix row_t = bringrow(i);
			Matrix coulmn_t = rhs.bringcoulmn(j);
			float value = row_t.dot(coulmn_t);
			rtn.put(i, j, value);
		}
	}
	return rtn;
}

Matrix Matrix::operator+(Matrix & rhs)
{
	if ((getrows() != rhs.getrows()) || (getcoulmns() != rhs.getcoulmns())) {
		return Matrix(0, 0);
	}
	Matrix rtn = Matrix(getrows(), getcoulmns());
	for (int ix = 0; ix < getrows(); ix++) {
		for (int iy = 0; iy < getcoulmns(); iy++) {
			rtn.put(ix, iy, (get(ix, iy) + rhs.get(ix, iy)));
		}
	}
	return rtn;
}

Matrix Matrix::operator=(Matrix & rhs)
{
	row = rhs.row;
	coulmn = rhs.coulmn;
	delete[] start;
	start = new float[row*coulmn];
	for (int i = 0; i < row*coulmn; i++) {
		start[i] = rhs.start[i];
	}
	return *this;
}


int Matrix::getrows() const
{
	return row;
}

int Matrix::getcoulmns() const
{
	return coulmn;
}

float Matrix::get(int rows, int coulmns)
{
	assert(rows < row);
	assert(coulmns < coulmn);
	return start[rows*coulmn + coulmns];
}

void Matrix::put(int rows, int coulmns, float value)
{
	assert(rows < row);
	assert(coulmns < coulmn);
	start[rows*coulmn + coulmns] = value;
}

void Matrix::print()
{
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < coulmn; j++) {
			float m = get(i,j);
			printf("%f,", m);
		}
		printf("\n");
	}
}

float Matrix::dot(Matrix & rhs)
{
	assert(row == 1);
	assert(rhs.coulmn == 1);
	assert(coulmn == rhs.row);
	float retn = 0;
	for (int i = 0; i < coulmn; i++) {
		float q = 1;
		q = start[i] * rhs.start[i];
		retn = retn + q;
	}
	return retn;
}
