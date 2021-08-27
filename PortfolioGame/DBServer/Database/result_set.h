#pragma once
class PreparedStatement;

class ResultSet
{
public:
	ResultSet(const ResultSet&) = delete;
	ResultSet& operator=(const ResultSet&) = delete;
public:
	explicit ResultSet(PreparedStatement& ps);
	~ResultSet();

	bool Fetch() const;
	void Close() const;
	void GetInt(int index, int* return_value) const;
	void GetBigInt(int index, long long* return_value) const;
	void GetDouble(int index, double* return_value) const;
	void GetString(int index, char* return_buffer, size_t size) const;
private:
	PreparedStatement& _ps;
};

