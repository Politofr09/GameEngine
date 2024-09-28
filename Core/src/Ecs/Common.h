#pragma once


namespace Core::Ecs
{

	struct PositionComponent
	{
		float x;
		float y;
		float z;
	};

	typedef PositionComponent RotationComponent;
	typedef PositionComponent ScaleComponent;
	typedef PositionComponent VelocityComponent;

	struct TransformComponent
	{
		PositionComponent Position;
		RotationComponent Rotation;
		ScaleComponent Scale;
	};

	struct ColorComponent
	{
		float r;
		float g;
		float b;
		float a;
	};

	// TODO: Add more of them! Fill this file with components!!!

}