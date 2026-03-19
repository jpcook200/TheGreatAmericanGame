#pragma once

/**
 * @class Vec2
 * @brief A lightweight 2D vector class for coordinate and physics math.
 */
class Vec2
{
public:

	float x = 0;
	float y = 0;

	Vec2();

	/**
	 * @brief Constructs a vector with specific coordinates.
	 * @param xin The initial X-coordinate.
	 * @param yin The initial Y-coordinate.
	 */
	Vec2(float xin, float yin);

	// --- Comparison Operators ---

	/** @param rhs The vector to compare against. */
	bool operator == (const Vec2& rhs) const;
	bool operator != (const Vec2& rhs) const;

	// --- Vector Arithmetic ---

	/** @brief Adds two vectors. Useful for position + velocity. */
	Vec2 operator + (const Vec2& rhs) const;

	/** @brief Subtracts rhs from this vector. Useful for finding a direction towards a target. */
	Vec2 operator - (const Vec2& rhs) const;

	/** @brief Divides both components by a scalar value. */
	Vec2 operator / (const float val) const;

	/** @brief Multiplies both components by a scalar value. Essential for velocity * dt. */
	Vec2 operator * (const float val) const;

	// --- Compound Assignment ---

	void operator += (const Vec2& rhs);
	void operator -= (const Vec2& rhs);
	void operator *= (const float val);
	void operator /= (const float val);

	// --- Math Helpers ---

	/**
	 * @brief Calculates the Euclidean distance to another point.
	 * @param rhs The destination point.
	 * @return The float distance between the two vectors.
	 */
	float dist(const Vec2& rhs) const;

	/**
	 * @brief Returns the magnitude (length) of the vector.
	 * @return The length calculated via Pythagorean theorem.
	 */
	float length() const;

	/**
	 * @brief Returns a unit vector pointing in the same direction.
	 * @return A new Vec2 with a length of 1.0.
	 */
	Vec2 normalized() const;
};