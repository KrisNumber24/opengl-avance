#version 330

in vec3 vViewSpaceNormal;

out vec3 fColor;

void main() {
	fColor = vViewSpaceNormal;
}