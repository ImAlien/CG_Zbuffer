#include "glut.h"
#include "obj.h"
#include "defs.h"
#include "frame.h"
#include <iostream>
#include <time.h>
using namespace std;
//定义全局变量
int WINDOW_WIDTH = 640, WINDOW_HEIGHT = 480;
vector<nodeClassifiedPolygon*> PolygonTable;
vector<nodeClassifiedPolygon*> PolygonTableTail;
map<int, vector<nodeClassifiedEdge*> > PolygonEdges;
vector<nodeClassifiedEdge*> EdgeTable;// 分类边表
vector<nodeClassifiedEdge*> EdgeTableTail;

nodeActivePolygon* activePolygonHead = new nodeActivePolygon();
nodeActivePolygon* activePolygonTail = new nodeActivePolygon();
nodeActiveEdgePair* activeEdgePairHead = new nodeActiveEdgePair();
nodeActiveEdgePair* activeEdgePairTail = new nodeActiveEdgePair();
map<int, nodeClassifiedPolygon*> id2Polygon;
vector<float> zbuffer_array;
Color EDGE_COLOR(255,255,255);

time_t start_time,model_time, draw_time;

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

Obj obj;
FrameBuffer frameBuffer;
void printMessage() {
	//obj创建完成
	//cout << obj.g_ymin << ' ' << obj.g_ymax << endl;
	//traversePolygonTable();
	//traverseEdgeTable();
	cout <<"面片数: " << obj.nface;
	cout << "  加载时间:" << model_time - start_time <<"  绘制时间:" << draw_time - model_time<<  endl;
}
void modelInit() {
	//加载obj文件
	start_time = clock();
	obj.loadFile("models/bunny.obj");  
	cout << obj.g_ymin << ' ' << obj.g_ymax << endl;
	//初始化分类多边形表
	for (int i = 0; i < WINDOW_HEIGHT; i++) {
		PolygonTable.push_back(new nodeClassifiedPolygon());
		EdgeTable.push_back(new nodeClassifiedEdge());
	}
	for (auto t : PolygonTable) PolygonTableTail.push_back(t);
	for (auto t : EdgeTable) EdgeTableTail.push_back(t);
	//
	zbuffer_array.resize(WINDOW_WIDTH);
	//初始化活化多边形双向链表
	activeEdgePairHead->next = activeEdgePairTail;
	activeEdgePairTail->pre = activeEdgePairHead;
	activePolygonHead->next = activePolygonTail;
	activePolygonTail->pre = activePolygonHead;
	//
	obj.createTable();
	model_time = clock();
	//cout << "table success" << endl;
	zbuffer();
	draw_time = clock();
	printMessage();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT/* | GL_DEPTH_BUFFER_BIT*/);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//frameBuffer.getFrame();

	//glRasterPos2i(0, 0);
	//cout << frameBuffer.w << ' ' << frameBuffer.h << endl;
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


