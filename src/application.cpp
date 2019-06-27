
// project
#include "application.hpp"
#include "cgra/cgra_geometry.hpp"
#include "cgra/cgra_gui.hpp"
#include "cgra/cgra_image.hpp"
#include "cgra/cgra_shader.hpp"
#include "cgra/cgra_wavefront.hpp"


using namespace std;
using namespace cgra;
using namespace glm;


void basic_model::draw(const glm::mat4 &view, const glm::mat4 proj) {
	mat4 modelview = view * modelTransform;
	
	glUseProgram(shader); // load shader and variables
	glUniformMatrix4fv(glGetUniformLocation(shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(modelview));
	glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(color));
	mesh.draw(); // draw
}


void Application::clearRules()
{
	rulesIndex = 0;
	rules.clear();
	for (int i = 0; i < 500; i++) {
		guirules[i] = '\0';
	}
}

Application::Application(GLFWwindow *window) : m_window(window) {
	
	shader_builder sb;
    sb.set_shader(GL_VERTEX_SHADER, CGRA_SRCDIR + std::string("//res//shaders//color_vert.glsl"));
	sb.set_shader(GL_FRAGMENT_SHADER, CGRA_SRCDIR + std::string("//res//shaders//color_frag.glsl"));
	GLuint shader = sb.build();
	

	vector<Variable> alphabet;
	vector<Rule> rules;
	Circle base(vec3(0,0,0), 1, vec3(0,0,0));
	std::vector<Circle> branches;
	branches.push_back(Circle(vec3(0, 10, -1), 1, vec3(1, 0, 1)));
	branches.push_back(Circle(vec3(0, 5, 4), 1, vec3(pi<float>() / 2, 0, 0)));
	/*Branch branch(base, branches);
	Variable start('A', branch);
	TreeFactory treeMaker(alphabet, rules, start);*/
	/*
	Turtle turtle = Turtle(glm::vec3(1, 0, 0), glm::vec3(1, 0, 0));
	Turtle turtle2 = Turtle(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
	turtle.draw("F[+F]F[+F]F[+F]F[+F]F");
	//m_models.at(0).mesh = turtle.createMesh();
	turtle2.draw("F[+F]F[+F]F[+F]F[+F]F");
	*/
	//m_models.at(1).mesh = turtle2.createMesh();
	Turtle turtle = Turtle();
	this->rules.push_back("EF:F[C+EF][C-EF][C^EF][C&EF]");
	this->rules.push_back("C:A(1)D(d*(1/2))R((1/10)/(2*l))");

	turtle.loadRules(this->rules);
	seed = rd();
	randomNumberGenerator = mt19937(seed);
	treeModel tree;
	tree.position = vec3(0, 0, 2);
	if (lines) {
		tree.model = basic_model();
		tree.model.shader = shader;
		tree.model.mesh = turtle.createMesh();
	}
	else {
		vector<gl_mesh> cylinders = turtle.cylinders(turtle.getCommand(axiom, depth), resolution, randomNumberGenerator);
		for (int i = 0; i < cylinders.size(); i++) {
			basic_model model = basic_model();
			model.shader = shader;
			model.mesh = cylinders.at(i);
			tree.cylinders.push_back(model);
		}
	}
	trees.push_back(tree);
	
	
	treeModel tree2;
	tree2.position = vec3(0, 0, -2);
	if (lines) {
		tree2.model = basic_model();
		tree2.model.shader = shader;
		tree2.model.mesh = tree.model.mesh;
	}
	else {
		tree2.cylinders = tree.cylinders;
	}
	trees.push_back(tree2);
	//vector<Branch> branches = turtle3.createBranches(turtle3.getCommand("F", depth), 10, mat4(1));
	
	
	/*m_models.at(0).mesh = treeMaker.createTree();
	vector<Vertex> points = base.createFullCircle(20);
	vector<mesh_vertex> vertices;
	vector<unsigned int> indices;
	for (int i = 0; i < points.size(); i++) {
		Vertex vertex = points.at(i);
		for (int j = 0; j < vertex.connectionsSize(); j++) {
			int connectionIndex = vertex.getConnection(j);
			if (vertex.useConnection(points.at(connectionIndex))) {
				indices.push_back(i);
				indices.push_back(connectionIndex);
			}
		}
	}
	for (int i = 0; i < points.size(); i++) {
		mesh_vertex vert;
		vert.pos = points.at(i).pos;
		vertices.push_back(vert);
	}
	mesh_builder builder;
	builder.vertices = vertices;
	builder.indices = indices;
	builder.mode = GL_LINES;
	m_models.at(1).mesh = builder.build();
	vertices.clear();
	indices.clear();
	points = branches.at(0).createFullCircle(20);
	for (int i = 0; i < points.size(); i++) {
		Vertex vertex = points.at(i);
		for (int j = 0; j < vertex.connectionsSize(); j++) {
			int connectionIndex = vertex.getConnection(j);
			if (vertex.useConnection(points.at(connectionIndex))) {
				indices.push_back(i);
				indices.push_back(connectionIndex);
			}
		}
	}
	for (int i = 0; i < points.size(); i++) {
		mesh_vertex vert;
		vert.pos = points.at(i).pos;
		vertices.push_back(vert);
	}
	builder.vertices = vertices;
	builder.indices = indices;
	builder.mode = GL_LINES;
	m_models.at(2).mesh = builder.build();
	vertices.clear();
	indices.clear();
	points = branches.at(1).createFullCircle(20);
	for (int i = 0; i < points.size(); i++) {
		Vertex vertex = points.at(i);
		for (int j = 0; j < vertex.connectionsSize(); j++) {
			int connectionIndex = vertex.getConnection(j);
			if (vertex.useConnection(points.at(connectionIndex))) {
				indices.push_back(i);
				indices.push_back(connectionIndex);
			}
		}
	}
	for (int i = 0; i < points.size(); i++) {
		mesh_vertex vert;
		vert.pos = points.at(i).pos;
		vertices.push_back(vert);
	}
	builder.vertices = vertices;
	builder.indices = indices;
	builder.mode = GL_LINES;
	m_models.at(3).mesh = builder.build();*/
}


void Application::render() {
	

	// retrieve the window hieght
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height); 

	m_windowsize = vec2(width, height); // update window size
	glViewport(0, 0, width, height); // set the viewport to draw to the entire window

	// clear the back-buffer
	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	// enable flags for normal/forward rendering
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LESS);

	// projection matrix
	mat4 proj = perspective(1.f, float(width) / height, 0.1f, 1000.f);

	// view matrix
	mat4 view = translate(mat4(1), vec3(0, 0, -m_distance))
		* rotate(mat4(1), m_pitch, vec3(1, 0, 0))
		* rotate(mat4(1), m_yaw,   vec3(0, 1, 0));


	// helpful draw options
	if (m_show_grid) drawGrid(view, proj);
	if (m_show_axis) drawAxis(view, proj);
	glPolygonMode(GL_FRONT_AND_BACK, (m_showWireframe) ? GL_LINE : GL_FILL);

	/*if (depth != oldDepth) {
		Turtle turtle = Turtle( glm::vec3(0, 1, 0));
		turtle.loadRules(this->rules);
		turtle.draw(turtle.getCommand(axiom, depth));
		m_models.at(0).mesh = turtle.createMesh();
		m_models.at(1).mesh = m_models.at(0).mesh;
		oldDepth = depth;
	}*/

	// draw the model
	for (int i = 0; i < trees.size(); i++) {
		if (trees.at(i).lines) {
			trees.at(i).model.draw(view * translate(mat4(1), trees.at(i).position), proj);
		}
		else {
			for (int j = 0; j < trees.at(i).cylinders.size(); j++) {
				trees.at(i).cylinders.at(j).draw(view * translate(mat4(1), trees.at(i).position), proj);
			}
		}
	}
	
}


void Application::renderGUI() {

	// setup window
	ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_Once);
	ImGui::Begin("Options", 0);

	// display current camera parameters
	ImGui::Text("Application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::SliderFloat("Pitch", &m_pitch, -pi<float>() / 2, pi<float>() / 2, "%.2f");
	ImGui::SliderFloat("Yaw", &m_yaw, -pi<float>(), pi<float>(), "%.2f");
	ImGui::SliderFloat("Distance", &m_distance, 0, 100, "%.2f", 2.0f);

	// helpful drawing options
	ImGui::Checkbox("Show axis", &m_show_axis);
	ImGui::SameLine();
	ImGui::Checkbox("Show grid", &m_show_grid);
	ImGui::Checkbox("Wireframe", &m_showWireframe);
	ImGui::SameLine();
	if (ImGui::Button("Screenshot")) rgba_image::screenshot(true);


	
	ImGui::Separator();
	ImGui::Text("Turtle Language:\n\nF: Move forward and draw a line/create a cylinder\nf: Move forward\n+: Turn right\n-: Turn Left\n^: Pitch up\n&: Pitch down\n/: Roll right\n\\: Roll left\n[: Save state\n]: Load last saved state\nD(EXP): Set distance to expression\nA(EXP): Set angle to expression\nR(EXP): Set radius of cylinder to expression\nEXP:\n\tExpressions support +-*/()\n\tEXP>EXP: A randomly generated number from one expression to another\n\td: The current distance moved\n\ta: The current change in angle\n\tr: The current radius\n\tl: The current level (number of saves on the stack) ");
	ImGui::Separator();

	// example of how to use input boxes
	if (ImGui::Button("Generate Random Seed")) {
		seed = rd();
		randomNumberGenerator = mt19937(seed);
	}
	if (ImGui::InputText("Seed", seedText, 7, ImGuiInputTextFlags_EnterReturnsTrue)) {
		std::string seedString(seedText);
		std::string seedStringNumber;
		for (int i = 0; i < seedString.size(); i++) {
			seedStringNumber += to_string((unsigned int)(seedString.at(i)));
		}
		seed = stoull(seedStringNumber);
		randomNumberGenerator = mt19937(seed);
	}
	ImGui::Text(to_string(seed).c_str());
	ImGui::Checkbox("Lines", &lines);
	ImGui::Checkbox("SelectAll", &selectAll);
	ImGui::SliderInt("SelectedTree", &selectedTree, 0, trees.size() - 1);
	
	
	
	ImGui::Combo("Presets", &presetNumber, stringPresetNames, 4);
	if(ImGui::Button("Use Preset")) {
			clearRules();
			for (int i = 0; i < 500; i++) {
				axiom[i] = '\0';
			}
			std::string s = std::string(presets[presetNumber][0]);
			for (int i = 0; i < s.size(); i++) {
				axiom[i] = s.at(i);
			}
			s = std::string(presets[presetNumber][1]);
			for (int i = 0; i < s.size(); i++) {
				guirules[i] = s.at(i);
			}
			rules.push_back(presets[presetNumber][2]);
			rules.push_back(presets[presetNumber][3]);
			rules.push_back(presets[presetNumber][4]);
			rulesIndex = s.size();
	}
	if (ImGui::Button("Clear Rules")) {
		clearRules();
	}
	ImGui::InputText("Axiom", axiom, 500);
	if (ImGui::InputText("Add Rule:", rule, 50, ImGuiInputTextFlags_EnterReturnsTrue)) {
		rules.push_back(std::string(rule));
		if (rules.size() != 1) {
			guirules[rulesIndex] = '\n';
		}
		for (int i = 0; i < rules.at(rules.size() - 1).size(); i++) {
			guirules[rulesIndex + i + 1] = rules.at(rules.size() - 1).at(i);
		}
		rulesIndex += rules.at(rules.size() - 1).size() + 1;
	}
	ImGui::Text(guirules);
	ImGui::SliderInt("Depth", &depth, 1, 6);
	if (ImGui::Button("Generate Tree")) {
		shader_builder sb;
		sb.set_shader(GL_VERTEX_SHADER, CGRA_SRCDIR + std::string("//res//shaders//color_vert.glsl"));
		sb.set_shader(GL_FRAGMENT_SHADER, CGRA_SRCDIR + std::string("//res//shaders//color_frag.glsl"));
		GLuint shader = sb.build();
		Turtle turtle = Turtle();
		turtle.loadRules(rules);
		gl_mesh mesh;
		vector<gl_mesh> cylinders;
		string command = turtle.getCommand(axiom, depth);
		//cout << command << endl;
		if (lines) {
			turtle.draw(command, randomNumberGenerator);
			mesh = turtle.createMesh();
		}
		else {
			cylinders = turtle.cylinders(command, resolution, randomNumberGenerator);
		}
		if (selectAll) {
			for (int i = 0; i < trees.size(); i++) {
				if (lines) {
					treeModel tree;
					tree.model.mesh = mesh;
					tree.model.shader = shader;
					tree.position = trees.at(i).position;
					tree.lines = true;
					trees.at(i) = tree;
					
				}
				else {
					trees.at(i).cylinders.clear();
					for (int j = 0; j < cylinders.size(); j++) {
						basic_model model = basic_model();
						model.shader = shader;
						model.mesh = cylinders.at(j);
						trees.at(i).cylinders.push_back(model);
						trees.at(i).lines = false;
					}
				}
			}
		}
		else {
			if (lines) {
				treeModel tree;
				tree.model.mesh = mesh;
				tree.model.shader = shader;
				tree.position = trees.at(selectedTree).position;
				tree.lines = true;
				trees.at(selectedTree) = tree;
				
			}
			else {
				trees.at(selectedTree).cylinders.clear();
				for (int j = 0; j < cylinders.size(); j++) {
					basic_model model = basic_model();
					model.shader = shader;
					model.mesh = cylinders.at(j);
					trees.at(selectedTree).cylinders.push_back(model);
					trees.at(selectedTree).lines = false;
				}
			}
		}
	}

	// finish creating window
	ImGui::End();
}


void Application::cursorPosCallback(double xpos, double ypos) {
	if (m_leftMouseDown) {
		vec2 whsize = m_windowsize / 2.0f;

		// clamp the pitch to [-pi/2, pi/2]
		m_pitch += float(acos(glm::clamp((m_mousePosition.y - whsize.y) / whsize.y, -1.0f, 1.0f))
			- acos(glm::clamp((float(ypos) - whsize.y) / whsize.y, -1.0f, 1.0f)));
		m_pitch = float(glm::clamp(m_pitch, -pi<float>() / 2, pi<float>() / 2));

		// wrap the yaw to [-pi, pi]
		m_yaw += float(acos(glm::clamp((m_mousePosition.x - whsize.x) / whsize.x, -1.0f, 1.0f))
			- acos(glm::clamp((float(xpos) - whsize.x) / whsize.x, -1.0f, 1.0f)));
		if (m_yaw > pi<float>()) m_yaw -= float(2 * pi<float>());
		else if (m_yaw < -pi<float>()) m_yaw += float(2 * pi<float>());
	}

	// updated mouse position
	m_mousePosition = vec2(xpos, ypos);
}


void Application::mouseButtonCallback(int button, int action, int mods) {
	(void)mods; // currently un-used

	// capture is left-mouse down
	if (button == GLFW_MOUSE_BUTTON_LEFT)
		m_leftMouseDown = (action == GLFW_PRESS); // only other option is GLFW_RELEASE
}


void Application::scrollCallback(double xoffset, double yoffset) {
	(void)xoffset; // currently un-used
	m_distance *= pow(1.1f, -yoffset);
}


void Application::keyCallback(int key, int scancode, int action, int mods) {
	(void)key, (void)scancode, (void)action, (void)mods; // currently un-used
}


void Application::charCallback(unsigned int c) {
	(void)c; // currently un-used
}