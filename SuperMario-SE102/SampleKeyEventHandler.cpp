#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"


void CSampleKeyHandler::KeyState(BYTE* states)
{
}

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}
