#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

geometry geom1;
geometry geom2;
effect eff;
target_camera cam;

bool load_content() {
  // *********************************
  // Set geometry type to triangle strip
	geom1.set_type(GL_TRIANGLE_STRIP);
  // *********************************
  // Positions
  vector<vec3> positions1{
      // *********************************
      // Add the position data for two triangles here
	  //shape 1
	  vec3(-1.0f, -1.0f, 0.0f),
	  vec3(1.0f, -1.0f, 0.0f), 
	  vec3(-1.0f, 1.0f, 0.0f),
	  vec3(1.0f, 1.0f, 0.0f),
      // *********************************
  };
  geom2.set_type(GL_TRIANGLE_STRIP);
  // *********************************
  // Positions
  vector<vec3> positions2{
	  // *********************************
	  // Add the position data for two triangles here
	  vec3(-1.0f, 2.0f, 0.0f),
	  vec3(1.0f, 2.0f, 0.0f),
	  vec3(-1.0f, 4.0f, 0.0f),
	  vec3(1.0f, 4.0f, 0.0f)
	  // *********************************
  };

  // Colours
  vector<vec4> colours{
		vec4(1.0f, 0.0f, 0.0f, 1.0f), 
		vec4(1.0f, 0.0f, 0.0f, 1.0f), 
		vec4(1.0f, 0.0f, 0.0f, 1.0f),
        vec4(1.0f, 0.0f, 0.0f, 1.0f)
  };
  // Add to the geometry
  geom1.add_buffer(positions1, BUFFER_INDEXES::POSITION_BUFFER);
  geom1.add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);

  geom2.add_buffer(positions2, BUFFER_INDEXES::POSITION_BUFFER);
  geom2.add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);

  // Load in shaders
  eff.add_shader("shaders/basic.vert", GL_VERTEX_SHADER);
  eff.add_shader("shaders/basic.frag", GL_FRAGMENT_SHADER);
  // Build effect
  eff.build();

  // Set camera properties
  cam.set_position(vec3(10.0f, 10.0f, 10.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
  cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
  return true;
}

bool update(float delta_time) {
  // Update the camera
  cam.update(delta_time);
  return true;
}

bool render() {
  // Bind effect
  renderer::bind(eff);
  // Create MVP matrix
  mat4 M(1.0f);
  auto V = cam.get_view();
  auto P = cam.get_projection();
  auto MVP = P * V * M;
  // Set MVP matrix uniform
  glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
  // Render geometry
  renderer::render(geom1);
  renderer::render(geom2);
  return true;
}

void main() {
  // Create application
  app application("05_Quads_with_Triangle_Strips");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}