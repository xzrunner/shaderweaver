
// sdf
#include "shaderweaver/node/Sphere.h"
#include "shaderweaver/node/Torus.h"

#include <SM_Vector.h>

RTTR_REGISTRATION
{

// sdf
rttr::registration::class_<sw::node::Sphere>("sw::Sphere")
	.constructor<>()
	(
		rttr::metadata((int)(sw::node::Sphere::ID_POS), sm::vec3(0, 0, 0)),
		rttr::metadata((int)(sw::node::Sphere::ID_RADIUS), 0.5f)
	)
;

rttr::registration::class_<sw::node::Torus>("sw::Torus")
	.constructor<>()
	(
		rttr::metadata((int)(sw::node::Torus::ID_POS), sm::vec3(0, 0, 0)),
		rttr::metadata((int)(sw::node::Torus::ID_RADIUS), sm::vec2(0.2f, 0.8f))
	)
;

}

namespace sw
{

void regist_rttr()
{
}

}
