#version 330

layout(location = 1) in vec3 aNormal;

out vec3 vColor;

void main() {
	vColor = aNormal;
}