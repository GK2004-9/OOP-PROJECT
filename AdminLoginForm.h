
#pragma once

#include "AdminBusForm.h"
#include "AdminBookingForm.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace SmartBusSeatBookingSystem {

    public ref class AdminLoginForm : public Form
    {
    private:
        TextBox^ txtUsername;
        TextBox^ txtPassword;
        Panel^ dashboard;

        Color navy;
        Color teal;

    public:
        AdminLoginForm()
        {
            navy = Color::FromArgb(39, 29, 83);
            teal = Color::FromArgb(0, 150, 136);

            BuildUI();
        }

    private:
        Button^ MakeButton(String^ text, int x, int y,
            int w, Color color, EventHandler^ handler)
        {
            Button^ b = gcnew Button();
            b->Text = text;
            b->Location = Point(x, y);
            b->Size = Drawing::Size(w, 45);
            b->BackColor = color;
            b->ForeColor = Color::White;
            b->Font = gcnew Drawing::Font(
                "Segoe UI", 11, FontStyle::Bold);
            b->FlatStyle = FlatStyle::Flat;
            b->FlatAppearance->BorderSize = 0;
            b->Click += handler;
            return b;
        }

        void BuildUI()
        {
            this->Text = "SmartBus - Admin Login";
            this->ClientSize = Drawing::Size(460, 440);
            this->StartPosition = FormStartPosition::CenterParent;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->BackColor = Color::WhiteSmoke;

            Panel^ header = gcnew Panel();
            header->Dock = DockStyle::Top;
            header->Height = 95;
            header->BackColor = navy;
            this->Controls->Add(header);

            Label^ title = gcnew Label();
            title->Text = "ADMIN LOGIN";
            title->ForeColor = Color::White;
            title->Font = gcnew Drawing::Font(
                "Segoe UI", 22, FontStyle::Bold);
            title->Location = Point(95, 25);
            title->Size = Drawing::Size(300, 45);
            header->Controls->Add(title);

            Label^ userLabel = gcnew Label();
            userLabel->Text = "Username";
            userLabel->Location = Point(65, 125);
            userLabel->AutoSize = true;
            this->Controls->Add(userLabel);

            txtUsername = gcnew TextBox();
            txtUsername->Location = Point(65, 150);
            txtUsername->Size = Drawing::Size(330, 32);
            txtUsername->Font = gcnew Drawing::Font(
                "Segoe UI", 12);
            txtUsername->MaxLength = 50;
            this->Controls->Add(txtUsername);

            Label^ passLabel = gcnew Label();
            passLabel->Text = "Password";
            passLabel->Location = Point(65, 200);
            passLabel->AutoSize = true;
            this->Controls->Add(passLabel);

            txtPassword = gcnew TextBox();
            txtPassword->Location = Point(65, 225);
            txtPassword->Size = Drawing::Size(330, 32);
            txtPassword->Font = gcnew Drawing::Font(
                "Segoe UI", 12);
            txtPassword->UseSystemPasswordChar = true;
            txtPassword->MaxLength = 100;
            this->Controls->Add(txtPassword);

            this->Controls->Add(MakeButton(
                "LOGIN", 65, 290, 330, teal,
                gcnew EventHandler(
                    this, &AdminLoginForm::Login)));

            this->Controls->Add(MakeButton(
                "BACK", 65, 350, 330, Color::SlateGray,
                gcnew EventHandler(
                    this, &AdminLoginForm::GoBack)));

            this->AcceptButton = nullptr;

            // Dashboard is hidden until login succeeds.
            dashboard = gcnew Panel();
            dashboard->Dock = DockStyle::Fill;
            dashboard->BackColor = Color::WhiteSmoke;
            dashboard->Visible = false;
            this->Controls->Add(dashboard);
            dashboard->BringToFront();

            Label^ welcome = gcnew Label();
            welcome->Text = "ADMIN DASHBOARD";
            welcome->Font = gcnew Drawing::Font(
                "Segoe UI", 21, FontStyle::Bold);
            welcome->ForeColor = navy;
            welcome->Location = Point(75, 35);
            welcome->Size = Drawing::Size(350, 50);
            dashboard->Controls->Add(welcome);

            dashboard->Controls->Add(MakeButton(
                "MANAGE BUSES", 65, 125, 330, teal,
                gcnew EventHandler(
                    this, &AdminLoginForm::OpenBusAdmin)));

            dashboard->Controls->Add(MakeButton(
                "MANAGE BOOKINGS", 65, 195, 330, navy,
                gcnew EventHandler(
                    this, &AdminLoginForm::OpenBookingAdmin)));

            dashboard->Controls->Add(MakeButton(
                "LOGOUT", 65, 285, 330, Color::Firebrick,
                gcnew EventHandler(
                    this, &AdminLoginForm::Logout)));

            dashboard->Controls->Add(MakeButton(
                "BACK TO MAIN", 65, 350, 330,
                Color::SlateGray,
                gcnew EventHandler(
                    this, &AdminLoginForm::GoBack)));
        }

        void Login(Object^ sender, EventArgs^ e)
        {
            // Demo credentials only.
            // Not suitable for real production security.
            if (txtUsername->Text == "admin" &&
                txtPassword->Text == "1234")
            {
                dashboard->Visible = true;
                dashboard->BringToFront();
                txtPassword->Clear();
            }
            else
            {
                MessageBox::Show(
                    "Invalid username or password.",
                    "Login Failed",
                    MessageBoxButtons::OK,
                    MessageBoxIcon::Warning);
                txtPassword->Clear();
            }
        }

        void OpenBusAdmin(Object^ sender, EventArgs^ e)
        {
            AdminBusForm^ form = gcnew AdminBusForm();
            form->ShowDialog(this);
        }

        void OpenBookingAdmin(Object^ sender, EventArgs^ e)
        {
            AdminBookingForm^ form =
                gcnew AdminBookingForm();
            form->ShowDialog(this);
        }

        void Logout(Object^ sender, EventArgs^ e)
        {
            dashboard->Visible = false;
            txtUsername->Clear();
            txtPassword->Clear();
        }

        void GoBack(Object^ sender, EventArgs^ e)
        {
            this->Close();
        }
    };
}