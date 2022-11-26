#include "Variable.hpp"

std::optional<double> scratch::Variable::into_number() const
{
	if (m_tag == None) {
		return std::nullopt;
	}
	if (m_tag == Double) {
		return this->m_double;
	}
	if (m_tag == Integer) {
		return static_cast<double>(this->m_integer);
	}
	if (m_tag == String) {
		try {
			return std::stod(*this->m_string);
		}
		catch (std::exception& e) {
			return std::nullopt;
		}
	}
}

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
	std::scoped_lock<std::mutex> this_lock(this->mutex);
	std::scoped_lock<std::mutex> other_lock(other.mutex);

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
	std::scoped_lock<std::mutex> this_lock(this->mutex);
	std::scoped_lock<std::mutex> other_lock(other.mutex);

	if (other.m_tag == None || this->m_tag == None) {
		return false;
	}
	const auto& this_number = this->into_number();
	const auto& other_number = other.into_number();
	if (this_number.has_value() && other_number.has_value()) {
		return this_number.value() == other_number.value();
	}
	if (this->m_tag == String && other.m_tag == String) {
		return this->m_string->compare(*other.m_string) == 0;
	}

	return false;
}
