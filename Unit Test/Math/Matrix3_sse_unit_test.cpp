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
	Matrix3f matrix;
    THROW_IF_FALSE(EQUAL(matrix[0][0], 0.0f), "Default constructor failed: matrix[0][0] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[0][1], 0.0f), "Default constructor failed: matrix[0][1] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[0][2], 0.0f), "Default constructor failed: matrix[0][2] != 0.0");

    THROW_IF_FALSE(EQUAL(matrix[1][0], 0.0f), "Default constructor failed: matrix[1][0] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[1][1], 0.0f), "Default constructor failed: matrix[1][1] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[1][2], 0.0f), "Default constructor failed: matrix[1][2] != 0.0");

    THROW_IF_FALSE(EQUAL(matrix[2][0], 0.0f), "Default constructor failed: matrix[2][0] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[2][1], 0.0f), "Default constructor failed: matrix[2][1] != 0.0");
    THROW_IF_FALSE(EQUAL(matrix[2][2], 0.0f), "Default constructor failed: matrix[2][2] != 0.0");
}

static void TEST_ArrayConstructor()
{	
	Matrix3f matrix{
		std::array<Vector3f, 3>{
			Vector3f{1.0f, 2.0f, 3.0f},
			Vector3f{4.0f, 5.0f, 6.0f},
			Vector3f{7.0f, 8.0f, 9.0f}
		}
	};

    THROW_IF_FALSE(EQUAL(matrix[0][0], 1.0f), "Default constructor failed: matrix[0][0] != 1.0f");
    THROW_IF_FALSE(EQUAL(matrix[0][1], 2.0f), "Default constructor failed: matrix[0][1] != 2.0f");
    THROW_IF_FALSE(EQUAL(matrix[0][2], 3.0f), "Default constructor failed: matrix[0][2] != 3.0f");

    THROW_IF_FALSE(EQUAL(matrix[1][0], 4.0f), "Default constructor failed: matrix[1][0] != 4.0f");
    THROW_IF_FALSE(EQUAL(matrix[1][1], 5.0f), "Default constructor failed: matrix[1][1] != 5.0f");
    THROW_IF_FALSE(EQUAL(matrix[1][2], 6.0f), "Default constructor failed: matrix[1][2] != 6.0f");

    THROW_IF_FALSE(EQUAL(matrix[2][0], 7.0f), "Default constructor failed: matrix[2][0] != 7.0f");
    THROW_IF_FALSE(EQUAL(matrix[2][1], 8.0f), "Default constructor failed: matrix[2][1] != 8.0f");
    THROW_IF_FALSE(EQUAL(matrix[2][2], 9.0f), "Default constructor failed: matrix[2][2] != 9.0f");
}

static void TEST_InitializeListConstructor()
{	
	Matrix3f matrix{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};

    THROW_IF_FALSE(EQUAL(matrix[0][0], 1.0f), "Default constructor failed: matrix[0][0] != 1.0f");
    THROW_IF_FALSE(EQUAL(matrix[0][1], 2.0f), "Default constructor failed: matrix[0][1] != 2.0f");
    THROW_IF_FALSE(EQUAL(matrix[0][2], 3.0f), "Default constructor failed: matrix[0][2] != 3.0f");

    THROW_IF_FALSE(EQUAL(matrix[1][0], 4.0f), "Default constructor failed: matrix[1][0] != 4.0f");
    THROW_IF_FALSE(EQUAL(matrix[1][1], 5.0f), "Default constructor failed: matrix[1][1] != 5.0f");
    THROW_IF_FALSE(EQUAL(matrix[1][2], 6.0f), "Default constructor failed: matrix[1][2] != 6.0f");

    THROW_IF_FALSE(EQUAL(matrix[2][0], 7.0f), "Default constructor failed: matrix[2][0] != 7.0f");
    THROW_IF_FALSE(EQUAL(matrix[2][1], 8.0f), "Default constructor failed: matrix[2][1] != 8.0f");
    THROW_IF_FALSE(EQUAL(matrix[2][2], 9.0f), "Default constructor failed: matrix[2][2] != 9.0f");
}

static void TEST_IdentityMatrix()
{
	Matrix3f matrix = Matrix3f::identity();

    THROW_IF_FALSE(EQUAL(matrix[0][0], 1.0f), "Default constructor failed: matrix[0][0] != 1.0f");
    THROW_IF_FALSE(EQUAL(matrix[0][1], 0.0f), "Default constructor failed: matrix[0][1] != 0.0f");
    THROW_IF_FALSE(EQUAL(matrix[0][2], 0.0f), "Default constructor failed: matrix[0][2] != 0.0f");

    THROW_IF_FALSE(EQUAL(matrix[1][0], 0.0f), "Default constructor failed: matrix[1][0] != 0.0f");
    THROW_IF_FALSE(EQUAL(matrix[1][1], 1.0f), "Default constructor failed: matrix[1][1] != 1.0f");
    THROW_IF_FALSE(EQUAL(matrix[1][2], 0.0f), "Default constructor failed: matrix[1][2] != 0.0f");

    THROW_IF_FALSE(EQUAL(matrix[2][0], 0.0f), "Default constructor failed: matrix[2][0] != 0.0f");
    THROW_IF_FALSE(EQUAL(matrix[2][1], 0.0f), "Default constructor failed: matrix[2][1] != 0.0f");
    THROW_IF_FALSE(EQUAL(matrix[2][2], 1.0f), "Default constructor failed: matrix[2][2] != 1.0f");
}

static void TEST_Translate()
{
    {
        const Matrix3f matrix;
        Vector3f translation{1.0f, 2.0f, 3.0f};
        Matrix3f result = matrix.translate(translation);

        THROW_IF_FALSE(EQUAL(result[0][0], 0.0f), "Case 1 failed: result[0][0] != 0.0f");
        THROW_IF_FALSE(EQUAL(result[0][1], 0.0f), "Case 1 failed: result[0][1] != 0.0f");
        THROW_IF_FALSE(EQUAL(result[0][2], 1.0f), "Case 1 failed: result[0][2] != 1.0f");

        THROW_IF_FALSE(EQUAL(result[1][0], 0.0f), "Case 1 failed: result[1][0] != 0.0f");
        THROW_IF_FALSE(EQUAL(result[1][1], 0.0f), "Case 1 failed: result[1][1] != 0.0f");
        THROW_IF_FALSE(EQUAL(result[1][2], 2.0f), "Case 1 failed: result[1][2] != 2.0f");

        THROW_IF_FALSE(EQUAL(result[2][0], 0.0f), "Case 1 failed: result[2][0] != 0.0f");
        THROW_IF_FALSE(EQUAL(result[2][1], 0.0f), "Case 1 failed: result[2][1] != 0.0f");
        THROW_IF_FALSE(EQUAL(result[2][2], 0.0f), "Case 1 failed: result[2][2] != 0.0f");
    }

    {
        const Matrix3f matrix;
        Matrix3f result = matrix.translate(Vector3f{1.0f, 2.0f, 3.0f});

        THROW_IF_FALSE(EQUAL(result[0][0], 0.0f), "Case 2 failed: result[0][0] != 0.0f");
        THROW_IF_FALSE(EQUAL(result[0][1], 0.0f), "Case 2 failed: result[0][1] != 0.0f");
        THROW_IF_FALSE(EQUAL(result[0][2], 1.0f), "Case 2 failed: result[0][2] != 1.0f");

        THROW_IF_FALSE(EQUAL(result[1][0], 0.0f), "Case 2 failed: result[1][0] != 0.0f");
        THROW_IF_FALSE(EQUAL(result[1][1], 0.0f), "Case 2 failed: result[1][1] != 0.0f");
        THROW_IF_FALSE(EQUAL(result[1][2], 2.0f), "Case 2 failed: result[1][2] != 2.0f");

        THROW_IF_FALSE(EQUAL(result[2][0], 0.0f), "Case 2 failed: result[2][0] != 0.0f");
        THROW_IF_FALSE(EQUAL(result[2][1], 0.0f), "Case 2 failed: result[2][1] != 0.0f");
        THROW_IF_FALSE(EQUAL(result[2][2], 0.0f), "Case 2 failed: result[2][2] != 0.0f");
    }

    {
        Matrix3f matrix;
        Vector3f translation{1.0f, 2.0f, 3.0f};
        matrix.translate(translation);

        THROW_IF_FALSE(EQUAL(matrix[0][0], 0.0f), "Case 3 failed: matrix[0][0] != 0.0f");
        THROW_IF_FALSE(EQUAL(matrix[0][1], 0.0f), "Case 3 failed: matrix[0][1] != 0.0f");
        THROW_IF_FALSE(EQUAL(matrix[0][2], 1.0f), "Case 3 failed: matrix[0][2] != 1.0f");

        THROW_IF_FALSE(EQUAL(matrix[1][0], 0.0f), "Case 3 failed: matrix[1][0] != 0.0f");
        THROW_IF_FALSE(EQUAL(matrix[1][1], 0.0f), "Case 3 failed: matrix[1][1] != 0.0f");
        THROW_IF_FALSE(EQUAL(matrix[1][2], 2.0f), "Case 3 failed: matrix[1][2] != 2.0f");

        THROW_IF_FALSE(EQUAL(matrix[2][0], 0.0f), "Case 3 failed: matrix[2][0] != 0.0f");
        THROW_IF_FALSE(EQUAL(matrix[2][1], 0.0f), "Case 3 failed: matrix[2][1] != 0.0f");
        THROW_IF_FALSE(EQUAL(matrix[2][2], 0.0f), "Case 3 failed: matrix[2][2] != 0.0f");
    }

    {
        Matrix3f matrix;
        matrix.translate(Vector3f{1.0f, 2.0f, 3.0f});

        THROW_IF_FALSE(EQUAL(matrix[0][0], 0.0f), "Case 4 failed: matrix[0][0] != 0.0f");
        THROW_IF_FALSE(EQUAL(matrix[0][1], 0.0f), "Case 4 failed: matrix[0][1] != 0.0f");
        THROW_IF_FALSE(EQUAL(matrix[0][2], 1.0f), "Case 4 failed: matrix[0][2] != 1.0f");

        THROW_IF_FALSE(EQUAL(matrix[1][0], 0.0f), "Case 4 failed: matrix[1][0] != 0.0f");
        THROW_IF_FALSE(EQUAL(matrix[1][1], 0.0f), "Case 4 failed: matrix[1][1] != 0.0f");
        THROW_IF_FALSE(EQUAL(matrix[1][2], 2.0f), "Case 4 failed: matrix[1][2] != 2.0f");

        THROW_IF_FALSE(EQUAL(matrix[2][0], 0.0f), "Case 4 failed: matrix[2][0] != 0.0f");
        THROW_IF_FALSE(EQUAL(matrix[2][1], 0.0f), "Case 4 failed: matrix[2][1] != 0.0f");
        THROW_IF_FALSE(EQUAL(matrix[2][2], 0.0f), "Case 4 failed: matrix[2][2] != 0.0f");
    }
}

int main()
{
	try
	{
		TEST_DefaultConstructor();
		TEST_ArrayConstructor();
		TEST_InitializeListConstructor();
        TEST_IdentityMatrix();
        TEST_Translate();
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