#include "stdafx.h"
#include "clsPlayer.h"


clsPlayer::clsPlayer(void)
{
	facing = 0;
	pause = 0;
	running = false; 
	colide = false; 
	prevX = 0;
	prevY = 0;
}


clsPlayer::~clsPlayer(void)
{
}

void clsPlayer::draw(LPD3DXSPRITE spt, float x, float y)
{
	RECT frame;
	SetRect(&frame, facing*8,animPos*8,(facing*8)+8,(animPos*8)+8);

	D3DXVECTOR3 position(xPos,yPos,0);
	spt->Draw(sprite,&frame,NULL,&position,D3DCOLOR_ARGB(trans,bR,bG,bB));

	curX = x;
	curY = y;
	if(!running)
		animPos = 0;
	running = false; 
}

void clsPlayer::SetPic(LPDIRECT3DDEVICE9 ldspt)
{
	D3DXCreateTextureFromFileEx(ldspt,
		L"Images/player.png",
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
	
bool clsPlayer::Damage()
{
	return false;
}

bool clsPlayer::move(int f)
{
	running = true; 
	facing = f;
	pause++;
	if(pause > 4)
	{		
		animPos++;
	
		if(animPos > 4)
			animPos = 1;
		pause = 0;
	}

	prevX = xPos;
	prevY = yPos;

	//y 160, 112
	//x 288, 192
	if(f == 0)
	{
		if(yPos < 64)
			yPos++;
	}
	else if(f == 1)
	{
		if(xPos < 128)
			xPos++;	
	}
	else if(f == 2)
	{
		if(xPos > 128)
			xPos--;
	}
	else if(f == 3)
	{
		if(yPos > 64)
			yPos--;	
	}
	
	if(colide)
	{
		colide = false;
		return false; 
	}

	return true; 

}

bool clsPlayer::WallColl(clsParent* col)
{
	float ox = col->GetCX();
	float oy = col->GetCY();
	float ow = col->GetW();
	float oh = col->GetH();
	int x = xPos;
	int y = yPos; 

	if(!((x + width < ox) || (x > ox + ow) || (y + height < oy) || (y > oy + oh)))
	{ 
		xPos = prevX;
		yPos = prevY;
		colide = true; 
	}

	return false; 
}


