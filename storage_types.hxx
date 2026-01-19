#include "package.hxx"
#include <list>

enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {

};

class PackageQueue{
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
        PackageQueueType get_queue_type() { return type; }
        size_t size() { return data.size(); }
        bool empty() { return data.empty(); }

        std::list<Package>::const_iterator cbegin() const { return data.cbegin(); }
        std::list<Package>::const_iterator cend() const { return data.cend(); }
        std::list<Package>::iterator begin() { return data.begin(); }
        std::list<Package>::iterator end() { return data.end(); }


    private:
        PackageQueueType type;
        std::list<Package> data = {};
};