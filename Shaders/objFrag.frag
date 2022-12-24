#version 330 core
#define MIN 5

in vec3 fragPos;

uniform sampler2D tex0;
uniform sampler2D norm_tex;
uniform vec3 eyePos;

void attenuate(out float val, in float dist) {
	// Calculating attenuation modifier along with safeguards to prevent math error
	if(abs(dist) == 0)
		val = MIN; //set to a minimum value, otherwise
	else
		/* If distance between light and object is not zero,
		 * attenuation is the minimum between the inverse
		 * of the distance squared and the MIN constant this
		 * was done so the object would not shine brighter
		 * than the source
		 */
		val = min(1 / pow(dist, 2), MIN);
}

uniform float dir_lumens;
uniform float dir_amb_str;
uniform vec3 dir_amb_col;
uniform vec3 dir_target;
uniform vec3 dir_color;
uniform float dir_phong;
uniform float dir_spec_str;

void dirLight(out vec3 sun, in vec3 normals) {
	vec3 norm = normalize(normals);
	vec3 direction = normalize(-dir_target);

	float diff = max(dot(norm, direction), 0.f);

	vec3 viewDir = normalize(eyePos - fragPos);
	vec3 reflection = reflect(-direction, norm);

	float spec = pow(max(dot(reflection, viewDir), 0), dir_phong);
	spec *= dir_spec_str;

	sun = dir_lumens * (dir_amb_str + spec + diff) * (dir_amb_col + dir_color);
}

uniform vec3 pt_src;
uniform float pt_lumens;
uniform float pt_amb_str;
uniform vec3 pt_amb_col;
uniform vec3 pt_color;
uniform float pt_phong;
uniform float pt_spec_str;

void ptLight(out vec3 bulb, in vec3 norms) {
	float val;

	vec3 norm = normalize(norms);

	vec3 src = pt_src;
	src.z -= 0.7;

	vec3 rayVec = src - fragPos;

	float dist = length(rayVec);

	attenuate(val, dist);

	vec3 dir = normalize(rayVec);
	vec3 viewDir = normalize(eyePos - fragPos);
	vec3 reflection = reflect(-dir, norm);

	float diff = max(dot(norm, dir), 0.f);

	float spec = pow(max(dot(reflection, viewDir), 0), pt_phong);

	spec *= pt_spec_str;

	bulb = val * pt_lumens * (pt_amb_str + spec + diff) * (pt_amb_col * pt_color);
}

in mat3 TBN;
in vec2 texCoord;
in vec3 normCoord;

out vec4 FragColor;
uniform bool fgState;
void main() {
	vec4 pixelColor = texture(tex0, texCoord);

	vec3 normal = texture(norm_tex, texCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(TBN * normal);

	vec3 norms = normCoord;
	vec3 sun;
		//norms = (normal-1)*2;
	vec3 bulb;
	dirLight(sun, norms);
	ptLight(bulb, norms);
	FragColor = pixelColor *
		vec4(sun + bulb, 1);
	if(fgState) {
		FragColor = mix(FragColor, vec4(1, 0, 0, 1), 0.5);
		// FragColor.r = 1;
		// FragColor -= vec4(0, 1, 1, 0);
	}

}