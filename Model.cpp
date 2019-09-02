// Class_______________________________________________________________________________________________________________
class Model
{
	public:
	char root_directory[_MODEL_MAX_PATH_SIZE];
	Mesh *mesh;
	int   meshes;
	Texture **loaded_texture;
	int loaded_textures;
	int max_textures;

	Model( const char *path );

	void draw( uint shader );
	void load( const char *path );

	void processAssimpInput( aiNode *node, const aiScene *model, int *current );
    void processAssimpVertices( aiMesh *mesh, Vertex *vertex );
	void processAssimpIndices( aiMesh *mesh, uint *indice );
	void processAssimpTextures( aiMesh *mesh, const aiScene *model, Texture **texture, uint *textures );
	Mesh processAssimpMesh( aiMesh *mesh, const aiScene *scene );
	void loadMaterialTextures( aiMaterial *material, aiTextureType assimp_type, TextureType type, Texture *texture, uint *textures );
};


// Constructors________________________________________________________________________________________________________
Model::Model( const char *path )
{
	this->mesh = NULL;
	this->meshes = 0;
	this->loaded_texture = ( Texture** ) malloc( sizeof( Texture* ) * _MODEL_INITIAL_MAX_TEXTURES );
	this->loaded_textures = 0;
	this->max_textures = _MODEL_INITIAL_MAX_TEXTURES;
	this->load( path );
}


// Methods_____________________________________________________________________________________________________________
void Model::draw( uint shader )
{
	for( int i = 0 ; i < this->meshes ; i++ )
	{
		this->mesh[i].draw( shader );
	}
}

void Model::load( const char *path )
{
	// Load assimp structure and treat errors
	Assimp::Importer loader;
	const aiScene *model = loader.ReadFile( path, aiProcess_Triangulate | aiProcess_FlipUVs );
	if ( model == NULL || model->mFlags & AI_SCENE_FLAGS_INCOMPLETE || model->mRootNode == NULL )
	{
		printf( "Assimp could not load model! Assimp Error: %s\n", loader.GetErrorString() );
		STOP;
	}

	// Prepare to recieve meshes
	this->meshes = model->mNumMeshes;
	this->mesh   = ( Mesh* ) malloc( sizeof( Mesh ) * this->meshes );

	// Initialize root directory variable
	strcpy( this->root_directory, path );
	char *root_directory_end = strrchr( this->root_directory, '/' );
	root_directory_end[1] = '\0';

	// Process recursive assimp structure
	int current_node = 0;
	this->processAssimpInput( model->mRootNode, model, &current_node );
}

void Model::processAssimpInput( aiNode *node, const aiScene *model, int *current )
{
	// Process current assimp node's meshes
	for( uint i = 0 ; i < node->mNumMeshes ; i++ )
	{
		aiMesh *mesh = model->mMeshes[node->mMeshes[i]];
		this->mesh[( *current )++] = this->processAssimpMesh( mesh, model );
	}

	// Process current assimp children
	for( uint i = 0 ; i < node->mNumChildren ; i++ )
	{
		this->processAssimpInput( node->mChildren[i], model, current );
	}
}

void Model::processAssimpVertices( aiMesh *mesh, Vertex *vertex )
{
	for( uint i = 0 ; i < mesh->mNumVertices ; i++ )
	{
		vec3 position;
		vec3 normal;
		vec2 texture_coords;

		// Position
		v3( position, mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z  );
		
		// Normal
		v3( normal,    mesh->mNormals[i].x,   mesh->mNormals[i].y,  mesh->mNormals[i].z );
		
		// Texture Coords
		if ( mesh->mTextureCoords[0] != NULL )
		{
			v2( texture_coords, mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y );
		}
		else
		{
			v2( texture_coords, 0, 0 );
		}

		vertex[i] = Vertex( position, normal, texture_coords );
	}
}

void Model::processAssimpIndices( aiMesh *mesh, uint *indice )
{
	for( uint i = 0 ; i < mesh->mNumFaces ; i++ )
	{
		aiFace face = mesh->mFaces[i];
		for( int j = 0 ; j < 3 ; j++ )
		{
			indice[i*3 + j] = face.mIndices[j];
		}
	}
}

void Model::processAssimpTextures( aiMesh *mesh, const aiScene *model, Texture **texture, uint *textures )
{
	// Se existe textura
	if ( mesh->mMaterialIndex >= 0 )
	{
		aiMaterial *material = model->mMaterials[mesh->mMaterialIndex];

		// Accumulate all texture types
		int total_textures = 0;
		total_textures += material->GetTextureCount( aiTextureType_DIFFUSE );
		total_textures += material->GetTextureCount( aiTextureType_SPECULAR );

		// If there can't be enough space for these textures, on the worst scenario ( none of them are already loaded )
		if ( total_textures + this->loaded_textures > this->max_textures )
		{
			// Realoc loaded texture array with suficiente size * 2
			this->max_textures = ( total_textures + this->loaded_textures ) * 2;

			Texture **new_max = ( Texture** ) malloc( sizeof( Texture* ) * this->max_textures );
			for( int j = 0 ; j < this->loaded_textures ; j++ )
			{
				new_max[j] = this->loaded_texture[j];
			}

			free( this->loaded_texture );

			this->loaded_texture = new_max;
		}

		// Aloc current node textures
		*texture = ( Texture* ) malloc( sizeof( Texture ) * total_textures );

		// Load textures
		this->loadMaterialTextures( material, aiTextureType_DIFFUSE, DIFFUSE, *texture, textures );
		this->loadMaterialTextures( material, aiTextureType_SPECULAR, SPECULAR, *texture, textures );
	}
}

Mesh Model::processAssimpMesh( aiMesh *mesh, const aiScene *model )
{
	uint vertices = mesh->mNumVertices;
	uint indices  = mesh->mNumFaces * 3;
	uint textures = 0;

	Vertex  *vertex  = ( Vertex* ) malloc( sizeof( Vertex ) * vertices );
	uint    *indice  = ( uint* )   malloc( sizeof( uint )   * indices  );
	Texture *texture = NULL;

	this->processAssimpVertices( mesh, vertex );
	this->processAssimpIndices ( mesh, indice );
	this->processAssimpTextures( mesh, model, &texture, &textures );

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

        bool already_loaded = false;

        for( int j = 0 ; j < this->loaded_textures && !already_loaded ; j++ )
        {
        	if ( strcmp( this->loaded_texture[j]->path, new_texture.path ) == 0 )
        	{
        		already_loaded = true;
        		texture[( *textures )++] = *( this->loaded_texture[j] );
        	}
        }

        if ( !already_loaded )
        {
	        // Initialize texture ID, Width and Height
	        new_texture.id = SDLGlWindow::createGlTexture( new_texture.path, &new_texture.width, &new_texture.height );
	        // Initialize texture Type
	        new_texture.type = type;

	        this->loaded_texture[this->loaded_textures++] = &texture[( *textures )];
	        texture[( *textures )++] = new_texture;
        }
    }
}