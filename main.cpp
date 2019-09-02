// Third part libraries and standards__________________________________________________________________________________
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cglm/cglm.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include "glad.c"


// Own autorship libraries_____________________________________________________________________________________________
#include "configuration.cpp"
#include "macros.cpp"
#include "CGLM.cpp"
#include "OpenGl.cpp"
#include "Shader.cpp"
#include "SDLGlWindow.cpp"
#include "Texture.cpp"
#include "Vertex.cpp"
#include "Mesh.cpp"
#include "Model.cpp"
#include "Camera.cpp"
#include "Time.cpp"


// TODO: Remove this, data should not be declared here
typedef struct {
	float x;
	float y;
	float sensitivity;
} Mouse;

// TODO: Remove all global variables if possible
SDLGlWindow window;
Mouse mouse;
bool running = true;

// TODO: This should be part of gamestate class, NYI
void handleInputEvents( Camera *cam, Time *game_time )
{
	SDL_Event event;

	while ( SDL_PollEvent( &event ) != 0 )
	{
		switch ( event.type )
		{
			case SDL_QUIT:
			{
				running = false;
				return;
			}
			case SDL_WINDOWEVENT:
			{
				//event.window.event
				switch ( event.window.event )
				{
					case SDL_WINDOWEVENT_RESIZED:
					{

						window.width = event.window.data1;
						window.height = event.window.data2;
						glViewport( 0, 0, window.width, window.height );
						break;
					}
					// TODO : maybe verify size changed event
				}
				break;
			}
			case SDL_KEYDOWN:
			{
				//event.key.timestamp;
				//event.key.state = SDL_PRESSED or SDL_RELEASED;
				//event.key.keysym;
				//event.key.keysym.sym = SDLK_a, SDLK_b, ...
				//event.key.keysym.mod = KMOD_NONE, KMODE_SHIFT, ...

				switch ( event.key.keysym.sym )
				{
					case ( SDLK_w ):
					{
						vec3 scaled;
						glm_vec3_scale( cam->front, cam->speed * game_time->delta, scaled );
						glm_vec3_add( cam->position, scaled, cam->position );
						break;
					}
					case ( SDLK_s ):
					{
						vec3 scaled;
						glm_vec3_scale( cam->front, cam->speed * game_time->delta, scaled );
						glm_vec3_sub( cam->position, scaled, cam->position );
						break;
					}
					case ( SDLK_a ):
					{
						vec3 scaled;
						glm_vec3_scale( cam->right, cam->speed * game_time->delta, scaled );
						glm_vec3_sub( cam->position, scaled, cam->position );
						break;
					}
					case ( SDLK_d ):
					{
						vec3 scaled;
						glm_vec3_scale( cam->right, cam->speed * game_time->delta, scaled );
						glm_vec3_add( cam->position, scaled, cam->position );
						break;
					}
				}
				break;
			}
			case SDL_KEYUP:
			{
				//event.key.timestamp;
				//event.key.state = SDL_PRESSED or SDL_RELEASED;
				//event.key.keysym;
				//event.key.keysym.sym = SDLK_a, SDLK_b, ...
				//event.key.keysym.mod = KMOD_NONE, KMODE_SHIFT, ...
				break;
			}
			case SDL_MOUSEMOTION:
			{
				//event.motion.timestamp;
				//event.motion.state = SDL_BUTTON_LMASK, SDL_BUTTON_RMASK, ...
				//event.motion.x;
				//event.motion.y;
				if ( event.motion.state & SDL_BUTTON_RMASK )
				{
					if ( mouse.x == -1 && mouse.y == -1 )
					{
						mouse.x = event.motion.x;
						mouse.y = event.motion.y;
					}

				    float x_offset = ( event.motion.x - mouse.x ) * cam->speed;
				    float y_offset = ( event.motion.y - mouse.y ) * cam->speed * ( -1 );

				    cam->y_rotation += x_offset;
				    cam->x_rotation += y_offset;

				    if( cam->x_rotation > 89.0f )
				    {
				        cam->x_rotation = 89.0f;
				    }
				    if( cam->x_rotation < -89.0f )
				    {
				        cam->x_rotation = -89.0f;
				    }

				    cam->front[0] = cos( glm_rad( cam->x_rotation ) ) * cos( glm_rad( cam->y_rotation ) );
				    cam->front[1] = sin( glm_rad( cam->x_rotation ) );
				    cam->front[2] = cos( glm_rad( cam->x_rotation ) ) * sin( glm_rad( cam->y_rotation ) );

				    glm_vec3_normalize( cam->front );
				    glm_vec3_cross( cam->front, cam->world_up, cam->right );
				    glm_vec3_normalize( cam->right );
				    glm_vec3_cross( cam->right, cam->front, cam->up );
				    glm_vec3_normalize( cam->up );
				}

				mouse.x = event.motion.x;
				mouse.y = event.motion.y;
				
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				//event.button.timestamp;
				//event.button.button = SDL_BUTTON_LEFT, SDL_BUTTON_X1, SDL_BUTTON_MIDDLE, ...
				//event.button.state = SDL_PRESSED or SDL_RELEASED;
				//event.button.x;
				//event.button.y;
				//event.button.clicks; ( >= 1 )
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				//event.button.timestamp;
				//event.button.button = SDL_BUTTON_LEFT, SDL_BUTTON_X1, SDL_BUTTON_MIDDLE, ...
				//event.button.state = SDL_PRESSED or SDL_RELEASED;
				//event.button.x;
				//event.button.y;
				//event.button.clicks; ( >= 1 )
				break;
			}
			case SDL_MOUSEWHEEL:
			{
				//event.wheel.timestamp;
				//event.wheel.y;
				break;
			}
		}
	}
}


float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


// Main Function_______________________________________________________________________________________________________
int main( int argc, char* args[] )
{
	window.initializeWindow();
	window.initializeImage();
	window.initializeAudio();
	window.initializeGlContext();

	OpenGl::enableDepth();

	uint vert_shader        = Shader::createVertexShader  ( "shader.vert" );
	uint frag_object_shader = Shader::createFragmentShader( "object_shader.frag" );
	uint frag_light_shader  = Shader::createFragmentShader( "light_shader.frag" );	
	uint shaders[2];	
	shaders[0] = vert_shader;
	uint prog_object_shader, prog_light_shader;
	shaders[1] = frag_object_shader;
	prog_object_shader = Shader::createShaderProgram( shaders, 2 );
	shaders[1] = frag_light_shader;
	prog_light_shader  = Shader::createShaderProgram( shaders, 2 );;

	uint box_VAO = OpenGl::createVertexArrayObject();
	uint box_VBO = OpenGl::createBuffer();
	OpenGl::fillVertexBufferObject( box_VAO, box_VBO, sizeof( float ) * 36 * 6, vertices, GL_STATIC_DRAW );
	OpenGl::defineAttribFormat( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), ( void* ) 0 );
	OpenGl::defineAttribFormat( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), ( void* ) ( 3 * sizeof( float ) ) );
	vec3 box_pos = { 0.0f, 0.0f, 0.0f };

	uint light_VAO = OpenGl::createVertexArrayObject();
	uint light_VBO = OpenGl::createBuffer();
	OpenGl::fillVertexBufferObject( light_VAO, light_VBO, sizeof( float ) * 36 * 6, vertices, GL_STATIC_DRAW );
	OpenGl::defineAttribFormat( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), ( void* ) 0 );
	OpenGl::defineAttribFormat( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), ( void* ) ( 3 * sizeof( float ) ) );
	vec3 light_pos = { 1.2f, 1.0f, 2.0f };

	//Model nanosuit = Model( "nanosuit/nanosuit.obj" );

	vec3 position = { 0.0f, 0.0f, 3.0f  };
	vec3 front    = { 0.0f, 0.0f, -1.0f };
	vec3 up       = { 0.0f, 1.0f, 0.0f  };

	Camera cam( position, front, up, 1.5f, &window.width, &window.height );

	// TODO: Polish this
	mouse.x = -1;
	mouse.y = -1;
	mouse.sensitivity = 0.05f;

	cam.initProjection();

	Time game_time( &window );

	while ( running )
	{
		game_time.update();

		vec4 clear_color = { 0.1f, 0.1f, 0.1f, 1.0f };
		OpenGl::clearBuffers( clear_color, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		handleInputEvents( &cam, &game_time );

		Shader::installProgram( prog_object_shader );
		cam.initView();
		vec3 box_scale       = { 1.0f, 1.0f, 1.0f };	
		vec3 box_rotate_axis = { 0.0f, 0.0f, 1.0f };
		cam.initModel( box_pos, box_scale, box_rotate_axis, 0.0f );
		vec3 material_ambient  = { 1.0f, 0.5f, 0.31f };
		vec3 material_diffuse  = { 1.0f, 0.5f, 0.31f };
		vec3 material_specular = { 0.5f, 0.5f, 0.5f };
		float material_shininess = 32.0f;
		vec3 light_ambient  = { 0.2f, 0.2f, 0.2f };
		vec3 light_diffuse  = { 0.5f, 0.5f, 0.5f };
		vec3 light_specular = { 1.0f, 1.0f, 1.0f };
		light_diffuse[0] = sin( game_time.current * 2.0f );
		light_diffuse[1] = sin( game_time.current * 0.7f );
		light_diffuse[2] = sin( game_time.current * 1.3f );
		Shader::initUniformArrayf ( prog_object_shader, "material.ambient",    material_ambient,   3 );
		Shader::initUniformArrayf ( prog_object_shader, "material.diffuse",    material_diffuse,   3 );
		Shader::initUniformArrayf ( prog_object_shader, "material.specular",   material_specular,  3 );
		Shader::initUniformArrayf ( prog_object_shader, "material.shininess", &material_shininess, 1 );
		Shader::initUniformArrayf ( prog_object_shader, "light.ambient",       light_ambient,     3 );
		Shader::initUniformArrayf ( prog_object_shader, "light.diffuse",       light_diffuse,     3 );
		Shader::initUniformArrayf ( prog_object_shader, "light.specular",      light_specular,    3 );
		Shader::initUniformArrayf ( prog_object_shader, "light.position",      light_pos,         3 );
		Shader::initUniformArrayf ( prog_object_shader, "cam_position",        cam.position,      3 );
		Shader::initUniformMatrix4( prog_object_shader, "view",       GL_FALSE, *( cam.view       ) );
		Shader::initUniformMatrix4( prog_object_shader, "projection", GL_FALSE, *( cam.projection ) );
		Shader::initUniformMatrix4( prog_object_shader, "model",      GL_FALSE, *( cam.model      ) );
		OpenGl::bindVertexArrayObject( box_VAO );
		OpenGl::drawArrays( GL_TRIANGLES, 36 );

		Shader::installProgram( prog_light_shader );
		cam.initView();
		vec3 light_scale       = { 1.0f, 1.0f, 1.0f };	
		vec3 light_rotate_axis = { 0.0f, 0.0f, 1.0f };
		cam.initModel( light_pos, light_scale, light_rotate_axis, 0.0f );
		Shader::initUniformMatrix4( prog_light_shader, "view",       GL_FALSE, *( cam.view       ) );
		Shader::initUniformMatrix4( prog_light_shader, "projection", GL_FALSE, *( cam.projection ) );
		Shader::initUniformMatrix4( prog_light_shader, "model",      GL_FALSE, *( cam.model      ) );
		OpenGl::bindVertexArrayObject( light_VAO );
		OpenGl::drawArrays( GL_TRIANGLES, 36 );

		//nanosuit.draw( shader_program );

		OpenGl::bindVertexArrayObject( 0 );

		window.updateWindow();
	}

	Shader::deleteProgram( prog_light_shader );
	Shader::deleteProgram( prog_object_shader );
	Shader::deleteShader( vert_shader );
	Shader::deleteShader( frag_light_shader );
	Shader::deleteShader( frag_object_shader );
	window.deleteWindow();

	return 0;
}