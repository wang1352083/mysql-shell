/* Copyright (c) 2014, 2015, Oracle and/or its affiliates. All rights reserved.

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2 of the License.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA */

#include "gtest/gtest.h"
#include "shellcore/server_registry.h"
#include "utils/utils_file.h"
#include "test_utils.h"

#include <stdexcept>

namespace shcore {
  namespace server_registry_tests
  {
    class Server_registry_test : public Shell_core_test_wrapper
    {
    protected:
      ngcommon::Logger* _logger;

      static std::string error;

      // You can define per-test set-up and tear-down logic as usual.
      virtual void SetUp()
      {
        Shell_core_test_wrapper::SetUp();

        std::string log_path = shcore::get_user_config_path();
        log_path += "mysqlsh.log";
        ngcommon::Logger::create_instance(log_path.c_str(), false, ngcommon::Logger::LOG_ERROR);
        _logger = ngcommon::Logger::singleton();
      }

      virtual void TearDown()
      {
      }
    };

    ngcommon::Logger* _logger;

    TEST(Server_registry_test, merge_file)
    {
      //const std::string file = Server_registry::get_default_config_path();
      const std::string file = "sample.json";
      shcore::delete_file(file);
      Server_registry* sr = new Server_registry(file);
      sr->load();
      Connection_options cs = sr->add_connection_options("dev_connection", "host=localhost; port=3305; dbUser=root; dbPassword=123; custom1=my custom value;;");
      // another way to add the connection, with autogenerated uuid.
      Connection_options cs2 = sr->add_connection_options("app1", "host=192.168.56.20; port=3306; dbUser=admin; dbPassword=456; ssl_ca=mysslca; ssl_cert=mysslcert; ssl_key=mysslkey; socket=mysocket;");
      // this saves the file in app data/mysql/mysqlx/.mysql_server_registry.cnf for windows
      // or $HOME/mysqlsh/.mysql_server_registry.cnf for Linux

      sr->merge();
      delete sr;

      Server_registry* sr2 = new Server_registry(file);
      sr2->load();

      Connection_options cs_bis = sr2->get_connection_options(cs.get_name());
      EXPECT_STREQ(cs_bis.get_server().c_str(), "localhost");
      EXPECT_STREQ(cs_bis.get_name().c_str(), "dev_connection");
      EXPECT_STREQ(cs_bis.get_user().c_str(), "root");
      EXPECT_STREQ(cs_bis.get_port().c_str(), "3305");
      EXPECT_THROW(cs_bis.get_value("custom1"), std::runtime_error);

      //Connection_options cs2_bis = sr2->get_connection_options(cs2.get_uuid());
      Connection_options cs2_bis = sr2->get_connection_options("app1");
      EXPECT_STREQ(cs2_bis.get_server().c_str(), "192.168.56.20");
      EXPECT_STREQ(cs2_bis.get_port().c_str(), "3306");
      EXPECT_STREQ(cs2_bis.get_user().c_str(), "admin");
      EXPECT_STREQ(cs2_bis.get_name().c_str(), "app1");
      EXPECT_STREQ(cs2_bis.get_value("ssl_ca").c_str(), "mysslca");
      EXPECT_STREQ(cs2_bis.get_value("ssl_cert").c_str(), "mysslcert");
      EXPECT_STREQ(cs2_bis.get_value("ssl_key").c_str(), "mysslkey");
      EXPECT_STREQ(cs2_bis.get_value("socket").c_str(), "mysocket");

      // Iterator of connections
      for (std::map<std::string, Connection_options>::const_iterator it = sr2->begin(); it != sr2->end(); ++it)
      {
        const Connection_options& csi = it->second;
        if (csi.get_uuid() == cs.get_uuid())
        {
          EXPECT_STREQ(csi.get_server().c_str(), "localhost");
          EXPECT_STREQ(csi.get_name().c_str(), "dev_connection");
          EXPECT_STREQ(csi.get_user().c_str(), "root");
          EXPECT_STREQ(csi.get_port().c_str(), "3305");
        }
        else
        {
          EXPECT_STREQ(csi.get_server().c_str(), "192.168.56.20");
          EXPECT_STREQ(csi.get_port().c_str(), "3306");
          EXPECT_STREQ(csi.get_user().c_str(), "admin");
          EXPECT_STREQ(csi.get_name().c_str(), "app1");
          EXPECT_STREQ(csi.get_value("ssl_ca").c_str(), "mysslca");
          EXPECT_STREQ(csi.get_value("ssl_cert").c_str(), "mysslcert");
          EXPECT_STREQ(csi.get_value("ssl_key").c_str(), "mysslkey");
          EXPECT_STREQ(csi.get_value("socket").c_str(), "mysocket");
        }
      }

      delete sr2;
    }

    TEST(Server_registry_test, other_merge)
    {
      Server_registry* sr = new Server_registry("sample.json");
      sr->load();

      Connection_options cs2 = sr->add_connection_options("app2", "host=192.168.56.20; port=3306; dbUser=admin; dbPassword=456");

      sr->merge();
      delete sr;

      Server_registry* sr2 = new Server_registry("sample.json");
      sr2->load();

      Connection_options cs2_bis = sr2->get_connection_options(cs2.get_name());
      EXPECT_STREQ(cs2_bis.get_server().c_str(), "192.168.56.20");
      EXPECT_STREQ(cs2_bis.get_port().c_str(), "3306");
      EXPECT_STREQ(cs2_bis.get_user().c_str(), "admin");
      EXPECT_STREQ(cs2_bis.get_name().c_str(), "app2");

      delete sr2;
    }

    TEST(Server_registry_test, add_override_update)
    {
      Server_registry* sr = new Server_registry("sample.json");
      sr->load();

      Connection_options cs1 = sr->add_connection_options("first_application", "host=192.168.56.20; port=3306; dbUser=admin; dbPassword=456");
      sr->merge();

      // Attempt to add existing connection
      try
      {
        Connection_options cs2 = sr->add_connection_options("first_application", "host=192.168.56.15; port=3306; dbUser=admin; dbPassword=456");
      }
      catch (shcore::Exception &err)
      {
        EXPECT_STREQ(err.what(), "The name 'first_application' already exists");
      }

      Connection_options cs3 = sr->get_connection_options("first_application");
      EXPECT_STREQ(cs3.get_server().c_str(), "192.168.56.20");

      // Now attempts again with override: updates data and UUID
      Connection_options cs2 = sr->add_connection_options("first_application", "host=192.168.56.15; port=3306; dbUser=admin; dbPassword=456", true);
      cs3 = sr->get_connection_options("first_application");
      EXPECT_STREQ(cs3.get_server().c_str(), "192.168.56.15");
      EXPECT_STRNE(cs2.get_uuid().c_str(), cs1.get_uuid().c_str());
      EXPECT_STREQ(cs2.get_uuid().c_str(), cs3.get_uuid().c_str());

      // Now attempts update: updates the data but the UUID remains
      Connection_options cs4 = sr->update_connection_options("first_application", "host=192.168.56.50; port=3306; dbUser=admin; dbPassword=456");
      Connection_options cs5 = sr->get_connection_options("first_application");
      EXPECT_STREQ(cs5.get_server().c_str(), "192.168.56.50");
      EXPECT_STREQ(cs2.get_uuid().c_str(), cs5.get_uuid().c_str());

      delete sr;
    }
  }
}