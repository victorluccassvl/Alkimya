// Related Data Structure______________________________________________________________________________________________
enum TextureType
{
	SPECULAR, DIFFUSE
};


// Class_______________________________________________________________________________________________________________
class Texture
{
	public:
	TextureType type;
	uint id;
	int  width;
	int  height;
	char path[_MODEL_MAX_PATH_SIZE];
};
