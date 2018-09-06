#define CATCH_CONFIG_MAIN

#include <unirender/gl/RenderContext.h>
#include <sw/node/PositionTrans.h>
#include <sw/node/Varying.h>
#include <sw/Evaluator.h>

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

int create_shader(const sw::Evaluator& eval)
{
	std::vector<std::string> textures;
	return RC->CreateShader(eval.GetVert().c_str(), eval.GetFrag().c_str(), textures);
}

void debug_print(const sw::Evaluator& eval)
{
	printf("//////////////////////////////////////////////////////////////////////////\n");
	printf("%s\n", eval.GetVert().c_str());
	printf("//////////////////////////////////////////////////////////////////////////\n");
	printf("%s\n", eval.GetFrag().c_str());
	printf("//////////////////////////////////////////////////////////////////////////\n");
}

}

TEST_CASE("shape2d") {
	init();

	// layout
	std::vector<ur::VertexAttrib> va_list;
	const int position_sz = 2;	// 2d
	int sz = position_sz * sizeof(float) + 4;
	va_list.push_back(ur::VertexAttrib("position", position_sz, sizeof(float), sz, 0));
	va_list.push_back(ur::VertexAttrib("color", 4, sizeof(uint8_t), sz, position_sz * sizeof(float)));

	auto layout_id = RC->CreateVertexLayout(va_list);
	RC->BindVertexLayout(layout_id);
	////

	auto pos_trans = std::make_shared<sw::node::PositionTrans>();
	auto pass_color = std::make_shared<sw::node::Varying>("color", sw::t_vec4);

	sw::Evaluator eva(pos_trans, pass_color);

	//debug_print(eva);
	int shader = create_shader(eva);

	REQUIRE(shader != 0);
}