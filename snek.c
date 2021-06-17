#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <wingdi.h>

#define TILE_SIZE 32
#define WIDTH 31
#define HEIGHT 17
#define WIN_WIDTH WIDTH*TILE_SIZE
#define WIN_HEIGHT HEIGHT*TILE_SIZE
#define TOTAL_TILES WIDTH*HEIGHT
#define startLen 3
#define foodcount 3
#define rect(x,y,dx,dy) {x,y,dx,dy}
#define a TILE_SIZE/4

int foodStartPos        [2] = {WIDTH-3,(HEIGHT-1)/2};
int startPos            [2] = {3,(HEIGHT-1)/2};
int snekLen                 = startLen;
int direction               = 1;
int best                    = 0;

int snekPos[TOTAL_TILES][3];
int foodPos             [2];

short keyLock               = 1;
short keyStop               = 1;


RECT winRect = {0,0,WIN_WIDTH,WIN_HEIGHT};

HBRUSH BACKGROUND_COLOR;
HBRUSH SNAKE_COLOR1;
HBRUSH SNAKE_COLOR2;
HBRUSH APPLE_COLOR1;
;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Update(HWND hwnd);

void restart(){
    direction=0;
    snekLen=startLen;
    for(int i=0;i<startLen;i++){
        snekPos[i][0] = startPos[0];
        snekPos[i][1] = startPos[1];
    }
    foodPos[0]    = foodStartPos[0];
    foodPos[1]    = foodStartPos[1];
}

int WINAPI winMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    BACKGROUND_COLOR = CreateSolidBrush(RGB(135, 167, 132));
    SNAKE_COLOR1     = CreateSolidBrush(RGB(134, 147, 118));
    SNAKE_COLOR2     = CreateSolidBrush(RGB(123, 136, 107));
    APPLE_COLOR1     = CreateSolidBrush(RGB(113, 73, 85));
    
    // Register the window class.
    const wchar_t CLASS_NAME[]  = L"Herom123";
    
    WNDCLASS wc={0};

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = BACKGROUND_COLOR;
    
    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"The Snek Game  -  Made by Herom123",                 // Window text
        WS_OVERLAPPEDWINDOW-WS_SIZEBOX-WS_MAXIMIZEBOX|WS_VISIBLE, // Window style
        CW_USEDEFAULT, CW_USEDEFAULT,   // Position
        WIN_WIDTH+2, WIN_HEIGHT+25,   // Size
        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg={0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if(keyLock && keyStop){
            if(GetKeyState(VK_UP) & 0x80 && direction!=3)
            {
                direction=4;keyLock=0;
            } else if(GetKeyState(VK_DOWN) & 0x80 && direction!=4)
            {
                direction=3;keyLock=0;
            } else if(GetKeyState(VK_LEFT) & 0x80 && direction!=1)
            {
                direction=2;keyLock=0;
            } else if(GetKeyState(VK_RIGHT) & 0x80 && direction!=2)
            {
                direction=1;keyLock=0;
            }
        } else if(!keyStop) {
            if(GetKeyState(VK_SPACE) & 0x80){
                restart();
                keyStop=1;
                direction=1;
                }
        }

        if(GetKeyState('P') & 0x80)
        {
            direction=0;
        } else if(GetKeyState('R') & 0x80)
        {
            restart();
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        {
            
            restart();
            SetTimer(hwnd, 1, 100, NULL); 
            return 0;
        }
        

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_TIMER:
        keyStop&&InvalidateRect(hwnd, NULL, FALSE);
        return 0;

    case WM_PAINT:
        if(keyStop)Update(hwnd);
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
LPCWSTR numbers[10]={L"0",L"1",L"2",L"3",L"4",L"5",L"6",L"7",L"8",L"9"};

#define letterPadding 7

int exp(int n,int ex){
    int l=n;
    for(int i=0;i<ex-1;i++) l*=n;
    return ex?l:1;
}

void drawNumber(HDC hdc,int number,int x, int y, int lenght, int format){

    LPCWSTR text[2]={L"",L""};
    
    for(int i=lenght-1;i>=0;i--)
    {
        int dividing = exp(10,i);
        int index    = number/dividing%10;
        text[i]      = numbers[index];
        DrawTextW( hdc, text[i], -1,  &((RECT)rect((letterPadding*(lenght-i-1))+x,y,0,0)), DT_NOCLIP|format);
    }
}



void Update(HWND hwnd){
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    SetBkMode(hdc,TRANSPARENT);
    SetTextColor(hdc, RGB(255,255,255));

    if(direction){
        int snekPosX = snekPos[snekLen-1][0];
        int snekPosY = snekPos[snekLen-1][1];
        if(snekPos[snekLen-1][0]==foodPos[0]&&snekPos[snekLen-1][1]==foodPos[1]){
            while (1)
            {
                foodPos[0]=rand()%WIDTH;
                foodPos[1]=rand()%HEIGHT;
                if(GetPixel(hdc,foodPos[0]*TILE_SIZE,foodPos[1]*TILE_SIZE)==RGB(135, 167, 132))
                break;
            }
            snekLen++;
        } else {
            memmove(&snekPos[0], &snekPos[1], sizeof(snekPos));
        }
        
        snekPos[snekLen-1][0] = snekPos[snekLen-2][0];
        snekPos[snekLen-1][1] = snekPos[snekLen-2][1];
        snekPos[snekLen-1][2] = direction;
        
        int  oddDirection = direction%2==1;
        short  verticalAxis = direction>2;
        int   scrMeasure   = verticalAxis?HEIGHT-1:WIDTH-1;

        if(snekPos[snekLen-1][verticalAxis]==(oddDirection?scrMeasure:0)) snekPos[snekLen-1][verticalAxis]=(oddDirection?0:scrMeasure);
        else snekPos[snekLen-1][verticalAxis]+=(oddDirection?1:-1);

        keyLock=1;
    }
    for(int i=0;i<snekLen;i++)
        if(snekLen-1!=i&&snekPos[i][0]==snekPos[snekLen-1][0]&&snekPos[i][1]==snekPos[snekLen-1][1]&&keyStop&&direction) {
            keyStop=0;
            direction=0;
            if(best<snekLen-startLen) best=snekLen-startLen;
            DrawTextW( hdc, L"BEST SCORE:", -1, &((RECT)rect(0,WIN_HEIGHT/2-20,WIN_WIDTH/2+30,0)), DT_SINGLELINE | DT_NOCLIP | DT_RIGHT ) ;
            drawNumber(hdc,best,WIN_WIDTH/2+30,WIN_HEIGHT/2-20,4,DT_LEFT|DT_SINGLELINE);
            DrawTextW( hdc, L"Press 'SPACE' to Start", -1, &((RECT)rect(WIN_WIDTH,WIN_HEIGHT/2,0,0)), DT_SINGLELINE | DT_NOCLIP | DT_CENTER ) ;
            goto endUpdate;
        }
        
    FillRect(hdc,&winRect,BACKGROUND_COLOR);
    FillRect(hdc,&((RECT)rect(foodPos[0]*TILE_SIZE+a,foodPos[1]*TILE_SIZE+a,(foodPos[0]+1)*TILE_SIZE-a,(foodPos[1]+1)*TILE_SIZE-a)),APPLE_COLOR1);
    
    for(int i=snekLen-1;i>=0;i--){
        int x=snekPos[i][0];
        int y=snekPos[i][1];
        FillRect(hdc,&((RECT)rect(x*TILE_SIZE,y*TILE_SIZE,(x+1)*TILE_SIZE,(y+1)*TILE_SIZE)),(snekLen-i+1)%2==1?SNAKE_COLOR1:SNAKE_COLOR2);
    }

    drawNumber(hdc,snekLen-startLen,WIDTH,0,4,DT_LEFT|DT_SINGLELINE);
    endUpdate:
    EndPaint(hwnd, &ps);
    return;
}

//build-cr snek.c winMain kernel32.lib user32.lib gdi32.lib&&snek