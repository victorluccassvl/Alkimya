#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 frag_color;

in vec3 normal;
in vec3 position;

uniform vec3 cam_position;
uniform Material material;
uniform Light light;

void main()
{
	// Ambient light
	vec3 ambient_light = light.ambient * material.ambient;

	// Diffuse light
	vec3 light_direction   = normalize( light.position - position );
	float diffuse_constant = dot( normalize( normal ), light_direction );
	diffuse_constant       = max( diffuse_constant, 0.0 );
	vec3 diffuse_light     = ( light.diffuse * material.diffuse ) * diffuse_constant;

	// Specular light
	vec3 cam_direction        = normalize( cam_position - position );
	vec3 reflection_direction = reflect( -light_direction, normalize( normal ) );
	float specular_constant   = max( dot( cam_direction, reflection_direction ), 0.0 );
	specular_constant         = pow( specular_constant, material.shininess );
	vec3 specular_light       = ( light.specular * material.specular ) * specular_constant;

	// Output
	vec3 result = ambient_light + diffuse_light + specular_light;
	frag_color = vec4( result, 1.0 );
}
