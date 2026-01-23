#ifndef NODES_HXX
#define NODES_HXX

#include <types.hxx>
#include <package.hxx>
#include <storage_types.hxx>
#include <optional>

class Ramp : public PackageSender {
    public:
        Ramp(ElementID id, TimeOffset di) : PackageSender(), id_(id), di_(di) {}
        void deliver_goods(Time t);
        TimeOffset get_delivery_interval() { return di_; }
        ElementID get_id() { return id_; }

    private:
        ElementID id_;
        TimeOffset di_;
        Time t_; // czy to jest potrzebne?
};

// klasa posiada:
// - konstruktor inicjalizujący bazę (PackageSender) oraz swoje pola
// - metoda służąca do dostarczania półproduktów (wywoływana w każdej turze)
// - getter do pobrania przedziału czasowego
// - getter do pobrania id półproduktu
// 
// - id półproduktu
// - przedział czasowy
// - czas 

class PackageSender : public ReceiverPreferences {
    public:
        ReceiverPreferences receiver_preferences;
        PackageSender() = default;
        PackageSender(PackageSender&&) = default ;
        void send_package();
        std::optional<Package>& get_sending_buffer() { return buffer_; }
    
    protected:
        void push_package(Package&& package) { buffer_.emplace(std::move(package)); }

    private:
        std::optional<Package> buffer_;
};

// klasa posiada:
// - obiekt preferencji odbiorcy
// - konstruktor domyślny
// - konstruktor przenoszący
// - metoda służąca do wysyłania paczek do odbiorcy
// - getter do pobrania zawartości bufora (referencja do optional pozwala modyfikować bufor z zewnątrz)
//
// - metoda umożliwiająca dziedziczącym klasom wkładanie paczek
// 
// - bufor

#endif