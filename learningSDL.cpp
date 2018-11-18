#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <assert.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

#define ERROR -1
#define FAILED -1

SDL_Window *window = NULL;
SDL_Surface *window_surface = NULL;
bool running = true;


void initializeSDLContext()
{
	if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) == FAILED )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		assert(false);
	}
	else
	{
		if ( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 2048 ) == FAILED )
		{
			printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
			assert(false);
		}

		{
			const char title[] = "Learning SDL2";
			int x_pos = SDL_WINDOWPOS_UNDEFINED;
			int y_pos = SDL_WINDOWPOS_UNDEFINED;
			int width = SCREEN_WIDTH;
			int height = SCREEN_HEIGHT;
			Uint32 flags = SDL_WINDOW_SHOWN;

			window = SDL_CreateWindow( title, x_pos, y_pos, width, height, flags );
		}

		if ( window == NULL )
		{
			printf( "SDL coult not create window! SDL_Error: %s\n", SDL_GetError() );
			assert(false);
		}
	}
}

void destroySDLContext()
{
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

	window_surface = SDL_GetWindowSurface( window );
	{
		SDL_Rect *dest_rectangle = NULL;
		Uint32 color = SDL_MapRGB( window_surface->format, 0xFF, 0xFF, 0xFF );
		SDL_FillRect( window_surface, dest_rectangle, color );
	}

	SDL_UpdateWindowSurface( window );

	while ( running )
	{
		handleInputEvents();
	}

	destroySDLContext();

	return 0;
}