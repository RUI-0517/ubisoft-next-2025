#pragma once
#include "Vector_Impl.h"

/**
 * \brief This class provides a syntactic sugar layer by integrating common vector operations.
 * \tparam TSelf The type of the derived vector class using this mixin.
 * \tparam TValue The type of the elements stored in the vector.
 * \note Example Usage: \code struct Vector<3, T> : SyntacticSugar<Vector<3, T>, T> \endcode
*/
template <typename TSelf, typename TValue>
struct SyntacticSugar
{
	[[nodiscard]] TValue dot(const TSelf& other) const
	{
		return ::dot(static_cast<const TSelf&>(*this), other);
	}

	[[nodiscard]] TValue magnitude() const
	{
		return ::magnitude(static_cast<const TSelf&>(*this));
	}

	[[nodiscard]] TValue magnitudeSquared() const
	{
		return ::magnitudeSquared(static_cast<const TSelf&>(*this));
	}

	[[nodiscard]] TSelf normalize() const
	{
		return ::normalize(static_cast<const TSelf&>(*this));
	}

	[[nodiscard]] TValue distance(const TSelf& other) const
	{
		return ::distance(static_cast<const TSelf&>(*this), other);
	}

	[[nodiscard]] TValue distanceSquared(const TSelf& other) const
	{
		return ::distanceSquared(static_cast<const TSelf&>(*this), other);
	}

	[[nodiscard]] TSelf lerp(const TSelf& other, float t) const
	{
		return ::lerp(static_cast<const TSelf&>(*this), other, t);
	}

	[[nodiscard]] TSelf hadamard(const TSelf& other) const
	{
		return ::hadamard(static_cast<const TSelf&>(*this), other);
	}

	template <typename = std::enable_if_t<std::is_floating_point_v<TValue>>>
	[[nodiscard]] TSelf pow(const TValue exponent) const
	{
		return ::pow(static_cast<const TSelf&>(*this), exponent);
	}

	[[nodiscard]] static TSelf tripleProduct(const TSelf& a, const TSelf& b, const TSelf& c)
	{
		return a.dot(c) * b - a.dot(b) * c;
	}

private:
	SyntacticSugar() = default;
	SyntacticSugar(const SyntacticSugar&) = default;
	SyntacticSugar(SyntacticSugar&&) noexcept = default;
	SyntacticSugar& operator=(const SyntacticSugar&) = default;
	SyntacticSugar& operator=(SyntacticSugar&&) noexcept = default;
	~SyntacticSugar() = default;

	friend TSelf;
};
