#include <iostream>
#include <easyx.h>
#include <conio.h>
#include <stdio.h>
#include <ctime>
#include <math.h>
#include "PerlinNoise.h"

using namespace std;
#define DEBUG(x) cout << x << endl
#define WIDTH 640
#define HEIGHT 480
#define Random(x, y) (x + rand() % ( y - x ))
#define PI 3.14159265f

void DrawSpiral()
{
	float r = 0;
	float a = 0;
	int i = 0;
	int count = 2000;
	while (true)
	{
		i++;
		int t = 1;
		r += 0.05f * t;
		a += PI / 300.0f * t;
		int x = r * sin(a) + WIDTH / 2;
		int y = r * cos(a) + HEIGHT / 2;
		if (t == 1)
		{
			setfillcolor(WHITE);
		}
		else
		{
			setfillcolor(BLACK);
		}
		solidcircle(x, y, 3);
		Sleep(1);
	}
}

void TestMouse()
{
	MOUSEMSG msg;
	int lastX = 0;
	int lastY = 0;
	bool bIsDown = false;
	while (true)
	{
		if(MouseHit())
		{
			msg = GetMouseMsg();
			if (msg.mkLButton && !bIsDown)
			{
				lastX = msg.x;
				lastY = msg.y;
				bIsDown = true;
			}
			if (!msg.mkLButton && bIsDown)
			{
				line(lastX, lastY, msg.x, msg.y);
				bIsDown = false;
			}
			DEBUG(msg.uMsg);			
		}		
		Sleep(1);
	}
	while (true)
	{

	}
}

void DrawScreen()
{
	PerlinNoise p = PerlinNoise();
	float offx = 0.01f;
	float offy = 0.01f;
	float startX = 0.0f;
	float startY = 0.0f;
	while (true)
	{
		offx += Random(0, 10) / 1000.0f;
		offy += Random(0, 10) / 1000.0f;
		for (size_t i = 0; i < WIDTH; i++)
		{
			for (size_t j = 0; j < HEIGHT; j++)
			{
				float x = startX + offx * i;
				float y = startY + offy * j;
				int c = p.noise(x, y, 0) * 255.0f;

				auto color = RGB(c, c, c);
				putpixel(i, j, color);

			}
			Sleep(0);
		}
		Sleep(1000);
	}

	while (true)
	{

	}
}

void DrawNoiseLine()
{
	PerlinNoise p = PerlinNoise();
	int size = 6000;
	float max = 40.0f;
	for (int i = 0; i < size; i++)
	{
		float x = i / (size / max);
		float y = HEIGHT / 2 + 100 - p.noise(x, 0, 0) * 200;
		int t = (WIDTH / (float)size) * i;
		solidcircle(t, (int)y, 0);
		Sleep(1);
	}
	while (true)
	{

	}
}



int MTRandom(int x, int y)
{
	int lerp = y - x;
	int res;
	while (true)
	{
		res = Random(0, lerp);
		int t = Random(0, lerp);
		if (t <= res)
		{
			break;
		}
	}
	res += x;
	return res;
}

class RandomCounter
{
	int* p;
	int count;
	int len;
public :
	RandomCounter(int _count)
	{		
		p = new int[_count];
		count = _count;
		len = WIDTH / _count;
		for (size_t i = 0; i < _count; i++)
		{
			p[i] = 0;
		}
	}
	void Step()
	{
		int i = MTRandom(0, count);
		p[i] = p[i] + 1;
		solidrectangle(i * len, HEIGHT - p[i] / 200, (i + 1) * len - 5, HEIGHT);
		
	}
};



void CounterRun(int i)
{
	RandomCounter rc = RandomCounter(i);
	while (true)
	{
		rc.Step();
		Sleep(0);
	}
}

class Walker
{
	int x;
	int y;
public :
	Walker()
	{
		x = WIDTH / 2;
		y = HEIGHT / 2;
	}
	void step_1()
	{
		int i = Random(0, 3) - 1;
		int j = Random(0, 3) - 1;
		x += i;
		y += j;
	}
	void step()
	{
		int i = Random(0, 4);
		if (i == 0)
		{
			x++;
		}
		else if (i == 1)
		{
			x--;
		}
		else if (i == 2)
		{
			y++;
		}
		else if (i == 3)
		{
			y--;
		}
	}
	void Draw()
	{
		solidcircle(x, y, 2);
	}
};

void WalkRun()
{
	Walker w = Walker();
	while (true)
	{
		w.step_1();
		w.Draw();
		Sleep(1);
	}
}

float SND(float x, int max, float u = 0, float i = 1)
{
	float y;
	float t = -pow(x - u, 2) / (2 * pow(i, 2));
	y = 1 / (sqrtf(2 * PI) * i) * exp(t);
	y = max * exp(t);
	return y;
}

void ClearScreen()
{
	auto color = getfillcolor();
	setfillcolor(BLACK);
	solidrectangle(0, HEIGHT, WIDTH, 0);
	setfillcolor(color);
}

void DrawSND(int count)
{
	int len = WIDTH / count;

	float t = 1;
	float* p1 = new float[count];
	float* p2 = new float[count];
	while (true)
	{		
		//ClearScreen();
		
		for (int i = 0; i < count; i++)
		{
			float x = (i - count / 2) / 100.0f;
			float y = SND(x, HEIGHT / 2, 0, t);
			p1[i] = x;
			p2[i] = y;
			auto color = getfillcolor();
			setfillcolor(BLACK);
			solidrectangle(i * len, 0, (i + 1) * len, HEIGHT);
			setfillcolor(color);
			solidrectangle(i * len, HEIGHT - y, (i + 1) * len, HEIGHT);
		}
		Sleep(100);		
		t += 0.01f;
	}
}
void Paint(int count, int r)
{
	for (int i = 0; i < count; i++)
	{
		float f = Random(-2000, 2000) / 500.0f;
		float t = 5000;
		t =  SND(f, t, 0.5f);
		float ag = Random(0, 361) / 2 / PI;
		int x = t * sin(ag) + 300;
		int y = t * cos(ag) + 300;
		auto color = RGB(Random(0, 256), Random(0, 256), Random(0, 256));
		setfillcolor(color);
		solidcircle(x, y, r);
		Sleep(2);
	}
}



int main()
{
	srand(time(0));
	auto h1 = initgraph(640, 480);	// 创建绘图窗口，大小为 640x480 像素
	setfillcolor(WHITE);

//	DrawNoiseLine();
//	CounterRun(30);
//	DrawSND(WIDTH);
//	Paint(200000, 5);
//	DrawScreen();
//	TestMouse();
	DrawSpiral();
	closegraph();			// 关闭绘图窗口
}



