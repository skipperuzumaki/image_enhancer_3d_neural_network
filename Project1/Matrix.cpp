#include "Matrix.h"
#include <cstdio>
#include <ctime>
#include <cstdlib>

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
	row = rows;
	coulmn = coulmns;
	start = new float[row*coulmn];
}

Matrix::Matrix(const Matrix &rhs)
{
	row = rhs.getrows();
	coulmn = rhs.getcoulmns();
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
			rtn->put(i, j, value);
		}
	}
	return rtn;
}

Matrix* Matrix::operator+(Matrix & rhs)
{
	if ((getrows() != rhs.getrows()) || (getcoulmns() != rhs.getcoulmns())) {
		return nullptr;
	}
	Matrix* rtn = new Matrix(getrows(), getcoulmns());
	for (int ix = 0; ix < getrows(); ix++) {
		for (int iy = 0; iy < getcoulmns(); iy++) {
			rtn->put(ix, iy, (get(ix, iy) + rhs.get(ix, iy)));
		}
	}
	return rtn;
}

Matrix* Matrix::operator=(const Matrix & rhs)
{
	row = rhs.row;
	coulmn = rhs.coulmn;
	delete[] start;
	start = new float[row*coulmn];
	for (int i = 0; i < row*coulmn; i++) {
		start[i] = rhs.start[i];
	}
	return this;
}

bool Matrix::operator==(const Matrix & rhs)
{
	if (row != rhs.row || coulmn != rhs.coulmn) {
		return false;
	}
	else {
		for (int i = 0; i < row*coulmn; i++) {
			if (start[i] != rhs.start[i]) {
				return false;
			}
		}
	}
	return true;
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
	return start[rows*coulmn + coulmns];
}

void Matrix::put(int rows, int coulmns, float value)
{
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
	float retn = 0;
	for (int i = 0; i < coulmn; i++) {
		float q = 1;
		q = start[i] * rhs.start[i];
		retn = retn + q;
	}
	return retn;
}

Matrix* Matrix::CalcVariance(Matrix* rhs)
{
	Matrix* rtn = new Matrix(row, coulmn);
	for (int i = 0; i < row*coulmn; i++) {
		rtn->start[i] = rhs->start[i] - start[i];
	}
	return rtn;
}

float Matrix::Maxval()
{
	float rtn = std::numeric_limits<float>::min();
	for (int i = 0; i < row*coulmn; i++) {
		if (start[i] > rtn) {
			rtn = start[i];
		}
	}
	return rtn;
}

void Matrix::Percentise(float maxval)
{
	for (int i = 0; i < row*coulmn; i++) {
		float k = (start[i] / maxval)*100.0f;
		start[i] = k;
	}
}

void Matrix::Setall(float val)
{
	for (int i = 0; i < row*coulmn; i++) {
		start[i] = val;
	}
}

Matrix* Matrix::Invert()
{
	Matrix* rtn = new Matrix(row, coulmn);
	for (int i = 0; i < row*coulmn; i++) {
		rtn->start[i] = -start[i];
	}
	return rtn;
}

void Matrix::Sigmoid()
{
	for (int i = 0; i < row*coulmn; i++) {
		start[i] = std::pow((1+std::exp(-start[i])),-1);
	}
}

void Matrix::RandomlyInitialise(float range)
{
	srand(int(time(0)));
	for (int i = 0; i < row*coulmn; i++) {
		int r = rand() % (int(range) * 2) - int(range);
		start[i] = float(r);
	}
}
