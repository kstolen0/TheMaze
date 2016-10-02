// jesusBaseball.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TheMaze.h"



// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"WindowClass";
    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(NULL,
                          L"WindowClass",
                          L"THE MAZE",
                          WS_OVERLAPPEDWINDOW, 
                          0, 45,
                          SCREEN_WIDTH, SCREEN_HEIGHT,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
    initD3D(hWnd);

	// initialise tranceperancies
	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	d3ddev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	d3ddev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

    // set up and initialize DirectInput
    initDInput(hInstance, hWnd);


	//		Setup sound
	initSound(); 

    MSG msg;


	//////////////////////////////////////////////////////////////////
	// initialise game object here////////////////////////////////////
	//////////////////////////////////////////////////////////////////

	pHead = NULL;
	pPlayer = NULL;
	screenX = 0;
	screenY = 0;
	left = false;
	right = false;
	down = false;
	up = false; 

	srand(time(NULL));

	for (int x = -32; x < 480; x += 16)
		for(int y = -32; y < 270; y += 16)
		{
			int rMax = rand() % 100;
			int rMake = rand() % 100;

			if(rMake < rMax)
				MakeWall(x,y,16,16);
			else if(rMake > 97)
				MakeBird(x,y,16,16);
			//else if(!pPlayer && x >= 64 && y >= 64)
				//MakePlayer(x+4,y+4,8,8);
		}
		MakePlayer(128,64,8,8);
	//x 192, y 112

	//	INIT SOUNDS


    // enter the main loop:
    while(TRUE)
    {
        DWORD starting_point = GetTickCount();

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

		
		//////////////////////////////////////////////////////
		//place main game logic here /////////////////////////
		//////////////////////////////////////////////////////
	
		CheckWalls();	
		CheckEnemy();
		AddWalls();
		EnemyLogic();		
		PlayerLogic();

		render_frame();
		detect_keys(hWnd);		
		

		while((GetTickCount() - starting_point) < 24); 
	}
	/////////////////////////////////
	//DELETE LISTS /////////////////
	///////////////////////////////

	clsParent* pTemp;
	clsParent* pDel;

	pTemp = pHead;

	while(pTemp != NULL)
	{
		pDel = pTemp;
		pTemp = pTemp->pNext;
		delete(pDel);
	}

	pTemp = pEnemy;

	while(pTemp != NULL)
	{
		pDel = pTemp;
		pTemp = pTemp->pNext;
		delete(pDel);
	}

	delete(pPlayer);

    // Clean up DirectInput
    cleanDInput();
    // clean up DirectX and COM
    cleanD3D();

    return msg.wParam;
}

// this is the function that detects keystrokes and displays them in a message box
void detect_keys(HWND hWnd)
{
    static BYTE keystate[256];    // create a static storage for the key-states
    dinkeyboard->Acquire();    // get access if we don't have it already
    dinkeyboard->GetDeviceState(256, (LPVOID)keystate);    // fill keystate with values
	

	if(keystate[DIK_S] & 0x80)
		if(down = pPlayer->move(0))
			screenY--;
	if(keystate[DIK_D] & 0x80)
		if(right = pPlayer->move(1))
			screenX--;
	if(keystate[DIK_A] & 0x80)
		if(left = pPlayer->move(2))
			screenX++;
	if(keystate[DIK_W] & 0x80)
		if(up = pPlayer->move(3))
			screenY++;

    if(keystate[DIK_ESCAPE] & 0x80)   // if the 'escape' key was pressed...
		PostMessage(hWnd, WM_DESTROY, 0, 0);   //delete the main window
    return;
}

// this is the function used to render a single frame
void render_frame(void)
{
    // clear the window
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(20, 20, 20), 1.0f, 0);
    d3ddev->BeginScene();    // begins the 3D scene
    d3dspt->Begin(NULL);    // begin sprite drawing


	////////////////////////////////////////////////////////
	//draw all game object here/////////////////////////////
	////////////////////////////////////////////////////////

	clsParent* pTemp;

	pTemp = pHead;

	while(pTemp)
	{
		pTemp->draw(d3dspt,screenX,screenY);
		pTemp = pTemp->pNext;
	}

	pTemp = pEnemy;

	while(pTemp)
	{
		pTemp->draw(d3dspt,screenX,screenY);
		pTemp = pTemp->pNext;
	}
	
	pPlayer->draw(d3dspt,screenX,screenY);

	///////////////////////////////////////////////////////
		///////////////		DRAW HUD		/////////////////
	/////////////////////////////////////////////////////


    d3dspt->End();    // end sprite drawing
    d3ddev->EndScene();    // ends the 3D scene
    d3ddev->Present(NULL, NULL, NULL, NULL);

    return;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}

// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
    d3ddev->Release();
    d3d->Release();
    return;
}

// this is the function that initializes DirectInput
void initDInput(HINSTANCE hInstance, HWND hWnd)
{
    // create the DirectInput interface
    DirectInput8Create(hInstance,    // the handle to the application
                       DIRECTINPUT_VERSION,    // the compatible version
                       IID_IDirectInput8,    // the DirectInput interface version
                       (void**)&din,    // the pointer to the interface
                       NULL);    // COM stuff, so we'll set it to NULL

    // create the keyboard device
    din->CreateDevice(GUID_SysKeyboard,    // the default keyboard ID being used
                      &dinkeyboard,    // the pointer to the device interface
                      NULL);    // COM stuff, so we'll set it to NULL    

    dinkeyboard->SetDataFormat(&c_dfDIKeyboard); // set the data format to keyboard format

    // set the control you will have over the keyboard
    dinkeyboard->SetCooperativeLevel(hWnd,
                                     DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

    return;    // return to WinMain()
}



// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    D3DPRESENT_PARAMETERS d3dpp;

    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = true;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = SCREEN_WIDTH;
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;

    // create a device class using this information and the info from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &d3ddev);

    D3DXCreateSprite(d3ddev, &d3dspt);    // create the Direct3D Sprite drawing object

	//initialise font object
    D3DXCreateFont(d3ddev,    // the D3D Device
                   12,    // font height of 30
                   0,    // default font width
                   FW_NORMAL,    // font weight
                   1,    // not using MipLevels
                   false,    // italic font
                   DEFAULT_CHARSET,    // default character set
                   OUT_DEFAULT_PRECIS,    // default OutputPrecision,
                   DEFAULT_QUALITY,    // default Quality
                   DEFAULT_PITCH | FF_DONTCARE,    // default pitch and family
                   L"Ariel",    // use Facename Arial
                   &dxfont);    // the font object

    return;
}

// this is the function that closes DirectInput
void cleanDInput(void)
{
    dinkeyboard->Unacquire();    // make sure the keyboard is unacquired
    din->Release();    // close DirectInput before exiting

    return;
}

//			SOUND FUNCTION

void initSound()
{
	CoInitializeEx( NULL, COINIT_MULTITHREADED );
	pXA2 = NULL;
	XAudio2Create( &pXA2, 0, XAUDIO2_DEFAULT_PROCESSOR );
	IXAudio2MasteringVoice* pMasterVoice = NULL;

	pXA2->CreateMasteringVoice( &pMasterVoice, XAUDIO2_DEFAULT_CHANNELS,
                            XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL );
}

void MakeWall(float x, float y, float w, float h)
{
	clsParent* pTemp;
	pTemp = new clsWall;
	pTemp->SetLocal(x,y,w,h);
	pTemp->SetStrt(screenX,screenY);
	pTemp->SetPic(d3ddev);
	int col = rand() % 80 + 176;
	pTemp->SetColor(col,col,col);
	pTemp->pNext = pHead;
	pHead = pTemp;
}

void MakeBird(float x, float y, float w, float h)
{
	clsParent* pTemp;
	pTemp = new clsBird;
	pTemp->SetLocal(x,y,w,h);
	pTemp->SetStrt(screenX,screenY);
	pTemp->SetPic(d3ddev);
	pTemp->pNext = pEnemy;
	pEnemy = pTemp;
}

void MakePlayer(float x, float y, float w, float h)
{
	pPlayer = new clsPlayer;
	pPlayer->SetLocal(x,y,w,h);
	pPlayer->SetStrt(screenX,screenY);
	pPlayer->SetPic(d3ddev);
}

void AddWalls()
{
	if(left && screenX % 16 == 0)
	{
		for(int i = 0; i < 282; i += 16)
		{
			int rMax = rand() % 100;
			int rMake = rand() % 100;

			if(rMake < rMax)
			{
				MakeWall(-16 - screenX,i - screenY,16,16);
				left = false; 
			}
			else if(rMake > 97)
				MakeBird(-16 - screenX, i - screenY, 16, 16);
		}
	}
	if(right && screenX % 16 == 0)
	{
		for(int i = 0; i < 282; i += 16)
		{
			int rMax = rand() % 100;
			int rMake = rand() % 100;

			if(rMake < rMax)
			{
				MakeWall(482 - screenX,i - screenY,16,16);
				right = false; 
			}
			else if(rMake > 97)
				MakeBird(482 - screenX, i - screenY, 16, 16);
		}
	}
	if(up && screenY % 16 == 0)
	{
		for(int i = 0; i < 482; i += 16)
		{
			int rMax = rand() % 100;
			int rMake = rand() % 100;

			if(rMake < rMax)
			{
				MakeWall(i - screenX,-16 - screenY,16,16);
				up = false; 
			}
			else if(rMake > 97)
				MakeBird(i - screenX, -16 - screenY, 16, 16);
				
		}
	}
	if(down && screenY % 16 == 0)
	{
		for(int i = 0; i < 482; i += 16)
		{
			int rMax = rand() % 100;
			int rMake = rand() % 100;

			if(rMake < rMax)
			{
				MakeWall(i - screenX,282 - screenY,16,16);
				down = false;
			}
			else if(rMake > 97)
				MakeBird(i - screenX, 282 - screenY, 16, 16);
		}
	}
}

void CheckWalls()
{
	clsParent* pTemp = pHead;

	while(pTemp != NULL)
	{
		if(pTemp->GetX() + screenX <= -32)
			pTemp = DeleteWall(pTemp);
		else if(pTemp->GetX() + screenX >= 495)		
			pTemp = DeleteWall(pTemp);			
		else if(pTemp->GetY() + screenY <= -32)			
			pTemp = DeleteWall(pTemp);	
		else if(pTemp->GetY() + screenY >= 297)			 
			pTemp = DeleteWall(pTemp);

		pTemp = pTemp->pNext;
	}
}

void CheckEnemy()
{
	clsParent* pTemp = pEnemy;

	while(pTemp != NULL)
	{
		if(pTemp->GetX() + screenX <= -32)
			pTemp = DeleteEnemy(pTemp);
		else if(pTemp->GetX() + screenX >= 495)		
			pTemp = DeleteEnemy(pTemp);			
		else if(pTemp->GetY() + screenY <= -32)			
			pTemp = DeleteEnemy(pTemp);	
		else if(pTemp->GetY() + screenY >= 297)			 
			pTemp = DeleteEnemy(pTemp);

		if(pTemp)
			pTemp = pTemp->pNext;
		else
			break;
	}
}

void EnemyLogic()
{
	clsBird* pEn = static_cast<clsBird*>(pEnemy);

	while(pEn)
	{
		clsParent* pTemp = pHead;
		while(pTemp)
		{
			if(pEn->WallCol(pTemp))
				if(pTemp->Damage())
				{
					DeleteWall(pTemp);
					break;
				}
			pTemp = pTemp->pNext;
		}
		pEn->move();
		pEn = static_cast<clsBird*>(pEn->pNext);
	}
	

}

void PlayerLogic()
{
	clsParent* pTemp = pHead;
	while(pTemp)
	{
		if(pPlayer->WallColl(pTemp))
			return;
		else
			pTemp = pTemp->pNext;
		
			
	}
}

clsParent* DeleteWall(clsParent* pWall)
{
	clsParent* pDel;

	if(pHead == pWall)
	{
		pDel = pHead;
		pHead = pHead->pNext;
		delete(pDel);
		return pHead;
	}
	else
	{
		clsParent* pPrev = pHead;
		clsParent* pTemp = pHead->pNext;

		while(pTemp)
		{
			if(pTemp == pWall)
			{
				pDel = pTemp;
				pPrev->pNext = pTemp->pNext;
				pTemp = pPrev->pNext;
				delete(pDel);
				return pPrev;
			}
			else
			{
				pPrev = pTemp;
				pTemp = pTemp->pNext;
			}
		}
	}
}

clsParent* DeleteEnemy(clsParent* pEn)
{
	clsParent* pDel;

	if(pEnemy == pEn)
	{
		pDel = pEnemy;
		pEnemy = pEnemy->pNext;
		delete(pDel);
		return pEnemy;
	}
	else
	{
		clsParent* pPrev = pEnemy;
		clsParent* pTemp = pEnemy->pNext;

		while(pTemp)
		{
			if(pTemp == pEn)
			{
				pDel = pTemp;
				pPrev->pNext = pTemp->pNext;
				pTemp = pPrev->pNext;
				delete(pDel);
				return pPrev;
			}
			else
			{
				pPrev = pTemp;
				pTemp = pTemp->pNext;
			}
		}
	}
}