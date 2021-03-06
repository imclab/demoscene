//: src:ResourcePsychoCircles.h
// thisisnotanengine
// @author: Soledad Penades
// @date_start: 11 august 2005  
// Declaration of CResourcePsychoCircles class

#ifndef RESOURCEPSYCHOCIRCLES_H
#define RESOURCEPSYCHOCIRCLES_H

#include "Resource.h"
#include "Algebra.h"


class CResourcePsychoCircles : public CResource {
private:
	// color
	CPoint m_Color;
	float m_radio;
	unsigned int m_numVertex;
	unsigned int m_numX;
	unsigned int m_numY;
	
	void drawCircle(CPoint center, float radio, unsigned int numVertex);
public:
		CResourcePsychoCircles();
	void play(float time);
	void init(void);
	void start(void);
	void deInit(void);
	string getType(void);
	void setParam(string name, string value);
};

#endif
