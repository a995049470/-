#include "Box2D\Box2D.h"
#include <iostream>
#include <easyx.h>
#include <conio.h>
#include <stdio.h>
#include <ctime>
#include <math.h>
#include <vector>
#include <math.h>
#include "PerlinNoise.h"

using namespace std;

#define DEBUG(x) cout << x << endl
#define WIDTH 640
#define HEIGHT 480
#define COE 10
#define DRAW true
#define SIDE 40
#define Random(x, y) (x + rand() % ( y - x ))
#define PI 3.14159265f


struct Action
{
private:
	vector<void(*)()> v;
public:
	Action(void(*_p)() = NULL)
	{
		if (_p == NULL)
		{
			return;
		}
		v.push_back(_p);
	}
	void Invoke()
	{
		for (auto i = v.begin(); i != v.end(); ++i)
		{
			(*(*i))();
		}
	}
	void Add(void(*_p)())
	{
		v.push_back(_p);
	}
	void Remove(void(*_p)())
	{
		for (auto i = v.begin(); i != v.end(); ++i)
		{
			if (*i == _p)
			{
				v.erase(i);
				break;
			}
		}
	}
};

template<typename T>
struct Action1
{
private:
	vector<void(*)(T)> v;
public:
	Action1(void(*_p)(T) = NULL)
	{
		if (_p == NULL)
		{
			return;
		}
		v.push_back(_p);
	}
	void Invoke(T arg1)
	{
		for (auto i = v.begin(); i != v.end(); ++i)
		{
			(*(*i))(arg1);
		}
	}
	void Add(void(*_p)(T))
	{
		v.push_back(_p);
	}
	void Remove(void(*_p)(T))
	{
		for (auto i = v.begin(); i != v.end(); ++i)
		{
			if (*i == _p)
			{
				v.erase(i);
				break;
			}
		}
	}

};
template<typename T1, typename T2>
struct Action2
{
private:
	vector<void(*)(T1, T2)> v;
public:	

	Action2(void(*_p)(T1, T2))
	{
		//p = _p;
		v.push_back(_p);
	}
	void Invoke(T1 arg1, T2 arg2)
	{
		for (auto i = v.begin(); i != v.end(); ++i)
		{
			(*(*i))(arg1, arg2);
		}
	}
	void Add(void(*_p)(T1, T2))
	{
		v.push_back(_p);
	}
	void Remove(void(*_p)(T1, T2))
	{
		for (auto i = v.begin(); i != v.end(); ++i)
		{
			if (*i == _p)
			{
				v.erase(i);
				break;
			}
		}
	}

};

b2Vec2 Rotate(b2Vec2 v, float ag)
{
	float x = v.x * cosf(ag) - v.y * sinf(ag);
	float y = v.x * sinf(ag) + v.y * cosf(ag);
	return b2Vec2(x, y);
}

b2Vec2 BoxPosToEasyXPos(b2Vec2 v)
{
	float x = v.x * COE;
	float y = HEIGHT - v.y * COE;
	return b2Vec2(x, y);
}

b2Vec2 EasyXPosToBoxPos(b2Vec2 v)
{
	float x = v.x / COE;
	float y = (HEIGHT - v.y) / COE;
	return b2Vec2(x, y);
}


class DrawUnit
{
private:
	b2Body *body;
	vector<float> shapInfo;
	//0是方形 1是线形 
	int shape;
	vector<b2Vec2> points;
	void (DrawUnit::* draw)();
public:
	DrawUnit(b2Body* _body,  float *f, const int size)
	{
		body = _body;
		
		for (int i = 0; i < size; i++)
		{
			shapInfo.push_back(f[i]);
		}
		shape = 0;
		draw = &DrawUnit::DrawBox;	
	}

	DrawUnit(b2Body* _body, b2Vec2 *v, const int size)
	{
		body = _body;
		for (size_t i = 0; i < size; i++)
		{
			points.push_back(v[i]);
		}
		shape = 1;
		draw = &DrawUnit::DrawLine;
	}

	~DrawUnit()
	{
	}
	
	void Draw()
	{
		if(draw != NULL) (this->*draw)();
	}
	void DrawLine()
	{
		if (body == NULL || points.size() == 0)
		{
			return;
		}
		auto center = body->GetPosition();
		for (size_t i = 0; i < points.size(); i++)
		{
			auto p = points[i] + center;
			int x = p.x * COE;
			int y = HEIGHT - p.y * COE;
			if (i == 0)
			{
				if(DRAW) moveto(x, y);
			}
			if(DRAW) lineto(x, y);
		}

	}
	b2Vec2 ToScreenPos(b2Vec2 v, float ag, float x, float y)
	{
		auto r = Rotate(v, ag);
		r.x += x;
		r.y += y;
		r = BoxPosToEasyXPos(r);
		return r;
	}
	void DrawBox()
	{
		if (body == NULL || shapInfo.size() == 0)
		{
			return;
		}
		auto ag = body->GetAngle();
		
		auto center = body->GetPosition();
		float x = center.x;
		float y = center.y;
		float tr = shapInfo[0];
		float tl = -shapInfo[0];
		float tu = shapInfo[1];
		float td = -shapInfo[1];
		b2Vec2 t1 = b2Vec2(tr, tu);
		b2Vec2 t2 = b2Vec2(tr, td);
		b2Vec2 t3 = b2Vec2(tl, td);
		b2Vec2 t4 = b2Vec2(tl, tu);
		t1 = ToScreenPos(t1, ag, x, y);
		t2 = ToScreenPos(t2, ag, x, y);
		t3 = ToScreenPos(t3, ag, x, y);
		t4 = ToScreenPos(t4, ag, x, y);
		POINT pts[] =
		{
			{ t1.x, t1.y },
			{ t2.x, t2.y },
			{ t3.x, t3.y },
			{ t4.x, t4.y },
		};
		if(draw) fillpolygon(pts, 4);
		//up = HEIGHT - up;
		//down = HEIGHT - down;

		//if (DRAW) solidrectangle(left, up, right, down);
		//DEBUG(shapInfo[0] << "  " << shapInfo[1]);
		//DEBUG(left << "  " << up << "  " << right << "  " << down);
	}

};

void Draw(vector<DrawUnit> &units)
{
	
	if (DRAW)
	{
		cleardevice(); 
		BeginBatchDraw();
	}
	for (auto i = units.begin(); i != units.end() ; ++i)
	{
		i->Draw();
	}
	if (DRAW) EndBatchDraw();
	
}



vector<DrawUnit> units;
b2Vec2 gravity(0.0f, -10.0f);
b2World world = b2World(gravity);

b2Body* CreateCube(float* info)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(info[0], info[1]);
	b2Body* body = world.CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(info[2], info[3]);
	float f[2] = { info[2], info[3] };
	DrawUnit u2 = DrawUnit(body, f, 2);
	units.push_back(u2);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	body->CreateFixture(&fixtureDef);
	return body;
}


b2Vec2 field[ WIDTH / SIDE][ HEIGHT / SIDE];


void SetField()
{
	PerlinNoise p(Random(0, 100));
	for (int i = 1; i < WIDTH / SIDE; i++)
	{
		for (int j = 1; j < HEIGHT / SIDE; j++)
		{
			float x = i / 10.0f;
			float y = j / 10.0f;
			float ag = p.noise(x, y, 0) * 4 * 3.14f;
			auto dir = b2Vec2(sinf(ag), cos(ag));
			field[i][j] = dir;
			//b2Vec2 center(i * SIDE, j * SIDE);
			//b2Vec2 end = center + 10 * dir;
			//b2Vec2 start = center - 10 * dir;
			//line(start.x, start.y, end.x, end.y);
			//solidcircle(end.x, end.y, 2);
		}
	}
}



int main()
{
	srand(time(0));
	if (DRAW)
	{
		initgraph(WIDTH, HEIGHT);
		/*setaspectratio(1, -1);
		setorigin(0, HEIGHT);*/
	}
	SetField();
	units.reserve(10);
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(WIDTH / 2 / COE, 2.0f);
	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	b2ChainShape shape;
	b2Vec2 ps[2] = { b2Vec2(WIDTH / 2 / COE, 2.0f), b2Vec2(-WIDTH / 2 / COE, 10.0f) };
	units.push_back(DrawUnit(groundBody, ps, 2));
	shape.CreateChain(ps, 2);
	groundBody->CreateFixture(&shape, 0.0f);

	float info1[] = { WIDTH / 2 / COE, 30.0f, 1.0f, 1.0f };
	auto b = CreateCube(info1) ;
	b->SetAngularVelocity(-10);
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	for (size_t i = 0; i < 600; i++)
	{
		world.Step(timeStep, velocityIterations, positionIterations);
		Draw(units);
		Sleep(timeStep * 1000);	
	}
	
	closegraph;
	while (true)
	{

	}
}

//int main()
//{
//	if (DRAW)
//	{
//		initgraph(WIDTH, HEIGHT);
//		setaspectratio(COE, -COE);
//		setorigin(0, HEIGHT);
//	}
//}
//




