#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include "camera.h"

float vert[] = {1,1,0,1,-1,0,-1,-1,0,-1,1,0};
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
void showAxis(int size){
        size++;
    glColor3f(1.0, 0.0, 0.0);
    glLineWidth(2);
    glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(size*4, 0.0, 0.0);
    glEnd();
    // ось Y, зеленая линия
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, size*4, 0.0);
    glEnd();

    // ось Z, синяя линия
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, size*4);
    glEnd();

}
void Init_Light(GLfloat theta)
{
glEnable(GL_LIGHTING); //общее освещения для всего пространства
glShadeModel(GL_SMOOTH);
GLfloat light_position[] = { 0.0f, 0.0f, 15.0f, 1.0f }; //позиция источника
GLfloat light_spot_direction[] = {0.0, 0.0, -1.0, 1.0}; // позиция цели
GLfloat light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f }; //параметры
GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //параметры
GLfloat light_specular[] = { 0.2f, 0.2f, 0.2f, 32.0f }; //параметры
glPushMatrix();
        glRotatef(theta,0,1,0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glTranslatef(0,0,15);
    glColor3f(1,1,1);
    glEnable(GL_VERTEX_ARRAY);
        glVertexPointer(3,GL_FLOAT,0,&vert);
        glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisable(GL_VERTEX_ARRAY);
glPopMatrix();

glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 55); // конус для
//направленного источника
glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_spot_direction);
glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 8.0); // экспонента
//убывания интенсивности
//задействование настроек для источника LIGHT0
glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
glEnable(GL_LIGHT0);
}
void Init_Material()
{
    glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
glEnable(GL_COLOR_MATERIAL); //разрешения использования
//материала
glShadeModel(GL_SMOOTH); // сглаживает границы
GLfloat material_ambient[] = { 0.2f, 0.2f, 0.2f, 0.0f };
GLfloat material_diffuse[] = { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 32.0f };
GLfloat material_shininess[] = { 50.0f }; //блеск материала
glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

}




void drawDoubleTruncatedCone(float radius, float height, int numSides) {
    glBegin(GL_QUAD_STRIP);
    glEnableClientState(GL_NORMAL_ARRAY);
    for (int i = 0; i <= numSides; i++) {
        float angle1 = 2.0f * M_PI * i / numSides;
        float angle2 = 2.0f * M_PI * (i + 1) / numSides;
        float x1 = radius * cos(angle1);
        float z1 = radius * sin(angle1);
        float x2 = radius * cos(angle2);
        float z2 = radius * sin(angle2);

        glNormal3f(x1, 0.0f, z1);
        glVertex3f(x1, 0.0f, z1);

        glNormal3f(x2, 0.0f, z2);
        glVertex3f(x2, 0.0f, z2);
    }
    glEnd();
}

void drawPrism(float radius, float height, int numSides) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < numSides; i++) {
        float angle = 2.0f * M_PI * i / numSides;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i < numSides; i++) {
        float angle = 2.0f * M_PI * i / numSides;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        glVertex3f(x, height, z);
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < numSides; i++) {
        float angle = 2.0f * M_PI * i / numSides;
        float nextAngle = 2.0f * M_PI * (i + 1) / numSides;
        float x1 = radius * cos(angle);
        float z1 = radius * sin(angle);
        float x2 = radius * cos(nextAngle);
        float z2 = radius * sin(nextAngle);

        glVertex3f(x1, 0.0f, z1);
        glVertex3f(x1, height, z1);

        glVertex3f(x2, 0.0f, z2);
        glVertex3f(x2, height, z2);
    }
    glEnd();
}

void drawPrismaticHyperboloid() {
    float radius = 1.0f;
    float height = 1.0f;
    int numSides = 6;

    glPushMatrix();

    // Поворачиваем объект так, чтобы его основание было параллельно плоскости XZ
    glRotatef(-90, 1, 0, 0);

    // Перемещаем объект в начальную точку
    glTranslatef(0.0f, -0.5f, 0.0f);
    drawPrism(radius, height, numSides);
    glPopMatrix();
}
 void drawHyperboloids(){
      for (int i = 0; i < 10; i++) {
            glPushMatrix();
            glColor4f(0,1,1,i*0.1);
            glRotatef(90+i*10,0,0,1);
            glTranslatef(4 + i * 0.1, i, 1);
            glScalef(1, 1, 2);
            drawPrismaticHyperboloid();
            glPopMatrix();
        }
 }



void Draw_Cube(){
glColor4f(0,1,0,0.5);
GLfloat vertices[] = {
-0.5f, -0.5f, -0.5f,
0.5f, -0.5f, -0.5f,
0.5f, 0.5f, -0.5f,
-0.5f, 0.5f, -0.5f,
-0.5f, -0.5f, 0.5f,
0.5f, -0.5f, 0.5f,
0.5f, 0.5f, 0.5f,
-0.5f, 0.5f, 0.5f
};
GLuint indices[] = {
0, 1, 2,
2, 3, 0,
1, 5, 6,
6, 2, 1,
7, 6, 5,
5, 4, 7,
4, 0, 3,
3, 7, 4,
4, 5, 1,
1, 0, 4,
3, 2, 6,
6, 7, 3
};
GLfloat normals[] = {
0.0f, 0.0f, -1.0f,
0.0f, 0.0f, -1.0f,
0.0f, 0.0f, -1.0f,
0.0f, 0.0f, -1.0f,
0.0f, 0.0f, 1.0f,
0.0f, 0.0f, 1.0f,
0.0f, 0.0f, 1.0f,
0.0f, 0.0f, 1.0f,
-1.0f, 0.0f, 0.0f,
-1.0f, 0.0f, 0.0f,
-1.0f, 0.0f, 0.0f,
-1.0f, 0.0f, 0.0f,
1.0f, 0.0f, 0.0f,
1.0f, 0.0f, 0.0f,
1.0f, 0.0f, 0.0f,
1.0f, 0.0f, 0.0f,
0.0f, -1.0f, 0.0f,
0.0f, -1.0f, 0.0f,
0.0f, -1.0f, 0.0f,
0.0f, -1.0f, 0.0f,
0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f
};
glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(3, GL_FLOAT, 0, vertices);
glEnableClientState(GL_NORMAL_ARRAY);
glNormalPointer(GL_FLOAT, 0, normals);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_NORMAL_ARRAY);
}
float normal_vert[]={0,0,1, 0,0,1, 0,0,1, 0,0,1};
void showWorld(int squares){
    glEnable(GL_DEPTH_TEST);
    showAxis(squares);

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3,GL_FLOAT,0,&vert);
        glEnableClientState(GL_NORMAL_ARRAY);
   glNormalPointer(GL_FLOAT,0,&normal_vert);
        for(int i = -squares; i < squares; i++)
            for(int j = -squares; j < squares; j++){
                glPushMatrix();
                    if((i+j) % 2 == 0 )
                        glColor3f(0,0.5,0);
                    else
                        glColor3f(1,1,1);
                    glTranslatef(i*2,j*2,0);
                    glDrawArrays(GL_TRIANGLE_FAN,0,4);
                glPopMatrix();
        }
   glDisable(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPushMatrix();
    drawHyperboloids();
    glTranslatef(2,2,1);
    Draw_Cube();
    glTranslatef(5,-2,4);
    Draw_Cube();
      glPopMatrix();


}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          GetSystemMetrics(SM_CXSCREEN),
                          GetSystemMetrics(SM_CYSCREEN),
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);
    glFrustum(-1,1,-1,1,2,100);
    /* program main loop */
    GLfloat xAngleSpeed = 20;
    GLfloat zAngleSpeed = 20;
    POINTFLOAT playerPos = {0,0};
    POINT curPos;
    POINT centerPos={250,250};
    ShowCursor(false);

    GLfloat dir=0.5f;
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glPushMatrix();
             GetCursorPos(&curPos);
             moveCamera(xAngleSpeed, zAngleSpeed, playerPos, curPos);
            Init_Light(theta);
            Init_Material();
             showWorld(5);


            glPopMatrix();
            SwapBuffers(hDC);
            if(theta>90)
                dir=-0.25f;
            if(theta<=-90)
                dir=0.25f;
            theta+=dir;


            Sleep (1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

