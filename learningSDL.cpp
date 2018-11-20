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

	while ( running )
	{
		handleInputEvents();

//


		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );
		SDL_GL_SwapWindow( window );
	}

	destroySDLContext();

	return 0;
}