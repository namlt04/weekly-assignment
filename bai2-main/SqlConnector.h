#pragma once
#include "cppconn/driver.h"
#include "cppconn/connection.h"
#include "cppconn/prepared_statement.h"
#include "cppconn/resultset.h"
#include <mutex>
#include <vector>
class SqlConnector
{


private:
	SqlConnector();
	static SqlConnector* instance;
	sql::Driver* driver;
	sql::Connection* conn;
	sql::PreparedStatement* pstm;
	sql::ResultSet* res;
	static std::mutex mtx;

public:
	static SqlConnector* getInstance();
	sql::Connection* getConnection();
	int Add(std::string Account, std::string HoTen, std::string QueQuan, std::string NgaySinh, int GioiTinh, std::string TruongHoc, std::string SoDienThoai);
	int Edit(std::string ID, std::string Account, std::string HoTen, std::string QueQuan, std::string NgaySinh, int GioiTinh, std::string TruongHoc, std::string SoDienThoai);
	BOOL Remove(std::vector<int> vt_Id);
	~SqlConnector();
	std::vector<std::vector<CString>> SqlConnector::getAllRecord();
};


