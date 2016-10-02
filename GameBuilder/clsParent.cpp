#include "stdafx.h"
#include "clsParent.h"


clsParent::clsParent(void)
{
	xPos = 0;
	yPos = 0;
	width = 0;
	height = 0;
	sprite = NULL; 
	animPos = 0;
	frameRate = 0;
	numFrames = 0;
	curX = 0;
	curY = 0;
	strtX = 0;
	strtY = 0;
	bR = 255;
	bG = 255;
	bB = 255;
	trans = 255; 

	dead = false; 

}


clsParent::~clsParent(void)
{
	if(sprite != NULL)
		sprite->Release();
}

void clsParent::draw(LPD3DXSPRITE spt)
{
	D3DXVECTOR3 position(xPos,yPos,0);
	spt->Draw(sprite,NULL,NULL,&position,D3DCOLOR_ARGB(255,255,255,255));
}

void clsParent::draw(LPD3DXSPRITE spt, float x, float y)
{
	D3DXVECTOR3 position(xPos,yPos,0);
	spt->Draw(sprite,NULL,NULL,&position,D3DCOLOR_ARGB(255,255,255,255));

	curX = x;
	curY = y;
}

void clsParent::SetPic(LPDIRECT3DDEVICE9 ldspt)
{
	//POLYMORPH
}

void clsParent::SetPic(LPDIRECT3DDEVICE9 ldspt,LPCWSTR pic)
{
	D3DXCreateTextureFromFileEx(ldspt,
		pic,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		NULL,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(255,0,255),
		NULL,
		NULL,
		&sprite);
}

void clsParent::SetLocal(float x, float y, float w, float h)
{
	xPos = x;
	yPos = y;
	width = w; 
	height = h; 
}

void clsParent::SetColor(int r, int g, int b)
{
	bR = r;
	bG = g;
	bB = b;
}

void clsParent::SetStrt(float x, float y)
{
	strtX = x;
	strtY = y;
}

float clsParent::GetX()
{
	return xPos;
}

float clsParent::GetY()
{
	return yPos;
}

float clsParent::GetW()
{
	return width;
}

float clsParent::GetH()
{
	return height;
}

float clsParent::GetCX()
{
	return xPos + curX;
}

float clsParent::GetCY()
{
	return yPos + curY;
}

float clsParent::GetStrtX()
{
	return strtX;
}
float clsParent::GetStrtY()
{
	return strtY;
}

void clsParent::move()
{
}

bool clsParent::move(int f)
{
	return false; 
}

bool clsParent::WallCol(clsParent* col)
{	
	return false; 
}


bool clsParent::Damage()
{
	return false; 
}