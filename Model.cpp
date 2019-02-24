enum TextureType
{
	SPECULAR, DIFFUSE
};

class Texture
{
	public:
		TextureType type;
		uint id;
		int width;
		int height;
		char path[_MODEL_MAX_PATH_SIZE];
};


class Vertex
{
	public:
		vec3 position;
		vec3 normal;
		vec2 texture_coords;

    	Vertex( vec3 position, vec3 normal, vec3 texture_coords );
};

Vertex::Vertex( vec3 position, vec3 normal, vec3 texture_coords )
{
	v3( this->position,             position[0],       position[1],  position[2] );
	v3( this->normal,                 normal[0],         normal[1],    normal[2] );
	v2( this->texture_coords, texture_coords[0], texture_coords[1] );
}


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

void Mesh::draw( uint shader )
{
	char texture_variable[100] = { "material_texture_\0" };
	char number[100];

	for( uint i = 0 ; i < this->textures ; i++ )
	{
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
    	printf( "a%d\n", glGetError() );
}


class Model
{
	public:
		Mesh *mesh;
		int meshes;
		char root_directory[_MODEL_MAX_PATH_SIZE];

		Model( const char *path );

		void draw( uint shader );
		void load( const char *path );
		void processAssimpInput( aiNode *node, const aiScene *model, int *current );
		Mesh processAssimpMesh( aiMesh *mesh, const aiScene *scene );
		void loadMaterialTextures( aiMaterial *material, aiTextureType assimp_type, TextureType type, Texture *texture, uint *textures );
};

Model::Model( const char *path )
{
	this->load( path );
}

void Model::draw( uint shader )
{
	for( int i = 0 ; i < this->meshes ; i++ )
	{
		this->mesh[i].draw( shader );
	}
}

void Model::load( const char *path )
{
	Assimp::Importer importer;
	const aiScene *model = importer.ReadFile( path, aiProcess_Triangulate | aiProcess_FlipUVs );

	if ( model == NULL || model->mFlags & AI_SCENE_FLAGS_INCOMPLETE || model->mRootNode == NULL )
	{
		printf( "Assimp could not load model! Assimp Error: %s\n", importer.GetErrorString() );
		STOP;
	}

	this->meshes = model->mNumMeshes;
	this->mesh   = ( Mesh* ) malloc( sizeof( Mesh ) * this->meshes );
	strcpy( this->root_directory, path );
	char *directory_end = strrchr( this->root_directory, '/' );
	directory_end[1] = '\0';

	int current_node = 0;

	this->processAssimpInput( model->mRootNode, model, &current_node );

	if ( current_node != this->meshes )
	{
		printf( "Something wrong while processing model\n" );
		STOP;
	}
}

void Model::processAssimpInput( aiNode *node, const aiScene *model, int *current )
{
	for( uint i = 0 ; i < node->mNumMeshes ; i++ )
	{
		aiMesh *mesh = model->mMeshes[node->mMeshes[i]];
		this->mesh[( *current )++] = this->processAssimpMesh( mesh, model );
	}

	for( uint i = 0 ; i < node->mNumChildren ; i++ )
	{
		this->processAssimpInput( node->mChildren[i], model, current );
	}
}

Mesh Model::processAssimpMesh( aiMesh *mesh, const aiScene *scene )
{
	Vertex  *vertex  = ( Vertex* ) malloc( sizeof( Vertex ) * mesh->mNumVertices  );
	uint    *indice  = ( uint* )   malloc( sizeof( uint )   * mesh->mNumFaces * 3 );
	Texture *texture = NULL;

	uint vertices = 0;
	uint indices  = 0;
	uint textures = 0;

	// Processing Vertices
	for( uint i = 0 ; i < mesh->mNumVertices ; i++ )
	{
		vec3 position;
		vec3 normal;
		vec2 texture_coords;
		v3( position, mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z  );
		v3( normal,    mesh->mNormals[i].x,   mesh->mNormals[i].y,  mesh->mNormals[i].z );
		if ( mesh->mTextureCoords[0] != NULL )
		{
			v2( texture_coords, mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y );
		}
		else
		{
			v2( texture_coords, 0, 0 );
		}

		vertex[vertices++] = Vertex( position, normal, texture_coords );
	}

	// Processing Indices
	for( uint i = 0 ; i < mesh->mNumFaces ; i++ )
	{
		aiFace face = mesh->mFaces[i];
		for( int j = 0 ; j < 3 ; j++ )
		{
			indice[indices++] = face.mIndices[j];
		}
	}

	// Processing Textures
	if ( mesh->mMaterialIndex >= 0 )
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

		int total_textures = material->GetTextureCount( aiTextureType_DIFFUSE );
		total_textures += material->GetTextureCount( aiTextureType_SPECULAR );

		texture = ( Texture* ) malloc( sizeof( Texture ) * total_textures );

		this->loadMaterialTextures( material, aiTextureType_DIFFUSE, DIFFUSE, texture, &textures );
		this->loadMaterialTextures( material, aiTextureType_SPECULAR, SPECULAR, texture, &textures );
	}
 
	return Mesh( vertex, vertices, indice, indices, texture, textures );
}

void Model::loadMaterialTextures( aiMaterial *material, aiTextureType assimp_type, TextureType type, Texture *texture, uint *textures )
{
    for( uint i = 0 ; i < material->GetTextureCount( assimp_type ) ; i++ )
    {
    	Texture new_texture;

    	// Create full path ( root directory + relative file path )
        aiString relative_path;
        material->GetTexture( assimp_type, i, &relative_path );
        strcpy( new_texture.path, this->root_directory );
        // Initialize texture Path
        strcat( new_texture.path, relative_path.C_Str() );
        // Initialize texture ID, Width and Height
        new_texture.id = SDLGlWindow::createGlTexture( new_texture.path, &new_texture.width, &new_texture.height );
        // Initialize texture Type
        new_texture.type = type;

        texture[( *textures )++] = new_texture;
    }
}