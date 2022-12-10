namespace scratch {
// lists are 1-indexed
class List {
private:
	
public:
	List();
	List(std::initalizer_list<Variable> list);
	
	void add(const Variable& elem);
	void delete(std::size_t index);
	void delete_all();
	void insert(const Variable& elem, std::size_t index);
	void replace(std::size_t index, const Variable& elem);
	Variable& item(std::size_t index);
	std::size_t item_number(const Variable& elem);
	std::size_t length();
	bool contains(const Variable& elem);
};
};