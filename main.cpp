#include "SDLGlWindow.cpp"
#include "OpenGl.cpp"

#include <stdio.h>

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

float vertex[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

SDLGlWindow window;
Camera cam;
Mouse mouse;
Time time;
bool running = true;

inline void v3x( vec3 vec, float x )
{
	vec[0] = x;
}

inline void v3y( vec3 vec, float y )
{
	vec[1] = y;
}

inline void v3z( vec3 vec, float z )
{
	vec[2] = z;
}

inline void v3all( vec3 vec, float x, float y, float z )
{
	v3x( vec, x );
	v3y( vec, y );
	v3z( vec, z );
}



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
						glm_vec3_scale( cam.front, cam.speed * time.delta, scaled );
						glm_vec3_add( cam.position, scaled, cam.position );
						break;
					}
					case ( SDLK_s ):
					{
						vec3 scaled;
						glm_vec3_scale( cam.front, cam.speed * time.delta, scaled );
						glm_vec3_sub( cam.position, scaled, cam.position );
						break;
					}
					case ( SDLK_a ):
					{
						vec3 scaled;
						glm_vec3_scale( cam.right, cam.speed * time.delta, scaled );
						glm_vec3_sub( cam.position, scaled, cam.position );
						break;
					}
					case ( SDLK_d ):
					{
						vec3 scaled;
						glm_vec3_scale( cam.right, cam.speed * time.delta, scaled );
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

	uint vertex_shader  = OpenGl::createVertexShader( "shader.vert" );
	uint fragment_shader = OpenGl::createFragmentShader( "shader.frag" );	
	uint shaders[2];
	shaders[0] = vertex_shader;
	shaders[1] = fragment_shader;
	uint shader_program = OpenGl::createShaderProgram( shaders, 2 );
		
	uint VAO = OpenGl::createVertexArrayObject();
	OpenGl::bindVertexArrayObject( VAO );
	uint VBO = OpenGl::createBuffer();
	OpenGl::bindVertexBufferObject( VBO );
	uint EBO = OpenGl::createBuffer();
	OpenGl::bindElementBufferObject( EBO );

	OpenGl::fillVertexBufferObject( VAO, VBO, sizeof( float ) * 180, vertex, GL_STATIC_DRAW );
	//OpenGl::fillElementBufferObject( VAO, VBO, sizeof( uint ) * 6, indces, GL_STATIC_DRAW );
	
	OpenGl::defineAttribFormat( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), 0 );
	OpenGl::defineAttribFormat( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), ( const void* ) ( 3 * sizeof( float ) ) );
	//OpenGl::defineAttribFormat( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), 0 );

	int width1, height1;
	int width2, height2;
	float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	uint texture1 = window.createGlTexture( "container.jpg", &width1, &height1, 3 );
	glTexParameteri(  GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
	glTexParameteri(  GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
	glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color );
	glTexParameteri(  GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
	glTexParameteri(  GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	uint texture2 = window.createGlTexture( "awesomeface.png", &width2, &height2, 4 );
	glTexParameteri(  GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
	glTexParameteri(  GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
	glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color );
	glTexParameteri(  GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
	glTexParameteri(  GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	OpenGl::bind2DTextureToUnit( GL_TEXTURE0, texture1 );
	OpenGl::bind2DTextureToUnit( GL_TEXTURE1, texture2 );
///////////////////////////////////////////////////////////////////////////////////////////////////
	mat4 view = GLM_MAT4_IDENTITY_INIT;
	mat4 projection = GLM_MAT4_IDENTITY_INIT;

	vec3 cube_positions[] = {
		{ 0.0f, 0.0f, 0.0f },
		{ 2.0f, 5.0f, -15.0f },
		{ -1.5f, -2.2f, -2.5f },
		{ -3.8f, -2.0f, -12.3f },
		{ 2.4f, -0.4f, -3.5f },
		{ -1.7f, 3.0f, -7.5f },
		{ 1.3f, -2.0f, -2.5f },
		{ 1.5f, 2.0f, -2.5f },
		{ 1.5f, 0.2f, -1.5f },
		{ -1.3f, 1.0f, -1.5f }
	};


	v3all( cam.world_up, 0.0f, 1.0f, 0.0f );
	v3all( cam.position, 0.0f, 0.0f, 3.0f );
	v3all( cam.front, 0.0f, 0.0f, -1.0f );
    glm_vec3_cross( cam.front, cam.world_up, cam.right );
    glm_vec3_normalize( cam.right );
    glm_vec3_cross( cam.right, cam.front, cam.up );
    glm_vec3_normalize( cam.up );
	cam.x_rot = 0.0f;
	cam.y_rot = -90.0f;
	cam.speed = 2.5f;

	mouse.x = -1;
	mouse.y = -1;
	mouse.sensitivity = 0.05f;

	glm_perspective( glm_rad( 45.0f ), window.width / window.height, 0.1f, 100.0f, projection );
///////////////////////////////////////////////////////////////////////////////////////////////////

	while ( running )
	{
		time.current = window.getMillisecondsAge() / 1000;
		time.delta = time.current - time.last;
		time.last = time.current;

		handleInputEvents();

		vec4 clear_color = { 0.2f, 0.3f, 0.3f, 1.0f };
		OpenGl::clearBuffers( clear_color, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		OpenGl::installShaderProgram( shader_program );

		OpenGl::initUniformMatrix4( shader_program, "view", GL_FALSE, view[0] );
		OpenGl::initUniformMatrix4( shader_program, "projection", GL_FALSE, projection[0] );
		OpenGl::initUniformTexture( shader_program, "text1", 0 );
		OpenGl::initUniformTexture( shader_program, "text2", 1 );

		OpenGl::bindVertexArrayObject( VAO );

		vec3 cam_target;
		glm_vec3_add( cam.position, cam.front, cam_target );
		glm_lookat( cam.position, cam_target, cam.up, view );

		for( unsigned int i = 0; i < 10; i++ )
		{
			mat4 model = GLM_MAT4_IDENTITY_INIT;
			glm_translate( model, cube_positions[i] );

			OpenGl::initUniformMatrix4( shader_program, "model", GL_FALSE, model[0] );

			OpenGl::activatePolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			OpenGl::drawArrays( GL_TRIANGLES, 36 );
		}

		OpenGl::bindVertexArrayObject( 0 );

		window.updateWindow();
	}


	OpenGl::deleteShaderProgram( shader_program );
	OpenGl::deleteShader( vertex_shader );
	OpenGl::deleteShader( fragment_shader );
	window.deleteWindow();

	return 0;
}