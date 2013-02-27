#version 150

in vec2 TexCoord;

uniform float alpha;
uniform sampler2D tex;

out vec4 outColor;

void main() {

   vec4 color = texture(tex, TexCoord);
   float malpha = min( alpha, color.x );
   malpha = min( malpha, color.y );
   malpha = min( malpha, color.z );
   outColor = vec4( 1.0, 1.0, 1.0, 0.1*malpha );

}