#include "Rect.h"
#include <algorithm> // until c++11 for std::swap
#include <utility> // since c++11 for std::swap

namespace Theodore {
	Rect::Rect() :left(0.f), right(0.f), top(0.f), bottom(0.f) {

	}

	Rect::Rect(const float _left, const float _top, const float _right, const float _bottom) {
		left = _left;
		top = _top;
		right = _right;
		bottom = _bottom;
	}

	Rect::Rect(const Rect &other) {
		left = other.left;
		top = other.top;
		right = other.right;
		bottom = other.bottom;
	}

	Rect& Rect::operator=(Rect other) {
		// copy and swap idiom (effective c++ section 11)
		Swap(*this, other);
		return *this;
	}

	bool Rect::operator==(const Rect &other) const {
		if (left == other.left && right == other.right && top == other.top && bottom == other.bottom)
			return true;

		return false;
	}

	bool Rect::operator!=(const Rect &other) const {
		return !(*this == other);
	}

	Rect Rect::operator+(const Rect& other) const {
		// using op= (more effective c++ section 22)
		return Rect(*this) += other;
	}

	Rect& Rect::operator+=(const Rect& other) {
		left += other.left;
		top += other.top;
		right += other.right;
		bottom += other.bottom;

		return *this;
	}

	Rect Rect::operator-(const Rect& other) const {
		return Rect(*this) -= other;
	}

	Rect& Rect::operator-=(const Rect& other) {
		left -= other.left;
		top -= other.top;
		right -= other.right;
		bottom -= other.bottom;

		return *this;
	}

	Rect Rect::operator*(const Rect& other) const {
		return Rect(*this) *= other;
	}

	Rect& Rect::operator*=(const Rect& other) {
		left *= other.left;
		top *= other.top;
		right *= other.right;
		bottom *= other.bottom;

		return *this;
	}

	Rect Rect::operator/(const Rect& other) const {
		return Rect(*this) /= other;
	}

	Rect& Rect::operator/=(const Rect& other) {
		left /= other.left;
		top /= other.top;
		right /= other.right;
		bottom /= other.bottom;

		return *this;
	}

	Rect& Rect::SetRect(const float _left, const float _top, const float _right, const float _bottom) {
		return *this = Rect(_left, _top, _right, _bottom);
	}

	void Rect::Swap(Rect& first, Rect& second) noexcept {
		using std::swap;
		swap(first.left, second.left);
		swap(first.top, second.top);
		swap(first.right, second.right);
		swap(first.bottom, second.bottom);
	}

	const Rect Rect::zero = Rect(0.f, 0.f, 0.f, 0.f);
	const Rect Rect::one = Rect(1.f, 1.f, 1.f, 1.f);
}