#include "pch.h"
#include "SqlConnector.h"

SqlConnector* SqlConnector::instance = nullptr;
std::mutex SqlConnector::mtx; 
SqlConnector::SqlConnector()
{

	try
	{
		driver = get_driver_instance();
		conn = driver->connect("tcp://127.0.0.1:3306", "namltd", "0000"); 
		conn->setSchema("namltd");
		conn->setClientOption("characterEncoding", "utf8mb4");
		std::string query = "create table if not exists tb_NhanVien("
			"ID INTEGER AUTO_INCREMENT PRIMARY KEY,"
			"Account VARCHAR(100),"
			"HoTen VARCHAR(100),"
			"QueQuan VARCHAR(100),"
			"NgaySinh DATETIME,"
			"GioiTinh INTEGER,"
			"TruongHoc VARCHAR(100),"
			"SoDienThoai VARCHAR(200)"
			"); ";
		pstm = conn->prepareStatement(query);
		pstm->executeUpdate(); 
	}
	catch (sql::SQLException* e)
	{
		// Không làm gì cả  noti
		AfxMessageBox(_T("Loi"));
	}
}

SqlConnector::~SqlConnector()
{
	delete conn;
	delete pstm; 
	delete res; 
}

SqlConnector* SqlConnector::getInstance()
{
	std::lock_guard<std::mutex> lock(mtx); 
	
	if (!instance)
	{

		// Nếu instance chưa khởi tạo
		instance = new SqlConnector(); 
	}
	// Nếu đã tồn tại 
	return instance;

}


sql::Connection* SqlConnector::getConnection()
{
	return conn; 
}
std::vector<std::vector<std::string>> SqlConnector::getAllRecord()
{
	std::vector < std::vector<std::string>> vt;
	pstm = conn->prepareStatement("Select * from tb_NhanVien ORDER BY ID DESC;");
	res = pstm->executeQuery(); 
	while (res->next())
	{
		std::vector<std::string> tmp;
		for (int i = 1; i <= 8; i++)
		{
			std::string text = res->getString(i); 
			tmp.push_back(text);
		}
		vt.push_back(tmp); 
		
	}
	return vt;
}
std::vector<std::string> SqlConnector::Add(std::vector<std::string>& vt_Info)
{
	// Thêm vào trong database	
	std::string query = "insert into tb_NhanVien(Account,HoTen,QueQuan,NgaySinh,GioiTinh,TruongHoc,SoDienThoai) values(?,?,?,?,?,?,?)"; 
	pstm = conn->prepareStatement(query);
 
	for (int i = 1; i <= 7; i++)
	{
		if (i == 4)
			pstm->setDateTime(i, vt_Info[i - 1]);
		else if (i == 5)
			pstm->setInt(5, std::stoi(vt_Info[i - 1]));

		else
			pstm->setString(i, vt_Info[i - 1]);
	}
	pstm->executeUpdate(); 

	// Lấy ID của record vừa thêm vào
	std::string queryId = "select last_insert_id();";
	pstm = conn->prepareStatement(queryId); 
	res = pstm->executeQuery(); 
	res->next(); 

	// Trả về 1 vector
	vt_Info.insert(vt_Info.begin(), res->getString(1)); 
	// ket qua tra ve bắt đầu từ 1

	return vt_Info;

}
int SqlConnector::Edit(std::vector<std::string>& vt_Info)
{
	std::string query = "update tb_NhanVien set Account = ?, HoTen = ?, QueQuan = ?, NgaySinh = ?, GioiTinh = ?, TruongHoc = ?, SoDienThoai = ?  where ID = ?"; 

	pstm = conn->prepareStatement(query);
	
	// vector gửi về bao gồm cả trường ID, do đó bắt đầu từ 1
	for (int i = 0; i <= 7; i++)
	{
		if (i == 0)
			pstm->setInt(i + 1, std::stoi(vt_Info[i])); 
		else if (i == 4)
			pstm->setDateTime(i + 1, vt_Info[i]);
		else if (i == 5)
			pstm->setInt(i + 1, std::stoi(vt_Info[i]));
		else
			pstm->setString(i + 1, vt_Info[i]);
	}
	pstm->executeUpdate();
	return pstm->executeUpdate();
	
}
BOOL SqlConnector::Remove(std::vector<int> vt_Id)
{
	std::string query = "delete from tb_NhanVien where ID = ? ";
	pstm = conn->prepareStatement(query);
	for (int id : vt_Id)
	{
		pstm->setInt(1, id);
		pstm->executeUpdate(); 
	}
	return TRUE; 
}
