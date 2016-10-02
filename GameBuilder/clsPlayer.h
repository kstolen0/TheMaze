#pragma once

#include "clsparent.h"

class clsPlayer :
	public clsParent
{
public:
	clsPlayer(void);
	~clsPlayer(void);

	void draw(LPD3DXSPRITE spt, float x, float y);
	void SetPic(LPDIRECT3DDEVICE9 ldspt);
	bool WallColl(clsParent* col);
	bool Damage();
	bool move(int f);

protected:
	int facing, pause; 
	int prevX, prevY;
	bool running,colide; 
};

