#include "frame.h"
#include "defs.h"
#include "obj.h"
extern Obj obj;
void FrameBuffer::getFrame() {
	obj.createTable();
	zbuffer();
	clearData();
}