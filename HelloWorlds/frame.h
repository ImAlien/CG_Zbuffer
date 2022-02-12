#pragma once
#include "defs.h"
#include <iostream>
extern int WINDOW_HEIGHT, WINDOW_WIDTH;
class FrameBuffer {
public:
	std::vector<GLubyte> buffer_array;	// 帧缓存
	int w, h;
	//int m_centerX, m_centerY, m_center;
	FrameBuffer() :w(WINDOW_WIDTH), h(WINDOW_HEIGHT) { buffer_array.resize(w*h * 3, 0); }

	//----更新高度和宽度，并重新设置其大小
	void ResizeBuffer(int width, int height) {
		if (w != width || h != height) {
			buffer_array.resize(w*h*3, 0);
		}
	}
	//---返回要写入的帧缓存位置-----
	int cal(int x, int y, int c) {
		return (y * w + x)*3+ c;
	}
	void setPixel(int x, int y, Color& color) {
		if (x < 0 || x >= w || y < 0 || y >= h) return;
		//print(color);
		buffer_array[cal(x, y, 0)] = color.r;
		buffer_array[cal(x, y, 1)] = color.b;
		buffer_array[cal(x, y, 2)] = color.g;
	}
	Color* getAddress(int x, int y) {
		if (y >= 0 && y < h && x >= 0 && x < w)
			return ((Color*)(&buffer_array[0] + (w*y + x) * 3));
		std::cout << "getAddress error" << std::endl;
		return nullptr;
	}
	void getFrame();
};