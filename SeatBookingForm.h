
#pragma once

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace SmartBusSeatBookingSystem {

    public ref class SeatBookingForm : public Form
    {
    private:
        Color navy = Color::FromArgb(39, 29, 83);
        Color teal = Color::FromArgb(0, 150, 136);

    public:
        SeatBookingForm(
            int bookingId,
            String^ passenger,
            String^ nic,
            String^ phone,
            String^ bus,
            String^ route,
            String^ departure,
            int seat,
            DateTime date,
            Decimal fare)
        {
            this->Text = "SmartBus | E-Ticket";
            this->ClientSize = Drawing::Size(610, 730);
            this->StartPosition = FormStartPosition::CenterParent;
            this->BackColor = Color::FromArgb(244, 247, 251);

            this->FormBorderStyle =
                System::Windows::Forms::FormBorderStyle::FixedDialog;

            this->MaximizeBox = false;

            // HEADER
            Panel^ top = gcnew Panel();
            top->Dock = DockStyle::Top;
            top->Height = 110;
            top->BackColor = navy;
            this->Controls->Add(top);

            Label^ brand = gcnew Label();
            brand->Text = "SMARTBUS  /  E-TICKET";
            brand->Font = gcnew Drawing::Font(
                "Segoe UI", 22, FontStyle::Bold);

            brand->ForeColor = Color::White;
            brand->Location = Point(32, 27);
            brand->Size = Drawing::Size(550, 48);

            top->Controls->Add(brand);

            // TICKET CARD
            Panel^ card = gcnew Panel();
            card->BackColor = Color::White;
            card->Location = Point(35, 130);
            card->Size = Drawing::Size(540, 490);
            this->Controls->Add(card);

            // SUCCESS MESSAGE
            Label^ success = gcnew Label();
            success->Text = "BOOKING CONFIRMED";
            success->Font = gcnew Drawing::Font(
                "Segoe UI", 19, FontStyle::Bold);

            success->ForeColor = teal;
            success->TextAlign = ContentAlignment::MiddleCenter;
            success->Location = Point(22, 12);
            success->Size = Drawing::Size(496, 48);

            card->Controls->Add(success);

            // TICKET DETAILS
            Label^ details = gcnew Label();
            details->Font = gcnew Drawing::Font("Segoe UI", 12);
            details->ForeColor = navy;
            details->Location = Point(35, 75);
            details->Size = Drawing::Size(475, 400);

            details->Text =
                "Booking ID       : " + bookingId.ToString() + "\n\n" +
                "Passenger        : " + passenger + "\n\n" +
                "NIC Number      : " + nic + "\n\n" +
                "Phone Number  : " + phone + "\n\n" +
                "Bus Number     : " + bus + "\n\n" +
                "Route                 : " + route + "\n\n" +
                "Travel Date       : " + date.ToString("dd MMM yyyy") + "\n\n" +
                "Departure         : " + departure + "\n\n" +
                "Seat Number     : " + seat.ToString("00") + "\n\n" +
                "TOTAL FARE       : Rs. " + fare.ToString("N2");

            card->Controls->Add(details);

            // INFORMATION
            Label^ hint = gcnew Label();
            hint->Text =
                "Keep your Booking ID to find or cancel this ticket later.";

            hint->Location = Point(45, 635);
            hint->Size = Drawing::Size(530, 30);
            hint->ForeColor = Color::DimGray;

            this->Controls->Add(hint);

            // FINISH BUTTON
            Button^ done = gcnew Button();
            done->Text = "FINISH / BACK TO HOME";

            done->Font = gcnew Drawing::Font(
                "Segoe UI", 12, FontStyle::Bold);

            done->Location = Point(160, 670);
            done->Size = Drawing::Size(290, 45);

            done->BackColor = teal;
            done->ForeColor = Color::White;
            done->FlatStyle = FlatStyle::Flat;
            done->FlatAppearance->BorderSize = 0;

            done->Click += gcnew EventHandler(
                this, &SeatBookingForm::Finish);

            this->Controls->Add(done);
        }

    private:
        void Finish(Object^ sender, EventArgs^ e)
        {
            this->Close();
        }
    };
}
