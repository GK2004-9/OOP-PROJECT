#pragma once
using namespace System;
using namespace MySql::Data::MySqlClient;
namespace SmartBusSeatBookingSystem {
    public ref class Database abstract sealed {
    public:
        // Replace YOUR_PASSWORD
        static MySqlConnection^ GetConnection() {
            return gcnew MySqlConnection(
                "Server=localhost;Port=3306;Database=smart_bus_db;Uid=root;Pwd=YOUR_PASSWORD;");
        }
    };
}
