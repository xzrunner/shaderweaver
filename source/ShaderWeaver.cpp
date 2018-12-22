#include "shaderweaver/ShaderWeaver.h"

namespace sw
{

CU_SINGLETON_DEFINITION(ShaderWeaver);

extern void regist_rttr();

ShaderWeaver::ShaderWeaver()
{
	regist_rttr();
}

}