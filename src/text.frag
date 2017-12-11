R"(
//
varying vec2 texcoord;
uniform sampler2D texture;
//uniform vec4 color;

void main() {
	//gl_FragColor = vec4(1.0, 1.0, 1.0, texture2D(texture, tecoord).r) * color;
	gl_FragColor = vec4(1.0, 1.0, 1.0, texture2D(texture, texcoord).a);
}

)"
