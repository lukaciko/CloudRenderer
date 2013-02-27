#version 150

in vec2 vertPos;
in vec2 texCoord;

//uniform mat4 model;
uniform vec3 position;
uniform mat4 view;
uniform mat4 proj;

out vec2 TexCoord;

void main() {
   mat4 model = mat4(1.0);
   model[3][0] = position.x;
   model[3][1] = position.y;
   model[3][2] = position.z;

   mat4 modelView = view * model;

   modelView[0][0] = 1.0f;
   modelView[0][1] = 0.0f;
   modelView[0][2] = 0.0f;
   modelView[1][0] = 0.0f;
   modelView[1][1] = 1.0f;
   modelView[1][2] = 0.0f;
   modelView[2][0] = 0.0f;
   modelView[2][1] = 0.0f;
   modelView[2][2] = 1.0f;

   gl_Position = proj * modelView * vec4( vertPos, 0.0, 1.0 );
   TexCoord = texCoord;

}