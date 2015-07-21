#version 330

in  vec2  fragmentTexCoord;
out float outHeight;

uniform sampler2D inPositions;
uniform sampler2D inPositionsPrev;

uniform int gridSizeX;
uniform int gridSizeZ;

void main(void)
{ 
  float x = fragmentTexCoord.x*float(gridSizeX);
  float z = fragmentTexCoord.y*float(gridSizeZ);
  
  vec2 invSize = vec2(1.0/float(gridSizeX), 1.0/float(gridSizeZ));
 
  float Hprev  = textureLod(inPositionsPrev, vec2(x,z)*invSize, 0).x; 

  float Hcurr1 = textureLod(inPositions, vec2(x + 1,z    )*invSize, 0).x; 
  float Hcurr2 = textureLod(inPositions, vec2(x,    z - 1)*invSize, 0).x; 
  float Hcurr3 = textureLod(inPositions, vec2(x,    z + 1)*invSize, 0).x; 
  float Hcurr4 = textureLod(inPositions, vec2(x - 1,z    )*invSize, 0).x; 

  float omega = 1.995;

  outHeight = (1.0 - omega)*Hprev + (omega*(Hcurr1 + Hcurr2 + Hcurr3 + Hcurr4))*0.25;
}

