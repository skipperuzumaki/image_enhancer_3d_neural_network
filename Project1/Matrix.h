#pragma once
#include <vector>

class Matrix {
private:
	int row;
	int coulmn;
	float * start;
public:
	Matrix bringrow(int row);
	Matrix bringcoulmn(int coulmn);
	int lenght();
	Matrix(int rows, int coulmns);
	Matrix(const Matrix &rhs);
	~Matrix();
	Matrix operator *(Matrix &rhs);
	Matrix operator +(Matrix &rhs);
	Matrix operator =(const Matrix &rhs);
	int getrows() const;
	int getcoulmns() const;
	float get(int row, int coulmn);
	void put(int row, int coulmn, float value);
	void print();
	float dot(Matrix &rhs);
};