#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

#include "package.hpp"
#include <list>

enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
    public:
        virtual ~IPackageStockpile() = default;
        virtual size_t size() const = 0;
        virtual bool empty() const = 0;
        virtual void push(Package&& package) = 0;

        // robię alias na ten śmieszny iterator, żeby się nie męczyć ~Krzychu
        using const_iterator = std::list<Package>::const_iterator;
        virtual const_iterator cbegin() const = 0;
        virtual const_iterator cend() const = 0;

};

class IPackageQueue : public IPackageStockpile {
    public:
        virtual Package pop() = 0;
        virtual PackageQueueType get_queue_type() const = 0;
};

class PackageQueue : public IPackageQueue {

    public:
        PackageQueue(PackageQueueType type_given) {this->type = type_given;}

    // Dla potomnych:
    //  - konstruktor inicjuje instację jak mu podać czy ma być kolejką FIFO czy LIFO
    //  - pakiety Package są przechowywane w liście pwywatnej na samym dole 
    //  - push() PRZENOSI (nie kopiuje) pakiet do listy
    //  - pop() usuwa pakiet z listy i go ZWRACA (interfejsowi) (przenosząc, nie kopiując)
    //  - a reszta to już wiadomo, na dole części publicznej macie iteratory
    //    bez możliwości i z możliwością modyfikacji zawartości kolejki 
    //                                              ~ Krzychu 19.01 A.D.2026

        void push(Package&& package) {data.push_back(std::move(package)); }
        Package pop();
        PackageQueueType get_queue_type() const override { return type; }
        size_t size() const override { return data.size(); }
        bool empty() const override { return data.empty(); }

        std::list<Package>::const_iterator cbegin() const override { return data.cbegin(); }
        std::list<Package>::const_iterator cend() const override { return data.cend(); }


    private:
        PackageQueueType type;
        std::list<Package> data = {};
};

#endif
