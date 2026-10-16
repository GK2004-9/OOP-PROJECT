
#pragma once

#include "Database.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace MySql::Data::MySqlClient;

namespace SmartBusSeatBookingSystem {

    public ref class AdminBookingForm : public Form
    {
    private:
        TextBox^ txtID;
        TextBox^ txtName;
        TextBox^ txtNIC;
        TextBox^ txtPhone;
        TextBox^ txtBusID;
        TextBox^ txtSeat;
        DateTimePicker^ dateBox;
        ComboBox^ statusBox;
        DataGridView^ grid;

        Color navy;
        Color teal;

    public:
        AdminBookingForm()
        {
            navy = Color::FromArgb(39, 29, 83);
            teal = Color::FromArgb(0, 150, 136);

            BuildUI();
            LoadBookings();
        }

    private:

        Label^ MakeLabel(String^ text, int x, int y)
        {
            Label^ l = gcnew Label();
            l->Text = text;
            l->Location = Point(x, y);
            l->Size = Drawing::Size(150, 25);
            l->Font = gcnew Drawing::Font("Segoe UI", 10);
            return l;
        }

        TextBox^ MakeTextBox(int x, int y)
        {
            TextBox^ t = gcnew TextBox();
            t->Location = Point(x, y);
            t->Size = Drawing::Size(220, 30);
            t->Font = gcnew Drawing::Font("Segoe UI", 10);
            return t;
        }

        Button^ MakeButton(
            String^ text, int x, int y,
            Color color, EventHandler^ handler)
        {
            Button^ b = gcnew Button();
            b->Text = text;
            b->Location = Point(x, y);
            b->Size = Drawing::Size(160, 43);
            b->BackColor = color;
            b->ForeColor = Color::White;
            b->FlatStyle = FlatStyle::Flat;
            b->FlatAppearance->BorderSize = 0;
            b->Font = gcnew Drawing::Font(
                "Segoe UI", 10, FontStyle::Bold);
            b->Click += handler;
            return b;
        }

        void BuildUI()
        {
            this->Text = "SmartBus - Booking Management";
            this->ClientSize = Drawing::Size(1160, 780);
            this->StartPosition = FormStartPosition::CenterScreen;
            this->BackColor = Color::WhiteSmoke;

            Panel^ header = gcnew Panel();
            header->Dock = DockStyle::Top;
            header->Height = 85;
            header->BackColor = navy;
            this->Controls->Add(header);

            Label^ title = gcnew Label();
            title->Text = "ADMIN - BOOKING MANAGEMENT";
            title->Location = Point(25, 20);
            title->Size = Drawing::Size(820, 45);
            title->ForeColor = Color::White;
            title->Font = gcnew Drawing::Font(
                "Segoe UI", 21, FontStyle::Bold);
            header->Controls->Add(title);

            Button^ back = MakeButton(
                "< BACK", 965, 20, teal,
                gcnew EventHandler(this, &AdminBookingForm::GoBack));
            header->Controls->Add(back);

            this->Controls->Add(MakeLabel("Booking ID", 30, 115));
            txtID = MakeTextBox(180, 110);
            txtID->ReadOnly = true;
            this->Controls->Add(txtID);

            this->Controls->Add(MakeLabel("Passenger Name", 30, 160));
            txtName = MakeTextBox(180, 155);
            this->Controls->Add(txtName);

            txtNIC = MakeTextBox(180, 200);
            txtNIC->MaxLength = 12;
            txtNIC->CharacterCasing = CharacterCasing::Upper;
            this->Controls->Add(txtNIC);
           

            txtPhone = MakeTextBox(180, 245);
            txtPhone->MaxLength = 10;
            this->Controls->Add(txtPhone);

            this->Controls->Add(MakeLabel("Bus ID", 510, 115));
            txtBusID = MakeTextBox(670, 110);
            this->Controls->Add(txtBusID);

            this->Controls->Add(MakeLabel("Seat Number", 510, 160));
            txtSeat = MakeTextBox(670, 155);
            this->Controls->Add(txtSeat);

            this->Controls->Add(MakeLabel("Booking Date", 510, 205));
            dateBox = gcnew DateTimePicker();
            dateBox->Location = Point(670, 200);
            dateBox->Size = Drawing::Size(220, 30);
            dateBox->Format = DateTimePickerFormat::Short;
            this->Controls->Add(dateBox);

            this->Controls->Add(MakeLabel("Booking Status", 510, 250));
            statusBox = gcnew ComboBox();
            statusBox->Location = Point(670, 245);
            statusBox->Size = Drawing::Size(220, 30);
            statusBox->DropDownStyle = ComboBoxStyle::DropDownList;
            statusBox->Items->Add("Confirmed");
            statusBox->Items->Add("Cancelled");
            statusBox->SelectedIndex = 0;
            this->Controls->Add(statusBox);

            this->Controls->Add(MakeButton(
                "ADD BOOKING", 30, 315, teal,
                gcnew EventHandler(this, &AdminBookingForm::AddBooking)));

            this->Controls->Add(MakeButton(
                "VIEW BOOKINGS", 210, 315, navy,
                gcnew EventHandler(this, &AdminBookingForm::RefreshBookings)));

            this->Controls->Add(MakeButton(
                "UPDATE BOOKING", 390, 315, Color::DarkOrange,
                gcnew EventHandler(this, &AdminBookingForm::UpdateBooking)));

            this->Controls->Add(MakeButton(
                "DELETE BOOKING", 570, 315, Color::Firebrick,
                gcnew EventHandler(this, &AdminBookingForm::DeleteBooking)));

            this->Controls->Add(MakeButton(
                "CLEAR", 750, 315, Color::SlateGray,
                gcnew EventHandler(this, &AdminBookingForm::ClearFields)));

            grid = gcnew DataGridView();
            grid->Location = Point(25, 390);
            grid->Size = Drawing::Size(1110, 355);
            grid->BackgroundColor = Color::White;
            grid->AutoSizeColumnsMode =
                DataGridViewAutoSizeColumnsMode::Fill;
            grid->SelectionMode =
                DataGridViewSelectionMode::FullRowSelect;
            grid->MultiSelect = false;
            grid->ReadOnly = true;
            grid->AllowUserToAddRows = false;
            grid->RowHeadersVisible = false;

            grid->CellClick += gcnew DataGridViewCellEventHandler(
                this, &AdminBookingForm::SelectRow);

            this->Controls->Add(grid);
        }

        void GoBack(Object^ sender, EventArgs^ e)
        {
            this->Close();
        }

        void LoadBookings()
        {
            try
            {
                MySqlConnection^ con = Database::GetConnection();
                con->Open();

                MySqlDataAdapter^ adapter = gcnew MySqlDataAdapter(
                    "SELECT BookingID, PassengerName, NIC, PhoneNumber, "
                    "BusID, SeatNumber, BookingDate, BookingStatus, TicketFare "
                    "FROM Bookings ORDER BY BookingID DESC", con);

                DataTable^ table = gcnew DataTable();
                adapter->Fill(table);
                grid->DataSource = table;

                con->Close();
            }
            catch (Exception^ ex)
            {
                MessageBox::Show("Load failed:\n" + ex->Message);
            }
        }

        void RefreshBookings(Object^ sender, EventArgs^ e)
        {
            LoadBookings();
        }

        void SelectRow(Object^ sender, DataGridViewCellEventArgs^ e)
        {
            if (e->RowIndex < 0)
                return;

            DataGridViewRow^ row = grid->Rows[e->RowIndex];

            txtID->Text = Convert::ToString(
                row->Cells["BookingID"]->Value);
            txtName->Text = Convert::ToString(
                row->Cells["PassengerName"]->Value);
            txtNIC->Text = Convert::ToString(
                row->Cells["NIC"]->Value);
            txtPhone->Text = Convert::ToString(
                row->Cells["PhoneNumber"]->Value);
            txtBusID->Text = Convert::ToString(
                row->Cells["BusID"]->Value);

            Object^ seatValue = row->Cells["SeatNumber"]->Value;
            txtSeat->Text =
                (seatValue == nullptr || seatValue == DBNull::Value)
                ? "" : Convert::ToString(seatValue);

            dateBox->Value = Convert::ToDateTime(
                row->Cells["BookingDate"]->Value);

            String^ status = Convert::ToString(
                row->Cells["BookingStatus"]->Value);

            statusBox->SelectedItem = status;
        }

        bool ValidateFields()
        {
            int busID, seat;

            if (String::IsNullOrWhiteSpace(txtName->Text) ||
                !Int32::TryParse(txtBusID->Text, busID) ||
                busID <= 0 ||
                !Int32::TryParse(txtSeat->Text, seat) ||
                seat <= 0)
            {
                MessageBox::Show(
                    "Enter a valid passenger name, Bus ID and Seat Number.");
                return false;
            }

            if (!System::Text::RegularExpressions::Regex::IsMatch(
                txtNIC->Text->Trim(),
                "^(?:[0-9]{9}[VvXx]|[0-9]{12})$"))
            {
                MessageBox::Show("Enter a valid NIC.");
                return false;
            }

            if (!System::Text::RegularExpressions::Regex::IsMatch(
                txtPhone->Text->Trim(), "^0[0-9]{9}$"))
            {
                MessageBox::Show("Enter a valid phone number.");
                return false;
            }

            return true;
        }

        // Check bus capacity and seat availability
        bool CheckSeat(MySqlConnection^ con, int excludeBookingID)
        {
            int busID = Convert::ToInt32(txtBusID->Text);
            int seat = Convert::ToInt32(txtSeat->Text);

            MySqlCommand^ capacity = gcnew MySqlCommand(
                "SELECT TotalSeats FROM Buses WHERE BusID=@bus", con);
            capacity->Parameters->AddWithValue("@bus", busID);

            Object^ result = capacity->ExecuteScalar();

            if (result == nullptr || result == DBNull::Value)
            {
                MessageBox::Show("Bus ID does not exist.");
                return false;
            }

            if (seat > Convert::ToInt32(result))
            {
                MessageBox::Show("Seat number exceeds bus capacity.");
                return false;
            }

            MySqlCommand^ check = gcnew MySqlCommand(
                "SELECT COUNT(*) FROM Bookings "
                "WHERE BusID=@bus AND SeatNumber=@seat "
                "AND BookingDate=@date "
                "AND BookingStatus='Confirmed' "
                "AND BookingID<>@exclude", con);

            check->Parameters->AddWithValue("@bus", busID);
            check->Parameters->AddWithValue("@seat", seat);
            check->Parameters->AddWithValue("@date", dateBox->Value.Date);
            check->Parameters->AddWithValue("@exclude", excludeBookingID);

            int count = Convert::ToInt32(check->ExecuteScalar());

            if (count > 0)
            {
                MessageBox::Show(
                    "This seat is already booked for the selected date.");
                return false;
            }

            return true;
        }

        // CREATE
        void AddBooking(Object^ sender, EventArgs^ e)
        {
            if (!ValidateFields())
                return;

            if (Convert::ToString(statusBox->SelectedItem) != "Confirmed")
            {
                MessageBox::Show("New bookings must be Confirmed.");
                return;
            }

            try
            {
                MySqlConnection^ con = Database::GetConnection();
                con->Open();

                if (!CheckSeat(con, 0))
                {
                    con->Close();
                    return;
                }

                MySqlCommand^ fareCmd = gcnew MySqlCommand(
                    "SELECT Fare FROM Buses WHERE BusID=@id", con);
                fareCmd->Parameters->AddWithValue(
                    "@id", Convert::ToInt32(txtBusID->Text));

                Decimal fare = Convert::ToDecimal(
                    fareCmd->ExecuteScalar());

                MySqlCommand^ cmd = gcnew MySqlCommand(
                    "INSERT INTO Bookings "
                    "(PassengerName, NIC, PhoneNumber, BusID, "
                    "SeatNumber, OriginalSeatNumber, BookingDate, "
                    "BookingStatus, TicketFare) "
                    "VALUES (@name, @nic, @phone, @bus, @seat, @seat, "
                    "@date, 'Confirmed', @fare)", con);

                cmd->Parameters->AddWithValue("@name", txtName->Text->Trim());
                cmd->Parameters->AddWithValue("@nic", txtNIC->Text->Trim());
                cmd->Parameters->AddWithValue("@phone", txtPhone->Text->Trim());
                cmd->Parameters->AddWithValue("@bus",
                    Convert::ToInt32(txtBusID->Text));
                cmd->Parameters->AddWithValue("@seat",
                    Convert::ToInt32(txtSeat->Text));
                cmd->Parameters->AddWithValue("@date", dateBox->Value.Date);
                cmd->Parameters->AddWithValue("@fare", fare);

                cmd->ExecuteNonQuery();
                con->Close();

                MessageBox::Show("Booking added successfully!");
                LoadBookings();
                ResetFields();
            }
            catch (Exception^ ex)
            {
                MessageBox::Show("Add failed:\n" + ex->Message);
            }
        }

        // UPDATE
        void UpdateBooking(Object^ sender, EventArgs^ e)
        {
            if (String::IsNullOrWhiteSpace(txtID->Text))
            {
                MessageBox::Show("Select a booking first.");
                return;
            }

            if (!ValidateFields())
                return;

            if (Convert::ToString(statusBox->SelectedItem) != "Confirmed")
            {
                MessageBox::Show(
                    "Use the Cancel Booking screen to cancel a booking.");
                return;
            }

            try
            {
                MySqlConnection^ con = Database::GetConnection();
                con->Open();

                int id = Convert::ToInt32(txtID->Text);

                MySqlCommand^ current = gcnew MySqlCommand(
                    "SELECT BookingStatus FROM Bookings "
                    "WHERE BookingID=@id", con);
                current->Parameters->AddWithValue("@id", id);

                Object^ currentStatus = current->ExecuteScalar();

                if (currentStatus == nullptr ||
                    Convert::ToString(currentStatus) != "Confirmed")
                {
                    con->Close();
                    MessageBox::Show(
                        "Only confirmed bookings can be updated.");
                    return;
                }

                if (!CheckSeat(con, id))
                {
                    con->Close();
                    return;
                }

                MySqlCommand^ fareCmd = gcnew MySqlCommand(
                    "SELECT Fare FROM Buses WHERE BusID=@bus", con);
                fareCmd->Parameters->AddWithValue(
                    "@bus", Convert::ToInt32(txtBusID->Text));

                Decimal fare = Convert::ToDecimal(
                    fareCmd->ExecuteScalar());

                MySqlCommand^ cmd = gcnew MySqlCommand(
                    "UPDATE Bookings SET "
                    "PassengerName=@name, NIC=@nic, PhoneNumber=@phone, "
                    "BusID=@bus, SeatNumber=@seat, "
                    "OriginalSeatNumber=@seat, BookingDate=@date, "
                    "TicketFare=@fare "
                    "WHERE BookingID=@id AND BookingStatus='Confirmed'",
                    con);

                cmd->Parameters->AddWithValue("@name", txtName->Text->Trim());
                cmd->Parameters->AddWithValue("@nic", txtNIC->Text->Trim());
                cmd->Parameters->AddWithValue("@phone", txtPhone->Text->Trim());
                cmd->Parameters->AddWithValue("@bus",
                    Convert::ToInt32(txtBusID->Text));
                cmd->Parameters->AddWithValue("@seat",
                    Convert::ToInt32(txtSeat->Text));
                cmd->Parameters->AddWithValue("@date", dateBox->Value.Date);
                cmd->Parameters->AddWithValue("@fare", fare);
                cmd->Parameters->AddWithValue("@id", id);

                int changed = cmd->ExecuteNonQuery();
                con->Close();

                MessageBox::Show(changed > 0
                    ? "Booking updated successfully!"
                    : "No booking was updated.");

                LoadBookings();
                ResetFields();
            }
            catch (Exception^ ex)
            {
                MessageBox::Show("Update failed:\n" + ex->Message);
            }
        }

        // DELETE
        void DeleteBooking(Object^ sender, EventArgs^ e)
        {
            if (String::IsNullOrWhiteSpace(txtID->Text))
            {
                MessageBox::Show("Select a booking first.");
                return;
            }

            System::Windows::Forms::DialogResult answer =
                MessageBox::Show(
                    "Permanently delete this booking record?",
                    "Confirm Delete",
                    MessageBoxButtons::YesNo,
                    MessageBoxIcon::Warning);

            if (answer !=
                System::Windows::Forms::DialogResult::Yes)
                return;

            try
            {
                MySqlConnection^ con = Database::GetConnection();
                con->Open();

                MySqlCommand^ cmd = gcnew MySqlCommand(
                    "DELETE FROM Bookings WHERE BookingID=@id", con);

                cmd->Parameters->AddWithValue(
                    "@id", Convert::ToInt32(txtID->Text));

                int deleted = cmd->ExecuteNonQuery();
                con->Close();

                MessageBox::Show(deleted > 0
                    ? "Booking deleted successfully!"
                    : "Booking not found.");

                LoadBookings();
                ResetFields();
            }
            catch (Exception^ ex)
            {
                MessageBox::Show("Delete failed:\n" + ex->Message);
            }
        }

        void ResetFields()
        {
            txtID->Clear();
            txtName->Clear();
            txtNIC->Clear();
            txtPhone->Clear();
            txtBusID->Clear();
            txtSeat->Clear();

            dateBox->Value = DateTime::Today;
            statusBox->SelectedIndex = 0;
            grid->ClearSelection();
        }

        void ClearFields(Object^ sender, EventArgs^ e)
        {
            ResetFields();
        }
    };
}
