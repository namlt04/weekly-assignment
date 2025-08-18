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

std::vector<std::vector<CString>> SqlConnector::getAllRecord()
{
	std::vector<std::vector<CString>> vt;
	pstm = conn->prepareStatement("Select * from tb_NhanVien ORDER BY ID DESC;");
	res = pstm->executeQuery();
	while (res->next())
	{
		std::vector<CString> tmp;
		for (int i = 1; i <= 8; i++)
		{
			std::string text = res->getString(i);

			CString conv = CA2T(text.c_str(), CP_UTF8);
			tmp.push_back(conv);

		}
		vt.push_back(tmp);

	}
	//CString noti;
	//noti.Format(_T("%d"), vt.size()); 
	//AfxMessageBox(noti);
	return vt;
}
int SqlConnector::Add(std::string Account, std::string HoTen, std::string QueQuan, std::string NgaySinh, int GioiTinh, std::string TruongHoc, std::string SoDienThoai)
{
	std::string name = "hehe";

	std::string query = "insert into tb_NhanVien(Account,HoTen,QueQuan,NgaySinh,GioiTinh,TruongHoc,SoDienThoai) values(?,?,?,?,?,?,?)";
	pstm = conn->prepareStatement(query);
	pstm->setString(1, Account);
	pstm->setString(2, HoTen);
	pstm->setString(3, QueQuan);
	pstm->setDateTime(4, NgaySinh);
	pstm->setInt(5, GioiTinh);

	pstm->setString(6, TruongHoc);
	pstm->setString(7, SoDienThoai);
	pstm->executeQuery();


	// Lấy ID của record vừa thêm vào
	std::string queryId = "select last_insert_id();";
	pstm = conn->prepareStatement(queryId);
	res = pstm->executeQuery();
	res->next();

	// Trả về 1 vector
	return res->getInt(1);

}
int SqlConnector::Edit(std::string ID, std::string Account, std::string HoTen, std::string QueQuan, std::string NgaySinh, int GioiTinh, std::string TruongHoc, std::string SoDienThoai)
{
	std::string query = "update tb_NhanVien set Account = ?, HoTen = ?, QueQuan = ?, NgaySinh = ?, GioiTinh = ?, TruongHoc = ?, SoDienThoai = ?  where ID = ?";

	pstm = conn->prepareStatement(query);
	pstm->setString(1, Account);
	pstm->setString(2, HoTen);
	pstm->setString(3, QueQuan);
	pstm->setDateTime(4, NgaySinh);
	pstm->setInt(5, GioiTinh);

	pstm->setString(6, TruongHoc);
	pstm->setString(7, SoDienThoai);
	pstm->setString(8, ID);
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
