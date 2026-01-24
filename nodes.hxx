#ifndef NODES_HXX
#define NODES_HXX

#include <types.hxx>
#include <package.hxx>
#include <storage_types.hxx>
#include <optional>

class IPackageReceiver {
    public:
        virtual ~IPackageReceiver() = default;
        virtual ElementID get_id() const = 0;
        virtual void receive_package(Package&& package) = 0;
        virtual IPackageStockpile::const_iterator cbegin() const = 0;
        virtual IPackageStockpile::const_iterator cend() const = 0;
};
// interfejs posiada:
// - wirtualny destruktor
// - metodę zwracającą id odbiorcy
// - metodę służącą do odbierania paczek
// - metody zwracające iteratory do składu paczek

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

class Storehause : public IPackageReceiver {
    public:
        Storehause(ElementID id, std::unique_ptr<IPackageStockpile> d) : id_(id), d_(std::move(d)) {}
        ElementID get_id() const override { return id_; }
        void receive_package(Package&& package) override {
            d_->push(std::move(package));
        }
        IPackageStockpile::const_iterator cbegin() const override { return d_->cbegin(); }
        IPackageStockpile::const_iterator cend() const override { return d_->cend(); }
    private:
        ElementID id_;
        std::unique_ptr<IPackageStockpile> d_;
};

// klasa posiada:
// - konstruktor inicjalizujący swoje pola
// - implementację metod interfejsu IPackageReceiver
// - id magazynu
// - unikalny wskaźnik do składu paczek
#endif