/**********************************************************************************************
*   rcore_stub – Minimal platform layer for custom GLES 3.2 context                            *
*                                                                                              *
*   • No DRM/KMS, GBM, EGL, or window creation.                                                *
*   • No evdev/joystick/keyboard/mouse handling – hook in your own.                           *
*   • Only provides the symbols rcore.c expects so the engine links.                          *
*                                                                                              *
*   Drop it in place of rcore_drm.c when you already own the GL context & input pipeline.      *
**********************************************************************************************/

#include "raylib.h"
#include "rlgl.h"          // Needed for rlDrawRenderBatchActive()
#include "raylib_internal.h" // Gives access to global CORE struct (path may vary)

//----------------------------------------------------------------------------------
// Externals
//----------------------------------------------------------------------------------
extern CoreData CORE;

//----------------------------------------------------------------------------------
// Platform init/shutdown (stubs)
//----------------------------------------------------------------------------------
int InitPlatform(void)
{
	// Assume caller already has a current GLES 3.2 context & has called InitWindow().
	CORE.Window.ready = true;

	// Let raylib build its default FBOs against the already‑current context.
	if (CORE.Window.render.width == 0) CORE.Window.render.width = CORE.Window.screen.width;
	if (CORE.Window.render.height == 0) CORE.Window.render.height = CORE.Window.screen.height;
	SetupFramebuffer(CORE.Window.render.width, CORE.Window.render.height);

	return 0;
}

void ClosePlatform(void)
{
	// Nothing to clean – user manages GL context & window.
}

//----------------------------------------------------------------------------------
// Window helpers – mostly no‑ops but keep the API surface intact
//----------------------------------------------------------------------------------
bool WindowShouldClose(void)                                  { return CORE.Window.shouldClose; }
void ToggleFullscreen(void)                                   {}
void ToggleBorderlessWindowed(void)                           {}
void MaximizeWindow(void)                                     {}
void MinimizeWindow(void)                                     {}
void RestoreWindow(void)                                      {}
void SetWindowState(unsigned int flags)                       {}
void ClearWindowState(unsigned int flags)                     {}
void SetWindowIcon(Image image)                               {}
void SetWindowIcons(Image *images,int count)                  {}
void SetWindowTitle(const char *title)                        { CORE.Window.title = title; }
void SetWindowPosition(int x,int y)                           {}
void SetWindowMonitor(int monitor)                            {}
void SetWindowMinSize(int w,int h)                            { CORE.Window.screenMin.width = w; CORE.Window.screenMin.height = h; }
void SetWindowMaxSize(int w,int h)                            { CORE.Window.screenMax.width = w; CORE.Window.screenMax.height = h; }
void SetWindowSize(int w,int h)                               {}
void SetWindowOpacity(float o)                                {}
void SetWindowFocused(void)                                   {}
void *GetWindowHandle(void)                                   { return NULL; }
int  GetMonitorCount(void)                                    { return 1; }
int  GetCurrentMonitor(void)                                  { return 0; }
Vector2 GetMonitorPosition(int m)                             { return (Vector2){0,0}; }
int  GetMonitorWidth(int m)                                   { return CORE.Window.display.width; }
int  GetMonitorHeight(int m)                                  { return CORE.Window.display.height; }
int  GetMonitorPhysicalWidth(int m)                           { return 0; }
int  GetMonitorPhysicalHeight(int m)                          { return 0; }
int  GetMonitorRefreshRate(int m)                             { return 0; }
const char *GetMonitorName(int m)                             { return "External"; }
Vector2 GetWindowPosition(void)                               { return (Vector2){0,0}; }
Vector2 GetWindowScaleDPI(void)                               { return (Vector2){1.0f,1.0f}; }

//----------------------------------------------------------------------------------
// Clipboard stubs
//----------------------------------------------------------------------------------
void SetClipboardText(const char *text)                       {}
const char *GetClipboardText(void)                            { return ""; }
Image GetClipboardImage(void)                                 { Image img={0}; return img; }

//----------------------------------------------------------------------------------
// Cursor helpers
//----------------------------------------------------------------------------------
void ShowCursor(void)                                         { CORE.Input.Mouse.cursorHidden = false; }
void HideCursor(void)                                         { CORE.Input.Mouse.cursorHidden = true; }
void EnableCursor(void)                                       { CORE.Input.Mouse.cursorHidden = false; }
void DisableCursor(void)                                      { CORE.Input.Mouse.cursorHidden = true; }

//----------------------------------------------------------------------------------
// Swap buffers – caller already swaps; we just flush raylib batches so nothing is lost.
//----------------------------------------------------------------------------------
void SwapScreenBuffer(void)
{
	rlDrawRenderBatchActive();
}

//----------------------------------------------------------------------------------
// Timing (same impl as original – context‑agnostic)
//----------------------------------------------------------------------------------
#include <time.h>

double GetTime(void)
{
	struct timespec ts={0};
	clock_gettime(CLOCK_MONOTONIC,&ts);
	unsigned long long ns=(unsigned long long)ts.tv_sec*1000000000LLU+(unsigned long long)ts.tv_nsec;
	return (double)(ns-CORE.Time.base)*1e-9;
}

void OpenURL(const char *url)                                 {}

//----------------------------------------------------------------------------------
// Input placeholders – roll your own
//----------------------------------------------------------------------------------
void PollInputEvents(void)                                    {}
int  SetGamepadMappings(const char *m)                        { return 0; }
void SetGamepadVibration(int g,float l,float r,float d)       {}
void SetMousePosition(int x,int y)                            { CORE.Input.Mouse.currentPosition=(Vector2){(float)x,(float)y}; }
void SetMouseCursor(int c)                                    {}
const char *GetKeyName(int k)                                 { return ""; }

//----------------------------------------------------------------------------------
// End of file
//----------------------------------------------------------------------------------
