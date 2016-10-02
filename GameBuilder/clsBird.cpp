#include "stdafx.h"
#include "clsBird.h"


clsBird::clsBird(void)
{
	face = rand() % 4;
	wait = 0;
	stop = false; 
}


clsBird::~clsBird(void)
{
}

void clsBird::draw(LPD3DXSPRITE spt, float x, float y)
{
	RECT frame;
	SetRect(&frame, face*16,animPos*16,(face*16)+16,(animPos*16)+16);

	D3DXVECTOR3 position(xPos + x,yPos + y,0);
	spt->Draw(sprite,&frame,NULL,&position,D3DCOLOR_ARGB(255,255,255,255));

	curX = x;
	curY = y;

	frameRate++;

	if(frameRate > 3)
	{
		frameRate = 0;
		animPos++;
		if(animPos > 5)
			animPos = 0;
	}


}

void clsBird::SetPic(LPDIRECT3DDEVICE9 ldspt)
{
	D3DXCreateTextureFromFileEx(ldspt,
		L"Images/bird.png",
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

void clsBird::move()
{
	if(!stop)
	{
		if(face == 1)
			xPos++;
		else if(face == 3)
			xPos--;
		else if(face == 0)
			yPos++;
		else
			yPos--;
	}
	else
	{
		wait++;
		if(wait > 2)
		{
			stop = false; 			
			wait = 0;
			face = rand() % 4;
		}
	}
}

bool clsBird::WallCol(clsParent* col)
{
	float ox = col->GetCX();
	float oy = col->GetCY();
	float ow = col->GetW();
	float oh = col->GetH();
	float x = xPos + curX;
	float y = yPos + curY;

	if(!((x + width <= ox) || (x >= ox + ow) || (y + height <= oy) || (y >= oy + oh)))
	{ 
		if(face == 1)
		{
			xPos = col->GetX()-16;
			//yPos = col->GetY();
		}
		else if(face == 3)
		{
			xPos = col->GetX()+16;
			//yPos = col->GetY();
		}
		else if(face == 0)
		{
			//xPos = col->GetX();
			yPos = col->GetY()-16;
		}
		else
		{
			//xPos = col->GetX();
			yPos = col->GetY()+16;
		}	
		stop = true;
		return true; 
	}

	return false; 
}

