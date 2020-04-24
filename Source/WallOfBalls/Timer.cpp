#include <windows.h>

#include "DataTypes.h"

timer_c::timer_c()
{
	m_start=timeGetTime();
	m_elapsed_s=(timeGetTime()-m_start)/1000.0;
	m_elapsed_ms=(timeGetTime()-m_start);
}

timer_c::~timer_c() {}

void timer_c::start(void)
{
	m_start=timeGetTime();
	m_elapsed_s=(timeGetTime()-m_start)/1000.0;
	m_elapsed_ms=(timeGetTime()-m_start);
}

double timer_c::getElapsedS(void)
{
	return m_elapsed_s;
}

int timer_c::getElapsedMS(void)
{
	return m_elapsed_ms;
}

void timer_c::update(void)
{
	m_elapsed_s=double(timeGetTime()-m_start)/1000.0;
	m_elapsed_ms=(timeGetTime()-m_start);
}