#include "main.h"
using namespace System;
using namespace System::Windows::Forms;
[STAThreadAttribute]
int main(array<String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    SmartBusSeatBookingSystem::MainForm^ form =
        gcnew SmartBusSeatBookingSystem::MainForm();
    Application::Run(form);
    return 0;
}
