#include "macros.cpp"
#include "configuration.cpp"

class Shader {

	public:

		static char* loadShaderSource( const char *path );
		static uint  createVertexShader( const char *shader_path );
		static uint  createFragmentShader( const char *shader_path );
		static uint  createShaderProgram( uint *shaders, int quantity );
		static void  deleteShader( uint shader );
		static void  installProgram( uint program );
		static void  deleteProgram( uint program );

		static void  initUniformArrayi( uint shader, const char *variable, uint count, int *array );
		static void  initUniformArrayu( uint shader, const char *variable, uint count, uint *array );
		static void  initUniformArrayf( uint shader, const char *variable, uint count, float *array );
		static void  initUniformMatrix2( uint shader, const char *variable, bool transpose, const GLfloat *matrix );
		static void  initUniformMatrix3( uint shader, const char *variable, bool transpose, const GLfloat *matrix );
		static void  initUniformMatrix4( uint shader, const char *variable, bool transpose, const GLfloat *matrix );
		static void  initUniformTexture( uint shader, const char *variable, uint socket );
};


char* Shader::loadShaderSource( const char *path )
{
	FILE *arq = NULL;
	char *text = NULL;

	arq = fopen( path, "r" );

	if ( arq == NULL )
	{
		printf( "Falha ao abrir o arquivo %s\n", path );
		STOP;
	}

	fseek( arq, 0, SEEK_END );
	int size = ftell( arq );
	rewind( arq );
	text = ( char* ) malloc( sizeof(char) * ( size + 1 ) );

	fread( text, size, 1, arq );
	text[size] = '\0';

	fclose( arq );

	return text;
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
	char info_log[_GL_MAX_SHADER_COMPILATION_LOG_SIZE];
	glGetShaderiv( new_vertex_shader, GL_COMPILE_STATUS, &success );

	if ( ! success )
	{
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
	char info_log[_GL_MAX_SHADER_COMPILATION_LOG_SIZE];
	glGetShaderiv( new_fragment_shader, GL_COMPILE_STATUS, &success );

	if ( ! success )
	{
		glGetShaderInfoLog( new_fragment_shader, _GL_MAX_SHADER_COMPILATION_LOG_SIZE, NULL, info_log );
		printf( "Falha ao compilar o fragment shader.\n%s\n", info_log );
		STOP;
	}

	return new_fragment_shader;
}


uint Shader::createShaderProgram( uint *shaders, int quantity )
{
	uint new_program;

	new_program = glCreateProgram();

	for( int i = 0; i < quantity ; i++ )
	{
		glAttachShader( new_program, shaders[i] );
	}

	glLinkProgram( new_program );

	int success;
	char info_log[_GL_MAX_SHADER_PROGRAM_COMPILATION_LOG_SIZE];
	glGetProgramiv( new_program, GL_LINK_STATUS, &success );

	if ( ! success )
	{
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


void Shader::initUniformArrayi( uint shader, const char *variable, uint count, int *array )
{
	uint location = glGetUniformLocation( shader, variable );

	switch ( count )
	{
		case 1:
		{
			glUniform1iv( location, 1, array );
			break;
		}
		case 2:
		{
			glUniform1iv( location, 2, array );
			break;
		}
		case 3:
		{
			glUniform1iv( location, 3, array );
			break;
		}
		case 4:
		{
			glUniform1iv( location, 4, array );
			break;
		}
	}
}


void Shader::initUniformArrayu( uint shader, const char *variable, uint count, uint *array )
{
	uint location = glGetUniformLocation( shader, variable );

	switch ( count )
	{
		case 1:
		{
			glUniform1uiv( location, 1, array );
			break;
		}
		case 2:
		{
			glUniform1uiv( location, 2, array );
			break;
		}
		case 3:
		{
			glUniform1uiv( location, 3, array );
			break;
		}
		case 4:
		{
			glUniform1uiv( location, 4, array );
			break;
		}
	}
}


void Shader::initUniformArrayf( uint shader, const char *variable, uint count, float *array )
{
	uint location = glGetUniformLocation( shader, variable );

	switch ( count )
	{
		case 1:
		{
			glUniform1fv( location, 1, array );
			break;
		}
		case 2:
		{
			glUniform1fv( location, 2, array );
			break;
		}
		case 3:
		{
			glUniform1fv( location, 3, array );
			break;
		}
		case 4:
		{
			glUniform1fv( location, 4, array );
			break;
		}
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