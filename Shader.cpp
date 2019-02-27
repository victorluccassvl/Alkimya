// Class_______________________________________________________________________________________________________________
class Shader
{
	public:
	static char* loadShaderSource( const char *shader_path );
	static uint  createVertexShader( const char *shader_path );
	static uint  createFragmentShader( const char *shader_path );
	static uint  createShaderProgram( uint *shader, int shaders );
	static void  deleteShader( uint shader );
	static void  installProgram( uint program );
	static void  deleteProgram( uint program );

	static void  initUniformArrayi( uint shader, const char *variable, int *array, uint array_size );
	static void  initUniformArrayu( uint shader, const char *variable, uint *array, uint array_size );
	static void  initUniformArrayf( uint shader, const char *variable, float *array, uint array_size );
	static void  initUniformMatrix2( uint shader, const char *variable, bool transpose, const GLfloat *matrix );
	static void  initUniformMatrix3( uint shader, const char *variable, bool transpose, const GLfloat *matrix );
	static void  initUniformMatrix4( uint shader, const char *variable, bool transpose, const GLfloat *matrix );
	static void  initUniformTexture( uint shader, const char *variable, uint socket );
};


// Methods_____________________________________________________________________________________________________________
char* Shader::loadShaderSource( const char *shader_path )
{
	FILE *shader_arq = fopen( shader_path, "r" );

	if ( shader_arq == NULL )
	{
		printf( "Falha ao abrir o arquivo %s\n", shader_path );
		STOP;
	}

	fseek( shader_arq, 0, SEEK_END );
	int shader_size = ftell( shader_arq );

	char *shader_text = ( char* ) malloc( sizeof( char ) * ( shader_size + 1 ) );

	rewind( shader_arq );

	fread( shader_text, shader_size, 1, shader_arq );

	shader_text[shader_size] = '\0';

	fclose( shader_arq );

	return shader_text;
}

uint Shader::createVertexShader( const char *shader_path )
{

	char *vertex_shader_source = Shader::loadShaderSource( shader_path );

	uint new_vertex_shader;

	new_vertex_shader = glCreateShader( GL_VERTEX_SHADER );

	glShaderSource( new_vertex_shader, 1, &vertex_shader_source, NULL );

	glCompileShader( new_vertex_shader );

	free( vertex_shader_source );

	int success;
	glGetShaderiv( new_vertex_shader, GL_COMPILE_STATUS, &success );

	if ( ! success )
	{
		// TODO: Treat possible log overflow
		char info_log[_GL_MAX_SHADER_COMPILATION_LOG_SIZE];
		glGetShaderInfoLog( new_vertex_shader, _GL_MAX_SHADER_COMPILATION_LOG_SIZE, NULL, info_log );
		
		printf( "Falha ao compilar o vertex shader.\n%s\n", info_log );
		STOP;
	}

	return new_vertex_shader;
}

uint Shader::createFragmentShader( const char *shader_path )
{

	char *fragment_shader_source = Shader::loadShaderSource( shader_path );

	uint new_fragment_shader;

	new_fragment_shader = glCreateShader( GL_FRAGMENT_SHADER );

	glShaderSource( new_fragment_shader, 1, &fragment_shader_source, NULL );

	glCompileShader( new_fragment_shader );

	free( fragment_shader_source );

	int success;
	glGetShaderiv( new_fragment_shader, GL_COMPILE_STATUS, &success );

	if ( ! success )
	{
		// TODO: Treat possible log overflow
		char info_log[_GL_MAX_SHADER_COMPILATION_LOG_SIZE];
		glGetShaderInfoLog( new_fragment_shader, _GL_MAX_SHADER_COMPILATION_LOG_SIZE, NULL, info_log );

		printf( "Falha ao compilar o fragment shader.\n%s\n", info_log );
		STOP;
	}

	return new_fragment_shader;
}

uint Shader::createShaderProgram( uint *shader, int shaders )
{
	uint new_program;

	new_program = glCreateProgram();

	for( int i = 0; i < shaders ; i++ )
	{
		glAttachShader( new_program, shader[i] );
	}

	glLinkProgram( new_program );

	int success;
	glGetProgramiv( new_program, GL_LINK_STATUS, &success );

	if ( ! success )
	{
		// TODO: Treat possible log overflow
		char info_log[_GL_MAX_SHADER_PROGRAM_COMPILATION_LOG_SIZE];
		glGetProgramInfoLog( new_program, _GL_MAX_SHADER_PROGRAM_COMPILATION_LOG_SIZE, NULL, info_log );

		printf( "Falha ao linkar o programa shader.\n%s\n", info_log );
		STOP;
	}

	return new_program;
}

void Shader::deleteShader( uint shader )
{
	glDeleteShader( shader );
}

void Shader::installProgram( uint program )
{
	glUseProgram( program );
}

void Shader::deleteProgram( uint program )
{
	glDeleteProgram( program );
}

void Shader::initUniformArrayi( uint shader, const char *variable, int *array, uint array_size )
{
	uint location = glGetUniformLocation( shader, variable );

	if ( array_size > 0 && array_size <= 4 )
	{
		glUniform1iv( location, array_size, array );
	}
	else
	{
		printf( "Parametro inválido para inicialização de variável do shader\n" );
		STOP;
	}
}

void Shader::initUniformArrayu( uint shader, const char *variable, uint *array, uint array_size )
{
	uint location = glGetUniformLocation( shader, variable );

	if ( array_size > 0 && array_size <= 4 )
	{
		glUniform1uiv( location, array_size, array );
	}
	else
	{
		printf( "Parametro inválido para inicialização de variável do shader\n" );
		STOP;
	}
}

void Shader::initUniformArrayf( uint shader, const char *variable, float *array, uint array_size )
{
	uint location = glGetUniformLocation( shader, variable );

	if ( array_size > 0 && array_size <= 4 )
	{
		glUniform1fv( location, array_size, array );
	}
	else
	{
		printf( "Parametro inválido para inicialização de variável do shader\n" );
		STOP;
	}
}

void Shader::initUniformMatrix2( uint shader, const char *variable, bool transpose, const GLfloat *matrix )
{
	uint location = glGetUniformLocation( shader, variable );
	if ( transpose )
	{
			glUniformMatrix2fv( location, 1, GL_TRUE, matrix );
	}
	else
	{
			glUniformMatrix2fv( location, 1, GL_FALSE, matrix );
	}
}

void Shader::initUniformMatrix3( uint shader, const char *variable, bool transpose, const GLfloat *matrix )
{
	uint location = glGetUniformLocation( shader, variable );
	if ( transpose )
	{
			glUniformMatrix3fv( location, 1, GL_TRUE, matrix );
	}
	else
	{
			glUniformMatrix3fv( location, 1, GL_FALSE, matrix );
	}
}

void Shader::initUniformMatrix4( uint shader, const char *variable, bool transpose, const GLfloat *matrix )
{
	uint location = glGetUniformLocation( shader, variable );
	if ( transpose )
	{
			glUniformMatrix4fv( location, 1, GL_TRUE, matrix );
	}
	else
	{
			glUniformMatrix4fv( location, 1, GL_FALSE, matrix );
	}
}

void Shader::initUniformTexture( uint shader, const char *variable, uint socket )
{
	uint location = glGetUniformLocation( shader, variable );

	glUniform1i( location, socket );
}