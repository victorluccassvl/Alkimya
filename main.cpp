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
#include "configuration.cpp"
#include "macros.cpp"
#include "CGLM.cpp"
#include "OpenGl.cpp"
#include "Shader.cpp"
#include "SDLGlWindow.cpp"
#include "Model.cpp"

typedef struct {
	vec3 position;
	vec3 front;
	vec3 right;
	vec3 up;
	vec3 world_up;
	float x_rot;
	float y_rot;
	float speed;
} Camera;

typedef struct {
	float x;
	float y;
	float sensitivity;
} Mouse;

typedef struct {
	float delta;
	float last;
	float current;
} Time;


SDLGlWindow window;
Camera cam;
Mouse mouse;
Time game_time;
bool running = true;


void handleInputEvents()
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
						glm_vec3_scale( cam.front, cam.speed * game_time.delta, scaled );
						glm_vec3_add( cam.position, scaled, cam.position );
						break;
					}
					case ( SDLK_s ):
					{
						vec3 scaled;
						glm_vec3_scale( cam.front, cam.speed * game_time.delta, scaled );
						glm_vec3_sub( cam.position, scaled, cam.position );
						break;
					}
					case ( SDLK_a ):
					{
						vec3 scaled;
						glm_vec3_scale( cam.right, cam.speed * game_time.delta, scaled );
						glm_vec3_sub( cam.position, scaled, cam.position );
						break;
					}
					case ( SDLK_d ):
					{
						vec3 scaled;
						glm_vec3_scale( cam.right, cam.speed * game_time.delta, scaled );
						glm_vec3_add( cam.position, scaled, cam.position );
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

				    float x_offset = ( event.motion.x - mouse.x ) * cam.speed;
				    float y_offset = ( event.motion.y - mouse.y ) * cam.speed * ( -1 );

				    cam.y_rot += x_offset;
				    cam.x_rot += y_offset;

				    if( cam.x_rot > 89.0f )
				    {
				        cam.x_rot = 89.0f;
				    }
				    if( cam.x_rot < -89.0f )
				    {
				        cam.x_rot = -89.0f;
				    }

				    cam.front[0] = cos( glm_rad( cam.x_rot ) ) * cos( glm_rad( cam.y_rot ) );
				    cam.front[1] = sin( glm_rad( cam.x_rot ) );
				    cam.front[2] = cos( glm_rad( cam.x_rot ) ) * sin( glm_rad( cam.y_rot ) );

				    glm_vec3_normalize( cam.front );
				    glm_vec3_cross( cam.front, cam.world_up, cam.right );
				    glm_vec3_normalize( cam.right );
				    glm_vec3_cross( cam.right, cam.front, cam.up );
				    glm_vec3_normalize( cam.up );
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

int main( int argc, char* args[] )
{
	window.initializeWindow();
	window.initializeImage();
	window.initializeAudio();
	window.initializeGlContext();

	OpenGl::enableDepth();

	uint vertex_shader  = Shader::createVertexShader( "shader.vert" );
	uint fragment_shader = Shader::createFragmentShader( "shader.frag" );	
	uint shaders[2];	
	shaders[0] = vertex_shader;
	shaders[1] = fragment_shader;
	uint shader_program = Shader::createShaderProgram( shaders, 2 );

	Model nanosuit = Model( "nanosuit/nanosuit.obj" );

	mat4 view = GLM_MAT4_IDENTITY_INIT;
	mat4 projection = GLM_MAT4_IDENTITY_INIT;

	v3( cam.world_up, 0.0f, 1.0f, 0.0f );
	v3( cam.position, 0.0f, 0.0f, 3.0f );
	v3( cam.front, 0.0f, 0.0f, -1.0f );
    glm_vec3_cross( cam.front, cam.world_up, cam.right );
    glm_vec3_normalize( cam.right );
    glm_vec3_cross( cam.right, cam.front, cam.up );
    glm_vec3_normalize( cam.up );
	cam.x_rot = 0.0f;
	cam.y_rot = -90.0f;
	cam.speed = 1.5f;

	mouse.x = -1;
	mouse.y = -1;
	mouse.sensitivity = 0.05f;

	glm_perspective( glm_rad( 45.0f ), window.width / window.height, 0.1f, 100.0f, projection );

	while ( running )
	{
		game_time.current = window.getMillisecondsAge() / 1000;
		game_time.delta = game_time.current - game_time.last;
		game_time.last = game_time.current;

		handleInputEvents();

		vec4 clear_color = { 0.05f, 0.05f, 0.05f, 1.0f };
		OpenGl::clearBuffers( clear_color, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		Shader::installProgram( shader_program );

		Shader::initUniformMatrix4( shader_program, "view", GL_FALSE, *view );
		Shader::initUniformMatrix4( shader_program, "projection", GL_FALSE, *projection );

		vec3 cam_target;
		glm_vec3_add( cam.position, cam.front, cam_target );
		glm_lookat( cam.position, cam_target, cam.up, view );

		mat4 model = GLM_MAT4_IDENTITY_INIT;
		vec3 position = { 0.0f, -1.75f, 0.0f };
		vec3 scale_ratio = { 0.2f, 0.2f, 0.2f };
		glm_translate( model, position );
		glm_scale( model, scale_ratio );

		Shader::initUniformMatrix4( shader_program, "model", GL_FALSE, *model );

		OpenGl::activatePolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		nanosuit.draw( shader_program );

		OpenGl::bindVertexArrayObject( 0 );

		window.updateWindow();
	}

	Shader::deleteProgram( shader_program );
	Shader::deleteShader( vertex_shader );
	Shader::deleteShader( fragment_shader );
	window.deleteWindow();

	return 0;
}