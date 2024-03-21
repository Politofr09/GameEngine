#pragma once

struct Vector2
{
	float x;
	float y;
};

struct Vector3
{
	float x;
	float y;
	float z;
};

struct Vector4
{
	float x;
	float y;
	float z;
	float w;
};

struct Color
{
	unsigned int r;
	unsigned int g;
	unsigned int b;
	unsigned int a;
};

struct Position
{
	Vector3 position;
	Vector4 rotation;
	Vector3 scale;
};

struct Square
{
	Vector2 position;
	Vector2 size;
	Color color;
};

struct HealthBar
{
	float value;
	float maxValue;
};