#include "GlInterface.h"

#include <GL/glew.h>

#include "WindowWin32.h"


/////GUI SPACE/////

GuiSpace::GuiSpace()
	: transToView(), viewToTrans(), view(), clamped()
{
	transToView.loadIdentity();
	viewToTrans.loadIdentity();
}

GuiSpace::GuiSpace(const Mat3f &tSpaceToView, const Mat3f &viewToTSpace, const ViewRect &view_, const Edges &clamp)
	: transToView(tSpaceToView), viewToTrans(viewToTSpace), base_transToView(tSpaceToView), base_viewToTrans(viewToTSpace),
		view(view_), clamped(clamp)
{ }



/////VERTEX/////

TVertex::TVertex()
	: pos(0, 0), color(1.0f, 0.0f, 1.0f, 1.0f)
{ }

TVertex::TVertex(const APoint &p, const Color &c)
	: pos(p), color(c)
{ }

                   
/////GL INTERFACE/////
//ShaderProgram *GlInterface::textSP = nullptr;
//GLuint GlInterface::text_vbID = 0;

GlInterface::GlInterface(const ViewRect &base_view)
	: baseView(base_view)
{
	Mat3f identity;
	identity.loadIdentity();

	spaceStack.push(GuiSpace(identity, identity, base_view, Edges()));

	setViewport(base_view);
}


Vec3f GlInterface::currToView(const Vec3f &v) const
{
	return spaceStack.top().transToView*v;
}

Vec3f GlInterface::viewToCurr(const Vec3f &v) const
{
	return spaceStack.top().viewToTrans*v;
}

void GlInterface::setViewport(const ViewRect &view)
{
	test = Viewport(view.x, baseView.h - (view.y + view.h), view.w, view.h);
	glViewport(test.x, test.y, test.w, test.h);
}

void GlInterface::setBaseSpace(const Mat3f &base_transToView, const Mat3f &base_viewToTrans, const ViewRect &base_view)
{
	spaceStack = std::stack<GuiSpace>();
	spaceStack.push(GuiSpace(base_transToView, base_viewToTrans, base_view, Edges()));
	
	baseView = base_view;
	setViewport(baseView);
}


bool GlInterface::isolateViewport(const APoint &pos, const AVec &size, const APoint &view_offset, bool clamp)
{
	//Last view
	Edges last_clamped = spaceStack.top().clamped;
	ViewRect last_view = spaceStack.top().view;

	//Get new view
	AVec pos_view = absoluteToViewPoint(pos),
		 size_view = absoluteToViewVec(size);
	ViewRect new_view(last_view.pos + pos_view, size_view);

	Edges clamped;

	if(clamp)
	{
		ViewRect clamped_view = ViewRect::findIntersection(new_view, last_view);

		clamped.l = clamped_view.x - new_view.x;
		clamped.t = clamped_view.y - new_view.y;
		clamped.r = new_view.w - clamped_view.w - clamped.l;
		clamped.b = new_view.h - clamped_view.h - clamped.t;
		
		//Must be above 0
		//clamped.l *= (clamped.l > 0);
		//clamped.t *= (clamped.t > 0);
		//clamped.r *= (clamped.r > 0);
		//clamped.b *= (clamped.b > 0);

		new_view = clamped_view;
	}

	if(new_view.isValid())
	{
		setViewport(new_view);
		
		GuiSpace top = spaceStack.top();
		top.view = new_view;
		top.clamped = clamped;

		//Add a translate to base_trans(_inv) by position offset
		Mat3f t_mat, t_mat_inv;
		//t_mat.loadTranslate(pos_view.x, pos_view.y);
		//t_mat_inv.loadTranslate(-pos_view.x, -pos_view.y);

		//top.base_trans = top.base_trans*t_mat;
		//top.base_trans_inv = t_mat_inv*top.base_trans_inv;
		
		//Make trans(_inv) base_trans(_inv) translated to correct for clamping
		//t_mat.loadTranslate(-clamped.l, -clamped.t);
		//t_mat_inv.loadTranslate(clamped.l, clamped.t);
		AVec t = viewToAbsolutePoint(APoint(-clamped.l, -clamped.t));
		//t = absoluteToViewVec(t);
		
		t_mat.loadTranslate(t.x, t.y);
		t_mat_inv.loadTranslate(-t.x, -t.y);

		top.transToView = top.transToView*t_mat;
		top.viewToTrans = t_mat_inv*top.viewToTrans;
		//top.trans = t_mat*top.trans;
		//top.trans_inv = top.trans_inv*t_mat_inv;
		
		//Set this as the next transform space
		spaceStack.push(top);

		return true;
	}
	else
		return false;
}

/*
bool GlInterface::isolateViewport(const APoint &pos, const AVec &size, bool clamp)
{
	Edges last_clamped = spaceStack.top().clamped;
	ViewRect last_view = spaceStack.top().view;
	
	//Get new view
	ViewRect new_view(last_view.pos - AVec(last_clamped.l, last_clamped.t) + absoluteToViewPoint(pos), absoluteToViewVec(size));

	Edges clamped;
	if(clamp)
	{
		ViewRect clamped_view = ViewRect::findIntersection(new_view, last_view);

		clamped.l = clamped_view.x - new_view.x;
		clamped.t = clamped_view.y - new_view.y;
		clamped.r = new_view.w - clamped_view.w - clamped.l;
		clamped.b = new_view.h - clamped_view.h - clamped.t;
		
		//Must be above 0
		clamped.l *= (clamped.l > 0);
		clamped.t *= (clamped.t > 0);
		clamped.r *= (clamped.r > 0);
		clamped.b *= (clamped.b > 0);

		new_view = clamped_view;
	}

	if(new_view.isValid())
	{
		setViewport(new_view);
		
		Edges old_clamped = spaceStack.top().clamped;

		GuiSpace top = spaceStack.top();
		top.view = new_view;
		top.clamped = clamped;
		
		//Add a translate by difference in clamped_view (element absolute --> view)
		Mat3f t_mat, t_mat_inv;
		AVec t = viewToAbsolutePoint(AVec(old_clamped.l, old_clamped.t) - AVec(clamped.l, clamped.t));
		t_mat.loadTranslate(t.x, t.y);
		t_mat_inv.loadTranslate(-t.x, -t.y);

		top.trans = top.trans*t_mat;
		top.trans_inv = t_mat_inv*top.trans_inv;

		spaceStack.push(top);

		return true;
	}
	else
		return false;
}
*/

void GlInterface::transformSpace(const Mat3f &absoluteToVirtual, const Mat3f &virtualToAbsolute)
{
	GuiSpace top = spaceStack.top();

	//Transform trans(_inv)
	top.transToView = top.transToView*absoluteToVirtual;
	top.viewToTrans = virtualToAbsolute*top.viewToTrans;

	//Transform base_trans(_inv)
	top.base_transToView = top.base_transToView*absoluteToVirtual;
	top.base_viewToTrans = virtualToAbsolute*top.base_viewToTrans;

	spaceStack.push(top);
}

void GlInterface::restoreLast()
{
	//Dont pop last space
	if(spaceStack.size() > 1)
		spaceStack.pop();
	
	setViewport(spaceStack.top().view);
}

//void GlInterface::setWindow(WindowWin32 *gl_window)
//{
//	wglMakeCurrent(gl_window->getHandle(), ;
//}



ViewRect GlInterface::getCurrView() const
{
	return spaceStack.top().view;
}

Edges GlInterface::getCurrClampedEdges() const
{
	return spaceStack.top().clamped;
}



APoint GlInterface::absoluteToViewPoint(const APoint &p) const
{
	Edges clamped = spaceStack.top().clamped;
	ViewRect view = spaceStack.top().view;

	Vec3f t_p(p.x, p.y, 1.0f);
	t_p = currToView(t_p);	//homogenous coordinate = 1.0 (includes translate)

	return APoint(t_p.x, t_p.y);	//Adjust for clamped edges
}

AVec GlInterface::absoluteToViewVec(const AVec &v) const
{
	Vec3f t_v(v.x, v.y, 0.0f);
	t_v = currToView(t_v);	//homogenous coordinate = 0.0 (excludes translate)

	return AVec(t_v.x, t_v.y);
}

Rect GlInterface::absoluteToViewRect(const Rect &r) const
{
	return Rect(absoluteToViewPoint(r.pos), absoluteToViewVec(r.size));
}

APoint GlInterface::viewToAbsolutePoint(const APoint &p) const
{
	Edges clamped = spaceStack.top().clamped;
	ViewRect view = spaceStack.top().view;

	Vec3f t_p(p.x, p.y, 1.0f);	//Adjust for clamped edges
	t_p = viewToCurr(t_p);

	return APoint(t_p.x, t_p.y);
}

AVec GlInterface::viewToAbsoluteVec(const AVec &v) const
{
	Vec3f t_v(v.x, v.y, 0.0f);
	t_v = viewToCurr(t_v);

	return AVec(t_v.x, t_v.y);
}

Rect GlInterface::viewToAbsoluteRect(const Rect &r) const
{
	return Rect(viewToAbsolutePoint(r.pos), viewToAbsoluteVec(r.size));
}



void GlInterface::setColor(float r, float g, float b, float a) const
{
	glColor4f(r, g, b, a);
}

void GlInterface::setColor(Color col) const
{
	glColor4f(col.r, col.g, col.b, col.a);
}

void GlInterface::drawShape(GLenum primitive_type, const std::vector<TVertex> &points, bool transform) const
{
	unsigned int	i,
					p_size = points.size();

	Edges clamped = spaceStack.top().clamped;

	ViewRect v = spaceStack.top().view;
	APoint v_pos = transform ? viewToAbsolutePoint(APoint(0, 0)) : APoint(0, 0);
	AVec v_size = transform ? viewToAbsoluteVec(v.size) : v.size;

	glBegin(primitive_type);
		for(i = 0; i < p_size; i++)
		{
			GlPoint gl_p = toGlPoint(points[i].pos - v_pos, v_size);
			//GlPoint gl_p = toGlPoint(absoluteToViewPoint(points[i].pos), v.size);

			setColor(points[i].color);
			glVertex2f(gl_p.x, gl_p.y);
		}
	glEnd();
}

void GlInterface::drawRect(const APoint &pos, const AVec &size) const
{
	ViewRect v = spaceStack.top().view;

	GlPoint gl_pos = toGlPoint(absoluteToViewPoint(pos), v.size);
	GlVec gl_size = toGlVec(absoluteToViewVec(size), v.size);
	//GlPoint gl_pos(-1.0f, -1.0f);
	//GlVec gl_size(2.0f, 2.0f);

	//setColor(1.0f, 1.0f, 0.0f);

	glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(gl_pos.x, gl_pos.y);
		glVertex2f(gl_pos.x, gl_pos.y + gl_size.y);
		glVertex2f(gl_pos.x + gl_size.x, gl_pos.y);
		glVertex2f(gl_pos.x + gl_size.x, gl_pos.y + gl_size.y);
	glEnd();
}

void GlInterface::drawRect(const Rect &r) const
{
	drawRect(r.pos, r.size);
}

void GlInterface::drawLine(const APoint &p1, const APoint &p2) const
{
	ViewRect v = spaceStack.top().view;

	GlPoint gl_p1 = toGlPoint(absoluteToViewPoint(p1), v.size),
			gl_p2 = toGlPoint(absoluteToViewPoint(p2), v.size);

	glBegin(GL_LINES);
		glVertex2f(gl_p1.x, gl_p1.y);
		glVertex2f(gl_p2.x, gl_p2.y);
	glEnd();
}

void GlInterface::drawPoint(const APoint &p) const
{
	GlPoint gl_p = toGlPoint(absoluteToViewPoint(p), spaceStack.top().view.size);

	glBegin(GL_POINTS);
		glVertex2f(gl_p.x, gl_p.y);
	glEnd();
}

void GlInterface::drawText(Text &t, AVec size, const APoint &p) const
{
	Edges clamped = spaceStack.top().clamped;
	AVec v_size = absoluteToViewVec(size);
	t.setCharacterSize(v_size.y);
	t.draw(absoluteToViewPoint(p), v_size, spaceStack.top().view.size);
}

void GlInterface::drawTexture(Texture &tex, const APoint &pos, const AVec &size) const
{
	tex.draw(absoluteToViewPoint(pos), absoluteToViewVec(size), spaceStack.top().view.size);
}