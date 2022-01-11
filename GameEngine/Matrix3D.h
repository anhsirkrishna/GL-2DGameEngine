#pragma once

#include <string>

class Matrix3D {
public:
	Matrix3D();
	void SetVal(unsigned int i, unsigned int j, float val);
	float GetVal(unsigned int i, unsigned int j) const;
	float* GetMatrixP();
	std::string PrintableStr();
	Matrix3D const operator*(Matrix3D const& rhs);
private:
	float matrix[16] = { 0 };
};

Matrix3D OrthographicProj(float left, float right, float bottom, float top, float front, float back);