// BismIllahIRRahmaanIRRaheem
/* entrypoint */
#include <runtime/enginedef.h>
#ifdef FLX_WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
#else
int main(int argc, char* argv[]) {
#endif // FLX_WIN32
    
    return 0;
}