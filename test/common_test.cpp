#define CATCH_CONFIG_MAIN

#include <unirender/gl/RenderContext.h>
#include <sw/Evaluator.h>

#include <sw/node/Uniform.h>
#include <sw/node/Input.h>
#include <sw/node/Output.h>
#include <sw/node/PositionTrans.h>
#include <sw/node/Tex2DSample.h>
#include <sw/node/ColorAddMul.h>
#include <sw/node/Vector4.h>

#include <catch/catch.hpp>
#include <gl/glew.h>
#include <glfw3.h>

namespace
{

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

bool InitGL()
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	GLFWwindow* window = glfwCreateWindow(100, 100, "rotate-crop", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	//// Initialize GLEW to setup the OpenGL Function pointers
	//if (glewInit() != GLEW_OK) {
	//	return -1;
	//}

	return true;
}

std::shared_ptr<ur::gl::RenderContext> RC = nullptr;

void init()
{
	static bool inited = false;
	if (!inited) {
		InitGL();
		RC = std::make_shared<ur::gl::RenderContext>(1024);
		inited = true;
	}
}

int create_shader(const sw::Evaluator& vert, const sw::Evaluator& frag)
{
	std::vector<std::string> textures;
	return RC->CreateShader(vert.GetShaderStr().c_str(), frag.GetShaderStr().c_str(), textures);
}

void debug_print(const sw::Evaluator& vert, const sw::Evaluator& frag)
{
	printf("//////////////////////////////////////////////////////////////////////////\n");
	printf("%s\n", vert.GetShaderStr().c_str());
	printf("//////////////////////////////////////////////////////////////////////////\n");
	printf("%s\n", frag.GetShaderStr().c_str());
	printf("//////////////////////////////////////////////////////////////////////////\n");
}

}

TEST_CASE("shape2d") {
	init();

	// layout

	std::vector<ur::VertexAttrib> va_list;
	const int sz = 12;
	va_list.push_back(ur::VertexAttrib("position", 2, sizeof(float),   sz, 0));
	va_list.push_back(ur::VertexAttrib("color",    4, sizeof(uint8_t), sz, 8));

	auto layout_id = RC->CreateVertexLayout(va_list);
	RC->BindVertexLayout(layout_id);

	// vert

	auto projection = std::make_shared<sw::node::Uniform>("u_projection", sw::t_mat4);
	auto modelview  = std::make_shared<sw::node::Uniform>("u_modelview",  sw::t_mat4);
	auto position   = std::make_shared<sw::node::Input>  ("position",     sw::t_flt4);
	auto pos_trans  = std::make_shared<sw::node::PositionTrans>();
	sw::make_connecting({ projection, 0 }, { pos_trans, sw::node::PositionTrans::IN_PROJ });
	sw::make_connecting({ modelview,  0 }, { pos_trans, sw::node::PositionTrans::IN_MODELVIEW });
	sw::make_connecting({ position,   0 }, { pos_trans, sw::node::PositionTrans::IN_POS });

	auto vert_in_col = std::make_shared<sw::node::Input>    ("color",   sw::t_col4);
	auto vert_out_color = std::make_shared<sw::node::Output>("v_color", sw::t_col4);
	sw::make_connecting({ vert_in_col, 0 }, { vert_out_color, 0 });

	// frag

	auto frag_in_col = std::make_shared<sw::node::Input>("v_color", sw::t_col4);

	// end

	sw::Evaluator vert({ pos_trans, vert_out_color }, sw::ST_VERT);
	sw::Evaluator frag({ frag_in_col }, sw::ST_FRAG);

	debug_print(vert, frag);
	int shader = create_shader(vert, frag);

	REQUIRE(shader != 0);
}

//TEST_CASE("sprite") {
//	init();
//
//	// layout
//	std::vector<ur::VertexAttrib> va_list;
//	const int sz = 24;
//	va_list.push_back(ur::VertexAttrib("position", 2, sizeof(float),   sz, 0));
//	va_list.push_back(ur::VertexAttrib("texcoord", 2, sizeof(float),   sz, 8));
//	va_list.push_back(ur::VertexAttrib("color",    4, sizeof(uint8_t), sz, 16));
//	va_list.push_back(ur::VertexAttrib("additive", 4, sizeof(uint8_t), sz, 20));
//
//	auto layout_id = RC->CreateVertexLayout(va_list);
//	RC->BindVertexLayout(layout_id);
//	////
//
//	auto pos_trans = std::make_shared<sw::node::PositionTrans>();
//	auto set_uv = std::make_shared<sw::node::VaryingSet>("texcoord", sw::t_uv);
//	auto set_mul = std::make_shared<sw::node::VaryingSet>("color", sw::t_col4);
//	auto set_add = std::make_shared<sw::node::VaryingSet>("additive", sw::t_col4);
//
//	auto get_uv = std::make_shared<sw::node::VaryingGet>("texcoord", sw::t_uv);
//	auto get_mul = std::make_shared<sw::node::VaryingGet>("color", sw::t_col4);
//	auto get_add = std::make_shared<sw::node::VaryingGet>("additive", sw::t_col4);
//
//	auto tex_sample = std::make_shared<sw::node::Tex2DSample>();
//	tex_sample->AddInput(get_uv);
//	auto col_add_mul = std::make_shared<sw::node::ColorAddMul>();
//	col_add_mul->AddInput(tex_sample);
//	col_add_mul->AddInput(get_mul);
//	col_add_mul->AddInput(get_add);
//	sw::Evaluator eva({ pos_trans, set_uv, set_mul, set_add }, col_add_mul);
//
//	debug_print(eva);
//	int shader = create_shader(eva);
//
//	REQUIRE(shader != 0);
//}