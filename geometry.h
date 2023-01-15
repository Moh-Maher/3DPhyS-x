//=================================================================================================
/*!
//  \file  geometry.h
//  \brief Primary include file of the 3DPhyS-x library
//
//  Copyright (c) 2022 Mohammed Maher Abdelrahim Mohammed
//  
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
*/
//=================================================================================================
#pragma once
#include <cmath>
#include <stdio.h>
#include<iostream>
#include <cstring>
#include <unistd.h>
#include <map>
//#include<cstdlib> // for std::atexit
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
std::map<std::string,std::string> Color = {
{"white","\x1B[37m"},{"red","\x1B[31m"},
{"green", "\x1B[32m"},
{"yellow", "\x1B[33m"},
{"blue", "\x1B[34m"},
{"magenta", "\x1B[35m"},
{"cyan", "\x1B[36m"}
};

//===================================================================
//
//===================================================================
class Cube{
private:
	float alpha, beta, gamma;
	std::string _color= "white";
	float cubeWidth = 20;
	int width = 160, height = 44;
	float zBuffer[160 * 44];
	char buffer[160 * 44];
	int background = ' ';
	int distanceFromCam = 100;
	float horizontalOffset;
	float K1 = 40;

	float incrementSpeed = 0.6;

	float x, y, z;
	float ooz;
	int xp, yp;
	int idx;
	
	float rotateX(const int& i, const int& j, const int& k);
	float rotateY(const int& i, const int& j, const int& k);
	float rotateZ(const int& i, const int& j, const int& k);
	void  calculateForSurface(const float& cubeX, const float& cubeY, 
				  const float& cubeZ, const char& ch);
public:
	Cube();
	Cube(const float& w):cubeWidth(w){}
	Cube(const float& w, std::string color):cubeWidth(w),_color(color){}
	void Draw();
	
};
//===================================================================
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
float Cube::rotateX(const int& i, const int& j, const int& k) {
	return (
		j * sin(alpha) * sin(beta) * cos(gamma) 
		- k * cos(alpha) * sin(beta) * cos(gamma) 
		+ j * cos(alpha) * sin(gamma) 
		+ k * sin(alpha) * sin(gamma) 
		+ i * cos(beta) * cos(gamma)
         	);
}
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
float Cube::rotateY(const int& i, const int& j, const int& k) {
	return ( 
	       j * cos(alpha) * cos(gamma) + k * sin(alpha) * cos(gamma) 
		 - j * sin(alpha) * sin(beta) * sin(gamma) 
		 + k * cos(alpha) * sin(beta) * sin(gamma)
		 - i * cos(beta) * sin(gamma) 
		);
}
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
float Cube::rotateZ(const int& i, const int& j, const int& k) {
	return (
  		 k * cos(alpha) * cos(beta)
  		 - j * sin(alpha) * cos(beta) 
  		 + i * sin(beta)
	      );
}
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
void Cube::calculateForSurface(const float& cubeX, const float& cubeY, 
			       const float& cubeZ, const char& ch) {
	x = rotateX(cubeX, cubeY, cubeZ);
	y = rotateY(cubeX, cubeY, cubeZ);
	z = rotateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

	ooz = 1 / z;

	xp = (int)(width / 2 + horizontalOffset + K1 * ooz * x * 2);
	yp = (int)(height / 2 + K1 * ooz * y);

	idx = xp + yp * width;
	if (idx >= 0 && idx < width * height) {
		if (ooz > zBuffer[idx]) {
			zBuffer[idx] = ooz;
			buffer[idx] = ch;
		}
	}
}
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
void Cube::Draw()
{
	auto clr = Color.find(_color);
	std::cout<<clr->second<<std::endl;
	
	while (1) {
	//int i=0;
	//for(;i<100;){
		memset(buffer, background, width * height);
		memset(zBuffer, 0, width * height * 4);
		horizontalOffset = -2 * cubeWidth;
		
		for(float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed)
		{
			for (float cubeY = -cubeWidth; cubeY < cubeWidth;
			cubeY += incrementSpeed) {
				calculateForSurface(cubeX, cubeY, -cubeWidth, '@'); // 1st face
				calculateForSurface(cubeWidth, cubeY, cubeX, '$'); // 2nd face
				calculateForSurface(-cubeWidth, cubeY, -cubeX, '~'); // 3rd face
				calculateForSurface(-cubeX, cubeY, cubeWidth, '#'); // 4th face 
				calculateForSurface(cubeX, -cubeWidth, -cubeY, ';'); // 5th face
				calculateForSurface(cubeX, cubeWidth, cubeY, '+'); // 6th face
				
			}
		}

		std::cout<<"\x1b[H";
		for (int k = 0; k < width * height; k++) {
			putchar(k % width ? buffer[k] : 10);
		}
		
		// increase angles slowly to reduce the velocity of rotation 
		alpha += 0.05; // ( angle of rotation around x-axis ) 
		beta  += 0.05;
		gamma += 0.01;
		usleep(8000 * 2);
		//i++;
	}
} 
