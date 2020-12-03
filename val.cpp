#include <map>
#include "val.h"

// add this with op 
Value Value::operator+(const Value& op) const {
	if (GetType() == op.GetType())
	{
		if (IsInt()) return Value(I + op.GetInt());
		if (IsReal()) return Value(R + op.GetReal());
	}
	else if (IsInt() && op.IsReal())
	{

		return Value((float)GetInt() + op.GetReal());
	}
	else if (IsReal() && op.IsInt())
	{
		return Value(GetReal() + (float)op.GetInt());
	}
	else
	{
		return Value();
	}
}

// subtract op from this
Value Value::operator-(const Value& op) const {
	if (GetType() == op.GetType())
	{
		if (IsInt()) return Value(I - op.GetInt());
		if (IsReal()) return Value(R - op.GetReal());
	}
	else if (IsInt() && op.IsReal())
	{

		return Value((float)GetInt() - op.GetReal());
	}
	else if (IsReal() && op.IsInt())
	{
		return Value(GetReal() - (float)op.GetInt());
	}
	else
	{
		return Value();
	}
}

// multiply this with op
Value Value::operator*(const Value& op) const {
	if (GetType() == op.GetType())
	{
		if (IsInt()) return Value(I * op.GetInt());
		if (IsReal()) return Value(R * op.GetReal());
	}
	else if (IsInt() && op.IsReal())
	{

		return Value((float)GetInt() * op.GetReal());
	}
	else if (IsReal() && op.IsInt())
	{
		return Value(GetReal() * (float)op.GetInt());
	}
	else
	{
		return Value();
	}
}

// divide this by op
Value Value::operator/(const Value& op) const {
	if (GetType() == op.GetType())
	{
		if (IsInt()) return Value(I / op.GetInt());
		if (IsReal()) return Value(R / op.GetReal());
	}
	else if (IsInt() && op.IsReal())
	{

		return Value((float)GetInt() / op.GetReal());
	}
	else if (IsReal() && op.IsInt())
	{
		return Value(GetReal() / ((float)op.GetInt()));
	}
	else
	{
		return Value();
	}
}
