class Model
{
	public:
		Mesh *mesh;
		int meshes;
		char root_directory[_MODEL_MAX_PATH_SIZE];
		Texture **loaded_texture;
		int loaded_textures;
		int max_textures;

		Model( const char *path );

		void draw( uint shader );
		void load( const char *path );
		void processAssimpInput( aiNode *node, const aiScene *model, int *current );
		Mesh processAssimpMesh( aiMesh *mesh, const aiScene *scene );
		void loadMaterialTextures( aiMaterial *material, aiTextureType assimp_type, TextureType type, Texture *texture, uint *textures );
};

Model::Model( const char *path )
{
	this->mesh = NULL;
	this->meshes = 0;
	this->loaded_texture = ( Texture** ) malloc( sizeof( Texture* ) * _MODEL_INITIAL_MAX_TEXTURES );
	this->loaded_textures = 0;
	this->max_textures = _MODEL_INITIAL_MAX_TEXTURES;
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

		if ( total_textures + this->loaded_textures > this->max_textures )
		{
			this->max_textures = total_textures + this->loaded_textures;

			Texture **new_max = ( Texture** ) malloc( sizeof( Texture* ) * this->max_textures );
			for( int j = 0 ; j < this->loaded_textures ; j++ )
			{
				new_max[j] = this->loaded_texture[j];
			}

			free( this->loaded_texture );

			this->loaded_texture = new_max;
		}

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