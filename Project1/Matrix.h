#pragma once

class Matrix {
private:
	int width;
	int height;
	float* start = nullptr;
public:
	Matrix(int wid, int hei);
	Matrix(const Matrix &rhs);
	~Matrix();
	int getwidth();
	int getheight();
	float get(int x, int y);
	void put(int x, int y, float value);
};