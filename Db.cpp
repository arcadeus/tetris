#include "pch.h"
#include <stdexcept>
#include "Db.h"

Db::Db(CSimpleIniA& config)
{
	m_conn = mysql_init(NULL);
	if (m_conn == NULL)
		throw std::runtime_error("Could not init DB");

	const std::string host = config.GetValue("mysql", "host", "localhost");
	const std::string user = config.GetValue("mysql", "user", "");
	const std::string pass = config.GetValue("mysql", "pass", "");
	const std::string db   = config.GetValue("mysql", "db", "tetris");

	if (mysql_real_connect(m_conn, host.c_str(), user.c_str(), pass.c_str(), "mysql", 0, NULL, 0) == NULL)
		throw std::runtime_error("DB Connection Error");

	if (mysql_query(m_conn, ("USE " + db).c_str()) != 0)
	{
		if (mysql_query(m_conn, ("CREATE DATABASE " + db).c_str()) != 0)
			throw std::runtime_error(("Failed to create database " + db).c_str());

		if (mysql_query(m_conn, ("USE " + db).c_str()) != 0)
			throw std::runtime_error("Faiuled to USE " + db);

		mysql_query(
			m_conn,
			"CREATE TABLE `scores` (\r\n"
			"   `stamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,\r\n"
			"   `score` int unsigned NOT NULL,\r\n"
			"   `duration` int unsigned NOT NULL,\r\n"
			"   PRIMARY KEY(`stamp`)\r\n"
			")"
		);
	}
}

void Db::query(const std::string& sql)
{
	mysql_query(m_conn, sql.c_str());
}

MYSQL_RES* Db::get_result()
{
	return mysql_store_result(m_conn);
}

Db::~Db()
{
	mysql_close(m_conn);
	m_conn = nullptr;
}
