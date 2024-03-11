// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function. Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
    const float radians = glm::radians(degrees);
	float cosine = glm::cos(radians);
	float sine = glm::sin(radians);
	vec3 norm_axis = glm::normalize(axis);
	mat3 id = mat3(1.0);
	mat3 M1 = cosine * id;

	float cosine_complement = (1-cosine);
	mat3 transpose = mat3(
		norm_axis[0]*norm_axis[0],norm_axis[0]*norm_axis[1],norm_axis[0]*norm_axis[2],
		norm_axis[0]*norm_axis[1],norm_axis[1]*norm_axis[1],norm_axis[1]*norm_axis[2],
		norm_axis[0]*norm_axis[2],norm_axis[1]*norm_axis[2],norm_axis[2]*norm_axis[2]
	);
	mat3 M2 = cosine_complement * transpose;

	mat3 A = mat3(
		0,norm_axis[2],-norm_axis[1],
		-norm_axis[2],0,norm_axis[0],
		norm_axis[1],-norm_axis[0],0
	);
	mat3 M3 = sine * A;
	return M1+M2+M3;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
    mat3 rotateLeft = rotate(degrees, up);
	eye = rotateLeft * eye;
	up = rotateLeft * up;	 
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
    vec3 right = glm::cross(up, eye);
	mat3 rotateUp = rotate(-degrees, right);
	eye = rotateUp * eye;
	up = rotateUp * up;	 
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
    vec3 w = glm::normalize(eye-center);
	vec3 u = glm::normalize(glm::cross(up,w));
	vec3 v = glm::normalize(glm::cross(w,u));
	mat4 final_matrix = mat4(
		u.x,v.x,w.x,0,
		u.y,v.y,w.y,0,
		u.z,v.z,w.z,0,
		-glm::dot(u, eye),-glm::dot(v, eye),-glm::dot(w, eye),1
	);
	mat4 ret = final_matrix;
	return ret;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
  float d = 1/ tan(fovy * 0.5 * pi / 180);;
  mat4 ret = mat4(
	d/aspect,0,0,0,
	0,d,0,0,
	0,0,-1.0*((zNear + zFar)/(zFar - zNear)),-1.0,
	0,0,-1.0* ((2 * zFar * zNear)/(zFar - zNear)),0
  );
  return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
  mat4 ret = mat4(
    sx,0,0,0,
    0,sy,0,0,
    0,0,sz,0,
    0,0,0,1
  );
  return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
  mat4 ret = mat4(
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	tx,ty,tz,1
  );
  return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
  vec3 x = glm::cross(up,zvec); 
  vec3 y = glm::cross(zvec,x); 
  vec3 ret = glm::normalize(y); 
  return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
