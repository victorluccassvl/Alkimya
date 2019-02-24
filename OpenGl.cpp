class OpenGl
{

	public:

		static void  enableDepth();

		static uint  createVertexArrayObject();
		static uint  createBuffer();
		static void  clearBuffers( vec4 clear_color, GLbitfield buffers );

		static void  bindVertexArrayObject( uint VAO );
		static void  bindVertexBufferObject( uint VBO );
		static void  bindElementBufferObject( uint EBO );
		static void  bind2DTextureToUnit( GLenum unit, uint texture );

		static void  fillVertexBufferObject( uint VAO, uint VBO, uint size, const GLvoid *data, GLenum mode );
		static void  fillElementBufferObject( uint VAO, uint VBO, uint size, const GLvoid *data, GLenum mode );

		static void  setTexture2DParam( uint texture, GLenum parameter, void *value, const char *type );
		static void  defineAttribFormat( uint location, uint components, GLenum type, GLboolean normalize, uint stride, const void *start );
		static void  activatePolygonMode( GLenum face, GLenum mode );
		static void  drawArrays( GLenum mode, uint count );
		static void  drawElements( GLenum mode, uint count );
};


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


uint OpenGl::createBuffer()
{
	uint buffer;

	glGenBuffers( 1, &buffer );

	return buffer;
}


void OpenGl::clearBuffers( vec4 clear_color, GLbitfield buffers )
{
	glClearColor( clear_color[0], clear_color[1], clear_color[2], clear_color[3] );
	glClear( buffers );
}


void OpenGl::bindVertexArrayObject( uint VAO )
{
	glBindVertexArray( VAO );
}


void OpenGl::bindVertexBufferObject( uint VBO )
{
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
}


void OpenGl::bindElementBufferObject( uint EBO )
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
}


void OpenGl::bind2DTextureToUnit( GLenum unit, uint texture )
{
	glActiveTexture( unit );
	glBindTexture( GL_TEXTURE_2D, texture );
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


void OpenGl::defineAttribFormat( uint location, uint components, GLenum type, GLboolean normalize, uint stride, const void *start )
{
	glVertexAttribPointer( location, components, type, normalize, stride, ( void* ) start );
	glEnableVertexAttribArray( location );
}


void OpenGl::activatePolygonMode( GLenum face, GLenum mode )
{
	glPolygonMode( face, mode );
}


void OpenGl::drawArrays( GLenum mode, uint count )
{
	glDrawArrays( mode, 0, count );
}


void OpenGl::drawElements( GLenum mode, uint count )
{
	glDrawElements( mode, count, GL_UNSIGNED_INT, 0 );
}