// 2D Vector___________________________________________________________________________________________________________
inline void v2x( vec2 vec, float x ) { vec[0] = x; }
inline void v2y( vec2 vec, float y ) { vec[1] = y; }
inline void v2g( vec2 vec, float g ) { vec[0] = g; }
inline void v2a( vec2 vec, float a ) { vec[1] = a; }
inline float v2x( vec2 vec ) { return vec[0]; }
inline float v2y( vec2 vec ) { return vec[1]; }
inline float v2g( vec2 vec ) { return vec[0]; }
inline float v2a( vec2 vec ) { return vec[1]; }
inline void v2( vec2 vec, float x, float y )
{
	vec[0] = x;
	vec[1] = y;
}
inline void v2( vec2 dest, vec2 cpy )
{
	dest[0] = cpy[0];
	dest[1] = cpy[1];
}


// 3D Vector___________________________________________________________________________________________________________
inline void v3x( vec3 vec, float x ) { vec[0] = x; }
inline void v3y( vec3 vec, float y ) { vec[1] = y; }
inline void v3z( vec3 vec, float z ) { vec[2] = z; }
inline void v3r( vec3 vec, float r ) { vec[0] = r; }
inline void v3g( vec3 vec, float g ) { vec[1] = g; }
inline void v3b( vec3 vec, float b ) { vec[2] = b; }
inline float v3x( vec3 vec ) { return vec[0]; }
inline float v3y( vec3 vec ) { return vec[1]; }
inline float v3z( vec3 vec ) { return vec[2]; }
inline float v3r( vec3 vec ) { return vec[0]; }
inline float v3g( vec3 vec ) { return vec[1]; }
inline float v3b( vec3 vec ) { return vec[2]; }
inline void v3( vec3 vec, float x, float y, float z )
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}
inline void v3( vec3 dest, vec3 cpy )
{
	dest[0] = cpy[0];
	dest[1] = cpy[1];
	dest[2] = cpy[2];
}


// 4D Vector___________________________________________________________________________________________________________
inline void v4x( vec4 vec, float x ) { vec[0] = x; }
inline void v4y( vec4 vec, float y ) { vec[1] = y; }
inline void v4z( vec4 vec, float z ) { vec[2] = z; }
inline void v4w( vec4 vec, float w ) { vec[3] = w; }
inline void v4r( vec4 vec, float r ) { vec[0] = r; }
inline void v4g( vec4 vec, float g ) { vec[1] = g; }
inline void v4b( vec4 vec, float b ) { vec[2] = b; }
inline void v4a( vec4 vec, float a ) { vec[3] = a; }
inline float v4x( vec4 vec ) { return vec[0]; }
inline float v4y( vec4 vec ) { return vec[1]; }
inline float v4z( vec4 vec ) { return vec[2]; }
inline float v4w( vec4 vec ) { return vec[3]; }
inline float v4r( vec4 vec ) { return vec[0]; }
inline float v4g( vec4 vec ) { return vec[1]; }
inline float v4b( vec4 vec ) { return vec[2]; }
inline float v4a( vec4 vec ) { return vec[3]; }
inline void v4( vec4 vec, float x, float y, float z, float w )
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
	vec[3] = w;
}
inline void v4( vec4 dest, vec4 cpy )
{
	dest[0] = cpy[0];
	dest[1] = cpy[1];
	dest[2] = cpy[2];
	dest[3] = cpy[3];
}