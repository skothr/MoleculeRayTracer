#ifndef APOLLO_TRANSFORM_CONTAINER_H
#define APOLLO_TRANSFORM_CONTAINER_H

#include "GuiElement.h"
#include "Container.h"

#include "VirtualTransform.h"

class TransformContainer : public Container
{
protected:
	TransformContainer(GuiProps flags);

	RVec	zoom			= RVec(1.0f, 1.0f),
			minZoom			= RVec(FLT_MIN, FLT_MIN),
			maxZoom			= RVec(FLT_MAX, FLT_MAX);
	APoint	zoomPivot		= APoint(0.0f, 0.0f),
			viewOffset		= APoint(0.0f, 0.0f),
			minViewOffset	= APoint(FLT_MIN, FLT_MIN),
			maxViewOffset	= APoint(FLT_MAX, FLT_MAX);
	//TODO: Rotation (???)
	
	VirtualTransform transform;

	virtual void drawChildren(GlInterface &gl) override;//, bool transform_body = true) override;

public:
	virtual ~TransformContainer();
	
	void setZoom(RVec new_zoom);
	void moveZoom(RVec d_zoom);
	void scaleZoom(RVec zoom_mult);
	RVec getZoom();
	void setZoomPivot(APoint new_pivot);
	void moveZoomPivot(AVec d_pivot);
	APoint getZoomPivot();
	void setViewOffset(APoint new_offset);
	//NOTE: scales d_offset to match current zoom.
	void moveViewOffset(AVec d_offset);
	APoint getViewOffset() const;
	
	void setMinZoom(RVec new_min_zoom);
	void setMaxZoom(RVec new_max_zoom);
	void setMinViewOffset(APoint new_min_view_offset);
	void setMaxViewOffset(APoint new_max_view_offset);

	virtual void updateTransforms();
	const VirtualTransform* getTransform() const;
	virtual Rect clampRect(const Rect &r) const override;
	
	//virtual void draw(GlInterface &gl) override;
	//virtual void update(const Time &dt) override;
};


#endif	//APOLLO_TRANSFORM_CONTAINER_H