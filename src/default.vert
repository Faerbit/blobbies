R"(

//#version 150

uniform mat4 MP;
attribute vec2 vPos;

void main()
{
   gl_Position = MP * vec4(vPos, 0.0, 1.0);
}
)"
