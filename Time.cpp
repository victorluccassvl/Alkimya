// Class_______________________________________________________________________________________________________________
class Time
{
	public:
	float current;
	float last;
	float delta;
	SDLGlWindow *window;

	Time( SDLGlWindow *window );

	void update();
};


// Constructors________________________________________________________________________________________________________
Time::Time( SDLGlWindow *window )
{
	this->window  = window;
	this->current = window->getMillisecondsAge() / 1000;
	this->last    = this->current;
	this->delta   = 0.0f;
}


// Methods_____________________________________________________________________________________________________________
void Time::update()
{
	this->last    = this->current;
	this->current = this->window->getMillisecondsAge() / 1000;
	this->delta   = this->current - this->last;
}