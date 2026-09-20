#include <monkey3/renderitem.h>
#include <monkey3/shaders/quadshader.h>
#include <monkey3/shaders/lineshader.h>


QuadRenderItem::QuadRenderItem(int maxPrimitives) :
	RenderItem(std::make_unique<QuadShader>(), maxPrimitives) { }

void QuadRenderItem::submitPrimitiveTyped(const QuadInfo& info) 
{
	// QuadInfo -> QuadVertex
}

LineRenderItem::LineRenderItem(int maxPrimitives) :
	RenderItem(std::make_unique<LineShader>(), maxPrimitives) {
}

void LineRenderItem::submitPrimitiveTyped(const LineInfo& info) {
	LineVertex v1;
	LineVertex v2;

	v1.pos = info.start;
	v1.color = info.color;
	v2.pos = info.end;
	v2.color = info.color;
	auto index = this->batch().vertices.size();
	this->batch().vertices.push_back(v1);
	this->batch().vertices.push_back(v2);
	this->batch().indices.push_back(index);
	this->batch().indices.push_back(index+1);
	//	v1.color = color;
	
	//	v2.pos = {end.x, end.y, 0.f};
	//	v2.color = color;
	//	_lineBatch.vertices.push_back(v1);
	//	_lineBatch.vertices.push_back(v2);
	//	_lineBatch.indices.push_back(index);
	//	_lineBatch.indices.push_back(index + 1);
	//}
}