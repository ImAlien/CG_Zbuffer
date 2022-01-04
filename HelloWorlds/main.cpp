#include "glut.h"
#include "obj.h"
#include "defs.h"
#include "frame.h"
#include <iostream>
using namespace std;
//定义全局变量
int WINDOW_WIDTH = 640, WINDOW_HEIGHT = 480;
vector<nodeClassifiedPolygon*> PolygonTable;
vector<nodeClassifiedEdge*> EdgeTable;
float fTranslate;
float fRotate;
float fScale = 1.0f;								// set inital scale value to 1.0f

void Draw_Triangle()									// This function draws a triangle with RGB colors
{
	glBegin(GL_TRIANGLES);
	// 设置顶点颜色
	glColor3f(1.0f, 0.0f, 0.0f);
	// 设置顶点坐标
	glVertex3f(-1.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	
	glEnd();
}

void reshape(int width, int height)
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	// Perspective view
	//gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	// Ortho view
	glOrtho(-4, 4, -4, 4, 0, 100);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void idle()
{
	glutPostRedisplay();
}

void redraw()
{
	// 如果想使用线框模式
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();									// 重置变换矩阵（回到世界坐标系）

	glPushMatrix(); // 设置一个新的坐标系
	glTranslatef(-2.0f, 0.0f, -6.0f);				// 向左平移
	glScalef(1.1f, 4.5f, 1.4f); // 设置三个轴分别的缩放比例
	Draw_Triangle();
	glPopMatrix(); // 回到前一个坐标系

	glPushMatrix();
	glRotatef(19.198f, 0.0f, 1.0f, 0.0f); // 绕轴（0, 1, 0)旋转19.198°（角度制）
	glTranslatef(0.0f, 0.0f, -6.0f);
	Draw_Triangle();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(2.0f, 0.0f, -6.0f);
	Draw_Triangle();
	glPopMatrix();

	glutSwapBuffers();
}
Obj obj;
FrameBuffer frameBuffer;
void modelInit() {
	for (int i = 0; i < WINDOW_HEIGHT; i++) {
		PolygonTable.push_back(new nodeClassifiedPolygon());
		EdgeTable.push_back(new nodeClassifiedEdge());
	}
	//cout << obj.g_ymax << ' ' << obj.g_ymin;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT/* | GL_DEPTH_BUFFER_BIT*/);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//getFrameBuffer();

	//glRasterPos2i(0, 0);
	cout << frameBuffer.w << ' ' << frameBuffer.h << endl;
	glDrawPixels(frameBuffer.w, frameBuffer.h,
		GL_RGB, GL_UNSIGNED_BYTE, frameBuffer.getAddress(0, 0));
	glutSwapBuffers();

	//displayInfo();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);  
	int windowHandle = glutCreateWindow("Z-buffer");
	modelInit();
	//system("pause");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}


