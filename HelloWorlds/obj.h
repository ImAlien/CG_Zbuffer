#pragma once
#include "defs.h"
#include <string>
#include <iostream>
extern int WINDOW_WIDTH, WINDOW_HEIGHT;

class Obj {
public:
	std::vector<Face> faces;
	std::vector<std::pair<double, int>> idSort;
	double g_ymin = 1e9, g_ymax = -1e9;
	double g_xmin = 1e9, g_xmax = -1e9;
	int nface = 0 , nvertex = 0;
	//int scale_size = 1;//ģ�͸��ݴ��ڣ��Ŵ�����С�ı���
	Obj() {}
	Obj(std::string filename) {
		loadFile(filename);
		createTable();
	}
	void loadFile(std::string filename);//���س�faces
	void loadFile(std::string filepath, std::string filename);
	void createTable();
};
