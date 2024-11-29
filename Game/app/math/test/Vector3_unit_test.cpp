#include "stdafx.h"
#include "../include/Vector3.h"
#include <iostream>
#include <stdexcept>

void THROW_IF_FALSE(const bool condition, const std::string& message)
{
	if (condition) return;
	throw std::runtime_error(message);
}

void TEST_DefaultConstructor()
{
	Vector3 vector;
	THROW_IF_FALSE(vector[0] == 0.0f, "Default constructor failed: x != 0");
	THROW_IF_FALSE(vector[1] == 0.0f, "Default constructor failed: y != 0");
	THROW_IF_FALSE(vector[2] == 0.0f, "Default constructor failed: z != 0");
}

void TEST_ParameterizedConstructor()
{
	Vector3 vector(1.0f, 2.0f, 3.0f);
	THROW_IF_FALSE(vector[0] == 1.0f, "Parameterized constructor failed: x != 1");
	THROW_IF_FALSE(vector[1] == 2.0f, "Parameterized constructor failed: y != 2");
	THROW_IF_FALSE(vector[2] == 3.0f, "Parameterized constructor failed: z != 3");
}

void TEST_InitializeListConstructor()
{
	Vector3 vector{1.0f, 2.0f, 3.0f};
	THROW_IF_FALSE(vector[0] == 1.0f, "Initializer list constructor failed: x != 1");
	THROW_IF_FALSE(vector[1] == 2.0f, "Initializer list constructor failed: y != 2");
	THROW_IF_FALSE(vector[2] == 3.0f, "Initializer list constructor failed: z != 3");
}

void TEST_Addition()
{
	const Vector3 vector1(1.0f, 2.0f, 3.0f);
	const Vector3 vector2(4.0f, 5.0f, 6.0f);
	Vector3 result = vector1 + vector2;
	THROW_IF_FALSE(result[0] == 5.0f, "Addition failed: x != 5");
	THROW_IF_FALSE(result[1] == 7.0f, "Addition failed: y != 7");
	THROW_IF_FALSE(result[2] == 9.0f, "Addition failed: z != 9");
}

void TEST_Subtraction()
{
	const Vector3 vector1(1.0f, 2.0f, 3.0f);
	const Vector3 vector2(2.0f, 4.0f, 9.0f);
	Vector3 result = vector2 - vector1;
	THROW_IF_FALSE(result[0] == 1.0f, "Subtraction failed: x != 1");
	THROW_IF_FALSE(result[1] == 2.0f, "Subtraction failed: y != 2");
	THROW_IF_FALSE(result[2] == 6.0f, "Subtraction failed: z != 6");
}

void TEST_ScalarMultiplication()
{
	const Vector3 vector(1.0f, 2.0f, 3.0f);
	Vector3 result = vector * 2.0f;
	THROW_IF_FALSE(result[0] == 2.0f, "Scalar multiplication failed: x != 2");
	THROW_IF_FALSE(result[1] == 4.0f, "Scalar multiplication failed: y != 4");
	THROW_IF_FALSE(result[2] == 6.0f, "Scalar multiplication failed: z != 6");
}

void TEST_DotProduct()
{
	const Vector3 vector1(1.0f, 2.0f, 3.0f);
	const Vector3 vector2(4.0f, 5.0f, 6.0f);
	const float result = vector1.dot(vector2);
	THROW_IF_FALSE(result == 32.0f, "Dot product failed: result != 32");
}

void TEST_CrossProduct()
{
	const Vector3 vector1(1.0f, 2.0f, 3.0f);
	const Vector3 vector2(4.0f, 5.0f, 6.0f);
	Vector3 result = vector1.cross(vector2);
	THROW_IF_FALSE(result[0] == -3.0f, "Cross product failed: x != -3");
	THROW_IF_FALSE(result[1] == 6.0f, "Cross product failed: y != 6");
	THROW_IF_FALSE(result[2] == -3.0f, "Cross product failed: z != -3");
}

void TEST_PlusEquals()
{
	Vector3 vector1(1.0f, 2.0f, 3.0f);
	const Vector3 vector2(4.0f, 5.0f, 6.0f);
	vector1 += vector2;
	THROW_IF_FALSE(vector1[0] == 5.0f, "Plus equals failed: x != 5");
	THROW_IF_FALSE(vector1[1] == 7.0f, "Plus equals failed: y != 7");
	THROW_IF_FALSE(vector1[2] == 9.0f, "Plus equals failed: z != 9");
}

void TEST_MinusEquals()
{
	Vector3 vector1(5.0f, 7.0f, 9.0f);
	const Vector3 vector2(4.0f, 5.0f, 6.0f);
	vector1 -= vector2;
	THROW_IF_FALSE(vector1[0] == 1.0f, "Minus equals failed: x != 1");
	THROW_IF_FALSE(vector1[1] == 2.0f, "Minus equals failed: y != 2");
	THROW_IF_FALSE(vector1[2] == 3.0f, "Minus equals failed: z != 3");
}

void TEST_TimesEquals()
{
	Vector3 vector(1.0f, 2.0f, 3.0f);
	vector *= 2.0f;
	THROW_IF_FALSE(vector[0] == 2.0f, "Times equals failed: x != 2");
	THROW_IF_FALSE(vector[1] == 4.0f, "Times equals failed: y != 4");
	THROW_IF_FALSE(vector[2] == 6.0f, "Times equals failed: z != 6");
}

void TEST_DivideEquals()
{
	Vector3 vector(2.0f, 4.0f, 6.0f);
	vector /= 2.0f;
	THROW_IF_FALSE(vector[0] == 1.0f, "Divide equals failed: x != 1");
	THROW_IF_FALSE(vector[1] == 2.0f, "Divide equals failed: y != 2");
	THROW_IF_FALSE(vector[2] == 3.0f, "Divide equals failed: z != 3");
}

void TEST_EqualsOperator()
{
	const Vector3 vector1(1.0f, 2.0f, 3.0f);
	const Vector3 vector2(1.0f, 2.0f, 3.0f);
	const Vector3 vector3(4.0f, 5.0f, 6.0f);

	THROW_IF_FALSE(vector1 == vector2, "Equals operator failed: vectors are not equal");
	THROW_IF_FALSE(vector1 != vector3, "Equals operator failed: vectors are equal");
}

int main(int argc, char** argv)
{
	std::cout << "Running Vector3 Tests...\n";
	TEST_DefaultConstructor();
	TEST_ParameterizedConstructor();
	TEST_InitializeListConstructor();
	TEST_Addition();
	TEST_Subtraction();
	TEST_ScalarMultiplication();
	TEST_DotProduct();
	TEST_CrossProduct();
	TEST_PlusEquals();
	TEST_MinusEquals();
	TEST_TimesEquals();
	TEST_DivideEquals();
	TEST_EqualsOperator();
	std::cout << "All tests passed!\n";
	return 0;
}
