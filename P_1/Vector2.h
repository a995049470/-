#pragma once
struct Vector2
{
public:
	int x;
	int y;
	Vector2(int _x = 0, int _y = 0)
	{
		x = _x;
		y = _y;
	}
	Vector2 operator + (Vector2 r)
	{
		Vector2 res = Vector2();
		res.x = x + r.x;
		res.y = y + r.y;
		return res;
	}

	Vector2 operator - (Vector2 r)
	{
		Vector2 res = Vector2();
		res.x = x - r.x;
		res.y = y - r.y;
		return res;
	}


};


