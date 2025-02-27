#include <iostream>
#include <cmath>
#include <stdexcept>
#include "Vector.h"

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
	Vector3f vector;
	THROW_IF_FALSE(EQUAL(vector[0], 0.0f), "Default constructor failed: x != 0");
	THROW_IF_FALSE(EQUAL(vector[1], 0.0f), "Default constructor failed: y != 0");
	THROW_IF_FALSE(EQUAL(vector[2], 0.0f), "Default constructor failed: z != 0");
}

static void TEST_ParameterizedConstructor()
{
	Vector3f vector(1.0f, 2.0f, 3.0f);
	THROW_IF_FALSE(EQUAL(vector[0], 1.0f), "Parameterized constructor failed: x != 1");
	THROW_IF_FALSE(EQUAL(vector[1], 2.0f), "Parameterized constructor failed: y != 2");
	THROW_IF_FALSE(EQUAL(vector[2], 3.0f), "Parameterized constructor failed: z != 3");
}

static void TEST_ExplicitConstructor()
{
    Vector3f vector(1.0f);
    THROW_IF_FALSE(EQUAL(vector[0], 1.0f), "ExplicitConstructor failed: x != 1");
    THROW_IF_FALSE(EQUAL(vector[1], 1.0f), "ExplicitConstructor failed: y != 1");
    THROW_IF_FALSE(EQUAL(vector[2], 1.0f), "ExplicitConstructor failed: z != 1");
}

static void TEST_InitializeListConstructor()
{
	Vector3f vector{1.0f, 2.0f, 3.0f};
	THROW_IF_FALSE(EQUAL(vector[0], 1.0f), "Initializer list constructor failed: x != 1");
	THROW_IF_FALSE(EQUAL(vector[1], 2.0f), "Initializer list constructor failed: y != 2");
	THROW_IF_FALSE(EQUAL(vector[2], 3.0f), "Initializer list constructor failed: z != 3");
}

static void TEST_Addition()
{
	const Vector3f vector1(1.0f, 2.0f, 3.0f);
	const Vector3f vector2(4.0f, 5.0f, 6.0f);
	Vector3f result = vector1 + vector2;
	THROW_IF_FALSE(EQUAL(result[0], 5.0f), "Addition failed: x != 5");
	THROW_IF_FALSE(EQUAL(result[1], 7.0f), "Addition failed: y != 7");
	THROW_IF_FALSE(EQUAL(result[2], 9.0f), "Addition failed: z != 9");
}

static void TEST_ScalarAddition()
{
    Vector3f vector(2.0f, 3.0f, 4.0f);
    float scalar = 1.0f;

    Vector3f result = vector + scalar;
    THROW_IF_FALSE(EQUAL(result[0], 3.0f), "ScalarAddition failed: x + scalar != 3");
    THROW_IF_FALSE(EQUAL(result[1], 4.0f), "ScalarAddition failed: y + scalar != 4");
    THROW_IF_FALSE(EQUAL(result[2], 5.0f), "ScalarAddition failed: z + scalar != 5");

    vector += scalar;
    THROW_IF_FALSE(EQUAL(vector[0], 3.0f), "ScalarAddition failed: x += scalar != 3");
    THROW_IF_FALSE(EQUAL(vector[1], 4.0f), "ScalarAddition failed: y += scalar != 4");
    THROW_IF_FALSE(EQUAL(vector[2], 5.0f), "ScalarAddition failed: z += scalar != 5");
}

static void TEST_Negation()
{
    const Vector3f vector(2.0f, -3.0f, 4.0f);
    Vector3f result = -vector;

    THROW_IF_FALSE(EQUAL(result[0], -2.0f), "Negation failed: x != -2");
    THROW_IF_FALSE(EQUAL(result[1], 3.0f), "Negation failed: y != 3");
    THROW_IF_FALSE(EQUAL(result[2], -4.0f), "Negation failed: z != -4");
}

static void TEST_Subtraction()
{
	const Vector3f vector1(1.0f, 2.0f, 3.0f);
	const Vector3f vector2(2.0f, 4.0f, 9.0f);
	Vector3f result = vector2 - vector1;
	THROW_IF_FALSE(EQUAL(result[0], 1.0f), "Subtraction failed: x != 1");
	THROW_IF_FALSE(EQUAL(result[1], 2.0f), "Subtraction failed: y != 2");
	THROW_IF_FALSE(EQUAL(result[2], 6.0f), "Subtraction failed: z != 6");
}

static void TEST_ScalarMultiplication()
{
	const Vector3f vector(1.0f, 2.0f, 3.0f);
	Vector3f result = vector * 2.0f;
	THROW_IF_FALSE(EQUAL(result[0], 2.0f), "Scalar multiplication failed: x != 2");
	THROW_IF_FALSE(EQUAL(result[1], 4.0f), "Scalar multiplication failed: y != 4");
	THROW_IF_FALSE(EQUAL(result[2], 6.0f), "Scalar multiplication failed: z != 6");
}

void Test_DotProduct()
{
	const Vector3f vector1(1.0f, 2.0f, 3.0f);
	const Vector3f vector2(4.0f, 5.0f, 6.0f);
	const float result = vector1.dot(vector2);
	THROW_IF_FALSE(EQUAL(result, 32.0f), "Dot product failed: result != 32");
}

static void TEST_CrossProduct()
{
	const Vector3f vector1(1.0f, 2.0f, 3.0f);
	const Vector3f vector2(4.0f, 5.0f, 6.0f);
	const Vector3f result = vector1.cross(vector2);
	THROW_IF_FALSE(EQUAL(result[0], -3.0f), "Cross product failed: x != -3");
	THROW_IF_FALSE(EQUAL(result[1], 6.0f), "Cross product failed: y != 6");
	THROW_IF_FALSE(EQUAL(result[2], -3.0f), "Cross product failed: z != -3");
}

static void TEST_PlusEquals()
{
	Vector3f vector1(1.0f, 2.0f, 3.0f);
	const Vector3f vector2(4.0f, 5.0f, 6.0f);
	vector1 += vector2;
	THROW_IF_FALSE(EQUAL(vector1[0], 5.0f), "Plus equals failed: x != 5");
	THROW_IF_FALSE(EQUAL(vector1[1], 7.0f), "Plus equals failed: y != 7");
	THROW_IF_FALSE(EQUAL(vector1[2], 9.0f), "Plus equals failed: z != 9");
}

static void TEST_MinusEquals()
{
	Vector3f vector1(5.0f, 7.0f, 9.0f);
	const Vector3f vector2(4.0f, 5.0f, 6.0f);
	vector1 -= vector2;
	THROW_IF_FALSE(EQUAL(vector1[0], 1.0f), "Minus equals failed: x != 1");
	THROW_IF_FALSE(EQUAL(vector1[1], 2.0f), "Minus equals failed: y != 2");
	THROW_IF_FALSE(EQUAL(vector1[2], 3.0f), "Minus equals failed: z != 3");
}

static void TEST_TimesEquals()
{
	Vector3f vector(1.0f, 2.0f, 3.0f);
	vector *= 2.0f;
	THROW_IF_FALSE(EQUAL(vector[0], 2.0f), "Times equals failed: x != 2");
	THROW_IF_FALSE(EQUAL(vector[1], 4.0f), "Times equals failed: y != 4");
	THROW_IF_FALSE(EQUAL(vector[2], 6.0f), "Times equals failed: z != 6");
}

static void TEST_DivideEquals()
{
	Vector3f vector(2.0f, 4.0f, 6.0f);
	vector /= 2.0f;
	THROW_IF_FALSE(EQUAL(vector[0], 1.0f), "Divide equals failed: x != 1");
	THROW_IF_FALSE(EQUAL(vector[1], 2.0f), "Divide equals failed: y != 2");
	THROW_IF_FALSE(EQUAL(vector[2], 3.0f), "Divide equals failed: z != 3");
}

static void TEST_EqualsOperator()
{
	const Vector3f vector1(1.0f, 2.0f, 3.0f);
	const Vector3f vector2(1.0f, 2.0f, 3.0f);
	const Vector3f Vector3f(4.0f, 5.0f, 6.0f);

	THROW_IF_FALSE(vector1 == vector2, "Equals operator failed: vectors are not equal");
	THROW_IF_FALSE(vector1 != Vector3f, "Equals operator failed: vectors are equal");
}

static void TEST_Magnitude()
{
	const Vector3f vector(3.0f, 4.0f, 0.0f);
	const float result = vector.magnitude();
	THROW_IF_FALSE(EQUAL(result, 5.0f), "Magnitude failed: length != 5");
}

static void TEST_MagnitudeSquared()
{
	const Vector3f vector(3.0f, 4.0f, 0.0f);
	float result = vector.magnitudeSquared();
	THROW_IF_FALSE(EQUAL(result, 25.0f), "MagnitudeSquared failed: length^2 != 25");
}

static void TEST_Normalize()
{
	const Vector3f vector(3.0f, 4.0f, 0.0f);
	Vector3f result = vector.normalize();
	THROW_IF_FALSE(result.magnitude() == 1.0f, "Normalize failed: magnitude != 1");
	THROW_IF_FALSE(EQUAL(result[0], 0.6f), "Normalize failed: x != 0.6");
	THROW_IF_FALSE(EQUAL(result[1], 0.8f), "Normalize failed: y != 0.8");
	THROW_IF_FALSE(EQUAL(result[2], 0.0f), "Normalize failed: z != 0");
}

static void TEST_Distance()
{
	const Vector3f vector1(1.0f, 2.0f, 3.0f);
	const Vector3f vector2(4.0f, 6.0f, 3.0f);
	const float result = vector1.distance(vector2);
	THROW_IF_FALSE(EQUAL(result, 5.0f), "Distance failed: distance != 5");
}

static void TEST_DistanceSquared()
{
	const Vector3f vector1(1.0f, 2.0f, 3.0f);
	const Vector3f vector2(4.0f, 6.0f, 3.0f);
	const float result = vector1.distanceSquared(vector2);
	THROW_IF_FALSE(EQUAL(result, 25.0f), "DistanceSquared failed: distance^2 != 25");
}

static void TEST_Lerp()
{
	const Vector3f vector1(0.0f, 0.0f, 0.0f);
	const Vector3f vector2(10.0f, 10.0f, 10.0f);
	Vector3f result = vector1.lerp(vector2, 0.5f);
	THROW_IF_FALSE(EQUAL(result[0], 5.0f), "Lerp failed: x != 5");
	THROW_IF_FALSE(EQUAL(result[1], 5.0f), "Lerp failed: y != 5");
	THROW_IF_FALSE(EQUAL(result[2], 5.0f), "Lerp failed: z != 5");
}

static void TEST_Hadamard()
{
	const Vector3f vector1(2.0f, 3.0f, 4.0f);
	const Vector3f vector2(1.0f, 0.5f, 2.0f);
	Vector3f result = vector1.hadamard(vector2);
	THROW_IF_FALSE(EQUAL(result[0], 2.0f), "Hadamard failed: x != 2");
	THROW_IF_FALSE(EQUAL(result[1], 1.5f), "Hadamard failed: y != 1.5");
	THROW_IF_FALSE(EQUAL(result[2], 8.0f), "Hadamard failed: z != 8");
}

static void TEST_Pow()
{
    const Vector3f vector(2.0f, 3.0f, 4.0f);
    const float exponent = 2.0f;
    Vector3f result = vector.pow(exponent);
    THROW_IF_FALSE(EQUAL(result[0], 4.0f), "Pow failed: x^2 != 4");
    THROW_IF_FALSE(EQUAL(result[1], 9.0f), "Pow failed: y^2 != 9");
    THROW_IF_FALSE(EQUAL(result[2], 16.0f), "Pow failed: z^2 != 16");
}

int main()
{
	try
	{
		TEST_DefaultConstructor();
		TEST_ParameterizedConstructor();
		TEST_ExplicitConstructor();
		TEST_InitializeListConstructor();
		TEST_Addition();
		TEST_ScalarAddition();
		TEST_Negation();
		TEST_Subtraction();
		TEST_ScalarMultiplication();
		Test_DotProduct();
		TEST_CrossProduct();
		TEST_PlusEquals();
		TEST_MinusEquals();
		TEST_TimesEquals();
		TEST_DivideEquals();
		TEST_EqualsOperator();
		TEST_Magnitude();
		TEST_MagnitudeSquared();
		TEST_Normalize();
		TEST_Distance();
		TEST_DistanceSquared();
		TEST_Lerp();
		TEST_Hadamard();
		TEST_Pow();
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
