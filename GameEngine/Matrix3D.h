#pragma once

#include <string>

/*
* A representation for a 4x4 matrix stored in a 1d float array.
* The matrix is a column major representation
*/
class Matrix3D {
public:
	/*Constructor sets the identity matrix*/
	Matrix3D();
	
	/*
	* Setter to set specific values
	* COLUMN MAJOR
	*/
	void SetVal(unsigned int i, unsigned int j, float val);
	
	/*
	* Returns the value at a particular index.
	* COLUMN MAJOR
	*/
	float GetVal(unsigned int i, unsigned int j) const;

	/*
	* Returns a pointer to the array that represents the Matrix
	*/
	float* GetMatrixP();

	/*
	* Returns the Matrix in a readable string for debug purposes
	* Returns std::string : The matrix in readable format
	*/
	std::string PrintableStr();

	/*
	* Matrix multiplication between two matrices
	* Does NOT verify the compatability of matrices for multiplication
	* Ensure that the two matrices can be multiplied before calling this
	* Returns Matrix3D : the result
	*/
	Matrix3D const operator*(Matrix3D const& rhs);
private:
	float matrix[16] = { 0 };
};


/*This function will create an orthographic projection matrix with the parameters specified*/
Matrix3D OrthographicProj(float left, float right, float bottom, float top, float front, float back);