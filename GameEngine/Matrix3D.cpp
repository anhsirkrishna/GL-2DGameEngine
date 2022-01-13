#include "Matrix3D.h"

/*Constructor sets the identity matrix*/
Matrix3D::Matrix3D() {
	matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1;
}

/*
* Setter to set specific values
* COLUMN MAJOR
*/
void Matrix3D::SetVal(unsigned int i, unsigned int j, float val) {
	matrix[(4 * j) + i] = val;
}

/*
* Returns the value at a particular index.
* COLUMN MAJOR
*/
float Matrix3D::GetVal(unsigned int i, unsigned int j) const {
	return matrix[(4 * j) + i];
}


/*
* Returns a pointer to the array that represents the Matrix
*/
float* Matrix3D::GetMatrixP() {
	return matrix;
}

/*
* Returns the Matrix in a readable string for debug purposes
* Returns std::string : The matrix in readable format
*/
std::string Matrix3D::PrintableStr() {
	std::string ret_string = "";
	for (unsigned int i = 0; i < 4; ++i) {
		for (unsigned int j = 0; j < 4; ++j) {
			ret_string += std::to_string(matrix[(4 * j) + i]);
			ret_string += ", \t ";
		}
		ret_string += "\n";
	}
	return ret_string;
}

/*
* Matrix multiplication between two matrices
* Does NOT verify the compatability of matrices for multiplication
* Ensure that the two matrices can be multiplied before calling this
* Returns Matrix3D : the result
*/
Matrix3D const Matrix3D::operator*(Matrix3D const& rhs) {
	Matrix3D result;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			result.SetVal(i, j, (GetVal(i, 0) * rhs.GetVal(0, j) + (GetVal(i, 1) * rhs.GetVal(1, j)) + (GetVal(i, 2) * rhs.GetVal(2, j))));
		}
	}

	return result;
}

/*This function will create an orthographic projection matrix with the parameters specified*/
Matrix3D OrthographicProj(float left, float right, float bottom, float top, float front, float back) {
	Matrix3D ret_matrix;

	ret_matrix.SetVal(0, 0, 2 / (right - left));
	ret_matrix.SetVal(1, 1, 2 / (top - bottom));
	ret_matrix.SetVal(2, 2, -2 / (back - front));

	ret_matrix.SetVal(0, 3, -(right + left) / (right - left));
	ret_matrix.SetVal(1, 3, -(top + bottom) / (top - bottom));
	ret_matrix.SetVal(2, 3, -(back + front) / (back - front));

	return ret_matrix;
}