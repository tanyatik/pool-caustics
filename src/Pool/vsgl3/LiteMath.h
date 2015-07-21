#pragma once

#include <math.h>

struct float2
{
  float x,y;
};

struct float3
{
  float3():x(0), y(0), z(0){}
  float3(float a,float b, float c): x(a), y(b), z(c) {}
  float3(const float arr[]) {x = arr[0]; y = arr[1]; z = arr[2];}
	void normalize() { 
		float len = sqrt(x*x + y*y + z*z);
		x /= len;
		y /= len;
		z /= len;
	}

  float x,y,z;
};

struct float4
{
  float4():x(0), y(0), z(0){}
  float4(float a,float b, float c, float d): x(a), y(b), z(c), w(d) {}

  float x,y,z,w;
};




struct float4x4
{
  float4x4(){identity();} 

  float4x4(const float arr[16]) 
  {
    row[0] = float4(arr[0],arr[1],arr[2],arr[3]);
    row[1] = float4(arr[4],arr[5],arr[6],arr[7]);
    row[2] = float4(arr[8],arr[9],arr[10],arr[11]);
    row[3] = float4(arr[12],arr[13],arr[14],arr[15]);
  }

  void identity() 
  { 
    row[0] = float4(1,0,0,0);
    row[1] = float4(0,1,0,0);
    row[2] = float4(0,0,1,0);
    row[3] = float4(0,0,0,1);
  }

  float& M(int x, int y) {return ((float*)row)[y*4+x]; }
  float  M(int x, int y) const {return ((float*)row)[y*4+x]; }
  
  float* L() {return (float*)row;}
  const float* L() const {return (float*)row;}

  float4 row[4]; 
};


inline float rnd(float s, float e)
{
  float t = (float)(rand())/(32767); 
  return s + t*(e-s);
}


