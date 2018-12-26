#include "macros.cpp"
#include "configuration.cpp"

class OpenGl {

	public:

		static char* loadShaderSource( const char *path );
		static uint  createVertexShader( const char *shader_path );
		static uint  createFragmentShader( const char *shader_path );
		static uint  createShaderProgram( uint *shaders, int quantity );
		static void  deleteShader( uint shader );
		static void  enableDepth();
		static uint  createVertexArrayObject();
		static void  bindVertexArrayObject( uint VAO );
		static uint  createBuffer();
		static void  bindVertexBufferObject( uint VBO );
		static void  bindElementBufferObject( uint EBO );
		static void  defineAttribFormat( uint location, uint components, GLenum type, GLboolean normalize, uint stride, const void *start );
		static void  fillVertexBufferObject( uint VAO, uint VBO, uint size, const GLvoid *data, GLenum mode );
		static void  fillElementBufferObject( uint VAO, uint VBO, uint size, const GLvoid *data, GLenum mode );
		static void  initUniformArrayi( uint shader, const char *variable, uint count, int *array );
		static void  initUniformArrayu( uint shader, const char *variable, uint count, uint *array );
		static void  initUniformArrayf( uint shader, const char *variable, uint count, float *array );
		static void  initUniformMatrix2( uint shader, const char *variable, bool transpose, const GLfloat *matrix );
		static void  initUniformMatrix3( uint shader, const char *variable, bool transpose, const GLfloat *matrix );
		static void  initUniformMatrix4( uint shader, const char *variable, bool transpose, const GLfloat *matrix );
		static void  clearBuffers( vec4 clear_color, GLbitfield buffers );
		static void  installShaderProgram( uint program );
		static void  deleteShaderProgram( uint program );
		static void  bind2DTextureToUnit( GLenum unit, uint texture );
		static void  initUniformTexture( uint shader, const char *variable, uint socket );
		static void  activatePolygonMode( GLenum face, GLenum mode );
		static void  drawArrays( GLenum mode, uint count );
		static void  setTexture2DParam( uint texture, GLenum parameter, void *value, const char *type );
};


char* OpenGl::loadShaderSource( const char *path )
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


uint OpenGl::createVertexShader( const char *shader_path )
{

	char *vertex_shader_source = OpenGl::loadShaderSource( shader_path );

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


uint OpenGl::createFragmentShader( const char *shader_path )
{

	char *fragment_shader_source = OpenGl::loadShaderSource( shader_path );

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


uint OpenGl::createShaderProgram( uint *shaders, int quantity )
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


void OpenGl::deleteShader( uint shader )
{
	glDeleteShader( shader );
}

void OpenGl::enableDepth()
{
	glEnable( GL_DEPTH_TEST );
}

uint OpenGl::createVertexArrayObject()
{
	uint VAO;

	glGenVertexArrays( 1, &VAO );

	return VAO;
}

void OpenGl::bindVertexArrayObject( uint VAO )
{
	glBindVertexArray( VAO );
}

uint OpenGl::createBuffer()
{
	uint buffer;

	glGenBuffers( 1, &buffer );

	return buffer;
}

void OpenGl::bindVertexBufferObject( uint VBO )
{
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
}

void OpenGl::bindElementBufferObject( uint EBO )
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
}


void OpenGl::defineAttribFormat( uint location, uint components, GLenum type, GLboolean normalize, uint stride, const void *start )
{
	glVertexAttribPointer( location, components, type, normalize, stride, ( void* ) start );
	glEnableVertexAttribArray( location );
}

void OpenGl::fillVertexBufferObject( uint VAO, uint VBO, uint size, const GLvoid *data, GLenum mode )
{
	OpenGl::bindVertexArrayObject( VAO );
	OpenGl::bindVertexBufferObject( VBO );
	glBufferData( GL_ARRAY_BUFFER, size, data, mode );
}

void OpenGl::fillElementBufferObject( uint VAO, uint VBO, uint size, const GLvoid *data, GLenum mode )
{
	OpenGl::bindVertexArrayObject( VAO );
	OpenGl::bindVertexBufferObject( VBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, size, data, mode );
}




void OpenGl::initUniformArrayi( uint shader, const char *variable, uint count, int *array )
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
void OpenGl::initUniformArrayu( uint shader, const char *variable, uint count, uint *array )
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
void OpenGl::initUniformArrayf( uint shader, const char *variable, uint count, float *array )
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
void OpenGl::initUniformMatrix2( uint shader, const char *variable, bool transpose, const GLfloat *matrix )
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
void OpenGl::initUniformMatrix3( uint shader, const char *variable, bool transpose, const GLfloat *matrix )
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
void OpenGl::initUniformMatrix4( uint shader, const char *variable, bool transpose, const GLfloat *matrix )
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

void OpenGl::clearBuffers( vec4 clear_color, GLbitfield buffers )
{
	glClearColor( clear_color[0], clear_color[1], clear_color[2], clear_color[3] );
	glClear( buffers );
}

void OpenGl::installShaderProgram( uint program )
{
	glUseProgram( program );
}

void OpenGl::deleteShaderProgram( uint program )
{
	glDeleteProgram( program );
}

void OpenGl::bind2DTextureToUnit( GLenum unit, uint texture )
{
	glActiveTexture( unit );
	glBindTexture( GL_TEXTURE_2D, texture );
}	

void OpenGl::initUniformTexture( uint shader, const char *variable, uint socket )
{
	uint location = glGetUniformLocation( shader, variable );

	glUniform1i( location, socket );
}

void OpenGl::activatePolygonMode( GLenum face, GLenum mode )
{
	glPolygonMode( face, mode );
}

void OpenGl::drawArrays( GLenum mode, uint count )
{
	glDrawArrays( mode, 0, count );
}
/*
void OpenGl::setTexture2DParam( uint texture, GLenum parameter, void *value, const char *type )
{	
	glBindTexture( GL_TEXTURE_2D, texture );

	if ( strcmp( type, "f" ) == 0 )
	{
		glTexParameterf( GL_TEXTURE_2D, parameter, ( float ) ( *value ) );
		return;
	}
	if ( strcmp( type, "i" ) == 0 )
	{
		glTexParameteri( GL_TEXTURE_2D, parameter, ( int ) ( *value ) );
		return;
	}
	if ( strcmp( type, "fv" ) == 0 )
	{
		glTexParameterfv( GL_TEXTURE_2D, parameter, ( float* ) ( value ) );
		return;
	}
	if ( strcmp( type, "iv" ) == 0 )
	{
		glTexParameteriv( GL_TEXTURE_2D, parameter, ( int* ) ( value ) );
		return;
	}
	if ( strcmp( type, "liv" ) == 0 )
	{
		glTexParameterliv( GL_TEXTURE_2D, parameter, ( long int* ) ( value ) );
		return;
	}
	if ( strcmp( type, "luiv" ) == 0 )
	{
		glTexParameterluiv( GL_TEXTURE_2D, parameter, ( long unsigned int* ) ( value ) );
		return;
	}
}*/