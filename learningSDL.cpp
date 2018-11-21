#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <stdio.h>
#include <assert.h>

#define ERROR -1
#define FAILED -1

SDL_Window *window = NULL;
SDL_GLContext context = NULL;
bool running = true;
int screen_width = 800;
int screen_height = 600;
float vertex[] = 
{
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};
unsigned int vertex_buffer_object;
unsigned int vertex_array_object;
unsigned int vertex_shader;
unsigned int fragment_shader;
unsigned int shader_program;


unsigned int createShader( char** shader_sources, int sources, GLenum shader_type )
{
	unsigned int new_shader;

	switch ( shader_type )
	{
		case GL_VERTEX_SHADER:
		{
			new_shader = glCreateShader( GL_VERTEX_SHADER );
			break;
		}

		case GL_FRAGMENT_SHADER:
		{
			new_shader = glCreateShader( GL_FRAGMENT_SHADER );
			break;
		}

		default:
		{
			printf( "Tipo inválido de shader. Criação abortada.\n" );
			assert(false);
			break;
		}
	}

	glShaderSource( new_shader, sources, shader_sources, NULL );

	glCompileShader( new_shader );

	int success;
	char info_log[512];
	glGetShaderiv( new_shader, GL_COMPILE_STATUS, &success );

	if ( ! success )
	{
		glGetShaderInfoLog( new_shader, 512, NULL, info_log );
		printf( "Falha ao compilar o vertex shader.\n%s\n", info_log );
		assert(false);
	}

	return new_shader;
}

unsigned int createProgram( unsigned int *shaders, int quantity )
{
	unsigned int new_program;

	new_program = glCreateProgram();

	for( int i = 0; i < quantity ; i++ )
	{
		glAttachShader( new_program, shaders[i] );
	}

	glLinkProgram( new_program );

	int success;
	char info_log[512];
	glGetProgramiv( new_program, GL_LINK_STATUS, &success );

	if ( ! success )
	{
		glGetProgramInfoLog( new_program, 512, NULL, info_log );
		printf( "Falha ao linkar o programa shader.\n%s\n", info_log );
		assert(false);
	}

	return new_program;
}

char* extractFileText( const char *path )
{
	FILE *arq = NULL;
	char *text = NULL;

	arq = fopen( path, "r" );

	if ( arq == NULL )
	{
		printf( "Falha ao abrir o arquivo %s\n", path );
		assert(false);
	}

	fseek( arq, 0, SEEK_END );
	int size = ftell( arq );
	rewind( arq );
	text = ( char* ) malloc( sizeof(char) * ( size + 1 ) );

	fread( text, size, 1, arq );
	text[size] = '\0';

	fclose( arq );

	return text;
}

void initializeSDLContext()
{
	if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) == FAILED )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		assert(false);
	}

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	if ( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 2048 ) == FAILED )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		assert(false);
	}

	{
		const char title[] = "Learning SDL2";
		int x_pos = SDL_WINDOWPOS_UNDEFINED;
		int y_pos = SDL_WINDOWPOS_UNDEFINED;
		int width = screen_width;
		int height = screen_height;
		Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

		window = SDL_CreateWindow( title, x_pos, y_pos, width, height, flags );
	}

	if ( window == NULL )
	{
		printf( "SDL coult not create window! SDL_Error: %s\n", SDL_GetError() );
		assert(false);
	}

	context = SDL_GL_CreateContext(window);

	if ( context == NULL )
	{
		printf( "SDL coult not create GL context! SDL_Error: %s\n", SDL_GetError() );
		assert(false);
	}

	if ( ! gladLoadGLLoader( ( GLADloadproc ) SDL_GL_GetProcAddress ) )
	{
		printf( "Failed to initialize GLAD\n" );
		assert(false);
	}

	glViewport( 0, 0, screen_width, screen_height );

	//SDL_GL_SetSwapInterval(1);
}

void destroySDLContext()
{
	SDL_GL_DeleteContext( context );
	SDL_DestroyWindow( window );
	Mix_Quit();
	SDL_Quit();
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
						screen_width = event.window.data1;
						screen_height = event.window.data2;
						glViewport( 0, 0, screen_width, screen_height );
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

void soundFunctions()
{
	Mix_Chunk *short_sound = Mix_LoadWAV( "soundpath" );
	Mix_Music *long_sound  = Mix_LoadMUS( "soundpath" );

	Mix_PlayChannel( -1, short_sound, 0 );
	Mix_PlayMusic( long_sound, -1 );
	Mix_PlayingMusic();
	Mix_ResumeMusic();
	Mix_PauseMusic();
	Mix_HaltMusic();

	Mix_FreeChunk( short_sound );
	Mix_FreeMusic( long_sound );
}

int main( int argc, char* args[] )
{
	initializeSDLContext();

	char *vertex_shader_source = extractFileText( "shader.vert" );
	vertex_shader = createShader( &vertex_shader_source, 1, GL_VERTEX_SHADER );
	free( vertex_shader_source );

	char *fragment_shader_source = extractFileText( "shader.frag" );
	fragment_shader = createShader( &fragment_shader_source, 1, GL_FRAGMENT_SHADER );
	free( fragment_shader_source );

	unsigned int shaders[2];
	shaders[0] = vertex_shader;
	shaders[1] = fragment_shader;

	shader_program = createProgram( shaders, 2 );

	glDeleteShader( vertex_shader );
	glDeleteShader( fragment_shader );

	{
		unsigned int num_buffers = 1;
		glGenVertexArrays( num_buffers, &vertex_array_object );
	}
	glBindVertexArray( vertex_array_object );
	{
		unsigned int num_buffers = 1;
		glGenBuffers( num_buffers, &vertex_buffer_object );
	}
	glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer_object );
	glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 9, vertex, GL_STATIC_DRAW );

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), ( void* ) 0 );
	glEnableVertexAttribArray( 0 );

	while ( running )
	{
		handleInputEvents();

		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		glUseProgram( shader_program );
		glBindVertexArray( vertex_array_object );
		glDrawArrays( GL_TRIANGLES, 0, 3 );

		SDL_GL_SwapWindow( window );
	}

	destroySDLContext();

	return 0;
}