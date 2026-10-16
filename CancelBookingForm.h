#pragma once
#include "Database.h"
using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace MySql::Data::MySqlClient;
namespace SmartBusSeatBookingSystem {
    public ref class CancelBookingForm : public Form {
    private:
        TextBox^ txtId;
        Label^ details;
        Button^ cancelButton;
        int currentId;
        Color navy = Color::FromArgb(39, 29, 83);
    public:
        CancelBookingForm() {
            currentId = -1;
            Text = "SmartBus | Find / Cancel Ticket";
            ClientSize = Drawing::Size(580, 630);
            StartPosition = FormStartPosition::CenterParent;
            BackColor = Color::FromArgb(244, 247, 251);
            FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            MaximizeBox = false;
            Panel^ top = gcnew Panel(); top->Dock = DockStyle::Top;
            top->Height = 105; top->BackColor = navy; Controls->Add(top);
            Label^ heading = gcnew Label(); heading->Text = "FIND / CANCEL TICKET";
            heading->Font = gcnew Drawing::Font("Segoe UI", 21, FontStyle::Bold);
            heading->ForeColor = Color::White; heading->Location = Point(28, 30);
            heading->Size = Drawing::Size(525, 45); top->Controls->Add(heading);
            Label^ prompt = gcnew Label(); prompt->Text = "Booking ID";
            prompt->Font = gcnew Drawing::Font("Segoe UI", 12);
            prompt->Location = Point(35, 133); prompt->AutoSize = true; Controls->Add(prompt);
            txtId = gcnew TextBox(); txtId->Location = Point(35, 168);
            txtId->Size = Drawing::Size(250, 35);
            txtId->Font = gcnew Drawing::Font("Segoe UI", 13); Controls->Add(txtId);
            Button^ find = gcnew Button(); find->Text = "FIND BOOKING";
            find->Location = Point(305, 166); find->Size = Drawing::Size(230, 42);
            find->BackColor = Color::FromArgb(0, 150, 136); find->ForeColor = Color::White;
            find->FlatStyle = FlatStyle::Flat; find->FlatAppearance->BorderSize = 0;
            find->Click += gcnew EventHandler(this, &CancelBookingForm::FindBooking);
            Controls->Add(find);
            Panel^ white = gcnew Panel(); white->BackColor = Color::White;
            white->Location = Point(30, 238); white->Size = Drawing::Size(520, 240);
            Controls->Add(white);
            details = gcnew Label(); details->Text = "Enter a Booking ID to see ticket details.";
            details->Font = gcnew Drawing::Font("Segoe UI", 11);
            details->Location = Point(22, 18); details->Size = Drawing::Size(475, 210);
            white->Controls->Add(details);
            cancelButton = gcnew Button(); cancelButton->Text = "CANCEL THIS TICKET";
            cancelButton->Location = Point(165, 502);
            cancelButton->Size = Drawing::Size(250, 48);
            cancelButton->BackColor = Color::IndianRed;
            cancelButton->ForeColor = Color::White;
            cancelButton->FlatStyle = FlatStyle::Flat;
            cancelButton->FlatAppearance->BorderSize = 0;
            cancelButton->Enabled = false;
            cancelButton->Click += gcnew EventHandler(this, &CancelBookingForm::CancelTicket);
            Controls->Add(cancelButton);
            // BACK BUTTON
            Button^ backButton = gcnew Button();

            backButton->Text = "< BACK TO HOME";
            backButton->Location = Point(165, 555);
            backButton->Size = Drawing::Size(250, 42);

            backButton->BackColor = navy;
            backButton->ForeColor = Color::White;

            backButton->Font = gcnew Drawing::Font(
                "Segoe UI", 11, FontStyle::Bold);

            backButton->FlatStyle = FlatStyle::Flat;
            backButton->FlatAppearance->BorderSize = 0;

            backButton->Click += gcnew EventHandler(
                this, &CancelBookingForm::GoBack);

            Controls->Add(backButton);
        }
    private:
        void GoBack(Object^ sender, EventArgs^ e)
        {
            this->Close();
        }
        void FindBooking(Object^ sender, EventArgs^ e) {
            currentId = -1; cancelButton->Enabled = false;
            details->Text = "";
            int id;
            if (!Int32::TryParse(txtId->Text, id) || id <= 0) {
                MessageBox::Show("Enter a valid numeric Booking ID."); return;
            }
            try {
                MySqlConnection^ con = Database::GetConnection(); con->Open();
                MySqlCommand^ cmd = gcnew MySqlCommand(
                    "SELECT k.BookingID, k.PassengerName, k.BookingDate, "
                    "k.OriginalSeatNumber, k.BookingStatus, k.TicketFare, "
                    "b.BusNumber, b.Route, b.DepartureTime "
                    "FROM Bookings k INNER JOIN Buses b ON k.BusID=b.BusID "
                    "WHERE k.BookingID=@id", con);
                cmd->Parameters->AddWithValue("@id", id);
                MySqlDataReader^ r = cmd->ExecuteReader();
                if (r->Read()) {
                    String^ status = Convert::ToString(r["BookingStatus"]);
                    String^ seat = r->IsDBNull(r->GetOrdinal("OriginalSeatNumber"))
                        ? "Unknown" : Convert::ToString(r["OriginalSeatNumber"]);
                    String^ price = r->IsDBNull(r->GetOrdinal("TicketFare"))
                        ? "Not recorded" : "Rs. " + Convert::ToDecimal(r["TicketFare"]).ToString("N2");
                    details->Text =
                        "Booking ID: " + id.ToString() + "\n" +
                        "Passenger: " + Convert::ToString(r["PassengerName"]) + "\n" +
                        "Bus: " + Convert::ToString(r["BusNumber"]) + "\n" +
                        "Route: " + Convert::ToString(r["Route"]) + "\n" +
                        "Departure: " + Convert::ToString(r["DepartureTime"]) + "\n" +
                        "Date: " + Convert::ToDateTime(r["BookingDate"]).ToString("dd MMM yyyy") + "\n" +
                        "Seat: " + seat + "\nFare: " + price + "\n" +
                        "Status: " + status;
                    if (status == "Confirmed") {
                        currentId = id; cancelButton->Enabled = true;
                    }
                }
                else details->Text = "Booking not found.";
                r->Close(); con->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Lookup failed:\n" + ex->Message);
            }
        }
        void CancelTicket(Object^ sender, EventArgs^ e) {
            if (currentId <= 0) return;
            if (MessageBox::Show("Cancel this booking? This demo does not process refunds.",
                "Confirm Cancellation", MessageBoxButtons::YesNo,
                MessageBoxIcon::Question) != System::Windows::Forms::DialogResult::Yes) return;
            try {
                MySqlConnection^ con = Database::GetConnection(); con->Open();
                // OriginalSeatNumber and TicketFare are retained as history.
                // NULL in SeatNumber frees the unique (BusID, SeatNumber, BookingDate) slot.
                MySqlCommand^ cmd = gcnew MySqlCommand(
                    "UPDATE Bookings SET BookingStatus='Cancelled', SeatNumber=NULL "
                    "WHERE BookingID=@id AND BookingStatus='Confirmed'", con);
                cmd->Parameters->AddWithValue("@id", currentId);
                int changed = cmd->ExecuteNonQuery(); con->Close();
                if (changed == 1) {
                    details->Text = "BOOKING CANCELLED\n\nThe seat is available for another booking.\n"
                        "Original seat and fare remain in booking history.";
                    MessageBox::Show("Ticket cancelled successfully.");
                }
                else {
                    details->Text = "Ticket was already cancelled or not found.";
                }
                currentId = -1; cancelButton->Enabled = false;
            }
            catch (Exception^ ex) {
                MessageBox::Show("Cancellation failed:\n" + ex->Message);
            }
        }
    };
}
