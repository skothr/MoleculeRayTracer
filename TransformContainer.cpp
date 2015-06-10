#include "TransformContainer.h"

/////TRANSFORM CONATINER/////
TransformContainer::TransformContainer(GuiProps flags)
	: Container(flags)
{
	transform.loadIdentity();
}

TransformContainer::~TransformContainer()
{

}

void TransformContainer::drawChildren(GlInterface &gl)//, bool transform_body)
{
	//Draw body
	//if(transform_body)
	gl.transformSpace(transform.getVirtualToAbsoluteMat(), transform.getAbsoluteToVirtualMat());

	bodyChildren.draw(gl);

	//if(transform_body)
	gl.restoreLast();

	//Draw floating
	floatingChildren.draw(gl);
}


void TransformContainer::setZoom(RVec new_zoom)
{
	RVec old_zoom = zoom;
	APoint old_center = transform.absoluteToVirtualPoint(zoomPivot);

	zoom = new_zoom;

	//Clamp zoom
	zoom.x = (zoom.x < minZoom.x) ? minZoom.x : zoom.x;
	zoom.x = (zoom.x > maxZoom.x) ? maxZoom.x : zoom.x;

	zoom.y = (zoom.y < minZoom.y) ? minZoom.y : zoom.y;
	zoom.y = (zoom.y > maxZoom.y) ? maxZoom.y : zoom.y;

	
	updateTransforms();

	APoint new_center = transform.absoluteToVirtualPoint(zoomPivot);
	
	AVec	center_diff = old_center - new_center;
	moveViewOffset(center_diff);
	
	//moveViewOffset(AVec(-zoomPivot));

	//APoint new_center = transform.absoluteToVirtualPoint(size*(1.0f/2.0f));

	//AVec diff = old_center - new_center;

	//moveViewOffset(diff);


	/*
	//Only update transforms if zoom changed
	if(zoom.x != old_zoom.x || zoom.y != old_zoom.y)
	{
		AVec	zoom2 = AVec(zoom.x*zoom.x, zoom.y*zoom.y),
				old_zoom2 = AVec(old_zoom.x*old_zoom.x, old_zoom.y*old_zoom.y);
		
		//If offset is zoomed (scale first in transform matrix):
		//AVec new_v_offset = viewOffset + APoint((zoomPivot.x)*(zoom.x - old_zoom.x)/(zoom.x*old_zoom.x), (zoomPivot.y)*(zoom.y - old_zoom.y)/(zoom.y*old_zoom.y));
		//If offset is not zoomed (translate first in transformation matrix)
		AVec new_v_offset = viewOffset + zoomPivot;
		new_v_offset = APoint(new_v_offset.x*(zoom.x/old_zoom.x) - zoomPivot.x, new_v_offset.y*(zoom.y/old_zoom.y) - zoomPivot.y);

		setViewOffset(new_v_offset);
		updateTransforms();
	}
	*/
}

void TransformContainer::moveZoom(RVec d_zoom)
{
	setZoom(zoom + d_zoom);
}

void TransformContainer::scaleZoom(RVec zoom_mult)
{
	setZoom(RVec(zoom.x*zoom_mult.x, zoom.y*zoom_mult.y));
}

RVec TransformContainer::getZoom()
{
	return zoom;
}

void TransformContainer::setZoomPivot(APoint new_pivot)
{
	zoomPivot = new_pivot;
}

void TransformContainer::moveZoomPivot(AVec d_pivot)
{
	setZoomPivot(zoomPivot + d_pivot);
}

APoint TransformContainer::getZoomPivot()
{
	return zoomPivot;
}

void TransformContainer::setViewOffset(APoint new_offset)
{
	APoint old_vo = viewOffset;
	viewOffset = new_offset;

	//Clamp viewOffset
	viewOffset.x = (viewOffset.x < minViewOffset.x) ? minViewOffset.x : viewOffset.x;
	viewOffset.x = (viewOffset.x > maxViewOffset.x) ? maxViewOffset.x : viewOffset.x;
	
	viewOffset.y = (viewOffset.y < minViewOffset.y) ? minViewOffset.y : viewOffset.y;
	viewOffset.y = (viewOffset.y > maxViewOffset.y) ? maxViewOffset.y : viewOffset.y;

	//Only update transforms if viewOffset changed
	if(viewOffset.x != old_vo.x || viewOffset.y != old_vo.y)
		updateTransforms();
}

//NOTE: scales d_offset to match current zoom.
void TransformContainer::moveViewOffset(AVec d_offset)
{
	setViewOffset(viewOffset + d_offset);
}

APoint TransformContainer::getViewOffset() const
{
	return viewOffset;
}
	
void TransformContainer::setMinZoom(RVec new_min_zoom)
{
	minZoom = new_min_zoom;
	setZoom(zoom);	//Make sure zoom is within new bounds;
}

void TransformContainer::setMaxZoom(RVec new_max_zoom)
{
	maxZoom = new_max_zoom;
	setZoom(zoom);	//Make sure zoom is within new bounds
}

void TransformContainer::setMinViewOffset(APoint new_min_view_offset)
{
	minViewOffset = new_min_view_offset;
	setViewOffset(viewOffset);	//Make sure viewOffset is within new bounds
}

void TransformContainer::setMaxViewOffset(APoint new_max_view_offset)
{
	maxViewOffset = new_max_view_offset;
	setViewOffset(viewOffset);	//Make sure viewOffset is within new bounds
}


void TransformContainer::updateTransforms()
{
	if(zoom.x == 1.0f && zoom.y == 1.0f && viewOffset.x == 0.0f && viewOffset.y == 0.0f)
	{
		//vTrans = false;
		transform.loadIdentity();
	}
	else
	{
		//vTrans = true;

		//Construct transform
		transform.loadIdentity();
		transform.translateSpace(viewOffset);
		transform.scaleSpace(zoom);
		//transform.translateSpace(-zoomPivot);

		/*
		absoluteToVirtualTransform.loadTranslate(viewOffset.x, viewOffset.y);
		absoluteToVirtualTransform.scale(1.0f/zoom.x, 1.0f/zoom.y);
	
		virtualToAbsoluteTransform.loadScale(zoom.x, zoom.y);
		virtualToAbsoluteTransform.translate(-viewOffset.x, -viewOffset.y);
		*/
	}
}

const VirtualTransform* TransformContainer::getTransform() const
{
	return &transform;
}

Rect TransformContainer::clampRect(const Rect &r) const
{
	return transform.absoluteToVirtualRect(ParentElement::clampRect(transform.virtualToAbsoluteRect(r)));
}

/*
void TransformContainer::draw(GlInterface &gl)
{
	if(isolateViewport(gl))
	{
		drawBackground(gl);
		drawChildren(gl);

		if(vertical->isVisible() && horizontal->isVisible())
		{
			gl.setColor(bgStateColors[cState]);
			gl.drawRect(APoint(size.x - SCROLLBAR_WIDTH, size.y - SCROLLBAR_WIDTH), AVec(SCROLLBAR_WIDTH, SCROLLBAR_WIDTH));
		}

		restoreViewport(gl);
	}
}
*/