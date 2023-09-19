#include <windows.h>
#include <string.h>
#include "VNPlayer.h"


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
	VNPlayer player;
	player.Play();

	return TRUE;
}
