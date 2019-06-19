#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stack>

using namespace std;

#include "readfile.h"
#include "types.h"
#include "variables.h"
#include "transform.h"

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool readvals(stringstream& s, const int numvals, float* values)
{
	for (int i = 0; i < numvals; i++) {
		s >> values[i];
		if (s.fail()) {
			cout << "Failed reading value " << i << " will skip\n";
			return false;
		}
	}

	return true;
}

void rightmultiply(const M44& M, stack<M44>& transfstack)
{
	M44& T = transfstack.top();
	T = T * M;
}

void readfile(const char* filename) {
	string str, cmd;
	ifstream in;
	in.open(filename);
	if (in.is_open()) {
		// I need to implement a matrix stack to store transforms.  
		// This is done using standard STL Templates 
		stack <M44> transfstack;
		M44 id; id.clear();
		id.mat[0][0] = 1.0; id.mat[1][1] = 1.0; id.mat[2][2] = 1.0; id.mat[3][3] = 1.0;
		transfstack.push(id); // identity

		getline (in, str);
		while (in) {
			if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
				// Ruled out comment and blank lines 

				stringstream s(str);
				s >> cmd;
				int i;
				float values[10]; // Position and color for light, colors for others
									// Up to 10 params for cameras.  
				bool validinput; // Validity of input 

				if (cmd == "size") {
					validinput = readvals(s, 2, values);
					if (validinput) {
						imgsize.w = (int)values[0]; imgsize.h = (int)values[1];
					}
				}
				else if (cmd == "camera") {
					validinput = readvals(s, 10, values); // 10 values eye cen up fov
					if (validinput) {
						cam.lf.x = values[0]; cam.lf.y = values[1]; cam.lf.z = values[2];
						cam.la.x = values[3]; cam.la.y = values[4]; cam.la.z = values[5];
						cam.u.x = values[6]; cam.u.y = values[7]; cam.u.z = values[8];
						cam.fovy = values[9] * (pi / (float)180);
						cam.fovx = ((float)imgsize.w / (float)imgsize.h) * cam.fovy;
					}
				}

				// Material Commands 
				// Ambient, diffuse, specular, shininess properties for each object.
				// Filling this in is pretty straightforward, so I've left it in 
				// the skeleton, also as a hint of how to do the more complex ones.
				// Note that no transforms/stacks are applied to the colors. 

				else if (cmd == "ambient") {
					validinput = readvals(s, 3, values); // colors 
					if (validinput) {
						for (i = 0; i < 3; i++) {
							ambient[i] = values[i];
						}
					}
				}
				else if (cmd == "diffuse") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						for (i = 0; i < 3; i++) {
							diffuse[i] = values[i];
						}
					}
				}
				else if (cmd == "specular") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						for (i = 0; i < 3; i++) {
							specular[i] = values[i];
						}
					}
				}
				else if (cmd == "emission") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						for (i = 0; i < 3; i++) {
							emission[i] = values[i];
						}
					}
				}
				else if (cmd == "shininess") {
					validinput = readvals(s, 1, values);
					if (validinput) {
						shininess = values[0];
					}
				}
				else if (cmd == "attenuation") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						for (i = 0; i < 3; i++) {
							attenuation[i] = values[i];
						}
					}
				}

				// Process the light, add it to database.
				else if (cmd == "point") {
					validinput = readvals(s, 6, values); // Position/color for lts.
					if (validinput) {
						PositionLight pl;
						pl.pos.x = values[0];
						pl.pos.y = values[1];
						pl.pos.z = values[2];
						pl.col[0] = values[3];
						pl.col[1] = values[4];
						pl.col[2] = values[5];

						pos_l.push_back(pl);
					}
				}
				else if (cmd == "directional") {
					validinput = readvals(s, 6, values); // direction/color for lts.
					if (validinput) {
						DirectionalLight dl;
						dl.dir.x = values[0];
						dl.dir.y = values[1];
						dl.dir.z = values[2];
						dl.col[0] = values[3];
						dl.col[1] = values[4];
						dl.col[2] = values[5];

						dir_l.push_back(dl);
					}
				}

				else if (cmd == "maxverts") {
					validinput = readvals(s, 1, values); // maxverts
					// ignore this command
				}

				else if (cmd == "maxdepth") {
					validinput = readvals(s, 1, values);
					if (validinput) {
						maxdepth = (int)values[0];
					}
				}

				else if (cmd == "vertex") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						V3 v;
						v.x = values[0];
						v.y = values[1];
						v.z = values[2];
						vertex.push_back(v);
					}
				}

				else if (cmd == "tri") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						Triangle t;
						t.v0 = vertex[(int)values[0]];
						t.v1 = vertex[(int)values[1]];
						t.v2 = vertex[(int)values[2]];

						// Set the object's light properties
						for (int i = 0; i < 3; i++) {
							t.ambient[i] = ambient[i];
							t.diffuse[i] = diffuse[i];
							t.specular[i] = specular[i];
							t.emission[i] = emission[i];
						}
						t.shininess = shininess;

						// Set the object's transform
						t.transform = transfstack.top();

						tri.push_back(t);
					}
				}

				else if (cmd == "sphere") {
					validinput = readvals(s, 4, values);
					if (validinput) {
						Sphere s;
						s.center.x = values[0];
						s.center.y = values[1];
						s.center.z = values[2];
						s.radius = values[3];

						// Set the object's light properties
						for (int i = 0; i < 3; i++) {
							s.ambient[i] = ambient[i];
							s.diffuse[i] = diffuse[i];
							s.specular[i] = specular[i];
							s.emission[i] = emission[i];
						}
						s.shininess = shininess;

						// Set the object's transform
						s.transform = transfstack.top();

						sph.push_back(s);
					}
				}
				
				else if (cmd == "translate") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						M44 m = myTranslate(values[0], values[1], values[2]);
						rightmultiply(m, transfstack);

					}
				}

				else if (cmd == "scale") {
					validinput = readvals(s, 3, values);
					if (validinput) {
						M44 m = myScale(values[0], values[1], values[2]);
						rightmultiply(m, transfstack);
					}
				}

				else if (cmd == "rotate") {
					validinput = readvals(s, 4, values);
					if (validinput) {
						V3 axis;
						axis.x = values[0];
						axis.y = values[1];
						axis.z = values[2];
						M33 m = myRotate(values[3], axis);
						M44 mr; mr.getval(
							m.mat[0][0], m.mat[1][0], m.mat[2][0], 0, \
							m.mat[0][1], m.mat[1][1], m.mat[2][1], 0, \
							m.mat[0][2], m.mat[1][2], m.mat[2][2], 0, \
							0, 0, 0, 1);
						rightmultiply(mr, transfstack);
					}
				}

				// I include the basic push/pop code for matrix stacks
				else if (cmd == "pushTransform") {
					transfstack.push(transfstack.top());
				}

				else if (cmd == "popTransform") {
					if (transfstack.size() <= 1) {
						cerr << "Stack has no elements.  Cannot Pop\n";
					}
					else {
						transfstack.pop();
					}
				}

				else {
					cerr << "Unknown Command: " << cmd << " Skipping \n";
				}
			}
			getline(in, str);
		}
	}
	else {
		cerr << "Unable to Open Input Data File " << filename << "\n";
		throw 2;
	}
}