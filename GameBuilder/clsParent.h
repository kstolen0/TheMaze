#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <stdlib.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

class clsParent
{
public:
	clsParent(void);
	~clsParent(void);

	virtual void draw(LPD3DXSPRITE spt);
	virtual void draw(LPD3DXSPRITE spt, float x, float y);

	virtual void SetPic(LPDIRECT3DDEVICE9 ldspt);
	virtual void SetPic(LPDIRECT3DDEVICE9 ldspt, LPCWSTR pic);

	void SetLocal(float x, float y, float w, float h);
	void SetColor(int r, int g, int b);

	float GetX();
	float GetY();
	float GetW();
	float GetH();
	float GetCX();
	float GetCY();
	float GetStrtX();
	float GetStrtY();

	void SetStrt(float x, float y);

	clsParent* pNext;

	virtual void move();
	virtual bool move(int f);
	virtual bool WallCol(clsParent* col);
	virtual bool Damage();

protected:
	LPDIRECT3DTEXTURE9 sprite;
	float xPos, yPos, width, height;
	float strtX, strtY;
	int animPos, frameRate, numFrames;
	bool dead; 

	int trans, bR, bG, bB; 
	float curX,curY;

};

