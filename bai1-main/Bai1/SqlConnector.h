#pragma once
#include "cppconn/driver.h"
#include "cppconn/connection.h"
#include "cppconn/prepared_statement.h"
#include "cppconn/resultset.h"
#include <mutex>
#include <vector>
#include "Information.h"
class SqlConnector
{


private : 
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
	int Edit(std::vector<std::string>& vt_Info);
	BOOL Remove(std::vector<int> vt_Id); 
	~SqlConnector(); 
	std::vector<std::vector<std::string>> SqlConnector::getAllRecord(); 
	std::vector<std::string> Add(std::vector<std::string>& vt_Info);
};

