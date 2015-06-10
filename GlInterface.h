#ifndef APOLLO_GL_INTERFACE_H
#define APOLLO_GL_INTERFACE_H

#include <vector>
#include <stack>

#include "Screen.h"
#include "Matrix.h"
//#include "TransformSpace.h"
#include "Viewport.h"
#include "Text.h"
#include "Rect.h"
#include "Color.h"

typedef unsigned int GLenum;

class ShaderProgram;
class WindowWin32;

//Description of a transform space
struct GuiSpace
{
	Mat3f		transToView,			//Transforms from current transform space to view space
				viewToTrans,			//Transforms from view space to current transform space (inverse of trans)
				base_transToView,		//trans without any correcting for clamping
				base_viewToTrans;		//trans_inv without any correcting for clamping

	ViewRect	view;					//Current viewport rect
	Edges		clamped;				//How much was clamped from the element to the viewport on each side

	GuiSpace();
	GuiSpace(const Mat3f &tSpaceToView, const Mat3f &viewToTSpace, const ViewRect &view_, const Edges &clamp);
};

//Position and color of a vertex (in GlInterface's current transform space)
struct TVertex
{
	APoint pos;
	Color color;

	TVertex();
	TVertex(const APoint &p, const Color &c);
};


class GlInterface
{
private:
	//Stack of GuiSpaces (top() is current)
	std::stack<GuiSpace> spaceStack;

	ViewRect baseView;
	Viewport test;

	//Transforms the given vector by the top trans matrix (current absolute -> view)
	Vec3f currToView(const Vec3f &v) const;
	//Transforms the given vector by the top trans_inv matrix (view -> current absolute)
	Vec3f viewToCurr(const Vec3f &v) const;

public:
	GlInterface(const ViewRect &base_view);
	~GlInterface() = default;

	//Sets the bottom of the stacks to the given parameters (clears everything but new space)
	void setBaseSpace(const Mat3f &base_transToView, const Mat3f &base_viewToTrans, const ViewRect &base_view);

	//Transforms the current space and adjusts view --> if view isnt visible, reverts back and returns false
	bool isolateViewport(const APoint &pos, const AVec &size, const APoint &view_offset, bool clamp = true);
	//Transforms the current transform by the given matrix
	void transformSpace(const Mat3f &absoluteToVirtual, const Mat3f &virtualToAbsolute);
	//Restores the last space (goes one level up)
	void restoreLast();

	void setViewport(const ViewRect &view);


	ViewRect getCurrView() const;
	Edges getCurrClampedEdges() const;
	

	//Transforms the given point/vector/rect from the current transform space to view space
	APoint absoluteToViewPoint(const APoint &p) const;
	AVec absoluteToViewVec(const AVec &v) const;
	Rect absoluteToViewRect(const Rect &r) const;
	//Transforms the given point/vector/rect from view space to the current transform space
	APoint viewToAbsolutePoint(const APoint &p) const;
	AVec viewToAbsoluteVec(const AVec &v) const;
	Rect viewToAbsoluteRect(const Rect &r) const;


	//Set color
	void setColor(float r, float g, float b, float a = 1.0f) const;
	void setColor(Color col) const;

	//Draw general shape
	void drawShape(GLenum primitive_type, const std::vector<TVertex> &points, bool transform = true) const;
	//Draw rectangle
	void drawRect(const APoint &pos, const AVec &size) const;
	void drawRect(const Rect &r) const;
	//Draw line
	void drawLine(const APoint &p1, const APoint &p2) const;
	//Draw point
	void drawPoint(const APoint &p) const;
	//Draw text
	void drawText(Text &t, AVec size, const APoint &p) const;
	//Draw texture
	void drawTexture(Texture &tex, const APoint &pos, const AVec &size) const;
};



#endif	//APOLLO_GL_INTERFACE_H