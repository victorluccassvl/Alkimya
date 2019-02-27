// Class_______________________________________________________________________________________________________________
class Mesh
{
	public:
	uint VAO, VBO, EBO;
	uint vertices, indices, textures;
	Vertex  *vertex;
	uint 	*indice;
	Texture *texture;

	Mesh( Vertex *vertex, uint vertices, uint *indice, uint indices, Texture *texture, uint textures );

	void draw( uint shader );
	void setup();
};


// Constructors________________________________________________________________________________________________________
Mesh::Mesh( Vertex *vertex, uint vertices, uint *indice, uint indices, Texture *texture, uint textures )
{
	this->vertices = vertices;
	this->indices  = indices;
	this->textures = textures;
	this->vertex   = vertex;
	this->indice   = indice;
	this->texture  = texture;

	this->setup();
}


// Methods_____________________________________________________________________________________________________________
void Mesh::draw( uint shader )
{
	for( uint i = 0 ; i < this->textures ; i++ )
	{
		char texture_variable[100] = { "material_texture_\0" };
		char number[10];
		
		uint diffuse_textures  = 0;
		uint specular_textures = 0;

		switch( this->texture[i].type )
		{
			case DIFFUSE:
			{
				diffuse_textures++;
				strcat( texture_variable, "diffuse\0" );
				sprintf( number, "%d", diffuse_textures );
				strcat( texture_variable, number );
				break;
			}
			case SPECULAR:
			{
				specular_textures++;
				strcat( texture_variable, "specular\0" );
				sprintf( number, "%d", specular_textures );
				strcat( texture_variable, number );
				break;
			}
			default:
			{
				printf( "While drawing mesh, unnexpected texture type.\n" );
				STOP;
			}
		}

		Shader::initUniformTexture( shader, texture_variable, i );

		OpenGl::bind2DTextureToUnit( GL_TEXTURE0 + i, this->texture[i].id );
	}

	OpenGl::bindVertexArrayObject( this->VAO );

	OpenGl::drawElements( GL_TRIANGLES, this->indices );

	OpenGl::bindVertexArrayObject( 0 );
}

void Mesh::setup()
{

	this->VAO = OpenGl::createVertexArrayObject();
	this->VBO = OpenGl::createBuffer();
	this->EBO = OpenGl::createBuffer();

	OpenGl::bindVertexArrayObject( this->VAO );

	OpenGl::bindVertexBufferObject( this->VBO );
	OpenGl::fillVertexBufferObject( this->VAO, this->VBO, this->vertices * sizeof( Vertex ), &this->vertex[0], GL_STATIC_DRAW );

	OpenGl::bindElementBufferObject( this->EBO );

	OpenGl::fillElementBufferObject( this->VAO, this->VBO, this->indices * sizeof( uint ), &this->indice[0], GL_STATIC_DRAW );

	// position
	OpenGl::defineAttribFormat( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void* ) 0 );

	// normal
	OpenGl::defineAttribFormat( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void* ) offsetof( Vertex, normal ) );

	//texture coords
	OpenGl::defineAttribFormat( 2, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void* ) offsetof( Vertex, texture_coords ) );

	OpenGl::bindVertexArrayObject( 0 );
}
