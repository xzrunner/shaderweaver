#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class CameraPos : public sw::Node
{
public:
	CameraPos()
		: Node("CameraPos")
	{
		InitVariables({
		}, {
			{ t_flt3 | t_unif, CamPosName() },
		}, {
		});
	}

    static const char* CamPosName() { return "u_cam_pos"; }

    RTTR_ENABLE(Node)

}; // CameraPos

}
}