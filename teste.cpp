#include <cglm/cglm.h>
#include <stdio.h>
#include <assert.h>

int main( int argc, char* args[] )
{

	versor orientation;

	glm_quat( orientation, glm_rad(90), 1.0f, 0.0f, 0.0f );

	mat4 transf;

	glm_quat_mat4( orientation, transf );

	

	return 0;
}