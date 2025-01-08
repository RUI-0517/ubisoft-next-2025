#include "Matrix.h"
#include <array>
#include <cmath>
#include <iostream>
#include <stdexcept>

static void THROW_IF_FALSE(const bool condition, const std::string &message)
{
	if (condition)
		return;
	throw std::runtime_error(message);
}

bool EQUAL(const float a, const float b, const float epsilon = 1e-5f)
{
	return std::fabs(a - b) < epsilon;
}

static void TEST_DefaultConstructor()
{
	Matrix4f matrix;
    THROW_IF_FALSE(EQUAL(matrix[0][0], 0.0f), "Default constructor failed: matrix[0][0] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[0][1], 0.0f), "Default constructor failed: matrix[0][1] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[0][2], 0.0f), "Default constructor failed: matrix[0][2] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[0][3], 0.0f), "Default constructor failed: matrix[0][3] != 0.0");

    THROW_IF_FALSE(EQUAL(matrix[1][0], 0.0f), "Default constructor failed: matrix[1][0] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[1][1], 0.0f), "Default constructor failed: matrix[1][1] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[1][2], 0.0f), "Default constructor failed: matrix[1][2] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[1][3], 0.0f), "Default constructor failed: matrix[1][3] != 0.0");

    THROW_IF_FALSE(EQUAL(matrix[2][0], 0.0f), "Default constructor failed: matrix[2][0] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[2][1], 0.0f), "Default constructor failed: matrix[2][1] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[2][2], 0.0f), "Default constructor failed: matrix[2][2] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[2][3], 0.0f), "Default constructor failed: matrix[2][3] != 0.0");

    THROW_IF_FALSE(EQUAL(matrix[3][0], 0.0f), "Default constructor failed: matrix[3][0] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[3][1], 0.0f), "Default constructor failed: matrix[3][1] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[3][2], 0.0f), "Default constructor failed: matrix[3][2] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[3][3], 0.0f), "Default constructor failed: matrix[3][3] != 0.0");
}

static void TEST_ArrayConstructor()
{	
	Matrix4f matrix{
		std::array<Vector4f, 4>{
			Vector4f{1.0f, 2.0f, 3.0f, 4.0f},
			Vector4f{5.0f, 6.0f, 7.0f, 8.0f},
			Vector4f{9.0f, 10.0f, 11.0f, 12.0f},
			Vector4f{13.0f, 14.0f, 15.0f, 16.0f}
		}
	};

    THROW_IF_FALSE(EQUAL(matrix[0][0], 1.0f), "Array constructor failed: matrix[0][0] != 1.0f");
    THROW_IF_FALSE(EQUAL(matrix[0][1], 2.0f), "Array constructor failed: matrix[0][1] != 2.0f");
    THROW_IF_FALSE(EQUAL(matrix[0][2], 3.0f), "Array constructor failed: matrix[0][2] != 3.0f");
    THROW_IF_FALSE(EQUAL(matrix[0][3], 4.0f), "Array constructor failed: matrix[0][3] != 4.0f");

    THROW_IF_FALSE(EQUAL(matrix[1][0], 5.0f), "Array constructor failed: matrix[1][0] != 5.0f");
    THROW_IF_FALSE(EQUAL(matrix[1][1], 6.0f), "Array constructor failed: matrix[1][1] != 6.0f");
    THROW_IF_FALSE(EQUAL(matrix[1][2], 7.0f), "Array constructor failed: matrix[1][2] != 7.0f");
    THROW_IF_FALSE(EQUAL(matrix[1][3], 8.0f), "Array constructor failed: matrix[1][3] != 8.0f");

    THROW_IF_FALSE(EQUAL(matrix[2][0], 9.0f), "Array constructor failed: matrix[2][0] != 9.0f");
    THROW_IF_FALSE(EQUAL(matrix[2][1], 10.0f), "Array constructor failed: matrix[2][1] != 10.0f");
    THROW_IF_FALSE(EQUAL(matrix[2][2], 11.0f), "Array constructor failed: matrix[2][2] != 11.0f");
    THROW_IF_FALSE(EQUAL(matrix[2][3], 12.0f), "Array constructor failed: matrix[2][3] != 12.0f");

    THROW_IF_FALSE(EQUAL(matrix[3][0], 13.0f), "Array constructor failed: matrix[3][0] != 13.0f");
    THROW_IF_FALSE(EQUAL(matrix[3][1], 14.0f), "Array constructor failed: matrix[3][1] != 14.0f");
    THROW_IF_FALSE(EQUAL(matrix[3][2], 15.0f), "Array constructor failed: matrix[3][2] != 15.0f");
    THROW_IF_FALSE(EQUAL(matrix[3][3], 16.0f), "Array constructor failed: matrix[3][3] != 16.0f");
}

static void TEST_InitializeListConstructor()
{	
	Matrix4f matrix{1.0f, 2.0f, 3.0f, 4.0f, 
                    5.0f, 6.0f, 7.0f, 8.0f, 
                    9.0f, 10.0f, 11.0f, 12.0f, 
                    13.0f, 14.0f, 15.0f, 16.0f};

    THROW_IF_FALSE(EQUAL(matrix[0][0], 1.0f), "Initialize List constructor failed: matrix[0][0] != 1.0f");
    THROW_IF_FALSE(EQUAL(matrix[0][1], 2.0f), "Initialize List constructor failed: matrix[0][1] != 2.0f");
    THROW_IF_FALSE(EQUAL(matrix[0][2], 3.0f), "Initialize List constructor failed: matrix[0][2] != 3.0f");
    THROW_IF_FALSE(EQUAL(matrix[0][3], 4.0f), "Initialize List constructor failed: matrix[0][3] != 4.0f");

    THROW_IF_FALSE(EQUAL(matrix[1][0], 5.0f), "Initialize List constructor failed: matrix[1][0] != 5.0f");
    THROW_IF_FALSE(EQUAL(matrix[1][1], 6.0f), "Initialize List constructor failed: matrix[1][1] != 6.0f");
    THROW_IF_FALSE(EQUAL(matrix[1][2], 7.0f), "Initialize List constructor failed: matrix[1][2] != 7.0f");
    THROW_IF_FALSE(EQUAL(matrix[1][3], 8.0f), "Initialize List constructor failed: matrix[1][3] != 8.0f");

    THROW_IF_FALSE(EQUAL(matrix[2][0], 9.0f), "Initialize List constructor failed: matrix[2][0] != 9.0f");
    THROW_IF_FALSE(EQUAL(matrix[2][1], 10.0f), "Initialize List constructor failed: matrix[2][1] != 10.0f");
    THROW_IF_FALSE(EQUAL(matrix[2][2], 11.0f), "Initialize List constructor failed: matrix[2][2] != 11.0f");
    THROW_IF_FALSE(EQUAL(matrix[2][3], 12.0f), "Initialize List constructor failed: matrix[2][3] != 12.0f");

    THROW_IF_FALSE(EQUAL(matrix[3][0], 13.0f), "Initialize List constructor failed: matrix[3][0] != 13.0f");
    THROW_IF_FALSE(EQUAL(matrix[3][1], 14.0f), "Initialize List constructor failed: matrix[3][1] != 14.0f");
    THROW_IF_FALSE(EQUAL(matrix[3][2], 15.0f), "Initialize List constructor failed: matrix[3][2] != 15.0f");
    THROW_IF_FALSE(EQUAL(matrix[3][3], 16.0f), "Initialize List constructor failed: matrix[3][3] != 16.0f");
}

static void TEST_IdentityMatrix()
{
	Matrix4f matrix = Matrix4f::identity();
    
    THROW_IF_FALSE(EQUAL(matrix[0][0], 1.0f), "Identity Matrix failed: matrix[0][0] != 1.0");
    THROW_IF_FALSE(EQUAL(matrix[0][1], 0.0f), "Identity Matrix failed: matrix[0][1] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[0][2], 0.0f), "Identity Matrix failed: matrix[0][2] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[0][3], 0.0f), "Identity Matrix failed: matrix[0][3] != 0.0");

    THROW_IF_FALSE(EQUAL(matrix[1][0], 0.0f), "Identity Matrix failed: matrix[1][0] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[1][1], 1.0f), "Identity Matrix failed: matrix[1][1] != 1.0");
    THROW_IF_FALSE(EQUAL(matrix[1][2], 0.0f), "Identity Matrix failed: matrix[1][2] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[1][3], 0.0f), "Identity Matrix failed: matrix[1][3] != 0.0");

    THROW_IF_FALSE(EQUAL(matrix[2][0], 0.0f), "Identity Matrix failed: matrix[2][0] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[2][1], 0.0f), "Identity Matrix failed: matrix[2][1] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[2][2], 1.0f), "Identity Matrix failed: matrix[2][2] != 1.0");
    THROW_IF_FALSE(EQUAL(matrix[2][3], 0.0f), "Identity Matrix failed: matrix[2][3] != 0.0");

    THROW_IF_FALSE(EQUAL(matrix[3][0], 0.0f), "Identity Matrix failed: matrix[3][0] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[3][1], 0.0f), "Identity Matrix failed: matrix[3][1] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[3][2], 0.0f), "Identity Matrix failed: matrix[3][2] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[3][3], 1.0f), "Identity Matrix failed: matrix[3][3] != 1.0");
}

// TODO: Translate & Scale Matrix

int main()
{
	try
	{
		TEST_DefaultConstructor();
		TEST_ArrayConstructor();
		TEST_InitializeListConstructor();
        TEST_IdentityMatrix();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Test failed: " << e.what() << "\n";
		return 1;
	}
	catch (...)
	{
		std::cerr << "An unknown error occurred during the tests.\n";
		return 1;
	}
	std::cout << "All tests passed!\n";
	return 0;
}