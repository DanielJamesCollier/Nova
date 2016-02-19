struct BaseLight
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;	
};

struct Attenuation
{
	float constant;
	float linear;
	float exp;
};

struct PointLight
{
	vec3 position;
	Attenuation atten;
	BaseLight base;
};

struct SpotLight
{
	PointLight base;
	vec3       direction;
	float	   cutoff;
};