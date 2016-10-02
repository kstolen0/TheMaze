#include "stdafx.h"
#include "clsWall.h"


clsWall::clsWall(void)
{
	trans = 255;
	bR = 255;
	bG = 255;
	bB = 255;
	wallType = rand() % 3;
}


clsWall::~clsWall(void)
{
}

void clsWall::draw(LPD3DXSPRITE spt, float x, float y)
{
	RECT frame;
	SetRect(&frame, animPos*16,wallType*16,(animPos*16)+16,(wallType*16)+16);

	D3DXVECTOR3 position(xPos + x,yPos + y,0);
	spt->Draw(sprite,&frame,NULL,&position,D3DCOLOR_ARGB(trans,bR,bG,bB));

	curX = x;
	curY = y;

}

void clsWall::SetPic(LPDIRECT3DDEVICE9 ldspt)
{
	D3DXCreateTextureFromFileEx(ldspt,
		L"Images/Wall-Grey2.png",
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

bool clsWall::Damage()
{
	animPos++;
	if(animPos > 3)
	{
		animPos--;
		return true;
	}
	return false; 
}