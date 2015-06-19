/*
 * Copyright (c) 2015, Oracle and/or its affiliates. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the
 * License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */


#ifndef _MYSQLX_CONNECTION_H_
#define _MYSQLX_CONNECTION_H_

#undef ERROR //Needed to avoid conflict with ERROR in mysqlx.pb.h
#include "mysqlx.pb.h"
#include "mysqlx_admin.pb.h"
#include "mysqlx_connection.pb.h"
#include "mysqlx_crud.pb.h"
#include "mysqlx_datatypes.pb.h"
#include "mysqlx_expr.pb.h"
#include "mysqlx_session.pb.h"
#include "mysqlx_sql.pb.h"

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>


#include "xerrmsg.h"

#define CR_UNKNOWN_ERROR        2000
#define CR_UNKNOWN_HOST         2005
#define CR_CONNECTION_ERROR     2002
#define CR_SERVER_GONE_ERROR    2006
#define CR_MALFORMED_PACKET     2027
#define CR_WRONG_HOST_INFO      2009
#define CR_COMMANDS_OUT_OF_SYNC 2014

namespace mysqlx
{
  class Connection
  {
  public:
    Connection();

    void connect(const std::string &uri, const std::string &pass);
    void connect(const std::string &host, int port);
    void connect(const std::string &host, int port, const std::string &schema,
                 const std::string &user, const std::string &pass);

    void close();

    void send(int mid, const Message &msg);
    Message *recv(int &mid);

    // Overrides for Client Session Messages
    void send(const Mysqlx::Session::AuthenticateStart &m) { send(Mysqlx::ClientMessages::SESS_AUTHENTICATE_START, m); };
    void send(const Mysqlx::Session::AuthenticateContinue &m) { send(Mysqlx::ClientMessages::SESS_AUTHENTICATE_CONTINUE, m); };
    void send(const Mysqlx::Session::Reset &m) { send(Mysqlx::ClientMessages::SESS_RESET, m); };
    void send(const Mysqlx::Session::Close &m) { send(Mysqlx::ClientMessages::SESS_CLOSE, m); };

    // Overrides for SQL Messages
    void send(const Mysqlx::Sql::PrepareStmt &m) { send(Mysqlx::ClientMessages::SQL_PREPARE_STMT, m); };
    void send(const Mysqlx::Sql::PreparedStmtClose &m) { send(Mysqlx::ClientMessages::SQL_PREPARED_STMT_CLOSE, m); };
    void send(const Mysqlx::Sql::PreparedStmtExecute &m) { send(Mysqlx::ClientMessages::SQL_PREPARED_STMT_EXECUTE, m); };
    void send(const Mysqlx::Sql::CursorFetchMetaData &m) { send(Mysqlx::ClientMessages::SQL_CURSOR_FETCH_META_DATA, m); };
    void send(const Mysqlx::Sql::CursorFetchRows &m) { send(Mysqlx::ClientMessages::SQL_CURSOR_FETCH_ROWS, m); };
    void send(const Mysqlx::Sql::CursorsPoll &m) { send(Mysqlx::ClientMessages::SQL_CURSORS_POLL, m); };
    void send(const Mysqlx::Sql::CursorClose &m) { send(Mysqlx::ClientMessages::SQL_CURSOR_CLOSE, m); };
    void send(const Mysqlx::Sql::StmtExecute &m) { send(Mysqlx::ClientMessages::SQL_STMT_EXECUTE, m); };

    // Overrides for CRUD operations
    void send(const Mysqlx::Crud::PrepareFind &m) { send(Mysqlx::ClientMessages::CRUD_PREPARE_FIND, m); };
    void send(const Mysqlx::Crud::PrepareInsert &m) { send(Mysqlx::ClientMessages::CRUD_PREPARE_INSERT, m); };
    void send(const Mysqlx::Crud::PrepareUpdate &m) { send(Mysqlx::ClientMessages::CRUD_PREPARE_UPDATE, m); };
    void send(const Mysqlx::Crud::PrepareDelete &m) { send(Mysqlx::ClientMessages::CRUD_PREPARE_DELETE, m); };
    void send(const Mysqlx::Crud::Find &m) { send(Mysqlx::ClientMessages::CRUD_FIND, m); };
    void send(const Mysqlx::Crud::Insert &m) { send(Mysqlx::ClientMessages::CRUD_INSERT, m); };
    void send(const Mysqlx::Crud::Update &m) { send(Mysqlx::ClientMessages::CRUD_UPDATE, m); };
    void send(const Mysqlx::Crud::Delete &m) { send(Mysqlx::ClientMessages::CRUD_DELETE, m); };

    // Overrides for Connection
    void send(const Mysqlx::Connection::CapabilitiesGet &m) { send(Mysqlx::ClientMessages::CON_CAPABILITIES_GET, m); };
    void send(const Mysqlx::Connection::CapabilitiesSet &m) { send(Mysqlx::ClientMessages::CON_CAPABILITIES_SET, m); };
    void send(const Mysqlx::Connection::Close &m) { send(Mysqlx::ClientMessages::CON_CLOSE, m); };

    boost::asio::ip::tcp::socket &socket() { return m_socket; }
  public:
    Result *execute_sql(const std::string &sql);

    Result *execute_find(const Mysqlx::Crud::Find &m);
    Result *execute_update(const Mysqlx::Crud::Update &m);
    Result *execute_insert(const Mysqlx::Crud::Insert &m);
    Result *execute_delete(const Mysqlx::Crud::Delete &m);

  private:
    void authenticate_plain(const std::string &user, const std::string &pass, const std::string &db);

  private:
    typedef boost::asio::ip::tcp tcp;
    
    boost::asio::io_service m_ios;
    boost::asio::ip::tcp::socket m_socket;
    bool m_trace_packets;
  };
  
  typedef boost::shared_ptr<Connection> ConnectionRef;

}

#endif