#pragma once
#include "clsparent.h"
class clsWall :
	public clsParent
{
public:
	clsWall(void);
	~clsWall(void);

	void draw(LPD3DXSPRITE spt, float x, float y);
	void SetPic(LPDIRECT3DDEVICE9 ldspt);
	
	bool Damage();

protected:
	int wallType;

};

