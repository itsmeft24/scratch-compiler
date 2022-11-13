#include "Variable.hpp"

scratch::Variable::Variable()
{
	m_tag = None;
	m_integer = 0;
}

scratch::Variable::Variable(const Variable& other)
{
	if (other.m_tag == None) {
		this->m_tag = None;
		this->m_integer = 0;
		return;
	}
	if (other.m_tag == Integer || other.m_tag == Double) {
		this->m_tag = other.m_tag;
		this->m_integer = other.m_integer;
	}
	if (other.m_tag == String) {
		this->m_tag = String;
		this->m_string = new std::string(*other.m_string);
	}
}

scratch::Variable::Variable(long long integer)
{
	this->m_tag = Integer;
	this->m_integer = integer;
}

scratch::Variable::Variable(int integer)
{
	this->m_tag = Integer;
	this->m_integer = integer;
}

scratch::Variable::Variable(double double_value)
{
	this->m_tag = Double;
	this->m_double = double_value;
}

scratch::Variable::Variable(const std::string& string)
{
	this->m_tag = String;
	this->m_string = new std::string(string);
}

scratch::Variable::~Variable()
{
	if (this->m_tag == String) {
		delete this->m_string;
		this->m_string = nullptr;
	}
	this->m_tag = None;
}

scratch::Variable& scratch::Variable::operator=(const Variable& other)
{
	if (this->m_tag == String) {
		delete this->m_string;
		this->m_string = nullptr;
	}
	
	if (other.m_tag == None) {
		this->m_tag = None;
		this->m_integer = 0;
	}
	if (other.m_tag == Integer || other.m_tag == Double) {
		this->m_tag = other.m_tag;
		this->m_integer = other.m_integer;
	}
	if (other.m_tag == String) {
		this->m_tag = String;
		this->m_string = new std::string(*other.m_string);
	}

	return *this;
}

bool scratch::Variable::operator==(const Variable& other)
{
	if (other.m_tag == None || this->m_tag == None) {
		return false;
	}

	return false;
}
