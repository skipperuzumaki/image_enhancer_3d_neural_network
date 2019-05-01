#include "Matrix.h"
#include <cstdio>
#include <cassert>

Matrix Matrix::bringrow(int row)
{
	Matrix ret = Matrix(1, getcoulmns());
	for (int i = 0; i < getcoulmns(); i++) {
		ret.put(0, i, get(row, i));
	}
	return ret;
}

Matrix Matrix::bringcoulmn(int coulmn)
{
	Matrix ret = Matrix(getrows(),1);
	for (int i = 0; i < getrows(); i++) {
		ret.put(i,0, get(i,coulmn));
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
	for (int i = 0; i < row; i++) {
		std::vector<float> temp;
		for (int j = 0; j < coulmn; j++) {
			temp.push_back(0.0f);
		}
		temp.shrink_to_fit();
		data.push_back(temp);
	}
	data.shrink_to_fit();
}

Matrix::Matrix(const Matrix &rhs)
	:
	Matrix(rhs.getrows(), rhs.getcoulmns())
{
	data = rhs.data;
}

Matrix::~Matrix()
{
	data.clear();
}

Matrix Matrix::operator*(Matrix & rhs)
{
	if (getcoulmns() != rhs.getrows()) {
	}
	int x = getrows();
	int y = rhs.getcoulmns();
	Matrix rtn = Matrix(x, y);
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			Matrix row_t = bringrow(i);
			Matrix coulmn_t = bringcoulmn(j);
			float value = 0;
			for (int k = 0; k < row_t.lenght(); k++) {
				value += row_t.get(0, k)*coulmn_t.get(k, 0);
			}
			rtn.put(i, j, value);
		}
	}
	return rtn;
}

Matrix Matrix::operator+(Matrix & rhs)
{
	if ((getrows() != rhs.getrows()) || (getcoulmns() != rhs.getcoulmns())) {
	}
	Matrix rtn = Matrix(getrows(), getcoulmns());
	for (int ix = 0; ix < getrows(); ix++) {
		for (int iy = 0; iy < getcoulmns(); iy++) {
			rtn.put(ix, iy, (get(ix, iy) + rhs.get(ix, iy)));
		}
	}
	return rtn;
}


int Matrix::getrows() const
{
	return row;
}

int Matrix::getcoulmns() const
{
	return coulmn;
}

float Matrix::get(int row, int coulmn)
{
	assert(row < getrows());
	assert(coulmn < getcoulmns());
	return data.at(row).at(coulmn);
}

void Matrix::put(int row, int coulmn, float value)
{
	assert(row < getrows());
	assert(coulmn < getcoulmns());
	data.at(row).at(coulmn) = value;
}

int main() {
	Matrix m = Matrix(2, 2);
	m.put(0, 0, 0);
	m.put(1, 0, 1);
	m.put(0, 1, 2);
	m.put(1, 0, 3);
	Matrix k = Matrix(2, 2);
	k.put(0, 0, 0);
	k.put(1, 0, 3);
	k.put(0, 1, 1);
	k.put(1, 0, 3);
	Matrix z = m * k;
	for (int i = 0; i < 2; i ++ ) {
		for (int j = 0; j < 2; j++) {
			float m = z.get(i, j);
			printf("%f,", m);
		}
	}
	scanf_s("%d");
	return 0;
}