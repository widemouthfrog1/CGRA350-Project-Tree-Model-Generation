
#pragma once

#include "Include.h"
#include "TreeFactory.h"
#include "Turtle.h"

// Basic model that holds the shader, mesh and transform for drawing.
// Can be copied and modified for adding in extra information for drawing
// including textures for texture mapping etc.
struct basic_model {
	GLuint shader = 0;
	cgra::gl_mesh mesh;
	glm::vec3 color{0.7};
	glm::mat4 modelTransform{1.0};
	GLuint texture;

	void draw(const glm::mat4 &view, const glm::mat4 proj);
};
struct treeModel {
	bool lines = false;
	basic_model model;
	vector<basic_model> cylinders;
	glm::vec3 position;
};
// geometry


class Turtle;

// Main application class
//
class Application {
private:
	// window
	glm::vec2 m_windowsize;
	GLFWwindow *m_window;

	// oribital camera
	float m_pitch = .86;
	float m_yaw = -.86;
	float m_distance = 20;

	// last input
	bool m_leftMouseDown = false;
	glm::vec2 m_mousePosition;

	// drawing flags
	bool m_show_axis = false;
	bool m_show_grid = false;
	bool m_showWireframe = false;

	// geometry
	std::vector<basic_model> m_models;

	//Project
	std::vector<treeModel> trees;
	std::vector<std::string> rules;

	//Project
	char axiom[500] = "EF";
	int depth = 1;
	int oldDepth = 1;
	int oldSelectedTree = 0;
	int selectedTree = 0;
	bool selectAll = true;
	char rule[50] = "";
	char guirules[500] = "EF:F[C+EF][C-EF][C^EF][C&EF]\nC:A(1)D(d*(1/2))R((1/10)/(2*l))";
	int rulesIndex = std::string(guirules).size();
	char presets[4][5][500] = 
	{ {"EF", "EF:F[C+EF][C-EF][C^EF][C&EF]\nC:A(1)D(d*(1/2))R((1/10)/(2*l))", "EF:F[C+EF][C-EF][C^EF][C&EF]", "C:A(1)D(d*(1/2))R((1/10)/(2*l))", ""},
	{"R((1/2))D(0.1)FD(0.3)R((4/5)*r)FD(0.7)R((11/12)*r)FD(2>3)R((10/11)*r)FC", "C:PF[PFC][B]\nP:A(0.1)+A(0.1)^D((d/6)/(d/2))R(0.8*r)\nB:A(1)/A(0.8)+D(6/(l*4))R(0.6*r)FC", "C:PF[PFC][B]", "P:A(0.1)+A(0.1)^D((d/6)/(d/2))R(0.8*r)", "B:A(1)/A(0.8)+D(6/(l*4))R(0.6*r)FC"}, 
	{"EF", "EF:F[C+EF][C-EF][C^EF][C&EF]\nC:A(0.5>1.2)D(d*(1/2))R((1/10)/(2*l))","EF:F[C+EF][C-EF][C^EF][C&EF]", "C:A(0.5>1.2)D(d*(1/2))R((1/10)/(2*l))", ""},
	{"R((1/2)>0.75)D(0.1)FD(0.3)R((4/5)*r)FD(0.7)R((11/12)*r)FD(2>3)R((10/11)*r)FC", "C:PF[PFC][B]\nP:A((0-0.1)>0.1)+A((0-0.1)>0.1)^D((d/6)/(d/2))R((0.9*r)>(0.95*r))\nB:A(0>6.28)/A(0.5>1.2)+D((3/(l*4))>(9/(l*4)))R((r/2/l)>(0.9*r/l))FC","C:PF[PFC][B]", "P:A((0-0.1)>0.1)+A((0-0.1)>0.1)^D((d/6)/(d/2))R((0.8*r)>(0.85*r))", "B:A(0>6.28)/A(0.5>1.2)+D((3/(l*4))>(9/(l*4)))R((r/2)>(0.7*r))FC"}
	};
	char * stringPresetNames[4] = { "Basic", "Complex", "Randomised Basic", "Randomised Complex" };
	int presetNumber = 1;
	int resolution = 3;
	bool lines = false;
	std::random_device rd;
	char seedText[9] = "";
	unsigned long long seed;
	std::mt19937 randomNumberGenerator;

	void clearRules();
public:
	// setup
	Application(GLFWwindow *);

	// disable copy constructors (for safety)
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	// rendering callbacks (every frame)
	void render();
	void renderGUI();

	// input callbacks
	void cursorPosCallback(double xpos, double ypos);
	void mouseButtonCallback(int button, int action, int mods);
	void scrollCallback(double xoffset, double yoffset);
	void keyCallback(int key, int scancode, int action, int mods);
	void charCallback(unsigned int c);
};