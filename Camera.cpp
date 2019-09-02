// Class_______________________________________________________________________________________________________________
class Camera
{
	public:
	mat4 view;
	mat4 projection;
	mat4 model;
	vec3 position;
	vec3 front;
	vec3 right;
	vec3 up;
	vec3 world_up;
	float x_rotation;
	float y_rotation;
	float speed;
	float field_of_vision;
	int *window_width;
	int *window_height;
	float projection_near_pane;
	float projection_far_pane;


	Camera( vec3 position, vec3 front, vec3 up, float speed, int *window_width, int *window_height );

	void initProjection();
	void initView();
	void initModel( vec3 translate, vec3 scale, vec3 rotate_axis, float rotate_angle );
};


// Constructors________________________________________________________________________________________________________
Camera::Camera( vec3 position, vec3 front, vec3 up, float speed, int *window_width, int *window_height )
{
	v3( this->position, position   );
	v3( this->front,    front      );
	v3( this->up,       up         );
	v3( world_up, 0.0f, 1.0f, 0.0f );

	this->window_height = window_height;
	this->window_width  = window_width;

	glm_vec3_normalize( this->front );
	glm_vec3_normalize( this->up    );

	glm_vec3_cross( this->front, this->up, this->right );

	this->x_rotation = 0.0f;
	this->y_rotation = 0.0f;

	this->field_of_vision      = 45.0f;
	this->projection_near_pane = 0.1f;
	this->projection_far_pane  = 100.0f;

	this->speed = speed;
}


// Methods_____________________________________________________________________________________________________________
void Camera::initProjection()
{
	float screen_ratio = ( ( float ) ( *this->window_width ) ) /  ( *this->window_height );
	float radian_fov = glm_rad( this->field_of_vision );

	glm_perspective( radian_fov, screen_ratio, this->projection_near_pane, this->projection_far_pane, this->projection );
}

void Camera::initView()
{
	vec3 target;
	glm_vec3_add( this->position, this->front, target );
	glm_lookat( this->position, target, this->up, this->view );
}

void Camera::initModel( vec3 translate, vec3 scale, vec3 rotate_axis, float rotate_angle )
{
	glm_mat4_identity( this->model );
	glm_translate( this->model, translate );
	//glm_rotate( this->model, rotate_angle, rotate_axis );
	glm_scale( this->model, scale );
}