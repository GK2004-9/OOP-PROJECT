
#pragma once

using namespace System;

namespace SmartBusSeatBookingSystem
{
    // BASE CLASS
    public ref class Person
    {
    private:
        String^ name;

    public:
        Person(String^ n)
        {
            name = n;
        }

        property String^ Name
        {
            String^ get()
            {
                return name;
            }
        }

        virtual String^ GetDetails()
        {
            return "Name: " + name;
        }
    };

    // DERIVED CLASS
    public ref class Passenger : public Person
    {
    private:
        String^ nic;
        String^ phone;

    public:
        Passenger(String^ n, String^ id, String^ p)
            : Person(n)
        {
            nic = id;
            phone = p;
        }

        property String^ NIC
        {
            String^ get()
            {
                return nic;
            }
        }

        property String^ Phone
        {
            String^ get()
            {
                return phone;
            }
        }

        virtual String^ GetDetails() override
        {
            return "Passenger: " + Name +
                "\nNIC: " + nic +
                "\nPhone: " + phone;
        }
    };
}
