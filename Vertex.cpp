// Class_______________________________________________________________________________________________________________
class Vertex
{
	public:
	vec3 position;
	vec3 normal;
	vec2 texture_coords;

	Vertex( vec3 position, vec3 normal, vec3 texture_coords );
};


// Constructors________________________________________________________________________________________________________
Vertex::Vertex( vec3 position, vec3 normal, vec3 texture_coords )
{
	v3( this->position,             position[0],       position[1],  position[2] );
	v3( this->normal,                 normal[0],         normal[1],    normal[2] );
	v2( this->texture_coords, texture_coords[0], texture_coords[1] );
}
