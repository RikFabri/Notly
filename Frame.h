#pragma once


struct RenderArea
{
	int x, y;
	int width, height;
};

class IFrame
{
public:
	virtual void Draw(const RenderArea& renderArea) = 0;

};

