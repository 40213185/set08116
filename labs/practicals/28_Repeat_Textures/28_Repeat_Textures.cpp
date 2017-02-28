#include <glm\glm.hpp>
#include <graphics_framework.h>
#include <memory>

using namespace std;
using namespace graphics_framework;
using namespace glm;

//shape2
geometry quad2;
mesh m_quad2;
//shape1
geometry quad1;
mesh m_quad1;

effect eff;
target_camera cam;
texture tex;
vec3 pos(0.0f, 0.0f, 0.0f);

bool load_content() {
	//quad1 data ******************************************************************************
	quad1.set_type(GL_TRIANGLE_STRIP);
  // Construct geometry object
  // Create pos data
  // Positions
  vector<vec3> positions{ vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f), vec3(-1.0f, 1.0f, 0.0f),
	  vec3(1.0f, 1.0f, 0.0f) };
  // *********************************
  // Define texture coordinates for shape
  vector<vec2> tex_coords{ 
	  vec2(0.0f, 0.0f), 
	  vec2(1.0f,0.0f), 
	  vec2(0.0f, 1.0f),
	  vec2(1.0f, 1.0f)
  };
  // *********************************
  // Add to the geometry
  quad1.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  // *********************************
  // Add texture coordinate buffer to geometry
  quad1.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
  // *********************************

  // Create mesh object
  m_quad1 = mesh(quad1);

  //quad 2 data ******************************************************************************
  quad2.set_type(GL_TRIANGLE_STRIP);
  // Construct geometry object
  // Create pos data
  // Positions
  vector<vec3> positions2{ 
	  vec3(-1.0f, 2.0f, 0.0f),
	  vec3(1.0f, 2.0f, 0.0f),
	  vec3(-1.0f, 4.0f, 0.0f),
	  vec3(1.0f, 4.0f, 0.0f) };
  
  // Add to the geometry
  quad2.add_buffer(positions2, BUFFER_INDEXES::POSITION_BUFFER);
  // *********************************
  // Add texture coordinate buffer to geometry
  quad2.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
  // *********************************

  // Create mesh object
  m_quad2 = mesh(quad2);

  // Load in texture shaders, !Note that are pulling in shader file from previous project!
  eff.add_shader("27_Texturing_Shader/simple_texture.vert", GL_VERTEX_SHADER);
  eff.add_shader("27_Texturing_Shader/simple_texture.frag", GL_FRAGMENT_SHADER);

  // *********************************
  // Build effect
  eff.build();
  // Load texture "textures/sign.jpg"
  tex = texture("textures/sign.jpg");
  // *********************************

  // Set camera properties
  cam.set_position(vec3(0.0f, 0.0f, 10.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
  cam.set_projection(quarter_pi<float>(), aspect, 1.0f, 1000.0f);

  return true;
}

bool update(float delta_time) {
	// Check if key is pressed
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP)) {
		pos += vec3(0.0f, 0.0f, -5.0f) * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN)) {
		pos += vec3(0.0f, 0.0f, 5.0f) * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT)) {
		pos += vec3(-5.0f, 0.0f, 0.0f) * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_RIGHT)) {
		pos += vec3(5.0f, 0.0f, 0.0f) * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_Z)) {
		pos += vec3(0.0f, 5.0f, 0.0f) * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_X)) {
		pos += vec3(0.0f, -5.0f, 0.0f) * delta_time;
	}
	// Update the camera
	cam.update(delta_time);
	return true;
}

bool render() {
  // Bind effect
  renderer::bind(eff);
  mat4 T(1.0f), R;
  // Create MVP matrix
  T = translate(mat4(1.0f), pos);
  auto V = cam.get_view();
  auto P = cam.get_projection();
  auto MVP = P * V * T;
  // Set MVP matrix uniform
  glUniformMatrix4fv(eff.get_uniform_location("MVP"), // Location of uniform
                     1,                               // Number of values - 1 mat4
                     GL_FALSE,                        // Transpose the matrix?
                     value_ptr(MVP));                 // Pointer to matrix data

  // *********************************
  // Bind texture to renderer
  renderer::bind(tex, 0);
  // Set the texture value for the shader here
  glUniform1i(eff.get_uniform_location("tex"), 0);
  // *********************************

  // Render the mesh
  renderer::render(m_quad1);
  renderer::render(m_quad2);

  return true;
}

void main() {
  // Create application
  app application("28_Repeat_Textures");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}