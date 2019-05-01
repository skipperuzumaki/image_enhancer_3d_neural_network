#pragma once
#include <vector>

class Matrix {
private:
	int row;
	int coulmn;
	std::vector<std::vector<float>> data;
	Matrix bringrow(int row);
	Matrix bringcoulmn(int coulmn);
public:
	int lenght();
	Matrix(int rows, int coulmns);
	Matrix(const Matrix &rhs);
	~Matrix();
	Matrix operator *(Matrix &rhs);
	Matrix operator +(Matrix &rhs);
	int getrows() const;
	int getcoulmns() const;
	float get(int row, int coulmn);
	void put(int row, int coulmn, float value);
};