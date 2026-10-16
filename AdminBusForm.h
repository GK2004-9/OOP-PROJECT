
#pragma once

#include "Database.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace MySql::Data::MySqlClient;

namespace SmartBusSeatBookingSystem {

    public ref class AdminBusForm : public Form
    {
    private:
        TextBox^ txtID;
        TextBox^ txtNumber;
        TextBox^ txtFrom;
        TextBox^ txtTo;
        TextBox^ txtTime;
        TextBox^ txtSeats;
        TextBox^ txtFare;

        DataGridView^ grid;

        Color navy;
        Color teal;

    public:
        AdminBusForm()
        {
            navy = Color::FromArgb(39, 29, 83);
            teal = Color::FromArgb(0, 150, 136);

            BuildUI();
            LoadBuses();
        }

    private:

        // CREATE LABEL
        Label^ MakeLabel(String^ text, int x, int y)
        {
            Label^ l = gcnew Label();

            l->Text = text;
            l->Location = Point(x, y);
            l->Size = Drawing::Size(140, 25);
            l->Font = gcnew Drawing::Font("Segoe UI", 10);

            return l;
        }

        // CREATE TEXTBOX
        TextBox^ MakeTextBox(int x, int y)
        {
            TextBox^ t = gcnew TextBox();

            t->Location = Point(x, y);
            t->Size = Drawing::Size(220, 30);
            t->Font = gcnew Drawing::Font("Segoe UI", 10);

            return t;
        }

        // CREATE BUTTON
        Button^ MakeButton(
            String^ text,
            int x,
            int y,
            Color color,
            EventHandler^ handler)
        {
            Button^ b = gcnew Button();

            b->Text = text;
            b->Location = Point(x, y);
            b->Size = Drawing::Size(150, 43);

            b->BackColor = color;
            b->ForeColor = Color::White;

            b->FlatStyle = FlatStyle::Flat;
            b->FlatAppearance->BorderSize = 0;

            b->Font = gcnew Drawing::Font(
                "Segoe UI", 10, FontStyle::Bold);

            b->Click += handler;

            return b;
        }

        // DESIGN ADMIN SCREEN
        void BuildUI()
        {
            this->Text = "SmartBus - Admin Bus Management";
            this->ClientSize = Drawing::Size(1100, 760);
            this->StartPosition = FormStartPosition::CenterScreen;
            this->BackColor = Color::WhiteSmoke;

            // HEADER
            Panel^ header = gcnew Panel();

            header->Dock = DockStyle::Top;
            header->Height = 85;
            header->BackColor = navy;

            this->Controls->Add(header);

            // TITLE
            Label^ title = gcnew Label();

            title->Text = "ADMIN - BUS MANAGEMENT";
            title->Location = Point(25, 20);
            title->Size = Drawing::Size(700, 45);

            title->ForeColor = Color::White;

            title->Font = gcnew Drawing::Font(
                "Segoe UI", 22, FontStyle::Bold);

            header->Controls->Add(title);

            // BACK BUTTON - NEW
            Button^ backButton = gcnew Button();

            backButton->Text = "< BACK";
            backButton->Location = Point(920, 20);
            backButton->Size = Drawing::Size(145, 43);

            backButton->BackColor = teal;
            backButton->ForeColor = Color::White;

            backButton->Font = gcnew Drawing::Font(
                "Segoe UI", 11, FontStyle::Bold);

            backButton->FlatStyle = FlatStyle::Flat;
            backButton->FlatAppearance->BorderSize = 0;

            backButton->Click += gcnew EventHandler(
                this, &AdminBusForm::GoBack);

            header->Controls->Add(backButton);

            // BUS ID
            this->Controls->Add(
                MakeLabel("Bus ID", 35, 115));

            txtID = MakeTextBox(180, 110);
            txtID->ReadOnly = true;
            this->Controls->Add(txtID);

            // BUS NUMBER
            this->Controls->Add(
                MakeLabel("Bus Number", 35, 160));

            txtNumber = MakeTextBox(180, 155);
            this->Controls->Add(txtNumber);

            // FROM CITY
            this->Controls->Add(
                MakeLabel("From City", 35, 205));

            txtFrom = MakeTextBox(180, 200);
            this->Controls->Add(txtFrom);

            // TO CITY
            this->Controls->Add(
                MakeLabel("To City", 35, 250));

            txtTo = MakeTextBox(180, 245);
            this->Controls->Add(txtTo);

            // DEPARTURE TIME
            this->Controls->Add(
                MakeLabel("Departure Time", 500, 115));

            txtTime = MakeTextBox(650, 110);
            this->Controls->Add(txtTime);

            // TOTAL SEATS
            this->Controls->Add(
                MakeLabel("Total Seats", 500, 160));

            txtSeats = MakeTextBox(650, 155);
            this->Controls->Add(txtSeats);

            // FARE
            this->Controls->Add(
                MakeLabel("Fare (Rs.)", 500, 205));

            txtFare = MakeTextBox(650, 200);
            this->Controls->Add(txtFare);

            // ADD BUS
            this->Controls->Add(
                MakeButton(
                    "ADD BUS",
                    35, 315,
                    teal,
                    gcnew EventHandler(
                        this, &AdminBusForm::AddBus)
                )
            );

            // VIEW BUSES
            this->Controls->Add(
                MakeButton(
                    "VIEW BUSES",
                    200, 315,
                    navy,
                    gcnew EventHandler(
                        this, &AdminBusForm::RefreshBuses)
                )
            );

            // UPDATE BUS
            this->Controls->Add(
                MakeButton(
                    "UPDATE BUS",
                    365, 315,
                    Color::DarkOrange,
                    gcnew EventHandler(
                        this, &AdminBusForm::UpdateBus)
                )
            );

            // DELETE BUS
            this->Controls->Add(
                MakeButton(
                    "DELETE BUS",
                    530, 315,
                    Color::Firebrick,
                    gcnew EventHandler(
                        this, &AdminBusForm::DeleteBus)
                )
            );

            // CLEAR FIELDS
            this->Controls->Add(
                MakeButton(
                    "CLEAR",
                    695, 315,
                    Color::SlateGray,
                    gcnew EventHandler(
                        this, &AdminBusForm::ClearFields)
                )
            );

            // DATA GRID
            grid = gcnew DataGridView();

            grid->Location = Point(30, 390);
            grid->Size = Drawing::Size(1040, 335);

            grid->BackgroundColor = Color::White;

            grid->AutoSizeColumnsMode =
                DataGridViewAutoSizeColumnsMode::Fill;

            grid->SelectionMode =
                DataGridViewSelectionMode::FullRowSelect;

            grid->MultiSelect = false;
            grid->ReadOnly = true;
            grid->AllowUserToAddRows = false;
            grid->RowHeadersVisible = false;

            grid->CellClick +=
                gcnew DataGridViewCellEventHandler(
                    this, &AdminBusForm::SelectRow);

            this->Controls->Add(grid);
        }

        // BACK TO MAIN SCREEN
        void GoBack(Object^ sender, EventArgs^ e)
        {
            this->Close();
        }

        // READ - LOAD BUSES
        void LoadBuses()
        {
            try
            {
                MySqlConnection^ con =
                    Database::GetConnection();

                con->Open();

                MySqlDataAdapter^ adapter =
                    gcnew MySqlDataAdapter(
                        "SELECT BusID, BusNumber, "
                        "FromCity, ToCity, "
                        "DepartureTime, TotalSeats, Fare "
                        "FROM Buses ORDER BY BusID",
                        con);

                System::Data::DataTable^ table =
                    gcnew System::Data::DataTable();

                adapter->Fill(table);

                grid->DataSource = table;

                con->Close();
            }
            catch (Exception^ ex)
            {
                MessageBox::Show(
                    "Could not load buses:\n" +
                    ex->Message);
            }
        }

        // REFRESH BUSES
        void RefreshBuses(Object^ sender, EventArgs^ e)
        {
            LoadBuses();
        }

        // SELECT ROW
        void SelectRow(
            Object^ sender,
            DataGridViewCellEventArgs^ e)
        {
            if (e->RowIndex < 0)
                return;

            DataGridViewRow^ row =
                grid->Rows[e->RowIndex];

            txtID->Text = Convert::ToString(
                row->Cells["BusID"]->Value);

            txtNumber->Text = Convert::ToString(
                row->Cells["BusNumber"]->Value);

            txtFrom->Text = Convert::ToString(
                row->Cells["FromCity"]->Value);

            txtTo->Text = Convert::ToString(
                row->Cells["ToCity"]->Value);

            txtTime->Text = Convert::ToString(
                row->Cells["DepartureTime"]->Value);

            txtSeats->Text = Convert::ToString(
                row->Cells["TotalSeats"]->Value);

            txtFare->Text = Convert::ToString(
                row->Cells["Fare"]->Value);
        }

        // VALIDATE FIELDS
        bool ValidateFields()
        {
            if (String::IsNullOrWhiteSpace(txtNumber->Text) ||
                String::IsNullOrWhiteSpace(txtFrom->Text) ||
                String::IsNullOrWhiteSpace(txtTo->Text) ||
                String::IsNullOrWhiteSpace(txtTime->Text))
            {
                MessageBox::Show(
                    "Please fill all bus details.");
                return false;
            }

            int seats;
            Decimal price;

            if (!Int32::TryParse(txtSeats->Text, seats) ||
                seats <= 0)
            {
                MessageBox::Show(
                    "Enter valid total seats.");
                return false;
            }

            if (!Decimal::TryParse(txtFare->Text, price) ||
                price < Decimal::Zero)
            {
                MessageBox::Show(
                    "Enter valid fare.");
                return false;
            }

            if (txtFrom->Text->Trim()->Equals(
                txtTo->Text->Trim(),
                StringComparison::OrdinalIgnoreCase))
            {
                MessageBox::Show(
                    "From City and To City must be different.");
                return false;
            }

            return true;
        }

        // CREATE - ADD BUS
        void AddBus(Object^ sender, EventArgs^ e)
        {
            if (!ValidateFields())
                return;

            try
            {
                MySqlConnection^ con =
                    Database::GetConnection();

                con->Open();

                MySqlCommand^ cmd =
                    gcnew MySqlCommand(
                        "INSERT INTO Buses "
                        "(BusNumber, Route, FromCity, ToCity, "
                        "DepartureTime, TotalSeats, Fare) "
                        "VALUES "
                        "(@n, @r, @f, @t, @time, @seats, @fare)",
                        con);

                AddBusParameters(cmd);

                cmd->ExecuteNonQuery();
                con->Close();

                MessageBox::Show(
                    "Bus added successfully!");

                LoadBuses();
                ResetFields();
            }
            catch (Exception^ ex)
            {
                MessageBox::Show(
                    "Add Bus failed:\n" +
                    ex->Message);
            }
        }

        // UPDATE - EDIT BUS
        void UpdateBus(Object^ sender, EventArgs^ e)
        {
            if (String::IsNullOrWhiteSpace(txtID->Text))
            {
                MessageBox::Show(
                    "Select a bus from the table.");
                return;
            }

            if (!ValidateFields())
                return;

            try
            {
                MySqlConnection^ con =
                    Database::GetConnection();

                con->Open();

                MySqlCommand^ check =
                    gcnew MySqlCommand(
                        "SELECT COALESCE(MAX(SeatNumber), 0) "
                        "FROM Bookings "
                        "WHERE BusID=@id "
                        "AND BookingStatus='Confirmed'",
                        con);

                check->Parameters->AddWithValue(
                    "@id", Convert::ToInt32(txtID->Text));

                int maxBookedSeat =
                    Convert::ToInt32(
                        check->ExecuteScalar());

                if (Convert::ToInt32(
                    txtSeats->Text) < maxBookedSeat)
                {
                    con->Close();

                    MessageBox::Show(
                        "Cannot reduce seats below an "
                        "existing confirmed seat number.");
                    return;
                }

                MySqlCommand^ cmd =
                    gcnew MySqlCommand(
                        "UPDATE Buses SET "
                        "BusNumber=@n, Route=@r, "
                        "FromCity=@f, ToCity=@t, "
                        "DepartureTime=@time, "
                        "TotalSeats=@seats, Fare=@fare "
                        "WHERE BusID=@id",
                        con);

                AddBusParameters(cmd);

                cmd->Parameters->AddWithValue(
                    "@id", Convert::ToInt32(txtID->Text));

                int changed = cmd->ExecuteNonQuery();

                con->Close();

                MessageBox::Show(
                    changed > 0
                    ? "Bus updated successfully!"
                    : "No bus was updated.");

                LoadBuses();
                ResetFields();
            }
            catch (Exception^ ex)
            {
                MessageBox::Show(
                    "Update Bus failed:\n" +
                    ex->Message);
            }
        }

        // DELETE - REMOVE BUS
        void DeleteBus(Object^ sender, EventArgs^ e)
        {
            if (String::IsNullOrWhiteSpace(txtID->Text))
            {
                MessageBox::Show(
                    "Select a bus first.");
                return;
            }

            System::Windows::Forms::DialogResult result =
                MessageBox::Show(
                    "Are you sure you want to delete this bus?",
                    "Confirm Delete",
                    MessageBoxButtons::YesNo,
                    MessageBoxIcon::Warning);

            if (result !=
                System::Windows::Forms::DialogResult::Yes)
                return;

            try
            {
                MySqlConnection^ con =
                    Database::GetConnection();

                con->Open();

                int id =
                    Convert::ToInt32(txtID->Text);

                MySqlCommand^ check =
                    gcnew MySqlCommand(
                        "SELECT COUNT(*) FROM Bookings "
                        "WHERE BusID=@id",
                        con);

                check->Parameters->AddWithValue(
                    "@id", id);

                int count =
                    Convert::ToInt32(
                        check->ExecuteScalar());

                if (count > 0)
                {
                    con->Close();

                    MessageBox::Show(
                        "Cannot delete this bus because "
                        "it has booking records.");
                    return;
                }

                MySqlCommand^ cmd =
                    gcnew MySqlCommand(
                        "DELETE FROM Buses "
                        "WHERE BusID=@id",
                        con);

                cmd->Parameters->AddWithValue(
                    "@id", id);

                int deleted = cmd->ExecuteNonQuery();

                con->Close();

                MessageBox::Show(
                    deleted > 0
                    ? "Bus deleted successfully!"
                    : "Bus not found.");

                LoadBuses();
                ResetFields();
            }
            catch (Exception^ ex)
            {
                MessageBox::Show(
                    "Delete Bus failed:\n" +
                    ex->Message);
            }
        }

        // COMMON PARAMETERS
        void AddBusParameters(MySqlCommand^ cmd)
        {
            String^ from =
                txtFrom->Text->Trim();

            String^ to =
                txtTo->Text->Trim();

            cmd->Parameters->AddWithValue(
                "@n", txtNumber->Text->Trim());

            cmd->Parameters->AddWithValue(
                "@r", from + " - " + to);

            cmd->Parameters->AddWithValue(
                "@f", from);

            cmd->Parameters->AddWithValue(
                "@t", to);

            cmd->Parameters->AddWithValue(
                "@time", txtTime->Text->Trim());

            cmd->Parameters->AddWithValue(
                "@seats",
                Convert::ToInt32(txtSeats->Text));

            cmd->Parameters->AddWithValue(
                "@fare",
                Decimal::Parse(txtFare->Text));
        }

        // CLEAR ALL TEXTBOXES
        void ResetFields()
        {
            txtID->Clear();
            txtNumber->Clear();
            txtFrom->Clear();
            txtTo->Clear();
            txtTime->Clear();
            txtSeats->Clear();
            txtFare->Clear();

            grid->ClearSelection();
        }

        void ClearFields(Object^ sender, EventArgs^ e)
        {
            ResetFields();
        }
    };
}
