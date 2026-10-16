
#pragma once

#include "Database.h"
#include "SeatBookingForm.h"
#include "Models.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace MySql::Data::MySqlClient;

namespace SmartBusSeatBookingSystem {

    public ref class BusDetailsForm : public Form
    {
    private:
        String^ fromCity;
        String^ toCity;
        DateTime travelDate;

        int busID;
        int totalSeats;
        int selectedSeat;

        String^ busNumber;
        String^ departure;
        Decimal fare;

        FlowLayoutPanel^ busList;
        FlowLayoutPanel^ seatMap;

        Label^ busInfo;
        Label^ selectionInfo;

        TextBox^ passengerBox;
        TextBox^ nicBox;
        TextBox^ phoneBox;

        Button^ confirmButton;

        Dictionary<int, Button^>^ seatButtons;

        Color navy;
        Color teal;
        Color amber;

    public:
        BusDetailsForm(String^ from, String^ to, DateTime date)
        {
            fromCity = from;
            toCity = to;
            travelDate = date;

            busID = -1;
            totalSeats = 0;
            selectedSeat = -1;

            navy = Color::FromArgb(39, 29, 83);
            teal = Color::FromArgb(0, 150, 136);
            amber = Color::FromArgb(245, 158, 11);

            seatButtons = gcnew Dictionary<int, Button^>();

            BuildUI();
            LoadBuses();
        }

    private:

        // CREATE LABEL
        Label^ L(String^ text, int x, int y,
            int w, int h, float size, bool bold, Color color)
        {
            Label^ label = gcnew Label();

            label->Text = text;
            label->Location = Point(x, y);
            label->Size = Drawing::Size(w, h);
            label->ForeColor = color;

            label->Font = gcnew Drawing::Font(
                "Segoe UI",
                size,
                bold ? FontStyle::Bold : FontStyle::Regular
            );

            return label;
        }

        // PAGE DESIGN
        void BuildUI()
        {
            this->Text = "SmartBus | Buses and Seats";
            this->ClientSize = Drawing::Size(1120, 850);
            this->StartPosition = FormStartPosition::CenterParent;
            this->BackColor = Color::FromArgb(244, 247, 251);
            this->MinimumSize = Drawing::Size(1100, 750);

            // HEADER
            Panel^ header = gcnew Panel();
            header->Dock = DockStyle::Top;
            header->Height = 115;
            header->BackColor = navy;
            this->Controls->Add(header);

            header->Controls->Add(
                L("AVAILABLE BUSES", 30, 12, 700, 48,
                    23, true, Color::White)
            );

            header->Controls->Add(
                L(fromCity + "   >>   " + toCity + "      " +
                    travelDate.ToString("dd MMM yyyy"),
                    32, 67, 850, 32, 13, false, Color::White)
            );

            // BACK BUTTON
            Button^ backButton = gcnew Button();

            backButton->Text = "< BACK";
            backButton->Location = Point(930, 30);
            backButton->Size = Drawing::Size(150, 45);
            backButton->BackColor = teal;
            backButton->ForeColor = Color::White;

            backButton->Font = gcnew Drawing::Font(
                "Segoe UI", 12, FontStyle::Bold);

            backButton->FlatStyle = FlatStyle::Flat;
            backButton->FlatAppearance->BorderSize = 0;

            backButton->Click += gcnew EventHandler(
                this, &BusDetailsForm::GoBack);

            header->Controls->Add(backButton);

            // LEFT SIDE - BUS LIST
            busList = gcnew FlowLayoutPanel();

            busList->Location = Point(20, 135);
            busList->Size = Drawing::Size(515, 670);
            busList->FlowDirection = FlowDirection::TopDown;
            busList->WrapContents = false;
            busList->AutoScroll = true;

            this->Controls->Add(busList);

            // RIGHT SIDE PANEL
            Panel^ right = gcnew Panel();

            right->BackColor = Color::White;
            right->Location = Point(555, 135);
            right->Size = Drawing::Size(540, 670);
            right->AutoScroll = true;

            this->Controls->Add(right);

            right->Controls->Add(
                L("CHOOSE YOUR SEAT", 22, 12, 490, 43,
                    20, true, navy)
            );

            busInfo = L(
                "Choose a bus on the left",
                22, 65, 495, 48, 12, false, navy);

            right->Controls->Add(busInfo);

            right->Controls->Add(
                L("Grey: Available   Amber: Booked   Teal: Selected",
                    22, 114, 495, 30, 10, false, Color::DimGray)
            );

            // SEAT MAP
            seatMap = gcnew FlowLayoutPanel();

            seatMap->Location = Point(30, 148);
            seatMap->Size = Drawing::Size(475, 225);
            seatMap->AutoScroll = true;
            seatMap->FlowDirection = FlowDirection::TopDown;
            seatMap->WrapContents = false;

            right->Controls->Add(seatMap);

            selectionInfo = L(
                "Selected Seat: None",
                25, 385, 475, 28, 12, true, teal);

            right->Controls->Add(selectionInfo);

            // PASSENGER DETAILS
            right->Controls->Add(
                L("PASSENGER DETAILS", 25, 420, 450, 30,
                    14, true, navy)
            );

            // PASSENGER NAME
            right->Controls->Add(
                L("Full Name", 25, 455, 180, 24,
                    10, false, navy)
            );

            passengerBox = gcnew TextBox();
            passengerBox->Location = Point(25, 480);
            passengerBox->Size = Drawing::Size(470, 30);
            passengerBox->Font = gcnew Drawing::Font(
                "Segoe UI", 11);

            right->Controls->Add(passengerBox);

            // NIC NUMBER
            right->Controls->Add(
                L("NIC Number", 25, 520, 180, 24,
                    10, false, navy)
            );

            nicBox = gcnew TextBox();
            nicBox->Location = Point(25, 545);
            nicBox->Size = Drawing::Size(470, 30);
            nicBox->Font = gcnew Drawing::Font(
                "Segoe UI", 11);
            nicBox->MaxLength = 12;

            right->Controls->Add(nicBox);

            // PHONE NUMBER
            right->Controls->Add(
                L("Phone Number", 25, 585, 180, 24,
                    10, false, navy)
            );

            phoneBox = gcnew TextBox();
            phoneBox->Location = Point(25, 610);
            phoneBox->Size = Drawing::Size(470, 30);
            phoneBox->Font = gcnew Drawing::Font(
                "Segoe UI", 11);
            phoneBox->MaxLength = 10;

            right->Controls->Add(phoneBox);

            // BOOK NOW BUTTON
            confirmButton = gcnew Button();

            confirmButton->Text = "BOOK NOW";
            confirmButton->Location = Point(145, 660);
            confirmButton->Size = Drawing::Size(250, 48);

            confirmButton->BackColor = teal;
            confirmButton->ForeColor = Color::White;

            confirmButton->Font = gcnew Drawing::Font(
                "Segoe UI", 12, FontStyle::Bold);

            confirmButton->FlatStyle = FlatStyle::Flat;
            confirmButton->FlatAppearance->BorderSize = 0;
            confirmButton->Enabled = false;

            confirmButton->Click += gcnew EventHandler(
                this, &BusDetailsForm::BookSeat);

            right->Controls->Add(confirmButton);

            right->Controls->Add(
                L("Demo fares only; no online payment or refunds.",
                    25, 725, 490, 25, 10, false, Color::DimGray)
            );
        }

        // BACK TO HOME
        void GoBack(Object^ sender, EventArgs^ e)
        {
            this->Close();
        }

        // LOAD AVAILABLE BUSES
        void LoadBuses()
        {
            busList->Controls->Clear();

            try
            {
                MySqlConnection^ con = Database::GetConnection();
                con->Open();

                MySqlCommand^ cmd = gcnew MySqlCommand(
                    "SELECT MIN(BusID) AS BusID, "
                    "BusNumber, DepartureTime, TotalSeats, Fare "
                    "FROM Buses "
                    "WHERE FromCity=@f AND ToCity=@t "
                    "GROUP BY BusNumber, DepartureTime, TotalSeats, Fare "
                    "ORDER BY DepartureTime", con);

                cmd->Parameters->AddWithValue("@f", fromCity);
                cmd->Parameters->AddWithValue("@t", toCity);

                MySqlDataReader^ r = cmd->ExecuteReader();

                int found = 0;

                while (r->Read())
                {
                    int id = Convert::ToInt32(r["BusID"]);
                    String^ number = Convert::ToString(r["BusNumber"]);
                    String^ time = Convert::ToString(r["DepartureTime"]);
                    int capacity = Convert::ToInt32(r["TotalSeats"]);
                    Decimal price = Convert::ToDecimal(r["Fare"]);

                    Panel^ card = gcnew Panel();
                    card->Size = Drawing::Size(470, 172);
                    card->Margin =
                        System::Windows::Forms::Padding(4, 5, 4, 12);

                    card->BackColor = Color::White;
                    card->BorderStyle = BorderStyle::FixedSingle;

                    Panel^ band = gcnew Panel();
                    band->Dock = DockStyle::Top;
                    band->Height = 43;
                    band->BackColor = navy;

                    card->Controls->Add(band);

                    band->Controls->Add(
                        L("BUS " + number, 14, 7, 420, 30,
                            14, true, Color::White)
                    );

                    card->Controls->Add(
                        L(fromCity + " >> " + toCity,
                            15, 51, 430, 31, 12, true, navy)
                    );

                    card->Controls->Add(
                        L("Departure: " + time,
                            15, 87, 230, 26, 11, false, navy)
                    );

                    card->Controls->Add(
                        L("Rs. " + price.ToString("N2"),
                            280, 86, 165, 28, 13, true, teal)
                    );

                    card->Controls->Add(
                        L("Total seats: " + capacity.ToString(),
                            15, 129, 210, 25, 10, false, navy)
                    );

                    Button^ select = gcnew Button();

                    select->Text = "BOOK SEAT";
                    select->Location = Point(284, 125);
                    select->Size = Drawing::Size(160, 36);

                    select->BackColor = amber;
                    select->ForeColor = Color::White;
                    select->FlatStyle = FlatStyle::Flat;
                    select->FlatAppearance->BorderSize = 0;

                    select->Tag = id;

                    select->Click += gcnew EventHandler(
                        this, &BusDetailsForm::SelectBus);

                    card->Controls->Add(select);
                    busList->Controls->Add(card);

                    found++;
                }

                r->Close();
                con->Close();

                if (found == 0)
                {
                    busList->Controls->Add(
                        L("No buses found for this route.",
                            10, 20, 460, 50, 13, true, navy)
                    );
                }
            }
            catch (Exception^ ex)
            {
                MessageBox::Show(
                    "Bus search failed:\n" + ex->Message);
            }
        }

        // SELECT BUS
        void SelectBus(Object^ sender, EventArgs^ e)
        {
            Button^ button = safe_cast<Button^>(sender);

            busID = Convert::ToInt32(button->Tag);

            LoadSeatMap();
        }

        // LOAD SEAT MAP
        void LoadSeatMap()
        {
            selectedSeat = -1;
            confirmButton->Enabled = false;
            selectionInfo->Text = "Selected Seat: None";

            seatMap->Controls->Clear();
            seatButtons->Clear();

            try
            {
                MySqlConnection^ con = Database::GetConnection();
                con->Open();

                MySqlCommand^ cmd = gcnew MySqlCommand(
                    "SELECT BusNumber, DepartureTime, TotalSeats, Fare "
                    "FROM Buses WHERE BusID=@id", con);

                cmd->Parameters->AddWithValue("@id", busID);

                MySqlDataReader^ r = cmd->ExecuteReader();

                if (!r->Read())
                {
                    r->Close();
                    con->Close();
                    return;
                }

                busNumber = Convert::ToString(r["BusNumber"]);
                departure = Convert::ToString(r["DepartureTime"]);
                totalSeats = Convert::ToInt32(r["TotalSeats"]);
                fare = Convert::ToDecimal(r["Fare"]);

                r->Close();

                // GET BOOKED SEATS
                List<int>^ occupied = gcnew List<int>();

                MySqlCommand^ booked = gcnew MySqlCommand(
                    "SELECT SeatNumber FROM Bookings "
                    "WHERE BusID=@id "
                    "AND BookingDate=@date "
                    "AND BookingStatus='Confirmed' "
                    "AND SeatNumber IS NOT NULL", con);

                booked->Parameters->AddWithValue("@id", busID);
                booked->Parameters->AddWithValue(
                    "@date", travelDate.Date);

                MySqlDataReader^ br = booked->ExecuteReader();

                while (br->Read())
                {
                    occupied->Add(
                        Convert::ToInt32(br["SeatNumber"]));
                }

                br->Close();
                con->Close();

                busInfo->Text =
                    busNumber + " | " + departure +
                    " | Rs. " + fare.ToString("N2") +
                    "\nAvailable seats: " +
                    (totalSeats - occupied->Count).ToString();

                // CREATE SEAT BUTTONS
                for (int start = 1; start <= totalSeats; start += 4)
                {
                    FlowLayoutPanel^ row = gcnew FlowLayoutPanel();

                    row->Size = Drawing::Size(425, 54);
                    row->WrapContents = false;
                    row->Margin =
                        System::Windows::Forms::Padding(0, 2, 0, 2);

                    for (int col = 0; col < 4; col++)
                    {
                        // AISLE
                        if (col == 2)
                        {
                            Panel^ aisle = gcnew Panel();

                            aisle->Size = Drawing::Size(38, 42);
                            aisle->Margin =
                                System::Windows::Forms::Padding(0, 5, 0, 5);

                            row->Controls->Add(aisle);
                        }

                        int n = start + col;

                        if (n > totalSeats)
                            break;

                        Button^ seat = gcnew Button();

                        seat->Text = n.ToString("00");
                        seat->Tag = n;
                        seat->Size = Drawing::Size(73, 42);
                        seat->Margin =
                            System::Windows::Forms::Padding(5);

                        seat->Font = gcnew Drawing::Font(
                            "Segoe UI", 11, FontStyle::Bold);

                        seat->FlatStyle = FlatStyle::Flat;
                        seat->FlatAppearance->BorderSize = 0;

                        if (occupied->Contains(n))
                        {
                            seat->BackColor = amber;
                            seat->ForeColor = Color::White;
                            seat->Enabled = false;
                        }
                        else
                        {
                            seat->BackColor =
                                Color::FromArgb(230, 233, 239);

                            seat->ForeColor = navy;

                            seat->Click += gcnew EventHandler(
                                this, &BusDetailsForm::SelectSeat);
                        }

                        seatButtons->Add(n, seat);
                        row->Controls->Add(seat);
                    }

                    seatMap->Controls->Add(row);
                }
            }
            catch (Exception^ ex)
            {
                MessageBox::Show(
                    "Could not load seat map:\n" + ex->Message);
            }
        }

        // SELECT SEAT
        void SelectSeat(Object^ sender, EventArgs^ e)
        {
            if (selectedSeat > 0 &&
                seatButtons->ContainsKey(selectedSeat))
            {
                seatButtons[selectedSeat]->BackColor =
                    Color::FromArgb(230, 233, 239);

                seatButtons[selectedSeat]->ForeColor = navy;
            }

            Button^ clicked = safe_cast<Button^>(sender);

            selectedSeat = Convert::ToInt32(clicked->Tag);

            clicked->BackColor = teal;
            clicked->ForeColor = Color::White;

            selectionInfo->Text =
                "Selected Seat: " + selectedSeat.ToString("00");

            confirmButton->Enabled = true;
        }

        // BOOK TICKET
        void BookSeat(Object^ sender, EventArgs^ e)
        {
            // GET PASSENGER DETAILS
            String^ passenger = passengerBox->Text->Trim();
            String^ nic = nicBox->Text->Trim();
            String^ phone = phoneBox->Text->Trim();

            // CHECK BUS AND SEAT
            if (busID < 1 || selectedSeat < 1)
            {
                MessageBox::Show("Choose a bus and seat.");
                return;
            }

            // CHECK PASSENGER NAME
            if (String::IsNullOrWhiteSpace(passenger))
            {
                MessageBox::Show("Enter passenger name.");
                return;
            }

            // CHECK NIC
            if (String::IsNullOrWhiteSpace(nic))
            {
                MessageBox::Show("Enter NIC Number.");
                return;
            }

            // CHECK PHONE NUMBER
            if (String::IsNullOrWhiteSpace(phone))
            {
                MessageBox::Show("Enter Phone Number.");
                return;
            }

            // VALIDATE NIC FORMAT
            if (!System::Text::RegularExpressions::Regex::IsMatch(
                nic, "^(?:[0-9]{9}[VvXx]|[0-9]{12})$"))
            {
                MessageBox::Show(
                    "Enter a valid NIC (9 digits + V/X or 12 digits).");
                return;
            }

            // VALIDATE PHONE NUMBER
            if (!System::Text::RegularExpressions::Regex::IsMatch(
                phone, "^0[0-9]{9}$"))
            {
                MessageBox::Show(
                    "Enter a valid 10-digit phone number.");
                return;
            }

            // OOP: OBJECT CREATION
            Passenger^ passengerObj = gcnew Passenger(
                passenger, nic, phone
            );

            // OOP: POLYMORPHISM
            Person^ personObj = passengerObj;

            // Calls the overridden Passenger GetDetails()
            String^ passengerDetails = personObj->GetDetails();

            try
            {
                MySqlConnection^ con = Database::GetConnection();
                con->Open();

                // SAVE BOOKING
                MySqlCommand^ cmd = gcnew MySqlCommand(
                    "INSERT INTO Bookings "
                    "(PassengerName, NIC, PhoneNumber, BusID, "
                    "SeatNumber, OriginalSeatNumber, BookingDate, "
                    "BookingStatus, TicketFare) "
                    "VALUES (@p, @nic, @phone, @b, @s, @s, "
                    "@d, 'Confirmed', @fare)", con);

                // USING OBJECT PROPERTIES
                cmd->Parameters->AddWithValue(
                    "@p", passengerObj->Name);

                cmd->Parameters->AddWithValue(
                    "@nic", passengerObj->NIC);

                cmd->Parameters->AddWithValue(
                    "@phone", passengerObj->Phone);

                cmd->Parameters->AddWithValue("@b", busID);
                cmd->Parameters->AddWithValue("@s", selectedSeat);
                cmd->Parameters->AddWithValue("@d", travelDate.Date);
                cmd->Parameters->AddWithValue("@fare", fare);

                cmd->ExecuteNonQuery();

                // GET BOOKING ID
                MySqlCommand^ idCmd = gcnew MySqlCommand(
                    "SELECT LAST_INSERT_ID()", con);

                int bookingId =
                    Convert::ToInt32(idCmd->ExecuteScalar());

                con->Close();

                // SHOW E-TICKET
                SeatBookingForm^ ticket = gcnew SeatBookingForm(
                    bookingId,
                    passengerObj->Name,
                    passengerObj->NIC,
                    passengerObj->Phone,
                    busNumber,
                    fromCity + " - " + toCity,
                    departure,
                    selectedSeat,
                    travelDate,
                    fare
                );

                ticket->ShowDialog();

                this->Close();
            }
            catch (MySqlException^ ex)
            {
                if (ex->Number == 1062)
                {
                    MessageBox::Show(
                        "This seat was just booked. Select another seat.");

                    LoadSeatMap();
                }
                else
                {
                    MessageBox::Show(
                        "Booking failed:\n" + ex->Message);
                }
            }
            catch (Exception^ ex)
            {
                MessageBox::Show(
                    "Booking failed:\n" + ex->Message);
            }
        }
    };
}
