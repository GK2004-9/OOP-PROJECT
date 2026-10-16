#pragma once

#include "Database.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace MySql::Data::MySqlClient;

namespace SmartBusSeatBookingSystem
{
    public ref class ViewBookingsForm : public Form
    {
    private:
        DataGridView^ dgvBookings;
        Button^ btnRefresh;
        Button^ btnBack;

    public:
        ViewBookingsForm()
        {
            this->Text = "View Booking Details";
            this->Size = Drawing::Size(800, 500);
            this->StartPosition =
                FormStartPosition::CenterScreen;

            Label^ title = gcnew Label();
            title->Text = "BOOKING DETAILS";
            title->Font = gcnew Drawing::Font(
                "Arial", 16, FontStyle::Bold);
            title->AutoSize = true;
            title->Location = Point(280, 20);
            this->Controls->Add(title);

            dgvBookings = gcnew DataGridView();
            dgvBookings->Location = Point(20, 70);
            dgvBookings->Size = Drawing::Size(740, 300);
            dgvBookings->ReadOnly = true;
            dgvBookings->AllowUserToAddRows = false;
            dgvBookings->AutoSizeColumnsMode =
                DataGridViewAutoSizeColumnsMode::Fill;

            this->Controls->Add(dgvBookings);

            btnRefresh = gcnew Button();
            btnRefresh->Text = "Refresh";
            btnRefresh->Location = Point(250, 395);
            btnRefresh->Size = Drawing::Size(120, 40);
            btnRefresh->Click += gcnew EventHandler(
                this, &ViewBookingsForm::btnRefresh_Click);
            this->Controls->Add(btnRefresh);

            btnBack = gcnew Button();
            btnBack->Text = "Back";
            btnBack->Location = Point(410, 395);
            btnBack->Size = Drawing::Size(120, 40);
            btnBack->Click += gcnew EventHandler(
                this, &ViewBookingsForm::btnBack_Click);
            this->Controls->Add(btnBack);

            LoadBookings();
        }

    private:
        void LoadBookings()
        {
            MySqlConnection^ con = nullptr;
            MySqlDataReader^ reader = nullptr;

            try
            {
                con = Database::GetConnection();
                con->Open();

                String^ query =
                    "SELECT b.BookingID, b.PassengerName, "
                    "bus.BusNumber, b.SeatNumber, b.BookingDate "
                    "FROM Bookings b "
                    "INNER JOIN Buses bus ON b.BusID = bus.BusID "
                    "ORDER BY b.BookingID DESC";

                MySqlCommand^ cmd =
                    gcnew MySqlCommand(query, con);

                reader = cmd->ExecuteReader();

                System::Data::DataTable^ table =
                    gcnew System::Data::DataTable();

                table->Load(reader);
                dgvBookings->DataSource = table;
            }
            catch (Exception^ ex)
            {
                MessageBox::Show(
                    "Unable to load bookings.\n" + ex->Message,
                    "Database Error");
            }
            finally
            {
                if (reader != nullptr)
                {
                    reader->Close();
                    delete reader;
                }

                if (con != nullptr)
                {
                    con->Close();
                    delete con;
                }
            }
        }

        void btnRefresh_Click(Object^ sender, EventArgs^ e)
        {
            LoadBookings();
        }

        void btnBack_Click(Object^ sender, EventArgs^ e)
        {
            this->Close();
        }
    };
}