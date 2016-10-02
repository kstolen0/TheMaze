#pragma once
#include "clsparent.h"
class clsBird :
	public clsParent
{
public:
	clsBird(void);
	~clsBird(void);

	void draw(LPD3DXSPRITE spt, float x, float y);
	void SetPic(LPDIRECT3DDEVICE9 ldspt);

	void move();
	bool WallCol(clsParent* col);


protected:
	int face, wait;
	bool stop;

};

