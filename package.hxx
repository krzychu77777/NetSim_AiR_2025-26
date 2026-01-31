#ifndef PACKAGE_HXX
#define PACKAGE_HXX

#include "types.hxx"
#include <set>
#include <algorithm>

class Package {
    public:
        Package() { this->id_ = acquire_id(); }
        Package(ElementID id_);
        Package(const Package& other) = delete;
        Package& operator=(const Package& other) = delete;

        Package(Package&& other) noexcept;
        Package& operator=(Package&& other) noexcept;

        ElementID get_id() const { return id_;}
        ~Package();

    private:
        ElementID id_;
        static std::set<ElementID> assigned_IDs;
        static std::set<ElementID> freed_IDs;

        static ElementID acquire_id();
        static void release_id(ElementID id_);
};

// klasa posiada:
// - konstruktor domyślny
// - konstruktor z parametrem
// - konstruktor kopiujący, który został usunięty
// - operator przypisania kopiującego, też usunięty
// - konstruktor przenoszący, który przyjmuje referencję na r-wartość
// - operator przypisania przenoszącego
// - getter do pozyskania id
// - destruktor, który zwalnia id
// 
// - zbiór przypisanych id
// - zbiór zwolnionych id
// - metoda statyczna do przypisania nowego id
// - metoda statyczna do zwalniania id

#endif