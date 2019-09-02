// Related Data Structure______________________________________________________________________________________________
struct SDL_State
{
	const Uint8 *keyboard;
	Uint32 mouse;
	int x;
	int y;
};


// Class_______________________________________________________________________________________________________________
class SDLGlWindow
{
	public:
	SDL_Window *window;
	SDL_GLContext context;
	SDL_State state;
	SDL_Event *event;
	bool context_exists;
	static bool image_initialized;
	static bool audio_initialized;

	char title[_SDL_WINDOW_MAX_TITLE_NAME];
	int x_pos;
	int y_pos;
	int width;
	int height;

	SDLGlWindow();

	void  deleteWindow();

	void  initializeWindow();
	void  initializeGlContext();

	static void initializeImage();
	static void initializeAudio();

	static uint createGlTexture( const char *texture_source, int *width, int *height );

	static void soundFunctions();

	static float getMillisecondsAge();

	void  updateWindow();

	bool  getEvent();
	void  getState();
};

bool SDLGlWindow::image_initialized = false;
bool SDLGlWindow::audio_initialized = false;


// Constructors________________________________________________________________________________________________________
SDLGlWindow::SDLGlWindow()
{
	this->window = NULL;
	this->context_exists = false;
	strcpy( this->title, _SDL_WINDOW_DEFAULT_TITLE );
	this->x_pos  = _SDL_WINDOW_DEFAULT_X_POS;
	this->x_pos  = _SDL_WINDOW_DEFAULT_Y_POS;
	this->width  = _SDL_WINDOW_DEFAULT_WIDTH;
	this->height = _SDL_WINDOW_DEFAULT_HEIGHT;
	this->event  = NULL;
}


// Methods_____________________________________________________________________________________________________________
void SDLGlWindow::deleteWindow()
{
	if ( this->context_exists )
	{
		SDL_GL_DeleteContext( context );
	}

	if ( this->window != NULL )
	{
		SDL_DestroyWindow( window );
	}

	while ( Mix_Init( 0 ) )
	{
		Mix_Quit();
	}

	IMG_Quit();

	SDL_Quit();
}

void SDLGlWindow::initializeWindow()
{
	if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) == FAILED )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		STOP;
	}

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, _GL_MAJOR_VERSION );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, _GL_MINOR_VERSION );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	this->window = SDL_CreateWindow( this->title, this->x_pos, this->y_pos, this->width, this->height, _SDL_WINDOW_FLAGS );

	if ( this->window == NULL )
	{
		printf( "SDL coult not create window! SDL_Error: %s\n", SDL_GetError() );
		STOP;
	}
}

void SDLGlWindow::initializeGlContext()
{
	if ( this->window == NULL )
	{
		printf( "SDL tried to initialize GL context without window\n" );
		STOP;
	}

	this->context = SDL_GL_CreateContext( this->window );

	if ( this->context == NULL )
	{
		printf( "SDL could not create GL context! SDL_Error: %s\n", SDL_GetError() );
		STOP;
	}

	if ( ! gladLoadGLLoader( ( GLADloadproc ) SDL_GL_GetProcAddress ) )
	{
		printf( "Failed to initialize GLAD\n" );
		STOP;
	}

	glViewport( this->x_pos, this->y_pos, this->width, this->height );

	this->context_exists = true;
}

void SDLGlWindow::initializeImage()
{
	if ( ( IMG_Init( _SDL_IMG_FLAGS ) & _SDL_IMG_FLAGS ) != _SDL_IMG_FLAGS )
	{
		printf( "SDL image could not initialize! SDL_Error: %s\n", IMG_GetError() );
		STOP;
	}

	SDLGlWindow::image_initialized = true;
}

void SDLGlWindow::initializeAudio()
{
	if ( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 2048 ) == FAILED )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		STOP;
	}

	SDLGlWindow::audio_initialized = true;
}

uint SDLGlWindow::createGlTexture( const char *texture_source, int *width, int *height )
{

	// Verify if image loader is up
	if ( ! SDLGlWindow::image_initialized )
	{
		printf( "SDL image not initialized, it can't create texture %s\n", texture_source );
		STOP;
	}

	// Try to load the texture image
	SDL_Surface *image = IMG_Load( texture_source );
	uint new_texture;

	if ( image == NULL )
	{
		printf( "SDL image could not load texture %s SDL_Error: %s\n", texture_source, IMG_GetError() );
		STOP;
	}

	// Verify if it has a valid type
	uint channels = image->format->BytesPerPixel;
 
	bool valid_data = channels == 3 || channels == 4;
	if ( ! valid_data )
	{
		printf( "Invalid type of image data, texture can't be created\n" );
		STOP;
	}

	// Create the texture
	*width = image->w;
	*height = image->h;

	glGenTextures( 1, &new_texture );
	glBindTexture( GL_TEXTURE_2D, new_texture );

	switch ( channels )
	{
		case 3:
		{
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, *width, *height, 0, GL_RGB, GL_UNSIGNED_BYTE, ( unsigned char* ) image->pixels );
			break;
		}
		case 4:
		{
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ( unsigned char* ) image->pixels );
			break;
		}
	}

	glGenerateMipmap( GL_TEXTURE_2D );

	SDL_FreeSurface( image );

	return new_texture;
}

void SDLGlWindow::soundFunctions()
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

float SDLGlWindow::getMillisecondsAge()
{
	return ( float ) SDL_GetTicks();
}

void SDLGlWindow::updateWindow()
{
	if ( this->window == NULL )
	{
		printf( "SDL cannot update window that doesn't exist\n" );
		STOP;
	}
	SDL_GL_SwapWindow( this->window );
}

bool SDLGlWindow::getEvent()
{
	if ( SDL_PollEvent( this->event ) == 0 )
	{
		return false;
	}
	else
	{
		return true;
	}
}

void SDLGlWindow::getState()
{

	// SDL_BUTTON(X) returns a mask to be tested with :
	// SDL_BUTTON_LEFT, SDL_BUTTON_RIGHT, SDL_BUTTON_MIDDLE
	// SDL_BUTTON_X1, SDL_BUTTON_X2

	// SDL_SCANCODE for keyboard tests

	SDL_PumpEvents();
	this->state.mouse = SDL_GetMouseState( &( this->state.x ), &( this->state.y ) );
	this->state.keyboard = SDL_GetKeyboardState( NULL );
}