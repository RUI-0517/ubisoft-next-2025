#include <stdafx.h>
#include "../include/Vector3.h"
#include <iostream>
#include <stdexcept>

static void THROW_IF_FALSE(const bool condition, const std::string& message)
{
	if (condition) return;
	throw std::runtime_error(message);
}

bool EQUAL(const float a, const float b, const float epsilon = 1e-5f)
{
	return std::fabs(a - b) < epsilon;
}

static void TEST_DefaultConstructor()
{
	Vector3 vector;
	THROW_IF_FALSE(EQUAL(vector[0], 0.0f), "Default constructor failed: x != 0");
	THROW_IF_FALSE(EQUAL(vector[1], 0.0f), "Default constructor failed: y != 0");
	THROW_IF_FALSE(EQUAL(vector[2], 0.0f), "Default constructor failed: z != 0");
}

static void TEST_ParameterizedConstructor()
{
	Vector3 vector(1.0f, 2.0f, 3.0f);
	THROW_IF_FALSE(EQUAL(vector[0], 1.0f), "Parameterized constructor failed: x != 1");
	THROW_IF_FALSE(EQUAL(vector[1], 2.0f), "Parameterized constructor failed: y != 2");
	THROW_IF_FALSE(EQUAL(vector[2], 3.0f), "Parameterized constructor failed: z != 3");
}

static void TEST_InitializeListConstructor()
{
	Vector3 vector{1.0f, 2.0f, 3.0f};
	THROW_IF_FALSE(EQUAL(vector[0], 1.0f), "Initializer list constructor failed: x != 1");
	THROW_IF_FALSE(EQUAL(vector[1], 2.0f), "Initializer list constructor failed: y != 2");
	THROW_IF_FALSE(EQUAL(vector[2], 3.0f), "Initializer list constructor failed: z != 3");
}

static void TEST_Addition()
{
	const Vector3 vector1(1.0f, 2.0f, 3.0f);
	const Vector3 vector2(4.0f, 5.0f, 6.0f);
	Vector3 result = vector1 + vector2;
	THROW_IF_FALSE(EQUAL(result[0], 5.0f), "Addition failed: x != 5");
	THROW_IF_FALSE(EQUAL(result[1], 7.0f), "Addition failed: y != 7");
	THROW_IF_FALSE(EQUAL(result[2], 9.0f), "Addition failed: z != 9");
}

static void TEST_Subtraction()
{
	const Vector3 vector1(1.0f, 2.0f, 3.0f);
	const Vector3 vector2(2.0f, 4.0f, 9.0f);
	Vector3 result = vector2 - vector1;
	THROW_IF_FALSE(EQUAL(result[0], 1.0f), "Subtraction failed: x != 1");
	THROW_IF_FALSE(EQUAL(result[1], 2.0f), "Subtraction failed: y != 2");
	THROW_IF_FALSE(EQUAL(result[2], 6.0f), "Subtraction failed: z != 6");
}

static void TEST_ScalarMultiplication()
{
	const Vector3 vector(1.0f, 2.0f, 3.0f);
	Vector3 result = vector * 2.0f;
	THROW_IF_FALSE(EQUAL(result[0], 2.0f), "Scalar multiplication failed: x != 2");
	THROW_IF_FALSE(EQUAL(result[1], 4.0f), "Scalar multiplication failed: y != 4");
	THROW_IF_FALSE(EQUAL(result[2], 6.0f), "Scalar multiplication failed: z != 6");
}

void TestDotProduct()
{
	const Vector3 vector1(1.0f, 2.0f, 3.0f);
	const Vector3 vector2(4.0f, 5.0f, 6.0f);
	const float result = vector1.dot(vector2);
	THROW_IF_FALSE(EQUAL(result, 32.0f), "Dot product failed: result != 32");
}

static void TEST_CrossProduct()
{
	const Vector3 vector1(1.0f, 2.0f, 3.0f);
	const Vector3 vector2(4.0f, 5.0f, 6.0f);
	Vector3 result = vector1.cross(vector2);
	THROW_IF_FALSE(EQUAL(result[0], -3.0f), "Cross product failed: x != -3");
	THROW_IF_FALSE(EQUAL(result[1], 6.0f), "Cross product failed: y != 6");
	THROW_IF_FALSE(EQUAL(result[2], -3.0f), "Cross product failed: z != -3");
}

static void TEST_PlusEquals()
{
	Vector3 vector1(1.0f, 2.0f, 3.0f);
	const Vector3 vector2(4.0f, 5.0f, 6.0f);
	vector1 += vector2;
	THROW_IF_FALSE(EQUAL(vector1[0], 5.0f), "Plus equals failed: x != 5");
	THROW_IF_FALSE(EQUAL(vector1[1], 7.0f), "Plus equals failed: y != 7");
	THROW_IF_FALSE(EQUAL(vector1[2], 9.0f), "Plus equals failed: z != 9");
}

static void TEST_MinusEquals()
{
	Vector3 vector1(5.0f, 7.0f, 9.0f);
	const Vector3 vector2(4.0f, 5.0f, 6.0f);
	vector1 -= vector2;
	THROW_IF_FALSE(EQUAL(vector1[0], 1.0f), "Minus equals failed: x != 1");
	THROW_IF_FALSE(EQUAL(vector1[1], 2.0f), "Minus equals failed: y != 2");
	THROW_IF_FALSE(EQUAL(vector1[2], 3.0f), "Minus equals failed: z != 3");
}

static void TEST_TimesEquals()
{
	Vector3 vector(1.0f, 2.0f, 3.0f);
	vector *= 2.0f;
	THROW_IF_FALSE(EQUAL(vector[0], 2.0f), "Times equals failed: x != 2");
	THROW_IF_FALSE(EQUAL(vector[1], 4.0f), "Times equals failed: y != 4");
	THROW_IF_FALSE(EQUAL(vector[2], 6.0f), "Times equals failed: z != 6");
}

static void TEST_DivideEquals()
{
	Vector3 vector(2.0f, 4.0f, 6.0f);
	vector /= 2.0f;
	THROW_IF_FALSE(EQUAL(vector[0], 1.0f), "Divide equals failed: x != 1");
	THROW_IF_FALSE(EQUAL(vector[1], 2.0f), "Divide equals failed: y != 2");
	THROW_IF_FALSE(EQUAL(vector[2], 3.0f), "Divide equals failed: z != 3");
}

static void TEST_EqualsOperator()
{
	const Vector3 vector1(1.0f, 2.0f, 3.0f);
	const Vector3 vector2(1.0f, 2.0f, 3.0f);
	const Vector3 vector3(4.0f, 5.0f, 6.0f);

	THROW_IF_FALSE(vector1 == vector2, "Equals operator failed: vectors are not equal");
	THROW_IF_FALSE(vector1 != vector3, "Equals operator failed: vectors are equal");
}

static void TEST_Magnitude()
{
	const Vector3 vector(3.0f, 4.0f, 0.0f);
	const float result = vector.magnitude();
	THROW_IF_FALSE(EQUAL(result, 5.0f), "Magnitude failed: length != 5");
}

static void TEST_MagnitudeSquared()
{
	const Vector3 vector(3.0f, 4.0f, 0.0f);
	float result = vector.magnitudeSquared();
	THROW_IF_FALSE(EQUAL(result, 25.0f), "MagnitudeSquared failed: length^2 != 25");
}

static void TEST_Normalize()
{
	const Vector3 vector(3.0f, 4.0f, 0.0f);
	Vector3 result = vector.normalize();
	THROW_IF_FALSE(result.magnitude() == 1.0f, "Normalize failed: magnitude != 1");
	THROW_IF_FALSE(EQUAL(result[0], 0.6f), "Normalize failed: x != 0.6");
	THROW_IF_FALSE(EQUAL(result[1], 0.8f), "Normalize failed: y != 0.8");
	THROW_IF_FALSE(EQUAL(result[2], 0.0f), "Normalize failed: z != 0");
}

static void TEST_Distance()
{
	const Vector3 vector1(1.0f, 2.0f, 3.0f);
	const Vector3 vector2(4.0f, 6.0f, 3.0f);
	const float result = vector1.distance(vector2);
	THROW_IF_FALSE(EQUAL(result, 5.0f), "Distance failed: distance != 5");
}

static void TEST_DistanceSquared()
{
	const Vector3 vector1(1.0f, 2.0f, 3.0f);
	const Vector3 vector2(4.0f, 6.0f, 3.0f);
	const float result = vector1.distanceSquared(vector2);
	THROW_IF_FALSE(EQUAL(result, 25.0f), "DistanceSquared failed: distance^2 != 25");
}

static void TEST_Lerp()
{
	const Vector3 vector1(0.0f, 0.0f, 0.0f);
	const Vector3 vector2(10.0f, 10.0f, 10.0f);
	Vector3 result = vector1.lerp(vector2, 0.5f);
	THROW_IF_FALSE(EQUAL(result[0], 5.0f), "Lerp failed: x != 5");
	THROW_IF_FALSE(EQUAL(result[1], 5.0f), "Lerp failed: y != 5");
	THROW_IF_FALSE(EQUAL(result[2], 5.0f), "Lerp failed: z != 5");
}

static void TEST_Hadamard()
{
	const Vector3 vector1(2.0f, 3.0f, 4.0f);
	const Vector3 vector2(1.0f, 0.5f, 2.0f);
	Vector3 result = vector1.hadamard(vector2);
	THROW_IF_FALSE(EQUAL(result[0], 2.0f), "Hadamard failed: x != 2");
	THROW_IF_FALSE(EQUAL(result[1], 1.5f), "Hadamard failed: y != 1.5");
	THROW_IF_FALSE(EQUAL(result[2], 8.0f), "Hadamard failed: z != 8");
}

static void TEST_Clamp()
{
	const Vector3 vector(10.0f, 20.0f, 30.0f);
	Vector3 result = vector.clamp(5.0f, 15.0f);
	THROW_IF_FALSE(EQUAL(result.magnitude(), 15.0f), "Clamp failed: magnitude != 15");
	result = vector.clamp(50.0f, 100.0f);
	THROW_IF_FALSE(EQUAL(result.magnitude(), 50.0f), "Clamp failed: magnitude != 50");
}

int main()
{
	std::cout << "Running Vector3 Tests...\n";
	TEST_DefaultConstructor();
	TEST_ParameterizedConstructor();
	TEST_InitializeListConstructor();
	TEST_Addition();
	TEST_Subtraction();
	TEST_ScalarMultiplication();
	TestDotProduct();
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
	TEST_Clamp();
	std::cout << "All tests passed!\n";
	return 0;
}
