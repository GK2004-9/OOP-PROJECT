
#pragma once

using namespace System;

namespace SmartBusSeatBookingSystem
{
    public ref class Bus
    {
    public:
        property int BusID;
        property String^ BusNumber;
        property String^ Route;
        property String^ DepartureTime;
        property int TotalSeats;

        Bus()
        {
            BusID = 0;
            BusNumber = "";
            Route = "";
            DepartureTime = "";
            TotalSeats = 40;
        }
    };

    public ref class Booking
    {
    public:
        property int BookingID;
        property String^ PassengerName;
        property int BusID;
        property int SeatNumber;
        property String^ BookingDate;
    };
}
