#pragma once

#include <string>		
#include <sstream>
#include <iostream>
#include <fstream>
#include <tchar.h>
using namespace std;

// Extra unicode defines
#ifdef _UNICODE	
#    define tstring wstring
#    define tcin wcin
#    define tcout std::wcout
#    define tstringstream std::wstringstream
#    define tofstream std::wofstream
#    define tifstream std::wifstream
#    define tfstream std::wfstream
#    define to_tstring std::to_wstring
#    define t_fopen_s std::_wfopen_s
#else
#    define tstring std::string
#    define tcin std::cin
#    define tcout std::cout
#    define tstringstream std::stringstream
#    define tofstream std::ofstream
#    define tifstream std::ifstream
#    define tfstream std::fstream
#    define to_tstring std::to_string
#    define t_fopen_s std::fopen_s
#endif

// ASSERT macro (assert())
/*
#ifndef NDEBUG
#define ASSERT \
	if ( false ) {} \
else \
struct LocalAssert { \
	int mLine; \
	LocalAssert(int line=__LINE__) : mLine(line) {} \
	LocalAssert(bool isOK, const TCHAR* message=_T("")) { \
	if ( !isOK ) { \
	tstringstream buffer; \
	buffer << _T("ERROR!! Assert failed on line ") << LocalAssert().mLine << _T(" in file '") << __FILE__ << _T("'\nBoodschap: \"") << message << _T("\"\n"); \
	OutputDebugString(buffer.str().c_str()); \
	__asm { int 3 } \
	} \
} \
} myAsserter = LocalAssert
#else
#define ASSERT \
	if ( true ) {} else \
struct NoAssert { \
	NoAssert(bool isOK, const TCHAR* message=_T("")) {} \
} myAsserter = NoAssert
#endif

#define GAME_ENGINE (GameEngine::GetSingleton())
*/



void AddThickLine(sf::VertexArray& vertices, const sf::Vector2f& point1, const sf::Vector2f& point2);
bool AreLinqued(b2Body* bodyA, b2Body* bodyB);
bool CCW(b2Vec2 p1, b2Vec2 p2, b2Vec2 p3);
bool ConnectTry(b2Body* bodyA, b2Body* bodyB); // Gameplay safties
bool ConnectBodys(b2Body* bodyA, b2Body* bodyB); // Just Do it!
unsigned int CountJoints(b2Body* body);
bool isIntersect(b2Vec2 p1, b2Vec2 p2, b2Vec2 q3, b2Vec2 q4);
bool FileExists(const string& filename);
bool FileExists(const wstring& filename);