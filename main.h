
#pragma once
#include "AdminLoginForm.h"
#include "AdminBookingForm.h"
#include "AdminBusForm.h"
#include "Database.h"
#include "BusDetailsForm.h"
#include "CancelBookingForm.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace MySql::Data::MySqlClient;

namespace SmartBusSeatBookingSystem {

    public ref class MainForm : public Form
    {
    private:
        ComboBox^ fromBox;
        ComboBox^ toBox;
        DateTimePicker^ dateBox;

        Color navy;
        Color teal;
        Color amber;

    public:
        MainForm()
        {
            navy = Color::FromArgb(39, 29, 83);
            teal = Color::FromArgb(0, 150, 136);
            amber = Color::FromArgb(245, 158, 11);

            BuildUI();
            LoadCities();
        }

    private:

        // CREATE BUTTON
        Button^ MakeButton(
            String^ text,
            int x, int y,
            int width, int height,
            Color color,
            EventHandler^ handler)
        {
            Button^ button = gcnew Button();

            button->Text = text;
            button->Location = Point(x, y);
            button->Size = Drawing::Size(width, height);
            button->BackColor = color;
            button->ForeColor = Color::White;

            button->Font = gcnew Drawing::Font(
                "Segoe UI", 11, FontStyle::Bold);

            button->FlatStyle = FlatStyle::Flat;
            button->FlatAppearance->BorderSize = 0;
            button->Click += handler;

            return button;
        }
        void OpenAdminLogin(Object^ sender, EventArgs^ e)
        {
            AdminLoginForm^ login = gcnew AdminLoginForm();
            login->ShowDialog(this);
            LoadCities();
        }

        // MAIN SCREEN DESIGN
        void BuildUI()
        {
            this->Text = "SmartBus | Online Reservation";
            this->ClientSize = Drawing::Size(900, 760);
            this->StartPosition = FormStartPosition::CenterScreen;
            this->BackColor = Color::FromArgb(244, 247, 251);

            this->FormBorderStyle =
                System::Windows::Forms::FormBorderStyle::FixedSingle;

            this->MaximizeBox = false;

            // HEADER
            Panel^ header = gcnew Panel();
            header->Dock = DockStyle::Top;
            header->Height = 145;
            header->BackColor = navy;
            this->Controls->Add(header);

            // LOGO
            Label^ logo = gcnew Label();
            logo->Text = "SMARTBUS";
            logo->Font = gcnew Drawing::Font(
                "Segoe UI", 31, FontStyle::Bold);

            logo->ForeColor = Color::White;
            logo->Location = Point(35, 20);
            logo->Size = Drawing::Size(540, 60);
            header->Controls->Add(logo);

            // SLOGAN
            Label^ slogan = gcnew Label();
            slogan->Text = "Travel Smart. Book Easy.";
            slogan->Font = gcnew Drawing::Font(
                "Segoe UI", 13);

            slogan->ForeColor = Color::LightCyan;
            slogan->Location = Point(40, 85);
            slogan->Size = Drawing::Size(450, 30);
            header->Controls->Add(slogan);

            Button^ adminLoginButton = MakeButton(
                "ADMIN LOGIN",
                660, 48, 205, 45,
                amber,
                gcnew EventHandler (this, &MainForm::OpenAdminLogin)
               );

            header->Controls->Add(adminLoginButton);

            // MAIN CARD
            Panel^ card = gcnew Panel();
            card->BackColor = Color::White;
            card->Location = Point(140, 180);
            card->Size = Drawing::Size(620, 435);
            this->Controls->Add(card);

            // TITLE
            Label^ title = gcnew Label();
            title->Text = "ONLINE SEAT RESERVATION";
            title->Font = gcnew Drawing::Font("Segoe UI", 19, FontStyle::Bold);

            title->ForeColor = navy;
            title->Location = Point(40, 24);
            title->Size = Drawing::Size(545, 45);
            card->Controls->Add(title);

            // FROM CITY LABEL
            Label^ fromLabel = gcnew Label();
            fromLabel->Text = "FROM CITY";
            fromLabel->Location = Point(45, 87);
            fromLabel->AutoSize = true;
            card->Controls->Add(fromLabel);

            // FROM CITY COMBOBOX
            fromBox = gcnew ComboBox();
            fromBox->Location = Point(45, 111);
            fromBox->Size = Drawing::Size(530, 36);
            fromBox->Font = gcnew Drawing::Font("Segoe UI", 12);

            fromBox->DropDownStyle =
                ComboBoxStyle::DropDownList;

            card->Controls->Add(fromBox);

            // TO CITY LABEL
            Label^ toLabel = gcnew Label();
            toLabel->Text = "TO CITY";
            toLabel->Location = Point(45, 166);
            toLabel->AutoSize = true;
            card->Controls->Add(toLabel);

            // TO CITY COMBOBOX
            toBox = gcnew ComboBox();
            toBox->Location = Point(45, 190);
            toBox->Size = Drawing::Size(530, 36);
            toBox->Font = gcnew Drawing::Font(
                "Segoe UI", 12);

            toBox->DropDownStyle =
                ComboBoxStyle::DropDownList;

            card->Controls->Add(toBox);

            // TRAVEL DATE LABEL
            Label^ dateLabel = gcnew Label();
            dateLabel->Text = "TRAVEL DATE";
            dateLabel->Location = Point(45, 245);
            dateLabel->AutoSize = true;
            card->Controls->Add(dateLabel);

            // DATE PICKER
            dateBox = gcnew DateTimePicker();
            dateBox->Location = Point(45, 270);
            dateBox->Size = Drawing::Size(530, 35);
            dateBox->Font = gcnew Drawing::Font("Segoe UI", 12);

            dateBox->Format = DateTimePickerFormat::Long;
            dateBox->MinDate = DateTime::Today;

            card->Controls->Add(dateBox);

            // SEARCH BUSES BUTTON
            Button^ searchButton = MakeButton("SEARCH BUSES",
                45, 341, 530, 53,
                teal,
                gcnew EventHandler(this, &MainForm::SearchBuses)
            );

            card->Controls->Add(searchButton);

            // FIND / CANCEL BOOKING BUTTON
            Button^ cancelButton = MakeButton(
                "FIND / CANCEL BOOKING",
                275, 645, 350, 50,
                amber,
                gcnew EventHandler(this, &MainForm::OpenCancellation)
            );

            this->Controls->Add(cancelButton);
        }

        // LOAD CITIES FROM MYSQL
        void LoadCities()
        {
            fromBox->Items->Clear();
            toBox->Items->Clear();

            try
            {
                MySqlConnection^ con =
                    Database::GetConnection();

                con->Open();

                MySqlCommand^ cmd = gcnew MySqlCommand(
                    "SELECT FromCity AS City FROM Buses "
                    "UNION "
                    "SELECT ToCity AS City FROM Buses "
                    "ORDER BY City", con);

                MySqlDataReader^ r = cmd->ExecuteReader();

                int count = 0;

                while (r->Read())
                {
                    if (!r->IsDBNull(0))
                    {
                        String^ city =
                            Convert::ToString(r["City"])->Trim();

                        if (!String::IsNullOrWhiteSpace(city))
                        {
                            fromBox->Items->Add(city);
                            toBox->Items->Add(city);
                            count++;
                        }
                    }
                }

                r->Close();
                con->Close();

                if (count > 0)
                {
                    fromBox->SelectedIndex = 0;
                    toBox->SelectedIndex = 0;
                }
                else
                {
                    MessageBox::Show("No cities found in the database.");
                }
            }
            catch (Exception^ ex)
            {
                MessageBox::Show(
                    "City Loading Error:\n" + ex->Message);
            }
        }

        // OPEN ADMIN BUS MANAGEMENT
        void OpenAdmin(Object^ sender, EventArgs^ e)
        {
            AdminBusForm^ admin =
                gcnew AdminBusForm();

            admin->ShowDialog(this);

            // Refresh cities after admin changes
            LoadCities();
        }

        // OPEN ADMIN BOOKING MANAGEMENT
        void OpenBookingAdmin(Object^ sender, EventArgs^ e)
        {
            AdminBookingForm^ bookingAdmin =
                gcnew AdminBookingForm();

            bookingAdmin->ShowDialog(this);
        }

        // SEARCH BUSES
        void SearchBuses(Object^ sender, EventArgs^ e)
        {
            if (fromBox->SelectedIndex < 0 ||
                toBox->SelectedIndex < 0)
            {
                MessageBox::Show("Choose both From and To cities.");
                return;
            }

            String^ from =
                Convert::ToString(fromBox->SelectedItem);

            String^ to =
                Convert::ToString(toBox->SelectedItem);

            if (String::Equals(
                from, to,
                StringComparison::OrdinalIgnoreCase))
            {
                MessageBox::Show("From and To must be different.");
                return;
            }

            BusDetailsForm^ results =
                gcnew BusDetailsForm(
                    from,
                    to,
                    dateBox->Value.Date
                );

            results->ShowDialog(this);
        }

        // OPEN CANCEL BOOKING SCREEN
        void OpenCancellation(Object^ sender, EventArgs^ e)
        {
            CancelBookingForm^ popup =
                gcnew CancelBookingForm();

            popup->ShowDialog(this);
        }
    };
}
